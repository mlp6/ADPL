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
}

void Bucket::tipped() {
    tip_time = millis();
    tip_count++;
}

void Bucket::publish() {
    Particle.publish(String("BUCKET"), String(tip_count));
}
