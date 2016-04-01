/*
 * Bucket.cpp - bucket tipping counter
 */

#include "application.h"
#include "Bucket.h"

Bucket::Bucket(int pin) {
    pinMode(pin, OUTPUT);
    _pin = pin;
    unsigned Long tip_time
    unsigned int tip_count = 0
}

void Pump::read() {
    // turn pump on and record time when that occurs
    if (digitalRead(_pin) == 1) {
        tip_time = millis();
        tip_count++
    }
}
