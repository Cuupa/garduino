#include "Arduino.h"
#include "HygrometerStatus.h"

enum Waterlevel
{
    WATER_LEVEL_OK,
    WATER_LEVEL_LOW,
    WATER_LEVEL_CRITICAL,
    WATER_LEVEL_DISABLED
};

enum MessageType
{
    MEASUREMENT,
    REQUEST
};

enum RequestType
{
    WEATHER_DATA
};

class Message
{
public:
    Message();

    void sendNotification(Waterlevel level, float liter);
    void sendNotification(HygrometerStatus status, float humidty, int sensorIndex);
    void sendNotification(float wateredVolume, int sensorIndex);

    void request(RequestType type, String response[]);

private:
    const String system_name = "Node1";
    const String status = "status";
    const String time = "time";
    const String temperature = "temperature";
    const String condition = "condition";
    const String evaporation = "evaporation";
    const String rainfallToday = "rainfall-today";
    const String rainfallTomorrow = "rainfall-tomorrow";

    String Message::getRequestType(RequestType type);
    String Message::getMessageType(MessageType type);
    String Message::getHygrometerStatus(HygrometerStatus status);
};
