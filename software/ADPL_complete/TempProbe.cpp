/* 
 * TempProbe.cpp - temperature probe (thermistor) class
 */

#include "Arduino.h"
#include "TempProbe.h"

TempProbe::TempProbe(int pin) {
    pinMode(pin, INPUT);
    _pin = pin;
}

void TempProbe::read() {
    // Code adapted from: https://learn.adafruit.com/thermistor/using-a-thermistor
    
    uint8_t i;

    // read in and sum all of the samples
    for (i=0; i < _NUMSAMPLES; i++) {
        _samples[i] = analogRead(_pin);
        Serial.println(_samples[i]);
        delay(_SAMPLE_DELAY);
    }

    // record what time the measurement was made
    timeRead = millis();
    
    // take the mean of the samples
    float temp = 0;
    for (i=0; i < _NUMSAMPLES; i++) {
        temp += _samples[i];
    }
    
    temp /= _NUMSAMPLES;

    // convert to resistance
    temp = (1023 / temp) - 1;
    temp = _SERIESRESISTOR / temp;

    Serial.print("Thermistor resistance (Ohm):"
    Serial.println(temp);

    // convert to deg C
    temp /= _THERMISTORNOMINAL;                     // (R/Ro)
    Serial.println(temp);
    temp = log(temp);                               // ln(R/Ro);
    Serial.println(temp);
    temp /= _BCOEFFICIENT;                          // 1/B * ln(R/Ro)
    Serial.println(temp);
    temp += 1.0 / (_TEMPERATURENOMINAL + 273.15);   // +1/To
    Serial.println(temp);
    temp = 1.0/temp;                                // invert
    Serial.println(temp);
    temp -= 273.15;                                 // convert to C
    Serial.println(temp);
}
