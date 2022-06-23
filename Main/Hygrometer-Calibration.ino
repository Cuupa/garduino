/*
This program helps calibrating the moisture sensors. 
First it measures the value of the sensor, when it's exposed to air (0% moist), then waits 5 seconds for you to put it into a glass of water, 
then it measures the value of the sensor when it's submerged (100% moist).

It then prints out these values.

These two values are needed for the main.ino to determine the correct moisture percentage.
*/


#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif


/*
Change this value to your analog input port you want to use
*/
const byte sensorAddress = A0;

/*
void setup()
{
    Serial.begin(9600);
    Serial.println("Begin Calibration");
    Serial.println("Keep sensor out of any substrate or water");
    delay(1000);
    Serial.println("Reading air-value of sensor ...");
    float airValue = getMoistureSensorValue(sensorAddress);
    Serial.println("... done");
    Serial.println("Put your sensor into a glass of water (waiting 5 seconds)");
    delay(5000);
    Serial.println("Reading water-value of sensor ...");
    float waterValue = getMoistureSensorValue(sensorAddress);
    Serial.println("... done");
    Serial.println("Air value: " + String(airValue));
    Serial.println("Water value: " + String(waterValue));
    Serial.println("exiting");
}

void loop()
{
}

/*
  Reads the moisture sensor 1000x and returning the average value
*/
/*
float getMoistureSensorValue(byte sensorPin)
{
    float sensorValue = 0;
    for (int i = 0; i <= 1000; i++)
    {
        sensorValue += analogRead(sensorPin);
        delay(1);
    }
    return sensorValue / 1000.0;
}
*/