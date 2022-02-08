#include <userint.h>
#include "break.h"

#include "test.h"


#define RFP35_DEBUG_WRITEHWCONFIG   		 // copy all Logfiles for Debug only  @GC 28.02.2012 
#define RFP35_DEBUG_GENERATEPARKXML 		 // copy all Logfiles for Debug only  @GC 10.09.2013 

#define RFP35_CHECKRTC_WITHOUT_NTP           // ab Firmware SIP-DECT 6.0SP2-BA11 NTP nicht mehr abfragen @GC 02.02.2016

#define RFP35_DEBUG_WLAN_TXPOWER   		     // copy all Logfiles for Debug only  @GC 25.07.2013 



int gWLANAnt1, gWLANAnt2, gWLANAnt3;  
char gResponseLocal[0x400];


char cInfo[256];

//  konvertiert ulongIp nach strIp
void local_ulongIp_to_strIp(unsigned long ulIp, char *strIp)
{
	sprintf(strIp,"%d.%d.%d.%d",((ulIp>>24)&0xff),((ulIp>>16)&0xff),((ulIp>>8)&0xff),((ulIp)&0xff));
	return;
}	

//  konvertiert strIp nach ulongIp
unsigned long local_strIp_to_ulongIp(char *strIp)
{
	unsigned long ip[4]={0,0,0,0};
	sscanf( strIp ,"%d.%d.%d.%d",&ip[3],&ip[2],&ip[1],&ip[0]);
	return(  ((ip[3]&0xff)<<24) + ((ip[2]&0xff)<<16) + ((ip[1]&0xff)<<8) + (ip[0]&0xff)  );
}	




/***********************************************************************************************
**		Main Testfunctionen
***********************************************************************************************/
/***************************************************************            
** SPECIFICATION:  contact nails, DUT-Power on
***************************************************************/    
TESTFUNCTION PowerOn(struct struct_test *act_structptr)						// @GC18.09.2012  
{
int 	retry;
int 	rc;
int     iMode;
char cInfo[256];

	set_testheader(0);

//LAN connection ist connected, only the power is switched
	if(MiniSys_set_OC(5, 1))   // switch power on via opto-switch
		return(0x1001); //1..8:0|1    
 	set_hintwindow("Power on",COL_NEUTRAL,(testflow->running_left ? LEFT : RIGHT));	  // LEFT, RIGHT     
   	time(&gl_timePowerOn);                 // Zeitstempel PowerOn fuer Test CheckRTC
// old ..
	CMD60_Reset (gpib_handle.cmd60);
	Sleep(1000);						   // wait for later current measurement
	strcpy(strIP_Address,"0.0.0.0");        		// global Variable used for all Test
	ipRfp_clearTempFiles();                         // clear all temporary files
	act_structptr->resultstatus = check_value(0);
	return(0);
}




TESTFUNCTION SupplyCurrentPSU(struct struct_test *act_structptr)	// @GC18.09.2012  
{
 // measure via 1 Ohm shunt resistor 
static int    initFlag;
double current,value=0;
int m=0;

	Sleep(5000);
	current = -1;
//	if(!initFlag)
	{
// setup ADC:
		if(MiniSys_ADC_RaSe(MINISYS_ADC_RANGE_DC, 2)) return (0x1010);  
		if(MiniSys_ADC_InpS(8, 16)) return (0x1011); 
		initFlag = 1;
	}

	for(m=0;m<20;m++)	
		{
		Sleep(500);
		if(MiniSys_ADC_getVdc(&current)) return (0x1012);
		if((current>0.05)&&(current<0.16))
			break;
		}

	current=current*1000;
	act_structptr->resultstatus = check_value(current);
	return(0);			   
}
	

TESTFUNCTION SSH_PortConnect(struct struct_test *act_structptr)	   // @GC16.01.2013  
{
int     i,n;
int 	retry;
int 	rc;
int     waittime;


	strcpy(strIP_Address,act_structptr->string[0]);        //global Variable used for all Test
// get boot up wait time	
	waittime = act_structptr->intpara[0];   // wait time between ping and ssh open 
//wrong parameter
	if ( waittime < 0 || waittime > 90 ) return(0x2022);
	for(i=waittime;i>0;i--)
	{
 		sprintf(cInfo,"boot wait: %d seconds",i);
		set_hintwindow( cInfo ,COL_NEUTRAL,(testflow->running_left ? LEFT : RIGHT));	  // LEFT, RIGHT
		Sleep(1000);
	}

//	if (!strcmp(strHwType,"RFP43")) //only for testing WLAN-POWER
//
// ipAddr von ulong nach string konvertieren
 	set_hintwindow("waiting....",COL_NEUTRAL,(testflow->running_left ? LEFT : RIGHT));	  // LEFT, RIGHT     
	for ( rc=0,  retry=0 ; retry < 10 ; retry++ )      
	{
		sprintf(cInfo,"ping %s",strIP_Address);	
		set_hintwindow( cInfo ,COL_NEUTRAL,(testflow->running_left ? LEFT : RIGHT));	  // LEFT, RIGHT     
		
		if ((rc=ipRfp_SystemPing(strIP_Address,1000)) != 0 )          // 1000 ms
		{
// ip found
			sprintf(cInfo,"SSH %s",strIP_Address);	
 			set_hintwindow( cInfo ,COL_NEUTRAL,(testflow->running_left ? LEFT : RIGHT));	  // LEFT, RIGHT     
			if ( (rc=ipRfp_libssh_Open(strIP_Address)) == 0 ) 
			{
				break;  // ok
			}	
		}
	}	
	if ( rc != 0 ) 
	{
		act_structptr->resultstatus = check_value(4);		// fail ssh don't work
		return(0);
	}	
	if(ipRfp_libssh_changeToRootUser())
	{
		act_structptr->resultstatus = check_value(41);		// fail ssh don't work
		return(0);
	}
	if ( ipRfp_libssh_WriteAndRead( "fmanu button test on\r", gResponseLocal, sizeof(gResponseLocal), "button test mode set to 'on'", 1.0 ) != 0 ) return(-1);       
	act_structptr->resultstatus =  check_value(0);
	return(0);
}

