# Embedded System for weather station

## hardware
ESP8266 (nodemcu)
wind speed sensor
wind direction resitor bridge
rain collection sensor
DS18b20 liquid temperature sensor
DHT11 air temperature and humidity sensor
boost/buck converter
external antenna

## firmware
compiled with PlatformIO, binary file -> `.pio/build/nodemcuv2/firmware.bin`
main.cpp in `src` folder
custom supporting libraries in `lib` folder
html templates stored in `data` folder

### Features

onBoard sensors:
* water temperature
* air temperature and humidity
* wind speed and direction
* rain fall volume

wifi features:
* modes: station or standalone access point
* wifi mode: LIGHT_SLEEP_T for reduced battery consumption
* http stand-alone server integrated
* optional: websocket server connection
* optional: send data to server
* time client for time synchronization
* wifi credentials saved in EEPROM, not hardcoded
* API for integration into existing network

firmware features:
* task scheduler for efficient processing
* file system for saving data to flash for long term storage and no data loss through power outage
* data history tracking and graphical output

hardware features:
* boost/buck converter for connecting to wide variety of DC power sources
* external antenna for increased wifi range and fidelity
