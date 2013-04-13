#include "circularbuffer.h"

// Initierar en cirkulär buffer
void cbInit (CircularBuffer *cb, int size) 
{
    cb->size  = size + 1; /* include empty elem */
    cb->start = 0;
    cb->end   = 0;
    cb->bytes = (uint8_t*)malloc(cb->size);
}

void cbFree(CircularBuffer *cb) {
    free(cb->bytes); /* OK if null */ 
}
 
int cbIsFull(CircularBuffer *cb) {
    return (cb->end + 1) % cb->size == cb->start; 
}
 
int cbIsEmpty(CircularBuffer *cb) {
    return cb->end == cb->start; 
}
 
/* Write an element, overwriting oldest element if buffer is full. App can
   choose to avoid the overwrite by checking cbIsFull(). */
void cbWrite(CircularBuffer *cb, uint8_t elem) {
    cb->bytes[cb->end] = elem;
    cb->end = (cb->end + 1) % cb->size;
    if (cb->end == cb->start)
        cb->start = (cb->start + 1) % cb->size; /* full, overwrite */
}

/* Read oldest element. App must ensure !cbIsEmpty() first. */
uint8_t cbRead(CircularBuffer *cb) {
	int oldstart = cb->start;
	cb->start = (cb->start + 1) % cb->size;
    return cb->bytes[oldstart];
}