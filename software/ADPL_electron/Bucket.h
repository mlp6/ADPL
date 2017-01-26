#ifndef Bucket_h
#define Bucket_h

#include "application.h"

class Bucket {
    public:
        Bucket(int pin);
        void tipped();
        unsigned long tip_time;
        unsigned int tip_count;
        void publish();
        double bucket_array[6];
        double updateFlow();
        double flow_rate;
    private:
        int iter;
};

#endif
