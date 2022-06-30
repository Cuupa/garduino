#!/usr/bin/env python3

import serial
import json
import requests
import time

from influxdb import InfluxDBClient
from bs4 import BeautifulSoup
from datetime import datetime

input_device = '/dev/ttyACM0'

influx_client = None

zip_code = ''

weather_url = 'https://www.proplanta.de/Wetter/profi-wetter.php?SITEID=60&PLZ={zip_code}&WETTERaufrufen=plz&Wtp=&SUCHE=Wetter&wT='
weather_request_header = {
    'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/103.0.0.0 Safari/537.36',
    'Accept-Encoding': 'gzip,deflate,sdch'
}

debug = True


def readWeatherConfig():
    zip = ''
    with open('weather.config') as f:
        for line in f:
            values = line.split('=')
            key = values[0].strip()
            value = values[1].strip()
            if key == 'zip_code':
                zip = value
    return zip


def readInfluxConfig():
    influx_host = ''
    influx_port = 0
    influx_username = ''
    influx_password = ''
    influx_database_name = ''

    with open('influxdb.config') as f:
        for line in f:
            values = line.split('=')
            key = values[0].strip()
            value = values[1].strip()
            if key == 'influx_host':
                influx_host = value
            elif key == 'influx_port':
                influx_port = value
            elif key == 'influx_username':
                influx_username = value
            elif key == 'influx_password':
                influx_password = value
            elif key == 'influx_database_name':
                influx_database_name = value

    return influx_host, influx_port, influx_username, influx_password, influx_database_name


def setup():
    influx_host, influx_port, influx_username, influx_password, influx_database_name = readInfluxConfig()
    global influx_client
    influx_client = InfluxDBClient(host=influx_host,
                                   port=influx_port,
                                   username=influx_username,
                                   password=influx_password,
                                   ssl=False,
                                   verify_ssl=False)

    databases = influx_client.get_list_database()

    database_exists = next(
        (item for item in databases if item['name'] == influx_database_name), False)
    if not database_exists:
        influx_client.create_database(influx_database_name)
    influx_client.switch_database(influx_database_name)

    global zip_code
    zip_code = readWeatherConfig()

def connect():
    success = False
    while not success:
        try:
            ser = serial.Serial(input_device, 9600, timeout=5)
            ser.reset_input_buffer()
            success = True
            return ser
        except:
            debug('Device not connected')


def listen():
    ser = connect()
    debug('listening')
    while True:
        try:
            if ser.in_waiting > 0:
                message = ser.readline().decode('utf-8').rstrip()
                debug('Received message')
                debug(message)
                if "Garduino - 1.0" in message:
                    continue
                if "Sensor-Pump pairs found" in message:
                    continue

                payload = json.loads(message)
                request_type = payload['type']

                if request_type == 'measurement':
                    json_payload = getData(payload)
                    influx_client.write_points(json_payload)
                elif request_type == 'request':
                    if payload['request'] == 'weather-data':
                        weather = getWeatherData()
                        response = json.dumps(weather)
                        debug("Response")
                        debug(response)
                        ser.write(response.encode('utf-8'))
        except IOError as ioe:
            connect()
        except Exception as e:
            debug(e)
            debug('failed to process ' + message)


def getData(payload):
    json_payload = []
    data = {}
    if 'water-level' in payload:
        data = {
            'measurement': payload['system-name'],
            'tags': {
                'type': 'water-level',
                'sensor-index': int(payload['sensor-index']),
                'system-name': payload['system-name'],
            },
            'time': datetime.now(),
            'fields': {
                'system-name': payload['system-name'],
                'water-level': float(payload['water-level']),
                'liter': float(payload['liter'])
            }
        }
    elif 'hygrometer-status' in payload:
        data = {
            'measurement': payload['system-name'],
            'tags': {
                'type': 'hygrometer',
                'sensor-index': int(payload['sensor-index']),
                'system-name': payload['system-name']
            },
            'time': datetime.now(),
            'fields': {
                'system-name': payload['system-name'],
                'hygrometer-status': payload['hygrometer-status'],
                'humidity': float(payload['humidity']),
                'sensor-index': int(payload['sensor-index'])
            }
        }
    elif 'watered-volume' in payload:
        data = {
            'measurement': payload['system-name'],
            'tags': {
                'type': 'watered',
                'sensor-index': int(payload['sensor-index']),
                'system-name': payload['system-name']
            },
            'time': datetime.now(),
            'fields': {
                'system-name': payload['system-name'],
                'watered-volume': float(payload['watered-volume']),
                'sensor-index': float(payload['sensor-index'])
            }
        }
    json_payload.append(data)
    return json_payload


def getWeatherData():
    url = weather_url.format(zip_code=zip_code)
    response = requests.get(url, headers=weather_request_header)
    if response.status_code != 200:
        print('error queueing weather')
        return {}

    data = {}
    document = BeautifulSoup(response.content, 'html.parser')
    current_condition = document.select(
        '.MITTEFORMULARSPALTE_WETTER > .SCHRIFT_FORMULAR_WERTE_MITTE')
    data['time'] = current_condition[1].text.replace(
        '.', ':').replace(' Uhr', '')
    data['temperature'] = current_condition[2].text.replace(',', '.')
    data['condition'] = getCondition(current_condition[4])

    # 0 today, 1 tomorrow, etc
    evaporation = document.find('tr', {'id': 'VERDUNST'}).select(
        '.SCHRIFT_FORMULAR_WERTE_MITTE')[0].text
    data['evaporation'] = getEvaporation(evaporation)

    data['rainfall-today'] = document.find('tr', {'id': 'NS_24H'}).findAll('span')[
        1].text.replace(',', '.')
    data['rainfall-tomorrow'] = document.find(
        'tr', {'id': 'NS_24H'}).findAll('span')[2].text.replace(',', '.')


def getEvaporation(evaporation):
    if evaporation == 'gering':
        return 'low'
    elif evaporation == 'mäßig':
        return 'medium'
    elif evaporation == 'hoch':
        return 'high'


def getCondition(element):
    condition = element.select('img', alt=True)[0]['alt']
    val = condition.split(':')[1].split()[0]

    if val == 'sonnig':
        return 'sunny'
    elif val == 'heiter':
        return 'bright'
    elif val == 'bedeckt':
        return 'cloudy'
    elif val == 'wolkig':
        return 'cloudy'
    elif val == 'stark bewölkt':
        return 'very cloudy'
    elif val == 'regen':
        return 'rain'


def debug(msg):
    if debug:
        print(msg)


if __name__ == '__main__':
    setup()
    listen()
