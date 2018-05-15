#include <Arduino.h>
#include "Config.h"

static void print_debug(String msg){
  if (DEBUG){
    Serial.println(msg);
  }
}

static void toAsciiIntArray(char array[], unsigned int length, int ascii[]){
  for (unsigned int i = 0; i < length; i++){
    ascii[i] = (unsigned int)array[i]; //cast char one by one to the correspondant in integer / ASCII
  }
}



