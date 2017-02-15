#ifndef Ignitor_h
#define Ignitor_h

#include "application.h"

class Ignitor {
    public:
        Ignitor(int pin);
        void fire();
        unsigned long timeLastFired;
    private:
        int _pin;
        static const int _ignitor_on_time = 10000;   // ms; keep on for 10 s

};

#endif
