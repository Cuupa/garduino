#!/bin/bash

sudo adduser garduino dialout
sudo apt-get install python3
sudo apt-get install python3-pip

python3 -m pip install pyserial
python3 -m pip install influxdb
python3 -m pip install beautifulsoup4