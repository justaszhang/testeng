#include <utility.h>
#include <ansi_c.h>
#include <gpib.h>
#include "HP53131A.h"
//==============================================================================
//
// Title:		HP53131A.c
// Purpose:		A short description of the implementation.
//
// Created on:	8/2/2021 at 2:05:45 PM by Mitel-Justas.
// Copyright:	. All Rights Reserved.
//
//==============================================================================

//==============================================================================
// Include files

//#include "HP53131A.h"

//==============================================================================
// Constants

//==============================================================================
// Types

//==============================================================================
// Static global variables

//==============================================================================
// Static functions

//==============================================================================
// Global variables

ViSession  UCT_HANDLE = 0;   

//==============================================================================
// Global functions

//*************************************************************
//* Init_Counter(); 	
//* 
//*************************************************************
int Init_Counter () {		
	
	char Temp_Buff[1024]; 
	int status = 1;
	
	int UCT_GPIB = 3;
	
    sprintf (Temp_Buff, "GPIB0::%d::INSTR", UCT_GPIB);
	

   //get handle for HP53131A instrument at UCT_GPIB address, open the device
   UCT_HANDLE = ibdev (0, UCT_GPIB, NO_SAD, T10s, 1, 0);
   if (ibsta & 0x8000) status = 0;
   
   //send interface clear to the instrument
   ibwrt (UCT_HANDLE, "IFC", strlen("IFC"));
   if (ibsta & 0x8000) status = 0;
   
   //send device clear to the instrument
   ibwrt (UCT_HANDLE, "DCL", strlen("DCL")); 
   if (ibsta & 0x8000) status = 0;
   
    //clear the device
   ibclr (UCT_HANDLE);
   if (ibsta & 0x8000) status = 0;  
   
   //clear status of the instrument
   ibwrt (UCT_HANDLE, "*CLS", strlen("*CLS"));
   if (ibsta & 0x8000) status = 0;
   
   //reset the instrunet
   ibwrt (UCT_HANDLE, "*RST", strlen("*RST")); 
   if (ibsta & 0x8000) status = 0; 	

   // Clear service request enable register
   ibwrt (UCT_HANDLE, "*SRE 0", strlen("*SRE 0")); 
   if (ibsta & 0x8000) status = 0;  
   
   // Clear event status enable register
   ibwrt (UCT_HANDLE, "*ESE 0", strlen("*ESE 0")); 
   if (ibsta & 0x8000) status = 0; 
   
   // Preset enable registers and transition filters for operation
   ibwrt (UCT_HANDLE, ":STATus:PRESet", strlen(":STATus:PRESet")); 
   if (ibsta & 0x8000) status = 0; 

   //Sets input coupling to AC
   ibwrt (UCT_HANDLE, ":INP:COUP AC", strlen(":INP:COUP AC")); 
   if (ibsta & 0x8000) status = 0; 

   //Sets input impedance
   ibwrt (UCT_HANDLE, ":INP:IMP 50", strlen(":INP:IMP 50")); 
   if (ibsta & 0x8000) status = 0; 

   //:FREQ:ARM:STOP:TIM 1.000
   ibwrt (UCT_HANDLE, ":FREQ:ARM:STOP:TIM 1.000", strlen(":FREQ:ARM:STOP:TIM 1.000")); 
   if (ibsta & 0x8000) status = 0;

   //Measure frequency on channel 1
   ibwrt (UCT_HANDLE, ":FUNC 'FREQ 1'", strlen(":FUNC 'FREQ 1'")); 
   if (ibsta & 0x8000) status = 0;
   
	
	return status;
   
}

//*************************************************************
//* 	
//* 
//*************************************************************
int TRIGGER_HP53131A_TO_MEASURE(double * freq)
{
 
	int status = 1;
	char Temp_Buff[1024];
    int	 numToRead = 200; 

    //commands to read uct 
    //Enable statistics.
    ibwrt (UCT_HANDLE, ":CALC3:AVERAGE ON", strlen(":CALC3:AVERAGE ON")); 
    if (ibsta & 0x8000) status = 0; 

    //Base statistics on 1 measurements.
    ibwrt (UCT_HANDLE, ":CALC3:AVERAGE:COUNT 1", strlen(":CALC3:AVERAGE:COUNT 1")); 
    if (ibsta & 0x8000) status = 0; 

    //On INIT, take 50 measurements
    ibwrt (UCT_HANDLE, ":TRIG:COUNT:AUTO ON", strlen(":TRIG:COUNT:AUTO ON")); 
    if (ibsta & 0x8000) status = 0; 

    //Start 50 measurements
    ibwrt (UCT_HANDLE, ":INIT", strlen(":INIT")); 
    if (ibsta & 0x8000) status = 0; 

    //Wait until 50 measurements are complete
    ibwrt (UCT_HANDLE, "*WAI", strlen("*WAI")); 
    if (ibsta & 0x8000) status = 0;

    //Asks for the statistics
    ibwrt (UCT_HANDLE, ":CALC3:AVERAGE:ALL?", strlen(":CALC3:AVERAGE:ALL?")); 
    if (ibsta & 0x8000) status = 0; 
	
	
    ibrd (UCT_HANDLE, Temp_Buff, numToRead);
    if (ibsta & 0x8000) status = 0;
	

	*freq = atof(Temp_Buff);
	
    return status;
}


int TRIGGER_HP53131A_TO_MEASURE_addon (double *freq) // 
{
	int		i,rc;
	
	double dFreq=0;
	double dFreqLast=0;


	// maximal 5 x Freq auslesen bis der Wert stabil ist

	*freq = 0.0;

	for ( i=0 ; i < 5 ; i++ )
	{
	
		if ( !(TRIGGER_HP53131A_TO_MEASURE( &dFreq)) )
		{
			return 0;
		}
		
		if ( i > 0 && dFreq == dFreqLast )
		{
			break;
		}
		
		dFreqLast = dFreq;
		
		Delay(0.2);  //200ms
		

	}
	
	// wenn Freq mehr als 2Hz schwankt dann fehler
	if ( (dFreq - dFreqLast) > 2.0 || (dFreqLast - dFreq) > 2.0 )
	{
		dFreq = -1;                 // fehler  27.06.2014 @GC
	}
	
	
	*freq = dFreq;
	
	return (1);
}

