#ifndef Bucket_h
#define Bucket_h

#include "application.h"

class Bucket {
    public:
        Bucket(int pin);
        void tipped();
        unsigned int tip_count;
};

#endif
