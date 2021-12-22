# Embedded System for weather station

## Hardware
* ESP8266 (nodemcu)  https://amzn.to/3yReeZZ
* weather station  https://amzn.to/32v8l8Z
  * wind speed sensor
  * wind direction resitor bridge
  * rain collection sensor
* DS18b20 liquid temperature sensor  https://amzn.to/3H4CUkF
* DHT22 air temperature and humidity sensor https://amzn.to/3pk99qb
* boost/buck converter  https://amzn.to/3H4SooL
* external antenna https://amzn.to/3yOvWxv

## Firmware
* compiled with PlatformIO, binary file -> `.pio/build/nodemcuv2/firmware.bin`
  * https://platformio.org/
* main.cpp in `src` folder
* custom supporting libraries in `lib` folder
* html templates stored in `data` folder

### Features

Sensors:
* water temperature
* air temperature and humidity
* wind speed and direction
* rain fall volume

Wifi features:
* modes: station or standalone access point
* wifi mode: LIGHT_SLEEP_T for reduced battery consumption
* http stand-alone server integrated
* optional: websocket server connection
* optional: send data to server
* time client for time synchronization
* wifi credentials saved in EEPROM, not hardcoded
* API for integration into existing network

Firmware features:
* task scheduler for efficient processing
* file system for saving data to flash for long term storage and no data loss through power outage
* data history tracking and graphical output

Hardware features:
* boost/buck converter for connecting to wide variety of DC power sources
* external antenna for increased wifi range and fidelity
