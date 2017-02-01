#ifndef ADPL_PINCHVALVE_H
#define ADPL_PINCHVALVE_H

class PinchValve{

    public:
        PinchValve(int dir_pin, int step_pin, int sleep_pin, int up_pin, int down_pin);
        void shiftUp();
        void shiftDown();
        bool up;
        bool down;
        double flow_rate;

    private:
        int _dir_pin;
        int _step_pin;
        int _sleep_pin;
        int _up_pin;
        int _down_pin;
};

#endif //ADPL_PINCHVALVE_H
