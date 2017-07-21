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
#define PUSH_BUTTON_RESOLUTION 1.0 // mm of movement
#define BATCH_MOVEMENT 3.0 // in mm

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

    //initialize pinch valve time attribute
    pinchValve.lastTime = 0;
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
    if(pinchValve.up) {
        // if the pinch valve var is set to up, move it up
        Log.info("Shifting pinch valve up...");
        //pinchValve.shiftUp(pinchValve.resolution);
        EEPROM.put(write_address, pinchValve.position);
        Log.info("Pinch valve shifted up.");
    }
    else {
        // if the pinch valve var is set to down, move it down
        Log.info("Shifting pinch valve down...");
        //pinchValve.shiftDown(pinchValve.resolution);
        EEPROM.put(write_address, pinchValve.position);
        Log.info("Pinch valve shifted down.");
    }

    currentTime = millis();
    if ((currentTime - pinchValve.lastTime) > ((3600 * VOLUME) / OPTIMAL_FLOW) && (!pinchValve.up)) {
        // if the difference between current and wait times > the volume of the bucket ((L*s)/h) *
        // the inverse of the optimal flow (hrs/L) (units cancel, leaving a number in seconds) AND
        // the pinch valve is down
        Log.info("Raising pinch valve...");
        // raise the pinch valve
        pinchValve.up = true;
        pinchValve.resolution = BATCH_MOVEMENT;
        pinchValve.lastTime = currentTime;
        Log.info("Pinch valve raised");
    }

    // handle bucket tip
    if (bucket.tip) {
        Log.info("Bucket has tipped.");
        // if bucket is tipped
        if(pinchValve.up){
            Log.info("Pinch valve is up. Lowering...");
            // if the pinch valve is up, lower it
            pinchValve.up = false;
            pinchValve.resolution = BATCH_MOVEMENT;
            Log.info("Pinch valve lowered.");
        }
        // tip handled; set tip bool to false
        bucket.tip = false;
        pinchValve.lastTime = millis();
        Log.info("Pinch valve is already down. No action needed.");
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

void res_pushed(){
    Log.info("Reset button pushed.");
    pinchValve.position = 0.0;
    pinchValve.lastTime = millis();
    pinchValve.up = false;
}

void up_pushed() {
    Log.info("Up button pushed.");
    pinchValve.resolution = PUSH_BUTTON_RESOLUTION;
    pinchValve.shiftUp(pinchValve.resolution);
}

void down_pushed() {
    Log.info("Down button pushed.");
    pinchValve.resolution = PUSH_BUTTON_RESOLUTION;
    pinchValve.shiftDown(pinchValve.resolution);
}
