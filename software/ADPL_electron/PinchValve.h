#ifndef ADPL_PINCHVALVE_H
#define ADPL_PINCHVALVE_H

class PinchValve{

    public:
        PinchValve(int dir_pin, int step_pin, int sleep_pin);
        void shiftUp();
        void shiftDown();
        bool up;
        bool down;

    private:
        int _dir_pin;
        int _step_pin;
        int _sleep_pin;
        static constexpr _QUARTERTURN int 400;
        static constexpr _DELAY float 1.0;

};

#endif //ADPL_PINCHVALVE_H