/*
TESTFUNCTION Check_MAC_Serial(struct struct_test *act_structptr)
{
int value=0;
char *p, *searchStr;
char localWriteBuffer[0x40];
int i;
// get MAC and serial 	
	if ( ipRfp_libssh_WriteAndRead( "fmanu conf\r", gResponseLocal, sizeof(gResponseLocal), "# ", 1.0 ) != 0 ) return(-2);  
//get MAC:
	searchStr= "mac     :  ";
	
	if ( (p = strstr(gResponseLocal,searchStr)) != NULL )
	{
		for ( p=p+strlen(searchStr) ; *p == ' ' ; p++ );			// anfang suchen
		for ( i=0; *p != '\n' && *p != '\r' && *p != '\0'; p++ )	// kopieren bis ende zeile
		{	
	    	gMAC_DUT[i]=toupper(*p);					// reading only LowChar  
			i++;									// change to Up
	    	gMAC_DUT[i]='\0';							//  
		}	
	}	
	
//get Serial:
	searchStr= "sn      :  ";
	
	if ( (p = strstr(gResponseLocal,searchStr)) != NULL )
	{
		for ( p=p+strlen(searchStr) ; *p == ' ' ; p++ );			// anfang suchen
		for ( i=0; *p != '\n' && *p != '\r' && *p != '\0'; p++ )	// kopieren bis ende zeile
		{	
	    	gSerialNr_DUT[i]= *p;	
	    	i++;									// 
	    	gSerialNr_DUT[i]='\0';							// 
		}	
	}	
 	
	set_char_serial_number(gSerialNr_DUT);
	sprintf(cInfo,"MAC:%s",gMAC_DUT);	
	print_resultdisplay(cInfo);
// scanned numbers coming from:
//	GetBarCode(gMAC_Scanned, gSerialNr_Scanned);
	if(strcmp(gMAC_DUT, gMAC_Scanned)) // MAC have to be the same
	{	
		act_structptr->resultstatus = check_value(10);
		return(0);   
	}
	if(strcmp(gSerialNr_DUT, gSerialNr_Scanned)) // serial number can be new, but the first 3 char (HW-Type) must the same
	{
		if(strncmp(gSerialNr_DUT, gSerialNr_Scanned,3))
		{	
			act_structptr->resultstatus = check_value(20);
			return(0);   
		}
		if ( ipRfp_libssh_WriteAndRead( "touch /tmp/setsn\r", gResponseLocal, sizeof(gResponseLocal), "# ", 1.0 ) != 0 ) return(-3);  
		sprintf(localWriteBuffer, "fmanu conf -sn %s\r", gSerialNr_Scanned);
		if ( ipRfp_libssh_WriteAndRead(localWriteBuffer, gResponseLocal, sizeof(gResponseLocal), "# ", 1.0 ) != 0 ) return(-3);  
		
	}
	act_structptr->resultstatus = check_value(value);
	return(0);   

}
*/

TESTFUNCTION Check_MAC_Serial(struct struct_test *act_structptr)
{
	int value=0, rfp47Exception;
	char *p, *searchStr;
	char localWriteBuffer[0x40];
	char serialNrRFP47Exception[0x20];
	char cHW[10] = {0};

	int i;
// get MAC and serial
	if ( ipRfp_libssh_WriteAndRead( "fmanu conf\r", gResponseLocal, sizeof(gResponseLocal), "# ", 1.0 ) != 0 ) return(-2);

	//////////////////////////
	//get HW type:
	searchStr= "hw      :  ";

	if ( (p = strstr(gResponseLocal,searchStr)) != NULL )
	{
		for ( p=p+strlen(searchStr) ; *p == ' ' ; p++ );
		for ( i=0; *p != '\n' && *p != '\r' && *p != '\0'; p++ )
		{
			cHW[i]=toupper(*p);
			i++;
			cHW[i]='\0';
		}
	}

	if(strncmp(cHW,"RFP44",5) == 0)
	{
		if(strncmp(gSerialNr_Scanned, "3BY",3) != 0)
			return(-1);
	}
	else if(strncmp(cHW,"RFP45",5) == 0)
	{
		if((strncmp(gSerialNr_Scanned, "3BX",3) != 0)&& (strncmp(gSerialNr_Scanned, "3HG",3) != 0) && (strncmp(gSerialNr_Scanned, "3E7",3) != 0)&& (strncmp(gSerialNr_Scanned, "3L9",3) != 0))
			return(-1);
	}
	else if(strncmp(cHW,"RFP47",5) == 0)
	{
		if((strncmp(gSerialNr_Scanned, "3BZ",3) != 0) && (strncmp(gSerialNr_Scanned, "3C1",3) != 0))
			return(-1);
	}
	else if(strncmp(cHW,"RFP48",5) == 0)
	{
		if(strncmp(gSerialNr_Scanned, "3C2",3) != 0)
			return(-1);
	}
	else
	{
		return(-1);

	}
	/////////////////////////////////
	////////////////////////






//get MAC:
	searchStr= "mac     :  ";

	if ( (p = strstr(gResponseLocal,searchStr)) != NULL )
	{
		for ( p=p+strlen(searchStr) ; *p == ' ' ; p++ );			// anfang suchen
		for ( i=0; *p != '\n' && *p != '\r' && *p != '\0'; p++ )	// kopieren bis ende zeile
		{
			gMAC_DUT[i]=toupper(*p);					// reading only LowChar
			i++;									// change to Up
			gMAC_DUT[i]='\0';							//
		}
	}

//get Serial:
	searchStr= "sn      :  ";

	if ( (p = strstr(gResponseLocal,searchStr)) != NULL )
	{
		for ( p=p+strlen(searchStr) ; *p == ' ' ; p++ );			// anfang suchen
		for ( i=0; *p != '\n' && *p != '\r' && *p != '\0'; p++ )	// kopieren bis ende zeile
		{
			gSerialNr_DUT[i]= *p;
			i++;									//
			gSerialNr_DUT[i]='\0';							//
		}
	}

	set_char_serial_number(gSerialNr_DUT);
	sprintf(cInfo,"MAC:%s",gMAC_DUT);
	print_resultdisplay(cInfo);
// scanned numbers coming from:
//	GetBarCode(gMAC_Scanned, gSerialNr_Scanned);
	if(strcmp(gMAC_DUT, gMAC_Scanned)) // MAC have to be the same
	{
		act_structptr->resultstatus = check_value(10);
		return(0);
	}

//#### new, 20181121: for Type RFP47 DRC we allow, that the boardlevel label/programmed number have 3BZ
	if(strcmp(gSerialNr_DUT, gSerialNr_Scanned)) // serial number can be new, but the first 3 char (HW-Type) must the same
	{
		// there is a difference
		rfp47Exception = 0;
		if(strncmp(gSerialNr_DUT, gSerialNr_Scanned,3))
		{
			/*// there is a difference, only allowed for RFP47 DRC
			if(!strncmp(gSerialNr_Scanned,"3C1",3))
			{
				// we have type RFP47 DRC
				if(!strncmp(gSerialNr_DUT,"3BZ",3))
				{
					// board level is type RFP47 indoor, make a copy and replace the product code from scanned label (for logging the original scanned label later)
					rfp47Exception = 1;
					strcpy(serialNrRFP47Exception,gSerialNr_Scanned);
					serialNrRFP47Exception[1] = 'B';
					serialNrRFP47Exception[2] = 'Z';
				}
				else // boardlevel != 3BZ
				{
					act_structptr->resultstatus = check_value(30);
					return(0);
				}
			}
			else
			{
				act_structptr->resultstatus = check_value(20);
				return(0);
			}*/
			// there is a difference  , not allowed
			act_structptr->resultstatus = check_value(20);
			return(0);
		}


		if ( ipRfp_libssh_WriteAndRead( "touch /tmp/setsn\r", gResponseLocal, sizeof(gResponseLocal), "# ", 1.0 ) != 0 ) return(-3);
		if(rfp47Exception)
			sprintf(localWriteBuffer, "fmanu conf -sn %s\r", serialNrRFP47Exception);
		else
			sprintf(localWriteBuffer, "fmanu conf -sn %s\r", gSerialNr_Scanned);
		if ( ipRfp_libssh_WriteAndRead(localWriteBuffer, gResponseLocal, sizeof(gResponseLocal), "# ", 1.0 ) != 0 ) return(-3);

		//verify SN overwrite
		if ( (p = strstr(gResponseLocal,"unknown")) != NULL )
		{
			act_structptr->resultstatus = check_value(20);
			return(0);
		}
		if ( (p = strstr(gResponseLocal,"mismatch")) != NULL )
		{
			act_structptr->resultstatus = check_value(20);
			return(0);
		}

		if ( (p = strstr(gResponseLocal,"not")) != NULL )
		{
			act_structptr->resultstatus = check_value(20);
			return(0);
		}
		if ( (p = strstr(gResponseLocal,"Error")) != NULL )
		{
			act_structptr->resultstatus = check_value(20);
			return(0);
		}
		if ( (p = strstr(gResponseLocal,"wrong")) != NULL )
		{
			act_structptr->resultstatus = check_value(20);
			return(0);
		}


		// get MAC and serial
		if ( ipRfp_libssh_WriteAndRead( "fmanu conf\r", gResponseLocal, sizeof(gResponseLocal), "# ", 1.0 ) != 0 ) return(-2);
		searchStr= "sn      :  ";

		if ( (p = strstr(gResponseLocal,searchStr)) != NULL )
		{
			for ( p=p+strlen(searchStr) ; *p == ' ' ; p++ );			// anfang suchen
			for ( i=0; *p != '\n' && *p != '\r' && *p != '\0'; p++ )	// kopieren bis ende zeile
			{
				gSerialNr_DUT[i]= *p;
				i++;									//
				gSerialNr_DUT[i]='\0';							//
			}
		}

		if(strcmp(gSerialNr_DUT, gSerialNr_Scanned)  != 0)
		{
			act_structptr->resultstatus = check_value(20);
			return(0);
		}






	}


	//check boot flash mac address
	char cMAC_DUT_bootflash[0x20] = {0};

	if ( ipRfp_libssh_WriteAndRead( "ifconfig\r", gResponseLocal, sizeof(gResponseLocal), "# ", 1.0 ) != 0 ) return(-2);

	searchStr= "HWaddr ";

	if ( (p = strstr(gResponseLocal,searchStr)) != NULL )
	{
		for ( p=p+strlen(searchStr) ; *p == ' ' ; p++ );			// anfang suchen
		for ( i=0; *p != '\n' && *p != '\r' && *p != '\0'; p++ )	// kopieren bis ende zeile
		{
			cMAC_DUT_bootflash[i]=toupper(*p);					// reading only LowChar
			i++;									// change to Up
			cMAC_DUT_bootflash[i]='\0';							//
		}
	}

	sprintf(cInfo,"boot MAC:%s",cMAC_DUT_bootflash);
	print_resultdisplay(cInfo);

	if(strncmp(cMAC_DUT_bootflash, gMAC_Scanned,17)) // boot flash MAC have to be the same
	{
		act_structptr->resultstatus = check_value(10);
		return(0);
	}


	act_structptr->resultstatus = check_value(value);
	return(0);

}

