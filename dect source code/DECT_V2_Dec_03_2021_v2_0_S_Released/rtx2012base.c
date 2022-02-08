#include <windows.h>
#include <ansi_c.h>
#include <userint.h> 
#include <utility.h>
#include <cvirte.h>
#include "rtx2012base.h"
#include "R2012dll.h"  


#define PASSLOSS 5




/*****************************************/
/*
/*
/*****************************************/
/*int main (int argc, char *argv[])
{
	
	DisableBreakOnLibraryErrors ();
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1; 
	
	
} 	   	   */










/*****************************************/
/* write 
/* 1-success, 0- fail
/*****************************************/

int __stdcall rtx2012_Write (char *command)
{
	int16 i_ = RtxWrt((signed char *)command);
	if(0 == i_)
		return 1;
	else return 0;
}

int __stdcall rtx2012_Query (char *command, char *output)
{
	int16 i_ = RtxWrt((signed char *)command);

	if(i_ != 0)
		return 0;

	i_ = RtxRd((signed char *)output);

	if(i_ > 0)
		return 1;
	else return 0;

}   
/*****************************************/
/* initial 
/* 1-success, 0-fail
/*****************************************/
int __stdcall rtx2012_Init (char *output)
{
   int i = 0;

   i = rtx2012_Write("CONF:COMM:PORT USB"); 
   if(i != 1)
	   return 0;
   
   
   i = rtx2012_Write("SYST:RESET"); 
   if(i != 1)
	   return 0;
   
   Delay(2);
   
   char outp[1024] = {0};
   
   i = rtx2012_Query("SYST:FIRM:VERS?",outp); 
   if(i != 1)
	   return 0;
   
     strcpy(output,"F/W ");
	 strcat(output,outp);  
   
    i = rtx2012_Query("SYST:SER:NUM?",outp); 
   if(i != 1)
	   return 0;
    strcat(output,",SN "); 
   
   strcat(output,outp);
   
   
     i = rtx2012_Query("SYST:HW:VERS?",outp); 
   if(i != 1)
	   return 0;
   
    strcat(output,",HW "); 
   
    strcat(output,outp);  
	
	
	
	i = rtx2012_Write("PROC:SEL:TEST NONE"); 
   if(i != 1)
	   return 0;
   
   	i = rtx2012_Write("CONF:SAMPLE:MODE CTRL"); 
   if(i != 1)
	   return 0;
   
   	i = rtx2012_Write("SENS:CORR:LOSS 0"); 
   if(i != 1)
	   return 0;
   
   
   	i = rtx2012_Write("CONF:ATT:RX ON"); 
   if(i != 1)
	   return 0;
   
   	i = rtx2012_Write("PROC:SAMP:FORC"); 
   if(i != 1)
	   return 0;
   
   	i = rtx2012_Write("CONF:MOD:DRIF:UNIT MS"); 
   if(i != 1)
	   return 0;
   
   	i = rtx2012_Write("CONF:TEST NOP0"); 
   if(i != 1)
	   return 0;
   
   
   
   
    strcat(output,"initial RTX success.");      
   
   
   
   return   1;
}

int __stdcall rtx2012_Loopback_Mode_EU_Setting (void)
{
	int i = 0;

   i = rtx2012_Write("PROC:SEL:TEST MAN"); 
   if(i != 1)
	   return 0;
   
     i = rtx2012_Write("CONF:TEST:TYPE DPP"); 
   if(i != 1)
	   return 0;
   
   
     i = rtx2012_Write("CONF:PP:RFPI \"0001400000\""); 
   if(i != 1)
	   return 0;
     i = rtx2012_Write("CONF:PP:DUMM:SLOT 0"); 
   if(i != 1)
	   return 0;
     i = rtx2012_Write("CONF:PP:DUMM:CARR 5"); 
   if(i != 1)
	   return 0;
     i = rtx2012_Write("CONF:PP:TRAF:SLOT 2"); 
   if(i != 1)
	   return 0;
     i = rtx2012_Write("CONF:PP:TRAF:CARR 0"); 
   if(i != 1)
	   return 0;
     i = rtx2012_Write(" CONF:CARR:OFFS 0"); 
   if(i != 1)
	   return 0;
     i = rtx2012_Write("CONF:PP:SIGN:MODE LOOP"); 
	 
	 
	 
	 i = rtx2012_Write("CONF:POW:PACK PP32Z"); 
   if(i != 1)
	   return 0;
	 
	 
   if(i != 1)
	   return 0;
     i = rtx2012_Write("CONF:BER:DATA FIG31"); 
   if(i != 1)
	   return 0;
   
   
       i = rtx2012_Write("CONF:PP:RF:LEV -45"); 
   if(i != 1)
	   return 0;
     i = rtx2012_Write("CONF:PP:RF:LEV -45"); 
   if(i != 1)
	   return 0;

   
       i = rtx2012_Write("CONF:ATT:RX ON"); 
   if(i != 1)
	   return 0;
     i = rtx2012_Write("PROC:SAMP:FORC"); 
   if(i != 1)
	   return 0;

       i = rtx2012_Write("CONF:POW:DYN:RANG LOW"); 
   if(i != 1)
	   return 0;

   
   return 1;			
   
   
   
    
}


