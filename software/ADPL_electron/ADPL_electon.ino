/* ADPL_complete.ino
 * This code includes all components - temp probes, level sensor, and relays
 * General overview of goals: Run pump based on tank level, operate burner based
 * on temp3 
 *
 * LICENSE: MIT (see LICENSE file)
 * 
 * Copyright (c) Aaron Forbis-Stokes and Mark Palmeri
 */

// AREF voltage (V) used for analog input reference (instead of default 5 V)
#define AREF 3.3 

#include "Valve.h"
// instantiate valve object on digital pin #4
Valve valve(4); 

#include "Pump.h"
// instatiate pump object on digital pin #7
Pump pump(7);

#include "TempProbe.h"
// instantiate temperature probe objects on analog pins A0-A4
TempProbe tempProbe1(A0);
TempProbe tempProbe2(A1);
TempProbe tempProbe3(A2); // might want to rename this object to tie it to ignitor func
TempProbe tempProbe4(A3);
TempProbe tempProbe5(A4);

#include "Ignitor.h"
// instantiate ignitor object on digital pin #2
Ignitor ignitor(2);
#define INCINERATE_LOW_TEMP 25  // will be 68 in field
#define INCINERATE_HIGH_TEMP 28 // will be 72 in field
#define IGNITE_DELAY 900000     // ms (15min); time between ignitor fires with open valve

#include "LevelSensor.h"
// instantiate level sensor object on analog pin A5
LevelSensor levelSensor(A5, AREF);
//define the current thresholds (mA) for the level sensor
#define LEVEL_MIN_MA 6
#define LEVEL_MAX_MA 18
#define KEEP_PUMP_ON_TIME 30000     // ms; keep pump on for 5 min for intermediate level
#define KEEP_PUMP_OFF_TIME 330000   // ms; keep pump off for 55 min after 5 min on time

unsigned long currentTime = 0;

//Code for SD card
#include <SPI.h>
#include "SD.h"
//#include <Wire.h> //Not necessary, I believe
//#include <OneWire.h> //Themrmocoupe
const int chipSelect = 10;   //pin 10 for SD card
#define WAIT_TO_START 0      //start readings immediately
File dataFile;  //SD card file name

void setup() {
    Serial.begin(9600);
    pinMode(10, OUTPUT); //SD card pin
    dataFile = SD.open("datalog.txt", FILE_WRITE);

    // define the reference for the analog input pins (3.3 V), which is more
    // stable than the default 5 V since we're not using any Wheatstone bridges
    // for each thermistor
    analogReference(EXTERNAL); 
}

void loop() { 
    // read probe temperatures
    tempProbe1.read();
    tempProbe2.read(); 
    tempProbe3.read(); 
    tempProbe4.read(); 
    tempProbe5.read(); 
 
    /* ==== Valve / Ignitor ====
     * Gas valve:
     *      open if temperature < 68 (INCINERATE_LOW_TEMP) 
     *      closed if temperature > 72 (INCINERATE_HIGH_TEMP)
     * When the temperature drops below 72, valve will not open until below 68.  
     * The ignitor will spark for 5 s every 15 minutes (IGNITE_DELAY) while * gas is on.
    */

    if (tempProbe3.temp <= INCINERATE_LOW_TEMP && valve.gasOn == false) {       
        valve.open();
        delay(100);
        ignitor.fire();
    }

    if(valve.gasOn == true) {

        currentTime = millis();

        if (tempProbe3.temp >= INCINERATE_HIGH_TEMP) {
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
 
    // SD card data collection commented out b/c connected to damaged Arduino
    // writeSDcard(TempProbe[0]);

    // temporary debugging serial print statements
    
    Serial.print(tempProbe1.temp);
    Serial.print(", ");
    Serial.print(tempProbe2.temp);
    Serial.print(", ");
    Serial.print(tempProbe3.temp);
    Serial.print(", ");
    Serial.print(tempProbe4.temp);
    Serial.print(", ");
    Serial.print(tempProbe5.temp);
    Serial.print(", ");
//    Serial.print(valve.gasOn);
//    Serial.print(", ");
//    Serial.print(pump.pumping);
//    Serial.print(", ");
    Serial.println(levelSensor.levelCurrentmA);
    dataFile.print(tempProbe1.temp);
    dataFile.print(", ");
    dataFile.print(tempProbe2.temp);
    dataFile.print(", ");
    dataFile.print(tempProbe3.temp);
    dataFile.print(", ");
    dataFile.print(tempProbe4.temp);
    dataFile.print(", ");
    dataFile.print(tempProbe5.temp);
    dataFile.print(", ");
    dataFile.print(valve.gasOn);
    dataFile.print(", ");
    dataFile.print(pump.pumping);
    dataFile.print(", ");
    dataFile.println(levelSensor.levelCurrentmA);
    
} // end loop()


/*
void writeSDcard(float TempProbe) {
    Serial.print(TempProbe);
    Serial.print(", ");
    dataFile.print(TempProbe);
    dataFile.print(", ");
}
*/
