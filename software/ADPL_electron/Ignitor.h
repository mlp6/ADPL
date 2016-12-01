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
        static const int _ignitor_on_time = 1000;   // ms; keep on for 1 s

};

#endif
