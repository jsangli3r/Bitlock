#include <Arduino.h>

#ifndef PAYLOAD_H
#define PAYLOAD_H

class Payload
{
  public:
    String website; 
    String username;
    String password; 
    
    unsigned int head_start_address;
    unsigned int head_end_address;
   
    Payload(unsigned int address, bool end_after_completing_website);

    bool standard_malloc();
    bool mralloc(String website, String username, String password);
    bool deletion_malloc();

    unsigned int payload_size();
    
    void uncipher();
    void cipher();
};

#endif



