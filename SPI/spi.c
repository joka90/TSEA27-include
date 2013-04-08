#include "spi.h"

#ifdef CONFIG_SPI_MASTER/*
Ställer in alla register för att agera som master.
*/
void SPI_MASTER_init(void)
{
	/* PRR0 = PSPI; // PSI måste vara noll för att enabla SPI modulen*/
	/* Set MOSI and SCK output, all others input */
	DDR_SPI = (1<<DD_MOSI)|(1<<DD_SCK);
	/* Enable SPI, Master, set clock rate fck/16 */
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
// fixa SÅ SS ÄR output annatrs buggar det, då den vill byta till slav
// initera styrportarna här också?
}


/*
Skriver direkt. Returnerar 0 för fel, 1 för lyckad sparning.
*/
uint8_t SPI_MASTER_write(uint8_t *msg, uint8_t len)
{
	for(uint8_t i = 0; i <=len; i++)
	{
		/* Start transmission */
		SPDR = msg[i];
		/* Wait for transmission complete */
		while(!(SPSR & (1<<SPIF)));
	}
	return 1;
}


/*
Läser direkt. Returnerar 0 för fel, 1 för lyckad läsning.
*/
uint8_t SPI_MASTER_read(uint8_t *msg, uint8_t *len)
{
	//send exchange byte
	SPDR=CMD_EXCHANGE_DATA;
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)));
	len=0b00011111&SPDR;//klipp bort typ

	msg[0]=SPDR;//spara första byten

	for(uint8_t i = 1; i <=len; i++)
	{
		//send exchange byte
		SPDR=CMD_EXCHANGE_DATA;
		/* Wait for transmission complete */
		while(!(SPSR & (1<<SPIF)));
		msg[i]=SPDR;//spara i:te byten
	}
	return 1;
}


/*
Väljer enhet som skall vara aktiv på bussen. Denna funktion kommer endast att finnas på styrenheten som är
master. Styrpinnar för slaveselect kommer vara hårdkodat. Returnerar 0 för fel, 1 för lyckad sparning.
*/
uint8_t SPI_select_unit(uint8_t unit)
{

}


#endif

#ifdef CONFIG_SPI_SLAVE

uint8_t recv_mode=0;
uint8_t current_packet_len=0;
uint8_t current_len;
volatile uint8_t t txbuffer[SPI_BUFFERSIZE];// volatile because read in interupt and in main program.,
volatile uint8_t  tx_start;
volatile uint8_t  tx_end;
volatile uint8_t  tx_size;
volatile uint8_t  rxbuffer[SPI_BUFFERSIZE];
volatile uint8_t  rx_start;
volatile uint8_t  rx_end;
volatile uint8_t  rx_size;

void circularBufferAdd(volatile uint8_t* buffer, uint8_t element, volatile uint8_t& end, volatile uint8_t& size, uint8_t SIZE)
{
    buffer[end] = element;
    if(end == SIZE)
    {
        end = 0;
    }
    else
    {
        end = end+1;
    }
    size = size +1;
}

uint8_t circularBufferRead(volatile uint8_t* buffer, volatile uint8_t& start, volatile uint8_t& size, uint8_t SIZE)
{
    uint8_t ret = buffer[start];
    if(start == SIZE)
    {
        start = 0;
    }
    else
    {
        start = start+1;
    }
    size = size - 1;
    return ret;
}

void circularBufferPutBack(volatile uint8_t* buffer, uint8_t element, volatile uint8_t& start, volatile uint8_t& size, uint8_t SIZE)
{
    if(start == 0)
    {
        start = SIZE;
    }
    else
    {
        start = start-1;
    }
    buffer[start] = element;
    size = size + 1;
}

/*
Ställer in alla register för att agera som slave.
*/
void SPI_SLAVE_init()
{
	/* PRR0 = PSPI; // PSI måste vara noll för att enabla SPI modulen*/
	/* Set MISO output, all others input */
	DDR_SPI = (1<<DD_MISO);
	/* Enable SPI */
	SPCR = (1<<SPE);
}


/*
Sparar ovanstående på skrivbuffern samt startar skrivningen vilken upphör när hela buffern skrivit klart.
Returnerar 0 för fel, 1 för lyckad sparning.
*/
uint8_t SPI_SLAVE_write(uint8_t *msg, uint8_t len)
{
//får paketet plats
if(len > SPI_BUFFERSIZE-tx_size)
{
    return 0;
}
//stoppa in paket i tx buffern
uint8_t i = 0;

while(i < len)
{
    circularBufferAdd(txbuffer, msg[i], tx_end, tx_size, SPI_BUFFERSIZE);
    i = i + 1;
}
return 1;


/*
Hållar reda på lite pekare osv på för skrivning. så att den inte skriver över saker. signalera om buffer full.

tx_end+len, pekar på nästa lediga plats.

*/
}


/*
Läser in nästa meddelande från buffern och sparar det i msg, samt dess längd i len. len sparas i bit 5 till 7 i
paketet. Returnerar 0 för fel (om buffern var tom), 1 för lyckad läsning
*/
uint8_t SPI_SLAVE_read(uint8_t *msg, uint8_t *len)
{
    *len = circularBufferRead(rxbuffer, rx_start, rx_size, SPI_BUFFERSIZE);
    msg[0] = *len
    *len = *len&0x07;
    uint8_t i = 0;
    if(rx_size < *len)
    {
        circularBufferPutBack(rxbuffer, msg[0], rx_start, rx_size, SPI_BUFFERSIZE);
        return 0;
    }
    while(i < *len)
    {
        msg[i+1] = circularBufferRead(rxbuffer, rx_start, rx_size, SPI_BUFFERSIZE);
        i = i + 1;
    }
    return 1;
/*
TODO
läs ut ett paket ur rx buffern

Hållar reda på lite pekare osv på för läsning. så att den inte skriver över saker. signalera om buffer full.
rx_start+len pekar på nästa paket efter vi läst ut ett.

*/
}

ISR(SPI_STC_vect)
{
	if(recv_mode)
	{
		current_len++;
		if(current_len==current_packet_len)
		{
		    recv_mode=0;
		}
        circularBufferAdd(rxbuffer, SPDR, rx_end, rx_size, SPI_BUFFERSIZE);
		//STOPPA byten in i SPI-RX-buffer(på  rx_end), uppdatera rx_end+len
	//TODO

	}
	else if(SPDR==CMD_EXCHANGE_DATA)
    {
        SPDR = circularBufferRead(txbuffer, tx_start, tx_size, SPI_BUFFERSIZE);
    }
    else
    {
        //new recive
        recv_mode=1;
        //läs ur längden ur byten, uppdatera räknaren.
        current_packet_len=0b00011111&SPDR;//klipp bort typ
        current_len=0;
        if(current_len==current_packet_len)
		{
		    recv_mode=0;
		}
        //STOPPA byten in i SPI-RX-buffen TODO
        circularBufferAdd(rxbuffer, SPDR, rx_end, rx_size, SPI_BUFFERSIZE);
	}
}


#endif
