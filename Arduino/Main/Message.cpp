#include "Message.h"
#include "ArduinoJson.h"
#include "Main.h"

Message::Message()
{
}

void Message::sendNotification(Waterlevel level, float liter)
{
    StaticJsonDocument<128> doc;
    doc["system-name"] = system_name;
    doc["water-level"] = getWaterLevel(level);
    doc["liter"] = String(liter);
    doc["type"] = getMessageType(MessageType::MEASUREMENT);

    String out;
    serializeJson(doc, out);
    Serial.println(out);
}

void Message::sendNotification(HygrometerStatus status, float humidity, int sensorIndex)
{
    StaticJsonDocument<192> doc;
    doc["system-name"] = system_name;
    doc["hygrometer-status"] = getHygrometerStatus(status);
    doc["humidity"] = String(humidity);
    doc["sensor-index"] = String(sensorIndex);
    doc["type"] = getMessageType(MessageType::MEASUREMENT);
    String out;
    serializeJson(doc, out);
    Serial.println(out);
}

void Message::sendNotification(float wateredVolume, int sensorIndex)
{
    StaticJsonDocument<192> doc;
    doc["system-name"] = system_name;
    doc["watered-volume"] = wateredVolume;
    doc["sensor-index"] = String(sensorIndex);
    doc["type"] = getMessageType(MessageType::MEASUREMENT);
    String out;
    serializeJson(doc, out);
    Serial.println(out);
}

void Message::request(RequestType type, String response[])
{
    if (!connectedToServer)
    {
        response[0] = "error";
        return;
    }
    
    StaticJsonDocument<64> doc;
    doc["type"] = getMessageType(MessageType::REQUEST);
    doc["request"] = getRequestType(type);
    String out;
    serializeJson(doc, out);
    Serial.println(out);

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

String Message::getRequestType(RequestType type)
{
    if (type == RequestType::WEATHER_DATA)
    {
        return "weather-data";
    }
    return "-1";
}

String Message::getMessageType(MessageType type)
{
    if (type == MessageType::REQUEST)
    {
        return "request";
    }
    else if (type == MessageType::MEASUREMENT)
    {
        return "measurement";
    }
    return "-1";
}

String Message::getHygrometerStatus(HygrometerStatus status)
{
    if (status == HygrometerStatus::HYGROMETER_OK)
    {
        return "hygrometer-ok";
    }

    else if (status == HygrometerStatus::HYGROMETER_NOT_CONNECTED)
    {
        return "hygrometer-not-connected";
    }

    else if (status == HygrometerStatus::HYGROMETER_OUT_OF_SOIL)
    {
        return "hygrometer-out-of-soil";
    }

    return "-1";
}

String Message::getWaterLevel(Waterlevel level)
{
    if (level == Waterlevel::WATER_LEVEL_OK)
    {
        return "water-level-ok";
    }

    else if (level == Waterlevel::WATER_LEVEL_LOW)
    {
        return "water-level-low";
    }

    else if (level == Waterlevel::WATER_LEVEL_CRITICAL)
    {
        return "water-level-critical";
    }

    else if (level == Waterlevel::WATER_LEVEL_DISABLED)
    {
        return "water-level-disabled";
    }
    return "-1";
}
