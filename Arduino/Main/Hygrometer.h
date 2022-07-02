#include "Arduino.h"

class Hygrometer
{
public:
  Hygrometer(byte address);
  Hygrometer();

  int getSensorValueInPercent(int sensorValue, int airValue, int waterValue);
  bool isSensorInSoil(float sensorValue, int airValue);
  bool isSensorConnected(float sensorValue);
  float getMoistureSensorValue();

private:
  byte _address;
};