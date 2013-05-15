/**
 * TSEA27 Elektronikprojekt
 *
 * IDENTIFIERING
 *
 * Modul: Alla
 * Filnamn: circularbuffer.h
 * Skriven av: T. Norlund
 * Datum: 2013-05-15
 * Version: 1.0
 *
 * BESKRIVNING
 *
 * Implementerar en cirkul�r buffer, som anv�nds f�r all lagra meddelanden
 */

#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <stdint.h>
#include <stdlib.h>
 
/* Cirkul�r buffer (mestadels kopierad fr�n wikipedia) */
typedef struct {
    uint8_t         size;   /* maximum number of elements           */
    uint8_t         start;  /* index of oldest element              */
    uint8_t         end;    /* index at which to write new element  */
	uint8_t			bytesUsed;
    uint8_t*     bytes;  /* vector of bytes                   */
} CircularBuffer;

/*
* Initiera och allocera minne f�r en cirkul�rbuffer av storlek size. 
*/
void cbInit(volatile CircularBuffer *cb, uint8_t size);

/*
* Deallokera minnet f�r den circul�ra buffern.
*/
void cbFree(volatile CircularBuffer *cb);

/*
* Returnerar 1 om buffern �r full 0 om inte.
*/
uint8_t cbIsFull(volatile CircularBuffer *cb);

/*
* Returnerar 1 om buffern �r tom 0 om inte.
*/
uint8_t cbIsEmpty(volatile CircularBuffer *cb);

/*
* Returnerar hur m�nga bytes som anv�nds i buffern.
*/
uint8_t cbBytesUsed(volatile CircularBuffer *cb);

/*
* Returnerar hur m�nga bytes som �r lediga i buffern.
*/
uint8_t cbBytesFree(volatile CircularBuffer *cb);

/*
* Returnerar �lsta elementet i buffern utan att �ndra p� start och slutpekarna.
*/
uint8_t cbPeek(volatile CircularBuffer *cb);

/* 
 * Skriv en ny byte till buffern. Skriver �ver f�r gamla element. Undvik det genom att l�sa av cbIsFull innan.
 */
void cbWrite(volatile CircularBuffer *cb, uint8_t elem);

/*
 * Returnerar �lsta elementet i buffern.
 */
uint8_t cbRead(volatile CircularBuffer *cb);

#endif
