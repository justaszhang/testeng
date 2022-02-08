//==============================================================================
//
// Title:       rtx2012_addon.h
//
// Purpose:     rtx2012 Add-on Function
//
// Created on:  07.30.2021 by Justas
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

#include <string.h>
#include <formatio.h>


#include "rtepubl.h"    
#include "rte_tool.h"   
#include "test.h"   



#include "rtx2012_addon.h"
#include "rtx2012base.h"


#define MAX_READ_BUFFER         4000L 

//==============================================================================


int rtx2012_addon_ClearError (void)
{
	int				i;
	int				iErrorCode=0;  
	char			cErrorMessage[512];


	for (i=0; i<10; i++)
	{
		if ( !rtx2012_ErrorQuery (&iErrorCode, cErrorMessage) )	continue;
		if ( iErrorCode == 0 )			break; 		 
	}
	return (1);

}

int rtx2012_addon_SetPacketTypePP32Z (void)
{

	int	i = rtx2012_Write("CONF:POW:PACK PP32Z");
	if(i != 1)
		return (0);

	return 1;
    
}

/*int rtx2012_addon_SendCommand (unsigned char *cCmd) // new [@JM211022]
{
	int				i,rc;
	int				iErrorCode=0;
	int				iNumBytesRead=0;
	char			cErrorMessage[512];
	unsigned char	cReadBuffer[MAX_GPIB_BUFFER];

	if ( (rc=rtx2012_Write ( cCmd)) != 1 )
	{
		for (i=0; i<10; i++)
		{
			if ( !rtx2012_ErrorQuery (cErrorMessage) )	continue;
			if (CompareStrings ("No Error", 0, cErrorMessage, 0, 1) == 0) 		break;

		}
		return(0x01);     //error
	}


	return (0);
}		*/


int rtx2012_addon_ReadNormalTransmitPower (double *transPower) // new [@GC02062011]
{
	int				i,rc;
	int				iErrorCode=0;
	int				iNumBytesRead=0;
	char			cErrorMessage[512];
	unsigned char	cReadBuffer[MAX_READ_BUFFER];
	unsigned char	cRespBuffer[MAX_READ_BUFFER];
	unsigned char	cStbBuf[40];
	unsigned char	cCmd[256];

	*transPower = 0.0;
	
	sprintf(cCmd, "READ:POW:TRAN:NORM?;*STB?\n");
	
	if ( !rtx2012_Query (cCmd,cReadBuffer) )
	{
		for (i=0; i<10; i++)
		{
			if ( !rtx2012_ErrorQuery (&iErrorCode, cErrorMessage) )	continue;
		    if ( iErrorCode == 0 )			break; 		
		}
		return(0);     //error
	}



	Scan (cReadBuffer, "%s>%s[t59x]%s", cRespBuffer, cStbBuf);

	*transPower  = atof(cRespBuffer); 
	
	return (1);
}



int rtx2012_addon_Connect (int type, char *cErrorInfo)
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
	if ( !rtx2012_addon_ReadFpRfpi ( confFpRFPI ) ) return(0);
	i=0;
	do	  // while ( dNTP < xx ....
	{
		if( !rtx2012_StatDev(&iStat))  return(0);
		if ( iStat == DCE )             //device Status IDLE,FP,PP,CE,BAN,RFG
		{
			if ( !rtx2012_ProcConnRel()) return(0);
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
					if ( !rtx2012_SensSignStat ( &iLock)) return(0);
				}
				while ( iLock != LOCKED	&& (++l<LOOPMAX_LOCKED) );   // 50*200ms = wait 10Sec at timeout

				if (iLock != LOCKED)
				{
					sprintf(cErrorInfo,"No_CMD_Lock");
					iRCode=0;  			//look-in failure
					goto EVALUATE_FUNCTION;
				}
				// wenn LOCK gesetzt, kann rfpi,dummyCarrier,dummySlot gelesen werden
				// es wird nur noch RFPI abgefragt
				iFlag=LOCKED;
				if ( !rtx2012_SensDetRfpi (sensDetRFPI)) return(0);
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
					if(!rtx2012_ProcSelTest(NOMODE)) return(0);
					// Manual Mode
					if(!rtx2012_ProcSelTest(MANUAL)) return(0);
				}
			}
			while ( iFlag != LOCKED && (++k<LOOPMAX_RFPI) );
			if (iFlag != LOCKED)
			{
				iRCode=0;  			//wrong RFPI failure
				goto EVALUATE_FUNCTION;
			}
			sprintf(cInfo,"CMD_Connect %d",j);
			set_hintwindow(cInfo,COL_NEUTRAL,(testflow->running_left ? LEFT : RIGHT));	  // LEFT, RIGHT
			// -- Connect Connect Connect ....
			Sleep(200);
			if( !rtx2012_ProcConnSet() )
			{
				// fail
				(void)rtx2012_addon_ClearError();
			}
			if( !rtx2012_StatDev( &iStat)) return(0);  //device Status IDLE,FP,PP,CE,BAN,RFG
		}
		while ( iStat != DCE && (++j<LOOPMAX_DCE) );   // max 10x CMD60_connect
		if (iStat != DCE)
		{
			sprintf(cErrorInfo,"No_CMD_Connect");
			iRCode=0;  			//connection-in failure
			goto EVALUATE_FUNCTION;
		}
		/*
		** check if connection is established and a little NTP is there
		*/
		if ( !rtx2012_ReadNtp ( &dNTP) )
		{
			// fail
			(void)rtx2012_addon_ClearError();
			dNTP=0;
		}
		// das ganze nochmal wenn CMD60_ReadNtp() keinen oder Negativen NTP hat
	}
	while ( dNTP < -25.00 && (++i<LOOPMAX_CONNECT) );         // a little NTP

	if ( dNTP < -25.00 )    // fail
	{
		sprintf(cErrorInfo, "No_NTP=%.2fdBm",dNTP);
		iRCode=0;  //failure
		goto EVALUATE_FUNCTION;
	}
	set_hintwindow("",COL_NEUTRAL,(testflow->running_left ? LEFT : RIGHT));	  // LEFT, RIGHT
	sprintf(cErrorInfo,"OK %.2fdBm",dNTP);
	
	iRCode = 1;
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



