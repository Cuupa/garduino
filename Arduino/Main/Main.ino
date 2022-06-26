/*

*/
#include "Message.h"
#include "Main.h"
#include "Hygrometer.h"
#include "Debug.h"
#include "Pump.h"
#include "PowerSaving.h"
#include "WaterLevelMeasurement.h"
#include "WaterRequirements.h"
#include "WaterDetermination.h"
#include "Placement.h"

int numberOfSensorPumpPairs = -1;

void setup()
{
  delay(1000);
  Serial.begin(BAUD_RATE);
  Serial.setTimeout(5000);

  debug(VERSION_NUMBER);

  numberOfSensorPumpPairs = (sizeof(sensorPumpPair) / sizeof(*sensorPumpPair));
  debug(String(numberOfSensorPumpPairs) + " Sensor-Pump pairs found");

  initPumps(sensorPumpPair, numberOfSensorPumpPairs);
}

void loop()
{
  String weatherData[] = {};
  request(RequestType::WEATHER_DATA, weatherData);

  bool isRaining = isCurrentlyRaining(weatherData);
  bool isRainfallSufficiant = isTodaysAndTomorrowsRainfallSufficient(weatherData);

  // iterate over every sensor
  for (int sensorIndex = 0; sensorIndex < numberOfSensorPumpPairs; sensorIndex++)
  {

    byte sensorAddress = sensorPumpPair[sensorIndex][0];
    byte pumpAddress = sensorPumpPair[sensorIndex][1];

    if (isPlantOutside(sensorIndex) && (isRaining || isRainfallSufficiant))
    {
      continue;
    }

    float sensorValue = getMoistureSensorValue(sensorAddress);

    if (!isHygrometerOkay(sensorValue, sensorIndex))
    {
      continue;
    }

    int soilMoisturePercent = getSensorValueInPercent(sensorValue, sensorIndex);
    sendNotification(HygrometerStatus::HYGROMETER_OK, soilMoisturePercent, sensorIndex);

    float waterlevel = getWaterLevelInLiter();
    float waterlevelPercentage = convertLiterToPercent(waterlevel);

    if (!isWaterLevelOkay(waterlevel, waterlevelPercentage))
    {
      continue;
    }

    float waterlevelBefore = waterlevel;
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

    float newWaterlevel = waterlevel;
    if (newWaterlevel < waterlevelBefore)
    {
      sendNotification(waterlevelBefore - newWaterlevel, sensorIndex);
    }
  }
  sleep(60);
}

bool isHygrometerOkay(float sensorValue, int sensorIndex)
{
  if (!isSensorInSoil(sensorValue, sensorIndex))
  {
    sendNotification(HygrometerStatus::HYGROMETER_OUT_OF_SOIL, sensorValue, sensorIndex);
    return false;
  }

  if (!isSensorConnected(sensorValue))
  {
    sendNotification(HygrometerStatus::HYGROMETER_NOT_CONNECTED, 0, sensorIndex);
    return false;
  }
  return true;
}

bool isWaterLevelOkay(float waterlevel, float waterlevelPercentage)
{
  if (isWaterLevelDisabled())
  {
    sendNotification(Waterlevel::WATER_LEVEL_DISABLED, waterlevel);
  }

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
