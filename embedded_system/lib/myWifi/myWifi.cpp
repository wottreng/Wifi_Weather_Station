#include <Arduino.h>
#include <string.h>
#include <WiFiClientSecure.h> 
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

// custom libs
#include "myWifi.h"
#include <myEEPROM.h>
#include <myF.h>
#include <myWebSocket.h>
#include <myWeather.h>
#include <myTemp.h>
#include <myFileSystem.h>
#include <myTime.h>
#include <myConfig.h>

extern bool debug ;

// local variables ---------------

// http server
ESP8266WebServer httpServer(80);

// AP vars
IPAddress apIP(10, 42, 20, 1);
const String APssid = "wifiLakeInfo";
const String APpswd = "";
int chan = 1;
int hidden = false;
int max_con = 4;
// update server
ESP8266HTTPUpdateServer httpUpdater;
const String hostName = "weather";
// wifi 
String WIFI_SSID;
String WIFI_PASS;

// eeprom wifi credential location
const byte wifiSSIDloc  = 0;
const byte wifiPSWDloc = 20;

// https client
//const char fingerprint[] PROGMEM =  "B7 CB 1D 1B 02 72 1D 0E 89 A7 94 92 55 38 A7 37 9B 5D CD C4";

// functions ==================================================================

// read wifi creds from eeprom
void myWifi_Init(bool debugInput = false){
    if(debugInput) debug = true;
    WIFI_SSID = read_eeprom(wifiSSIDloc); // string
    WIFI_PASS = read_eeprom(wifiPSWDloc);
}

// wifi handle client and update MDNS, run in main loop
void myWifi_Loop()
{
    httpServer.handleClient();
    MDNS.update();
}

// wifi cred html page
void wifiCredentialUpdatePage() {
  String setuphtml = "<!DOCTYPE HTML><html><head>"
                     "<title>ESP Input Form</title>"
                     "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
                     "</head><body><p>current wifi AP SSID: &nbsp <b>";
  setuphtml += WIFI_SSID;
  setuphtml += "</b></p>"
               "<form action=\"/input\" method=\"POST\">"
               " Wifi ssid :<br>"
               " <input type=\"text\" name=\"wifi SSID\" value=\"\">"
               " <br>"
               " Wifi password:<br>"
               " <input type=\"password\" name=\"password\" value=\"\">"
               " <br><br>"
               " <input type=\"submit\" value=\"Submit\">"
               "</form> "
               "</body></html>";
  String htmlConfig = setuphtml;
  httpServer.send(200, "text/html", htmlConfig);  //send string to browser
}

// This routine is executed when you change wifi AP
void APinputChange() {
  WIFI_SSID = httpServer.arg("wifi SSID");
  WIFI_PASS = httpServer.arg("password"); //string
  writeToEEPROM(WIFI_SSID,wifiSSIDloc);
  writeToEEPROM(WIFI_PASS,wifiPSWDloc);  

  String s = "<meta http-equiv=\"refresh\" content=\"30; url='/'\" />"
             "<body> trying to connect to new wifi connection </br> "
             "if connection is successful you will be disconnected from soft AP</br>"
             "please wait a minute then click below to go to main page</body>"
             "<br/><p>click to go back to main page:&nbsp <a href=\"/\"><button style=\"display: block;\">AP Main Page</button></a>";
  httpServer.send(200, "text/html", s);  //Send web page
  httpServer.stop();
  myWifi_connectWifi();
}



