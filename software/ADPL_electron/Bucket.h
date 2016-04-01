#ifndef Bucket_h
#define Bucket_h

#include "application.h"

class Bucket {
    public:
        Bucket(int pin);
        void read();
        unsigned long tip_time;
        unsigned int tip_count;
    private:
        int _pin;
};

#endif
