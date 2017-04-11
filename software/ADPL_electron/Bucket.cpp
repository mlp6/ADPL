/*
 * Bucket.cpp - bucket tipping counter
 */

#include "application.h"
#include "Bucket.h"

Bucket::Bucket(int pin, double volume, double optimal_flow): _VOLUME(volume), _OPTIMAL_FLOW(optimal_flow)
 {
    pinMode(pin, INPUT_PULLDOWN);
    unsigned int tip_count = 0;
    Particle.variable("bucket", (int) tip_count);
    unsigned long lastTime = 0;
}

void Bucket::tipped() {
    tip_count++;
}

void Bucket::updateFlow(){
    baseFlow = (unsigned long) _VOLUME*3600*(1/_OPTIMAL_FLOW);
    highFlow = baseFlow - _OPTIMALBOUND;
    lowFlow = baseFlow + _OPTIMALBOUND;

    timeRead = millis();
    tipTime = (unsigned long)(timeRead-lastTime);
    lastTime = timeRead;
    tip = false;
}

void Bucket::publish() {
    Particle.publish(String("BUCKET"), String(tip_count));
}
