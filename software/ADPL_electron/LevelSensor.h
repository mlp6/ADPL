#ifndef LevelSensor_h
#define LevelSensor_h

#include "application.h"

class LevelSensor {
    public:
        LevelSensor(int pin);
        void read();
        float levelCurrentmA;
    private:
        int _pin;
        float _readVoltage;
        // analog input reference voltage
        // define resistor to convert current to voltage (Ohm)
        static constexpr float _currentVoltRes = 237;
};

#endif
