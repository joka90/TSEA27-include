//// PRIVATA (INTERNA) VARIABLER
////   -Används ej utanför uart.c
#include "../circularbuffer.h"

CircularBuffer _rxMessageBuffer; // Buffer för mottagna meddelanden
uint8_t _noOfMessagesInBuffer; // Antalet hela meddelanden i _rxMessageBuffer
uint8_t _remainingBytes; // Anger antalet återstående bytes som ännu inte läst in, i ett meddelande som håller på att tas emot

#ifndef UART_H
#define UART_H


//// PUBLIKA FUNKTIONER

/*
Sparar ovanstående på skrivbuffern samt startar skrivningen vilken upphör när hela buffern skrivit klart.
Returnerar 0 för fel, 1 för lyckad sparning.
*/
uint8_t UART_writeMessage(uint8_t msg[]);

/*
Läser in nästa datapaket från buffern och sparar det i msg, samt dess längd i len. len sparas i bit 5 till 7 i paketet.
Returnerar 0 för fel (om buffern var tom), 1 för lyckad läsning.
*/
uint8_t UART_readMessage(uint8_t *msg);

/*
Ställer in alla register samt hastighet för porten.
*/
void UART_init();

#endif
