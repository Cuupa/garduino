#include "Placement.h"

bool isPlantOutside(int sensorIndex)
{
    return sensorPlacement[sensorIndex][1] == outside;
}

bool isPlantOutsideRoofed(int sensorIndex)
{
    return sensorPlacement[sensorIndex][1] == outside_roofed;
}