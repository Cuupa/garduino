#line 2 HygrometerTest.ino

#include "AUnit.h"
#include "Hygrometer.h"

test(correct){
    int sensorValue = 500;
    int sensorIndex = 0;
    bool result = isSensorInSoil(sensorValue, sensorIndex);

    assertTrue(result);
}

test(incorrect){
    int sensorValue = 600;
    int sensorIndex = 0;

    bool result = isSensorInSoil(sensorValue, sensorIndex);

    assertTrue(result);
}

void setup(){
    delay(1000); // wait for stability on some boards to prevent garbage Serial
    Serial.begin(115200); // ESP8266 default of 74880 not supported on Linux
    while (!Serial); // for the Arduino Leonardo/Micro only

    //TestRunner::setVerbosity(Verbosity::kAll);    
}

void loop(){
    TestRunner::run();
}