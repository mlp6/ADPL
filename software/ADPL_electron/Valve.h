#ifndef Valve_h
#define Valve_h

#include "application.h"

class Valve {
    public:
        Valve(int pin);
        void open();
        void close();
        bool gasOn;
    private:
        int _pin;
        unsigned long _delay_before_ignition = 100;  // add some delay before igniting
};

#endif
