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

#include "Valve.h"
// instantiate valve object on digital pin #4
Valve valve(VALVE);

#include "Pump.h"
// instatiate pump object on digital pin #7
Pump pump(PUMP);

#include "TempProbe.h"
TempProbe tempHXCI("HXCI", HXCI);
TempProbe tempHXCO("HXCO", HXCO);
TempProbe tempHTR("HTR", HTR);
TempProbe tempHXHI("HXHI", HXHI);
TempProbe tempHXHO("HXHO", HXHO);

#include "Ignitor.h"
// instantiate ignitor object on digital pin #2
Ignitor ignitor(IGNITOR);
#define INCINERATE_LOW_TEMP 25  // will be 68 in field
#define INCINERATE_HIGH_TEMP 28 // will be 72 in field
#define IGNITE_DELAY 900000     // ms (15min); time between ignitor fires with open valve

#include "LevelSensor.h"
// instantiate level sensor object on analog pin B5
LevelSensor levelSensor(B5);
//define the current thresholds (mA) for the level sensor
#define LEVEL_MIN_MA 6
#define LEVEL_MAX_MA 18
#define KEEP_PUMP_ON_TIME 30000     // 5 min
#define KEEP_PUMP_OFF_TIME 330000   // 55 min off time after 5 min on time

#include "Bucket.h"
// instantiate bucket tipping sensor
Bucket bucket(C0);
#define BUCKET_TIP_COUNT_DELAY 1000  // 1 min delay

// initialize some time counters
unsigned long currentTime = 0;
unsigned long last_publish_time = 0;

void setup() {
    Serial.begin(9600);
}

void loop() {
    // read and publish probe temperatures
    tempHXCI.read();
    tempHXCO.read();
    tempHTR.read();
    tempHXHI.read();
    tempHXHO.read();
    //tempProbe6.read();
    //tempProbe7.read();

    currentTime = millis();
    if (currentTime > (last_publish_time + PUBLISH_DELAY)) {
        tempHXCI.publish();
        tempHXCO.publish();
        tempHTR.publish();
        tempHXHI.publish();
        tempHXHO.publish();
        //tempProbe6.publish();
        //tempProbe7.publish();
        bucket.publish();
        last_publish_time = currentTime;
    }

    /* ==== Valve / Ignitor ====
     * Gas valve:
     *      open if temperature < 68 (INCINERATE_LOW_TEMP)
     *      closed if temperature > 72 (INCINERATE_HIGH_TEMP)
     * When the temperature drops below 72, valve will not open until below 68.
     * The ignitor will spark for 5 s every 15 minutes (IGNITE_DELAY) while * gas is on.
    */

    if (tempHTR.temp <= INCINERATE_LOW_TEMP && valve.gasOn == false) {
        valve.open();
        delay(100);
        ignitor.fire();
    }

    if(valve.gasOn == true) {

        currentTime = millis();

        if (tempHTR.temp >= INCINERATE_HIGH_TEMP) {
            valve.close();
        }
        // if 15 min have elapsed since last ignitor fire, then fire again
        else if(currentTime > (ignitor.timeLastFired + IGNITE_DELAY)) {
            ignitor.fire();
        }
    }

    /* ==== Water Level Pump ====
    Pump off when the level is <2", Remain on when >24". When the level is
    2"<x<24", the pump should be on for 5 minutes, off 55 minutes.

    Sensor output is 4 mA at bottom (4") and 20 mA at top (24").
    Resistor is 237 Ohm

    TO DO: Values for comparison need to be updated for conversion to inches
    */
    levelSensor.read();

    if (levelSensor.levelCurrentmA < LEVEL_MIN_MA && pump.pumping) {
        pump.turnOff();
    }
    else if (levelSensor.levelCurrentmA > LEVEL_MAX_MA && !pump.pumping) {
        pump.turnOn();
    }
    else if(levelSensor.levelCurrentmA > LEVEL_MIN_MA && levelSensor.levelCurrentmA <= LEVEL_MAX_MA) {

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
    if (currentTime > (bucket.tip_time + BUCKET_TIP_COUNT_DELAY)) {
        bucket.read(); // read will also count if HIGH
    }

} // end loop()
