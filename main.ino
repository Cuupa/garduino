/*


*/
// 1 hour pause before the next cycle
const unsigned long pause = 1000UL * 60UL * 60UL * 1UL;

int numberOfSensorPumpPairs = -1;

// key-value pairs of every sensor-pump-pair
const byte sensorPumpPair[][2] = {
    {A0, 13},
    {A1, 12}};

const int sensorAirValues[] = {600, 130};
const int sensorWaterValues[] = {290, 90};

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
  initMotors();
  numberOfSensorPumpPairs = (sizeof(sensorPumpPair) / sizeof(*sensorPumpPair));
}

void loop()
{
  // iterate over every sensor
  for (int sensorIndex = 0; sensorIndex < numberOfSensorPumpPairs; sensorIndex++)
  {

    byte sensorAddress = sensorPumpPair[sensorIndex][0];
    byte pumpAddress = sensorPumpPair[sensorIndex][1];
    int airValue = sensorAirValues[sensorIndex];
    int waterValue = sensorWaterValues[sensorIndex];

    float sensorValue = getMoistureSensorValue(sensorAddress);

    if (!isSensorInSoil(sensorValue, airValue) || !isSensorConnected(sensorValue))
    {
      continue;
    }

    int soilMoisturePercent = getSensorValueInPercent(sensorValue, airValue, waterValue);

    while (needsWatering(soilMoisturePercent))
    {
      if (!isSensorInSoil(sensorValue, airValue) || !isSensorConnected(sensorValue))
      {
        continue;
      }
      pump(2000, pumpAddress);
      sensorValue = getMoistureSensorValue(sensorAddress);
    }
  }
  delay(pause);
}

/*
  Will set every Motors to OUTPUT
*/
void initMotors()
{
  for (int sensorIndex = 0; sensorIndex < numberOfSensorPumpPairs; sensorIndex++)
  {
    byte pumpAddress = sensorPumpPair[sensorIndex][1];
    pinMode(pumpAddress, OUTPUT);
    // The relayboard (P2H15880) is off on HIGH
    digitalWrite(pumpAddress, HIGH);
  }
}

int getSensorValueInPercent(int sensorValue, int airValue, int waterValue)
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

bool needsWatering(float soilMoisturePercent)
{
  return soilMoisturePercent > HIGH_WATER_REQUIREMENTS;
}

/*
The sensor value
*/
bool isSensorInSoil(float sensorValue, float airValue)
{
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
  Activates the pump with the given address for the given ammount of time
*/
void pump(float ms, byte pump)
{
  digitalWrite(pump, LOW);
  delay(ms);
  digitalWrite(pump, HIGH);
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
