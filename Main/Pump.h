#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

void initPumps(byte sensorPumpPair[][2], int numberOfSensorPumpPairs);

void pump(float ms, byte address);