#ifndef _CMD60_ADDON_C
#define _CMD60_ADDON_C
#endif
//==============================================================================
//
// Title:       CMD60_addon.c
//
// Purpose:     CMD60 Add-on Funktionen
//
// Created on:  12.04.2012 by G.Cordes
// Copyright:   . All Rights Reserved.
//
//==============================================================================

//==============================================================================
// Include files
#include <windows.h>
#include <winsock.h>
#include <userint.h>
#include <utility.h>
#include <ansi_c.h>
//#include <stdlib.h>
//#include <stdio.h> 
//#include <errno.h>
#include <string.h>
#include <formatio.h>


#include "CMD60.h"
#include "CMD.h"
#include "CMD60_addon.h"

#include "rtepubl.h"    
#include "rte_tool.h"   
#include "test.h"   




//==============================================================================
// Global variables

//==============================================================================
// Static global variables



//==============================================================================
// Constants
#define MAX_GPIB_BUFFER         4000L //[@JM270807]






//==============================================================================
// Global functions

/***********************************************************************************************
**
**   CMD60 Add-on Funktionen
**
***********************************************************************************************/

int CMD60_addon_Register (int InstrSession, char buf[]) // new [@JM080918]
{
	ViStatus  CMD60_status = VI_SUCCESS;
	ViInt32   reg_STB;

	Scan (buf, "%s>%d", &reg_STB);
	
	if ((reg_STB != 0) && (reg_STB != 16))
		CMD60_status = VI_ERROR_REGISTER_STB;

	return CMD60_status;
}

int CMD60_addon_ClearError (int InstrSession) // new [@GC26.03.2012]
{
	int				i;
	int				iErrorCode=0;
	char			cErrorMessage[512];
	
	
	for (i=0; i<10; i++)
	{
		if ( CMD60_ErrorQuery (InstrSession, &iErrorCode, cErrorMessage) )	continue;
		if ( iErrorCode == 0 )												break;
	}
	return (0);
}	

