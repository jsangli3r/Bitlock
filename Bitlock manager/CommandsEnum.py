INCOMING_SECURITY_STATUS = "security_status_in"
OUTGOING_SECURITY_STATUS = "security_status_out"  # security_status_out <status>

INCOMING_MESSAGE = "msg_in"
OUTGOING_MESSAGE = "msg_out"

INCOMING_PRIVATE_KEY = "private_key_in"  # private_key_in <key>
OUTGOING_PRIVATE_KEY = "private_key_out"  # private_key_in <check_bool>

INCOMING_ASK_PAYLOAD = "payload_request_in"  # payload_request_in <index>
OUTGOING_ASK_PAYLOAD = "payload_request_out"  # payload_request_out <website> <username> <password>

INCOMING_PAYLOAD_DELETE = "payload_delete_in"  # payload_delete_in <index>
OUTGOING_PAYLOAD_DELETE = "payload_delete_out"  # payload_delete_out <bool>

INCOMING_PAYLOAD_CHANGE = "payload_modify_in"  # payload_modify_in <index> <website> <username> <password>
OUTGOING_PAYLOAD_CHANGE = "payload_modify_out"  # payload_modify_out <bool>

INCOMING_RESET = "reset_in"  # reset EEPROM / private key (will be useful when crypto will work)
OUTGOING_RESET = "reset_out"

INCOMING_SET_PRIVATE_KEY = "set_pkey_in"  # same as up
OUTGOING_SET_PRIVATE_KEY = "set_pkey_out"

INCOMING_FLUSH_LOADED_PAYLOADS = "flush_loaded_payloads_in"  # refresh payload ram register
OUTGOING_FLUSH_LOADED_PAYLOADS = "flush_loaded_payloads_out"  # flush_loaded_payloads_out <index[]> <website[]> get back payloads indexes and website (website only to identifiy the payload for the client)

INCOMING_ASK_PAYLOADS_POOL = "payloads_pool_request_in"
OUTGOING_ASK_PAYLOADS_POOL = "payloads_pool_request_out"

INCOMING_MEMORY_USAGE = "memory_usage_in"
OUTGOING_MEMORY_USAGE = "memory_usage_out"

INCOMING_DEBUG = "debug_in"
OUTGOING_DEBUG = "debug_out"

INCOMING_PAYLOAD_ADD = "payload_add_in"
OUTGOING_PAYLOAD_ADD = "payload_add_out"


