#include <Arduino.h>
#include <stdlib.h> //lib to work with string
#include <EEPROM.h>

#include "CommandsInterpreter.h"
#include "CommandsEnum.h"
#include "Config.h"
#include "Payload.h"
#include "PayloadManager.cpp"


/**
 * Algorithm to determine the number of parameters in a raw commmand
 *  Basically making the difference between simple parameter like pk, pk+1 to "pk"
 *    Based on the search of the index of the quote
 *  Count the spaces
 *  Return the parameters as unsigned int >= 0
 */
unsigned int CommandsInterpreter::parametersCount(char data[], unsigned int data_size)
{
  String raw = String(data); //converting char array to string by simply invoking a String instance
  unsigned int parameters_count = 0; //declare the returned value

  /**
   * We admit that there's no more that one parameter of the "pk" form
   */
  int first_quote_index = raw.indexOf("\""); //"\" to escape the quote, can be < 0, so can't be unsigned
  if (first_quote_index != -1) //if one quote was found
  {
    int second_quote_index = raw.indexOf("\"", (first_quote_index + 1)); //even if
    if (second_quote_index != -1) //if both quotes were found
    {
      raw.remove((first_quote_index - 1), ((second_quote_index - first_quote_index) + 1)); //+1 because when we remove from first_quote_index the length : (second_quote_index - first_quote_index) we kept one space which was used for the quoted argument
      parameters_count++; //incrementing the parameters count, must be 0 at that time, so we can factorize it by parameters_count = 1
    }
  }

  //fuzzy but reconvert to char array, this method is the easiest way to count the space characters
  unsigned int temp_data_size = (raw.length() + 1); //+1 for the '\0'
  char temp_data[temp_data_size]; //data buffer
  raw.toCharArray(temp_data, temp_data_size); //str=>char[]

  for (unsigned int i = 0; i < temp_data_size; i++)
  {
    if ((int)temp_data[i] == 32) //32 is the decimal representation of a space from the ASCII table
    {
      parameters_count++; //incrementing the parameters count
    }
  }

  return parameters_count; //return the parameters count as unsigned int
}

/**
 * Command Router
 * Complex string analysis system based on booleans
 * Route the recognized command to the specified method in order to respond
 */
void CommandsInterpreter::str_command_router(char data[], unsigned int data_size) {

  //String cleaning center section
  String command = "";
  unsigned int parameters_count = parametersCount(data, data_size);
  String parameters[parameters_count];
  unsigned short spaces_iterator = 0;
  bool submiting_quote_in_progress = false;
  bool submiting_space_in_param = false;

  for (unsigned int i = 0; i < (data_size - 1); i++) // 0 <= i < (data_size -1), escaping the '\0'
  {
    char individual = data[i]; //getting back the char individually at the indice i
    int individual_decimal = (int)individual; //storing the integer representation of the char by casting it to an integer
    bool its_a_space = (individual_decimal == 32 ? true : false); //32 = " "
    bool its_a_quote = (individual_decimal == 34 ? true : false); //34 = "\""

    if (its_a_space)
    {
      if (submiting_quote_in_progress)
      {
        parameters[(spaces_iterator - 1)] += individual; //at the indice (space_iterator - 1), because space_iterator > 0 at this stage
        submiting_space_in_param = true;
      }
      else
      {
        spaces_iterator += 1;
      }
    }
    else if (its_a_quote)
    {
      if (!submiting_quote_in_progress)
      {
        submiting_quote_in_progress = true;
      }
      else
      {
        submiting_quote_in_progress = false;
      }
    }

    if (spaces_iterator == 0)
    {
      command += individual; //incrementing the command value as long as the space_iterator is null
    }
    else
    {
      if (!its_a_quote && !its_a_space)
      {
        if (!submiting_space_in_param)
        {
          parameters[(spaces_iterator - 1)] += individual;
        }
        else
        {
          submiting_space_in_param = false;
        }
      }
    }
  }

  /**
   * Conditionnal structure to route the specified command
   */
  if (command == INCOMING_SECURITY_STATUS)
  {
    security_status();
  }
  else if (command == INCOMING_PRIVATE_KEY)
  {
    private_key_in(parameters[0]);
  }
  else if (command == INCOMING_ASK_PAYLOADS_POOL)
  {
    outgoing_ask_payloads_pool();
  }
  else if (command == INCOMING_ASK_PAYLOAD)
  {
    if (parameters_count != 1) //checking parameters length, in a future update we'll raise exception
    {
      //error
      return; 
    }
    outgoing_ask_payload("", parameters[0].toInt()); //1st arg = hash null atm because security system is not created, converting the header_start_address to integer
  }
  else if (command == INCOMING_PAYLOAD_DELETE)
  {
    if (parameters_count != 1) //checking parameters length, in a future update we'll raise exception
    {
      //error
      return; 
    }
    outgoing_payload_delete(parameters[0].toInt());
  }
  else if (command == INCOMING_MEMORY_USAGE)
  {
    outgoing_memory_usage();
  }
  else if (command == INCOMING_PAYLOAD_ADD)
  {
    if (parameters_count != 3) //checking parameters length, in a future update we'll raise exception
    {
      return;
    }
    
    //getting back from the parameters array the Payload attributes
    String website = parameters[0];
    String username = parameters[1];
    String password = parameters[2];
    
    outgoing_payload_add(website, username, password);
  }
  else if (command == INCOMING_PAYLOAD_CHANGE)
  {
    if (parameters_count != 4) //checking parameters length, in a future update we'll raise exception
    {
      return; 
    }

    //getting back from the parameters array the Payload attributes
    unsigned int payload_head_start_address = parameters[0].toInt(); //converting the header_start_address to integer
    String website = parameters[1];
    String username = parameters[2];
    String password = parameters[3];

    outgoing_payload_modify(payload_head_start_address, website, username, password);
  }
}