// check wifi status and connect wifi or else create AP
void myWifi_connectWifi( bool New_Connection)
{
    if (WiFi.status() != 3 || New_Connection == true) // if not conneted 
    {
        wifi_set_sleep_type(LIGHT_SLEEP_T); // Auto modem sleep is default, light sleep for power savings
        WiFi.persistent(false);
        WiFi.setOutputPower(20);
        WiFi.mode(WIFI_STA);
        WiFi.hostname(hostName.c_str());
        WiFi.begin(WIFI_SSID, WIFI_PASS);
        const byte maxTry = 10; // try this many times to connect
        byte x = 0;
        while (x <= maxTry) {
            delay(800);
            byte conStatus = WiFi.status();
            x += 1;
            if (conStatus == 3) { // connected to AP                
                x = 100;
                myTimeInit(); // init time client
                if (debug) {
                    digitalWrite(getLEDpin(), HIGH); // off
                    Serial.println("");
                    Serial.print("Connected to ");
                    Serial.println(WIFI_SSID);
                    Serial.print("IP address: ");
                    Serial.println(WiFi.localIP());
                }
                // enable websocket server
                webSocketServerSwitch(false); // ***********************

            } else if (conStatus != 3 && x == maxTry) {
                update_time_client_status(false);
                // disable socket server
                webSocketServerSwitch(false);
                // access point mode
                WiFi.mode(WIFI_AP);
                WiFi.setOutputPower(16); // 20.5 -> 0 in .25 increments
                WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0)); //#
                WiFi.softAP(APssid, APpswd, chan, hidden, max_con);
                if (debug) {
                    digitalWrite(getLEDpin(), LOW); // on
                    Serial.println("no AP connection, starting hotspot");
                }
            }
        }
        myWifi_web_server_config();
    }
}

// url not found
void myWifi_notFound() {  //when stuff after / is incorrect
  String s = "<meta http-equiv=\"refresh\" content=\"5; url='/'\" />"
             "<body> not a know page for ESP server </br> you will be directed automaticly or click button below to be redirected to main page</br>"
             "<br/><p>click to go back to main page:&nbsp <a href=\"/\"><button style=\"display: block;\">AP Main Page</button></a>";
  httpServer.send(200, "text/html", s);  //Send web page
}

// return weather history
void myWifi_weather_history(){
    httpServer.send(200, "text/html", return_weather_history_html());
}

// return json data format
void myWifi_jsonData() {
  String output = "{"
                  "\"waterTempF\":\"" + String(return_water_temp(), 2) + "\"," +
                  "\"airTempF\":\"" + String(return_air_temp(), 2) + "\"," +
                  "\"airHumidity%\":\"" + String(return_air_humidity(), 2) + "\"," +
                  // weather data json
                  getWeatherDataJSON();

  if (debug) {
    if (digitalRead(getLEDpin()))
      output += "\"LED\":\"off\"";
    else
      output += "\"LED\":\"on\"";
  }

  output += "\"time_now\":\"" +  return_current_time() + "\","
            "\"time_client_status\":\"" +  return_time_client_status() + "\","
            "\"websocketStatus\":\"" + return_SocketStatus() +    "\","
            "\"Wifi_SSID\":\"" +  WIFI_SSID + "\",";

  byte mode = WiFi.getMode();
  if (mode == 1)
    output += "\"Wifi_Mode\":\"Station\",";
  else if (mode == 2)
    output += "\"Wifi_Mode\":\"Access Point\",";
  else if (mode == 3)
    output += "\"Wifi_Mode\":\"STA_AP\",";
  else if (mode == 0)
    output += "\"Wifi_Mode\":\"Off\",";

  output += "\"Wifi_Power_dBm\":\"20\","
            "\"Wifi_PhyMode\":\"" + String(WiFi.getPhyMode()) + "\"," // 3:
            "\"Wifi_Sleep_Mode\":\""  + String(WiFi.getSleepMode()) + "\"," // 1: auto light_sleep, 2: auto modem sleep
            "\"Wifi_IP_addr\":\""  + WiFi.localIP().toString() + "\",";

  byte conStatus = WiFi.status();
  if (conStatus == 0)
    output += "\"Wifi_Status\":\"Idle\",";
  else if (conStatus == 3)
    output += "\"Wifi_Status\":\"Connected\",";
  else if (conStatus == 1)
    output += "\"Wifi_Status\":\"not connected\",";
  else if (conStatus == 7)
    output += "\"Wifi_Status\":\"Disconnected\",";
  else
      output += "\"Wifi_Status\":\"" + String(conStatus) + "\",";

  output += "\"Wifi_Signal_dBm\":\"" + String(WiFi.RSSI()) +  "\","
            "\"Core Frequency MHz\":\"" +  String(ESP.getCpuFreqMHz()) +   "\","
            "\"reason last reset\":\"" +    ESP.getResetReason() +    "\","
            "\"free HEAP\":\"" + String(ESP.getFreeHeap()) +  "\"," 
            "\"ESP ID\":\"" + String(ESP.getChipId()) +  "\","
            "\"sketch size\":\"" + String(ESP.getSketchSize()) +  "\","  
            "\"FS info\":\"" + myFileSystem_info() +  "\","  
            "\"version\":\"" + getVersion() + "\""
            "}";

  httpServer.send(200, "application/json", output);
}

