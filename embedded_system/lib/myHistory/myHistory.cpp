#include <Arduino.h>
#include <string.h>

// custom libs
#include "myHistory.h"

extern bool debug;

// local variables ======================================

// iterate through array and return comma delimited string
String return_int_array_history(int array[], uint8_t array_length, uint8_t start_index)
{
    String output = "";

    if (start_index == 0)
    {
        for (int i = 0; i < (array_length - 1); i++) // iterate over array length
        {
            output += String(array[i]) + ", ";
        }
        output += String(array[array_length - 1]);
    }
    else
    {
        for (int i = start_index; i < array_length; i++) // start at start_index and iterate to end
        {
            output += String(array[i]) + ", ";
        }
        for (int i = 0; i < (start_index - 1); i++) // start at 0 and go to start_index - 2
        {
            output += String(array[i]) + ", ";
        }
        output += String(array[start_index - 1]) ; // end array at start_index - 1
    }

    return output;
}

// iterate through array and return comma delimited string
String return_float_array_history(float array[], uint8_t array_length, uint8_t start_index)
{
    String output = "";

    if (start_index == 0)
    {
        for (int i = 0; i < (array_length - 1); i++) // iterate over array length
        {
            output += String(array[i]) + ", ";
        }
        output += String(array[array_length - 1]);
    }
    else
    {
        for (int i = start_index; i < array_length; i++) // start at start_index and iterate to end
        {
            output += String(array[i]) + ", ";
        }
        for (int i = 0; i < (start_index - 1); i++) // start at 0 and go to start_index - 2
        {
            output += String(array[i]) + ", ";
        }
        output += String(array[start_index - 1]) ; // end array at start_index - 1
    }

    return output;
}

// iterate through array and return comma delimited string
String return_char_array_history(char array[][6], uint8_t array_length, uint8_t start_index)
{
    String output = "";

    if (start_index == 0)
    {
        for (uint8_t i = 0; i < (array_length - 1); i++) // iterate over array length
        {
            output += "\"" + String(array[i]) + "\", ";
        }
        output += "\"" + String(array[array_length - 1]) + "\"";
    }
    else
    {
        for (uint8_t i = start_index; i < array_length; i++) // start at start_index and iterate to end
        {
            output += "\"" + String(array[i]) + "\", ";
        }
        for (uint8_t i = 0; i < (start_index - 1); i++) // start at 0 and go to start_index - 2
        {
            output += "\"" + String(array[i]) + "\", ";
        }
        output += "\"" + String(array[start_index - 1]) + "\""; // end array at start_index - 1
    }

    return output;
}