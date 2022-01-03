#pragma once

#include <Arduino.h>
#include <string.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>


void myWifi_Init(bool debugInput);

void myWifi_Loop();

void myWifi_connectWifi(bool New_Connection = false);

void myWifi_web_server_config();

//