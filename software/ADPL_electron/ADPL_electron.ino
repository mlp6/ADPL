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