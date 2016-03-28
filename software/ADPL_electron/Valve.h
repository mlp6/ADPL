#ifndef Valve_h
#define Valve_h

#include "application.h"

class Valve {
    public:
        Valve(int pin);
        void open();
        void close();
        boolean gasOn;
    private:
        int _pin;
};

#endif
