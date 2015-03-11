/* ADPL_complete.ino
 *
 * This code includes all components - temp probes, level sensor, and relays
 * General overview of goals: Run pump based on tank level, operate burner based
 * on temp3 
 *
 * LICENSE: MIT (see LICENSE file)
 * 
 * Copyright (c) Aaron Forbis-Stokes and Mark Palmeri
 *
 */

#include "Valve.h"
Valve valve(4);     // instantiate valve object on digital pin #4

//Code for SD card
//#include <SPI.h>
//#include "SD.h"
//#include <Wire.h>
//#include <OneWire.h>
//
////for SD card
//const int chipSelect = 10;   //pin 10 for SD card
//#define WAIT_TO_START 0      //start readings immediately

// temperature (thermistor) variables
#define PROBE1 A0 
#define PROBE2 A1 
#define PROBE3 A2
#define PROBE4 A3
#define PROBE5 A4
const int numTempProbes = 5;
float TempProbe[numTempProbes];

// ignitor variables
#define IGNITOR_PIN 2   // digital output pin #2
#define INCINERATE_LOW_TEMP 25
#define INCINERATE_HIGH_TEMP 28
const unsigned long ignite_delay = 900000;  // ms (15min); time between ignitor fires 
                                            // when valve is open
unsigned long last_ignite_time;             // time (ms) that will be returned from millis()
                                            // when ignitor last fired

// water level variables
#define LEVEL_PIN A5
#define PUMP_PIN 7      // digital output pin #7
#define LEVEL_MIN 400
#define LEVEL_MAX 1200
#define KEEP_PUMP_ON_TIME 30000     // ms; keep pump on for 5 min for intermediate level
#define KEEP_PUMP_OFF_TIME 330000   // ms; keep pump off for 55 min after 5 min on time
unsigned long pumpTurnOffTime = 0;  // initialize to start the pump in the OFF state
unsigned long pumpTurnOnTime;
boolean PumpOn = false;             // Variable for if pump is on/off

unsigned long current_time;
//
//File dataFile;  //SD card file name

void setup() {
    Serial.begin(9600);
    pinMode(PROBE1, INPUT);
    pinMode(PROBE2, INPUT);
    pinMode(PROBE3, INPUT);
    pinMode(PROBE4, INPUT);
    pinMode(PROBE5, INPUT);
    pinMode(LEVEL_PIN, INPUT);
    pinMode(PUMP_PIN, OUTPUT);
    pinMode(IGNITOR_PIN, OUTPUT);
    // pinMode(10, OUTPUT); //SD card pin
    // dataFile = SD.open("datalog.txt", FILE_WRITE);

    // need to define the reference for the analog input pins (3.3 V), which is more 
    // stable than the default 5 V since we're not using any Wheatstone bridges
    // for each thermistor
    analogReference(EXTERNAL); 
}

