#include "WaterRequirements.h"

bool needsWatering(float soilMoisturePercent, int requirement)
{
  return soilMoisturePercent < requirement;
}
