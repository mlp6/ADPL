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
        static const int _NUMSAMPLES = 100;    // number of samples to read and average
        static const int _SAMPLE_DELAY = 10;   // ms
        int _samples[_NUMSAMPLES];
        static const float _THERMISTORNOMINAL = 10000.0;   // resistance (Ohm) @ 25 deg C
        static const float _TEMPERATURENOMINAL = 25.0;     // temp. (C) for nominal R 
        static const float _SERIESRESISTOR  = 10000.0;     // value of 'other' R (Ohm)
        static const float _BCOEFFICIENT = 3950.0;         // beta coefficient
};

#endif
