/*

*/
#include "Message.h"
#include "Main.h"
#include "Debug.h"
#include "PowerSaving.h"
#include "WaterLevelMeasurement.h"
#include "WaterDetermination.h"
#include "IrrigationSystem.h"

int numberOfIrrigationSystems = -1;

IrrigationSystem irrigationSystem;
Message message;

void setup()
{
  delay(1000);
  Serial.begin(BAUD_RATE);
  Serial.setTimeout(5000);

  debug(VERSION_NUMBER);

  numberOfIrrigationSystems = (sizeof(irrigationSystem.sensorPumpPair) / sizeof(*irrigationSystem.sensorPumpPair));
  debug(String(numberOfIrrigationSystems) + " Sensor-Pump pairs found");
  delay(10000)
}

void loop()
{
  String weatherData[] = {};
  message.request(RequestType::WEATHER_DATA, weatherData);

  bool isRaining = isCurrentlyRaining(weatherData);
  bool isRainfallSufficiant = isTodaysAndTomorrowsRainfallSufficient(weatherData);

  // iterate over every sensor
  for (int systemIndex = 0; systemIndex < numberOfIrrigationSystems; systemIndex++)
  {
    if (irrigationSystem.isPlantOutside(systemIndex) && (isRaining || isRainfallSufficiant))
    {
      continue;
    }

    float sensorValue = irrigationSystem.getMoistureSensorValue(systemIndex);

    if (!isHygrometerOkay(sensorValue, systemIndex))
    {
      continue;
    }

    int soilMoisturePercent = irrigationSystem.getSensorValueInPercent(sensorValue, systemIndex);
    message.sendNotification(HygrometerStatus::HYGROMETER_OK, soilMoisturePercent, systemIndex);

    float waterlevel = getWaterLevelInLiter();
    float waterlevelPercentage = convertLiterToPercent(waterlevel);

    if (!isWaterLevelOkay(waterlevel, waterlevelPercentage))
    {
      continue;
    }

    float waterlevelBefore = waterlevel;
    while (irrigationSystem.needsWatering(systemIndex, soilMoisturePercent))
    {
      waterlevel = getWaterLevelInLiter();
      waterlevelPercentage = convertLiterToPercent(waterlevel);

      if (!isHygrometerOkay(sensorValue, systemIndex))
      {
        break;
      }

      if (!isWaterLevelOkay(waterlevel, waterlevelPercentage))
      {
        break;
      }

      irrigationSystem.pump(2000, systemIndex);
      sensorValue = irrigationSystem.getMoistureSensorValue(systemIndex);
      soilMoisturePercent = irrigationSystem.getSensorValueInPercent(sensorValue, systemIndex);
    }

    float newWaterlevel = waterlevel;
    if (newWaterlevel < waterlevelBefore)
    {
      message.sendNotification(waterlevelBefore - newWaterlevel, systemIndex);
    }
  }
  sleep(60);
}

bool isHygrometerOkay(float sensorValue, int sensorIndex)
{
  if (!irrigationSystem.isSensorInSoil(sensorValue, sensorIndex))
  {
    message.sendNotification(HygrometerStatus::HYGROMETER_OUT_OF_SOIL, sensorValue, sensorIndex);
    return false;
  }

  if (!irrigationSystem.isSensorConnected(sensorValue))
  {
    message.sendNotification(HygrometerStatus::HYGROMETER_NOT_CONNECTED, 0, sensorIndex);
    return false;
  }
  return true;
}

bool isWaterLevelOkay(float waterlevel, float waterlevelPercentage)
{
  if (isWaterLevelDisabled())
  {
    message.sendNotification(Waterlevel::WATER_LEVEL_DISABLED, waterlevel);
  }

  if (isWaterLevelCritical(waterlevelPercentage))
  {
    message.sendNotification(Waterlevel::WATER_LEVEL_CRITICAL, waterlevel);
    return false;
  }

  else if (isWaterLevelLow(waterlevelPercentage))
  {
    message.sendNotification(Waterlevel::WATER_LEVEL_LOW, waterlevel);
  }

  else
  {
    message.sendNotification(Waterlevel::WATER_LEVEL_OK, waterlevel);
  }

  return true;
}
