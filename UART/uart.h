#ifndef UART_H
#define UART_H

#include "../circularbuffer.h"

//// PRIVATA (INTERNA) VARIABLER
////   -Används ej utanför uart.c
CircularBuffer _rxMessageBuffer; // Buffer för mottagna meddelanden

//// PUBLIKA FUNKTIONER
//uint8_t handshaken;

void UART_writeSyncMessage();

/*
Sparar ovanstående på skrivbuffern samt startar skrivningen vilken upphör när hela buffern skrivit klart.
Returnerar 0 för fel, 1 för lyckad sparning.
*/
uint8_t UART_writeMessage(uint8_t* msg, uint8_t type, uint8_t size);

/*
Läser in nästa datapaket från buffern och sparar det i msg, samt dess längd i len. len sparas i bit 5 till 7 i paketet.
Returnerar 0 för fel (om buffern var tom), 1 för lyckad läsning.
*/
uint8_t UART_readMessage(uint8_t *msg, uint8_t *type, uint8_t *length);

/*
Ställer in alla register samt hastighet för porten.
*/
void UART_init();

/*
Handskakar med PCn
*/
void UART_handshake();

#endif