int __stdcall rtx2012_Nonsignal_Connect (void)
{
	

	int i = 0;
	
	
/*	
		i = rtx2012_Write("SENS:CORR:LOSS 5");
	if(i != 1)
		return 0;   */
	
	
	i = rtx2012_Write("PROC:CONN:SET");
	if(i != 1)
		return 0;
	
	Delay(0.3);

	
	char outp[1024] = {0};  
	
	int iLoop = 0;

	for(iLoop = 0; iLoop < 100; iLoop++)
	{
		i = rtx2012_Query("STAT:DEV?",outp);
		if(i != 1)
			return 0;



		if(strcmp(outp,"DCE") == 0)
			break;


		Delay(0.1);

	}
	
	if(iLoop == 100) 
	   	return 0;    

	
	
	
	
	i = rtx2012_Write("PROC:SAMP:FORC");
	if(i != 1)
		return 0;
	

	i = rtx2012_Query("READ:SLOT:SIZE?",outp);
	if(i != 1)
		return 0;

	if(strcmp(outp,"40") != 0)
	{
		//not use P32Z
		return 0;
	}

	
	
	
	


	return 1;
	
	

}

int __stdcall rtx2012_Nosignal_Disconnect (void)
{
	int	i = rtx2012_Write("PROC:CONN:REL ");
	if(i != 1)
		return 0;



/*	i = rtx2012_Write("SYST:RESET");
	if(i != 1)
		return 0;

	Delay(2);		 */

	return 1;
	
}



int __stdcall rtx2012_Reset (void)
{
	int i = rtx2012_Write("SYST:RESET");
	if(i != 1)
		return 0;

	Delay(2);

	return 1;
	
}



int __stdcall rtx2012_Read_NTP (char *power)
{
	
	int i = 0;
	char outp[1024] = {0};
	
	//Additional R&S CMD60 settings: DATA 00001111	  Modulation scheme for PP and FP-test
	i = rtx2012_Write("CONF:BER:DATA BS0F");
	if(i != 1)
		return 0;



	i = rtx2012_Query("READ:NTP?",outp);
	if(i != 1)
		return 0;
	
	
	if(strcmp(outp,"INV") == 0)
	{
		*power = -999.0;
		return 0;
	}

	
	strcpy(power,outp);
	

	

	return 1;       
	

}

int __stdcall rtx2012_Read_FrequencyOffset (char *freqoffset)
{

	int i = 0;
	char outp[1024] = {0};
	
	//CONF:MOD:DATA BS0F


	i = rtx2012_Write("CONF:BER:DATA BS0F");
//	i = rtx2012_Write("CONF:MOD:DATA BS0F");   
	if(i != 1)
		return 0;
	
	Delay(2);

	i = rtx2012_Query("READ:FREQ:OFFS?",outp);
	if(i != 1)
		return 0;





	strcpy(freqoffset,outp);  //Hz



	return 1;

}

int __stdcall rtx2012_Read_Deviation (char *BField)
{
	int i = 0;
	char outp[1024] = {0};
	
	//CONF:MOD:DATA BS0F

	i = rtx2012_Write("CONF:BER:DATA BS0F");
	if(i != 1)
		return 0;


	i = rtx2012_Query("READ:BF?",outp);
	if(i != 1)
		return 0;





	strcpy(BField,outp);



	return 1;
}

int __stdcall rtx2012_Read_FrequencyDriftSlot (char *freqDrift)
{
	int i = 0;
	char outp[1024] = {0};
	
	//CONF:MOD:DATA BS55
	
	
	i = rtx2012_Write("CONF:BER:DATA BS55");
	if(i != 1)
		return 0;
	
	
	
	i = rtx2012_Write("CONF:MOD:DRIF:UNIT SLOT");
	if(i != 1)
		return 0;



	i = rtx2012_Query("READ:FREQ:DRIF:SLOT?",outp);
	if(i != 1)
		return 0;
	
	
	strcpy(freqDrift,outp);



	return 1;
}

int __stdcall rtx2012_Read_ReceiverSensitivity (char *BERFER, double fLev)
{
	int i = 0;
	char outp[1024] = {0};
	char cmd[1024] = {0};

//CONF:MOD:DATA PSRB
	

	i = rtx2012_Write("CONF:BER:DATA PSRB");
	if(i != 1)
		return 0;
	
	sprintf(cmd,"CONF:BER:RF:LEV %f",(fLev));
	
	i = rtx2012_Write(cmd);
	if(i != 1)
		return 0;





	i = rtx2012_Query("READ:BER:LTER?",outp);
	if(i != 1)
		return 0;


	strcpy(BERFER,outp);







	return 1;
}


int __stdcall rtx2012_Read_TemplateMatch (char *matching)
{
	int i = 0;
	char outp[1024] = {0};

//	CONF:MOD:DATA BS0F
	i = rtx2012_Write("CONF:BER:DATA BS0F");
	if(i != 1)
		return 0;

	i = rtx2012_Write("CONF:PP:RF:LEV -45.0");
	if(i != 1)
		return 0;
	
	
		int iLoop = 0;

	for(iLoop = 0; iLoop < 100; iLoop++)
	{
		i = rtx2012_Query("STAT:DEV?",outp);
		if(i != 1)
			return 0;



		if(strcmp(outp,"DCE") == 0)
			break;


		Delay(0.1);

	}
	
	if(iLoop == 100) 
	   	return 0; 
	
	
	
		for(iLoop = 0; iLoop < 100; iLoop++)
	{
		i = rtx2012_Query("CALC:LIM:POW:TRAN:MAT?",outp);
		if(i != 1)
			return 0;



		if(strcmp(outp,"MATC") == 0)
			break;


		Delay(0.1);

	}
	
	if(iLoop == 100) 
	   	return 0; 



	strcpy(matching,outp);







	return 1;
    
}
