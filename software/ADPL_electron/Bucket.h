#ifndef Bucket_h
#define Bucket_h

#include "application.h"

class Bucket {
    public:
        Bucket(int pin);
        void tipped();
        unsigned int tip_count;
        bool tip;
        void publish();
        void updateFlow(int currentTime);
        double flow_rate;
    private:
        double bucket_array[5];
        int iter;
        double time_diff;
};

#endif