int rtx2012_addon_ReadFpRfpi (char *rfpiValue)
{
	
	
	int i = 0;
	char outp[1024] = {0};


	i = rtx2012_Query("CONF:FP:RFPI?",outp);
	if(i != 1)
		return (0);
	
	strcpy(rfpiValue,"\"");


	strcat(rfpiValue,outp);
	
	strcat(rfpiValue,"\"");


	
	return 1;
    
}
/*int rtx2012_addon_deactivate_SW_Demod(int InstrSession)  // [GC01062011]	
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
	if( !rtx2012_ConfMessEscMT("7000000000")) return(0); //[@GC120511]

	iStat=0;
	if( !rtx2012_StatDev( &iStat)) return(0); //[@GC120511] 
	
	// only if connect
	if ( iStat == DCE )
	{

		// habe beides ausgetestet, beide Versionen erfüllen den gleichen Zweck
		// Ich hab mich für das Original entschieden
		// @GC 06.06.2011
#ifdef CMD60_USE_SEND_DECT_COMMAND
		// Laut Rohde&schwarz ist dies für den Kommando Mode
		// deactivate SW-demod for BER
		if( !rtx2012_Write ("SEND:DECT:COMMAND \"600000\"") ) return(0);
		
#else
		// Dies schickt die obige Esc-Sequence ab
		if( !rtx2012_Write ("SEND:MESS:ESC:MT") ) return(0);
#endif		
		
	}	

	return(1);
	
}	

				*/


int rtx2012_addon_activate_SW_Demod()  // [GC01062011]	
{
	MessagePopup("Error", "something goes wrong.");
	return 0;
	
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
	if( !rtx2012_ConfMessEscMT("EE60010000")) return(0); //[@GC120511]


	iStat=0;
	if( !rtx2012_StatDev( &iStat)) return(0); //[@GC120511] 
	
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
		if( !rtx2012_Write ("SEND:DECT:COMMAND \"6001000000\"") ) return(0);
#else		
		// Dies schickt die obige Esc-Sequence ab
		if( !rtx2012_Write ("SEND:MESS:ESC:MT;") ) return(0);
#endif		

	}

	return(1);
	
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

int rtx2012_addon_deactivate_SW_Demod(int InstrSession)  // [GC01062011]	
{
	MessagePopup("Error", "something goes wrong.");
	return 0;
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
	if( !rtx2012_ConfMessEscMT("7000000000")) return(0); //[@GC120511]

	iStat=0;
	if( !rtx2012_StatDev( &iStat)) return(0); //[@GC120511] 
	
	// only if connect
	if ( iStat == DCE )
	{

		// habe beides ausgetestet, beide Versionen erfüllen den gleichen Zweck
		// Ich hab mich für das Original entschieden
		// @GC 06.06.2011
#ifdef CMD60_USE_SEND_DECT_COMMAND
		// Laut Rohde&schwarz ist dies für den Kommando Mode
		// deactivate SW-demod for BER
		if( !rtx2012_Write ("SEND:DECT:COMMAND \"600000\"") ) return(0);
		
#else
		// Dies schickt die obige Esc-Sequence ab
		if( !rtx2012_Write ("SEND:MESS:ESC:MT") ) return(0);
#endif		
		
	}	

	return(1);
	
}	
