#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <stdint.h>
#include <stdlib.h>
 
/* Circular buffer object */
typedef struct {
    uint8_t         size;   /* maximum number of elements           */
    uint8_t         start;  /* index of oldest element              */
    uint8_t         end;    /* index at which to write new element  */
	uint8_t			bytesUsed;
    uint8_t*     bytes;  /* vector of bytes                   */
} CircularBuffer;

// För att initiera en cirkulär buffer
void cbInit(volatile CircularBuffer *cb, uint8_t size);

void cbFree(volatile CircularBuffer *cb);

uint8_t cbIsFull(volatile CircularBuffer *cb);

uint8_t cbIsEmpty(volatile CircularBuffer *cb);

uint8_t cbBytesUsed(volatile CircularBuffer *cb);

uint8_t cbBytesFree(volatile CircularBuffer *cb);

uint8_t cbPeek(volatile CircularBuffer *cb);

/* Write an element, overwriting oldest element if buffer is full. App can
   choose to avoid the overwrite by checking cbIsFull(). */
void cbWrite(volatile CircularBuffer *cb, uint8_t elem);

/* Read oldest element. App must ensure !cbIsEmpty() first. */
uint8_t cbRead(volatile CircularBuffer *cb);

#endif