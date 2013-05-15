/**
 * TSEA27 Elektronikprojekt
 *
 * IDENTIFIERING
 *
 * Modul: Kommunikationsenheten
 * Filnamn: uart.h
 * Skriven av: T. Norlund, J. Källström
 * Datum: 2013-05-15
 * Version: 1.0
 *
 * BESKRIVNING
 *
 * Funktonalitet för att kommunicera via UART
 */

#ifndef UART_H
#define UART_H

#include "../circularbuffer.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

//// PRIVATA (INTERNA) VARIABLER
////   -Används ej utanför uart.c
CircularBuffer _rxMessageBuffer; // Buffer för mottagna meddelanden

//// PUBLIKA FUNKTIONER

/*
 Skickar en s.k. synkroniseringssvärm (5 0xFF i rad). Används för att PC:n ska vara
 säker på att det som kommer därefter är början på ett nytt meddelande
*/
void UART_writeSyncMessage();

/*
 Skickar meddelandet som ges av msg, byte för byte.
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


#endif
