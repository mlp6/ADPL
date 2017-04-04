#ifndef Bucket_h
#define Bucket_h

#include "application.h"

class Bucket {
    public:
        Bucket(int pin, double optimal_flow, int bucket_volume);
        void tipped();
        unsigned int tip_count;
        bool tip;
        void publish();
        void updateFlow(int currentTime);
        double tipTime;
        double baseFlow;
        double highFlow;
        double lowFlow;
    private:
        double lastTime;
        double _OPTIMALBOUND = 90; // +/- 90 seconds

};

#endif