/*===========================================================================*/
/* Function :
/*     CMD60_addon_SetPacketTypePP32Z
/* Purpose :
/*     This function permits to ...
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
int CMD60_addon_SetPacketTypePP32Z (int InstrSession) // new [@JM080918]
{
	int				i,rc;
	int				iErrorCode=0;
	int				iNumBytesRead=0;
	char			cErrorMessage[512];
	unsigned char	cReadBuffer[MAX_GPIB_BUFFER];
	unsigned char	cCmd[256];

	sprintf(cCmd, "CONF:POW:PACK PP32Z;*STB?\n");
	if ( (rc=CMD60_WriteInstrData (InstrSession, cCmd)) < 0 )
	{
		for (i=0; i<10; i++)
		{
			if ( CMD60_ErrorQuery (InstrSession, &iErrorCode, cErrorMessage) )	continue;
			if ( iErrorCode == 0 )												break;
		}
		return(rc);     //error
	}

	if ( (rc=CMD60_ReadInstrData (InstrSession, MAX_GPIB_BUFFER, cReadBuffer, &iNumBytesRead)) < 0 )
	{
		for (i=0; i<10; i++)
		{
			if ( CMD60_ErrorQuery (InstrSession, &iErrorCode, cErrorMessage) )	continue;
			if ( iErrorCode == 0 )												break;
		}
		return(rc);     //error
	}
	
	if ( (rc=CMD60_addon_Register (InstrSession, cReadBuffer)) < 0 )
	{
		for (i=0; i<10; i++)
		{
			if ( CMD60_ErrorQuery (InstrSession, &iErrorCode, cErrorMessage) )	continue;
			if ( iErrorCode == 0 )												break;
		}
		return(rc);     //error
	}
	
	return (0);
}

/*===========================================================================*/
/* Function :
/*     CMD60_addon_SendCommand
/* Purpose :
/*     This function permits to ...
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     cCmd
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
int CMD60_addon_SendCommand (int InstrSession, unsigned char *cCmd) // new [@JM211022]
{
	int				i,rc;
	int				iErrorCode=0;
	int				iNumBytesRead=0;
	char			cErrorMessage[512];
	unsigned char	cReadBuffer[MAX_GPIB_BUFFER];

	if ( (rc=CMD60_WriteInstrData (InstrSession, cCmd)) < 0 )
	{
		for (i=0; i<10; i++)
		{
			if ( CMD60_ErrorQuery (InstrSession, &iErrorCode, cErrorMessage) )	continue;
			if ( iErrorCode == 0 )												break;

		}
		return(rc);     //error
	}

	if ( (rc=CMD60_ReadInstrData (InstrSession, MAX_GPIB_BUFFER, cReadBuffer, &iNumBytesRead)) < 0 )
	{
		for (i=0; i<10; i++)
		{
			if ( CMD60_ErrorQuery (InstrSession, &iErrorCode, cErrorMessage) )	continue;
			if ( iErrorCode == 0 )												break;
		}
		return(rc);     //error
	}
	
	if ( (rc=CMD60_addon_Register (InstrSession, cReadBuffer)) < 0 )
	{
		for (i=0; i<10; i++)
		{
			if ( CMD60_ErrorQuery (InstrSession, &iErrorCode, cErrorMessage) )	continue;
			if ( iErrorCode == 0 )												break;
		}
		return(rc);     //error
	}
	return (0);
}

/*===========================================================================*/
/* Function :
/*     CMD60_addon_GetFirmwareVersion
/* Purpose :
/*     This function permits to ...
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     firmwareVersion
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
int CMD60_addon_GetFirmwareVersion (int InstrSession,char *firmwareVersion) // new [@GC25052011]
{
	int				i,rc;
	int				iErrorCode=0;
	int				iNumBytesRead=0;
	char			cErrorMessage[512];
	unsigned char	cReadBuffer[MAX_GPIB_BUFFER];

	//SYSTem:FIRMware:VERSion?
	
	if ( (rc=CMD60_WriteInstrData (InstrSession, "SYST:FIRM:VERS?")) < 0 )
	{
		for (i=0; i<10; i++)
		{
			if ( CMD60_ErrorQuery (InstrSession, &iErrorCode, cErrorMessage) )	continue;
			if ( iErrorCode == 0 )												break;
		}
		return(rc);     //error
	}

	if ( (rc=CMD60_ReadInstrData (InstrSession, MAX_GPIB_BUFFER, cReadBuffer, &iNumBytesRead)) < 0 )
	{
		for (i=0; i<10; i++)
		{
			if ( CMD60_ErrorQuery (InstrSession, &iErrorCode, cErrorMessage) )	continue;
			if ( iErrorCode == 0 )												break;
		}
		return(rc);     //error
	}
	cReadBuffer[iNumBytesRead] = '\0';
	
	strcpy(firmwareVersion,cReadBuffer);
	
	return (0);    //ok
}

/*===========================================================================*/
/* Function :
/*     CMD60_addon_GetOption
/* Purpose :
/*     This function permits to ...
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     sysOption
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
int CMD60_addon_GetOption (int InstrSession,char *sysOption) // new [@GC25052011]
{
	int				i,rc;
	int				iErrorCode=0;
	int				iNumBytesRead=0;
	char			cErrorMessage[512];
	unsigned char	cReadBuffer[MAX_GPIB_BUFFER];

	//SYSTem:OPTions?
	//Options B4, B41, B6, and B61 exist
	
	if ( (rc=CMD60_WriteInstrData (InstrSession, "SYST:OPT?")) < 0 )
	{
		for (i=0; i<10; i++)
		{
			if ( CMD60_ErrorQuery (InstrSession, &iErrorCode, cErrorMessage) )	continue;
			if ( iErrorCode == 0 )												break;
		}
		return(rc);     //error
	}

	if ( (rc=CMD60_ReadInstrData (InstrSession, MAX_GPIB_BUFFER, cReadBuffer, &iNumBytesRead)) < 0 )
	{
		for (i=0; i<10; i++)
		{
			if ( CMD60_ErrorQuery (InstrSession, &iErrorCode, cErrorMessage) )	continue;
			if ( iErrorCode == 0 )												break;
		}
		return(-1);     //error
	}
	cReadBuffer[iNumBytesRead] = '\0';
	
	strcpy(sysOption,cReadBuffer);
	
	return (0);      //ok
}

/*===========================================================================*/
/* Function :
/*     CMD60_addon_ReadNormalTransmitPower
/* Purpose :
/*     This function permits to ...
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     transPower
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
int CMD60_addon_ReadNormalTransmitPower (int InstrSession,double *transPower) // new [@GC02062011]
{
	int				i,rc;
	int				iErrorCode=0;
	int				iNumBytesRead=0;
	char			cErrorMessage[512];
	unsigned char	cReadBuffer[MAX_GPIB_BUFFER];
	unsigned char	cRespBuffer[MAX_GPIB_BUFFER];
	unsigned char	cStbBuf[40];
	unsigned char	cCmd[256];

	*transPower = 0.0;
	
	sprintf(cCmd, "READ:POW:TRAN:NORM?;*STB?\n");
	
	if ( (rc=CMD60_WriteInstrData (InstrSession, cCmd)) < 0 )
	{
		for (i=0; i<10; i++)
		{
			if ( CMD60_ErrorQuery (InstrSession, &iErrorCode, cErrorMessage) )	continue;
			if ( iErrorCode == 0 )												break;
		}
		return(rc);     //error
	}

	if ( (rc=CMD60_ReadInstrData (InstrSession, MAX_GPIB_BUFFER, cReadBuffer, &iNumBytesRead)) < 0 )
	{
		for (i=0; i<10; i++)
		{
			if ( CMD60_ErrorQuery (InstrSession, &iErrorCode, cErrorMessage) )	continue;
			if ( iErrorCode == 0 )												break;
		}
		return(rc);     //error
	}

	Scan (cReadBuffer, "%s>%s[t59x]%s", cRespBuffer, cStbBuf);

	if ( (rc=CMD60_addon_Register (InstrSession, cStbBuf)) < 0 )
	{
		for (i=0; i<10; i++)
		{
			if ( CMD60_ErrorQuery (InstrSession, &iErrorCode, cErrorMessage) )	continue;
			if ( iErrorCode == 0)												break;
		}
		return(rc);     //error
	}	
	
	if (Scan (cRespBuffer, "%s>%f", transPower) == 0)
		return(VI_ERROR_INV_MEAS);		//error 
	
	return (0);
}



/*===========================================================================*/
/* Function :
/*     CMD60_addon_ReadAfmCoun
/* Purpose :
/*     This function permits to ...
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     freq
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
int CMD60_addon_ReadAfmCoun (int InstrSession,double *freq) // new [@GC10062014]
{
	int		i,rc;
	
	double dFreq=0;
	double dFreqLast=0;


	// maximal 5 x Freq auslesen bis der Wert stabil ist

	*freq = 0.0;

	for ( i=0 ; i < 5 ; i++ )
	{
	
		if ( (rc=CMD60_ReadAfmCoun(InstrSession, &dFreq)) )
		{
			return(rc);
		}
		
		if ( i > 0 && dFreq == dFreqLast )
		{
			break;
		}
		
		dFreqLast = dFreq;
		
		Sleep(200);  //200ms
		

	}
	
	// wenn Freq mehr als 2Hz schwankt dann fehler
	if ( (dFreq - dFreqLast) > 2.0 || (dFreqLast - dFreq) > 2.0 )
	{
		dFreq = -1;                 // fehler  27.06.2014 @GC
	}
	
	
	*freq = dFreq;
	
	return (0);
}




/*===========================================================================*/
/* Function :
/*     CMD60_addon_ReadFpRfpi
/* Purpose :
/*     This function permits to ...
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     RFPIValue
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
int CMD60_addon_ReadFpRfpi (int InstrSession, char RfpiValue[])
{
	int				i,rc;
	int				iErrorCode=0;
	int				iNumBytesRead=0;
	char			cErrorMessage[512];
	unsigned char	cReadBuffer[MAX_GPIB_BUFFER];
	unsigned char	cRespBuffer[MAX_GPIB_BUFFER];
	unsigned char	cStbBuf[40];
	unsigned char	cCmd[256];



//"%s<CONF:FP:RFPI \"%s\";*STB?\n"
//"%s<SENS:DET:RFPI?;*STB?\n"
	sprintf(cCmd, "CONF:FP:RFPI?;*STB?\n");
	
	if ( (rc = CMD60_WriteInstrData (InstrSession, cCmd)) < 0 )
	{
		for (i=0; i<10; i++)
		{
			if ( CMD60_ErrorQuery (InstrSession, &iErrorCode, cErrorMessage) )	continue;
			if ( iErrorCode == 0 )												break;
		}
		return(rc);     //error
	}

	if ( (rc = CMD60_ReadInstrData (InstrSession, MAX_GPIB_BUFFER, cReadBuffer, &iNumBytesRead)) < 0 )
	{
		for (i=0; i<10; i++)
		{
			if ( CMD60_ErrorQuery (InstrSession, &iErrorCode, cErrorMessage) )	continue;
			if ( iErrorCode == 0 )												break;
		}
		return(rc);     //error
	}
	cReadBuffer[iNumBytesRead] = '\0';
	
	
	Scan (cReadBuffer, "%s>%s[t59x]%s", cRespBuffer, cStbBuf);



	if ( (rc=CMD60_addon_Register (InstrSession, cStbBuf)) < 0 )
	{
		for (i=0; i<10; i++)
		{
			if ( CMD60_ErrorQuery (InstrSession, &iErrorCode, cErrorMessage) )	continue;
			if ( iErrorCode == 0)												break;
		}
		return(rc);     //error
	}	
	
	if (Scan (cRespBuffer, "%s>%s", RfpiValue) == 0)
	{	
		for (i=0; i<10; i++)
		{
			if ( CMD60_ErrorQuery (InstrSession, &iErrorCode, cErrorMessage) )	continue;
			if ( iErrorCode == 0)												break;
		}
		return(VI_ERROR_INV_MEAS);		//error 
	}	
	
	return (0);

}











/*
**==============================================================================
**
**  CMD60 activate SW Demodulation
**
**  Return = 0                 OK
**  Return = 0x02e0 ...0x02e3  CMD60 IO-Error
**
**==============================================================================
*/

