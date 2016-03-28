/*
 * LevelSensor.cpp - water level sensor class
 */

#include "application.h"
#include "LevelSensor.h"

LevelSensor::LevelSensor(int pin) {
    pinMode(pin, INPUT);
    _pin = pin;
}

void LevelSensor::read() {
    _readVoltage = analogRead(_pin) * 3.3 / 4095;
    // current will range from 4 - 20 mA, which should correspond to 2 - 12"
    // the actual level will be calibrated in the Medcal software
    levelCurrentmA = _readVoltage * 1000 / _currentVoltRes;
}
