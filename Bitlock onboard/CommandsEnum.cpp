#include <Arduino.h>
#include "CommandsEnum.h"

/**
 * FILE THAT CONTAINS ALL COMMANDS, THAT'S AN ENUM LIKE. 
 */
 
const String INCOMING_SECURITY_STATUS = "security_status_in";
const String OUTGOING_SECURITY_STATUS = "security_status_out"; //security_status_out <status>

const String INCOMING_MESSAGE = "msg_in";
const String OUTGOING_MESSAGE = "msg_out";

const String INCOMING_PRIVATE_KEY = "private_key_in"; //private_key_in <key>
const String OUTGOING_PRIVATE_KEY = "private_key_out"; //private_key_in <check_bool>

const String INCOMING_ASK_PAYLOAD = "payload_request_in"; //payload_request_in <index>
const String OUTGOING_ASK_PAYLOAD = "payload_request_out"; //payload_request_out <website> <username> <password>

const String INCOMING_PAYLOAD_DELETE = "payload_delete_in"; //payload_delete_in <index>
const String OUTGOING_PAYLOAD_DELETE = "payload_delete_out"; //payload_delete_out <bool>

const String INCOMING_PAYLOAD_CHANGE = "payload_modify_in"; //payload_modify_in <index> <website> <username> <password>
const String OUTGOING_PAYLOAD_CHANGE = "payload_modify_out"; //payload_modify_out <bool>

const String INCOMING_RESET = "reset_in"; //reset EEPROM / private key (will be useful when crypto will work)
const String OUTGOING_RESET = "reset_out";

const String INCOMING_SET_PRIVATE_KEY = "set_pkey_in"; //same as up
const String OUTGOING_SET_PRIVATE_KEY = "set_pkey_out";

const String INCOMING_FLUSH_LOADED_PAYLOADS = "flush_loaded_payloads_in"; //refresh payload ram register
const String OUTGOING_FLUSH_LOADED_PAYLOADS = "flush_loaded_payloads_out"; //flush_loaded_payloads_out <index[]> <website[]> get back payloads indexes and website (website only to identifiy the payload for the client)

const String INCOMING_ASK_PAYLOADS_POOL = "payloads_pool_request_in";
const String OUTGOING_ASK_PAYLOADS_POOL = "payloads_pool_request_out";

const String INCOMING_MEMORY_USAGE = "memory_usage_in";
const String OUTGOING_MEMORY_USAGE = "memory_usage_out"; 

const String INCOMING_DEBUG = "debug_in";
const String OUTGOING_DEBUG = "debug_out";

const String INCOMING_PAYLOAD_ADD = "payload_add_in";
const String OUTGOING_PAYLOAD_ADD = "payload_add_out";




