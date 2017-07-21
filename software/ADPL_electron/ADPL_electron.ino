/* ADPL_complete.ino
 * Master MCU code for all components:
 * + temp probes
 * + relays
 * + bucket tip
 *
 * LICENSE: Apache v2.0 (see LICENSE file)
 *
 * Copyright (c) 2015-2016 Mark Palmeri (Duke University)
 */

SYSTEM_THREAD(ENABLED);  // parallel user and system threads
                         // to allow function w/o cell connectivity

unsigned long SYS_VERSION;

#define PUBLISH_DELAY 150000  // 2.5 min b/w variable publish

#include "pin_mapping.h"

#include "TempProbe.h"
TempProbe tempHXCI("HXCI", HXCI);
TempProbe tempHXCO("HXCO", HXCO);
TempProbe tempHTR("HTR", HTR);
TempProbe tempHXHI("HXHI", HXHI);
TempProbe tempHXHO("HXHO", HXHO);

#include "Valve.h"
Valve valve(VALVE);
#include "Ignitor.h"
Ignitor ignitor(IGNITOR);
#define INCINERATE_LOW_TEMP 68  // will be 68 in field
#define INCINERATE_HIGH_TEMP 72 // will be 72 in field
#define IGNITE_DELAY 900000     // 15 min between ignitor fires with open valve

#include "Pump.h"
Pump pump(PUMP);
#define KEEP_PUMP_ON_TIME 10000     // 10s on every 30min
#define KEEP_PUMP_OFF_TIME 1790000   // 30min-10s off time (29 min 50s)

#include "Bucket.h"
#define VOLUME 250.0 //300 mL, varies by location
#define OPTIMAL_FLOW 5.0 //5.0 L/hr, varies by location
Bucket bucket(BUCKET, VOLUME, OPTIMAL_FLOW);

#include "PinchValve.h"
PinchValve pinchValve(DIR, STEP, SLEEP, UP, DOWN, RESET);
#define FEEDBACK_RESOLUTION 0.125 // mm of movement 16/turn
#define PUSH_BUTTON_RESOLUTION 1.0 // mm of movement
#define HALF_RESOLUTION 0.5 // mm of movement
#define UNCLOG_RESOLUTION 4.0 // mm of movment
#define MAX_POSITION 5.0 // in mm
#define MIN_POSITION 0.0 // in mm

// initialize some time counters
unsigned long currentTime = 0;
unsigned long last_publish_time = 0;
int temp_count = 1;
int write_address = 0;

//initialize the loghandler
SerialLogHandler logHandler;

void setup() {
    Log.info("Starting setup...");
    Serial.begin(9600);
    pinchValve.position = EEPROM.get(write_address, pinchValve.position);
    Particle.variable("currentTime", currentTime);
    // count bucket tips on one-shot rise
    attachInterrupt(BUCKET, bucket_tipped, RISING);
    // collect the system firmware version to fetch OTA
    SYS_VERSION = System.versionNumber();
    Particle.variable("SYS_VERSION", SYS_VERSION);
    // sense buttons
    attachInterrupt(UP, up_pushed, FALLING);
    attachInterrupt(DOWN, down_pushed, FALLING);
    attachInterrupt(RESET, res_pushed, FALLING);
    Log.info("Setup complete! System is running version %s", (const char*)SYS_VERSION);
}

void loop() {
    Log.trace("Starting loop...");
    // read the push buttons
    currentTime = millis();

    // rotate through temp probes, only reading 1 / loop since it takes 1 s / read
    temp_count = read_temp(temp_count);
    if ((currentTime - last_publish_time) > PUBLISH_DELAY) {
        Log.info("Publishing data...");
        last_publish_time = publish_data(last_publish_time);
        Log.info("Publish complete!");
    }

    // measure temp, determine if light gas
    if (tempHTR.temp <= INCINERATE_LOW_TEMP && !valve.gasOn) {
        Log.info("Temperature is too low. Igniting gas...");
        Log.trace("Opening valve...");
        valve.open();
        Log.trace("Valve opened.");
        delay(100);
        Log.trace("Firing ignitor...");
        ignitor.fire();
        Log.info("Ignition complete.");
    }

    if(valve.gasOn) {
        currentTime = millis();
        if (tempHTR.temp >= INCINERATE_HIGH_TEMP) {
            Log.info("Temperature is too high. Closing gas valve...");
            valve.close();
            Log.info("Valve closed.");
        }
        // if 15 min have elapsed since last ignitor fire, then fire again
        else if((currentTime - ignitor.timeLastFired) > IGNITE_DELAY) {
            Log.info("Designated amount of time has passed since last fire. Igniting...");
            ignitor.fire();
            Log.info("Ignition complete.");
        }
    }

    currentTime = millis();
    if (!pump.pumping && (currentTime - pump.offTime) > KEEP_PUMP_OFF_TIME) {
        Log.info("Pump has been OFF for the designated amount of time. Turning pump on...");
        pump.turnOn();
        Log.info("Pump turned on.");
    }
    else if (pump.pumping) {
        if ((currentTime - pump.onTime) > KEEP_PUMP_ON_TIME) {
            Log.info("Pump has been ON for the designated amount of time. Turning pump off...");
            pump.turnOff();
            Log.info("Pump turned off.");
        }
    }

    // flag variables changed in attachInterrupt function
    if(pinchValve.down) {
        Log.info("Shifting pinch valve down...");
        pinchValve.shiftDown(pinchValve.resolution);
        EEPROM.put(write_address, pinchValve.position);
        Log.info("Pinch valve shifted down.");
    }
    if(pinchValve.up) {
        Log.info("Shifting pinch valve up...");
        pinchValve.shiftUp(pinchValve.resolution);
        EEPROM.put(write_address, pinchValve.position);
        Log.info("Pinch valve shifted up.");
    }

    // unclog if no tip in a long while
    // open all the way up and come back to optimum
    currentTime = millis();
    if ((currentTime - bucket.lastTime) > (2 * bucket.lowFlow)) {
        Log.info("No tip for awhile. Unclogging...");
        pinchValve.unclog(UNCLOG_RESOLUTION);
        bucket.lastTime = currentTime;
        Log.info("Unclogging complete.");

        if(pinchValve.clogCounting >= 2 && pinchValve.position < MAX_POSITION) {
            Log.warn("Many unclogging attempts made.");
            Log.warn("Attempting to unclog - moving pinch valve up...");
            pinchValve.up = true;
            pinchValve.resolution = PUSH_BUTTON_RESOLUTION;
            Log.warn("Pinch valve moved.");
        }

    }

    if(bucket.tip) {
        Log.info("Bucket tipped.");
        pinchValve.clogCounting = 0;
        bucket.updateFlow();
        if (bucket.tipTime < bucket.highFlow && bucket.tipTime > bucket.highestFlow && pinchValve.position > MIN_POSITION) {
            Log.info("Moving pinch valve down...");
            pinchValve.down = true;
            pinchValve.resolution = FEEDBACK_RESOLUTION;
            Log.info("Pinch valve moved.");
        }
        else if (bucket.tipTime > bucket.lowFlow && pinchValve.position < MAX_POSITION){
            Log.info("Moving pinch valve up...");
            pinchValve.up = true;
            pinchValve.resolution = FEEDBACK_RESOLUTION;
            Log.info("Pinch valve moved.");
        }
        else if (bucket.tipTime < bucket.highestFlow){
            Log.warn("Sudden large flow detected. Handling...");
            pinchValve.down = true; // handles sudden large flow
            pinchValve.resolution = HALF_RESOLUTION;
            Log.warn("Large flow handled.");
        }
    }

}

