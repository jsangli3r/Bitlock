#include <Arduino.h>
#include <EEPROM.h>

#include "PayloadManager.cpp"
#include "Payload.h"
#include "Util.cpp"

/**
 * @desc = Payload object constructor.
 */
Payload::Payload(unsigned int payload_head_start_address, bool stop_after_completing_website)
{
    //début séquençage EEPROM (récupération des données)
    size_t eeprom_length = EEPROM.length();
    unsigned int data_start_iterations = 0;
    unsigned int data_end_iterations = 0;
    unsigned int iterator_start = (payload_head_start_address + 1);
    unsigned int payload_head_end_address = 0;

    head_start_address = payload_head_start_address;
    
    for (unsigned int i = iterator_start; i < eeprom_length; i++) //boucle exploratrice (dora), navigue entre (PATTERN_HEAD_START + 1) et PATTERN_HEAD_END dans l'EEPROM
    { 
      byte value = EEPROM.read(i); //valeur lue

      if (i == iterator_start && EEPROM.read(i - 1) != PASSWORD_BLOCK_HEAD_START_PATTERN) //block doesn't exists
      {
        break; //stop the loop from running 
      }

      //finding data by an interating process
      if (value == PASSWORD_BLOCK_DATA_START_PATTERN) //début séquence données, chaque déb = 2 / fin = 3 (valeurs de références)
      { 
          data_start_iterations++;
      }
      else if (value == PASSWORD_BLOCK_DATA_END_PATTERN) //fin séquence données
      { 
        data_end_iterations++;
      }
      else if (value == PASSWORD_BLOCK_HEAD_END_PATTERN) //fin payload, arrêt de la boucle
      {
        head_end_address = i;
        break;
      }
      else
      {
        char c = value; //conversion valeur decimal vers caractère via la table ASCII

        if (data_start_iterations == 1 && data_end_iterations == 0)
        {
          website += c; //concact website
        }
        else if (data_start_iterations == 2 && data_end_iterations == 1)
        { 
          if (stop_after_completing_website)
          {
            break; //stop the loop
          }
          username += c;  //concact username 
        }
        else if (data_start_iterations == 3 && data_end_iterations == 2)
        {
          password += c; //concact password
        }
      }
    }
}

/**
 * Delete current payload from EEPROM.
 * Might works
 */
bool Payload::deletion_malloc()
{
  unsigned int eeprom_total_length = EEPROM.length();
  unsigned int last_payload_end_address = lastPayloadEndAddress();
  unsigned int payload_offset_diff = (head_end_address - head_start_address); //payload length equivalent to payload_size()
  //Serial.println("pod = " + String(payload_offset_diff));
  
  for (unsigned int i = head_start_address; i < last_payload_end_address; i++)
  {
      if (i >= head_start_address && i <= head_end_address) //if head_start_address <= i <= head_end_address then filling the EEPROM with 0
      {
        //Serial.println("0 at " + String(i));
        EEPROM.write(i, (byte)0);  
      }
      else
      {
        unsigned short value = EEPROM.read(i);
      
        /*if (value == PASSWORD_BLOCK_HEAD_END_PATTERN && EEPROM.read((i + 1)) == 0){
            break;
        }*/

        Serial.println("writing " + String(value) + " at " + (abs(i - payload_offset_diff)));
        EEPROM.write((abs(i - payload_offset_diff)), value); //writing at (i - payload_offset_diff)
      }
  }

  return true;
}

/**
 * Memory Reallocation : update EEPROM memory using another block.
 */
