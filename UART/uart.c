#include "uart.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
/*
Ställer in alla register samt hastighet för porten.
*/
void UART_init()
{
	/// SÄTT UPP USART
	
	/* Set baud rate */
	uint16_t baud = 99;//63;
	UBRR0 = baud;
	//UBRR0H = (unsigned char)(baud>>8);
	//UBRR0L = (unsigned char)baud;
	/* Enable receiver and transmitter, and enable interrupts on receiving */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);
	/* Set frame format: 8data, 1stop bit */
	UCSR0C = (0<<USBS0)|(3<<UCSZ00);
	
	/// SÄTT UPP RX-BUFFER

	cbInit(&_rxMessageBuffer, 100);
}

void UART_handshake(void)
{
	uint8_t svar=0;
	while(svar!=2)
	{
		/* Wait for empty transmit buffer */
		while (!( UCSR0A & (1<<UDRE0)));
		/* Put data into buffer, sends the data */
		UDR0 = 1;
		
		/* Wait for data to be received */
		_delay_ms(200);
		if((UCSR0A & (1<<RXC0)))
		{
			/* Get and return received data from buffer */
			svar=UDR0;
		}
	}
	/* Wait for empty transmit buffer */
	while (!( UCSR0A & (1<<UDRE0)));
	/* Put data into buffer, sends the data */
	UDR0 = 3;
}

/*
Skickar meddelandet som ges av msg, byte för byte.
*/
void UART_writeMessage(uint8_t* msg, uint8_t type, uint8_t size)
{
	/* Wait for empty transmit buffer */
	while (!( UCSR0A & (1<<UDRE0)));
			
	/* Put data into buffer, sends the data */
	UDR0 = (type<<5)|size;//header data
	
	for(int i = 0; i < size; i++){
		
		/* Wait for empty transmit buffer */
		while (!( UCSR0A & (1<<UDRE0)));
		
		/* Put data into buffer, sends the data */
		UDR0 = msg[i];
	}
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
		
// 		// Är denna byte är början på ett nytt meddelande?
// 		if(_remainingBytes == 0){
// 			
// 			_remainingBytes = b&0x1F;
// 			
// 		}else{
// 			_remainingBytes--;
// 			// Om detta var sista byten i ett meddelande
// 			if(_remainingBytes == 0)
// 				_noOfMessagesInBuffer++;
// 		}
		
	}else{
		// rx-bufferten är full, vad göra?
		// ändra pinne så att det inte går att skicka? PD5 skall göras hög. /Johan
	}
}
