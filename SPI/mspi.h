#ifndef MSPI_H
#define MSPI_H

/*
Ställer in alla register för att agera som master.
*/
void MSPI_init_master();

uint8_t MSPI_exchange(uint8_t data);

#endif
