#include <Arduino.h>

#ifndef CONFIG_H
#define CONFIG_H

/**
 * CONSTANTS DEPT
 */
extern const short DEFAULT_TRANSMISSION_SPEED; //9600 bits/s
extern const bool DEBUG;


/**
* VARIABLES DEPT
*/

 
/*
 * UNLOCKED = 0 : can send data to the manager software
 * WAITING_FOR_FINGERPRINT_CHECK = 1 : pending to get fingerprint check
 * WAITING_FOR_PRIVATE_KEY_CHECK = 2 : pending to get private key from the manager software
 * WAITING_FOR_BOTH_CHECK = 3 : pending to get both security checks
 */
extern const byte SECURITY_STATUS_LOCKED_FINGERPRINT_OK_WAITING_FOR_PRIVATE_KEY;
extern const byte SECURITY_STATUS_LOCKED_PRIVATE_KEY_OK_WAITING_FOR_FINGERPRINT;
extern const byte SECURITY_STATUS_LOCKED_WAITING_FOR_BOTH_CHECK;
extern const byte SECURITY_STATUS_UNLOCKED; //Storing it in byte because it's taking less ram.

extern byte SECURITY_STATUS;

extern const String PRIVATE_KEY;


extern const byte PASSWORD_BLOCK_HEAD_START_PATTERN;
extern const byte PASSWORD_BLOCK_HEAD_END_PATTERN;


extern const byte PASSWORD_BLOCK_DATA_START_PATTERN;
extern const byte PASSWORD_BLOCK_DATA_END_PATTERN;
#endif



