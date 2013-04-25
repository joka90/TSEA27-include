#include "spi_slave.h"
#include "../circularbuffer.h"

#define DDR_SPI DDRB
#define DD_MOSI PB5
#define DD_MISO PB6
#define DD_SCK PB7

volatile uint8_t recv_mode=0;
volatile uint8_t transmit_mode=0;
volatile uint8_t current_packet_len=0;
volatile uint8_t current_len;
volatile CircularBuffer txbuffer;
volatile CircularBuffer rxbuffer;
volatile uint8_t minOneMsgInBuffer;

/*
Ställer in alla register för att agera som slave.
*/
void SPI_SLAVE_init()
{
	minOneMsgInBuffer = 0;
	cbInit (&txbuffer, SPI_BUFFERSIZE);
	cbInit (&rxbuffer, SPI_BUFFERSIZE);
	/* PRR0 = PSPI; // PSI måste vara noll för att enabla SPI modulen*/
	/* Set MISO output, all others input */
	DDR_SPI = 0;
	DDR_SPI = (1<<DD_MISO);
	/* Enable SPI */
	SPCR = (1<<SPE)|(1<<SPIE);
	
	SPDR = CMD_EXCHANGE_DATA; // Skicka ej redo, om vi inte hunnit handskaka
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
	if(minOneMsgInBuffer == 0)
	{
		minOneMsgInBuffer = 1;
	}
	return 1;
}

/*
http://www.avrfreaks.net/index.php?name=PNphpBB2&file=printview&t=42998&start=0
When a serial transfer is complete, the SPIF flag is set. An interrupt is generated if SPIE in
SPCR is set and global interrupts are enabled. If SS is an input and is driven low when the SPI is
in Master mode, this will also set the SPIF flag. SPIF is cleared by hardware when executing the
corresponding interrupt handling vector. Alternatively, the SPIF bit is cleared by first reading the
SPI Status Register with SPIF set, then accessing the SPI Data Register (SPDR). 

*/
ISR(SPI_STC_vect)
{
	uint8_t data = SPDR;
	uint8_t status = SPIF;
	if(data==CMD_EXCHANGE_DATA)
	{
		if(cbBytesUsed(&txbuffer) == 0)
		{
			minOneMsgInBuffer = 0;
			SPDR=CMD_EXCHANGE_DATA;//svara att den är tom, detta kommer skrivas över vid SPI_write då en ny överföring görs pga minOneMsgInBuffer ==0
		}
		else if(minOneMsgInBuffer == 1)// Används för att inte läsa ut head medans resterande medelande skrivs in i txbuffern
		{
			data=cbRead(&txbuffer);//skicka första byten
			current_packet_len=0b00011111&data;//klipp bort typ
			SPDR=data;//skicka första byten
			/* Wait for transmission complete */
			while(!(SPSR & (1<<SPIF)));//undra om vi måste clera interuptbiten då vi befinner oss i interuptet?
			current_len=0;
			while(current_len<current_packet_len)
			{
				SPDR=cbRead(&txbuffer);//skicka första byten
				/* Wait for transmission complete */
				while(!(SPSR & (1<<SPIF)));
				current_len++;
			}
		}
		else
		{
			SPDR=CMD_EXCHANGE_DATA;// Skicka ej redo om medans vi skriver in i txbuffern.
		}
	}
	else
	{
		//new recive
		//läs ur längden ur byten, uppdatera räknaren.
		current_packet_len=0b00011111&data;//klipp bort typ
		cbWrite(&rxbuffer, data);//spara första byten
		current_len=0;
		while(current_len<current_packet_len)
		{
			/* Wait for reception complete */
			while(!(SPSR & (1<<SPIF)))//undra om vi måste clera interuptbiten då vi befinner oss i interuptet?
			data = SPDR;
			cbWrite(&rxbuffer, data);
			current_len++;
		}
	}
	SPDR=CMD_EXCHANGE_DATA;// För att ifall vi inte hinner till SPI_SLAVE_write, så skicka att vi inte hunnit
}
