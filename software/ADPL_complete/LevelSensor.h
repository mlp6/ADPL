#ifndef LevelSensor_h
#define LevelSensor_h

#include "Arduino.h"

class LevelSensor {
    public:
        LevelSensor(int pin);
        void read();
        float level;
    private:
        int _pin;
        int _rawRead;
};

#endif
