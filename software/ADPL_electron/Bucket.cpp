/*
 * Bucket.cpp - bucket tipping counter
 */

#include "application.h"
#include "Bucket.h"

Bucket::Bucket(int pin, double optimal_flow, int bucket_volume) {
    pinMode(pin, INPUT_PULLDOWN);
    unsigned int tip_count = 0;
    Particle.variable("bucket", (int) tip_count);

    // sec/bucket tip
    double baseFlow = (1/(optimal_flow*1000*(1/bucket_volume)*(1/3600)));
    double highFlow = baseFlow-_OPTIMALBOUND;
    double lowFlow = baseFlow+_OPTIMALBOUND;
    double tipTime = 0.0;
    double lastTime = 0.0;
}

void Bucket::tipped() {
    tip_count++;
}

void Bucket::updateFlow(int currentTime){
    tipTime = currentTime/(1000)-lastTime;
    lastTime = currentTime/(1000);
    tip = false;
}

void Bucket::publish() {
    Particle.publish(String("BUCKET"), String(tip_count));
}
