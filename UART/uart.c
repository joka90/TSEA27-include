/**
 * TSEA27 Elektronikprojekt
 *
 * IDENTIFIERING
 *
 * Modul: Kommunikationsenheten
 * Filnamn: uart.c
 * Skriven av: T. Norlund, J. Källström
 * Datum: 2013-05-15
 * Version: 1.0
 *
 * BESKRIVNING
 *
 * Funktonalitet för att kommunicera via UART
 */

#include "uart.h"

/*
Ställer in alla register samt hastighet för porten.
*/
void UART_init()
{
	/// SÄTT UPP USART
	
	uint16_t baud = 99;
	UBRR0 = baud;
	// Sätt på receiver, transmitter och interrupt
	UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1 << RXCIE0);
	// Set frame format: 8data, 1stop bit
	UCSR0C = (0<<USBS0)|(3<<UCSZ00);

	/// SÄTT UPP RX-BUFFER
	cbInit(&_rxMessageBuffer, 100);
}

/*
 Skickar en s.k. synkroniseringssvärm (5 0xFF i rad). Används för att PC:n ska vara
 säker på att det som kommer därefter är början på ett nytt meddelande
*/
void UART_writeSyncMessage()
{
	while (!( UCSR0A & (1<<UDRE0)));
	UDR0 = 255;
	while (!( UCSR0A & (1<<UDRE0)));
	UDR0 = 255;
	while (!( UCSR0A & (1<<UDRE0)));
	UDR0 = 255;
	while (!( UCSR0A & (1<<UDRE0)));
	UDR0 = 255;
	while (!( UCSR0A & (1<<UDRE0)));
	UDR0 = 255;
	while (!( UCSR0A & (1<<UDRE0)));
}


/*
 Skickar meddelandet som ges av msg, byte för byte.
*/
uint8_t UART_writeMessage(uint8_t* msg, uint8_t type, uint8_t size)
{
	/* Vänta på en tom transmit-buffert */
	while (!( UCSR0A & (1<<UDRE0)));
	
	// Skicka headern
	UDR0 = (type<<5)|size;
	
	for(int i = 0; i < size; i++){
		
		// Vänta på en tom transmit-buffert
		while (!( UCSR0A & (1<<UDRE0)));
		
		UDR0 = msg[i];
	}
	return 1;
}


/*
Läser in nästa datapaket från buffern och sparar det i msg, samt dess längd i len. len sparas i bit 5 till 7 i paketet.
Returnerar 0 för fel (om buffern var tom), 1 för lyckad läsning.
*/
uint8_t UART_readMessage(uint8_t *msg, uint8_t *type, uint8_t *length)
{
	//kolla om det finns några hela medelanden
	if(cbBytesUsed(&_rxMessageBuffer)==0)
		return 0;
		
	uint8_t length_msg = cbPeek(&_rxMessageBuffer)&0x1f;
	if(cbBytesUsed(&_rxMessageBuffer) < length_msg+1)
	{
		return 0;
	}

	uint8_t firstByte = cbRead(&_rxMessageBuffer);
	*type=0b11100000&firstByte;
	*type = *type>>5;
	*length = firstByte&0x1F;

	for(int i = 0; i < *length; i++){
		msg[i] = cbRead(&_rxMessageBuffer);
	}

	return 1;
}

/*
 Avbrott för mottagen byte. Lägg in mottagna byten i rx-buffer.
*/
ISR(USART0_RX_vect){
	
	// Läs in den mottagna byten i rx-bufferten, om den inte är full
	if(!cbIsFull(&_rxMessageBuffer)){
		
		uint8_t b = UDR0;
		
		// Lägg till i bufferten
		cbWrite(&_rxMessageBuffer, b);
		
	}else{
		// rx-bufferten är full, gör inget!
	}
}
