#include "Debug.h"
#include "Hygrometer.h"

Hygrometer::Hygrometer(byte address)
{
  _address = address;
}

Hygrometer::Hygrometer(){
  _address = 0;
}

int Hygrometer::getSensorValueInPercent(int sensorValue, int airValue, int waterValue)
{
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
bool Hygrometer::isSensorInSoil(float sensorValue, int airValue)
{
  Serial.println(sensorValue);
  Serial.println(airValue);
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
  Serial.println(percent);
  return percent > 2.0;
}

bool Hygrometer::isSensorConnected(float sensorValue)
{
  return sensorValue > 50;
}

/*
  Reads the moisture sensor 100x and returning the average value
*/
float Hygrometer::getMoistureSensorValue()
{
  float sensorValue = 0;
  for (int i = 0; i <= 100; i++)
  {
    sensorValue += analogRead(_address);
    delay(1);
  }
  return sensorValue / 100.0;
}
