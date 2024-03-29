﻿/**
 * TSEA27 Elektronikprojekt
 *
 * IDENTIFIERING
 *
 * Modul: Styrenheten
 * Filnamn: spi_master.c
 * Skriven av: T. Norlund, J. Källström
 * Datum: 2013-05-15
 * Version: 1.0
 *
 * BESKRIVNING
 *
 * Hanterar läsning och skrivning av meddelanden via SPI för en master
 */
#include "spi_master.h"

#define DDR_SPI DDRB
#define DD_MOSI PB5
#define DD_MISO PB6
#define DD_SCK PB7

#include <util/delay.h>

/*
 Ställer in alla register för att agera som master.
*/
void SPI_MASTER_init(void)
{
	/* Set MOSI and SCK output, all others input */
	DDR_SPI = 0;
	DDR_SPI = (1<<DD_MOSI)|(1<<DD_SCK);
	
	// Ställ in SS
	DDR_SPI |= (1<<DDB4)|(1<<DDB3);
	PORTB |= (1<<PB4)|(1<<PB3);// sätt båda slavarna höga
	
	// Enable SPI, Master, set clock rate fck/128
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR1)|(1<<SPR0);
}


/*
 Skriver direkt. Returnerar 0 för fel, 1 för lyckad sparning.
*/
uint8_t SPI_MASTER_write(uint8_t *msg, uint8_t type, uint8_t len)
{	
	SPDR = (type<<5)|len;
	while(!(SPSR & (1<<SPIF)));
	_delay_us(100);//time to enter interupt TODO TUNE
	for(uint8_t i = 0; i < len; i++)
	{
		_delay_us(10);//time to do some if else in the interutp TODO TUNE
		/* Start transmission */
		SPDR = msg[i];
		/* Wait for transmission complete */
 		while(!(SPSR & (1<<SPIF)));
	}
	return 1;
}


/** 
 * Läser direkt. Returnerar 0 för fel, 1 för lyckad läsning.
 */
uint8_t SPI_MASTER_read(uint8_t *msg, uint8_t* type, uint8_t *len)
{
	_delay_us(40);//hur lång tid det tar för att komma till ett interupt?
	//send exchange byte
	SPDR=CMD_EXCHANGE_DATA;
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)));
	uint8_t data = SPDR;
	if(data != CMD_EXCHANGE_DATA)//kolla så inte Txbuffer är tom på slaven.
	{
		*len=0b00011111&data;//klipp bort typ
		*type=0b11100000&data;
		*type = *type>>5;
		for(uint8_t i = 0; i < *len; i++)
		{
			_delay_us(10);//hur lång tid det tar för att komma till ett interupt?
			//send exchange byte
			SPDR=CMD_EXCHANGE_DATA;
			/* Wait for transmission complete */
			while(!(SPSR & (1<<SPIF)));
			volatile uint8_t tSPDR = SPDR;
			msg[i]=tSPDR;//spara i:te byten
		}
		return 1;
	}
	else
	{
		return 0;
	}
}


/*
Väljer att kommunikationsenheten skall vara aktiv på bussen. Denna funktion kommer endast att finnas på styrenheten som är
master.
*/
void SPI_set_kom(uint8_t t)
{
	if(t==START)
	{
		PORTB &= ~(1<<PB4);
	}
	else
	{
		PORTB |= (1<<PB4);
	}
}

/*
 Väljer att sensorenheten skall vara aktiv på bussen. Denna funktion kommer endast att finnas på styrenheten som är
 master.
*/
void SPI_set_sensor(uint8_t t)
{
	if(t==START)
	{
		PORTB &= ~(1<<PB3);
	}
	else
	{
		PORTB |= (1<<PB3);
	}
}
