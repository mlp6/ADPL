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
    _levelCurrentmA = _readVoltage * 1000 / _currentVoltRes;

    if (_levelCurrentmA < _level_min_mA) {
        tooLow = true;
        tooHigh = false;
    }
    else if (_levelCurrentmA > _level_max_mA) {
        tooLow = false;
        tooHigh = true;
    }
    else if (_levelCurrentmA > _level_min_mA && _levelCurrentmA < _level_max_mA) {
        tooLow = false;
        tooHigh = false;
    }

}
