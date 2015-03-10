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

    pinMode(probePin, INPUT);
    _probePin = probePin;

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
    averageTemp = _SERIESRESISTOR / averageTemp;

    TempProbe = averageTemp / _THERMISTORNOMINAL;       // (R/Ro)
    TempProbe = log(TempProbe);                         // ln(R/Ro);
    TempProbe /= _BCOEFFICIENT;                         // 1/B * ln(R/Ro)
    TempProbe += 1.0 / (_TEMPERATURENOMINAL + 273.15);  // +1/To
    TempProbe = 1.0/TempProbe;                          // invert
    TempProbe -= 273.15;                                // convert to C

    return TempProbe;
}
