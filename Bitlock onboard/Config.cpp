#include <Arduino.h>
#include "Config.h"

/**
 * FILE TO DEFINE ALL THE SHARED VALUES BETWEEN FILES
 */
 
const short DEFAULT_TRANSMISSION_SPEED = 9600; //baud.s^(-1)
const bool DEBUG = true; //debug output will be printed

/**
 * Ideal security status
 */
const byte SECURITY_STATUS_LOCKED_FINGERPRINT_OK_WAITING_FOR_PRIVATE_KEY = 3;
const byte SECURITY_STATUS_LOCKED_PRIVATE_KEY_OK_WAITING_FOR_FINGERPRINT = 2;
const byte SECURITY_STATUS_LOCKED_WAITING_FOR_BOTH_CHECK = 1; //waiting for private key + fingerprint
const byte SECURITY_STATUS_UNLOCKED = 0; //Storing it in byte because it's taking less ram.

byte SECURITY_STATUS = SECURITY_STATUS_LOCKED_WAITING_FOR_BOTH_CHECK; //Default security_status

const String PRIVATE_KEY = "42";

/**
 * RECOGNITION PATTERNS
 */
const byte PASSWORD_BLOCK_HEAD_START_PATTERN = 1; //start of a payload
const byte PASSWORD_BLOCK_HEAD_END_PATTERN = 4; //end of a payload

const byte PASSWORD_BLOCK_DATA_START_PATTERN = 2; //start of a payload data entry
const byte PASSWORD_BLOCK_DATA_END_PATTERN = 3; //end of a payload data entry


