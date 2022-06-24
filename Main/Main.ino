/*

*/
#include "Arduino.h"
#include "Notification.h"
#include "Main.h"
#include "Hygrometer.h"
#include "Debug.h"
#include "Pump.h"
#include "PowerSaving.h"
#include "WaterLevelMeasurement.h"
#include "WaterRequirements.h"

int numberOfSensorPumpPairs = -1;

void setup()
{
  delay(1000);
  Serial.begin(BAUD_RATE);
  numberOfSensorPumpPairs = (sizeof(sensorPumpPair) / sizeof(*sensorPumpPair));
  debug(VERSION_NUMBER);
  debug(String(numberOfSensorPumpPairs) + " Sensor-Pump pairs found");
  initPumps(sensorPumpPair, numberOfSensorPumpPairs);
}

void loop()
{
  // iterate over every sensor
  for (int sensorIndex = 0; sensorIndex < numberOfSensorPumpPairs; sensorIndex++)
  {

    byte sensorAddress = sensorPumpPair[sensorIndex][0];
    byte pumpAddress = sensorPumpPair[sensorIndex][1];

    float sensorValue = getMoistureSensorValue(sensorAddress);

    if (!isHygrometerOkay(sensorValue, sensorIndex))
    {
      continue;
    }

    int soilMoisturePercent = getSensorValueInPercent(sensorValue, sensorIndex);
    sendNotification(HygrometerStatus::OK, soilMoisturePercent);

    float waterlevel = getWaterLevelInLiter();
    float waterlevelPercentage = convertLiterToPercent(waterlevel);

    if (!isWaterLevelOkay(waterlevel, waterlevelPercentage))
    {
      continue;
    }

    while (needsWatering(soilMoisturePercent, HIGH_WATER_REQUIREMENTS))
    {
      waterlevel = getWaterLevelInLiter();
      waterlevelPercentage = convertLiterToPercent(waterlevel);

      if (!isHygrometerOkay(sensorValue, sensorIndex))
      {
        break;
      }

      if (!isWaterLevelOkay(waterlevel, waterlevelPercentage))
      {
        break;
      }

      pump(2000, pumpAddress);
      sensorValue = getMoistureSensorValue(sensorAddress);
      soilMoisturePercent = getSensorValueInPercent(sensorValue, sensorIndex);
    }
  }
  sleep(60);
}

bool isHygrometerOkay(float sensorValue, int sensorIndex)
{
  if (!isSensorInSoil(sensorValue, sensorIndex))
  {
    sendNotification(HygrometerStatus::OUT_OF_SOIL, 0);
    return false;
  }

  if (!isSensorConnected(sensorValue))
  {
    sendNotification(HygrometerStatus::NOT_CONNECTED, 0);
    return false;
  }
  return true;
}

bool isWaterLevelOkay(float waterlevel, float waterlevelPercentage)
{
  if (isWaterLevelCritical(waterlevelPercentage))
  {
    sendNotification(Waterlevel::WATER_LEVEL_CRITICAL, waterlevel);
    return false;
  }

  else if (isWaterLevelLow(waterlevelPercentage))
  {
    sendNotification(Waterlevel::WATER_LEVEL_LOW, waterlevel);
  }

  else
  {
    sendNotification(Waterlevel::WATER_LEVEL_OK, waterlevel);
  }

  return true;
}