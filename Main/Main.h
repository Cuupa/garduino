#define BAUD_RATE 9600
#define DEBUG_MODE
#define VERSION_NUMBER F("Garduino - 1.0")

#if defined(ARDUINO) && ARDUINO >= 100 
#include "Arduino.h"
#else
#include "WProgram.h"
#endif


// 1 hour pause before the next cycle
const unsigned long pause = 1000UL * 60UL * 60UL * 1UL;

// key-value pairs of every sensor-pump-pair
const byte sensorPumpPair[][2] = {
    {A0, 13},
    {A1, 12}};
