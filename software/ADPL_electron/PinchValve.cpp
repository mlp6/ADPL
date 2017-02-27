/*
 * PinchValve.cpp - pinch valve control system
 */

#include "application.h"
#include "PinchValve.h"

PinchValve::PinchValve(int dir_pin, int step_pin, int sleep_pin, int up_pin, int down_pin) {
    pinMode(dir_pin, OUTPUT);
    pinMode(step_pin, OUTPUT);
    pinMode(sleep_pin, OUTPUT);
    pinMode(up_pin, INPUT);
    pinMode(down_pin, INPUT);

    _dir_pin = dir_pin;
    _step_pin = step_pin;
    _sleep_pin = sleep_pin;
    _up_pin = up_pin;
    _down_pin = down_pin;

    digitalWrite(_sleep_pin, LOW);
    bool up = false;
    bool down = false;
}

void PinchValve::shiftUp() {
    // shifts the linear step motor a full quarter turn up, opening valve
    digitalWrite(_dir_pin, LOW);
    digitalWrite(_sleep_pin, HIGH);
    for (int i = 0; i < 400; i++) {
        digitalWrite(_step_pin, HIGH);
        delay(1);
        digitalWrite(_step_pin, LOW);
        delay(1);
    };
    up = false;
    digitalWrite(_sleep_pin, LOW);
};

void PinchValve::shiftDown() {
    // shifts the linear step motor a full quarter turn down, closing valve
    digitalWrite(_dir_pin, HIGH);
    digitalWrite(_sleep_pin, HIGH);
    for (int i = 0; i < 400; i++) {
        digitalWrite(_step_pin, HIGH);
        delay(1);
        digitalWrite(_step_pin, LOW);
        delay(1);
    };
    down = false;
    digitalWrite(_sleep_pin, LOW);
};
