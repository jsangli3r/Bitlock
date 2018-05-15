#include <Arduino.h>

/**
 * COMMANDSINTERPRETER HEADER FILE
 *  DECLARE THE STATIC FUNCTIONS AND METHODS
 */
class CommandsInterpreter
{
  public:
    static void str_command_router(char array[], unsigned int array_size);

    static void security_status();
    static bool private_key_in(String private_key); //receive private key from the manager software
    static void private_key_out(bool ok);
    
    static void outgoing_msg(String text);

    static void outgoing_ask_payload(String hash, unsigned int start_address);

    static void outgoing_payload_delete(unsigned int start_address);
    static void outgoing_ask_payloads_pool();
    static void outgoing_payload_add(String website, String username, String password);
    static void outgoing_memory_usage();
    static void outgoing_payload_modify(unsigned int start_address, String website, String username, String password);
    
    static unsigned int parametersCount(char data[], unsigned int data_size);
};



