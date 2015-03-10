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

//variables for analog inputs - temp and level
#define PROBE1 A0 //probes 1-5 are thermistors plugged into analog inputs 0-4
#define PROBE2 A1 
#define PROBE3 A2
#define PROBE4 A3
#define PROBE5 A4
#define LEVEL A5
#define THERMISTORNOMINAL 10000 //resistance @ 25 degrees C
#define TEMPERATURENOMINAL 25  //temp. for nominal resistance
#define NUMSAMPLES 100  //number of samples to take and average
#define BCOEFFICIENT 3950 //the beta coefficient of the thermistor (usually 3000-4000)
#define SERIESRESISTOR 10000 //value of 'other' resistor
const unsigned long Delay = 1000; // define total delay in ms (1 sec)

int samples[NUMSAMPLES];
int samples2[NUMSAMPLES];
int samples3[NUMSAMPLES];
int samples4[NUMSAMPLES];
int samples5[NUMSAMPLES];
//File dataFile;  //SD card file name

//variables for relays
int pump = 7;       //Pump relay signal is digital input pin #7
boolean PumpOn = true;     //Variable for if pump is on/off
int valve = 4;      //Valve relay signal is digital input pin #4
int ValveOn = 0;    //Variable for if valve is on/off
int ignitor = 2;    //Ignitor relay signal is digital input pin #4
int IgnitorOn = 0;  //Variable for if ignitor is on/off
int z=0;            //timer for ignitor
unsigned long spark_delay = 900000; //in ms (15min)
unsigned int a = 0;   // pump off counter //"unsigned int" means positive integer
unsigned int b = 0;   // pump on counter
const unsigned int amax = 55; //pump off time in ms (55 min)(pass of original program is 1 minute, add 1 each time to 55)
const unsigned int bmax = 5;  //pump on time in ms (5 min)

void setup()
{
  Serial.begin(9600); //serial transmission to computer in 9600 bits/sec
    analogReference(EXTERNAL);            //for thermistor
//  pinMode(10, OUTPUT); //SD card pin
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
  pinMode(pump, OUTPUT);
  pinMode(valve, OUTPUT);
  pinMode(ignitor, OUTPUT);
  
//  dataFile = SD.open("datalog.txt", FILE_WRITE);
}

