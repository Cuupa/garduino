# Disclaimer
This is a personal project done in my spare time. Use at your own risk.
The amazon links in the hardware section are affiliate links. You still pay the price you normally would, but I get a small commision if you buy something.

:warning: - Pay close attention

# Hardware
## Raspberry Pi 
I use an old Pi 2 as a gateway to fetch weather data.
[raspberrypi.com](https://www.raspberrypi.com/products/raspberry-pi-4-model-b/)

## Arduino Uno R3
This is a board for prototyping. It'll be replaces by a arduino micro, when the hardware will be installed on location.

[store.arduino.cc](https://store.arduino.cc/products/arduino-uno-rev3)

## Arduino Micro
Board used for production.
[store.arduino.cc](https://store.arduino.cc/products/arduino-micro)

## Elegoo electronic set 
Be aware you need a seperate 9V power supply if you don't want to power the components via the arduino.

[amazon](https://amzn.to/39TeAqW)

## WayinTop 3 3V-5V pumps :warning:
Not the best pumps, but rather cheap and used for prototyping.
They don't seem have insulation to protect the electronics from the water, so use at your own risk.
You'd better buy a more expensive one, but I didn't know better and wanted some cheap pumps for prototyping.
(source: https://youtu.be/GNNzLwCidCE?t=250)

[amazon](https://amzn.to/3OmWYmE)

## Elegoo 4 channel 5V DC relay
Be aware that LOW seems to activate the relay instead of HIGH. The code is written that way. If you use a different relay, you might want to change that. 

Also see [Changes you probably have to make - Relay](#relay)

[amazon](https://amzn.to/3NtVxl7)

## KeeYees capacitive hygrometers 
[amazon](https://amzn.to/3OEAihn)


# Software
The software in this git repository is as of now a simple prototype and is under active development.

If you want to use the software for yourself feel free to change it to match your needs.

## Dependencies
- AUnit [documentation](https://www.arduino.cc/reference/en/libraries/aunit/)
- LowPower [documentation](https://github.com/rocketscream/Low-Power)
- ArduinoJson [documentation](https://arduinojson.org/)
## Changes you probably have to make


### Hygrometers and pumps
In ```Main.h``` theres a section which probably looks like this:

``` C++
const byte sensorPumpPair[][2] = {
    {A0, 13},
    {A1, 12}
};
```

The first value of these key-value-pairs is the analog pin which the sensor is connected to, the second value the digital pin, for the pump.

Sensor on ```A0``` activates the pump on pin ```13``` if necessary, sensor on A1 acitvates pump on pin ```12```.

### Relay
In ```Pump.cpp``` there's an ```initPumps```-Function. The relay I use seems to be off on ```HIGH``` and on on ```LOW```. Depending on the relay you use, you might want to change that.

### Hygrometer calibration
Depending on your assembly the hygrometers can return different values.
During my prototyping sensor 1 returned a value of around 600, while sensor 2 returned a value around 130 while both being exposed to air.

I already provided a simple program for finding the base values of the sensor.

Open the ```Hygrometer-Calibration.ino``` and change the analog input to the pin you connected the hygrometer to. Or just leave it and connect the sensor to A0 :D

When you start the program, it first measures the "air-value" (so leave the sensor lying on your table or something). Then you have a 5 second pause to place the sensor in a glass of water. It then measures the "water-value" and logs it to the serial console.

Copy these two values and change the two lines ```Hygrometer.h``` accordingly.

``` C++
const int sensorAirValues[] = {600, 130};
const int sensorWaterValues[] = {290, 90};
```

In this example I have two sensors. Sensor 1 has an air-value of 600 and a water-value of 290, while sensor 2 has 130 and 90 accordingly.

Repeat these steps for every sensor you have.


### Water level measurement

### Weather data