TESTFUNCTION LogDUTSerialNumber(struct struct_test *act_structptr)
{
	act_structptr->resultstatus = check_value_str(gSerialNr_DUT);	   
	return(0);                                   
}                                            

TESTFUNCTION LogScannedSerialNumber(struct struct_test *act_structptr)
{
	act_structptr->resultstatus = check_value_str(gSerialNr_Scanned);	   
	return(0);                                   
}                                            

TESTFUNCTION LogMAC(struct struct_test *act_structptr)
{
	act_structptr->resultstatus = check_value_str(gMAC_Scanned);	   
	return(0);                                   
}                                            

TESTFUNCTION CheckLED_red(struct struct_test *act_structptr)
{
int value=0;

	if(ipRfp_libssh_setLED("red"))
		return(0x0501);
	Sleep(500);
	value=MiniSys_getLightSensor();  
	if(!strncmp(gSerialNr_DUT,"3C2",3))	   // add offset for RFP48 (longer lightpipe, other sensorposition)
		value += act_structptr->intpara[0];
	act_structptr->resultstatus = check_value(value);
	return(0);   

}

TESTFUNCTION CheckLED_green(struct struct_test *act_structptr)
{
int value=0;

	if(ipRfp_libssh_setLED("green"))
		return(0x0501);
	Sleep(500);
	value=MiniSys_getLightSensor();  
	if(!strncmp(gSerialNr_DUT,"3C2",3))	   // add offset for RFP48 (longer lightpipe, other sensorposition)
		value += act_structptr->intpara[0];
	act_structptr->resultstatus = check_value(value);
	return(0);   
}

TESTFUNCTION CheckLED_blue(struct struct_test *act_structptr)
{
int value=0;

	if(ipRfp_libssh_setLED("blue"))
		return(0x0501);
	Sleep(500);
	value=MiniSys_getLightSensor();  
	if(!strncmp(gSerialNr_DUT,"3C2",3))	   // add offset for RFP48 (longer lightpipe, other sensorposition)
		value += act_structptr->intpara[0];
	act_structptr->resultstatus = check_value(value);
	if(ipRfp_libssh_setLED("system"))
		return(0x0501);
	return(0);   
}

