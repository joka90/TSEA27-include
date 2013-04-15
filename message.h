#ifndef MESSAGE_H
#define MESSAGE_H

#include <stdint.h>

//exchange databit 
#define CMD_EXCHANGE_DATA        0xE0 // 111 0 0000

//styr skicka
#define TYPE_REQUEST_SENSOR_DATA 0x00 // 000
#define TYPE_REQUEST_PC_MESSAGE  0x01 // 001
#define TYPE_DEBUG_DATA			 0x02 // 010
#define TYPE_MAP_DATA			 0x03 // 011

//styr taemot
#define TYPE_SENSOR_DATA		 0x00 // 000
#define TYPE_MANUAL_COMMAND		 0x01 // 001
#define TYPE_CHANGE_PARM		 0x02 // 010
#define TYPE_NO_PC_MESSAGES		 0x03 // 011
#define TYPE_EMERGENCY_STOP		 0x04 // 100

#define START 1 
#define END 0 

/*
 Skapar ett meddelande (msg) från en given typ (type) och given parameterarray (param)
*/
void createMessage(uint8_t* msg, uint8_t type, uint8_t* param, uint8_t param_size);

/*
 Returnerar meddelandets typ
*/
uint8_t getMessageType(uint8_t *msg);

#endif
