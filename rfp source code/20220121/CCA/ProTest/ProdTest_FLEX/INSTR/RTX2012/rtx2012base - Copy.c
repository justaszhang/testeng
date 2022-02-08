#include <windows.h>
#include <formatio.h>
#include <ansi_c.h>
#include <userint.h> 
#include <utility.h>
#include <cvirte.h>
#include "rtx2012base.h"
#include "R2012dll.h"  


//#define PASSLOSS 5




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
int __stdcall rtx2012_Initialize (char *output)
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

int __stdcall rtx2012_ProcConnSet (void)
{
	int i = 0;
	
	
	i = rtx2012_Write("PROC:CONN:SET");
	if(i != 1)
		return 0;
	
	
	return 1;
    
}

int __stdcall rtx2012_ConfFpSignMode (int mode)
{
	
	char wrtBuf[1024] = {0};  
	char  IeeeSyntax[][5] = {"NORM", "LOOP", "ECHO"};  
	
	sprintf(wrtBuf,"CONF:FP:SIGN:MODE %s",IeeeSyntax[mode]);
	
	int	i = rtx2012_Write(wrtBuf);
	if(i != 1)
		return 0;

	return 1;
}

int __stdcall rtx2012_ConfFpAnt (int antennaNumber)
{
	char wrtBuf[1024] = {0};  
	
	sprintf(wrtBuf,"CONF:FP:ANT %d",antennaNumber);
	
	int	i = rtx2012_Write(wrtBuf);
	if(i != 1)
		return 0;

	return 1;  
	
	
	
}

int __stdcall rtx2012_ConfFpRfLev (double RFLevel_dBm)
{
     
	  
	char wrtBuf[1024] = {0};  
	
	sprintf(wrtBuf," CONF:FP:RF:LEV %f dBm",RFLevel_dBm);
	
	int	i = rtx2012_Write(wrtBuf);
	if(i != 1)
		return 0;

	return 1;  
}

int __stdcall rtx2012_ConfFpChannels (int trafficSlotNumber,
                                      int trafficCarrierNumber,
                                      double carrierOffset)
{	  
	char wrtBuf[1024] = {0};  
	
	sprintf(wrtBuf,"CONF:FP:CARR:OFFS %f;:CONF:FP:TRAF:SLOT %d;:CONF:FP:TRAF:CARR %d",carrierOffset,trafficSlotNumber,trafficCarrierNumber);
	
	int	i = rtx2012_Write(wrtBuf);
	if(i != 1)
		return 0;

	return 1;	
}

int __stdcall rtx2012_ConfFpRfpi (char RFPI[])
{
    
	char wrtBuf[1024] = {0};  
	
	sprintf(wrtBuf,"CONF:FP:RFPI \"%s\"",RFPI);
	
	int	i = rtx2012_Write(wrtBuf);
	if(i != 1)
		return 0;

	return 1;
}

int __stdcall rtx2012_ConfBerEvalWind (int evaluationWindow)
{
	char wrtBuf[1024] = {0};

	sprintf(wrtBuf,"CONF:BER:EVAL:WIND %d",evaluationWindow);

	int	i = rtx2012_Write(wrtBuf);
	if(i != 1)
		return 0;

	return 1;
}

int __stdcall rtx2012_ConfBerRfLev (double RFLevel_dBm)
{
	char wrtBuf[1024] = {0};  
	
	sprintf(wrtBuf,"CONF:BER:RF:LEV %f",RFLevel_dBm);
	
	int	i = rtx2012_Write(wrtBuf);
	if(i != 1)
		return 0;

	return 1;
    
}

int __stdcall rtx2012_ConfModDataType (int dataTypeInBField)
{
    
	
	char wrtBuf[1024] = {0};  
	char IeeeSyntax[][5] = {"PSRB", "BS55", "BS33", "BS0F", "F31"}; 
	
	sprintf(wrtBuf,"CONF:MOD:DATA:TYPE %s",IeeeSyntax[dataTypeInBField]);
	
	int	i = rtx2012_Write(wrtBuf);
	if(i != 1)
		return 0;

	return 1;

}


