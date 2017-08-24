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

#include "PublishDataCell.h"
PublishDataCell cellPublisher;

#define PUBLISH_DELAY 150000  // 2.5 min b/w variable publish

#include "SD/SdFat.h"
#include "PublishDataSD.h"
#include "pin_mapping.h"
// Software SPI.  Use any digital pins.
// MISO => B1, MOSI => B2, SCK => B0, CS => B4
SdFatSoftSpi<SD_DO_PIN, SD_DI_PIN, SD_CLK_PIN> sd;
File sdFile;
PublishDataSD sdPublisher;

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

#include "error_codes.h"

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


// Determine existence of SD card via the CD pin
bool SDCARD = (bool)digitalRead(SD_CD_PIN);

void setup() {
  
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

    // Initialize SdFat or print a detailed error message and halt
    // Use half speed like the native library.
    // Change to SPI_FULL_SPEED for more performance.
    if(SDCARD){
        if (!sd.begin(SD_CS_PIN, SPI_HALF_SPEED)) {
            logError(SD_INIT_FAIL);
        } 
    } 
}

void loop() {
    // read the push buttons
    currentTime = millis();
    // rotate through temp probes, only reading 1 / loop since it takes 1 s / read
    temp_count = read_temp(temp_count);
    if ((currentTime - last_publish_time) > PUBLISH_DELAY) {
        bool publishedCell;
        bool publishedSD;

        if (Particle.connected()) { //Returns true if the device is connected to the cellular network
            if (cellPublisher.publish(tempHXCI.temp, tempHXCO.temp, tempHTR.temp, tempHXHI.temp,
                                      tempHXHO.temp, int(valve.gasOn), int(bucket.tip_count))) {
                publishedCell = true;
            } else {
                logError(CELL_PUB_FAIL);
                publishedCell = false;
            }

            if (SDCARD && sdFile.open("adpl_data.txt", O_READ) && sdFile.peek() != -1) {
                // if an sd card is present AND the file is opened for reading successfully AND there is data in it
                // close the file so as not to interfere with the pushToCell function
                sdFile.close();
                if (sdPublisher.pushToCell(sdFile)) {
                    // if the data push was successful
                    if (sd.remove("adpl_data.txt")) {
                        // if the file was removed
                    } else {
                        // if the file failed to be removed
                        logError(SD_FILE_REMOVE_FAIL);
                    }
                } else {
                    // if the data push failed
                    logError(SD_DATA_PUSH_FAIL);
                }

            }
        } else if (SDCARD){ //only publish to SD card if there is no cell connection
            if(sdPublisher.publish(tempHXCI.temp, tempHXCO.temp, tempHTR.temp, tempHXHI.temp, tempHXHO.temp,
                                   int(valve.gasOn), int(bucket.tip_count), sdFile)){
                publishedSD = true;
            } else {
                logError(SD_PUB_FAIL);
                publishedSD = false;
            }
        } else {
            logError(TOTAL_PUB_FAIL);
        }
        if (publishedCell || publishedSD) {
            // reset the bucket tip count after every successful publish
            // (webserver will accumulate count)
            last_publish_time = millis();
            bucket.tip_count = 0;
        }
    }
    // measure temp, determine if light gas
    if (tempHTR.temp <= INCINERATE_LOW_TEMP && !valve.gasOn) {
        valve.open();
        delay(100);
        ignitor.fire();
    }

    if(valve.gasOn) {
        currentTime = millis();
        if (tempHTR.temp >= INCINERATE_HIGH_TEMP) {
            valve.close();
        }
            // if 15 min have elapsed since last ignitor fire, then fire again
        else if((currentTime - ignitor.timeLastFired) > IGNITE_DELAY) {
            ignitor.fire();
        }
    }

    currentTime = millis();
    if (!pump.pumping && (currentTime - pump.offTime) > KEEP_PUMP_OFF_TIME) {
        pump.turnOn();
    }
    else if (pump.pumping) {
        if ((currentTime - pump.onTime) > KEEP_PUMP_ON_TIME) {
            pump.turnOff();
        }
    }

    // flag variables changed in attachInterrupt function
    if(pinchValve.down) {
        pinchValve.shiftDown(pinchValve.resolution);
        EEPROM.put(write_address, pinchValve.position);
    }
    if(pinchValve.up) {
        pinchValve.shiftUp(pinchValve.resolution);
        EEPROM.put(write_address, pinchValve.position);
    }

    // unclog if no tip in a long while
    // open all the way up and come back to optimum
    currentTime = millis();
    if ((currentTime - bucket.lastTime) > (2 * bucket.lowFlow)) {
        pinchValve.unclog(UNCLOG_RESOLUTION);
        bucket.lastTime = currentTime;


        if(pinchValve.clogCounting >= 2 && pinchValve.position < MAX_POSITION){
            pinchValve.up = true;
            pinchValve.resolution = PUSH_BUTTON_RESOLUTION;
        }

    }

    if(bucket.tip) {
        pinchValve.clogCounting = 0;
        bucket.updateFlow();
        if (bucket.tipTime < bucket.highFlow && bucket.tipTime > bucket.highestFlow && pinchValve.position > MIN_POSITION) {
            pinchValve.down = true;
            pinchValve.resolution = FEEDBACK_RESOLUTION;
        }
        else if (bucket.tipTime > bucket.lowFlow && pinchValve.position < MAX_POSITION){
            pinchValve.up = true;
            pinchValve.resolution = FEEDBACK_RESOLUTION;
        }
        else if (bucket.tipTime < bucket.highestFlow){
            pinchValve.down = true; // handles sudden large flow
            pinchValve.resolution = HALF_RESOLUTION;
        }
    }
}

int read_temp(int temp_count) {
    switch (temp_count) {
        case 1:
            tempHXCI.read();
            temp_count++;
            break;
        case 2:
            tempHXCO.read();
            temp_count++;
            break;
        case 3:
            tempHTR.read();
            temp_count++;
            break;
        case 4:
            tempHXHI.read();
            temp_count++;
            break;
        case 5:
            tempHXHO.read();
            temp_count = 1;
            break;
    }
    return temp_count;
}

void bucket_tipped() {
    bucket.tipped();
    bucket.tip = true;
}

void res_pushed() {
    pinchValve.position = 0.0;
    pinchValve.up = true;
    pinchValve.resolution = PUSH_BUTTON_RESOLUTION;
    bucket.lastTime = millis();
}

void up_pushed() {
    pinchValve.up = true;
    pinchValve.resolution = PUSH_BUTTON_RESOLUTION;
}

void down_pushed() {
    pinchValve.down = true;
    pinchValve.resolution = PUSH_BUTTON_RESOLUTION;
}

void logError(int errorCode) {
    sdPublisher.logError(errorCode);
    if(Particle.connected()){
        Particle.publish("ERROR", errorCode);
    }
}
