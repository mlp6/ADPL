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
        int clogCounting;
    private:
        int _dir_pin;
        int _step_pin;
        int _sleep_pin;
        static constexpr float _DELAY = 500;
        static constexpr int _MICROSTEP = 8; // Easy Driver has microstepping feature default of 8, could be adjusted with M1, M2, for now constant
        static constexpr int _STEPSPERMM = 800; // in this motor case 2mm per revolution, microstep of 8, 200 full steps per rotation, so 1600 for 2mm
        int turn_count;

};

#endif //ADPL_PINCHVALVE_H
