/*
 * TempProbe.cpp - temperature probe (thermistor) class
 */

#include "application.h"
#include "TempProbe.h"
#include "math.h"

TempProbe::TempProbe(String name, int pin) {
    pinMode(pin, INPUT);
    _pin = pin;
    _name = name;
	  Particle.variable(String(_pin), temp);
}

void TempProbe::read() {
    // Code adapted from: https://learn.adafruit.com/thermistor/using-a-thermistor

    uint8_t i;
    temp = 0;

    // read in and sum all of the samples
    for (i=0; i < _NUMSAMPLES; i++) {
        _samples[i] = analogRead(_pin);
        delay(_SAMPLE_DELAY);
    }

    // record what time the measurement was made
    timeRead = millis();

    // take the mean of the samples
    for (i=0; i < _NUMSAMPLES; i++) {
        temp += _samples[i];
    }

    temp /= _NUMSAMPLES;

    // convert to resistance
    temp = (4095 / temp) - 1;
    temp = _SERIESRESISTOR / temp;

    // convert to deg C
    temp /= _THERMISTORNOMINAL;                     // (R/Ro)
    temp = log(temp);                               // ln(R/Ro);
    temp /= _BCOEFFICIENT;                          // 1/B * ln(R/Ro)
    temp += 1.0 / (_TEMPERATURENOMINAL + 273.15);   // +1/To
    temp = 1.0/temp;                                // invert
    temp -= 273.15;                                 // convert to C
}

void TempProbe::publish() {
    Particle.publish(String(_name), String::format("%.1f", temp));
}
