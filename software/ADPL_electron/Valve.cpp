/* 
 * Valve.cpp - gas valve control
 */

#include "application.h"
#include "Valve.h"

Valve::Valve(int pin) {
    pinMode(pin, OUTPUT);
    _pin = pin;
    // make sure gas valve is OFF to start with
    close();
    Particle.variable("gasOn", gasOn); 
}

void Valve::open() {
    digitalWrite(_pin, HIGH);
    delay(_delay_before_ignition);
    gasOn = true;
}

void Valve::close() {
    digitalWrite(_pin, LOW);
    gasOn = false;
}
