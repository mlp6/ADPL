#ifndef Pump_h
#define Pump_h

#include "application.h"

class Pump {
    public:
        Pump(int pin);
        void turnOn();
        unsigned long onTime;
        void turnOff();
        unsigned long offTime;
        boolean pumping;
    private:
        int _pin;
};

#endif
