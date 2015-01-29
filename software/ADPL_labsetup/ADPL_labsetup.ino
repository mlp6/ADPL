//from other codes
#include <SPI.h>
#include "SD.h"
#include <Wire.h>
#include <OneWire.h>

//for SD card
const int chipSelect = 10;   //pin 10 for SD card
#define WAIT_TO_START 0      //start readings immediately

//for Pump control, setup with level sensor sending digital output, pins 8 & 13 are inputs from sensor signal
 unsigned int a = 0;   // pump off counter //"unsigned int" means positive integer
 unsigned int b = 0;   // pump on counter
const unsigned int amax = 57; //pump off time in ms (57 min)(pass of program is 1 minute, add 1 each time to 57)
const unsigned int bmax = 3;  //pump on time in ms (3 min)
int pump = 4;                //digital pin for pump
const int MinDepth = 13 ;    //Low water level pin 13
const int MaxDepth = 8;    //high water level pin 8
//const byte DepthMeter = A5; //analog input pin #5

//spark and valve
unsigned long spark_delay = 900000; //in ms (15min)
int ValveOn = 0;
int valve = 2;          //digital pin for valve
int ignitor = 7;        //digital pin for ignitor
int z=0;                //timer for ignitor

//variables for thermistor temp calculation
#define PROBE1 A0 //probes 1-5 are thermistors plugged into analog inputs 0-4
#define PROBE2 A1
#define PROBE3 A2
#define PROBE4 A3
#define PROBE5 A4
#define THERMISTORNOMINAL 10000 //resistance @ 25 degrees C
#define TEMPERATURENOMINAL 25  //temp. for nominal resistance
#define NUMSAMPLES 100  //number of samples to take and average
#define BCOEFFICIENT 3950 //the beta coefficient of the thermistor (usually 3000-4000)
#define SERIESRESISTOR 10000 //value of 'other' resistor
const unsigned long Delay = 60000; // define total delay in ms (1 minute)
//OneWire  ds(3);  // thermocoupler pins (3 & 5); not used in this sketch
//OneWire ds2(5);


//also needs pins for valve, ignitor

int samples[NUMSAMPLES];
int samples2[NUMSAMPLES];
int samples3[NUMSAMPLES];
int samples4[NUMSAMPLES];
int samples5[NUMSAMPLES];
File dataFile;

void setup()
{
  Serial.begin(9600); //serial transmission to computer in 9600 bits/sec
    analogReference(EXTERNAL);            //for thermistor
    pinMode(valve,OUTPUT);
    pinMode(ignitor,OUTPUT);
    pinMode(pump, OUTPUT);
    pinMode(MinDepth,INPUT);
    pinMode(MaxDepth,INPUT);
      
  pinMode(10, OUTPUT); //SD card pin
  
  
  dataFile = SD.open("datalog.txt", FILE_WRITE);
}

