#ifndef Ignitor_h
#define Ignitor_h

#include "application.h"

class Ignitor {
    public:
        Ignitor(int pin);
        void fire();
        unsigned long timeLastFired;
        unsigned int repeatRefireAttempts = 0;
        unsigned int repeatRefireLimit = 5;
        unsigned int refireDelay = 60000; // ms; delay between ignition attempts
    private:
        int _pin;
        static const int _ignitor_on_time = 10000;   // ms; keep on for 10 s

};

#endif
