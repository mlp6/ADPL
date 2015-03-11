/* 
 * Valve.cpp - gas valve control
 */

#include "Arduino.h"
#include "Valve.h"

Valve::Valve(int pin) {
    pinMode(pin, OUTPUT);
    _pin = pin;
}

void Valve::open() {
    digitalWrite(_pin, HIGH);
    gasOn = true;
}

void Valve::close() {
    digitalWrite(_pin, LOW);
    gasOn = false;
}
