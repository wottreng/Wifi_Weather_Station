#include <Arduino.h>
#include <string.h>

// custom libs
#include "myScheduler.h"
#include <myTemp.h>
#include <myWeather.h>
#include <myWifi.h>
#include <myTime.h>
#include <myWebSocket.h>

// local vars
unsigned long last_check_1s = 0;
unsigned long last_check_60s = 0;
unsigned long last_check_5m = 0;
bool DEBUG = false;

// init 1sec timer
void myScheduler_init(bool debug){
    if(debug) DEBUG = true;
    last_check_1s = millis();
    last_check_60s = millis();
    last_check_5m = millis();
}

void myScheduler_asap(){
    // check for websocket communication
    websocketLoop();
    // check for wifi communication
    myWifi_Loop();
}

// tasks to complete every 1 sec
void myScheduler_1s()
{
    if (millis() - last_check_1s >= 1000) {
        //if(DEBUG) Serial.println("1s");
        last_check_1s = millis();
        updateWeather_EverySecond();
    }
}

// tasks to complete every 60 seconds
void myScheduler_60s()
{
    if (millis() - last_check_60s >= 60000) {
        //if(DEBUG) Serial.println("60s");
        last_check_60s = millis();
        // 
        myTemp_check_sensors();
        //
        myWeather_calc_weather();
        //
        myTime_update_time_client();
    }
}

// tasks to complete every 5 min
void myScheduler_5m()
{
    if (millis() - last_check_5m >= 300000) {
        last_check_5m = millis();
        myWifi_connectWifi(false);
    }
}