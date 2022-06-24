#include "Arduino.h"

enum Waterlevel
{
    WATER_LEVEL_OK,
    WATER_LEVEL_LOW,
    WATER_LEVEL_CRITICAL
};
enum HygrometerStatus
{
    OK,
    OUT_OF_SOIL,
    NOT_CONNECTED
};

void sendNotification(Waterlevel level, float liter);
void sendNotification(HygrometerStatus status, float humidty);