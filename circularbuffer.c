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

#include "circularbuffer.h"

/*
* Initiera och allocera minne för en cirkulärbuffer av storlek size.
*/
void cbInit (volatile CircularBuffer *cb, uint8_t size) 
{
    cb->size  = size;
    cb->start = 0;
    cb->end   = 0;
	cb->bytesUsed = 0;
    cb->bytes = (uint8_t*)malloc(cb->size);
}

/*
* Deallokera minnet för den circulära buffern.
*/
void cbFree(volatile CircularBuffer *cb) {
    free(cb->bytes); /* OK if null */
}
 
/*
* Returnerar 1 om buffern är full 0 om inte.
*/
uint8_t cbIsFull(volatile CircularBuffer *cb) {
    return (cb->bytesUsed == cb->size);
}
 
/*
* Returnerar 1 om buffern är tom 0 om inte.
*/
uint8_t cbIsEmpty(volatile CircularBuffer *cb) {
    return (cb->bytesUsed == 0); 
}

/*
* Returnerar hur många bytes som används i buffern.
*/
uint8_t cbBytesUsed(volatile CircularBuffer *cb)
{
	return cb->bytesUsed;
}	

/*
* Returnerar hur många bytes som är lediga i buffern.
*/
uint8_t cbBytesFree(volatile CircularBuffer *cb)
{
	return (cb->size-cb->bytesUsed);
}

/*
* Returnerar älsta elementet i buffern utan att ändra på start och slutpekarna.
*/
uint8_t cbPeek(volatile CircularBuffer *cb)
{
	return cb->bytes[cb->start];
}
 
/* 
 * Skriv en ny byte till buffern. Skriver över för gamla element. Undvik det genom att läsa av cbIsFull innan.
 */
void cbWrite(volatile CircularBuffer *cb, uint8_t elem) {
    cb->bytes[cb->end] = elem;
	uint8_t newEnd = cb->end+1;
	if(cb->size<=newEnd)
	{
		newEnd=0;
	}
    cb->end = newEnd;//(cb->end + 1) % cb->size;
    if (cb->end == cb->start)
	{
        	uint8_t newStart = cb->start+1;
        	if(cb->size<=newStart)
        	{
	        	newStart=0;
        	}
        	cb->start = newStart; /* full, overwrite */
	}		
	else
	{
		cb->bytesUsed = cb->bytesUsed+1;
	}		
}

/*
 * Returnerar älsta elementet i buffern.
 */
uint8_t cbRead(volatile CircularBuffer *cb) {
	if(cb->bytesUsed == 0)
	{
		return 0xff;
	}
	uint8_t oldstart = cb->start;
	cb->bytesUsed = cb->bytesUsed-1;
	uint8_t newStart = cb->start+1;
	if(cb->size<=newStart)
	{
		newStart=0;
	}
	cb->start = newStart;
    return cb->bytes[oldstart];
}