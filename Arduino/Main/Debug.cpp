#include "Debug.h"

void debug(String msg)
{
    if (debug_mode)
    {
        Serial.println("Debug: " + msg);
    }
}

void debug(String msg[])
{
    if (debug_mode)
    {
        int length = (sizeof(msg) / sizeof(*msg));
        String value;
        for (int i = 0; i < length; i++)
        {
            value += msg[i];
        }

        debug(msg);
    }
}