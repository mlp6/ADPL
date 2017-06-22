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
#define SDCARD true  // true/false depending on writing data to SD card


#if SDCARD
#include "SD/SdFat.h"
// Primary SPI with DMA
// SCK => A3, MISO => A4, MOSI => A5, SS => A2 (default)
SdFat sd;
const uint8_t chipSelect = SS;
File myFile;
#endif

#include "PublishDataCell.h"
PublishDataCell cellPublisher;

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

//TEST VAR
bool sDSuccess = false;

//initialize the loghandler
SerialLogHandler logHandler;

void setup() {
    Serial.begin(9600);
    // Wait for USB Serial
    while (!Serial) {
        SysCall::yield();
    }

    Serial.println("Type any character to start");
    while (Serial.read() <= 0) {
        SysCall::yield();
    }

    // Initialize SdFat or print a detailed error message and halt
    // Use half speed like the native library.
    // Change to SPI_FULL_SPEED for more performance.
    if (!sd.begin(chipSelect, SPI_HALF_SPEED)) {
        sd.initErrorHalt();
    }
}

void loop() {

    // read the push buttons
    currentTime = millis();
    // rotate through temp probes, only reading 1 / loop since it takes 1 s / read
    temp_count = read_temp(temp_count);
    if ((currentTime - last_publish_time) > PUBLISH_DELAY) {
        bool publishedCell = false;
        bool publishedSD = false;

        Log.info("Testing whether particle is connected...");
        if (Particle.connected()) { //Returns true if the device is connected to the cellular network
            Log.info("Particle is connected. Publishing over cell...");
            if (cellPublisher.publish(tempHXCI.temp, tempHXCO.temp, tempHTR.temp, tempHXHI.temp,
                                      tempHXHO.temp, int(valve.gasOn), int(bucket.tip_count))) {
                Log.info("Cell publish successful.");
                publishedCell = true;
            } else {
                Log.error("Cell publish failed.");
                publishedCell = false;
            }

        } else {
            Log.warn("Particle is not connected.");
        }
        if (publishedCell || publishedSD) {
            Log.info("At least one publishing method was successful. Adjusting variables accordingly...");
            // reset the bucket tip count after every successful publish
            // (webserver will accumulate count)
            last_publish_time = millis();
            bucket.tip_count = 0;
            Log.info("Variables adjusted.");
        }
    }
    //******************************SD CARD****************************************
    // open the file for write at end like the "Native SD library"
    if (!myFile.open("test.txt", O_RDWR | O_CREAT | O_AT_END)) {
        sd.errorHalt("opening test.txt for write failed");
    }
    // if the file opened okay, write to it:

    Serial.print("Writing to test.txt...");
    publishSD();

    // close the file:
    myFile.close();
    Serial.println("done.");
    delay(1000);
    //******************************SD CARD****************************************

    // measure temp, determine if light gas
    if (tempHTR.temp <= INCINERATE_LOW_TEMP && !valve.gasOn) {
        Log.info("Temperature is too low. Igniting gas...");
        valve.open();
        delay(100);
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

        if(pinchValve.clogCounting >= 2 && pinchValve.position < MAX_POSITION){
            Log.warn("%s", pinchValve.clogCounting, " unclogging attempts made.");
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

void publishSD(){
    //TEMPORARY FOR TESTING
    int gasOn = 1;
    int bucket_tip_count = 2;

    char data_str[69];
    char* fmt_string_SD = "time:%d,HXCI:%.1f,HXCO:%.1f,HTR:%.1f,HXHI:%.1f,HXHO:%.1f,V:%d,B:%d";
    char* fmt_string_no_bucket_SD = "time:%d,HXCI:%.1f,HXCO:%.1f,HTR:%.1f,HXHI:%.1f,HXHO:%.1f,V:%d";

    sprintf(data_str, (bucket_tip_count > 0) ? fmt_string_SD : fmt_string_no_bucket_SD,
            millis(), tempHXCI.temp, tempHXCO.temp, tempHTR.temp, tempHXHI.temp, tempHXHO.temp,
            gasOn, bucket_tip_count);

    myFile.println(data_str);
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

void res_pushed(){
    Log.info("Moving pinch valve...");
    pinchValve.position = 0.0;
    pinchValve.up = true;
    pinchValve.resolution = PUSH_BUTTON_RESOLUTION;
    bucket.lastTime = millis();
    Log.info("Pinch valve moved.");
}

void up_pushed(){
    Log.info("Moving pinch valve up...");
    pinchValve.up = true;
    pinchValve.resolution = PUSH_BUTTON_RESOLUTION;
    Log.info("Pinch valve moved.");
}

void down_pushed(){
    Log.info("Moving pinch valve down...");
    pinchValve.down = true;
    pinchValve.resolution = PUSH_BUTTON_RESOLUTION;
    Log.info("Pinch valve moved.");
}
