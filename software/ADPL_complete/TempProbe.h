#ifndef TempProbe_h
#define TempProbe_h

#include "Arduino.h"

class TempProbe {
    public:
        TempProbe(int pin);
        void read();
        float temp;
        unsigned long timeRead;
    private:
        int _pin;
        const int _NUMSAMPLES = 100;    // number of samples to read and average
        const int _SAMPLE_DELAY = 1;    // ms
        int _sumSamples = 0;
        float _sampleMean = 0;
        const float _THERMISTORNOMINAL = 10000.0;   // resistance (Ohm) @ 25 degrees C
        const float _TEMPERATURENOMINAL = 25.0;     // temp. (C) for nominal resistance
        const float _SERIESRESISTOR  = 10000.0;     // value of 'other' resistor (Ohm)
        const float _BCOEFFICIENT = 3950.0;         // beta coefficient of the thermistor 
                                                    // (usually 3000-4000)
};

#endif
