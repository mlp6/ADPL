#ifndef ADPL_PINCHVALVE_H
#define ADPL_PINCHVALVE_H

class PinchValve{

    public:
        PinchValve(int dir_pin, int step_pin, int sleep_pin, int up_pin, int down_pin, int reset_pin);
        void shiftUp(double res);
        void shiftDown(double res);
        volatile bool up;
        volatile bool down;
        volatile double position;
        double resolution;

    private:
        int _dir_pin;
        int _step_pin;
        int _sleep_pin;
        static constexpr float _DELAY = 1.0;
        int turn_count;

};

#endif //ADPL_PINCHVALVE_H
