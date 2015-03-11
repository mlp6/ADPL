/* 
 * Pump.cpp - pump control
 */

#include "Arduino.h"
#include "Pump.h"

Pump::Pump(int pin) {
    pinMode(pin, OUTPUT);
    _pin = pin;
    unsigned long onTime;
    unsigned long offTime = 0;
}

void Pump::turnOn() {
    // turn pump on and record time when that occurs
    digitalWrite(_pin, HIGH);
    pumping = true;
    onTime = millis();
}

void Pump::turnOff() {
    // turn pump off and record time when that occurs
    digitalWrite(_pin, LOW);
    pumping = false;
    offTime = millis();
}
