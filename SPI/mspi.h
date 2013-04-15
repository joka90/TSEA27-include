#ifndef MSPI_H
#define MSPI_H

#define MSPI_SS_LOW PORTD &= ~(1<<PD2)
#define MSPI_SS_HIGH PORTD |= (1<<PD2) 

/*
Ställer in alla register för att agera som master.
*/
void MSPI_init_master();

uint8_t MSPI_exchange(uint8_t data);

#endif
