#pragma once

#include <Arduino.h>
#include <string.h>

void myWeatherInit(bool debugInput = false);

String getWindDataHTML();

String getWeatherDataJSON();

void updateWeather_EverySecond();

void myWeather_calc_weather();

String getRainDataHTML();

float get_wind_speed();

String get_cardinal_dir(int dir);

int get_wind_direction();

String return_water_temp_html();

String return_air_temp_humidity_html();

String return_weather_history_html();
