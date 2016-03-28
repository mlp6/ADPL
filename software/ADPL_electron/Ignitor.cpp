/* 
 * Ignitor.cpp - pump control
 */

#include "application.h"
#include "Ignitor.h"

Ignitor::Ignitor(int pin) {
    pinMode(pin, OUTPUT);
    _pin = pin;
    timeLastFired = 0;
}

void Ignitor::fire() {
    // turn ignitor on for 5 sec and record time when that occurs
    digitalWrite(_pin, HIGH);
    delay(_ignitor_on_time);
    digitalWrite(_pin, LOW);
    timeLastFired = millis();
}
