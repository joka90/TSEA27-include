#ifndef MSPI_H
#define MSPI_H

#define MSPI_SS_LOW PORTD &= ~(1<<PD2)
#define MSPI_SS_HIGH PORTD |= (1<<PD2) 

/*
Ställer in alla register för att agera som master.
*/
void MSPI_init_master();

/*
* Skickar data till slaven och retunerar slavens tidigare data. Dvs det som låg i dess skicka register.
*/
uint8_t MSPI_exchange(uint8_t data);

#endif
