#include <Arduino.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include <string.h>
#include <DHT.h>

// custom libs
#include "myTemp.h"

extern bool debug;

// local variables ======================================

// ds18b20 liquid temp sensor
#define ONEWIREBUS 4 // pin 4, D2 on board
float waterTemp = 0.0;
//float water_temp_history_24h[24];

// DHT sensor -------------
#define DHTPIN 5 // pin D1
#define DHTTYPE DHT22 // white sensor
DHT dht(DHTPIN, DHTTYPE);
float dhtTemp = 0.0;
float dhtHumid = 0.0;
float heatIndex = 0.0;
float air_temp_history_24h[24];

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONEWIREBUS);
// Pass our oneWire reference to Dallas Temperature sensor
DallasTemperature sensors(&oneWire);

// indexs
//byte temp_history_index_60m = 0;

// last check timers
//unsigned long temp_Loop_60s_last_check = 0;
//unsigned long temp_Loop_60m_last_check = 0;

// start ds18b20 sensor
void myTempInit(bool debugInput = false)
{
    if(debugInput)
        debug = true;
    // one wire bus
    sensors.begin();
    // dht sensor
    dht.begin(); 
}

// check sensors, run in scheduler
void myTemp_check_sensors()
{
    check_water_temp_DS18B20();
    check_air_temp_humidity_DHT11();
}

/*
void myTemp_Loop_60m(){
        if ((millis() - temp_Loop_60m_last_check) > 3600000) {
        update_temp_history();
        temp_Loop_60m_last_check = millis();
    }
}
*/

// return ds18b20 temp data
float return_water_temp(){
    return waterTemp;
}

// return air temp from dht11 sensor
float return_air_temp(){
    return dhtTemp;
}

// air humidity from dht11 sensor
float return_air_humidity(){
    return dhtHumid;
}

// ds18b20 sensor water temp
void check_water_temp_DS18B20()
{
    sensors.requestTemperatures();
    // float temperatureC = sensors.getTempCByIndex(0);
    waterTemp = sensors.getTempFByIndex(0);
}

/*
// temp history
void update_temp_history()
{
    air_temp_history_24h[temp_history_index_60m] = dhtTemp;
    water_temp_history_24h[temp_history_index_60m] = waterTemp;
    // timeClient.getFormattedTime().substring(0, 5).toCharArray(tempHistoryTime[historyIndex], 6);
    temp_history_index_60m++;
    if (temp_history_index_60m > 23) {
        temp_history_index_60m = 0;
        //historyIndexMaxed = true;
    }
}
*/

// DHT22 function
void check_air_temp_humidity_DHT11()
{
    float t = dht.readTemperature(true);
    float h = dht.readHumidity();

    if (isnan(h) || isnan(t)) {
        // do nothing, bad reading
    } else {
        dhtHumid = h;
        dhtTemp = t;
        heatIndex = dht.computeHeatIndex();
    }
}