/*
 * Bucket.cpp - bucket tipping counter
 */

#include "application.h"
#include "Bucket.h"

Bucket::Bucket(int pin) {
    pinMode(pin, INPUT_PULLDOWN);
    unsigned int tip_count = 0;
    Particle.variable("bucket", (int) tip_count);

    int iter = 0;
    double bucket_array[5] = {0, 0, 0, 0, 0};
    double flow_rate = 0.0;
    double time_diff = 0.000;
}

void Bucket::tipped() {
    tip_count++;
}

void Bucket::updateFlow(int currentTime){
    iter++;
    int count = iter%5;
    bucket_array[count] = currentTime/60000.0;

    if (iter%5!=4){
      time_diff = bucket_array[count] - bucket_array[count+1];
    }
    else{
      time_diff = bucket_array[4] - bucket_array[0];
    }
    flow_rate = (4.0/time_diff);
    tip = false;
}

void Bucket::publish() {
    Particle.publish(String("BUCKET"), String(tip_count));
}
