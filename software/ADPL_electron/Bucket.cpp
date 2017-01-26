/*
 * Bucket.cpp - bucket tipping counter
 */

#include "application.h"
#include "Bucket.h"

Bucket::Bucket(int pin) {
    pinMode(pin, INPUT_PULLDOWN);
    unsigned long tip_time = 0;
    unsigned int tip_count = 0;
    Particle.variable("bucket", (int) tip_count);
    double bucket_array[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    iter = 0;
}

void Bucket::tipped() {
    tip_time = millis();
    tip_count++;
    iter++;
    bucket_array[iter%6] = tip_count;
}

void Bucket::updateFlow(){
    flow_rate = (bucket_array[5] - bucket_array[0])/0.25;
}

void Bucket::publish() {
    Particle.publish(String("BUCKET"), String(tip_count));
}