void loop()
{ 
  //Temperature probe codes
 uint8_t i;
 uint8_t v;
 uint8_t w;
 uint8_t x;
 uint8_t y;

 float average = 0;
 float average2 = 0;
 float average3 = 0;
 float average4 = 0;
 float average5 = 0;

 for (i=0; i< NUMSAMPLES; i++) {
   samples[i] = analogRead(PROBE1);
 delay(Delay/NUMSAMPLES);
 }
 for (v=0; v<NUMSAMPLES;v++){
   samples2[v] = analogRead(PROBE2);
 }
 for (w=0; w<NUMSAMPLES;w++){
   samples3[w] = analogRead(PROBE3);
 }
 for (x=0; x<NUMSAMPLES;x++){
   samples4[x] = analogRead(PROBE4);
 }
 for (y=0; y<NUMSAMPLES;y++){
   samples5[y] = analogRead(PROBE5);
 }

 for (i=0; i< NUMSAMPLES; i++) {
 average += samples[i];
 }
 for (v=0; v< NUMSAMPLES; v++) {
 average2 += samples2[v];
 }
 for (w=0; w< NUMSAMPLES; w++) {
 average3 += samples3[w];
 }
 for (x=0; x< NUMSAMPLES; x++) {
 average4 += samples4[x];
 }
 for (y=0; y< NUMSAMPLES; y++) {
 average5 += samples5[y];
 }
 average/=NUMSAMPLES;
 average2/=NUMSAMPLES;
 average3/=NUMSAMPLES;
 average4/=NUMSAMPLES;
 average5/=NUMSAMPLES;
 
 average = 1023 / average - 1;
 average2 = 1023 / average2 - 1;
 average3 = 1023 / average3 - 1;
 average4 = 1023 / average4 - 1;
 average5 = 1023 / average5 - 1;
 average = SERIESRESISTOR / average;
 average2 = SERIESRESISTOR/average2;
 average3 = SERIESRESISTOR/average3;
 average4 = SERIESRESISTOR/average4;
 average5 = SERIESRESISTOR/average5;
 
 float TempProbe1;
 TempProbe1 = average / THERMISTORNOMINAL; //(R/Ro)
 TempProbe1 = log(TempProbe1); // ln(R/Ro);
 TempProbe1 /= BCOEFFICIENT;  // 1/B * ln(R/Ro)
 TempProbe1 += 1.0 / (TEMPERATURENOMINAL + 273.15);  // +1/To
 TempProbe1 = 1.0/TempProbe1;  //invert
 TempProbe1 -= 273.15;   //convert to C
 
 float TempProbe2;
 TempProbe2 = average2 / THERMISTORNOMINAL; //(R/Ro)
 TempProbe2 = log(TempProbe2); // ln(R/Ro);
 TempProbe2 /= BCOEFFICIENT;  // 1/B * ln(R/Ro)
 TempProbe2 += 1.0 / (TEMPERATURENOMINAL + 273.15);  // +1/To
 TempProbe2 = 1.0/TempProbe2;  //invert
 TempProbe2 -= 273.15;   //
 
 float TempProbe3;
 TempProbe3 = average3 / THERMISTORNOMINAL; //(R/Ro)
 TempProbe3 = log(TempProbe3); // ln(R/Ro);
 TempProbe3 /= BCOEFFICIENT;  // 1/B * ln(R/Ro)
 TempProbe3 += 1.0 / (TEMPERATURENOMINAL + 273.15);  // +1/To
 TempProbe3 = 1.0/TempProbe3;  //invert
 TempProbe3 -= 273.15;   //
 
 float TempProbe4;
 TempProbe4 = average4 / THERMISTORNOMINAL; //(R/Ro)
 TempProbe4 = log(TempProbe4); // ln(R/Ro);
 TempProbe4 /= BCOEFFICIENT;  // 1/B * ln(R/Ro)
 TempProbe4 += 1.0 / (TEMPERATURENOMINAL + 273.15);  // +1/To
 TempProbe4 = 1.0/TempProbe4;  //invert
 TempProbe4 -= 273.15;   //
 
 float TempProbe5;
 TempProbe5 = average5 / THERMISTORNOMINAL; //(R/Ro)
 TempProbe5 = log(TempProbe5); // ln(R/Ro);
 TempProbe5 /= BCOEFFICIENT;  // 1/B * ln(R/Ro)
 TempProbe5 += 1.0 / (TEMPERATURENOMINAL + 273.15);  // +1/To
 TempProbe5 = 1.0/TempProbe5;  //invert
 TempProbe5 -= 273.15;   // 
 
 //end temperature probe codes
 
 //Level sensor code
 float depth;
  depth=analogRead(LEVEL);
  //Insert conversion of Arduino reading to level in inches
  //Sensor output is 4mA at bottom (4") and 20mA at top (24"). Resistor is 237 Ohm
 
  Serial.print(TempProbe1);
  Serial.print(", ");
  Serial.print(TempProbe2);
  Serial.print(", ");
  Serial.print(TempProbe3);
  Serial.print(", ");
  Serial.print(TempProbe4);
  Serial.print(", ");
  Serial.print(TempProbe5);
  Serial.println(", ");
  Serial.println(depth);
  Serial.print(ValveOn);
  Serial.print(", ");
  Serial.print(IgnitorOn);
  Serial.print(", ");
  Serial.println(PumpOn);
 
 //commands for valve/ignitor
 //The goal here is to have the gas valve open (high) when the 
 //temperature is <68 and closed when the temperature >72.
 //When the temperature drops below 72, valve will not open until below 68.
 //While the valve is open, we want the ignitor to spark for 5 seconds
 //every 15 minutes.
    if(TempProbe3<=25)      //temp is less than 25 for lab check
   { 
     
    digitalWrite(valve,HIGH);    // turn on heater (open valve, ignite)
    ValveOn = 1;
    delay (10);
   digitalWrite(ignitor,HIGH);
   IgnitorOn = 1;
   delay (5000);
   digitalWrite(ignitor,LOW);
   IgnitorOn=0;
   }
   if (TempProbe3>=28)     //temp is greater than 28
   {
    ValveOn = 0;
     digitalWrite(valve,LOW);                        //turn off heater (close valve)
   }
 
 //This code is an attempt to have the ignitor spark for 5 seconds
//every 15 minutes when the valve is open. 
 z++; //timer for ignitor
unsigned long spark = spark_delay/Delay*NUMSAMPLES;  //spark_delay=900000 (15min), Delay=60000 (1min), NUMSAMPLES=100; spark=1500 (1.5sec)
if (z>=spark)
{
  if(ValveOn == 1){        //This time should only run when the valve is on
  digitalWrite(ignitor, HIGH);
  z = 0;
  delay(5000); //time in ms the ignitor is on, 5sec
  digitalWrite(ignitor,LOW);
}}
 
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
/* Serial.print(TempProbe1);
  Serial.print(", ");
 dataFile.print(TempProbe1);
  dataFile.print(", ");
*/
 //end SD card data
 
}
  
