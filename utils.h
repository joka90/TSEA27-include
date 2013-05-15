/**
 * TSEA27 Elektronikprojekt
 *
 * IDENTIFIERING
 *
 * Modul: Alla
 * Filnamn: utils.h
 * Skriven av: J. Källström
 * Datum: 2013-05-15
 * Version: 1.0
 *
 * BESKRIVNING
 *
 * Allmänna hjälpfunktioner
 */

#ifndef UTILS_H
#define UTILS_H
/*
* Spara funtioner som är bra att ha på flera enheter här. T.ex räkna ut median, Medelvärde eller dyl.
* Spara div. macron med mer.
*/
#include <stdint.h>

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

/*
* Filtrerar och bildar medelvärdet av en array genom att filterar bort spikar som aviker för mycket. threshold anger hur mycket det får skilja mellan två närliggande mätningar.
*/
uint16_t filterSampleArrayMeanPlusPlus(volatile uint16_t  *samples, uint8_t numOfSamples, uint8_t threshold);
/*
* Filtrerar och bildar medelvärdet av en array genom att filterar bort spikar som aviker för mycket. threshold anger hur mycket det får skilja mellan två närliggande mätningar.
* Om ett det skiljer för mycket mellan två värden avbryts medelvärdesbildningen
* Börjar att medelvädesbilda och räknar sig ned från start.
*/
uint16_t filterMeanTimeAware(volatile uint16_t  *samples, uint8_t numOfSamples, uint8_t threshold, uint8_t start);

/*
* Returnerar absolutbeloppet av differansen av två värden.
*/
uint16_t absThreshold(uint16_t a1, uint16_t a2);

/*
* Sorterar array a.
*/
void insertionSort(volatile uint16_t *a, uint8_t array_size);

/*
* Returnerar medianen utav array a utan att ändra datat i a.
*/
uint16_t median(volatile uint16_t *a, uint8_t size);

/*
* Tvinga fram en casting. Då kompilatorn är oförmögen att göra det själv.
*/
int16_t int8to16(int8_t in);
#endif
