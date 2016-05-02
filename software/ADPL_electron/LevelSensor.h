#ifndef LevelSensor_h
#define LevelSensor_h

#include "application.h"

class LevelSensor {
    public:
        LevelSensor(int pin);
        void read();
        boolean tooLow;
        boolean tooHigh;
    private:
        int _pin;
        float _readVoltage;
        float _levelCurrentmA;
        // define mA thresholds for level sensor
        static constexpr float _level_min_mA = 6; // <2"
        static constexpr float _level_max_mA = 18; //>24"
        // analog input reference voltage
        // define resistor to convert current to voltage (Ohm)
        static constexpr float _currentVoltRes = 237;
};

#endif
