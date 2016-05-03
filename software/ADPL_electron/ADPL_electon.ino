/* ADPL_complete.ino
 * This code includes all components - temp probes, level sensor, and relays
 * General overview of goals: Run pump based on tank level, operate burner based
 * on temp3
 *
 * LICENSE: MIT (see LICENSE file)
 *
 * Copyright (c) 2015-2016 Aaron Forbis-Stokes and Mark Palmeri (Duke University)
 */

#define PUBLISH_DELAY 150000  // 2.5 min b/w variable publish

// Define GPIO pins by function
#define HXCI A0 // Heat Exchanger Cold Inlet
#define HXCO A1 // Heat Exchanger Cold Output
#define HTR A2  // Heater
#define HXHI A3 // Heat Exchanger Hot Inlet
#define HXHO A4 // Heat Exchanger Hot Outlet
#define PUMP 7
#define VALVE 4
#define IGNITOR 2
#define LEVEL B5
#define BUCKET D0


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
#define INCINERATE_LOW_TEMP 25  // will be 68 in field
#define INCINERATE_HIGH_TEMP 28 // will be 72 in field
#define IGNITE_DELAY 900000     // 15 min between ignitor fires with open valve

#include "Pump.h"
Pump pump(PUMP);
#define KEEP_PUMP_ON_TIME 30000     // 5 min
#define KEEP_PUMP_OFF_TIME 330000   // 55 min off time after 5 min on time
#include "LevelSensor.h"
LevelSensor levelSensor(LEVEL);

#include "Bucket.h"
Bucket bucket(BUCKET);
#define BUCKET_TIP_COUNT_DELAY 10000  // 1 min delay

// initialize some time counters
unsigned long currentTime = 0;
unsigned long last_publish_time = 0;

void setup() {
    Serial.begin(9600);
}

void loop() {
    tempHXCI.read();
    tempHXCO.read();
    tempHTR.read();
    tempHXHI.read();
    tempHXHO.read();

    currentTime = millis();
    if ((currentTime - last_publish_time) > PUBLISH_DELAY) {
        tempHXCI.publish();
        tempHXCO.publish();
        tempHTR.publish();
        tempHXHI.publish();
        tempHXHO.publish();
        bucket.publish();
        last_publish_time = currentTime;
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

    // read water level, determine pump on/off
    levelSensor.read();

    if (levelSensor.tooLow && pump.pumping) {
        pump.turnOff();
    }
    else if (levelSensor.tooHigh && !pump.pumping) {
        pump.turnOn();
    }
    // When the level is 2"<x<24", the pump should be on for 5 min, off 55 min.
    else if(!levelSensor.tooLow && !levelSensor.tooHigh) {
        currentTime = millis();
        if (!pump.pumping && (currentTime - pump.offTime) > KEEP_PUMP_OFF_TIME) {
            pump.turnOn();
        }
        else if (pump.pumping) {
            if ((currentTime - pump.onTime) > KEEP_PUMP_ON_TIME) {
                pump.turnOff();
            }
        }
    }

    // count bucket tip
    currentTime = millis();
    if ((currentTime - bucket.tip_time) > BUCKET_TIP_COUNT_DELAY) {
        bucket.read(); // read will also count if HIGH
    }

} // end loop()
