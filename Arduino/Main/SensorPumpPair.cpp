#include "Arduino.h"
#include "SensorPumpPair.h"

SensorPumpPair::SensorPumpPair(int sensorAddress, int pumpAddress)
{
    Hygrometer meter(sensorAddress);
    _hygrometer = meter;

    Pump pump(pumpAddress);
    _pump = pump;
};

SensorPumpPair::SensorPumpPair()
{
};

void SensorPumpPair::init()
{
    _pump.init();
}

float SensorPumpPair::getMoistureSensorValue()
{
    return _hygrometer.getMoistureSensorValue();
}

bool SensorPumpPair::isSensorInSoil(float sensorValue, int airValue)
{
    return _hygrometer.isSensorInSoil(sensorValue, airValue);
}

int SensorPumpPair::getSensorValueInPercent(int sensorValue, int airValue, int waterValue)
{
    return _hygrometer.getSensorValueInPercent(sensorValue, airValue, waterValue);
}

void SensorPumpPair::pump(float ms)
{
    return _pump.pump(ms);
}

