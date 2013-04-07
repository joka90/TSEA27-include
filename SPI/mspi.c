/*
Ställer in alla register för att agera som master.
*/
void MSPI_init_master()
{

}

/*
Sparar ovanstående på skrivbuffern samt startar skrivningen vilken upphör när hela buffern skrivit klart.
Returnerar 0 för fel, 1 för lyckad sparning.
*/
uint8_t MSPI_write(uint8_t *msg, uint8_t len)
{

}


/*
Läser in nästa datapaket från buffern och sparar det i msg, samt dess längd i len. len sparas i bit 5 till 7 i paketet.
Returnerar 0 för fel, 1 för lyckad läsning.
*/
uint8_t MSPI_read(uint8_t *msg, uint8_t *len)
{

}
