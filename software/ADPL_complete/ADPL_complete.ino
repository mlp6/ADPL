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
#define VALVE_PIN 4     // digital output pin #4
#define PUMP_PIN 7      // digital output pin #7
#define IGNITOR_PIN 2   // digital output pin #2
#define INCINERATE_LOW_TEMP 25
#define INCINERATE_HIGH_TEMP 28

const unsigned long Delay = 1000; // define total delay in ms (1 sec)

//File dataFile;  //SD card file name

//variables for relays
boolean PumpOn = true;      // Variable for if pump is on/off
boolean ValveOn = false;    // Variable for if valve is on/off
unsigned int a = 0;         // pump off counter 
unsigned int b = 0;         // pump on counter
const unsigned int amax = 55; // pump off time in ms (55 min)
                              // (pass of original program is 1 minute, add 1 each time to 55)
const unsigned int bmax = 5;  //pump on time in ms (5 min)

const int numTempProbes = 5;
float TempProbe[numTempProbes];

const unsigned long ignite_delay = 900000;  // ms (15min); time between ignitor fires 
                                            // when valve is open
unsigned long last_ignite_time;             // time (ms) that will be returned from millis()
                                            // when ignitor last fired
unsigned long currentTime;

void setup() {
    Serial.begin(9600);
    pinMode(PROBE1, INPUT);
    pinMode(PROBE2, INPUT);
    pinMode(PROBE3, INPUT);
    pinMode(PROBE4, INPUT);
    pinMode(PROBE5, INPUT);
    pinMode(LEVEL, INPUT);
    pinMode(PUMP_PIN, OUTPUT);
    pinMode(VALVE_PIN, OUTPUT);
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
        openValve();
        delay(10);
        fireIgnitor()
    }

    if(ValveOn == true) {

        current_time = millis();

        if (TempProbe[2] >= INCINERATE_HIGH_TEMP) {
            closeValve();
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
    
    TO DO: 
        * FIX: Keep timing from delaying the rest of the loop. 
        * Values for comparison need to be updated for conversion to inches
    */
    if(depth >= 400)    //level is above the minimum
   {delay(5);
     if(depth >= 400)
     {delay(5);
     if(depth <= 1200)  //level is above min and below max
     {
       if(b<bmax)    //if the time is less than max time on
        {
          b++;
          digitalWrite(PUMP_PIN, HIGH);  //turn pump on
          PumpOn = true;
        }
        else        //if max time on has been reached, turn off
        {
          if(a<amax)
          {
            a++;
            digitalWrite(PUMP_PIN, LOW);
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
            digitalWrite(PUMP_PIN, HIGH);  //turn the pump on
          }
        }
     }
   }
   else    //level is below minimum
      {
        digitalWrite(PUMP_PIN, LOW);  //turn pump off and remain off
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


void openValve() {
    digitalWrite(VALVE_PIN, HIGH);
    ValveOn = true;
}

void closeValve() {
    digitalWrite(VALVE_PIN, LOW);
    ValveOn = false;
}
void fireIgnitor() {
    const int IGNITOR_ON_TIME = 5000;   // ms
    digitalWrite(IGNITOR_PIN, HIGH);
    delay(IGNITOR_ON_TIME)
    digitalWrite(IGNITOR_PIN, LOW);
}