int read_temp(int temp_count) {
    Log.trace("Reading temperatures...");
    switch (temp_count) {
        case 1:
            Log.trace("Reading temp: heat exchanger cold inlet...");
            tempHXCI.read();
            temp_count++;
            Log.trace("Reading complete.");
            break;
        case 2:
            Log.trace("Reading temp: heat exchanger cold outlet...");
            tempHXCO.read();
            temp_count++;
            Log.trace("Reading complete");
            break;
        case 3:
            Log.trace("Reading temp: heater...");
            tempHTR.read();
            temp_count++;
            Log.trace("Reading complete.");
            break;
        case 4:
            Log.trace("Reading temp: heat exchanger hot inlet...");
            tempHXHI.read();
            temp_count++;
            Log.trace("Reading complete.");
            break;
        case 5:
            Log.trace("Reading temp: heat exchanger hot outlet...");
            tempHXHO.read();
            temp_count = 1;
            Log.trace("Reading complete.");
            break;
    }
    Log.trace("Reading complete.");
    return temp_count;
}

void bucket_tipped() {
    bucket.tipped();
    bucket.tip = true;
}

int publish_data(int last_publish_time) {
    Log.info("Publishing data...");
    bool publish_success;
    char data_str [69];

    // allow for data str to be created that doesn't update bucket if count = 0
    const char* fmt_string = "HXCI:%.1f,HXCO:%.1f,HTR:%.1f,HXHI:%.1f,HXHO:%.1f,V:%d,B:%d";
    const char* fmt_string_no_bucket = "HXCI:%.1f,HXCO:%.1f,HTR:%.1f,HXHI:%.1f,HXHO:%.1f,V:%d";

    // bucket.tip_count will be ignored if not needed by sprintf
    Log.trace("Formatting data string...");
    sprintf(data_str, (bucket.tip_count > 0) ? fmt_string : fmt_string_no_bucket,
            tempHXCI.temp, tempHXCO.temp, tempHTR.temp, tempHXHI.temp, tempHXHO.temp,
            int(valve.gasOn), int(bucket.tip_count));
    Log.trace("String formatted.");

    publish_success = Particle.publish("DATA",data_str);

    if (publish_success) {
        last_publish_time = currentTime;
        // reset the bucket tip count after every successful publish
        // (webserver will accumulate count)
        bucket.tip_count = 0;
        Log.info("Publishing complete.");
    } else {
        Log.error("Publishing failed.");
    }
    return last_publish_time;
}

void res_pushed() {
    Log.info("Moving pinch valve...");
    pinchValve.position = 0.0;
    pinchValve.up = true;
    pinchValve.resolution = PUSH_BUTTON_RESOLUTION;
    bucket.lastTime = millis();
    Log.info("Pinch valve moved.");
}

void up_pushed() {
    Log.info("Moving pinch valve up...");
    pinchValve.up = true;
    pinchValve.resolution = PUSH_BUTTON_RESOLUTION;
    Log.info("Pinch valve moved.");
}

void down_pushed() {
    Log.info("Moving pinch valve down...");
    pinchValve.down = true;
    pinchValve.resolution = PUSH_BUTTON_RESOLUTION;
    Log.info("Pinch valve moved.");
}

void logError(int errorCode) {
    Log.error("Returned error code: " + errorCode);
    Particle.publish("ERROR", errorCode);
}
