#ifndef MESSAGE_H
#define MESSAGE_H
#include <stdint.h>

//exchange databit 
#define TYPE_CMD_EXCHANGE_DATA        0x07 // 111

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

//sensor taemot
#define TYPE_REQUEST_SENSOR_DATA 0x00 // 000

//sensor skicka
#define TYPE_REPLY_SENSOR_DATA   0x00 // 000

#define IDSENSOR1 0x00
#define IDSENSOR2 0x01
#define IDSENSOR3 0x02
#define IDSENSOR4 0x03
#define IDSENSOR5 0x04
#define IDSENSOR6 0x05
#define IDSENSOR7 0x06
#define IDSENSOR8 0x07
#define IDGYROSENSOR 0x08
#define IDSPEEDRIGHT 0x09
#define IDSPEEDLEFT 0x0a

//reglerparam
#define PARAMLEFTCUSTOM 0x00
#define PARAMRIGHTCUSTOM 0x01

//styrkommandon
#define FORWARD_COMMAND 0
#define RIGHT_90_COMMAND 1
#define LEFT_90_COMMAND 2
#define RIGHT_45_COMMAND 3
#define LEFT_45_COMMAND 4
#define VIRTUAL_REVERSE_COMMAND 5
#define CUSTOM_STEERING_COMMAND 6

#define START 1 
#define END 0 

//macros
#define BIT(x) (1 << (x))
#define SETBITS(x,y) ((x) |= (y))
#define CLEARBITS(x,y) ((x) &= (~(y)))
#define SETBIT(x,y) SETBITS((x), (BIT((y))))
#define CLEARBIT(x,y) CLEARBITS((x), (BIT((y))))
#define BITSET(x,y) ((x) & (BIT(y)))
#define BITCLEAR(x,y) !BITSET((x), (y))
#define BITSSET(x,y) (((x) & (y)) == (y))
#define BITSCLEAR(x,y) (((x) & (y)) == 0)
#define BITVAL(x,y) (((x)>>(y)) & 1)

//macros to code/decode messages
#define GET_LEN(x) (x)&0b00011111
#define SET_LEN(x) (x)

#define GET_TYPE(x) (((x)&0b11100000)>>5)
#define SET_TYPE(x) ((x)<<5)

#define CREATE_HEAD(x,y) ((x)<<5)|(y)

/*
 Skapar ett meddelande (msg) från en given typ (type) och given parameterarray (param)
*/
void createMessage(uint8_t* msg, uint8_t type, uint8_t* param, uint8_t param_size);

/*
 Returnerar meddelandets typ
*/
uint8_t getMessageType(uint8_t *msg);

#endif
