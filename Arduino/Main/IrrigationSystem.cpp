#include "IrrigationSystem.h"

IrrigationSystem::IrrigationSystem()
{
    for (int i = 0; i< numberOfIrrigationSystems; i++)
    {   
        SensorPumpPair pair(addresses[i][0], addresses[i][1]);
        sensorPumpPair[i] = pair;
    }
};

void IrrigationSystem::init()
{
    int numberOfPairs = (sizeof(sensorPumpPair) / sizeof(*sensorPumpPair));
    for (int i = 0; i < numberOfPairs; i++)
    {
        sensorPumpPair[i].init();
    }
}

bool IrrigationSystem::isPlantOutside(int irrigationLoopIndex)
{
    return placements[irrigationLoopIndex] == Placement::OUTSIDE;
};

bool IrrigationSystem::isPlantOutsideRoofed(int irrigationLoopIndex)
{
    return placements[irrigationLoopIndex] == Placement::OUTSIDE_ROOFED;
};

bool IrrigationSystem::needsWatering(int irrigationLoopIndex, float soilMoistureInPercent)
{
    return waterRequirements[irrigationLoopIndex] < soilMoistureInPercent;
}

 float IrrigationSystem::getMoistureSensorValue(int systemIndex)
 {
    sensorPumpPair[systemIndex].getMoistureSensorValue();
 }

 int IrrigationSystem::getSensorValueInPercent(int sensorValue, int systemIndex)
 {
    int airValue = sensorAirValues[systemIndex];
    int waterValue = sensorWaterValues[systemIndex];
    sensorPumpPair[systemIndex].getSensorValueInPercent(sensorValue, airValue, waterValue);
 }

bool IrrigationSystem::isSensorInSoil(float sensorValue, int systemIndex)
{
    int airValue = sensorAirValues[systemIndex];
    sensorPumpPair[systemIndex].isSensorInSoil(sensorValue, airValue);
}
bool IrrigationSystem::isSensorConnected(float sensorValue)
{
    return sensorValue > 50;
}

void IrrigationSystem::pump(float ms, int systemIndex)
{
    sensorPumpPair[systemIndex].pump(ms);
}