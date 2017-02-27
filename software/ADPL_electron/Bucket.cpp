/*
 * Bucket.cpp - bucket tipping counter
 */

#include "application.h"
#include "Bucket.h"

Bucket::Bucket(int pin) {
    pinMode(pin, INPUT_PULLDOWN);
    unsigned int tip_count = 0;
    Particle.variable("bucket", (int) tip_count);
    double bucket_array[6] = {8.0, 8.0, 8.0, 8.0, 8.0, 8.0};
    iter = 0;
    double flow_rate = 0.0;
}

void Bucket::tipped() {
    tip_count++;
}

double Bucket::updateFlow(){
    iter++;
    bucket_array[iter%6] = tip_count;
    if (iter!=5){
        flow_rate = 4*(bucket_array[iter]-bucket_array[iter+1]);
    }
    else{
        flow_rate = 4*(bucket_array[5]-bucket_array[0]);
    }
    return flow_rate;
}

void Bucket::publish() {
    Particle.publish(String("BUCKET"), String(tip_count));
}