TESTFUNCTION Check_button(struct struct_test *act_structptr)
{
int i;

	iCheckOkay = 0;
	DisplayPanel (PanelBreak);		// 



// need time for this: long timeout !!	
	if ( ipRfp_libssh_WriteAndRead( "fmanu button test on\r", gResponseLocal, sizeof(gResponseLocal), "button test mode set to 'on'", 1.0 ) != 0 ) return(-1);       
	if ( ipRfp_libssh_WriteAndRead( "fmanu button\r", gResponseLocal, sizeof(gResponseLocal), "button state: unpressed", 1.0 ) != 0 ) return(-2);

//activate actor
	MiniSys_write("Rel:7:1");
	MiniSys_write("Rel:8:1");

	do
	{
	
		SetCtrlVal (PanelBreak, PANELBREAK_LED,1);	
		set_hintwindow("Please press Button !!",COL_RED,RIGHT);
		ProcessDrawEvents (); 
		ProcessSystemEvents();
		Sleep(300);
		set_hintwindow("Please press Button !!",COL_YELLOW, RIGHT);
		SetCtrlVal (PanelBreak, PANELBREAK_LED,0);	
		ProcessDrawEvents (); 
		Sleep(300);
		if ( ipRfp_libssh_WriteAndRead( "fmanu button\r", gResponseLocal, sizeof(gResponseLocal), "button state:", 1.0 ) != 0 ) return(-2);  
		if(strstr(gResponseLocal, " pressed")) break;
 		ProcessSystemEvents();
		if(iCheckOkay == 0x752) //is set from okay-button
			break;
		
	}while(1);

//deactivate actor
	MiniSys_write("Rel:7:0");
	MiniSys_write("Rel:8:0");

	HidePanel (PanelBreak);
	if(iCheckOkay == 0x752) 
		act_structptr->resultstatus = check_value(0);
	else
		act_structptr->resultstatus = check_value(1);
	return(0);   
}


TESTFUNCTION IP_PortConnect(struct struct_test *act_structptr)
{
int i;
int error;
int retry;
int rc;
char respBuffer[256];
	sw_stand[0]= '0';
	sw_stand[1]= '0';
	sw_stand[2]= '0';
 	set_hintwindow("enableRSX",COL_NEUTRAL,(testflow->running_left ? LEFT : RIGHT));	  // LEFT, RIGHT     
	if ( ipRfp_libssh_enableRSX() != 0 )
	{	
		act_structptr->resultstatus = check_value(1);
		return(0);
	}
 	set_hintwindow("openRSX",COL_NEUTRAL,(testflow->running_left ? LEFT : RIGHT));	  // LEFT, RIGHT     
// 3 x Retry @GC 22.01.2013
// 6 x Retry @GC 28.02.2013
// open socket
	for ( i=5 ; i > 0 ; i-- )
	{	
		Sleep(2500);   //2Sec need to start rsx deamon
		if ( (rc=ipRfp_rsx_Open(strIP_Address,38477)) == 0 )
			break;
	}
	if ( rc != 0 )
	{	
		act_structptr->resultstatus = check_value(2);
		return(0);
	}
	Sleep(200);
 	set_hintwindow("getSwVersion",COL_NEUTRAL,(testflow->running_left ? LEFT : RIGHT));	  // LEFT, RIGHT     
	if (  ipRfp_rsx_GetSoftwareVersion_Retry(respBuffer,5) == NULL )     // 5x Retry 
	{
		act_structptr->resultstatus = check_value(3);
		return(0);
	}	
// hier nur auslesen und in globale Variable schreiben
	sw_stand[0]=respBuffer[0];
	sw_stand[1]=respBuffer[1];
	sw_stand[2]=respBuffer[2];
 	set_hintwindow("EnterTestmode",COL_NEUTRAL,(testflow->running_left ? LEFT : RIGHT));	  // LEFT, RIGHT     
	if ( ipRfp_rsx_EnterTestmode() )
	{
		act_structptr->resultstatus = check_value(4);
		return(0);
	}
// check enter testmode is ready
// funct only in testmode
// use default Freq Band
	if ( ipRfp_rsx_SetFrequencyBand((unsigned char)1) )       
	{
		act_structptr->resultstatus = check_value(5);
		return(0);
	}
 	set_hintwindow("",COL_NEUTRAL,(testflow->running_left ? LEFT : RIGHT));	  // LEFT, RIGHT     
	act_structptr->resultstatus = check_value(6);
	return(0);
}


TESTFUNCTION CheckDECTSwVersion(struct struct_test *act_structptr)
{

char temp_str[256];
char *pSoftwRelease;

	if(testflow->running_left)
		pSoftwRelease=softw_release_left;
	else
		pSoftwRelease=softw_release_right;
// hier nur auswerten
// ist im IP_PortConnect gelesen worden
	sprintf(temp_str,"%d.%d.%d",sw_stand[0],sw_stand[1],sw_stand[2]);
//MessagePopup ("Status", temp_str);
	act_structptr->resultstatus = check_value_str_cmp(temp_str,pSoftwRelease);
	return(0);
}




