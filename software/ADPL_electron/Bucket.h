#ifndef Bucket_h
#define Bucket_h

#include "application.h"

class Bucket {
    public:
        Bucket(int pin);
        void tipped();
        unsigned int tip_count;
        void publish();
        int bucket_array[6];
        double updateFlow(bool was_successful);
        double flow_rate;
        bool was_successful;
    private:
        int iter;
        int sum;
};

#endif
