#pragma once

#include <Arduino.h>
#include <string.h>

void myScheduler_init(bool debug = false);

void myScheduler_asap();

void myScheduler_1s();

void myScheduler_60s();

void myScheduler_5m();

//