/*
 * TSEA27 Elektronikprojekt
 *
 * IDENTIFIERING
 *
 * Modul: Alla
 * Filnamn: message.c
 * Skriven av: T. Norlund
 * Datum: 2013-05-15
 * Version: 1.0
 *
 * BESKRIVNING
 *
 * Funktonalitet f�r att hantera meddelanden som skickas mellan delsystemen
 */

#ifndef MESSAGE_H
#define MESSAGE_H
#include <stdint.h>

// Default
#define TYPE_CMD_EXCHANGE_DATA        0x07 // 111

// Styr skicka
#define TYPE_REQUEST_SENSOR_DATA 0x00 // 000
#define TYPE_REQUEST_PC_MESSAGE  0x01 // 001
#define TYPE_DEBUG_DATA			 0x02 // 010
#define TYPE_MAP_DATA			 0x03 // 011

// Styr taemot
#define TYPE_SENSOR_DATA		 0x00 // 000
#define TYPE_MANUAL_COMMAND		 0x05 // 101 , kan ej anv�nda 0x01 001 pga krock
#define TYPE_CHANGE_PARM		 0x02 // 010
#define TYPE_NO_PC_MESSAGES		 0x03 // 011
#define TYPE_EMERGENCY_STOP		 0x04 // 100

// Sensor taemot
#define TYPE_REQUEST_SENSOR_DATA 0x00 // 000
#define TYPE_CALIBRATE_GYRO		 0x01 // 001

// Sensor skicka
#define TYPE_REPLY_SENSOR_DATA   0x00 // 000
#define TYPE_REPLY_CALIBRATE_GYRO 0x01 // 001

// ID
#define LONGFRONT 0x00
#define LONGRIGHT 0x01
#define LONGREAR 0x02
#define LONGLEFT 0x03
#define SHORTFRONTRIGHT 0x04
#define SHORTFRONTLEFT 0x05
#define SHORTREARRIGHT 0x06
#define SHORTREARLEFT 0x07
#define IDGYROSENSOR 0x08
#define IDSPEEDRIGHT 0x09
#define IDSPEEDLEFT 0x0a

// Reglerparam
#define	L1_STRAIGHTX 0x00
#define L2_STRAIGHTTHETA 0x01
#define L3_STRAIGHTOMEGA 0x02
#define L1_TURNTHETA 0x03
#define L2_TURNOMEGA 0x04
#define PARAMRIGHTCUSTOM 0x05
#define PARAMLEFTCUSTOM 0x06

// Styrkommandon
#define FORWARD_COMMAND 0
#define RIGHT_90_COMMAND 1
#define LEFT_90_COMMAND 2
#define RIGHT_45_COMMAND 3
#define LEFT_45_COMMAND 4
#define VIRTUAL_REVERSE_COMMAND 5
#define CUSTOM_STEERING_COMMAND 6
#define NULL_COMMAND 7
#define TURN_FINE 8
#define START 1 
#define END 0 

// Makros f�r att koda/avkoda headern
#define GET_LEN(x) (x)&0b00011111
#define SET_LEN(x) (x)

#define GET_TYPE(x) (((x)&0b11100000)>>5)
#define SET_TYPE(x) ((x)<<5)

#define CREATE_HEAD(x,y) ((x)<<5)|(y)

/*
 Skapar ett meddelande (msg) fr�n en given typ (type) och given parameterarray (param)
*/
void createMessage(uint8_t* msg, uint8_t type, uint8_t* param, uint8_t param_size);

/*
 Returnerar meddelandets typ
*/
uint8_t getMessageType(uint8_t *msg);

#endif