int __stdcall rtx2012_Reset (void)
{

	int i = 0;


	i = rtx2012_Write("SYST:RESET");
	if(i != 1)
		return 0;

	Delay(2);

	return 1;

}

int __stdcall rtx2012_ErrorQuery (int *Error_Code, char errorMessage[])
{
	int i = 0;
	char outp[1024] = {0};
	
	char Result1[255];
	char Result2[255];       


	i = rtx2012_Query(":SYST:ERR?",outp);
	if(i != 1)
		return 0;
	
	
	
	Scan (outp, "%s>%s[xt44]%s", Result1, Result2);


	*Error_Code = atoi(Result1);
	


	strcpy(errorMessage,Result2);
	
	return 1;

    
}

int __stdcall rtx2012_SensSignStat (int *lockState)
{
	int i = 0;
	char outp[1024] = {0};


	i = rtx2012_Query("SENS:SIGN:STAT?",outp);
	if(i != 1)
		return 0;

	
	
	if (CompareStrings ("LOCK", 0, outp, 0, 1) == 0)
		*lockState = 0;
	else if (CompareStrings ("UNL", 0, outp, 0, 1) == 0)
		*lockState = 1;

	return 1;

}

int __stdcall rtx2012_SensDetRfpi (char RFPIValue[])
{
    
	int i = 0;
	char outp[1024] = {0};


	i = rtx2012_Query("SENS:DET:RFPI?",outp);
	if(i != 1)
		return 0;


	strcpy(RFPIValue,outp);

	return 1;
}

int __stdcall rtx2012_CalcLimPowTranTolMatc (int *verdictForNTP)
{
    
	
	int i = 0;
	char outp[1024] = {0};


	i = rtx2012_Query("CALC:LIM:POW:TRAN:TOL:MATC?",outp);
	if(i != 1)
		return 0;



	if (CompareStrings ("MATC", 0, outp, 0, 1) == 0)
		*verdictForNTP = 0;
	else if (CompareStrings ("NMAT", 0, outp, 0, 1) == 0)
		*verdictForNTP = 1;
	else if (CompareStrings ("INV", 0, outp, 0, 1) == 0)
		*verdictForNTP = 2;
	
	return 1;

}

int __stdcall rtx2012_CalcLimPowTranMatc (int *verdictForPowerRamp)
{
    
	int i = 0;
	char outp[1024] = {0};


	i = rtx2012_Query("CALC:LIM:POW:TRAN:MATC?",outp);
	if(i != 1)
		return 0;



	if (CompareStrings ("MATC", 0, outp, 0, 1) == 0)
		*verdictForPowerRamp = 0;
	else if (CompareStrings ("NMAT", 0, outp, 0, 1) == 0)
		*verdictForPowerRamp = 1;
	else if (CompareStrings ("INV", 0, outp, 0, 1) == 0)
		*verdictForPowerRamp = 2;
	
	return 1;
	
	
	
	
}

int __stdcall rtx2012_ReadNtp (double *NTPValue)
{
    
	int i = 0;
	char outp[1024] = {0};


	i = rtx2012_Query("READ:NTP?",outp);
	if(i != 1)
		return 0;
	
	
	if(strcmp(outp,"INV") == 0)
	{
		*NTPValue = -999.0;
		return 0;
	}

	
	*NTPValue = atof(outp);
	

	

	return 1;       
	
}

int __stdcall rtx2012_ReadBerLter (double *BERValue, double *FERValue)
{
	char	 	Result1[255];
	char	 	Result2[255];
	
   	int i = 0;
	char outp[1024] = {0};


	i = rtx2012_Query("READ:BER:LTER?",outp);
	if(i != 1)
		return 0;
	
	

	Scan (outp, "%s>%s[xt44]%s", Result1, Result2);

	*BERValue = atof(Result1);

	*FERValue = atof(Result2);

	if (CompareStrings ("NAN", 0, Result1, 0, 1) == 0)
	{
		*BERValue = 9999;
		return 0;
	}
	
		
	if (CompareStrings ("NAN", 0, Result2, 0, 1) == 0)  
	{
		*FERValue = 9999;
		return 0;
	}
	

	

	return 1;       
	 
}


