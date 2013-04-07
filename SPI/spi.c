#ifdef CONFIG_SPI_MASTER

#include "spi.h"

/*
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
volatile uint8_t  rxbuffer[SPI_BUFFERSIZE];
volatile uint8_t  rx_start;
volatile uint8_t  rx_end;


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
/*

stoppa in paket i tx buffern

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
	if(start+len<end)
	{}
/*

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

		//STOPPA byten in i SPI-RX-buffer(på  rx_end), uppdatera rx_end+len
	

	}
	else
	{
		if(SPDR==CMD_EXCHANGE_DATA)
		{
		    //send from tx-buffer(läs från tx start.) Uppdatera tx-start med +1
			if(tx_start+current_len<SPI_BUFFERSIZE)
			{
				SPDR = txbuffer[tx_start+current_len];
			}
			else
			{
				SPDR = txbuffer[tx_start+current_len-SPI_BUFFERSIZE];// TODO
			}
		}
		else
		{
			//new recive
			recv_mode=1;
			//läs ur längden ur byten, uppdatera räknaren.
			current_packet_len=0b00011111&SPDR;//klipp bort typ
			current_len=0;

			//STOPPA byten in i SPI-RX-buffen SAMMA KOD PÅ TVÅ STÄLLEN
			if(rx_start+current_len < rx_end)
			{
				rxbuffer[rx_start+current_len];
				//send confirm
				SPDR = OK; // om full eller inte?
			}
			else if(rx_start+current_len>SPI_BUFFERSIZE)
			{
				if(rx_start+current_len-SPI_BUFFERSIZE < rx_end)
				{
					rxbuffer[rx_start+current_len-SPI_BUFFERSIZE];// TODO tänk igen, känns rätt
					//send confirm
					SPDR = OK; // om full eller inte?
				}
			}

		}
	}
}


#endif
