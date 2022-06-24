#include "Arduino.h"
#include "Notification.h"
#include "ArduinoJson.h"

void sendNotification(Waterlevel level, float liter)
{
    StaticJsonDocument<64> doc;
    doc["water-level"] = level;
    doc["liter"] = String(liter);
    serializeJson(doc, Serial);
}

void sendNotification(HygrometerStatus status, float humidity)
{
    StaticJsonDocument<64> doc;
    doc["hygrometer-status"] = status;
    doc["humidity"] = String(humidity);
    serializeJson(doc, Serial);
}