#include <Arduino.h>
#include <string.h>

// custom libs
#include "myScheduler.h"
#include <myTemp.h>
#include <myWeather.h>

// local vars
unsigned long last_check_60s = 0;

// tasks to complete every 60 seconds
void mySchedule_60s()
{
    if (millis() - last_check_60s >= 60000) {
        last_check_60s = millis();
        // 
        myTemp_every_60s();
        //
        myWeather_every_60s();
    }
}