void loop() { 
   
    // read probe temperatures
    for (int probeNum = 0; probeNum < numTempProbes; probeNum++) {
        TempProbe[probeNum] = readProbeTemp(probeNum);
    }

    for (int probeNum = 0; probeNum < numTempProbes; probeNum++) {
        Serial.print(TempProbe[probeNum]);
        Serial.print(", ");
    }

    Serial.print(valve.gasOn);
    Serial.print(", ");
    // Serial.print(IgnitorOn);
    // Serial.print(", ");
    Serial.println(PumpOn);
 
    /* ==== Valve / Ignitor ====
    Gas valve open when the temperature is < 68 (INCINERATE_LOW_TEMP) and
    closed when the temperature > 72 (INCINERATE_HIGH_TEMP).  When the
    temperature drops below 72, valve will not open until below 68.  The
    ignitor will spark for 5 seconds every 15 minutes while the valve is open.
    */

    if (TempProbe[2] <= INCINERATE_LOW_TEMP) {       
        valve.open();
        delay(10);
        fireIgnitor();
    }

    if(valve.gasOn == true) {

        current_time = millis();

        if (TempProbe[2] >= INCINERATE_HIGH_TEMP) {
            valve.close();
        }
        // if 15 min have elapsed since last ignitor fire, then fire again
        else if(current_time > (last_ignite_time + ignite_delay)) {     
            fireIgnitor();
            last_ignite_time = millis();
        }
    }    
 
    /* ==== Water Level Pump ====
    Pump off when the level is <2", Remain on when >24". When the level is
    2"<x<24", the pump should be on for 5 minutes, off 55 minutes.  
    
    Sensor output is 4 mA at bottom (4") and 20 mA at top (24").
    Resistor is 237 Ohm

    TO DO: Values for comparison need to be updated for conversion to inches
    */
    float waterLevel = analogRead(LEVEL_PIN);
    Serial.println(waterLevel);

    if (waterLevel > LEVEL_MAX && !PumpOn) {
        turnPumpOn();
    }
    else if(waterLevel > LEVEL_MIN && waterLevel <= LEVEL_MAX) {

        current_time = millis();

        if (!PumpOn && (current_time - pumpTurnOffTime) > KEEP_PUMP_OFF_TIME) {
            turnPumpOn();
            pumpTurnOnTime = millis();
        }
        else if (PumpOn) {
            if ((current_time - pumpTurnOnTime) > KEEP_PUMP_ON_TIME) {
                turnPumpOff();
                pumpTurnOffTime = millis();
            }
        }
    }   
 
    // SD card data collection commented out b/c connected to damaged Arduino
    // writeSDcard(TempProbe[0]);

} // end loop()


// read temperature from probe (thermistor)
float readProbeTemp(int ProbePin) {

    const float THERMISTORNOMINAL  = 10000.0;   // resistance (Ohm) @ 25 degrees C
    const float TEMPERATURENOMINAL = 25.0;      // temp. (C) for nominal resistance
    const float SERIESRESISTOR  = 10000.0;      // value of 'other' resistor (Ohm)
    const float BCOEFFICIENT = 3950.0;          // beta coefficient of the thermistor 
                                                // (usually 3000-4000)
    const int NUMSAMPLES = 100;                 // number of samples to take and average
    const int SAMPLE_DELAY = 1;                 // ms

    float TempProbe;
    float averageTemp = 0;
    int samples[NUMSAMPLES];

    for (int i=0; i < NUMSAMPLES; i++) {
        averageTemp += (float) analogRead(ProbePin);
        delay(SAMPLE_DELAY);
    }

    averageTemp /= (float) NUMSAMPLES;
    averageTemp = 1023. / averageTemp - 1;
    averageTemp = SERIESRESISTOR / averageTemp;

    TempProbe = averageTemp / THERMISTORNOMINAL;        // (R/Ro)
    TempProbe = log(TempProbe);                         // ln(R/Ro);
    TempProbe /= BCOEFFICIENT;                          // 1/B * ln(R/Ro)
    TempProbe += 1.0 / (TEMPERATURENOMINAL + 273.15);   // +1/To
    TempProbe = 1.0/TempProbe;                          // invert
    TempProbe -= 273.15;                                // convert to C

    return TempProbe;
}


void fireIgnitor() {
    const int IGNITOR_ON_TIME = 5000;   // ms
    digitalWrite(IGNITOR_PIN, HIGH);
    delay(IGNITOR_ON_TIME);
    digitalWrite(IGNITOR_PIN, LOW);
}

void turnPumpOn() {
    digitalWrite(PUMP_PIN, HIGH); 
    PumpOn = true;
}

void turnPumpOff() {
    digitalWrite(PUMP_PIN, LOW);
    PumpOn = false;
}

/*
void writeSDcard(float TempProbe) {
    Serial.print(TempProbe);
    Serial.print(", ");
    dataFile.print(TempProbe);
    dataFile.print(", ");
}
*/