TESTFUNCTION RFP_CMD60_connect(struct struct_test *act_structptr)
{
char   szRFPI[20]  = "1000000000";
int    i;
int    rc;
char   cInfo[256];
char   cErrorInfo[256];
double dNTP;
double dHfOffset;     
double dRfLevel; 
double dCarrierOffset;
int    iFrequencyBand;
int    iPowerLevel;
int rfpart,radio;	

/*
#define RFP44_3BY 50006973
#define RFP45_3BX 50006972
#define RFP47_3BZ 50006974
#define RFP47DRC_3C1 50006975
#define RFP48_3C2 50006976
*/


	if((!strncmp(gSerialNr_DUT,"3C1",3)) || (!strncmp(gSerialNr_DUT,"3BZ",3)))
	{ //RFP47DRC_3C1 and 3BZ
// switch to connector board with SMA
		if(MiniSys_set_OC(8, 1)) return(0x1026); // OC8 for selection  radiated or Nails, 0 == radiated
	}
	else
	{
// check part type and switch the DECT-connection
// set internal Antenna Relais , if we use connector board with SMA, switch on !
		if(MiniSys_set_OC(8, 0)) return(0x1026); // OC8 for selection  radiated or Nails, 0 == radiated
	}
//assign HF-offset, traficcarrier and -slot dependent from radio
	dHfOffset = act_structptr->doublepara[1];
	iTrafficslot = 6;		 // iTrafficslot gerade = gerades Radio (0)
	iTrafficcarrier = 0;
	dCarrierOffset = 0;
	radio=0;                 
	(void)CMD60_Reset(gpib_handle.cmd60);
//CMD-setup:	
// Set Fixed Part 
	if(CMD60_ConfTest(gpib_handle.cmd60, FP)) return(0x020c);
// new setup for radiated measurement:
	if(!act_structptr->intpara[0])
	{
// set the correct antenna on SMA-connector..
		if(MiniSys_set_OC(6, 0)) return(0x1026); // OC6 for selection   Nails, 0 == left, 1=right
//set Antenna   0 or 1	
		if(CMD60_ConfFpAnt(gpib_handle.cmd60, 0)) return(0x0214);   // default after reset is 0
		if(ipRfp_rsx_SetDiversity(0x01)) return(0x0525);	  //sollte J33 sein
	}
	else
	{
// set the correct antenna on SMA-connector..
		if(MiniSys_set_OC(6, 1)) return(0x1026); // OC6 for selection   Nails, 0 == left, 1=right
// !!!! 10180905: do not work, the RFP makes every time connect via J33 :((, it works, there is enougth power (diversity switch: about 23..27 dBm), hmmm ...		
		if(CMD60_ConfFpAnt(gpib_handle.cmd60, 1)) return(0x0214);   // default after reset is 0, set to 1
		if(ipRfp_rsx_SetDiversity(0x02)) return(0x0525);	  //sollte J32 sein
	}
// Loopback Mode	
	if(CMD60_ConfFpSignMode(gpib_handle.cmd60, LOOPBACK)) return(0x020d);
//set packet type PP32Z (prolonged preamble)
	if( CMD60_addon_SetPacketTypePP32Z(gpib_handle.cmd60) != 0 ) return(0x023e);
// RF-Level:
	dRfLevel = -45.0 + dHfOffset;
	if(dRfLevel > -40)	dRfLevel = -40;
	if(CMD60_ConfFpRfLev(gpib_handle.cmd60, dRfLevel)) return(0x020e);
//Traficslot + -carrier
	if(CMD60_ConfFpChannels(gpib_handle.cmd60, iTrafficslot, iTrafficcarrier, dCarrierOffset)) return(0x020f);
// loop back data type : 00001111 
	if(CMD60_ConfModDataType(gpib_handle.cmd60,FIG2)) return(0x0210);
// set Bursts for NTP and MAX/MIN modulation (1..12000 default=4)
	if(CMD60_ConfAverBurs  (gpib_handle.cmd60, 4)) return(0x0211);
//set Bursts for Timing  (2..2000  default=32)
	if(CMD60_ConfTimAverBurs (gpib_handle.cmd60, 32)) return(0x0212);
//set packet type PP32Z (prolonged preamble)
	if( CMD60_addon_SetPacketTypePP32Z(gpib_handle.cmd60) != 0 ) return(0x0213); //[@JM080918]
//set "Radio Fixed Part Identity"
	if((CMD60_ConfFpRfpi(gpib_handle.cmd60, szRFPI)) != 0 ) return(0x0215);
// Manual Mode 
	if(CMD60_ProcSelTest(gpib_handle.cmd60, MANUAL)) return(0x0216);
// setup connection 
	rc = CMD60_addon_Connect(gpib_handle.cmd60, /*function*/0, cErrorInfo);
	if (rc < 0)           // connection fail
	{
		act_structptr->resultstatus = check_value_TXT_cmp(cErrorInfo,"F");   // immer fail
		return(0);        // look-in failure
	}
	else if (rc > 0 )     // CMD60 IO-error
	{
		return(rc);       // IO-error(0x02f0 - 0x02ff);
	}
	act_structptr->resultstatus = check_value(0);
	return(0);
}

TESTFUNCTION PowerRF_J33(struct struct_test *act_structptr)
{
int rfpart;
double dNTP;

// if RFP with connector-board SMA, we have to switch..
	if((!strncmp(gSerialNr_DUT,"3C1",3)) || (!strncmp(gSerialNr_DUT,"3BZ",3)))
	{ //RFP47DRC_3C1 and 3BZ
// set the correct antenna on SMA-connector..
		if(MiniSys_set_OC(6, 0)) return(0x1026); // OC6 for selection   Nails, 0 == left, 1=right
	}

//	if(MiniSys_set_OC(6,0)) return(0x1000);

	if(ipRfp_rsx_SetDiversity(0x01)) return(0x0525);	  //sollte J33 sein
	
// loop back data type : 00001111
	if(CMD60_ConfModDataType (gpib_handle.cmd60, FIG2)) return(0x021a);
// set Bursts for NTP and MAX/MIN modulation (1..12000 default=4)
	if(CMD60_ConfAverBurs  (gpib_handle.cmd60, 4)) return(0x021b);
//set Bursts for Timing  (2..2000  default=32)
	if(CMD60_ConfTimAverBurs (gpib_handle.cmd60, 32)) return(0x021c);
//Sleep(200);  // 200 ms 
	if (CMD60_ReadNtp (gpib_handle.cmd60, &dNTP))
	{
// fail NaN
		dNTP = -200;                              // on error -200  
	}
	else
	{
		//if((strncmp(gSerialNr_DUT,"3C1",3)) || (strncmp(gSerialNr_DUT,"3BZ",3)))   // add offset, if no SMA-connector (conducted measurement)
		   if(!strncmp(gSerialNr_DUT,"3BY",3) || !strncmp(gSerialNr_DUT,"3BX",3) || !strncmp(gSerialNr_DUT,"3HG",3) ||!strncmp(gSerialNr_DUT,"3E7",3) ||!strncmp(gSerialNr_DUT,"3L9",3)|| !strncmp(gSerialNr_DUT,"3C2",3))// add offset, if no SMA-connector (conducted measurement) 
			dNTP += act_structptr->doublepara[0];
	}	
	act_structptr->resultstatus = check_value(dNTP);
	return(0);		
}


TESTFUNCTION PowerRF_J32(struct struct_test *act_structptr)
{
int rfpart;
double dNTP;

// if RFP with connector-board SMA, we have to switch..
	if((!strncmp(gSerialNr_DUT,"3C1",3)) || (!strncmp(gSerialNr_DUT,"3BZ",3)))
	{ //RFP47DRC_3C1 and 3BZ
// set the correct antenna on SMA-connector..
		if(MiniSys_set_OC(6, 1)) return(0x1026); // OC6 for selection   Nails, 0 == left, 1=right
	}

	if(ipRfp_rsx_SetDiversity(0x02)) return(0x0525);	  //sollte J32 sein
// loop back data type : 00001111
	if(CMD60_ConfModDataType (gpib_handle.cmd60, FIG2)) return(0x021a);
// set Bursts for NTP and MAX/MIN modulation (1..12000 default=4)
	if(CMD60_ConfAverBurs  (gpib_handle.cmd60, 4)) return(0x021b);
//set Bursts for Timing  (2..2000  default=32)
	if(CMD60_ConfTimAverBurs (gpib_handle.cmd60, 32)) return(0x021c);
//Sleep(200);  // 200 ms 
	if (CMD60_ReadNtp (gpib_handle.cmd60, &dNTP))
	{
// fail NaN
		dNTP = -200;                              // on error -200  
	}
	else
	{	
		//if((strncmp(gSerialNr_DUT,"3C1",3)) || (strncmp(gSerialNr_DUT,"3BZ",3)))   // add offset, if no SMA-connector (conducted measurement)
				if(!strncmp(gSerialNr_DUT,"3BY",3) || !strncmp(gSerialNr_DUT,"3BX",3) ||!strncmp(gSerialNr_DUT,"3HG",3) || !strncmp(gSerialNr_DUT,"3E7",3) || !strncmp(gSerialNr_DUT,"3L9",3)|| !strncmp(gSerialNr_DUT,"3C2",3))// add offset, if no SMA-connector (conducted measurement)
			dNTP += act_structptr->doublepara[0];
	}	
	act_structptr->resultstatus = check_value(dNTP);
	return(0);		
}




