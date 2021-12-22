#include <Arduino.h>
#include <string.h>

// custom libs
#include "myHistory.h"

extern bool debug;

// local variables ======================================

// iterate through array and return comma delimited string
String return_int_array_history(int array[], int array_length, bool stop_at_zero)
{
    String output = "";

    for (int i = 0; i < (array_length - 1); i++)
    {
        if(stop_at_zero == true && array[i] == 0){
            output += "0";
            return output;
        }
        output += String(array[i]) + ",";
    }
    output += String(array[array_length - 1]);

    return output;
}

// iterate through array and return comma delimited string
String return_float_array_history(float array[], int array_length, bool stop_at_zero)
{
    String output = "";

    for (int i = 0; i < (array_length - 1); i++)
    {
        if( stop_at_zero == true && array[i] == 0.00){
            output += "0";
            return output;
        }
        output += String(array[i]) + ",";
    }
    output += String(array[array_length - 1]);

    return output;
}

// iterate through array and return comma delimited string
String return_char_array_history(char array[][6], int array_length)
{
    String output = "";

    for (int i = 0; i < (array_length - 1); i++)
    {
        output += String(array[i]) + ",";
    }
    output += String(array[array_length - 1]);

    return output;
}