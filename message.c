
#include "message.h"

/*
 Skapar ett meddelande (msg) från en given typ (type) och given parameterarray (param)
*/
void createMessage(uint8_t *msg, uint8_t type, uint8_t *param)
{
	msg = (uint8_t*)malloc(sizeof(param)+1);
	uint8_t firstbyte = (type << 5)|(sizeof(param));
	msg[0] = firstbyte;
	for(int i = 1; i<=sizeof(param);i++)
		msg[i] = param[i-1];
}

/*
 Returnerar meddelandets typ
*/
uint8_t getMessageType(uint8_t *msg){
	return (msg[0]>>5);
}