TESTFUNCTION PowerRF_J33New (struct struct_test *act_structptr)			 //RFP_CMD60_connect + Power
{
char   szRFPI[20]  = "1000000000";
int    i;
int    rc;
char   cInfo[256];
char   cErrorInfo[256];
double dNTP;
double dHfOffset;     
double dRfLevel; 
double dCarrierOffset;
int    iFrequencyBand;
int    iPowerLevel;
int rfpart,radio;	

/*
#define RFP44_3BY 50006973
#define RFP45_3BX 50006972
#define RFP47_3BZ 50006974
#define RFP47DRC_3C1 50006975
#define RFP48_3C2 50006976
*/


	if((!strncmp(gSerialNr_DUT,"3C1",3)) || (!strncmp(gSerialNr_DUT,"3BZ",3)))
	{ //RFP47DRC_3C1 and 3BZ
// switch to connector board with SMA
		if(MiniSys_set_OC(8, 1)) return(0x1026); // OC8 for selection  radiated or Nails, 0 == radiated
	}
	else
	{
// check part type and switch the DECT-connection
// set internal Antenna Relais , if we use connector board with SMA, switch on !
		if(MiniSys_set_OC(8, 0)) return(0x1026); // OC8 for selection  radiated or Nails, 0 == radiated
	}
//assign HF-offset, traficcarrier and -slot dependent from radio
	dHfOffset = act_structptr->doublepara[1];
	iTrafficslot = 6;		 // iTrafficslot gerade = gerades Radio (0)
	iTrafficcarrier = 0;
	dCarrierOffset = 0;
	radio=0;                 
	(void)CMD60_Reset(gpib_handle.cmd60);
//CMD-setup:	
// Set Fixed Part 
	if(CMD60_ConfTest(gpib_handle.cmd60, FP)) return(0x020c);
// new setup for radiated measurement:
// set the correct antenna on SMA-connector: select J33 for diversity and CMD60 set antenna 0
	if(MiniSys_set_OC(6, 0)) return(0x1026); // OC6 for selection   Nails, 0 == left, 1=right
//set Antenna   0 or 1	
	if(CMD60_ConfFpAnt(gpib_handle.cmd60, 0)) return(0x0214);   // default after reset is 0
	if(ipRfp_rsx_SetDiversity(0x01)) return(0x0525);	  //sollte J33 sein
// Loopback Mode	
	if(CMD60_ConfFpSignMode(gpib_handle.cmd60, LOOPBACK)) return(0x020d);
//set packet type PP32Z (prolonged preamble)
	if( CMD60_addon_SetPacketTypePP32Z(gpib_handle.cmd60) != 0 ) return(0x023e);
// RF-Level:
	dRfLevel = -45.0 + dHfOffset;
	if(dRfLevel > -40)	dRfLevel = -40;
	if(CMD60_ConfFpRfLev(gpib_handle.cmd60, dRfLevel)) return(0x020e);
//Traficslot + -carrier
	if(CMD60_ConfFpChannels(gpib_handle.cmd60, iTrafficslot, iTrafficcarrier, dCarrierOffset)) return(0x020f);
// loop back data type : 00001111 
	if(CMD60_ConfModDataType(gpib_handle.cmd60,FIG2)) return(0x0210);
// set Bursts for NTP and MAX/MIN modulation (1..12000 default=4)
	if(CMD60_ConfAverBurs  (gpib_handle.cmd60, 4)) return(0x0211);
//set Bursts for Timing  (2..2000  default=32)
	if(CMD60_ConfTimAverBurs (gpib_handle.cmd60, 32)) return(0x0212);
//set packet type PP32Z (prolonged preamble)
	if( CMD60_addon_SetPacketTypePP32Z(gpib_handle.cmd60) != 0 ) return(0x0213); //[@JM080918]
//set "Radio Fixed Part Identity"
	if((CMD60_ConfFpRfpi(gpib_handle.cmd60, szRFPI)) != 0 ) return(0x0215);
// Manual Mode 
	if(CMD60_ProcSelTest(gpib_handle.cmd60, MANUAL)) return(0x0216);
// setup connection 
	rc = CMD60_addon_Connect(gpib_handle.cmd60, /*function*/0, cErrorInfo);
	if (rc < 0)           // connection fail
	{
		act_structptr->resultstatus = check_value_TXT_cmp(cErrorInfo,"F");   // immer fail
		return(0);        // look-in failure
	}
	else if (rc > 0 )     // CMD60 IO-error
	{
		return(rc);       // IO-error(0x02f0 - 0x02ff);
	}
	Sleep(200);  // 200 ms 
	if (CMD60_ReadNtp (gpib_handle.cmd60, &dNTP))
	{
// fail NaN
		dNTP = -200;                              // on error -200  
	}
	else
	{
		//if((strncmp(gSerialNr_DUT,"3C1",3)) || (strncmp(gSerialNr_DUT,"3BZ",3)))   // add offset, if no SMA-connector (conducted measurement)
		   if(!strncmp(gSerialNr_DUT,"3BY",3) || !strncmp(gSerialNr_DUT,"3BX",3) ||!strncmp(gSerialNr_DUT,"3HG",3) || !strncmp(gSerialNr_DUT,"3E7",3)  || !strncmp(gSerialNr_DUT,"3L9",3)  || !strncmp(gSerialNr_DUT,"3C2",3))// add offset, if no SMA-connector (conducted measurement) 
			dNTP += act_structptr->doublepara[0];
	}	
	act_structptr->resultstatus = check_value(dNTP);
	return(0);		
}


