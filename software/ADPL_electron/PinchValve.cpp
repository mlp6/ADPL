/*
 * PinchValve.cpp - pinch valve control system
 */

#include "application.h"
#include "PinchValve.h"

PinchValve::PinchValve(int dir_pin, int step_pin, int sleep_pin) {
    pinMode(dir_pin, OUTPUT);
    pinMode(step_pin, OUTPUT);
    pinMode(sleep_pin, OUTPUT);
    _dir_pin = dir_pin;
    _step_pin = step_pin;
    _sleep_pin = sleep_pin;
    flow_pos = 0;
    particle.variable("flow_pos", flow_pos);
    //develop a reset
}

void PinchValve::shiftOpen() {
    //need to investigate which direction opens, closes
    digitalWrite(_dir_pin, HIGH);
    for (int i = 0; i < 100; i++) {
        digitalWrite(_step_pin, HIGH);
        delay(100);
    };
    flow_pos +=100;
};

void PinchValve::shiftClose() {
    //need to investigate which direction opens, closes
    digitalWrite(_dir_pin, LOW);
    for (int i = 0; i < 100; i++) {
        digitalWrite(_step_pin, HIGH);
        delay(100);
    };
    flow_pos -= 100;
};
