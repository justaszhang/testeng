#include "inifile.h"
#include "test.h"

int get_testflow()
{
// hole prüfstatus, lege date ab, addressen sind für pp und fp parts gleich
char	testflow,
		golden,
		fine_date[3],
		temp[12],string1[20];
unsigned char buffer[512];
char *ptemp = temp;

FILE *fa;
int i;
	if(BIOS_ReadEEPROMByte (0x1, &testflow))
		return(0x05a1);
	if(testflow == 0xff)		// if passed functional test?
		 {
		 MessagePopup ("Prüfstatus - Fehler",
				   "DUT hat keinen gültigen Prüfstatus: not passed in FunctionalTest");
		 return(0x1);
		 }
	if((testflow & 0x1) & (!(testflow & 0x10)))		// if passed functional test?
		 {
		 MessagePopup ("Prüfstatus - Fehler",
				   "DUT hat keinen gültigen Prüfstatus: not passed in FunctionalTest");
		 return(0x1);
		 }

	if(BIOS_ReadEEPROMByte (0xa, &golden))
		return(0x1);
	if(golden)	
		 {
		 return(0);
		 }

	testflow |= 0x2;	//vorerst auf failed test setzen
	if ((BIOS_WriteEEPROM (0x1, &testflow, 1)) != 0 ) return(0x050b); 
 	ptemp = DateStr ();
 	fine_date[0] = (ptemp[8]-0x30)*16 + (ptemp[9]-0x30);
 	fine_date[2] = (ptemp[3]-0x30)*16 + (ptemp[4]-0x30);
 	fine_date[1] = (ptemp[0]-0x30)*16 + (ptemp[1]-0x30);
	BIOS_WriteEEPROM (0x15, fine_date, 3);
	return(0);		 
}	



