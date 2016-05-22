/* ADPL_complete.ino
 * Master MCU code for all components:
 * + temp probes
 * + level sensor
 * + relays
 * + bucket tip
 *
 * LICENSE: Apache v2.0 (see LICENSE file)
 *
 * Copyright (c) 2015-2016 Mark Palmeri (Duke University)
 */

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
#define BUCKET_TIP_COUNT_DELAY 10000  // 10 s delay

// initialize some time counters
unsigned long currentTime = 0;
unsigned long last_publish_time = 0;

char temps_str [69];

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
        sprintf(temps_str,"HXCI:%.1f,HXCO:%.1f,HTR:%.1f,HXHI:%.1f,HXHO:%.1f",
                tempHXCI.temp, tempHXCO.temp, tempHTR.temp, tempHXHI.temp, tempHXHO.temp);
        Particle.publish("TEMPS",temps_str);
        delay(1000);
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
        if (bucket.tipped) {
          bucket.publish();
        }
    }

} // end loop()