TESTFUNCTION PowerRF_J32New (struct struct_test *act_structptr)			 //RFP_CMD60_connect + Power
{
char   szRFPI[20]  = "1000000000";
int    i;
int    rc;
char   cInfo[256];
char   cErrorInfo[256];
double dNTP;
double dHfOffset;     
double dRfLevel; 
double dCarrierOffset;
int    iFrequencyBand;
int    iPowerLevel;
int rfpart,radio;	

/*
#define RFP44_3BY 50006973
#define RFP45_3BX 50006972
#define RFP47_3BZ 50006974
#define RFP47DRC_3C1 50006975
#define RFP48_3C2 50006976
*/


	if((!strncmp(gSerialNr_DUT,"3C1",3)) || (!strncmp(gSerialNr_DUT,"3BZ",3)))
	{ //RFP47DRC_3C1 and 3BZ
// switch to connector board with SMA
		if(MiniSys_set_OC(8, 1)) return(0x1026); // OC8 for selection  radiated or Nails, 0 == radiated
	}
	else
	{
// check part type and switch the DECT-connection
// set internal Antenna Relais , if we use connector board with SMA, switch on !
		if(MiniSys_set_OC(8, 0)) return(0x1026); // OC8 for selection  radiated or Nails, 0 == radiated
	}
//assign HF-offset, traficcarrier and -slot dependent from radio
	dHfOffset = act_structptr->doublepara[1];
	iTrafficslot = 6;		 // iTrafficslot gerade = gerades Radio (0)
	iTrafficcarrier = 0;
	dCarrierOffset = 0;
	radio=0;                 
	(void)CMD60_Reset(gpib_handle.cmd60);
//CMD-setup:	
// Set Fixed Part 
	if(CMD60_ConfTest(gpib_handle.cmd60, FP)) return(0x020c);
// set the correct antenna on SMA-connector: select J32 for diversity and CMD60 set antenna 1
	if(MiniSys_set_OC(6, 1)) return(0x1026); // OC6 for selection   Nails, 0 == left, 1=right
// !!!! 10180905: do not work, the RFP makes every time connect via J33 :((, it works, there is enougth power (diversity switch: about 23..27 dBm), hmmm ...		
	if(CMD60_ConfFpAnt(gpib_handle.cmd60, 1)) return(0x0214);   // default after reset is 0, set to 1
	if(ipRfp_rsx_SetDiversity(0x02)) return(0x0525);	  //sollte J32 sein
// Loopback Mode	
	if(CMD60_ConfFpSignMode(gpib_handle.cmd60, LOOPBACK)) return(0x020d);
//set packet type PP32Z (prolonged preamble)
	if( CMD60_addon_SetPacketTypePP32Z(gpib_handle.cmd60) != 0 ) return(0x023e);
// RF-Level:
	dRfLevel = -45.0 + dHfOffset;
	if(dRfLevel > -40)	dRfLevel = -40;
	if(CMD60_ConfFpRfLev(gpib_handle.cmd60, dRfLevel)) return(0x020e);
//Traficslot + -carrier
	if(CMD60_ConfFpChannels(gpib_handle.cmd60, iTrafficslot, iTrafficcarrier, dCarrierOffset)) return(0x020f);
// loop back data type : 00001111 
	if(CMD60_ConfModDataType(gpib_handle.cmd60,FIG2)) return(0x0210);
// set Bursts for NTP and MAX/MIN modulation (1..12000 default=4)
	if(CMD60_ConfAverBurs  (gpib_handle.cmd60, 4)) return(0x0211);
//set Bursts for Timing  (2..2000  default=32)
	if(CMD60_ConfTimAverBurs (gpib_handle.cmd60, 32)) return(0x0212);
//set packet type PP32Z (prolonged preamble)
	if( CMD60_addon_SetPacketTypePP32Z(gpib_handle.cmd60) != 0 ) return(0x0213); //[@JM080918]
//set "Radio Fixed Part Identity"
	if((CMD60_ConfFpRfpi(gpib_handle.cmd60, szRFPI)) != 0 ) return(0x0215);
// Manual Mode 
	if(CMD60_ProcSelTest(gpib_handle.cmd60, MANUAL)) return(0x0216);
// setup connection 
	rc = CMD60_addon_Connect(gpib_handle.cmd60, /*function*/0, cErrorInfo);
	if (rc < 0)           // connection fail
	{
		act_structptr->resultstatus = check_value_TXT_cmp(cErrorInfo,"F");   // immer fail
		return(0);        // look-in failure
	}
	else if (rc > 0 )     // CMD60 IO-error
	{
		return(rc);       // IO-error(0x02f0 - 0x02ff);
	}
	Sleep(200);  // 200 ms 
	if (CMD60_ReadNtp (gpib_handle.cmd60, &dNTP))
	{
// fail NaN
		dNTP = -200;                              // on error -200  
	}
	else
	{
		//if((strncmp(gSerialNr_DUT,"3C1",3)) || (strncmp(gSerialNr_DUT,"3BZ",3)))   // add offset, if no SMA-connector (conducted measurement)
		   if(!strncmp(gSerialNr_DUT,"3BY",3) || !strncmp(gSerialNr_DUT,"3BX",3)||!strncmp(gSerialNr_DUT,"3HG",3)|| !strncmp(gSerialNr_DUT,"3E7",3) || !strncmp(gSerialNr_DUT,"3L9",3)  || !strncmp(gSerialNr_DUT,"3C2",3))// add offset, if no SMA-connector (conducted measurement) 
			dNTP += act_structptr->doublepara[0];
	}	
	act_structptr->resultstatus = check_value(dNTP);
	return(0);
}





TESTFUNCTION WLanTxPower_Ant1(struct struct_test *act_structptr) 
{
int i;

	if(strncmp(gSerialNr_DUT,"3C2",3)) // test for WLAN-Type only
	{ // no match, discard test
		setPassAndLog("nd"); 
		act_structptr->resultstatus = 0;
		return(0);
	}

//set to error:	
	gWLANAnt1 = -100;
	gWLANAnt2 = -100;
	gWLANAnt3 = -100;


 	set_hintwindow("init WLAN, needs 8..12s",COL_NEUTRAL,(testflow->running_left ? LEFT : RIGHT));	  // LEFT, RIGHT     
	if ( ipRfp_libssh_WriteAndRead( "bit\r", gResponseLocal, sizeof(gResponseLocal), "WLAN MODULE                              FOUND", 1.0 ) != 0 ) return(-1); 
// need time for this: long timeout !!	
	if ( ipRfp_libssh_WriteAndRead( "fmanu wlan 2.4 g on\r", gResponseLocal, sizeof(gResponseLocal), "ssid rfp4gManu", 12.0 ) != 0 ) return(-2);       
	i = WLanEmbedded_ListAP(&gWLANAnt1, &gWLANAnt2, &gWLANAnt3);
	
	act_structptr->resultstatus = check_value(gWLANAnt1);
	return(0);
}


TESTFUNCTION WLanTxPower_Ant3(struct struct_test *act_structptr) 
{
	if(strncmp(gSerialNr_DUT,"3C2",3)) // test for WLAN-Type only
	{ // no match, discard test
		setPassAndLog("nd"); 
		act_structptr->resultstatus = 0;
		return(0);
	}
	act_structptr->resultstatus = check_value(gWLANAnt3);
	return(0);
}

