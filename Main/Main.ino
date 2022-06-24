/*

*/
<<<<<<< HEAD
#include "Arduino.h"

=======
/*
>>>>>>> ca7584ba7c3efdd671d392383fb553f694981ad2
#include "Main.h"
#include "Hygrometer.h"
#include "Debug.h"
#include "Pump.h"
#include "PowerSaving.h"
#include "WaterLevelMeasurement.h"

int numberOfSensorPumpPairs = -1;
*/

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
      debug("Sensor not in soil");
      digitalWrite(2, HIGH);
      continue;
    }

    debug("Sensor in soil");
    digitalWrite(2, LOW);
    float waterlevel = getWaterLevelInLiter();
    float waterlevelPercentage = convertLiterToPercent(waterlevel);

    if(isWaterLevelCritical(waterlevelPercentage)){
      continue;
    }

    if(isWaterLevelLow(waterlevelPercentage)) {
        debug("Waterlevel low");
    }

    int soilMoisturePercent = getSensorValueInPercent(sensorValue,sensorIndex);

    int soilMoisturePercent = getSensorValueInPercent(sensorValue,sensorIndex);
    debug(String(soilMoisturePercent));
    while (needsWatering(soilMoisturePercent))
    {
     
      if (!isSensorInSoil(sensorValue, sensorIndex) || !isSensorConnected(sensorValue))
      {
         debug("Sensor not in soil");
        digitalWrite(2, HIGH);
        break;
      }
       digitalWrite(2, LOW);
      debug("Needs water");
      digitalWrite(3, HIGH);
      pump(2000, pumpAddress);
      sensorValue = getMoistureSensorValue(sensorAddress);
      soilMoisturePercent = getSensorValueInPercent(sensorValue,sensorIndex);
      debug(String(soilMoisturePercent));
    }

    debug("Done watering");
    digitalWrite(3, LOW);
  }
  //sleep(60);
}

bool needsWatering(float soilMoisturePercent)
{
  return soilMoisturePercent < HIGH_WATER_REQUIREMENTS;
}

