#pragma once

#include <Arduino.h>
#include <string.h>

float return_water_temp();

float return_air_temp();

float return_air_humidity();

void myTempInit(bool debugInput);

void myTemp_every_60s();

void check_air_temp_humidity_DHT11();

void check_water_temp_DS18B20();