int CMD60_addon_activate_SW_Demod(int InstrSession)  // [GC01062011]	
{
int iStat;

		//
		//SEND:DECT:COMMAND "6001000000";*STB?
		//SEND:DECT:COMMAND "600000";*STB?
		//
		//CONF:MESS:ESC:MT "EE60010000";*STB?
		//CONF:MESS:ESC:MT "EE60000000";*STB?
		//
		//SEND:MESS:ESC:MT;*STB?
		//

	// Dies setzt nur die Esc-Sequence in die Anzeige des CMD60
	// erst durch drücken des Button wird sie aktiviert
	// activate SW-demod for BER
	if( CMD60_ConfMessEscMT(InstrSession,"EE60010000") != 0 ) return(0x02e0); //[@GC120511]


	iStat=0;
	if( CMD60_StatDev(InstrSession, &iStat) != 0 ) return(0x02e1); //[@GC120511] 
	
	// only if connect
	if ( iStat == DCE )
	{
		
		// habe beides ausgetestet, beide Versionen erfüllen den gleichen Zweck
		// Ich hab mich für das Original entschieden
		// @GC 06.06.2011
#undef CMD60_USE_SEND_DECT_COMMAND
#ifdef CMD60_USE_SEND_DECT_COMMAND
		// Laut Rohde&schwarz ist dies für den Kommando Mode
		// activate SW-demod for BER
		if( CMD60_addon_SendCommand (InstrSession, "SEND:DECT:COMMAND \"6001000000\";*STB?\n") ) return(0x02e2);
#else		
		// Dies schickt die obige Esc-Sequence ab
		if( CMD60_addon_SendCommand (InstrSession, "SEND:MESS:ESC:MT;*STB?\n") ) return(0x02e3);
#endif		

	}

	return(0);
	
}	