void loop()
{ 
  //Pump stuff - Level sensor sends signal to "low" (yellow) when below 2"; signal to "high" (purple/green) when above 12"
  int LowDepth = digitalRead(MinDepth); 
int HighDepth = digitalRead(MaxDepth);
     
   if(LowDepth == 0) // above minimum depth
  {
    delay(5);
    int LowDepth = digitalRead(MinDepth);
    if(LowDepth == 0)
    {  delay(5);
    int LowDepth = digitalRead(MinDepth);
    if(LowDepth == 0)
    {  delay(5);
    int LowDepth = digitalRead(MinDepth);
    if(LowDepth == 0)
    {  delay(5);
    int LowDepth = digitalRead(MinDepth);
    if(LowDepth == 0)
    {
    if(HighDepth == 0) // below max depth but higher than min
    {
     if(b<bmax)//Turn on the pump, count for pump on
        {
          b++;
          digitalWrite(pump,HIGH); //pump on
        } 
        
      else //count for pump off
      {
        if(a<amax) //count for pump off
      {
        a++ ;
        digitalWrite(pump,LOW);

      }
        else //reset timer
        {
          a=0;
          b=0;
        }
      }
    }
    else //above max height, pump on
    {
      delay(5);
      int HighDepth = digitalRead(MaxDepth);
      if (HighDepth == 1)
      {delay(5);
      int HighDepth = digitalRead(MaxDepth);
      if (HighDepth == 1)
      {delay(5);
      int HighDepth = digitalRead(MaxDepth);
      if (HighDepth == 1)
      {
        
    digitalWrite(pump,HIGH);  //pump on when level above maximum
    }
  else{
      if(a<amax) //count for pump off
      {
        a++ ;
        digitalWrite(pump,LOW);
      }
      else //count for pump on
      {
        if(b<bmax)
        {
          b++;
          digitalWrite(pump,HIGH); //pump on
        }
        else //reset timer
        {
          a=0;
          b=0;
        }
      }
    }
  }
  else {
      if(a<amax) //count for pump off
      {
        a++ ;
       digitalWrite(pump,LOW);

      }
      else //count for pump on
      {
        if(b<bmax)
        {
          b++;
          digitalWrite(pump,HIGH); //pump on
        }
        else //reset timer
        {
          a=0;
          b=0;
        }
      }
    }
}
else
{
      if(a<amax) //count for pump off
      {
        a++ ;
        digitalWrite(pump,LOW);

      }
      else //count for pump on
      {
        if(b<bmax)
        {
          b++;
          digitalWrite(pump,HIGH); //pump on
        }
        else //reset timer
        {
          a=0;
          b=0;
        }
      }
    }
}
}
}
}
}
}
else //below minimum height
  {
    digitalWrite(pump,LOW);  //turn pump off when level below minimum
  }
    //end of pump stuff
    
    
   //thermosistor temp calculation
 uint8_t i ;
 uint8_t v;
 uint8_t w;
 uint8_t x;
 uint8_t y;
 float average;
 float average2;
 float average3;
 float average4;
 float average5;
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
   samples4[y] = analogRead(PROBE5);
 }
 average = 0;  
 average2 = 0;
 average3 = 0;
 average4 = 0;
 average5 = 0;
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
 average4 += samples3[x];
 }
 for (y=0; y< NUMSAMPLES; y++) {
 average5 += samples3[y];
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
 TempProbe4 = 1.0/TempProbe3;  //invert
 TempProbe4 -= 273.15;   //
 
   float TempProbe5;
 TempProbe5 = average5 / THERMISTORNOMINAL; //(R/Ro)
 TempProbe5 = log(TempProbe5); // ln(R/Ro);
 TempProbe5 /= BCOEFFICIENT;  // 1/B * ln(R/Ro)
 TempProbe5 += 1.0 / (TEMPERATURENOMINAL + 273.15);  // +1/To
 TempProbe5 = 1.0/TempProbe3;  //invert
 TempProbe5 -= 273.15;   //
//end thermistors
 
//begin SD card
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
 dataFile.print(TempProbe1);
  dataFile.print(", ");
  dataFile.print(TempProbe2);
  dataFile.print(", ");
  dataFile.print(TempProbe3);
  dataFile.print(", ");
  dataFile.print(TempProbe4);
  dataFile.print(", ");
  dataFile.print(TempProbe5);
  dataFile.println(", ");
 //end SD card data
 //can I add a line to show if pump is on/off??
 
 

   if(TempProbe3<=68)      //temp is less than 68
   { 
     
    digitalWrite(valve,HIGH);                     // turn on heater (open valve, ignite)
    delay (10);
   digitalWrite(ignitor,HIGH);
   delay (5000);
   digitalWrite(ignitor,LOW);
   ValveOn = 1;
   }
   if (TempProbe3>=72)     //temp is greater than 72
   {
    ValveOn = 0;
     digitalWrite(valve,LOW);                        //turn off heater (close valve)
   }
 

z++; //timer for ignitor
unsigned long spark = spark_delay/Delay*NUMSAMPLES;  //spark_delay=900000 (15min), Delay=60000 (1min), NUMSAMPLES=100; spark=1500 (1.5sec)
if (z>spark)
{
  if(ValveOn == 1){
  digitalWrite(ignitor, HIGH);
  z = 0;
  delay(5000); //time in ms the ignitor is on, 5sec
  digitalWrite(ignitor,LOW);
}}
}
  
