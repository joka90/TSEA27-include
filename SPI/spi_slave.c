#include "spi_slave.h"

#define DDR_SPI DDRB
#define DD_MOSI PB5
#define DD_MISO PB6
#define DD_SCK PB7

uint8_t recv_mode=0;
uint8_t current_packet_len=0;
uint8_t current_len;
volatile uint8_t txbuffer[SPI_BUFFERSIZE];// volatile because read in interupt and in main program.,
volatile uint8_t tx_start;
volatile uint8_t tx_end;
volatile uint8_t tx_size;
volatile uint8_t rxbuffer[SPI_BUFFERSIZE];
volatile uint8_t rx_start;
volatile uint8_t rx_end;
volatile uint8_t rx_size;

void circularBufferAdd(volatile uint8_t* buffer, uint8_t element, volatile uint8_t* end, volatile uint8_t* size, uint8_t SIZE)
{
	buffer[*end] = element;
	if(*end == SIZE)
	{
		*end = 0;
	}
	else
	{
		*end = *end+1;
	}
	*size = *size +1;
}

uint8_t circularBufferRead(volatile uint8_t* buffer, volatile uint8_t* start, volatile uint8_t* size, uint8_t SIZE)
{
	uint8_t ret = buffer[*start];
	if(*start == SIZE)
	{
		*start = 0;
	}
	else
	{
		*start = *start+1;
	}
	*size = *size - 1;
	return ret;
}

void circularBufferPutBack(volatile uint8_t* buffer, uint8_t element, volatile uint8_t* start, volatile uint8_t* size, uint8_t SIZE)
{
	if(*start == 0)
	{
		*start = SIZE;
	}
	else
	{
		*start = *start-1;
	}
	buffer[*start] = element;
	*size = *size + 1;
}

/*
Ställer in alla register för att agera som slave.
*/
void SPI_SLAVE_init()
{
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
	if(len+2 > SPI_BUFFERSIZE-tx_size)
	{
		return 0;
	}
	circularBufferAdd(txbuffer, (type<<5)||len, &tx_end, &tx_size, SPI_BUFFERSIZE);//add header
	//stoppa in paket i tx buffern
	uint8_t i = 0;
	while(i < len)
	{
		circularBufferAdd(txbuffer, msg[i], &tx_end, &tx_size, SPI_BUFFERSIZE);
		i = i + 1;
	}
	return 1;
}


/*
Läser in nästa meddelande från buffern och sparar det i msg, samt dess längd i len. len sparas i bit 5 till 7 i
paketet. Returnerar 0 för fel (om buffern var tom), 1 för lyckad läsning
*/
uint8_t SPI_SLAVE_read(uint8_t *msg, uint8_t* type, uint8_t *len)
{
	if(rx_size == 0)
	{
		return 0;
	}
	*len = circularBufferRead(rxbuffer, &rx_start, &rx_size, SPI_BUFFERSIZE);
	*type=0b11100000&*len;
	*type = *type>>5;
	*len = *len&0x1f;
	
	if(rx_size < *len)
	{
		circularBufferPutBack(rxbuffer, msg[0], &rx_start, &rx_size, SPI_BUFFERSIZE);
		return 0;
	}
	uint8_t i = 0;
	while(i <= *len)
	{
		msg[i] = circularBufferRead(rxbuffer, &rx_start, &rx_size, SPI_BUFFERSIZE);
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
	txbuffer[7] = data;
	if(recv_mode)
	{
		current_len++;
		if(current_len==current_packet_len)
		{
			recv_mode=0;
		}
		circularBufferAdd(rxbuffer, data, &rx_end, &rx_size, SPI_BUFFERSIZE);
		//STOPPA byten in i SPI-RX-buffer(på  rx_end), uppdatera rx_end+len
		//TODO
		SPDR=data;//trigger confirm?

	}
	else if(data==CMD_EXCHANGE_DATA)
	{
		SPDR = circularBufferRead(txbuffer, &tx_start, &tx_size, SPI_BUFFERSIZE);
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
		circularBufferAdd(rxbuffer, data, &rx_end, &rx_size, SPI_BUFFERSIZE);
		SPDR=data;//trigger confirm?

	}
}