/*
**==============================================================================
**
**  CMD60 deactivate SW Demodulation
**
**  Return = 0                 OK
**  Return = 0x02e4 ...0x02e7  CMD60 IO-Error
**
*/

int CMD60_addon_deactivate_SW_Demod(int InstrSession)  // [GC01062011]	
{
int iStat;
		//
		//SEND:DECT:COMMAND "6001000000";*STB?
		//SEND:DECT:COMMAND "600000";*STB?
		//
		//CONF:MESS:ESC:MT "EE60010000";*STB?
		//CONF:MESS:ESC:MT "EE60000000";*STB?
		//
		//SEND:MESS:ESC:MT;*STB?
		//

	// Dies setzt nur die Esc-Sequence in die Anzeige des CMD60
	// erst durch drücken des Button wird sie aktiviert
	// deactivate SW-demod for BER
	//if( CMD60_ConfMessEscMT(InstrSession,"EE60000000") != 0 ) return(0x02e4); //[@GC120511]
	if( CMD60_ConfMessEscMT(InstrSession,"7000000000") != 0 ) return(0x02e4); //[@GC120511]

	iStat=0;
	if( CMD60_StatDev(InstrSession, &iStat) != 0 ) return(0x02e5); //[@GC120511] 
	
	// only if connect
	if ( iStat == DCE )
	{

		// habe beides ausgetestet, beide Versionen erfüllen den gleichen Zweck
		// Ich hab mich für das Original entschieden
		// @GC 06.06.2011
#ifdef CMD60_USE_SEND_DECT_COMMAND
		// Laut Rohde&schwarz ist dies für den Kommando Mode
		// deactivate SW-demod for BER
		if( CMD60_addon_SendCommand (InstrSession, "SEND:DECT:COMMAND \"600000\";*STB?\n") ) return(0x02e6);
		
#else
		// Dies schickt die obige Esc-Sequence ab
		if( CMD60_addon_SendCommand (InstrSession, "SEND:MESS:ESC:MT;*STB?\n") ) return(0x02e7);
#endif		
		
	}	

	return(0);
	
}	

