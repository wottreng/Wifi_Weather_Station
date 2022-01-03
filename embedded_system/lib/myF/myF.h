#pragma once

#include <Arduino.h>
#include <string.h>


void myFinit(bool debugInput = false);

void reboot();

void blinkLED();

String getVersion();

const byte getLEDpin();

void deepSleep();