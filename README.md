# Wifi_Weather_Station
Full featured stand alone http/websocket weather station server

This is a full stack project with an embedded device connecting back to an edge server for servering multiple public clients

The embedded system consists of an ESP8266 NodeMCU connected to multiple sensors which can run on battery power. It can send data to local clients through an integrated HTTP web server or send data back to an edge server for greater demand requirements.

The backend server software consists of a Flask web server capable of serving full featured web pages. I recommend an public facing NGINX reverse proxy directed at a Gunicorn WSGI HTTP server for serving clients.

Cheers,
Mark




TLDR: it records weather stuff and can talk to a server if need be.

TO DO:
* build server backend
* device firmware is 80%, some features not implemented yet
