#ifdef CONFIG_SPI_MASTER
/*
Ställer in alla register för att agera som master.
*/
void SPI_MASTER_init()
{

}


/*
Skriver direkt. Returnerar 0 för fel, 1 för lyckad sparning.
*/
uint8_t SPI_MASTER_write(uint8_t *msg, uint8_t len)
{

}


/*
Läser direkt. Returnerar 0 för fel, 1 för lyckad läsning.
*/
uint8_t SPI_MASTER_read(uint8_t *msg, uint8_t *len)
{

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
/*
Ställer in alla register för att agera som slave.
*/
void SPI_SLAVE_init()
{

}


/*
Sparar ovanstående på skrivbuffern samt startar skrivningen vilken upphör när hela buffern skrivit klart.
Returnerar 0 för fel, 1 för lyckad sparning.
*/
uint8_t SPI_SLAVE_write(uint8_t *msg, uint8_t len)
{

}


/*
Läser in nästa meddelande från buffern och sparar det i msg, samt dess längd i len. len sparas i bit 5 till 7 i
paketet. Returnerar 0 för fel (om buffern var tom), 1 för lyckad läsning
*/
uint8_t SPI_SLAVE_read(uint8_t *msg, uint8_t *len)
{

}


#endif 

#endif
