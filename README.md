# Disclaimer
This is a personal project done in my spare time. Use at your own risk.
The amazon links in the hardware section are affiliate links. You still pay the price you normally would, but I get a small commision if you buy something.

:warning: - Pay close attention

:construction: - Under construction. May change frequently and is not stable

# Goal
Since we don't have a power outlet on our balcony my requirements are that the system will work with a 5V DC Solar panel.

# Hardware
## Raspberry Pi 
I use an Raspberry Pi Zero W 1.1 as a gateway to fetch weather data.

[raspberrypi.com](https://www.raspberrypi.com/products/raspberry-pi-zero/)

## Arduino Micro
Board used for production. The micro will be soldered to the [PCB](#pcb) and will handle the sensor data and pumps.

[store.arduino.cc](https://store.arduino.cc/products/arduino-micro)

## WayinTop 3 3V-5V pumps :warning:
Not the best pumps, but rather cheap and used for prototyping.
They don't seem have insulation to protect the electronics from the water, so use at your own risk.
You'd better buy a more expensive one, but I didn't know better and wanted some cheap pumps for prototyping.

(source: https://youtu.be/GNNzLwCidCE?t=250)

[amazon](https://amzn.to/3OmWYmE)

## 5V DC submergeable pump
I use this pump as a more reliable pump to water the plants

[amazon](https://amzn.to/3o1D1Gq)

## Elegoo 4 channel 5V DC relay
Be aware that LOW seems to activate the relay instead of HIGH. The code is written that way. If you use a different relay, you might want to change that. 

Also see [Changes you probably have to make - Relay](#relay)

[amazon](https://amzn.to/3NtVxl7)

## KeeYees capacitive hygrometers 
[amazon](https://amzn.to/3OEAihn)

## Waterproof case
If you use an arduino uno and a standard sized raspberry pi like a Pi 4 this case is definitely to small.
[amazon](https://amzn.to/3IxMej0)

## Cables
## USB-extension for the pump.
Since the pump uses 5V DC over USB and the controller is somewhat far away of the water reservoir and the pump, I use these as an extension cord.

[amazon](https://amzn.to/3P59U0J)

## PCB
I designed the PCB using [easyeda](https://easyeda.com).

The final circuit board looks something like this:

<img src="https://raw.githubusercontent.com/Cuupa/garduino/main/docs/PCB/PCB.svg" width="300%" height="300">

All files including Gerber files can be found here: [docs/PCB/](docs/PCB/)


# Software
The software in this git repository is as of now a simple prototype and is under active development.

If you want to use the software for yourself feel free to change it to match your needs.

## Dependencies
- AUnit [documentation](https://www.arduino.cc/reference/en/libraries/aunit/)
- LowPower [documentation](https://github.com/rocketscream/Low-Power)
- ArduinoJson [documentation](https://arduinojson.org/)
## Changes you probably have to make


### Hygrometers and pumps
In ```IrrigationSystem.h``` theres a section which probably looks like this:

``` C++
    static const int numberOfIrrigationSystems = 2;

    SensorPumpPair sensorPumpPair[numberOfIrrigationSystems];

    Placement placements[numberOfIrrigationSystems] = {
        Placement::OUTSIDE_ROOFED,
        Placement::INSIDE};

    WaterRequirement waterRequirements[numberOfIrrigationSystems] = {
        WaterRequirement::HIGH_WATER_REQUIREMENTS,
        WaterRequirement::MEDIUM_WATER_REQUIREMENTS};

    //  key-value pairs of every sensor-pump-pair
    //  Each entry will be an irrigation system
    const byte addresses[numberOfIrrigationSystems][2] = {
        {A0, 11},
        {A1, 12}};

    int sensorAirValues[numberOfIrrigationSystems] = {590, 130};
    int sensorWaterValues[numberOfIrrigationSystems] = {285, 90};
```
The first line is the number of irrigation systems. Unfortunately, I'm a java, not a c++ dev, so I did not manage to find a more elegant solution... yet

Change the number to the number of your irrigation systems, watering loops whatever you call it. Following, there's where you placed your plants and then, what the water requirements are. 

Then, there's the addresses of your sensor-pump-pairs.

The first value of these key-value-pairs is the analog pin which the sensor is connected to, the second value the digital pin, for the pump.

Sensor on ```A0``` activates the pump on pin ```11``` if necessary, sensor on ```A1``` acitvates pump on pin ```12``` and so on. 

The three fields ```placement```, ```waterRequirements``` and ```addresses``` corresponds one another, so keep attention to the order of your values.

For example sensor-pump-pair number one (or at index 0 to be precise) reads as follows:

Sensor at ```A0```

Pump at ```11```

Placement ```outside-roofed```

Water requirements ```high-water-requirement```


### Relay
In ```Pump.cpp``` there's an ```init```-Function. The relay I use seems to be off on ```HIGH``` and on on ```LOW```. Depending on the relay you use, you might want to change that.

### Hygrometer calibration
Depending on your assembly the hygrometers can return different values.
During my prototyping sensor 1 returned a value of around 600, while sensor 2 returned a value around 130 while both being exposed to air.

I already provided a simple program for finding the base values of the sensor.

Open the ```Hygrometer-Calibration.ino``` and change the analog input to the pin you connected the hygrometer to. Or just leave it and connect the sensor to A0 :D

When you start the program, it first measures the "air-value" (so leave the sensor lying on your table or something). Then you have a 5 second pause to place the sensor in a glass of water. It then measures the "water-value" and logs it to the serial console.

Copy these two values and change the two lines named ```sensorAirValues``` and ```sensorWaterValues``` in file ```IrrigationSystem.h``` accordingly.

``` C++
const int sensorAirValues[] = {600, 130};
const int sensorWaterValues[] = {290, 90};
```

In this example I have two sensors. Sensor 1 has an air-value of 600 and a water-value of 290, while sensor 2 has 130 and 90 accordingly.

Repeat these steps for every sensor you have.

This is neccessary to calculate the moisture percentage.



### Water level measurement :construction:

### Communicating with a gateway
If you use a rapsberry pi gateway like I do, change the ```connectedToServer``` to ```true```. This constant is located in ```Main.h```

### Weather data
There is a directory called ```Raspberry Pi``` where are scripts for setting up your pi. Be sure to perform the step at [Communicating with a gateway](#Communicating with a gateway)

Open ```setup_communication.sh```and change the username matching your system. In my case it's called garduino. Then run these steps.

Then copy the ```server.py```, ```influxdb.config``` and ```weather.config``` to your pi to a directoy you might seem fit (like ```/opt/garduino/```). Change the values in the config to match your needs.

As soon you start the ```server.py```(```python3 sever.py```), it listens to any incoming request over serial.

### Grafana monitoring :construction:
If you want to visualize the data gathered by the sensors there is a script called ```setup_monitoring.sh```.

Set your credentials and run the script to install influxdb and grafana.