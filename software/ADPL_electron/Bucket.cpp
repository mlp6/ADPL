/*
 * Bucket.cpp - bucket tipping counter
 */

#include "application.h"
#include "Bucket.h"

Bucket::Bucket(int pin) {
    pinMode(pin, INPUT);
    _pin = pin;
    unsigned long tip_time = 0;
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
    else{
      tipped = false;
    }
}

void Bucket::publish() {
    Particle.publish(String("BUCKET"), String(tip_count));
}