bool Payload::mralloc(String new_website, String new_username, String new_password)
{
  unsigned int eeprom_total_length = EEPROM.length();
  unsigned int last_payload_available_end_address = lastPayloadEndAddress();
  unsigned int plength = payload_size();
  unsigned int pprime_length = (new_website.length() + new_username.length() + new_password.length() + 2^3); //payload_size sum(attributs_length) + 2*header + 3*2 data_heads = 2^3s
  unsigned int predicted_head_end_address = 0;

  Serial.println("pl=" + String(plength));
  Serial.println("ppl=" + String(pprime_length));
  Serial.println("lpa=" + String(last_payload_available_end_address));
  
  if (pprime_length > plength)
  {
    predicted_head_end_address = pprime_length + head_start_address - 1;
    last_payload_available_end_address += (pprime_length - plength);

    Serial.println("pea=" + String(predicted_head_end_address));
    Serial.println("lpa=" + String(last_payload_available_end_address));
    
    if (predicted_head_end_address > eeprom_total_length)
    {
      
    }
    
    unsigned int next_payload_collision_start_address = 0;
    unsigned int next_payload_collision_end_address = 0;
    
    for (unsigned int i = (head_end_address + 1); i < eeprom_total_length; i++)
    {
      unsigned short value = EEPROM.read(i);

      if (value == PASSWORD_BLOCK_HEAD_START_PATTERN)
      {
        next_payload_collision_start_address = i; //break
      }
      else if (value == PASSWORD_BLOCK_HEAD_END_PATTERN)
      {
        next_payload_collision_end_address = i; //break
        break;
        /*if (next_payload_collision_start_address != 0){ //next_password_block_collision_end_address : événement certain
          break;
        }*/
      }
    }

    Serial.println("npsa=" + String(next_payload_collision_start_address));
    Serial.println("npea=" + String(next_payload_collision_end_address));
    
    for (unsigned int a = next_payload_collision_start_address; a <= last_payload_available_end_address; a++)
    {
      Serial.println("a=" + String(a));
      unsigned short value = EEPROM.read(a);
      Serial.println(value);
      unsigned int new_address = a + (pprime_length - plength);
      Serial.println("na=" + String(new_address));
      
      EEPROM.write(new_address, value);
    }
  }
  else
  {
    predicted_head_end_address = head_start_address - pprime_length;
    if (predicted_head_end_address < 0)
    {
      
    }
    
    unsigned int before_payload_collision_start_address = 0;
    unsigned int before_payload_collision_end_address = 0;
    
    for (unsigned int i = (head_start_address - 1); i < 0; i--)
    {
      unsigned short value = EEPROM.read(i);

      if (value == PASSWORD_BLOCK_HEAD_START_PATTERN)
      {
        before_payload_collision_start_address = i; //break
      }
      else if (value == PASSWORD_BLOCK_HEAD_END_PATTERN)
      {
        before_payload_collision_end_address = i; //break
        break;
        /*if (next_payload_collision_start_address != 0){ //next_password_block_collision_end_address : événement certain
          break;
        }*/
      }
    }

    for (unsigned int i = before_payload_collision_start_address; i < last_payload_available_end_address; i++)
    {
      unsigned short value = EEPROM.read(i);
      unsigned int new_address = (predicted_head_end_address + 1) + i;
      
      EEPROM.write(new_address, value);
    }
  }

  
  website = new_website;
  username = new_username;
  password = new_password;

  head_start_address = predicted_head_end_address - pprime_length + 1;
  head_end_address = predicted_head_end_address;
  
  standard_malloc();
  /*unsigned int l1 = website.length();
  unsigned int l2 = username.length();
  unsigned int l3 = password.length();
  unsigned int l1p = new_website.length();
  unsigned int l2p = new_username.length();
  unsigned int l3p = new_password.length();

  //reasign local var for standard malloc
  if (new_website != website)
  {
    website = new_website;
  }
  if (new_username != username)
  {
    username = new_username;
  }
  if (new_password != password)
  {
    password = new_password;
  }
  
  //if the lengths are not equals we must make a progressive malloc, either a standard malloc is orderedb
  bool progressive_malloc_needed = (l1p != l1 || l2p != l2 || l3p != l3);
  if (!progressive_malloc_needed) //no need for a progressive malloc
  {
     standard_malloc();
     return false;
  }

  Serial.println("PMalloc started");
  
  unsigned int eeprom_total_length = EEPROM.length();

  //ERROR CAN'T HANDLE NEGATIVE LENGTH CHANGES
  int website_dist = (l1p - l1);
  int username_dist = (l2p - l2);
  int password_dist = (l3p - l3);
  int dist_sum = website_dist + username_dist + password_dist; //somme de toutes les différences

  //unsigned int predicted_new_head_start_address = head_start_address; Adresse de départ du bloc fixe
  unsigned int predicted_new_head_end_address = head_end_address + dist_sum;

  //next_block_finder
  unsigned short next_password_block_collision_start_address = 0;
  unsigned short next_password_block_collision_end_address = 0;

  for (unsigned int i = (head_end_address + 1); i < eeprom_total_length; i++)
  {
      unsigned short value = EEPROM.read(i);

      if (value == PASSWORD_BLOCK_HEAD_START_PATTERN){
        next_password_block_collision_start_address = i; //break
      }
      else if (value == PASSWORD_BLOCK_HEAD_END_PATTERN){
        next_password_block_collision_end_address = i; //break

        if (next_password_block_collision_start_address != 0){ //next_password_block_collision_end_address : événement certain
          break;
        }
      }
  }

  if ((predicted_new_head_end_address + next_password_block_collision_start_address) > eeprom_total_length){
    return false;
  }

  size_t eeprom_buffer_size = (eeprom_total_length - next_password_block_collision_start_address);
  unsigned short last_payload_address = nextAvailableAddress();
  byte eeprom_buffer[eeprom_buffer_size];
  unsigned short eeprom_buffer_iterator = 0;
  for (unsigned int i = next_password_block_collision_start_address; i < last_payload_address; i++)
  {
      unsigned short value = EEPROM.read(i);

      eeprom_buffer[eeprom_buffer_iterator] = value;
      eeprom_buffer_iterator += 1;
  }
  
  boolean malloc_succeed = standard_malloc();
  if (!malloc_succeed)
  {
    return false;
  }

  for (unsigned int i = 0; i < eeprom_buffer_size; i++)
  {
    unsigned short address = ((predicted_new_head_end_address + 1) + i);

    EEPROM.write(address, eeprom_buffer[i]);
  }

  return true;*/
}

