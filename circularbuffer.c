#include "circularbuffer.h"

// Initierar en cirkulär buffer
//Struct partly copied from wikipedia
void cbInit (CircularBuffer *cb, uint8_t size) 
{
    cb->size  = size;
    cb->start = 0;
    cb->end   = 0;
	cb->bytesUsed = 0;
    cb->bytes = (uint8_t*)malloc(cb->size);
}

void cbFree(CircularBuffer *cb) {
    free(cb->bytes); /* OK if null */
}
 
int cbIsFull(CircularBuffer *cb) {
    return (cb->bytesUsed == cb->size);
}
 
int cbIsEmpty(CircularBuffer *cb) {
    return (cb->bytesUsed == 0); 
}

uint8_t cbBytesUsed(CircularBuffer *cb)
{
	return cb->bytesUsed;
}	

uint8_t cbBytesFree(CircularBuffer *cb)
{
	return (cb->size-cb->bytesUsed);
}

uint8_t cbPeek(CircularBuffer *cb)
{
	return cb->bytes[cb->start];
}
 
/* Write an element, overwriting oldest element if buffer is full. App can
   choose to avoid the overwrite by checking cbIsFull(). */
void cbWrite(CircularBuffer *cb, uint8_t elem) {
    cb->bytes[cb->end] = elem;
    cb->end = (cb->end + 1) % cb->size;
    if (cb->end == cb->start)
	{
        cb->start = (cb->start + 1) % cb->size; /* full, overwrite */
	}		
	else
	{
		cb->size = cb->size+1;
	}		
}

/* Read oldest element. App must ensure !cbIsEmpty() first. */
uint8_t cbRead(CircularBuffer *cb) {
	if(cb->size == 0)
	{
		return 0xff;
	}
	uint8_t oldstart = cb->start;
	cb->size = cb->size-1;
	cb->start = (cb->start + 1) % cb->size;
    return cb->bytes[oldstart];
}