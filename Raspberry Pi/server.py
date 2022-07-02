#!/usr/bin/env python3

import serial
import json
import requests
import time

from influxdb import InfluxDBClient
from bs4 import BeautifulSoup
from datetime import datetime

# Constants
_utf_8 = 'utf-8'

_weather_config_file = 'weather.config'
_influx_config_file = 'influx.config'

# weather config constants
_zip_code = 'zip_code'

# Influxdb config file constants
_influx_host = 'influx_host'
_influx_port = 'influx_port'
_influx_username = 'influx_username'
_influx_password = 'influx_password'
_influx_database_name = 'influx_database_name'


# Payload constants

# Requests
_type = 'type'
_measurement = 'measurement'
_request = 'request'
_weather_data = 'weather-data'

_water_level = 'water-level'
_hygrometer_status = 'hygrometer-status'
_watered_volume = 'watered-volume'
_hygrometer = 'hygrometer'
_humidity = 'humidity'
_system_name = 'system-name'
_sensor_index = 'sensor-index'
_liter = 'liter'
_watered = 'watered'

# influx
_tags = 'tags'
_time = 'time'
_fields = 'fields'

# agrarwetter
_agrarwetter_current_conditions_selector = '.MITTEFORMULARSPALTE_WETTER > .SCHRIFT_FORMULAR_WERTE_MITTE'
_rainfall_selector = 'tr', {'id': 'NS_24H'}
_evaporation_selector_1 = 'tr', {'id': 'VERDUNST'}
_evaporation_selector_2 = '.SCHRIFT_FORMULAR_WERTE_MITTE'
_span = 'span'

_temperature = 'temperature'
_condition = 'condition'
_evaporation = 'evaporation'
_rainfall_today = 'rainfall-today'
_rainfall_tomorrow = 'rainfall-tomorrow'

_gering = 'gering'
_maessig = 'mäßig'
_hoch = 'hoch'

_low = 'low'
_medium = 'medium'
_high = 'high'

_sonnig = 'sonnig'
_heiter = 'heiter'
_bedeckt = 'bedeckt'
_wolkig = 'wolkig'
_stark_bewoelkt = 'stark bewölkt'
_regen = 'regen'
_sunny = 'sunny'
_bright = 'bright'
_cloudy = 'cloudy'
_very_cloudy = 'very_cloudy'
_rain = 'rain'


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
    with open(_weather_config_file) as f:
        for line in f:
            values = line.split('=')
            key = values[0].strip()
            value = values[1].strip()
            if key == _zip_code:
                zip = value
    return zip


def readInfluxConfig():
    influx_host = ''
    influx_port = 0
    influx_username = ''
    influx_password = ''
    influx_database_name = ''

    with open(_influx_config_file) as f:
        for line in f:
            values = line.split('=')
            key = values[0].strip()
            value = values[1].strip()
            if key == _influx_host:
                influx_host = value
            elif key == _influx_port:
                influx_port = value
            elif key == _influx_username:
                influx_username = value
            elif key == _influx_password:
                influx_password = value
            elif key == _influx_database_name:
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
    while 1:
        try:
            ser = serial.Serial(input_device, 9600, timeout=5)
            ser.reset_input_buffer()
            return ser
        except:
            debug('Device not connected')


def listen():
    ser = connect()
    debug('listening')
    while 1:
        try:
            if ser.in_waiting > 0:
                message = ser.readline().decode(_utf_8).rstrip()
                debug('Received message')
                debug(message)
                if "Debug: " in message:
                    continue

                payload = json.loads(message)
                request_type = payload[_type]

                if request_type == _measurement:
                    json_payload = getData(payload)
                    influx_client.write_points(json_payload)
                elif request_type == _request:
                    if payload[_request] == _weather_data:
                        weather = getWeatherData()
                        response = json.dumps(weather)
                        debug("Response")
                        debug(response)
                        ser.write(response.encode(_utf_8))
        except IOError as ioe:
            ser = connect()
        except Exception as e:
            debug(e)
            debug('failed to process ' + message)


def getData(payload):
    json_payload = []
    data = {}
    if _water_level in payload:
        data = {
            _measurement: payload[_system_name],
            _tags: {
                _type: _water_level,
                _sensor_index: int(payload[_sensor_index]),
                _system_name: payload[_system_name],
            },
            _time: datetime.now(),
            _fields: {
                _system_name: payload[_system_name],
                _water_level: float(payload[_water_level]),
                _liter: float(payload[_liter])
            }
        }
    elif _hygrometer_status in payload:
        data = {
            _measurement: payload[_system_name],
            _tags: {
                _type: _hygrometer,
                _sensor_index: int(payload[_sensor_index]),
                _system_name: payload[_system_name]
            },
            _time: datetime.now(),
            _fields: {
                _system_name: payload[_system_name],
                _hygrometer_status: payload[_hygrometer_status],
                _humidity: float(payload[_humidity]),
                _sensor_index: int(payload[_sensor_index])
            }
        }
    elif _watered_volume in payload:
        data = {
            _measurement: payload[_system_name],
            _tags: {
                _type: _watered,
                _sensor_index: int(payload[_sensor_index]),
                _system_name: payload[_system_name]
            },
            _time: datetime.now(),
            _fields: {
                _system_name: payload[_system_name],
                _watered_volume: float(payload[_watered_volume]),
                _sensor_index: float(payload[_sensor_index])
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
        _agrarwetter_current_conditions_selector)
    data[_time] = current_condition[1].text.replace(
        '.', ':').replace(' Uhr', '')
    data[_temperature] = current_condition[2].text.replace(',', '.')
    data[_condition] = getCondition(current_condition[4])

    # 0 today, 1 tomorrow, etc
    evaporation = document.find(_evaporation_selector_1).select(
        _evaporation_selector_2)[0].text
    data[_evaporation] = getEvaporation(evaporation)

    data[_rainfall_today] = document.find(_rainfall_selector).findAll(_span)[
        1].text.replace(',', '.')
    data[_rainfall_tomorrow] = document.find(
        _rainfall_selector).findAll(_span)[2].text.replace(',', '.')


def getEvaporation(evaporation):
    if evaporation == _gering:
        return _low
    elif evaporation == _maessig:
        return _medium
    elif evaporation == _hoch:
        return _high


def getCondition(element):
    condition = element.select('img', alt=True)[0]['alt']
    val = condition.split(':')[1].split()[0]

    if val == _sonnig:
        return _sunny
    elif val == _heiter:
        return _bright
    elif val == _bedeckt:
        return _cloudy
    elif val == _wolkig:
        return _cloudy
    elif val == _stark_bewoelkt:
        return _very_cloudy
    elif val == _regen:
        return _rain


def debug(msg):
    if debug:
        print(msg)


if __name__ == '__main__':
    setup()
    listen()
