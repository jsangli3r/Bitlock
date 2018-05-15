#include <Arduino.h>
#include <EEPROM.h>

#include "Config.h"

//static unsigned int payload_register_pool[256][2] = {};
static String hash = ""; //crc32

/**
 * @desc = Populate the payload register pool with the start and end addresses of which payload. What we gain to do that ? Response time.
 */
static int** getPayloadsPool()
{
    
}

/**
 * Check if the payload is stored in the RAM registery.
 */
/*static bool payloadExists(unsigned int start_address){
   size_t max_length = 256;
   for (unsigned int i = 0; i < max_length; i++){
      if (payload_register_pool[i][0] == start_address){ //[1] must be set
        return true;
      }
   }
   return false;
}*/

/**
 * @desc = Get the next available address which can store a new payload.
 * @return = (unsigned int) : 0 = there's no more space or there's a memory corruption
 */
static unsigned int nextAvailableAddress(){
  unsigned int max_length = EEPROM.length();
  for (unsigned int address = 0; address < max_length; address++){
    byte value = EEPROM.read(address);
    if (value == PASSWORD_BLOCK_HEAD_END_PATTERN && EEPROM.read((address + 1)) == 0){
      return (address + 1);
    }
  }
  return 0;
}

/**
 * @desc = Get the last payload end address stored in the EEPROM.
 * @return = (unsigned int) : EEPROM address
 * @see nextAvailableAddress()
 */
static unsigned int lastPayloadEndAddress(){
  return (nextAvailableAddress() - 1);
}

/**
 * @desc = Check if the EEPROM is full.
 * @return = (bool) : true, memory is full.
 * @see nextAvailableAddress()
 */
static bool memoryIsFull(){
  return nextAvailableAddress() == 0;
}

/**
 * @desc = Get the amount of available bits in the EEPROM.
 * @return = (unsigned int) : amount of available bits.
 * @see nextAvailableAddress()
 */
static unsigned int availableBits(){
  int next_a_address = nextAvailableAddress();
  int max_length = EEPROM.length();

  return (max_length - next_a_address);
}

/**
 * @desc = Get the amount available bytes in the EEPROM.
 * @return = (unsigned int) : amount of available bytes.
 * @see availableBits()
 */
static unsigned int availableBytes(){
  return (availableBits() / 8);
}

/**
 * @desc = Get the percent of EEPROM fillment.
 * @return = (unsigned float) : percent of memory usage.
 * @see lastPayloadEndAddress()
 */
static double memoryUsageInPercent(){
  return ((double)lastPayloadEndAddress() / (double)EEPROM.length()) * (double)100;
}

/**
 * @desc = Erase all EEPROM data by putting zeros from 0 to EEPROM max address. Must be unlocked to work.
 * @return = (bool) : false if there's a security status concern.
 */
static bool resetMemory(){
  /*if (SECURITY_STATUS != SECURITY_STATUS_UNLOCKED){
    return false; 
  }*/
  
  for (unsigned int i = 0; i < EEPROM.length(); i++)
  {
    EEPROM.write(i, 0);
  }
  return true;
}




