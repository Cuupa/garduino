#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "Hygrometer.h"

int getSensorValueInPercent(int sensorValue, int sensorIndex)
{
  int airValue = sensorAirValues[sensorIndex];
  int waterValue = sensorWaterValues[sensorIndex];
  int value = map(sensorValue, airValue, waterValue, 0, 100);

  if (value > 100)
  {
    return 100;
  }
  else if (value < 0)
  {
    return 0;
  }
  return value;
}

/*
The sensor value
*/
bool isSensorInSoil(float sensorValue, int sensorIndex)
{

  int airValue = sensorAirValues[sensorIndex];

  float percent = 0;
  if (airValue > sensorValue)
  {
    float a = airValue - sensorValue;
    float b = (airValue + sensorValue) / 2;
    percent = (a / b) * 100.0;
  }
  else
  {
    int a = sensorValue - airValue;
    int b = (sensorValue + airValue) / 2;
    percent = (a / b) * 100;
  }

  return percent > 10.0;
}

bool isSensorConnected(float sensorValue)
{
  return sensorValue > 50;
}

/*
  Reads the moisture sensor 100x and returning the average value
*/
float getMoistureSensorValue(byte sensorPin)
{
  float sensorValue = 0;
  for (int i = 0; i <= 100; i++)
  {
    sensorValue += analogRead(sensorPin);
    delay(1);
  }
  return sensorValue / 100.0;
}