//===main page builder=========================
void myWifi_mainPageHTMLpageBuilder() 
{
String html = "<!DOCTYPE html>"
"<html lang='en'>"
"<head>"
"<meta charset='UTF-8'>"
"<title>Weather Station</title>"
"<meta name='viewport' content='width=device-width, initial-scale=1'/>"
"<meta http-equiv='refresh' content='20; url='/''  />"
"</head>"
"<style>"
"body {"
"position: relative;"
"top: 0;"
"left: 0;"
"color: white;"
"background-color: transparent;"
"height: calc(100vh - 10px);"
"width: calc(100vw - 10px);"
"border: #ffffff solid 5px;"
"border-radius: 20px;"
"margin: 0;"
"padding: 0;"
"z-index: 0;"
"}"
".flexContainer {"
"background-image: linear-gradient(180deg, #00008B, black);"
"z-index: 1;"
"position: relative;"
"margin: 0;"
"padding: 0;"
"width: 100%;"
"height: 100%;"
"display: flex;"
"text-align: center;"
"align-items: center;"
"align-content: center;"
"flex-wrap: nowrap;"
"flex-direction: column;"
"overflow-y: scroll;"
"font-weight: bold;"
"}"
".title {"
"padding: 10px;"
"font-size: x-large;"
"}"
".subTitle {"
"padding: 5px;"
"font-size: large;"
"}"
".subFlexBlock {"
"z-index: 2;"
"font-size: large;"
"position: relative;"
"left: 0;"
"margin: 0;"
"margin-bottom: 10px;"
"padding: 10px;"
"width: 100%;"
"max-width: 360px;"
"height: auto;"
"display: flex;"
"flex-direction: column;"
"text-align: center;"
"align-items: center;"
"align-content: center;"
"}"
".greenBorder {"
"background-color: black;"
"color: white;"
"width: 90%;"
"border: #FAF0E6 solid 6px;"
"border-radius: 20px;"
"}"
".whiteBorder {"
"background-color: #818181;"
"color: #18453B;"
"width: 80%;"
"border: #ffffff solid 6px;"
"border-radius: 20px;"
"}"
".blackBorder {"
"background-color: #18453B;"
"color: white;"
"width: 85%;"
"border: #000000 solid 6px;"
"border-radius: 20px;"
"}"
".textBlocks {"
"font-size: large;"
"text-align: center;"
"align-content: center;"
"align-items: center;"
"padding: 2px;"
"margin: 0;"
"}"
"button {"
"margin: 0;"
"font-size: large;"
"padding: 5px 10px 5px 10px;"
"color: #b4b4b4;"
"background-color: black;"
"border-radius: 10px;"
"border: #18453B solid 4px;"
"text-decoration: underline #b4b4b4;"
"}"
".greyText{"
"color: #b4b4b4;"
"}"
"</style>"
"<body>"
"<div class='flexContainer'>"
"<div class='title greyText'>LakeHouse Weather Station</div>"
"<div id='date'></div>"
"<script type='text/JavaScript' defer>"
"let a = new Date();"
"let b = String(a).split(' GMT')[0];"
"document.getElementById('date').innerHTML = `${b}`;"
"//console.log(b);"
"</script>"
"<div class='subFlexBlock greenBorder'>"
"<p class='textBlocks greyText'>LAKE</p>" + return_water_temp_html() + "</div>"
"<div class='subFlexBlock greenBorder'>"
"<p class='textBlocks greyText'>AIR</p>" + return_air_temp_humidity_html() + "</div>"
"<div class='subFlexBlock greenBorder'>"
"<p class='textBlocks greyText'>WIND</p>" + getWindDataHTML() +"</div>"
"<div class='subFlexBlock greenBorder'>"
"<p class='textBlocks greyText'>RAIN</p>" + getRainDataHTML() +"</div>"
"<div class='subFlexBlock greenBorder'>"
"<p class='textBlocks greyText'> <a href=\"/history\">Weather History</a> </p>"
"</div>"
/*
"<div class='subFlexBlock whiteBorder'>"
"<span class='textBlocks'>Change Wifi Access Point: </span><a href='/APsetup'><button style='display: block;'>Wifi Setup</button></a>"
"</div>"
*/
"<p>Version " + getVersion() +"</p>"
"</div>"
"</body>"
"</html>";

httpServer.send(200, "text/html", html); // send string to browser
}

