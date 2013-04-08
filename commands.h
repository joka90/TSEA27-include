#ifndef COMMANDS_H
#define COMMANDS_H


//exchange databit 
#define CMD_EXCHANGE_DATA        0xE0 // 111 0 0000

//styr skicka
#define CMD_STYR_GET_SENSOR_DATA 0x00 // 000 0 0000
#define CMD_STYR_GET_PC_MESSAGE  0x20 // 001 0 0000
#define CMD_STYR_SEND_DEBUG_DATA 0x40 // 010 0 0000
#define CMD_STYR_SEND_MAP_DATA   0x60 // 011 0 0000

//styr taemot
#define CMD_STYR_SENSOR_DATA     0x00 // 000 0 0000
#define CMD_STYR_MANUAL_COMMAND  0x20 // 001 0 0000
#define CMD_STYR_CHANGE_PARM     0x40 // 010 0 0000
#define CMD_STYR_NO_PC_MESSAGES  0x60 // 011 0 0000

#define START 1 
#define END 0 

#endif
