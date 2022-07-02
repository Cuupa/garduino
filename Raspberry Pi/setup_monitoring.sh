#!/bin/bash
# This sets up influxdb on a raspberry pi (likely debian buster)
influx_admin = ""
influx_password= ""

curl https://repos.influxdata.com/influxdb.key | gpg --dearmor | sudo tee /usr/share/keyrings/influxdb-archive-keyring.gpg >/dev/null

echo "deb [signed-by=/usr/share/keyrings/influxdb-archive-keyring.gpg] https://repos.influxdata.com/debian $(lsb_release -cs) stable" | sudo tee /etc/apt/sources.list.d/influxdb.list

sudo apt-get update
sudo apt-get install -y influxdb
sudo systemctl unmask influxdb
sudo systemctl enable influxdb
sudo systemctl start influxdb


influx
CREATE USER $influx_admin WITH PASSWORD \'$influx_password\' WITH ALL PRIVILEGES 

sudo nano /etc/influxdb/influxdb.conf

auth-enabled = true
pprof-enabled = true
pprof-auth-enabled = true
ping-auth-enabled = true