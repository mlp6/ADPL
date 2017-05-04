/*
 * Bucket.cpp - bucket tipping counter
 */

#include "application.h"
#include "Bucket.h"

Bucket::Bucket(int pin, double volume, double optimal_flow){
    pinMode(pin, INPUT_PULLDOWN);

    baseFlow = (unsigned long) volume*3600*(1/optimal_flow); // in ms
    highFlow = baseFlow - _OPTIMALBOUND; // in ms
    lowFlow = baseFlow + _OPTIMALBOUND; // in ms
    highestFlow = (unsigned long) volume*3600*(1/_HIGHESTFLOW); // in ms

    unsigned int tip_count = 0;
    Particle.variable("bucket", (int) tip_count);
    unsigned long lastTime = 0;
}

void Bucket::tipped() {
    tip_count++;
}

void Bucket::updateFlow(){
    timeRead = millis(); // make private
    tipTime = timeRead-lastTime;
    lastTime = timeRead;
    tip = false;
}

void Bucket::publish() {
    Particle.publish(String("BUCKET"), String(tip_count));
}
