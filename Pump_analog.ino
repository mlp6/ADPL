//from other codes
#include <SPI.h>
#include "SD.h"
#include <Wire.h>
#include <OneWire.h>

//for SD card
const int chipSelect = 10;
#define WAIT_TO_START 0

//for Pump
 unsigned int a = 0;   // pump off counter //"unsigned int" means positive integer
 unsigned int b = 0;   // pump on counter
const unsigned int amax = 57000; //pump off time in ms (57 min)(pass of program is 1 minute, add 1 each time to 57)
const unsigned int bmax = 3000;  //pump on time in ms (3 min)
int pump = 4;
#define LEVELRESISTOR 100
#define LEVELPIN A0 //trail with definitions, here with level sensor in A0
int analogLEVEL = 5; //trial with definitation, here with level sensor in A5
int val = 0;

void setup()
{
  Serial.begin(9600); //serial transmission to computer in 9600 bits/sec
    analogReference(EXTERNAL);            
    pinMode(pump, OUTPUT);
   // pinMode(analogLEVEL,INPUT);
//    pinMode(MinDepth,INPUT);
//    pinMode(MaxDepth,INPUT);
      
  pinMode(10, OUTPUT); //SD card pin
 
}

void loop()
{ 
//  float reading; //trial using level input in A0
//  reading = analogRead(LEVELPIN);
//  Serial.println(reading);
  val = digitalRead(analogLEVEL); //trial using level input in A5
  Serial.println(val);
  delay(1000);
}
/* Code for pump operation with digital output instead of analog below  
  //Pump - Level sensor sends signal to "low" (yellow) when below 2"; signal to "high" (purple/green) when above 12"
  delay(1000); //delay 1 sec and read again
  int LowDepth = digitalRead(MinDepth); 
  int HighDepth = digitalRead(MaxDepth);
  
       
   if(LowDepth == 0 && HighDepth == 0) // above minimum depth and below max depth
  {
    //Trial setup
    digitalWrite(pump,HIGH); //pump on
    delay(10000); //pump on for 10s
    digitalWrite(pump,LOW); //pump off
    delay(10000); //pump off for 10s, start loop over

    //Real setup
//    delay(20);
//    int LowDepth = digitalRead(MinDepth); 
//    int HighDepth = digitalRead(MaxDepth);
//    if(LowDepth == 0 && HighDepth == 0) // above minimum depth and below max depth
//    {
//       if(b<bmax)//Turn on the pump, count for pump on
//        {
//          b++;
//          digitalWrite(pump,HIGH); //pump on
//        } 
//        
//      else //count for pump off
//      {
//        if(a<amax) //count for pump off
//      {
//        a++ ;
//        digitalWrite(pump,LOW);
//
//      }
//        else //reset timer
//        {
//          a=0;
//          b=0;
//        }
//      }
//    }
  }
  else if (HighDepth == 1) //not between high and low, check if above max height
    {
      //Trial setup
    digitalWrite(pump,HIGH); //pump on
    delay(10000); //pump on for 10s
    //digitalWrite(pump,LOW); //pump off
      
      //Real setup
//      delay(5);
//      int HighDepth = digitalRead(MaxDepth);
//      if (HighDepth == 1)
//      {delay(5);
//      int HighDepth = digitalRead(MaxDepth);
//      if (HighDepth == 1)
//      {delay(5);
//      int HighDepth = digitalRead(MaxDepth);
//      if (HighDepth == 1)
//      {
//        
//    digitalWrite(pump,HIGH);  //level above max, pump on until not true
//    }
//  }}
   }
 
  else //below minimum height
  {
    digitalWrite(pump,LOW);  //turn pump off when level below minimum
  }
    //end of pump 
    
  //show if level is above/below low/high level
  Serial.print(LowDepth);
  Serial.print(", ");
  Serial.println(HighDepth);
}
*/

 
   

