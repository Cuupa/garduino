#include "Arduino.h"

#define BAUD_RATE 9600
#define VERSION_NUMBER F("Garduino - 1.0")

/*
Set to true or false, if connected to a serial (eg raspberry pi, running the server.py)
The timeout defined in Main.ino::setup will step in, if it's set to true but the server is not present, 
but if you don't want to waste 5 seconds per run better set it to false
*/
const bool connectedToServer = false;


// key-value pairs of every sensor-pump-pair
const byte sensorPumpPair[][2] = {
    {A0, 13},
    {A1, 12}};
