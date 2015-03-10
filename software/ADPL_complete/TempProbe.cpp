/*
    TempProbe.cpp - temperature probe (thermistor) class

    The MIT License (MIT)

    Copyright (c) 2015 Aaron Forbis-Stokes and Mark Palmeri

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

#include "Arduino.h"
#include "TempProbe.h"

TempProbe::TempProbe(int probePin) {

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
