#include "spi_slave.h"
#include "../circularbuffer.h"

#define DDR_SPI DDRB
#define DD_MOSI PB5
#define DD_MISO PB6
#define DD_SCK PB7

uint8_t recv_mode=0;
uint8_t current_packet_len=0;
uint8_t current_len;
volatile CircularBuffer txbuffer;
volatile CircularBuffer rxbuffer;
uint8_t SPDRFilled;

/*
Ställer in alla register för att agera som slave.
*/
void SPI_SLAVE_init()
{
	SPDRFilled = 0;
	cbInit (&txbuffer, SPI_BUFFERSIZE);
	cbInit (&rxbuffer, SPI_BUFFERSIZE);
	/* PRR0 = PSPI; // PSI måste vara noll för att enabla SPI modulen*/
	/* Set MISO output, all others input */
	DDR_SPI = 0;
	DDR_SPI = (1<<DD_MISO);
	/* Enable SPI */
	SPCR = (1<<SPE)|(1<<SPIE);
}


/*
Sparar ovanstående på skrivbuffern samt startar skrivningen vilken upphör när hela buffern skrivit klart.
Returnerar 0 för fel, 1 för lyckad sparning.
*/
uint8_t SPI_SLAVE_write(uint8_t *msg, uint8_t type, uint8_t len)
{
	//får paketet plats
	if(len+1 > cbBytesFree(&txbuffer))
	{
		return 0;
	}
	cbWrite(&txbuffer, (type<<5)|len);//add header
	//stoppa in paket i tx buffern
	uint8_t i = 0;
	while(i < len)
	{
		cbWrite(&txbuffer, msg[i]);
		++i;
	}
	if(SPDRFilled == 0)
	{
		SPDR = cbRead(&txbuffer);
		SPDRFilled = 1;
	}
	return 1;
}


/*
Läser in nästa meddelande från buffern och sparar det i msg, samt dess längd i len. len sparas i bit 5 till 7 i
paketet. Returnerar 0 för fel (om buffern var tom), 1 för lyckad läsning
*/
uint8_t SPI_SLAVE_read(uint8_t *msg, uint8_t* type, uint8_t *len)
{
	if(cbBytesUsed(&rxbuffer) == 0)
	{
		return 0;
	}
	*len = cbPeek(&rxbuffer);
	*type=0b11100000&*len;
	*type = *type>>5;
	*len = *len&0x1f;
	
	if(cbBytesUsed(&rxbuffer) < *len+1)
	{
		return 0;
	}
	uint8_t dummy = cbRead(&rxbuffer);
	uint8_t i = 0;
	while(i < *len)
	{
		msg[i] = cbRead(&rxbuffer);
		i++;
	}
	return 1;
	/*
	läs ut ett paket ur rx buffern
	
	Hållar reda på lite pekare osv på för läsning. så att den inte skriver över saker. signalera om buffer full.
	rx_start+len pekar på nästa paket efter vi läst ut ett.

	*/
}

ISR(SPI_STC_vect)
{
	uint8_t data = SPDR;
	if(recv_mode)
	{
		current_len++;
		if(current_len==current_packet_len)
		{
			recv_mode=0;
		}
		cbWrite(&rxbuffer, data);
	}
	else if(data==CMD_EXCHANGE_DATA)
	{
		if(cbBytesUsed(&txbuffer) == 0)
		{
			SPDRFilled = 0;
		}
		else
		{
			uint8_t datatemp = cbRead(&txbuffer);
			SPDR = datatemp;	
		}
	}
	else
	{
		//new recive
		recv_mode=1;
		//läs ur längden ur byten, uppdatera räknaren.
		current_packet_len=0b00011111&data;//klipp bort typ
		current_len=0;
		if(current_len==current_packet_len)
		{
			recv_mode=0;
		}
		cbWrite(&rxbuffer, data);
	}
}
