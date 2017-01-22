#ifndef ADPL_PINCHVALVE_H
#define ADPL_PINCHVALVE_H

class PinchValve{

    public:
        PinchValve(int dir_pin, int step_pin, int sleep_pin);
        void shiftOpen();
        void shiftClose();
        int flow_pos;
    private:
        _dir_pin;
        _step_pin;
        _sleep_pin;
};

#endif //ADPL_PINCHVALVE_H
