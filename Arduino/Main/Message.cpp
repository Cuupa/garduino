#include "Message.h"
#include "ArduinoJson.h"

const String status = "status";
const String time = "time";
const String temperature = "temperature";
const String condition = "condition";
const String evaporation = "evaporation";
const String rainfallToday = "rainfall-today";
const String rainfallTomorrow = "rainfall-tomorrow";

void sendNotification(Waterlevel level, float liter)
{
    StaticJsonDocument<69> doc;
    doc["water-level"] = level;
    doc["liter"] = String(liter);
    doc["type"] = NotificationType::MEASUREMENT;
    serializeJson(doc, Serial);
}

void sendNotification(HygrometerStatus status, float humidity, int sensorIndex)
{
    StaticJsonDocument<192> doc;
    doc["hygrometer-status"] = status;
    doc["humidity"] = String(humidity);
    doc["sensor-index"] = String(sensorIndex);
    doc["type"] = NotificationType::MEASUREMENT;
    serializeJson(doc, Serial);
}

void sendNotification(float wateredVolume, int sensorIndex)
{
    StaticJsonDocument<192> doc;
    doc["watered-volume"] = wateredVolume;
    doc["sensor-index"] = String(sensorIndex);
    doc["type"] = NotificationType::MEASUREMENT;
    serializeJson(doc, Serial);
}

void request(RequestType type, String response[])
{
    
    StaticJsonDocument<64> doc;
    doc["type"] = NotificationType::REQUEST;
    doc["request"] = type;
    serializeJson(doc, Serial);

    String responseJson = Serial.readStringUntil('\n');

    StaticJsonDocument<192> responseDoc;
    DeserializationError error = deserializeJson(responseDoc, responseJson);
    
    if (error)
    {
        response[0] = "error";
    }
    response[0] = "ok";
    response[1] = responseDoc[time].as<String>();
    response[2] = responseDoc[temperature].as<String>();
    response[3] = responseDoc[condition].as<String>();
    response[4] = responseDoc[evaporation].as<String>();
    response[5] = responseDoc[rainfallToday].as<String>();
    response[6] = responseDoc[rainfallTomorrow].as<String>();
}