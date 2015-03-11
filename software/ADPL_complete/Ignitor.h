#ifndef Ignitor_h
#define Ignitor_h

#include "Arduino.h"

class Ignitor {
    public:
        Ignitor(int pin);
        void fire();
        unsigned long timeLastFired = 0;
    private:
        int _pin;
        const int _ignitor_on_time = 5000;   // ms; keep on for 5 s

};

#endif
