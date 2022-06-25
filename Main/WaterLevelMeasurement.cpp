#include "Arduino.h"

#include "math.h"
#include "WaterLevelMeasurement.h"

void initWaterLevelSensor()
{
    if (enabled)
    {
        pinMode(pingPin, OUTPUT);
        pinMode(echoPin, INPUT);
    }
}

float getWaterLevelInLiter()
{
    if (enabled)
    {
        clear();
        ping();
        long duration = pulseIn(echoPin, HIGH);
        float distance = duration * 0.034 / 2;
        return calculateVolume(distance, tankHeight, tankVolume);
    }
    return -1.0;
}

float convertLiterToPercent(float volume)
{
    float a = tankVolume - volume;
    float b = (tankVolume + volume) / 2;
    return (a / b) * 100.0;
}

bool isWaterLevelLow(float waterlevel)
{
    if (enabled)
    {
        return waterlevel < 10 && waterlevel > 3;
    }
    return false;
}

bool isWaterLevelCritical(float waterlevel)
{
    if (enabled)
    {
        return waterlevel < 3;
    }
    return false;
}

bool isWaterLevelDisabled()
{
    return !enabled;
}

float calculateVolume(float distance, float height, float volume)
{
    float radius = tankDiameter / 2;
    float radiusSquared = radius * radius;
    return M_PI * radiusSquared * (distance / 10);
}

float calculateDiameter(float height, float volume)
{
    float a = volume / M_PI / (height / 10);
    float b = sqrt(a);
    return b * 2 * 10;
}

void ping()
{
    digitalWrite(pingPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(pingPin, LOW);
}

void clear()
{
    digitalWrite(pingPin, LOW);
    delayMicroseconds(2);
}
