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

#include "circularbuffer.h"

/*
* Initiera och allocera minne f�r en cirkul�rbuffer av storlek size.
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
* Deallokera minnet f�r den circul�ra buffern.
*/
void cbFree(volatile CircularBuffer *cb) {
    free(cb->bytes); /* OK if null */
}
 
/*
* Returnerar 1 om buffern �r full 0 om inte.
*/
uint8_t cbIsFull(volatile CircularBuffer *cb) {
    return (cb->bytesUsed == cb->size);
}
 
/*
* Returnerar 1 om buffern �r tom 0 om inte.
*/
uint8_t cbIsEmpty(volatile CircularBuffer *cb) {
    return (cb->bytesUsed == 0); 
}

/*
* Returnerar hur m�nga bytes som anv�nds i buffern.
*/
uint8_t cbBytesUsed(volatile CircularBuffer *cb)
{
	return cb->bytesUsed;
}	

/*
* Returnerar hur m�nga bytes som �r lediga i buffern.
*/
uint8_t cbBytesFree(volatile CircularBuffer *cb)
{
	return (cb->size-cb->bytesUsed);
}

/*
* Returnerar �lsta elementet i buffern utan att �ndra p� start och slutpekarna.
*/
uint8_t cbPeek(volatile CircularBuffer *cb)
{
	return cb->bytes[cb->start];
}
 
/* 
 * Skriv en ny byte till buffern. Skriver �ver f�r gamla element. Undvik det genom att l�sa av cbIsFull innan.
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
 * Returnerar �lsta elementet i buffern.
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