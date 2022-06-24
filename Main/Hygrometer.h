#include "Arduino.h"

const int sensorAirValues[] = {590, 130};
const int sensorWaterValues[] = {285, 90};

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
