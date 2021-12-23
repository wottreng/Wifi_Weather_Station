# Wifi_Weather_Station
Full featured stand alone http/websocket weather station server 🌤️

This is a full stack project with an embedded device connecting back to an edge server for servering multiple public clients

The embedded system consists of an ESP8266 NodeMCU connected to multiple sensors which can run on DC battery power. It can send data to local clients through an integrated HTTP web server or send data back to an edge server for greater demand requirements.

The backend server software consists of a Flask web server capable of serving full featured web pages and a websocket server for realtime data updates. I recommend a public facing NGINX reverse proxy server directed at a Gunicorn WSGI HTTP server for serving clients.

Cheers, \
Mark 🍻




TLDR: it records weather stuff and can talk to a server if you want.

TO DO:
* build server backend 🖥️
* device firmware is 80%, it runs but needs refinement and some features not implemented yet

CONTRIBUTE:
⚠️ feel free to comment and contribute to make this better! ⚠️

## web server output: 

![My image](https://github.com/wottreng/Wifi_Weather_Station/blob/0b0fcf4423432e7075c24fc4eeaa40551129381f/embedded_system/pics/appScreen.png)  

## weather station 

![Weather_Station](https://github.com/wottreng/Wifi_Weather_Station/blob/2bd008bd6837a7c4a689a93b267ae7e6c6f58afd/embedded_system/pics/weatherStation.png)
