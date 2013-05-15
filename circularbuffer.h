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
 * Implementerar en cirkulär buffer, som används för all lagra meddelanden
 */

#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <stdint.h>
#include <stdlib.h>
 
/* Cirkulär buffer (mestadels kopierad från wikipedia) */
typedef struct {
    uint8_t         size;   /* maximum number of elements           */
    uint8_t         start;  /* index of oldest element              */
    uint8_t         end;    /* index at which to write new element  */
	uint8_t			bytesUsed;
    uint8_t*     bytes;  /* vector of bytes                   */
} CircularBuffer;

/*
* Initiera och allocera minne för en cirkulärbuffer av storlek size. 
*/
void cbInit(volatile CircularBuffer *cb, uint8_t size);

/*
* Deallokera minnet för den circulära buffern.
*/
void cbFree(volatile CircularBuffer *cb);

/*
* Returnerar 1 om buffern är full 0 om inte.
*/
uint8_t cbIsFull(volatile CircularBuffer *cb);

/*
* Returnerar 1 om buffern är tom 0 om inte.
*/
uint8_t cbIsEmpty(volatile CircularBuffer *cb);

/*
* Returnerar hur många bytes som används i buffern.
*/
uint8_t cbBytesUsed(volatile CircularBuffer *cb);

/*
* Returnerar hur många bytes som är lediga i buffern.
*/
uint8_t cbBytesFree(volatile CircularBuffer *cb);

/*
* Returnerar älsta elementet i buffern utan att ändra på start och slutpekarna.
*/
uint8_t cbPeek(volatile CircularBuffer *cb);

/* 
 * Skriv en ny byte till buffern. Skriver över för gamla element. Undvik det genom att läsa av cbIsFull innan.
 */
void cbWrite(volatile CircularBuffer *cb, uint8_t elem);

/*
 * Returnerar älsta elementet i buffern.
 */
uint8_t cbRead(volatile CircularBuffer *cb);

#endif
