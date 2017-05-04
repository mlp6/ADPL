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
        unsigned long baseFlow;
        unsigned long highFlow;
        unsigned long highestFlow;
        unsigned long lowFlow;
        unsigned long timeRead;
        unsigned long tipTime;
        unsigned long lastTime;
    private:
        static constexpr unsigned long _OPTIMALBOUND = 60000; // +/- 60 seconds
        static constexpr double _HIGHESTFLOW = 15.0; // in L/hr
};

#endif
