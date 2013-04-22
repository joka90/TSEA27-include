#include "circularbuffer.h"

// Initierar en cirkulär buffer
//Struct partly copied from wikipedia
void cbInit (volatile CircularBuffer *cb, uint8_t size) 
{
    cb->size  = size;
    cb->start = 0;
    cb->end   = 0;
	cb->bytesUsed = 0;
    cb->bytes = (uint8_t*)malloc(cb->size);
}

void cbFree(volatile CircularBuffer *cb) {
    free(cb->bytes); /* OK if null */
}
 
uint8_t cbIsFull(volatile CircularBuffer *cb) {
    return (cb->bytesUsed == cb->size);
}
 
uint8_t cbIsEmpty(volatile CircularBuffer *cb) {
    return (cb->bytesUsed == 0); 
}

uint8_t cbBytesUsed(volatile CircularBuffer *cb)
{
	return cb->bytesUsed;
}	

uint8_t cbBytesFree(volatile CircularBuffer *cb)
{
	return (cb->size-cb->bytesUsed);
}

uint8_t cbPeek(volatile CircularBuffer *cb)
{
	return cb->bytes[cb->start];
}
 
/* Write an element, overwriting oldest element if buffer is full. App can
   choose to avoid the overwrite by checking cbIsFull(). */
void cbWrite(volatile CircularBuffer *cb, uint8_t elem) {
    cb->bytes[cb->end] = elem;
    cb->end = (cb->end + 1) % cb->size;
    if (cb->end == cb->start)
	{
        cb->start = (cb->start + 1) % cb->size; /* full, overwrite */
	}		
	else
	{
		cb->bytesUsed = cb->bytesUsed+1;
	}		
}

/* Read oldest element. App must ensure !cbIsEmpty() first. */
uint8_t cbRead(volatile CircularBuffer *cb) {
	if(cb->bytesUsed == 0)
	{
		return 0xff;
	}
	uint8_t oldstart = cb->start;
	cb->bytesUsed = cb->bytesUsed-1;
	cb->start = (cb->start + 1) % cb->size;
    return cb->bytes[oldstart];
}