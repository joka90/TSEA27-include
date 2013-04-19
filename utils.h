#ifndef UTILS_H
#define UTILS_H
/*
* Spara funtioner som är bra att ha på flera enheter här. T.ex räkna ut median, Medelvärde eller dyl.
* Spara div. macron med mer.
*/

//macros
#define BIT(x) (1 << (x))
#define SETBITS(x,y) ((x) |= (y))
#define CLEARBITS(x,y) ((x) &= (~(y)))
#define SETBIT(x,y) SETBITS((x), (BIT((y))))
#define CLEARBIT(x,y) CLEARBITS((x), (BIT((y))))
#define BITSET(x,y) ((x) & (BIT(y)))
#define BITCLEAR(x,y) !BITSET((x), (y))
#define BITSSET(x,y) (((x) & (y)) == (y))
#define BITSCLEAR(x,y) (((x) & (y)) == 0)
#define BITVAL(x,y) (((x)>>(y)) & 1)

uint16_t filterSampleArrayMeanPlusPlus(volatile uint16_t  *samples, uint8_t numOfSamples, uint8_t threshold);
uint16_t absThreshold(uint16_t a1, uint16_t a2);
void insertionSort(uint16_t *a, uint8_t array_size);
uint16_t median(uint16_t *a, uint8_t size);

#endif
