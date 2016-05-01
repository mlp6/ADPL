/*
 * Bucket.cpp - bucket tipping counter
 */

#include "application.h"
#include "Bucket.h"

Bucket::Bucket(int pin) {
    pinMode(pin, OUTPUT);
    _pin = pin;
    unsigned long tip_time;
    unsigned int tip_count = 0;
    boolean tipped = false;
    Particle.variable("bucket", (int) tip_count);
}

void Bucket::read() {
    // turn pump on and record time when that occurs
    if (digitalRead(_pin) == 1) {
        tip_time = millis();
        tipped = true;
        tip_count++;
    }
}

void Bucket::publish() {
    Particle.publish("bucket_tip_time", String(tip_time));
    Particle.publish("bucket_tip_count", String(tip_count));
}
