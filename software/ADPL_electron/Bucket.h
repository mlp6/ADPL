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
        static constexpr unsigned long _OPTIMALBOUND = 45000; // +/- 45 seconds
        static constexpr double _HIGHESTFLOW = 15.0;
        double _VOLUME;
        double _OPTIMAL_FLOW;
};

#endif
