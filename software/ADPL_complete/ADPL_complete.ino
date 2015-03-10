//This code includes all componenets - temp probes, level sensor, and relays
//General overview of goals: Run pump based on tank level, operate burner based on temp3
//SD card data collection commented out b/c connected to damaged Arduino

//Code for SD card
//#include <SPI.h>
//#include "SD.h"
//#include <Wire.h>
//#include <OneWire.h>
//
////for SD card
//const int chipSelect = 10;   //pin 10 for SD card
//#define WAIT_TO_START 0      //start readings immediately


// variables for analog inputs - temp and level
// probes 1-5 are thermistors plugged into analog inputs 0-4
#define PROBE1 A0 
#define PROBE2 A1 
#define PROBE3 A2
#define PROBE4 A3
#define PROBE5 A4
#define LEVEL A5

const unsigned long Delay = 1000; // define total delay in ms (1 sec)

//File dataFile;  //SD card file name

//variables for relays
int pump = 7;               // Pump relay signal is digital input pin #7
boolean PumpOn = true;      // Variable for if pump is on/off
int valve = 4;              // Valve relay signal is digital input pin #4
boolean ValveOn = false;    // Variable for if valve is on/off
int ignitor = 2;            // Ignitor relay signal is digital input pin #4
boolean IgnitorOn = true;   // Variable for if ignitor is on/off
int z=0;                    // timer for ignitor
unsigned long spark_delay = 900000; //in ms (15min)
unsigned int a = 0;         // pump off counter 
unsigned int b = 0;         // pump on counter
const unsigned int amax = 55; // pump off time in ms (55 min)
                              // (pass of original program is 1 minute, add 1 each time to 55)
const unsigned int bmax = 5;  //pump on time in ms (5 min)

const int numTempProbes = 5;
float TempProbe[numTempProbes];

void setup() {
    Serial.begin(9600); //serial transmission to computer in 9600 bits/sec
    analogReference(EXTERNAL);            //for thermistor
    //  pinMode(10, OUTPUT); //SD card pin
    pinMode(PROBE1, INPUT);
    pinMode(PROBE2, INPUT);
    pinMode(PROBE3, INPUT);
    pinMode(PROBE4, INPUT);
    pinMode(PROBE5, INPUT);
    pinMode(LEVEL, INPUT);
    pinMode(pump, OUTPUT);
    pinMode(valve, OUTPUT);
    pinMode(ignitor, OUTPUT);
  
    //  dataFile = SD.open("datalog.txt", FILE_WRITE);
}

void loop() { 
   
    // read probe temperatures

    for (int probeNum = 0; probeNum < numTempProbes; probeNum++) {
        TempProbe[probeNum] = readProbeTemp(probeNum);
    }

    //Level sensor code
    float depth;
    depth = analogRead(LEVEL);
    //Insert conversion of Arduino reading to level in inches
    //Sensor output is 4mA at bottom (4") and 20mA at top (24"). Resistor is 237 Ohm
 
    for (int probeNum = 0; probeNum < numTempProbes; probeNum++) {
        Serial.print(TempProbe[probeNum]);
        Serial.print(", ");
    }
    Serial.println(depth);
    Serial.print(ValveOn);
    Serial.print(", ");
    Serial.print(IgnitorOn);
    Serial.print(", ");
    Serial.println(PumpOn);
 
    /* Valve / Ignitor Activation
    The goal here is to have the gas valve open (high) when the temperature is
    <68 and closed when the temperature >72.  When the temperature drops below
    72, valve will not open until below 68.  While the valve is open, we want
    the ignitor to spark for 5 seconds every 15 minutes.
    */

    if (TempProbe[2] <= 25) {       
        digitalWrite(valve,HIGH);
        ValveOn = true;
        delay(10);
        digitalWrite(ignitor,HIGH);
        IgnitorOn = true;
        delay(5000);
        digitalWrite(ignitor,LOW);
        IgnitorOn = false;
   }

   if (TempProbe[2] >= 28) {
        ValveOn = false;
        digitalWrite(valve, LOW);
   }
 
    //This code is an attempt to have the ignitor spark for 5 seconds
    //every 15 minutes when the valve is open. 
    z++; //timer for ignitor
    // MARK COMMENTED OUT THE LINE BELOW; ONE BELOW THAT FOR TESTING
    //unsigned long spark = spark_delay/Delay*NUMSAMPLES;  
    unsigned long spark = spark_delay;  
    ////spark_delay=900000 (15min), Delay=60000 (1min), NUMSAMPLES=100; spark=1500 (1.5sec)
    if (z>=spark) {
        if(ValveOn == true){        //This time should only run when the valve is on
            digitalWrite(ignitor, HIGH);
            z = 0;
            delay(5000);    // time in ms the ignitor is on, 5sec
            digitalWrite(ignitor,LOW);
        }
    }
 
 //This code needs to be updated. This is controlling pump on/off
 //The goal is to have the pump off when the level is <2",
 //Remain on when >24". When the level is 2"<x<24", the pump should
 //be on for 5 minutes, off 55 minutes.
 //My challenge is keeping this timing from delaying the rest 
 //of the loop
 //Values for comparison need to be updated for conversion to inches
   if(depth >= 400)    //level is above the minimum
   {delay(5);
     if(depth >= 400)
     {delay(5);
     if(depth <= 1200)  //level is above min and below max
     {
       if(b<bmax)    //if the time is less than max time on
        {
          b++;
          digitalWrite(pump,HIGH);  //turn pump on
          PumpOn = true;
        }
        else        //if max time on has been reached, turn off
        {
          if(a<amax)
          {
            a++;
            digitalWrite(pump,LOW);
            PumpOn = false;
          }
          else    //reset timer
          {
            a=0;
            b=0;
          }
        }
     }
       else      //level is above min AND above max
        {
          delay(5);
          if(depth <= 1200)
          {
            digitalWrite(pump,HIGH);  //turn the pump on
          }
        }
     }
   }
   else    //level is below minimum
      {
        digitalWrite(pump,LOW);  //turn pump off and remain off
      }
   
  
    //begin SD card
    /* Serial.print(TempProbe[0]);
      Serial.print(", ");
     dataFile.print(TempProbe[0]);
      dataFile.print(", ");
    */
     //end SD card data
 
}


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
    float average = 0;
    int samples[NUMSAMPLES];

    for (int i=0; i < NUMSAMPLES; i++) {
        samples[i] = analogRead(ProbePin);
        delay(SAMPLE_DELAY);
    }

    for (int i=0; i< NUMSAMPLES; i++) {
        average += samples[i];
    } 
    average/=NUMSAMPLES;
    average = 1023 / average - 1;
    average = SERIESRESISTOR / average;

    TempProbe = average / THERMISTORNOMINAL;            // (R/Ro)
    TempProbe = log(TempProbe);                         // ln(R/Ro);
    TempProbe /= BCOEFFICIENT;                          // 1/B * ln(R/Ro)
    TempProbe += 1.0 / (TEMPERATURENOMINAL + 273.15);   // +1/To
    TempProbe = 1.0/TempProbe;                          // invert
    TempProbe -= 273.15;                                // convert to C

    return TempProbe;
}
