#ifndef LevelSensor_h
#define LevelSensor_h

#include "Arduino.h"

class LevelSensor {
    public:
        LevelSensor(int pin, float analogRef);
        void read();
        float levelCurrentmA;
    private:
        int _pin;
        float _readVoltage;
        // analog input reference voltage
        float _AREF;
        // define resistor to convert current to voltage (Ohm)
        static const float _currentVoltRes = 237;
};

#endif
