/**
 * IMPORTS,
 *  <x> : local computer file, located in the compiler directory
 *  "x" : local project file
 *  
 *  MAIN PROJECT FILE
 */
 
#include <MemoryFree.h>
#include <SoftwareSerial.h>
#include <FPS_GT511C3.h>
#include <EEPROM.h>

#include "Config.h"
#include "Payload.h"
#include "PayloadManager.cpp"
#include "CommandsInterpreter.h"
#include "Util.cpp"

static FPS_GT511C3 scanner(10, 11); // fingerprint scanner instance, RX (receiver data linked on pin 10), TX (transmitter data linked on pin 11)

static const unsigned int BUZZER_PIN = 2; //buzzer output pin

/*
 * Intialiazation
 *  Opening serial port
 *  Declaring outputs
 *  Opening communication between the fingerprint scanner and the Arduino
 */
void setup()
{
  Serial.begin(DEFAULT_TRANSMISSION_SPEED);
  print_debug("PPL onboard software - alpha 13517 (~stable)");

  assignPins();
  scanner.Open(); 
  
  //printBornedValues(0, 100);*/
}

/*
 * Declaring outputs
 */
void assignPins()
{
  pinMode(BUZZER_PIN, OUTPUT); //buzzer
}

/**
 * Loop execute the code inside nominaly at 16MHz, 16 000 iterations per second
 *  Checking security status
 *    Checking if receiving serial data from fingerprint scanner
 *      Checking if the given inputs is recognized as a known finger
 *        Changing security status  
 *          Tone the buzzer with unlock sound
 *        Tone the buzzer with locked sound
 *  Checking if there's incoming data on the serial port
 */
void loop()
{
  if (isWaitingForFingerprint()) //FINGERPINT HANDLER
  {
      scanner.SetLED(true);

      if (fingerDataIsIncoming())
      {
          if (fingerIsKnown())
          {
              if (SECURITY_STATUS == SECURITY_STATUS_LOCKED_PRIVATE_KEY_OK_WAITING_FOR_FINGERPRINT)
              {
                SECURITY_STATUS = SECURITY_STATUS_UNLOCKED;
              }
              else
              {
                SECURITY_STATUS = SECURITY_STATUS_LOCKED_FINGERPRINT_OK_WAITING_FOR_PRIVATE_KEY;
              }
              scanner.SetLED(false);
         
              CommandsInterpreter::outgoing_msg("Finger recognized");
              buzzer_finger_recognized();
          }
          else
          {
             CommandsInterpreter::outgoing_msg("Finger not recognized");
             buzzer_finger_not_recognized();
          }
      }
  }
  
  if (incomingData()) //CommandsInterpreter SECT
  {
    String incoming = Serial.readString(); //reading a string from Serial

    //treating string as a char array is easier to interprete raw command
    unsigned int incoming_length = incoming.length() + 1; //incoming length are always incremented length, because of the escape char '\0'
    char incoming_data_buffer[incoming_length]; //buffer (point where data are stored in the memory) initialized with the incoming_length
   
    incoming.toCharArray(incoming_data_buffer, incoming_length); //converting the string to char array  and put it in the data buffer
    CommandsInterpreter::str_command_router(incoming_data_buffer, incoming_length); //route the char array to the CommandsInterpreter
  }
}

/*
 * Checking by booleans if the SECURITY_STATUS is UNLOCKED
 */
bool isFullyUnlocked()
{
  return SECURITY_STATUS == SECURITY_STATUS_UNLOCKED;  
}

/**
 * Checking if there's incoming data, if incoming > 0 then there's
 */
bool incomingData()
{
  return Serial.available() > 0;  
}

/*
* Checking the status of unlock of the fingerprint scanner by checking the security status
*/
bool isWaitingForFingerprint()
{
  return SECURITY_STATUS == SECURITY_STATUS_LOCKED_WAITING_FOR_BOTH_CHECK || SECURITY_STATUS == SECURITY_STATUS_LOCKED_PRIVATE_KEY_OK_WAITING_FOR_FINGERPRINT;
}

/*
* Checking if the scanner is presed
*/ 
bool fingerDataIsIncoming()
{
  return scanner.IsPressFinger();
}

/**
* Checking if the inputed finger is known
*/
bool fingerIsKnown()
{
  scanner.CaptureFinger(false); //false for not saving as high quality fingerprint images 
  int finger_ID = scanner.Identify1_N(); //checkinf if the finger id is in the interval [0, 200]

  return finger_ID < 200; //if finger_ID <= 200 it's an unknown finger, why 200 ? because GT511C3 can handle 200 fingerprints*/
}

/**
 * Ringing the buzzer in order for the user to understand that his finger is recognized 
 */
void buzzer_finger_recognized()
{
  tone(BUZZER_PIN, 800); //tone(pin, 800Hz)
  delay(100); //sleeping the main thread for about 100ms, 1/10s
  tone(BUZZER_PIN, 600); //600Hz
  delay(500); //sleeping the main thread for about 500 ms, 1/2s
  noTone(BUZZER_PIN); //0Hz
}

/**
 * Ringing the buzzer in order for the user to understand that his finger is not recognized
 */
void buzzer_finger_not_recognized()
{
  tone(BUZZER_PIN, 200); //200Hz
  delay(500); //sleeping the main thread for about 500ms, 1/2s
  noTone(BUZZER_PIN);
}

/**
 * Debug void to print EEPROM values as hexadecimal in a interval to the Serial port
 * RAW, instable
 */
void printBornedValues(int a_start, int a_end){
  for (unsigned int i = a_start; i < a_end; i++){
    byte data = EEPROM.read(i);
    if (data < 16) {Serial.print("0");} //adding a 0 in order to respect the hex format if data < 16

    Serial.print(data, HEX);
  }
}