TESTFUNCTION WLanTxPower_Ant2(struct struct_test *act_structptr) 
{
	if(strncmp(gSerialNr_DUT,"3C2",3)) // test for WLAN-Type only
	{ // no match, discard test
		setPassAndLog("nd"); 
		act_structptr->resultstatus = 0;
		return(0);
	}
	act_structptr->resultstatus = check_value(gWLANAnt2);
	return(0);
}


TESTFUNCTION FactoryDeliveryState(struct struct_test *act_structptr)
{
//After finishing all tests please send below command
//Comand: fmanu factorycleanup 			 
 	set_hintwindow("FactoryCleanup",COL_NEUTRAL,(testflow->running_left ? LEFT : RIGHT));	  // LEFT, RIGHT     
//this command kills the ssh-connection
	if ( ipRfp_libssh_WriteAndRead( "fmanu factorycleanup\r", gResponseLocal, sizeof(gResponseLocal), "#", 3.0 ) != 0 ) return(-1);       
	Sleep(3000);  // 3Sec     see testspec  
	set_hintwindow("Done",COL_NEUTRAL,(testflow->running_left ? LEFT : RIGHT));	  // LEFT, RIGHT     
	act_structptr->resultstatus = check_value(0);     
	return(0);   //ok
}

TESTFUNCTION CheckSoftwareVersion(struct struct_test *act_structptr)
{
//After finishing all tests please send below command
//Comand: fmanu factorycleanup 			 
 	set_hintwindow("CheckSoftwareVersion",COL_NEUTRAL,(testflow->running_left ? LEFT : RIGHT));	  // LEFT, RIGHT     
//this command kills the ssh-connection
	if ( ipRfp_libssh_WriteAndRead( "fmanu version\r", gResponseLocal, sizeof(gResponseLocal), "#", 3.0 ) != 0 ) return(-1);
	Sleep(2000);  // 3Sec     see testspec    
	if(0 == strncmp(gSerialNr_DUT,"3E7",3)) // test for WLAN-Type only
	{ // no match, discard test 
		if(FindPattern (gResponseLocal, 0, -1, "SIP-DECT 8.2SP1-FK13", 0, 0) == -1)
		{
			act_structptr->resultstatus = check_value(1);
		}
		else
		{
			act_structptr->resultstatus = check_value(0);
		}	
	}

	else if(0 == strncmp(gSerialNr_DUT,"3L9",3)) // egpty
	{
		// no match, discard test
		if(FindPattern (gResponseLocal, 0, -1, "SIP-DECT 8.3SP1-GI15", 0, 0) == -1)
		{
			act_structptr->resultstatus = check_value(1);
		}
		else
		{
			act_structptr->resultstatus = check_value(0);
		}
	}
	else
	{
		if(FindPattern (gResponseLocal, 0, -1, "SIP-DECT 8.0-HF06DI16", 0, 0) == -1)
		{
			act_structptr->resultstatus = check_value(1);
		}
		else
		{
			act_structptr->resultstatus = check_value(0);
		}	
	
	}
	
	

	set_hintwindow("Check Finish",COL_NEUTRAL,(testflow->running_left ? LEFT : RIGHT));	  // LEFT, RIGHT     
    
	return(0);   //ok
}

TESTFUNCTION CheckBootFlashImage(struct struct_test *act_structptr)
{
//After finishing all tests please send below command
//Comand: fmanu factorycleanup 			 
 	set_hintwindow("CheckBootFlashImage",COL_NEUTRAL,(testflow->running_left ? LEFT : RIGHT));	  // LEFT, RIGHT     
//this command kills the ssh-connection
	if ( ipRfp_libssh_WriteAndRead( "cat /proc/cmdline\r", gResponseLocal, sizeof(gResponseLocal), "#", 3.0 ) != 0 ) return(-1);
	
	if(strstr(gResponseLocal,"RFP4G_UBootVer=0.5") == NULL) // NOT FIND CORRECT BOOT IMAGE
	{ // no match, discard test 
	
		act_structptr->resultstatus = check_value(1);
	
	}
	else
	{
	
		act_structptr->resultstatus = check_value(0);
	
	
	}
	
	

	set_hintwindow("Check Finish",COL_NEUTRAL,(testflow->running_left ? LEFT : RIGHT));	  // LEFT, RIGHT     
    
	return(0);   //ok
}



TESTFUNCTION SetCheckCountryTag(struct struct_test *act_structptr)
{
	if(0 == strncmp(gSerialNr_Scanned,"3L9",3)) // orange only
	{
		set_hintwindow("Program country tag",COL_NEUTRAL,(testflow->running_left ? LEFT : RIGHT));	  // LEFT, RIGHT

		if ( ipRfp_libssh_WriteAndRead( "echo Egypt > /var/opt/ip_rfp/otp_country_tag\r", gResponseLocal, sizeof(gResponseLocal), "#", 3.0 ) != 0 ) return(-1);

		print_resultdisplay(gResponseLocal);

		if ( ipRfp_libssh_WriteAndRead( "cat /var/opt/ip_rfp/otp_country_tag\r", gResponseLocal, sizeof(gResponseLocal), "#", 3.0 ) != 0 ) return(-1);
		print_resultdisplay(gResponseLocal);
		if(strstr(gResponseLocal,"Egypt") == NULL) // NOT FIND CORRECT COUNTRY TAG
		{
			// no match, discard test

			act_structptr->resultstatus = check_value(1);

		}

		set_hintwindow("Program & Check Finish",COL_NEUTRAL,(testflow->running_left ? LEFT : RIGHT));	  // LEFT, RIGHT
		act_structptr->resultstatus = check_value(0);
	}
	else
	{
		print_resultdisplay("Not Egypt models, don't need test this step");
		set_hintwindow("Not Egypt models, don't need test this step",COL_NEUTRAL,(testflow->running_left ? LEFT : RIGHT));	  // LEFT, RIGHT
		act_structptr->resultstatus = check_value(0);
	}


	
	return(0);   //ok
}

TESTFUNCTION Disconnect_V2(struct struct_test *act_structptr)
{
char cInfo[256];

	iBarCodeRight = 0;	   // undefine Mac & serial
	if(MiniSys_set_OC(5, 0))   // switch power off via opto-switch
		return(0x1001); //1..8:0|1    
 	set_hintwindow("Power off",COL_NEUTRAL,(testflow->running_left ? LEFT : RIGHT));	  // LEFT, RIGHT     

	(void)ipRfp_rsx_Close();
	(void)ipRfp_libssh_Close();
	
	strIP_Address[0] = '\0';
// 20180905, not realy need, but RFP47 have sometimes trouble with OC-control
   MiniSys_write ("RST");
	
	act_structptr->resultstatus = check_value(0);
	return(0);
}

