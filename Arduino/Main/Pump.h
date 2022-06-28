#include "Arduino.h"

class Pump
{
    public:
        Pump(byte address);
        Pump();
        void init();
        void pump(float ms);
    private:
        byte _address;
};