/*
**==============================================================================
**
**  CMD60 Connection mit Retry
**
**	1.) warten auf Status CMD60_SensSignStat()=Locked  ,im polling Mode
**		wenn nicht Locked, dann warten und nochmals abfragen
**	    wenn Locked, 
**		RFPI, DummyBarrier,DummySlot ueberpruefen (ob das richtige Device Locked ist)
**	2.)	CMD60_ProcConnSet() senden
**		warten ob CMD60_StatDev()=DCE  , CMD_Connected
**		wenn Status nicht Connected dann wiederholen
**	3.) NTP auslesen und abfragen ob groesser 1.00 dBm	
**		wenn NTP kleiner dann wiederholen
**
** Return = 0                 Connection ok
** Return = -1                Connection fail
** Return = 0x02f0 ...0x02ff  CMD60 IOError
**
**
** Input:
**    int  InstrSession
**    int  type             0=Loopmax fuer Functional-Test   1=Loopmax fuer Final-Test
** Output: 
**    char *cErrorInfo	    Fehler Info wird als Text in cErrorInfo abgelegt.
**
**
*/
int CMD60_addon_Connect(int InstrSession, int type,char *cErrorInfo)  // [GC12042012]	
{
char   sensDetRFPI[20];						// detected RFPI "1000000000";
char   confFpRFPI[20];						// configured FP RFPI "1000000000";
int    sensDetDummyCarrier;
int    sensDetDummySlot;
double dNTP;
int    iRCode=0; 
int    i,j,k,l;
int    rc;
long   iFlag;
long   iLock;
long   iStat;
char   cInfo[256];
	
//CMD60 Setup
	int    LOOPMAX_LOCKED;
	int    LOOPMAX_RFPI;
	int    LOOPMAX_DCE;
	int    LOOPMAX_CONNECT;

	if ( type == 0 )     // functional test
	{
		LOOPMAX_LOCKED  = 50;	// 50x 200ms = 10Sec Timeout for CMD_Locked 
	    LOOPMAX_RFPI    = 3;	// 3x  sensDetRFPI, sensDetDummyCarrier, sensDetDummySlot
	    LOOPMAX_DCE     = 3;	// 3x  CMD60_Connect
	    LOOPMAX_CONNECT = 3;	// 3x  Retry if NTP to Low or Negative
	}
	else				// final test
	{
		LOOPMAX_LOCKED  = 50;	// 50x 200ms = 10Sec Timeout for CMD_Locked 
	    LOOPMAX_RFPI    = 3;	// 3x  sensDetRFPI, sensDetDummyCarrier, sensDetDummySlot
	    LOOPMAX_DCE     = 15;	// 15x CMD60_Connect
	    LOOPMAX_CONNECT = 3;	// 3x  Retry if NTP to Low or Negative
	}
		
	strcpy(cErrorInfo,"");
// read configured FP RFPI out
	if ( CMD60_addon_ReadFpRfpi (InstrSession, confFpRFPI ) ) return(0x02f0);
	i=0;
	do	  // while ( dNTP < xx ....
	{	
		if( CMD60_StatDev(InstrSession, &iStat))  return(0x02f1);
		if ( iStat == DCE )             //device Status IDLE,FP,PP,CE,BAN,RFG 
		{	
			if ( CMD60_ProcConnRel(InstrSession)) return(0x02f2); 
		}
		j=0;
		do  // while ( iStat != DCE... 
		{
			k=0;
			do  // while ( iFlag != LOCKED      // confFpRFPI != sensDetRFPI
			{	
				// Vor dem eigentlichen Verbindungsaufbau (connection) muss auf LOCKED gewartet werden
				// nachdem der CMD conected ist kann LOCK nicht mehr abgefragt werden.
				sprintf(cInfo,"Wait_Locked %d",j);
				set_hintwindow(cInfo,COL_NEUTRAL,(testflow->running_left ? LEFT : RIGHT));	  // LEFT, RIGHT     
				l=0;
				do  // while ( iLock != LOCKED...
				{	
					Sleep(200);    //200ms
					if ( CMD60_SensSignStat (InstrSession, &iLock)) return(0x02f3);
				}while ( iLock != LOCKED	&& (++l<LOOPMAX_LOCKED) );  // 50*200ms = wait 10Sec at timeout
				
				if (iLock != LOCKED)
				{
					sprintf(cErrorInfo,"No_CMD_Lock");
					iRCode=-1;  			//look-in failure
					goto EVALUATE_FUNCTION;
				}
				// wenn LOCK gesetzt, kann rfpi,dummyCarrier,dummySlot gelesen werden
				// es wird nur noch RFPI abgefragt
				iFlag=LOCKED;
				if ( CMD60_SensDetRfpi (InstrSession, sensDetRFPI)) return(0x02f4); 
				// '"1000000000"' mit '"1000000000"' vergleichen  das " muss vorne und hinten abgeschnitten werden 
				if ( memcmp(&confFpRFPI[1],&sensDetRFPI[1], 10 ) != 0 ) 
				{
					sprintf(cErrorInfo,"RFPI=%s",sensDetRFPI);
					iFlag=UNLOCKED;     //failure 
				}
				// wenn sensDetRFPI oder sensDetDummyCarrier oder sensDetDummySlot falsch ist, dann nochmal
				if (iFlag != LOCKED)
				{
					// No Testmode
					if(CMD60_ProcSelTest(InstrSession, NOMODE)) return(0x02f7);
					// Manual Mode 
					if(CMD60_ProcSelTest(InstrSession, MANUAL)) return(0x02f8);
				}	
			}
			while ( iFlag != LOCKED && (++k<LOOPMAX_RFPI) ); 
			if (iFlag != LOCKED)
			{
				iRCode=-1;  			//wrong RFPI failure
				goto EVALUATE_FUNCTION;
			}
			sprintf(cInfo,"CMD_Connect %d",j);
			set_hintwindow(cInfo,COL_NEUTRAL,(testflow->running_left ? LEFT : RIGHT));	  // LEFT, RIGHT     
			// -- Connect Connect Connect ....
			Sleep(200);   
			if( CMD60_ProcConnSet(InstrSession) ) 
			{	
				// fail
				(void)CMD60_addon_ClearError(InstrSession);
			}	
			if( CMD60_StatDev(InstrSession, &iStat) != 0 ) return(0x02f9);  //device Status IDLE,FP,PP,CE,BAN,RFG 
		}
		while ( iStat != DCE && (++j<LOOPMAX_DCE) );   // max 10x CMD60_connect
		if (iStat != DCE)
		{
			sprintf(cErrorInfo,"No_CMD_Connect");
			iRCode=-1;  			//connection-in failure
			goto EVALUATE_FUNCTION;
		}
		/*
		** check if connection is established and a little NTP is there
		*/
		if ( CMD60_ReadNtp (InstrSession, &dNTP) ) 
		{
			// fail
			(void)CMD60_addon_ClearError(gpib_handle.cmd60);
			dNTP=0;
		}	
		// das ganze nochmal wenn CMD60_ReadNtp() keinen oder Negativen NTP hat 
	}
	while ( dNTP < -25.00 && (++i<LOOPMAX_CONNECT) );         // a little NTP  

	if ( dNTP < -25.00 )    // fail
	{
		sprintf(cErrorInfo, "No_NTP=%.2fdBm",dNTP);
		iRCode=-1;  //failure
		goto EVALUATE_FUNCTION;
	}
	set_hintwindow("",COL_NEUTRAL,(testflow->running_left ? LEFT : RIGHT));	  // LEFT, RIGHT     
	sprintf(cErrorInfo,"OK %.2fdBm",dNTP);
	/*
	** === Ende Connection =================
	*/
EVALUATE_FUNCTION:
	
#undef RFP35_DEBUG_CONNECTION_LOG		     // Logging fuer CMD_connection Logging
	
#ifdef RFP35_DEBUG_CONNECTION_LOG
	{
	  FILE *fp;
	  char cDebugInfo[256];
	  
      sprintf(cDebugInfo,"iNTP %-2d jDCE %-2d kRFPI %-2d lLOCKED %-2d %s",i,j,k,l,cErrorInfo);
	  
	  fp=fopen("connection_log.txt","a");
      fprintf(fp,"%s %s %s %-28s %s\n",DateStr(), TimeStr(), structQPDS.strExID,globals->sequ_select->structpointer[globals->sequ_select->act_index]->username,cDebugInfo);
      fclose(fp);
	}	
#endif //RFP35_DEBUG_CONNECTION_LOG
	
	
	return(iRCode);	
}

