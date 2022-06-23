#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

const int sensorAirValues[] = {600, 130};
const int sensorWaterValues[] = {290, 90};

int getSensorValueInPercent(int sensorValue, int sensorIndex);

/*
The sensor value
*/
bool isSensorInSoil(float sensorValue, int sensorIndex);

bool isSensorConnected(float sensorValue);

/*
  Reads the moisture sensor 100x and returning the average value
*/
float getMoistureSensorValue(byte sensorPin);
