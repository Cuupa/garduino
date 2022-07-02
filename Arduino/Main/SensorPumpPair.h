#include "Hygrometer.h"
#include "Pump.h"
#include "Arduino.h"

class SensorPumpPair
{
    public:
        SensorPumpPair(int sensorAddress, int pumpAddress);
        SensorPumpPair();
        void init();

        int getSensorValueInPercent(int sensorValue, int airValue, int waterValue);
        float getMoistureSensorValue();
        bool isSensorInSoil(float sensorValue, int airValue);
        void pump(float ms);


    private:
        Hygrometer _hygrometer;
        Pump _pump;
};