// make request to a server
void http_request() {
  //
  WiFiClient client;
  HTTPClient http;
  const String httpHost = myConfig_server0;
  //
  if (http.begin(client, httpHost)) {
    int httpCode = http.GET();
    // connected server
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      if (debug) Serial.printf("[HTTP] GET... code: %d\n", httpCode);
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        if (debug) {
          String payload = http.getString();
          Serial.println(payload);
        }
      }
    } else {
      if (debug)
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
  } else {
    if (debug)
      Serial.printf("[HTTP} Unable to connect\n");
  }
}

// post data to server
void http_post_request(){

}
// direct api call
void myWifi_argData()
{
    if (httpServer.arg("websocket") != "") {
        int value = (httpServer.arg("websocket")).toInt();
        if (value == 1) webSocketServerSwitch(true);
        else webSocketServerSwitch(false);
      
    }
    
    myWifi_jsonData();
}

// http server config
void myWifi_web_server_config()
{
    MDNS.begin(hostName.c_str());

    //------server--------------------
    // server.stop();
    httpServer.onNotFound(myWifi_notFound); // after ip address /
    httpServer.on("/", HTTP_GET, myWifi_mainPageHTMLpageBuilder);
    httpServer.on("/APsetup", HTTP_GET, wifiCredentialUpdatePage); // see current AP setup
    httpServer.on("/input", HTTP_POST, APinputChange); // change AP setup
    httpServer.on("/api0", HTTP_GET, myWifi_argData);
    httpServer.on("/history", HTTP_GET, myWifi_weather_history);
    httpServer.on("/reboot", HTTP_GET, reboot);
    httpUpdater.setup(&httpServer);
    httpServer.begin();
    MDNS.addService("http", "tcp", 80);
}

//===
/*
// send data via https
void sendData(String dataX) {
  // vars
  // https://circuits4you.com/2019/01/10/esp8266-nodemcu-https-secured-get-request/

  const char *host = "jsonplaceholder.typicode.com";
  const int httpsPort = 443; // HTTPS= 443 and HTTP = 80

  //
  String message =  "uploadData=" + dataX + "&xkeyx=wottreng69&fileName=lakeTemp";
  String messageLength = String(message.length());
  // Your Domain name with URL path or IP address with path
  WiFiClientSecure httpsClient;
  httpsClient.setFingerprint(fingerprint);
  httpsClient.setTimeout(15000); // 15 Seconds
  // Serial.print("HTTPS Connecting");
  int counter = 0; // retry counter
  while ((!httpsClient.connect(host, httpsPort)) && (counter < 30)) {
    delay(100);
    // Serial.print(".");
    counter++;
  }
  if (counter == 30) {
    // Serial.println("Connection failed");
    delay(10);
  } else {
    // Serial.println("Connected to web");
    // POST Data
    String Link = "/19sdf8g31h";
    httpsClient.print(String("POST ") + Link + " HTTP/1.1\r\n" +
                      "Host: " + host + "\r\n" +
                      "Content-Type: application/x-www-form-urlencoded" +
                      "\r\n" + "Content-Length: " + messageLength + "\r\n\r\n" +
                      message + "\r\n" + "Connection: close\r\n\r\n");
  }

  // Serial.println("request sent");

  while (httpsClient.connected()) {
    String line = httpsClient.readStringUntil('\n');
    if (line == "\r") {
      // Serial.println("headers received");
      break;
    }
  }

  String line;
  while (httpsClient.available()) {
    line = httpsClient.readStringUntil('\n'); // Read Line by Line
    // Serial.println(line);                      //Print response
  }
}
*/

//