#include "Arduino.h"
#include "SensorPumpPair.h"
#include "HygrometerStatus.h"

enum Placement
{
    INSIDE,
    OUTSIDE,
    OUTSIDE_ROOFED
};

enum WaterRequirement
{
    LOW_WATER_REQUIREMENTS = 5,
    MEDIUM_WATER_REQUIREMENTS = 10,
    HIGH_WATER_REQUIREMENTS = 20
};

enum Evaporation
{
    LOW_EVAPORATION,
    MEDIUM_EVAPORATION,
    HIGH_EVAPORATION
};

class IrrigationSystem
{
public:
    IrrigationSystem();

    bool isPlantOutside(int irrigationLoopIndex);
    bool isPlantOutsideRoofed(int irrigationLoopIndex);
    bool needsWatering(int irrigationLoopIndex, float soilMoistureInPercent);
    float getMoistureSensorValue(int systemIndex);
    int getSensorValueInPercent(int sensorValue, int systemIndex);
    bool isSensorInSoil(float sensorValue, int systemIndex);
    bool isSensorConnected(float sensorValue);

    void pump(float ms, int systemIndex);

    void init();

    static const int numberOfIrrigationSystems = 2;

    SensorPumpPair sensorPumpPair[numberOfIrrigationSystems];

    Placement placements[numberOfIrrigationSystems] = {
        Placement::OUTSIDE_ROOFED,
        Placement::OUTSIDE_ROOFED};

    WaterRequirement waterRequirements[numberOfIrrigationSystems] = {
        WaterRequirement::HIGH_WATER_REQUIREMENTS,
        WaterRequirement::HIGH_WATER_REQUIREMENTS};

    //  key-value pairs of every sensor-pump-pair
    //  Each entry will be an irrigation system
    const byte addresses[numberOfIrrigationSystems][2] = {
        {A0, 11},
        {A1, 12}};

    int sensorAirValues[numberOfIrrigationSystems] = {590, 130};
    int sensorWaterValues[numberOfIrrigationSystems] = {285, 90};

};