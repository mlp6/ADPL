#ifndef Bucket_h
#define Bucket_h

#include "application.h"

class Bucket {
    public:
        Bucket(int pin, double volume, double optimal_flow);
        void tipped();
        unsigned int tip_count;
        bool tip;
        void publish();
        void updateFlow();
        void setup();
        unsigned long tipTime;
        unsigned long baseFlow;
        unsigned long highFlow;
        unsigned long lowFlow;
        unsigned long timeRead;
        unsigned long lastTime;
    private:
        static constexpr unsigned long _OPTIMALBOUND = 90000; // +/- 90 seconds
        double _VOLUME;
        double _OPTIMAL_FLOW; 
};

#endif
