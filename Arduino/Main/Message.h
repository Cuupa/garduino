#include "Arduino.h"

enum Waterlevel
{
    WATER_LEVEL_OK,
    WATER_LEVEL_LOW,
    WATER_LEVEL_CRITICAL,
    WATER_LEVEL_DISABLED
};

enum HygrometerStatus
{
    HYGROMETER_OK,
    HYGROMETER_OUT_OF_SOIL,
    HYGROMETER_NOT_CONNECTED
};

enum NotificationType
{
    MEASUREMENT,
    REQUEST
};

enum RequestType
{
    WEATHER_DATA
};

void sendNotification(Waterlevel level, float liter);
void sendNotification(HygrometerStatus status, float humidty, int sensorIndex);
void sendNotification(float wateredVolume, int sensorIndex);

void request(RequestType type, String response[]);