int __stdcall rtx2012_ConfMessEscMT (char MTEscapeSequence[])
{  
		
	char wrtBuf[1024] = {0};  
	sprintf(wrtBuf,"CONF:MESS:ESC:MT \"%s\"",MTEscapeSequence);
	
	int	i = rtx2012_Write(wrtBuf);
	if(i != 1)
		return 0;

	return 1;
}

int __stdcall rtx2012_StatDev (int *currentInstrumentState)
{
	int i = 0;
	char outp[1024] = {0};


	i = rtx2012_Query("STAT:DEV?",outp);
	if(i != 1)
		return 0;


	 if (CompareStrings ("IDLE", 0, outp, 0, 1) == 0)		
		*currentInstrumentState  = 1;
    else if (CompareStrings ("DFP", 0, outp, 0, 1) == 0)
    	*currentInstrumentState = 2;
    else if (CompareStrings ("DPP", 0, outp, 0, 1) == 0)
    	*currentInstrumentState = 3;
    else if (CompareStrings ("DCE", 0, outp, 0, 1) == 0)
    	*currentInstrumentState = 4;
    else if (CompareStrings ("DBAN", 0, outp, 0, 1) == 0)
    	*currentInstrumentState = 5;
    else if (CompareStrings ("DRFG", 0, outp, 0, 1) == 0)
    	*currentInstrumentState = 6;

	return 1;
    
}

int __stdcall rtx2012_ConfTimAverBurs (int numberOfBursts)
{	 
	char wrtBuf[1024] = {0};
	
	sprintf(wrtBuf,"CONF:TIM:AVER:BURS %d",numberOfBursts);
	
	int	i = rtx2012_Write(wrtBuf);
	if(i != 1)
		return 0;

	return 1;
}

int __stdcall rtx2012_ConfAverBurs (int numberOfBursts)
{
	char wrtBuf[1024] = {0};  

	
	sprintf(wrtBuf,"CONF:AVER:BURS %d",numberOfBursts);
	
	int	i = rtx2012_Write(wrtBuf);
	if(i != 1)
		return 0;

	return 1;
}





int __stdcall rtx2012_ConfTest (int DUTType)
{
	char wrtBuf[1024] = {0};  
	char IeeeSyntax[][4] = {"DFP", "DPP"}; 
	
	sprintf(wrtBuf,"CONF:TEST %s",IeeeSyntax[DUTType]);
	
	int	i = rtx2012_Write(wrtBuf);
	if(i != 1)
		return 0;

	return 1;

    
}

int __stdcall rtx2012_ProcSelTest (int mode)
{
    
	char wrtBuf[1024] = {0};  
	char IeeeSyntax[][5] = {"NONE", "MAN", "BAN", "RFG"}; 

	
	sprintf(wrtBuf,"PROC:SEL %s",IeeeSyntax[mode]);
	
	int	i = rtx2012_Write(wrtBuf);
	if(i != 1)
		return 0;

	return 1;
}

int __stdcall rtx2012_CalcLimPowTranTempCle (void)
{
      
	int	i = rtx2012_Write("CALC:LIM:POW:TRAN:TEMP:CLE");
	if(i != 1)
		return 0;

	return 1; 
}

int __stdcall rtx2012_CalcLimPowTranTempTol (double lowerNTP_dBm,
                                             double upperNTP_dBm)
{
	
	 
	char wrtBuf[1024] = {0};
	
	sprintf(wrtBuf," CALC:LIM:POW:TRAN:TEMP:TOL:LOW %f;:CALC:LIM:POW:TRAN:TEMP:TOL:UPP %f",lowerNTP_dBm,upperNTP_dBm);
	
	int	i = rtx2012_Write(wrtBuf);
	if(i != 1)
		return 0;

	return 1;
    
}

int __stdcall rtx2012_ProcConnRel (void)
{
   	int	i = rtx2012_Write("PROC:CONN:REL ");
	if(i != 1)
		return 0;

	return 1; 
}



int __stdcall rtx2012_Read_FrequencyOffset (char *freqoffset)
{

	int i = 0;
	char outp[1024] = {0};
	
	//CONF:MOD:DATA BS0F


	i = rtx2012_Write("CONF:BER:DATA BS0F");
	if(i != 1)
		return 0;

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
