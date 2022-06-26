#include "Debug.h"

/*
  Will set every pump to OUTPUT
*/
void initPumps(byte sensorPumpPair[][2], int numberOfSensorPumpPairs)
{
  for (int sensorIndex = 0; sensorIndex < numberOfSensorPumpPairs; sensorIndex++)
  {
    byte pumpAddress = sensorPumpPair[sensorIndex][1];
    debug("Output on address '" + String(pumpAddress) + "' configured");
    pinMode(pumpAddress, OUTPUT);
    // The relayboard (P2H15880) is off on HIGH
    digitalWrite(pumpAddress, HIGH);
  }
}


/*
  Activates the pump with the given address for the given ammount of time
*/
void pump(float ms, byte address)
{
  digitalWrite(address, LOW);
  delay(ms);
  digitalWrite(address, HIGH);
}