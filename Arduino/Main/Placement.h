#include "Arduino.h"

const byte inside = 0;
const byte outside = 1;
const byte outside_roofed = 2;

const byte sensorPlacement[][2] = {
    {A0, 2},
    {A1, 2}};


bool isPlantOutside(int sensorIndex);
bool isPlantOutsideRoofed(int sensorIndex);