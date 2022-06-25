#include "WaterRequirements.h"
#include "Arduino.h"

bool needsWatering(float soilMoisturePercent, int requirement)
{
  return soilMoisturePercent < requirement;
}
