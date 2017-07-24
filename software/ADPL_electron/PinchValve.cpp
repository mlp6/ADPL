/*
 * PinchValve.cpp - pinch valve control system
 */

#include "application.h"
#include "PinchValve.h"

PinchValve::PinchValve(int dir_pin, int step_pin, int sleep_pin, int up_pin, int down_pin, int res_pin) {
    pinMode(dir_pin, OUTPUT);
    pinMode(step_pin, OUTPUT);
    pinMode(sleep_pin, OUTPUT);

    pinMode(up_pin, INPUT_PULLUP);
    pinMode(down_pin, INPUT_PULLUP);
    pinMode(res_pin, INPUT_PULLUP);

    _dir_pin = dir_pin;
    _step_pin = step_pin;
    _sleep_pin = sleep_pin;

    _STEPSPERMM = (_FULLSTEPSPERTURN/_MMPERTURN)*_MICROSTEP;

    digitalWrite(_sleep_pin, LOW);
    bool up = false;
    double position = 0.0;
    double resolution = 0.0;
    Particle.variable("position", position);
    int clog_counter = 0;
}

void PinchValve::shiftDown(double res) {
    int turn_count = res*(_STEPSPERMM); // 1600 steps/2mm movement
    digitalWrite(_dir_pin, LOW);
    digitalWrite(_sleep_pin, HIGH);
    for (int i = 0; i < turn_count; i++) {
        digitalWrite(_step_pin, HIGH);
        delayMicroseconds(_DELAY);
        digitalWrite(_step_pin, LOW);
        delayMicroseconds(_DELAY);
    };
    down = true;
    position -= res;
    digitalWrite(_sleep_pin, LOW);
};

void PinchValve::shiftUp(double res) {
    int turn_count = res*(_STEPSPERMM); // 1600 steps/2mm movement
    digitalWrite(_dir_pin, HIGH);
    digitalWrite(_sleep_pin, HIGH);
    for (int i = 0; i < turn_count; i++) {
        digitalWrite(_step_pin, HIGH);
        delayMicroseconds(_DELAY);
        digitalWrite(_step_pin, LOW);
        delayMicroseconds(_DELAY);
    };
    up = false;
    position += res;
    digitalWrite(_sleep_pin, LOW);
};

void PinchValve::unclog(float unclog_resolution) {
    shiftUp(unclog_resolution);
    shiftDown(unclog_resolution);
    shiftUp(unclog_resolution);
    shiftDown(unclog_resolution);
    clogCounting++;
}