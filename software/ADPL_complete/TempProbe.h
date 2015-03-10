#ifndef TempProbe_h
#define TempProbe_h

#include "Arduino.h"

class TempProbe {
    public:
        TempProbe(int probePin);
        int readProbe(int probePin);
        float meanTemp(int samples);
    private:
        int _probePin;
}

#endif