/**
 * Allocate current payload instance data into EEPROM.
 */
bool Payload::standard_malloc()
{
  print_debug("standard_malloc executed");
  int payload_lengths[3] = { website.length(), username.length(), password.length() };

  //checking if every attribute is not 
  for (unsigned int i = 0; i < 3; i++){
    print_debug("payload_lengths[" + String(i) + "] : " + String(payload_lengths[i]));
    if (payload_lengths[i] > 64){
      return false;
    }
  }

  //payloads chars buffer, 3 rows (3 attributes) with a maxium of 64 columns (characters)
  char payload_chars[3][64];

  //convert every string payload attributes to char array in order to get them individually
  website.toCharArray(payload_chars[0], (payload_lengths[0] + 1)); //+ 1 to buffer length because of the end character presence "\0" ? WTF that's non sense, because "\0" is not filled in the memory ...
  username.toCharArray(payload_chars[1], (payload_lengths[1] + 1));
  password.toCharArray(payload_chars[2], (payload_lengths[2] + 1));

  unsigned int address = head_start_address;
  print_debug("Writing head_start_pattern at : 0x" + String(address)); 
  EEPROM.write(address, PASSWORD_BLOCK_HEAD_START_PATTERN); //block start
  address += 1;

  print_debug("Factorised writing process is starting");
  for (unsigned int i = 0; i < 3; i++) //Factorised loop to write payload attributes into the EEPROM
  {
      print_debug("FW i = " + String(i));
      for (unsigned int j = 0; j <= payload_lengths[i]; j++)
      {
          print_debug("FW j = " + String(j));
          unsigned short ascii_index = (int)payload_chars[i][j]; //get the decimal representation of the char located at [i][j]
          print_debug("ascii_index = " + String(ascii_index) + " | payload_chars[" + String(i) + "][" + String(j) + "]");
          
          if (j == payload_lengths[i])
          {
            print_debug("Writing block_data_end_pattern at 0x" + String(address));
            
            EEPROM.write(address, PASSWORD_BLOCK_DATA_END_PATTERN);
          }
          else
          {
            if (j == 0)
            {
              print_debug("Writing block_data_start_pattern at 0x" + String(address));
              EEPROM.write(address, PASSWORD_BLOCK_DATA_START_PATTERN);
              address += 1;
            }

            print_debug("Writing ascii index : 0x" + String(ascii_index) + " at 0x" + String(address));
            EEPROM.write(address, ascii_index);
          }

          address += 1;
          print_debug("Current address : " + String(address));
      }
  }

  print_debug("Writing block_head_end_pattern at 0x" + String(address));
  EEPROM.write(address, PASSWORD_BLOCK_HEAD_END_PATTERN);
  return true;
}

unsigned int Payload::payload_size()
{
  return (website.length() + username.length() + password.length() + 2^3);  
}

/**
 * Future implementation for cryptography (BOTH)
 */
void Payload::cipher()
{
}

void Payload::uncipher()
{
}



