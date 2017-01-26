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
    int flow_pos = 0;
    particle.Variable("flow_pos", (int) flow_pos);
    bool up = false;
    bool down = false;
}

void PinchValve::read(){
    // closed switch, pulls down the level, reads this pin
    if (digitalRead(_up_pin) == LOW){
        up = true;
    }
    if (digitalRead(_down_pin) == LOW){
        down = true;
    }
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
    flow_pos +=400;
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
    flow_pos -= 400;
    down = false;
    digitalWrite(_sleep_pin, LOW);
};
