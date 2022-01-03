#include <Arduino.h>
#include <string.h>

// custom libraries
#include <myEEPROM.h>
#include <myF.h>
#include <myTemp.h>
#include <myWeather.h>
#include <myWebSocket.h>
#include <myWifi.h>
#include <myScheduler.h>
#include <myTime.h>
/*
  weather and lake temp sensor for dock up north

   pinMode(, OUTPUT/INPUT);pin# 1,3,15,13,12,14,2,0,4,5,16,9,10
   ADC0: analogRead(A0)
   interupt pins: D0-D8 = pin 16,5,4,

  weather station wiring:
  4 wire harnes:
    Winddir:
      black =>(jumper to A0) => 1k resistor => ground
      green => 3.3v
    windspeed:
      red => ground, 0v
      yellow => D5

  2 wire harness - rain bucket
  red => ground, 0v
  green => D6

  new features: websocket client
  to do: change path
*/

const bool debug = false;

//=========================================================
void setup()
{
    if (debug) Serial.begin(9600);
    //
    myFinit(debug);
    //
    myWeatherInit(debug);
    //
    EEPROMinit();
    //
    myWifi_Init(debug);
    //
    myTempInit(debug);
    // 
    myWifi_connectWifi();
    //
    myScheduler_init(debug);
}
//==================================================
void loop()
{
    myScheduler_asap();
    myScheduler_1s();
    myScheduler_60s();
    myScheduler_5m();
}

// =======================================
