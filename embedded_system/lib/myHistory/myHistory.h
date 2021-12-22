#pragma once

#include <Arduino.h>
#include <string.h>

String return_int_array_history(int array[], int array_length, bool stop_at_zero = true);

String return_float_array_history(float array[], int array_length, bool stop_at_zero = true);

String return_char_array_history(char array[][6], int array_length);
