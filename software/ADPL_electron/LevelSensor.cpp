/*
 * LevelSensor.cpp - water level sensor class
 */

#include "Arduino.h"
#include "LevelSensor.h"

LevelSensor::LevelSensor(int pin, float analogRef) {
    pinMode(pin, INPUT);
    _pin = pin;
    _AREF = analogRef;
}

void LevelSensor::read() {
    _readVoltage = analogRead(_pin) * _AREF / 1023;
    // current will range from 4 - 20 mA, which should correspond to 2 - 12"
    // the actual level will be calibrated in the Medcal software
    levelCurrentmA = _readVoltage * 1000 / _currentVoltRes;
}
