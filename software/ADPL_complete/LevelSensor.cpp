/*
 * LevelSensor.cpp - water level sensor class
 */

#include "Arduino.h"
#include "LevelSensor.h"

LevelSensor::LevelSensor(int pin) {
    pinMode(pin, INPUT);
    _pin = pin;
}

void LevelSensor::read() {
    _rawRead = analogRead(_pin);
    level = (float) _rawRead;
    // TODO: NEED TO CONVERT TO USEFUL UNIT (INCHES)
}
