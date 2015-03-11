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
    
    int _sumSamples = 0; 
    float _sampleMean = 0.;
    
    // read in and sum all of the samples
    for (int i=0; i < _NUMSAMPLES; i++) {
        _sumSamples += analogRead(_pin);
        delay(_SAMPLE_DELAY);
    }

    // record what time the measurement was made
    timeRead = millis();
    
    // take the mean of the samples
    _sampleMean = (float) _sumSamples / (float) _NUMSAMPLES;
    _sampleMean = 1023. / _sampleMean - 1.;
    _sampleMean = _SERIESRESISTOR / _sampleMean;

    // convert to deg C
    temp = _sampleMean / _THERMISTORNOMINAL;        // (R/Ro)
    temp = log(temp);                               // ln(R/Ro);
    temp /= _BCOEFFICIENT;                          // 1/B * ln(R/Ro)
    temp += 1.0 / (_TEMPERATURENOMINAL + 273.15);   // +1/To
    temp = 1.0/temp;                                // invert
    temp -= 273.15;                                 // convert to C
}
