#include <Arduino.h>

#ifndef COMMANDS_E_H //in order to not create in memory the same variables
#define COMMANDS_E_H

/**
 * HEADER FILE THAT CONTAINS ALL COMMANDS VARS, THAT'S AN ENUM LIKE. 
 */
 
extern const String INCOMING_SECURITY_STATUS;
extern const String OUTGOING_SECURITY_STATUS;

extern const String INCOMING_MESSAGE;
extern const String OUTGOING_MESSAGE;

extern const String INCOMING_PRIVATE_KEY;
extern const String OUTGOING_PRIVATE_KEY;

extern const String INCOMING_ASK_PAYLOAD;
extern const String OUTGOING_ASK_PAYLOAD;

extern const String INCOMING_PAYLOAD_ADD;
extern const String OUTGOING_PAYLOAD_ADD;

extern const String INCOMING_PAYLOAD_DELETE;
extern const String OUTGOING_PAYLOAD_DELETE;

extern const String INCOMING_PAYLOAD_CHANGE;
extern const String OUTGOING_PAYLOAD_CHANGE;

extern const String INCOMING_RESET;
extern const String OUTGOING_RESET;

extern const String INCOMING_SET_PRIVATE_KEY;
extern const String OUTGOING_SET_PRIVATE_KEY;

extern const String INCOMING_ASK_PAYLOADS_POOL;
extern const String OUTGOING_ASK_PAYLOADS_POOL;

extern const String INCOMING_FLUSH_LOADED_PAYLOADS;
extern const String OUTGOING_FLUSH_LOADED_PAYLOADS;

extern const String INCOMING_MEMORY_USAGE;
extern const String OUTGOING_MEMORY_USAGE;

extern const String INCOMING_DEBUG;
extern const String OUTGOING_DEBUG;

#endif