/**
   In order to enroll new finger, secst must be unlocked or
*/

/**
   @desc = Send the current status to the manager software.
*/
void CommandsInterpreter::security_status()
{
  Serial.println(OUTGOING_SECURITY_STATUS + " " + String(SECURITY_STATUS));
}

/**
   @desc = Check if the sended private key is the right one.
   @return = (bool) : true if it's the right one.
*/
bool CommandsInterpreter::private_key_in(String private_key)
{
  bool ok = private_key == PRIVATE_KEY;
  //Serial.println(String(strcmp(private_key, PRIVATE_KEY)));
  if (ok)
  {
    if (SECURITY_STATUS == SECURITY_STATUS_LOCKED_FINGERPRINT_OK_WAITING_FOR_PRIVATE_KEY)
    {
      SECURITY_STATUS = SECURITY_STATUS_UNLOCKED;
    }
    else if (SECURITY_STATUS == SECURITY_STATUS_LOCKED_WAITING_FOR_BOTH_CHECK)
    {
      SECURITY_STATUS = SECURITY_STATUS_LOCKED_PRIVATE_KEY_OK_WAITING_FOR_FINGERPRINT;
    }
  }
  private_key_out(ok); //sending back result
  return ok;
}

/**
   @desc = Sending back a boolean as the result of the private key comparison.
*/
void CommandsInterpreter::private_key_out(bool ok)
{
  Serial.println("pkeys_out " + String(ok));
}

/**
   @desc = Send an outgoing message to the manager software.
*/
void CommandsInterpreter::outgoing_msg(String text)
{
  Serial.println("msg_out \"" + text + "\"");
}

void CommandsInterpreter::outgoing_ask_payload(String hash, unsigned int start_address)
{
    if (SECURITY_STATUS != SECURITY_STATUS_UNLOCKED)
    {
      outgoing_msg("board not fully unlocked");
      return;  
    }
    
    Payload p(start_address, false);
    
    String website = p.website;
    String username = p.username;
    String password = p.password;

    String data = OUTGOING_ASK_PAYLOAD + " " + website + " " + username + " " + password;
    Serial.println(data);
}

void CommandsInterpreter::outgoing_payload_delete(unsigned int start_address)
{
    if (SECURITY_STATUS != SECURITY_STATUS_UNLOCKED)
    {
      outgoing_msg("board not fully unlocked");
      return;  
    }
    
    Payload p(start_address, false);

    bool ok = p.deletion_malloc();
    
    String data = OUTGOING_PAYLOAD_DELETE;
    data += " " + String(ok);
    
    Serial.println(data);
}

void CommandsInterpreter::outgoing_memory_usage()
{
    Serial.println(OUTGOING_MEMORY_USAGE + " " + memoryUsageInPercent());
}

void CommandsInterpreter::outgoing_payload_add(String website, String username, String password)
{
    if (SECURITY_STATUS != SECURITY_STATUS_UNLOCKED)
    {
      outgoing_msg("board not fully unlocked");
      return;  
    }
    
    Payload p(nextAvailableAddress(), true);

    p.website = website;
    p.username = username;
    p.password = password;

    bool standard_malloc = p.standard_malloc();
    
    Serial.println(OUTGOING_PAYLOAD_ADD + " " + String(standard_malloc));
}

void CommandsInterpreter::outgoing_payload_modify(unsigned int start_address, String website, String username, String password)
{
    if (SECURITY_STATUS != SECURITY_STATUS_UNLOCKED)
    {
      outgoing_msg("board not fully unlocked");
      return;  
    }

    Payload p(start_address, false);
    bool mralloc = p.mralloc(website, username, password);

    Serial.println(OUTGOING_PAYLOAD_CHANGE + " " + String(mralloc));
}

void CommandsInterpreter::outgoing_ask_payloads_pool()
{
   if (SECURITY_STATUS != SECURITY_STATUS_UNLOCKED)
   {
      outgoing_msg("board not fully unlocked");
      return;  
   }
    
  size_t max_length = EEPROM.length();
  
  String command = "";
  command += OUTGOING_ASK_PAYLOADS_POOL + " ";
  
  for (unsigned int address = 0; address < max_length; address++)
  {
    byte readed_value = EEPROM.read(address);
        
    if (readed_value == PASSWORD_BLOCK_HEAD_START_PATTERN)//1 = start of "heading"; 4 = end of "transmission"; => Arbitrary choosen from ASCII table
    {
      Payload p(address, true);
      
      //p.website.remove(0, 1);
      command += String(address) + ";" + p.website + " ";
    }
    else if (readed_value == PASSWORD_BLOCK_HEAD_END_PATTERN)
    {
      byte forward_value = EEPROM.read(address + 1);
      
      if (forward_value != PASSWORD_BLOCK_HEAD_START_PATTERN) //stop the loop
        break;
    }
  }

  command.remove((command.length() - 1), 1); //removing uneeded space at the end
  
  Serial.println(command);
}

