#ifndef SPI_SLAVE_H
#define SPI_SLAVE_H

#include <avr/interrupt.h>

//exchange databit 
#define CMD_EXCHANGE_DATA        0xE0 // 111 0 0000

#define SPI_BUFFERSIZE        	 16 // 111 0 0000

#define START 1 
#define END 0 

/*
Ställer in alla register för att agera som slave.
*/
void SPI_SLAVE_init();

/*
Sparar ovanstående på skrivbuffern samt startar skrivningen vilken upphör när hela buffern skrivit klart.
Returnerar 0 för fel, 1 för lyckad sparning.
*/
uint8_t SPI_SLAVE_write(uint8_t *msg, uint8_t type, uint8_t len);

/*
Läser in nästa meddelande från buffern och sparar det i msg, samt dess längd i len. len sparas i bit 5 till 7 i
paketet. Returnerar 0 för fel (om buffern var tom), 1 för lyckad läsning
*/
uint8_t SPI_SLAVE_read(uint8_t *msg, uint8_t *len);

#endif
