#ifndef Bucket_h
#define Bucket_h

#include "application.h"

class Bucket {
    public:
        Bucket(int pin);
        void tipped();
        unsigned int tip_count;
        void publish();
        void updateFlow(bool was_successful, int publish_delay);
        double flow_rate;
        bool was_successful;
    private:
        int bucket_array[6];
        int iter;
        double sum;
};

#endif
