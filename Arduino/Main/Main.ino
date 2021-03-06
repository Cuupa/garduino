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
  // Serial.setTimeout(5000);

  debug(VERSION_NUMBER);

  numberOfIrrigationSystems = (sizeof(irrigationSystem.sensorPumpPair) / sizeof(*irrigationSystem.sensorPumpPair));
  debug(String(numberOfIrrigationSystems) + " Sensor-Pump pairs found");
  delay(10000);
}

void loop()
{
  String weatherData[] = {};
  debug("Requesting weather data");
  message.request(RequestType::WEATHER_DATA, weatherData);

  debug("Received weather data");
  debug(weatherData);

  bool isRaining = isCurrentlyRaining(weatherData);
  bool isRainfallSufficiant = isTodaysAndTomorrowsRainfallSufficient(weatherData);
  Evaporation evaporation = getEvaporation(weatherData);

  debug("It's raining: " + String(isRaining));
  debug("Rainfall is sufficiant: " + String(isRainfallSufficiant));

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

  int sleepTimeInMinutes = 60;
  if (evaporation == Evaporation::LOW_EVAPORATION)
  {
    // sleep 3h
    sleepTimeInMinutes = 180;
  }

  else if (evaporation == Evaporation::MEDIUM_EVAPORATION)
  {
    // sleep 1h
    sleepTimeInMinutes = 60;
  }

  else if (evaporation == Evaporation::HIGH_EVAPORATION)
  {
    // sleep 30 min
    sleepTimeInMinutes = 30;
  }
  sleep(sleepTimeInMinutes);
}

bool isHygrometerOkay(float sensorValue, int sensorIndex)
{
  if (!irrigationSystem.isSensorInSoil(sensorValue, sensorIndex))
  {
    message.sendNotification(HygrometerStatus::HYGROMETER_OUT_OF_SOIL, 0, sensorIndex);
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
    return true;
  }

  if (isWaterLevelCritical(waterlevelPercentage))
  {
    message.sendNotification(Waterlevel::WATER_LEVEL_CRITICAL, waterlevel);
    return false;
  }

  else if (isWaterLevelLow(waterlevelPercentage))
  {
    message.sendNotification(Waterlevel::WATER_LEVEL_LOW, waterlevel);
    return true;
  }

  else
  {
    message.sendNotification(Waterlevel::WATER_LEVEL_OK, waterlevel);
    return true;
  }
}

Evaporation getEvaporation(String weatherData[])
{
  if (weatherData[0].equals("ok"))
  {
    String evaporationString = weatherData[4];
    if (evaporationString == "low")
    {
      return Evaporation::LOW_EVAPORATION;
    }

    else if (evaporationString == 'medium')
    {
      return Evaporation::MEDIUM_EVAPORATION;
    }
    else if (evaporationString == 'high')
    {
      return Evaporation::HIGH_EVAPORATION;
    }
  }
  // No data. Returning medium as default
  return Evaporation::MEDIUM_EVAPORATION;
}