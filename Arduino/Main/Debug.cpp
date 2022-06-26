#include "Debug.h"

void debug(String msg)
{
    if (debug_mode)
    {
        Serial.println(msg);
    }
}