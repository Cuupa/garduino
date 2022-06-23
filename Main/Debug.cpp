#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif


void debug(String msg)
{
    #ifdef DEBUG_MODE
    Serial.println(msg);
    #endif
}