#define BAUD_RATE 9600
#define VERSION_NUMBER F("Garduino - 1.0")

#include "Arduino.h"


// 1 hour pause before the next cycle
const unsigned long pause = 1000UL * 60UL * 60UL * 1UL;

// key-value pairs of every sensor-pump-pair
const byte sensorPumpPair[][2] = {
    {A0, 13}//,
    //{A1, 12}
    };
