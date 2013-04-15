#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <stdint.h>

 
/* Circular buffer object */
typedef struct {
    uint8_t         size;   /* maximum number of elements           */
    uint8_t         start;  /* index of oldest element              */
    uint8_t         end;    /* index at which to write new element  */
	uint8_t			bytesUsed;
    uint8_t*     bytes;  /* vector of bytes                   */
} CircularBuffer;

// För att initiera en cirkulär buffer
void cbInit(CircularBuffer *cb, uint8_t size);

void cbFree(CircularBuffer *cb);

int cbIsFull(CircularBuffer *cb);

int cbIsEmpty(CircularBuffer *cb);

/* Write an element, overwriting oldest element if buffer is full. App can
   choose to avoid the overwrite by checking cbIsFull(). */
void cbWrite(CircularBuffer *cb, uint8_t elem);

/* Read oldest element. App must ensure !cbIsEmpty() first. */
uint8_t cbRead(CircularBuffer *cb);

#endif