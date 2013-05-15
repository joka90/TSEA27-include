/*
Ställer in alla register för att agera som master.
*/
#include <avr/io.h>
#include <avr/interrupt.h>

#include "mspi.h"

void MSPI_init_master()
{
	UBRR1 = 0;
	/* Setting the XCKn port pin as output, enables master mode. PD2 as SS output*/
	DDRD = 0b00010110|(1<<PD2);
	//XCKn_DDR |= (1<<XCKn);
	/* Set MSPI mode of operation and SPI data mode 0. */
	UCSR1C = (1<<UMSEL11)|(1<<UMSEL10)|(0<<UCPOL1)|(0<<U2X1)|(0<<2);
	/* Enable receiver and transmitter. */
	UCSR1B = (1<<RXEN1)|(1<<TXEN1);
	/* Set baud rate. */
	/* IMPORTANT: The Baud Rate must be set after the transmitter is enabled
	*/
	UBRR1 = 0;
}
/*
* Skickar data till slaven och retunerar slavens tidigare data. Dvs det som låg i dess skicka register.
*/
uint8_t MSPI_exchange(uint8_t data)
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR1A & (1<<UDRE1)) );
	/* Put data into buffer, sends the data */
	UDR1 = data;
	/* Wait for data to be received */
	while ( !(UCSR1A & (1<<RXC1)) );
	/* Get and return received data from buffer */
	return UDR1;
}
