/*

*/

#include "Main.h"
#include "Hygrometer.h"
#include "Debug.h"
#include "Pump.h"
#include "PowerSaving.h"

#if defined(ARDUINO) && ARDUINO >= 100      // #  https://github.com/adafruit/DHT-sensor-library/issues/1   changed 2015 Feb 21
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

int numberOfSensorPumpPairs = -1;


/*
70% for high water requirements
*/
const int HIGH_WATER_REQUIREMENTS = 70;

/*
50% for medium water requirements
*/
const int MEDIUM_WATER_REQUIREMENTS = 50;

/*
30% for low water requirements
*/
const int LOW_WATER_REQUIREMENTS = 30;

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

    if (!isSensorInSoil(sensorValue, sensorIndex) || !isSensorConnected(sensorValue))
    {
      continue;
    }

    int soilMoisturePercent = getSensorValueInPercent(sensorValue,sensorIndex);

    while (needsWatering(soilMoisturePercent))
    {
      if (!isSensorInSoil(sensorValue, sensorIndex) || !isSensorConnected(sensorValue))
      {
        continue;
      }
      pump(2000, pumpAddress);
      sensorValue = getMoistureSensorValue(sensorAddress);
    }
  }
  sleep(60);
}


bool needsWatering(float soilMoisturePercent)
{
  return soilMoisturePercent > HIGH_WATER_REQUIREMENTS;
}
