#ifndef DIVISIONFACTOR_H
#define DIVISIONFACTOR_H
/**
* Denna fil är till för att spara konstanter som används för att optimera upplösningen i en byte.
* Använd konstanter likt 2^n.
* Vet ej om kompilatorn optimerar mult / div med vänster resp. högerskift. Förhoppningsvis gör den det.
* 
* Ex. DIVFAC_ROTSENSOR 2 innebär multiplikation med två innan lagring och division med två i observatören.
*
*/
#define DIVFAC_ROTSENSOR 2

#define DIVFAC_DISTSHORTSENSOR 2

#endif
