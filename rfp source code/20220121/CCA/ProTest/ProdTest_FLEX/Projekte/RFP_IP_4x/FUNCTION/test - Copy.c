
#include "test.h"
#include "MiniSystem.h"

#define RFP35_DEBUG_ProgramFirmware  // copy all Logfiles for Debug only  @GC 25.06.2014 

#define RFP35_CHECKRTC_WITHOUT_NTP   // ab Firmware SIP-DECT 6.0SP2-BA11 NTP nicht mehr abfragen @GC 02.02.2016


#undef RFP35_DEBUG_BITER_LOG         // Logging fuer BER Test
//#define RFP35_DEBUG_BITER_LOG 1         // Logging fuer BER Test
#define RFP35_DEBUG_BMCCLOCK_LOG 2        // Logging fuer Adjust BMCCLOCK Test          

unsigned char gDutTxData[0x10000];  
extern int Print2StandardIO;
int InternalTemp, ExternalTemp;


/***********************************************************************************************/
void changeToPPM(double *dBer, double *dFer)
{
  *dBer = (double)((int) (*dBer * 1e6));          // 0.001 * 1000000 = 1000         in ppm umwandeln
  *dFer = (double)((int) ((1e6/100) * *dFer));    // 1000000 / 100 * 1% = 10000     in ppm umwandeln
}



int DEBUG_COPY_LOGFILE(struct struct_test *act_structptr, char *cInfo)  
{
char tempA[80];
char tempB[80];
char cDirName[256];
char cBatchName[256];
FILE *fp;  
// zusatz Info Speichern
		remove ("c:\\temp\\info.log");
		if ( cInfo != NULL )
		{	
			fp=fopen("c:\\temp\\info.log", "w");
			fprintf(fp,"%s",cInfo); 
			fclose(fp);
		}	
// Verzeichniss Namen erzeugen
// fail_datum_exid_testname
		strcpy(tempA, DateStr() );
		strcpy(tempB, tempA );
// change format 	MM-TT-JJJJ to JJJJ-MM-TT
//					0123456789 to 0123456789
		tempA[0] = tempB[6];
		tempA[1] = tempB[7];
		tempA[2] = tempB[8];
		tempA[3] = tempB[9];
		tempA[4] = '-';
		tempA[5] = tempB[0];
		tempA[6] = tempB[1];
		tempA[7] = '-';
		tempA[8] = tempB[3];
		tempA[9] = tempB[4];
		tempA[10] = '\0';
// JJ:MM:SS
		strcpy(tempB,TimeStr());
		tempB[0] = tempB[0];
		tempB[1] = tempB[1];
		tempB[2] = tempB[3];
		tempB[3] = tempB[4];
		tempB[4] = tempB[6];
		tempB[5] = tempB[7];
		tempB[6] = '\0';
//    fail_jjjj-mm-tt-hhmmss_exid_testname
//		sprintf(cDirName,"G:\\Protest\\tmp\\fail_funct_%s-%s_%s_%s",tempA,tempB,structQPDS.strExID,act_structptr->username);
		sprintf(cDirName,"G:\\Protest\\tmp\\fail_funct_%s-%s_%s_%s",tempA,tempB,DutParameter.strExID,act_structptr->username);
		// Batch Datei Erzeugen
		sprintf(cBatchName,"c:\\temp\\debug_copy_logs.cmd");
		fp=fopen(cBatchName, "w");
		
		//if not exist g:\Protest\tmp\fail_xxxx  mkdir g:\Protest\tmp\fail_xxxx
		fprintf(fp,"if not exist %s mkdir %s\n",cDirName,cDirName);
		
		fprintf(fp,"copy /y /b c:\\temp\\rs232.log %s\n",cDirName);
		fprintf(fp,"copy /y /b c:\\temp\\rsx.log %s\n",cDirName);
		fprintf(fp,"copy /y /b c:\\temp\\ssh.log %s\n",cDirName);
		fprintf(fp,"copy /y /b c:\\temp\\plink_*.log %s\n",cDirName);
		fprintf(fp,"copy /y /b c:\\temp\\pscp_*.log %s\n",cDirName);
		fprintf(fp,"copy /y /b c:\\temp\\openocd_*.log %s\n",cDirName);
		fprintf(fp,"copy /y /b c:\\temp\\info.log %s\n",cDirName);
		fclose(fp);
		system(cBatchName);
	return(0);	
}

	
/***********************************************************************************************
**
**		Main Testfunctionen
**
**
***********************************************************************************************/
TESTFUNCTION PowerOn(struct struct_test *act_structptr)						// @GC18.09.2012  
{
int 	retry;
int 	rc;
int     iMode;
char cInfo[256];
// set Boot-Mode via TFTP:
	if(MiniSys_set_OC(5, 1))   // switch boot mode to TFTP (connect Boot-Pin with 1,8V)
		return(0x1001); //1..8:0|1    
//LAN connection ist connected, only the power is switched
	if(MiniSys_set_OC(4, 1))   // switch power on via opto-switch
		return(0x1001); //1..8:0|1    
 	set_hintwindow("Power on",COL_NEUTRAL,(testflow->running_left ? LEFT : RIGHT));	  // LEFT, RIGHT     
   	time(&gl_timePowerOn);                 // Zeitstempel PowerOn fuer Test CheckRTC
// old ..
	CMD60_Reset (gpib_handle.cmd60);
	
	Sleep(1000);
	set_testheader(0);
	strcpy(strIP_Address,"0.0.0.0");        		// global Variable used for all Test
	ipRfp_clearTempFiles();                         // clear all temporary files
	act_structptr->resultstatus = check_value(0);
	time(&gl_timePowerOn);          // Zeitstempel PowerOn
	return(0);
}



TESTFUNCTION PowerOnCheckTFTP(struct struct_test *act_structptr)						// @GC18.09.2012  
{
int 	i,retry, count;
int answerCount = 0;
int 	rc;
int     iMode;
char cInfo[256];
char debugStr[0x100];
// set Boot-Mode via TFTP:
	if(MiniSys_set_OC(5, 1))   // switch boot mode to TFTP (connect Boot-Pin with 1,8V)
		return(0x1001); //1..8:0|1    

/* 
// power on in startup-function (system.c), the switch is every time powered on
//LAN connection ist connected, only the power is switched
	if(MiniSys_set_OC(4, 1))   // switch power on via opto-switch
		return(0x1001); //1..8:0|1    
*/		
 	set_hintwindow("Power on",COL_NEUTRAL,(testflow->running_left ? LEFT : RIGHT));	  // LEFT, RIGHT     
   	time(&gl_timePowerOn);                 // Zeitstempel PowerOn fuer Test CheckRTC
// old ..
	CMD60_Reset (gpib_handle.cmd60);
	
	Sleep(1000);

	set_testheader(0);
	strcpy(strIP_Address,"0.0.0.0");        		// global Variable used for all Test
	ipRfp_clearTempFiles();                         // clear all temporary files
	act_structptr->resultstatus = check_value(0);
	time(&gl_timePowerOn);          // Zeitstempel PowerOn
	Print2StandardIO = 1;
	i = ipRfp_rs232_WaitForBootUp(12, "Load address: 0x1000000",NULL,NULL,NULL,NULL);  //timeOut in Sec
	i = ipRfp_rs232_WaitForBootUp(12, "###############",NULL,NULL,NULL,NULL);  //timeOut in Sec
//	Print2StandardIO = 0;
	return(0);
}




TESTFUNCTION SupplyCurrentPSU(struct struct_test *act_structptr)	// @GC18.09.2012  
{
 // measure via 1 Ohm shunt resistor 
double current;
double offset;  // current comsumption by switch


	offset = act_structptr->doublepara[0];

	
	current = -1;
// setup ADC:
	if(MiniSys_ADC_RaSe(MINISYS_ADC_RANGE_DC, 2)) return (0x1010);  
	if(MiniSys_ADC_InpS(8, 16)) return (0x1011); 
	if(MiniSys_ADC_getVdc(&current)) 
	{
		Sleep(200);
		if(MiniSys_ADC_getVdc(&current)) 
		{
			Sleep(200);
			if(MiniSys_ADC_getVdc(&current)) 
				return (0x1012);
		}
	}
	current *= 1000.0;
	act_structptr->resultstatus = check_value(current-offset);
	return(0);			   
}
	


TESTFUNCTION LogSerialNumber(struct struct_test *act_structptr)
{

	act_structptr->resultstatus = check_value_str(strSerial);	   
	return(0);                                   
}                                            


TESTFUNCTION LogMAC(struct struct_test *act_structptr)
{
	act_structptr->resultstatus = check_value_str(strMAC);	  
	return(0);                                   
}                                            


TESTFUNCTION Clock_25MHz(struct struct_test *act_structptr)
{
 /*
TP315   				CLK 25MHz     50ppm
Pneumatik Nadel OC5/PN3  Nr2
*/
int retry;
double dResultValue;
double dResultFreq;
double dFreqSoll = 25000000;  // Hz 
double dFreqLower;
double dFreqUpper;		

//char cInfo[256];


	dFreqLower = dFreqSoll + ((dFreqSoll / 1e6) * act_structptr->lowerlimit); 	// -50ppm	
	dFreqUpper = dFreqSoll + ((dFreqSoll / 1e6) * act_structptr->upperlimit); 	// +50ppm	
// set OC8, Relay for 25MHz
	if(MiniSys_set_OC(8,1)) return(0x1000);
	Sleep(100);
	if((CMD60_ConfAfmForm (gpib_handle.cmd60Cnt, LT30Mhz, RMS)) != 0 ) return(0x0200);
	for ( retry=0 ; retry < 3 ; retry++ )
	{
		if(CMD60_ReadAfmCoun (gpib_handle.cmd60Cnt, &dResultFreq))
		{
			dResultFreq=-1;
			continue;
		}    
		if (  dResultFreq >= dFreqLower && dResultFreq <= dFreqUpper )
		    break;
		
	}
	if ( dResultFreq == 0 ) dResultFreq = 1;  // 0 kann nicht geteilt werden
	dResultValue = (dResultFreq/dFreqSoll - 1) * 1e6;  // compute to ppm
// clear OC8
	if(MiniSys_set_OC(8,0)) return(0x1001);
	act_structptr->resultstatus = check_value(dResultValue);
	return(0);
} 




TESTFUNCTION RS232_CheckLinkStatus(struct struct_test *act_structptr)  
{
char temp_str[256];
char *pEthDev;
char *pExpected;
char linkStatus[256];
int  ret;
int retry;
double current;


	ret = ipRfp_rs232_WaitForBootUp(12, "Please press Enter to activate this console",NULL,NULL,NULL,NULL);  //timeOut in Sec
	pEthDev = act_structptr->string[0];            // "eth0"
	pExpected = act_structptr->string[1];          // eth0: "link up, 1000 Mb's, full duplex"
	if(!strncmp(strSerial,"3C2",3))
	{ //RFP48_3C2, only WLAN have 1000MBiits
		pExpected = act_structptr->string[2];           // eth0: link up, 100 Mb's, full duplex
	}

// wait for LinkUp
// Link Status abfragen 		
// eth0 
	for ( retry=20; retry > 0  ; retry-- )
	{
		ret=ipRfp_rs232_getLinkStatus(pEthDev, linkStatus);
		if ( ret == 0 &&  strncmp(pExpected,linkStatus,7) == 0 )  // compare only "link up"
		    break;
		Sleep(200);    // 200ms
	}	
	if ( ret != 0 )
	{
    	return(0x0502);    		
    }	
	sprintf(temp_str,"%s %s", pEthDev, linkStatus);	
 	set_hintwindow( temp_str ,COL_NEUTRAL,(testflow->running_left ? LEFT : RIGHT));	  // LEFT, RIGHT     
// link up 1000M
	act_structptr->resultstatus = check_value_str_cmp(linkStatus,pExpected);
	return(0);
	
}



TESTFUNCTION RS232_getIpAddr(struct struct_test *act_structptr)  
{
int  ret,count;
char *pExpIpAddrMin;
char *pExpIpAddrMax;
long ipMin[4]={0,0,0,0};
long ipMax[4]={0,0,0,0};
long ip[4]   ={0,0,0,0};
char temp_str[256];

	pExpIpAddrMin = act_structptr->string[0];          // expected ipaddr Minimal
	pExpIpAddrMax = act_structptr->string[1];          // expected ipaddr Maximal
// ip Addr über rs232 auslesen und in die globale Variable schreiben
// 100MBit need more time ...
	count = 4;
	do
	{
		if ( ipRfp_rs232_getIpAddress(strIP_Address)) return(0x0503);
 		set_hintwindow( strIP_Address ,COL_NEUTRAL,(testflow->running_left ? LEFT : RIGHT));	  // LEFT, RIGHT     
 		if(strcmp(strIP_Address,pExpIpAddrMin))
 			Sleep(2000);
 		else break;
 	}while(count--);
 	
// in Long Int packen    
	sscanf(pExpIpAddrMin,"%d.%d.%d.%d",&ipMin[3],&ipMin[2],&ipMin[1],&ipMin[0]);
	ipMin[0] = ((ipMin[3]&0xff)<<24) + ((ipMin[2]&0xff)<<16) + ((ipMin[1]&0xff)<<8) + ipMin[0];
	sscanf(pExpIpAddrMax,"%d.%d.%d.%d",&ipMax[3],&ipMax[2],&ipMax[1],&ipMax[0]);
	ipMax[0] = ((ipMax[3]&0xff)<<24) + ((ipMax[2]&0xff)<<16) + ((ipMax[1]&0xff)<<8) + ipMax[0];
	sscanf(strIP_Address,"%d.%d.%d.%d",&ip[3],&ip[2],&ip[1],&ip[0]);
	ip[0] = ((ip[3]&0xff)<<24) + ((ip[2]&0xff)<<16) + ((ip[1]&0xff)<<8) + ip[0];
//sprintf(temp_str,"ipMin=0x%08x < ip=0x%08x < ipMax=0x%08x",ipMin[0],ip[0],ipMax[0]);
//MessagePopup("status",temp_str);
// ipAddr < ipMin
	if ( ip[0] < ipMin[0] )
	{
		//error
		//sprintf(temp_str,"error_min = %s < %s",strIP_Address,pExpIpAddrMin);
		//MessagePopup("status",temp_str);
		act_structptr->resultstatus = check_value_str_cmp(strIP_Address,pExpIpAddrMin);
		return(0);
	}	
// ipAddr > ipMax
	if ( ip[0] > ipMax[0] )
	{
		//error
		//sprintf(temp_str,"error_max = %s > %s",strIP_Address,pExpIpAddrMax);
		//MessagePopup("status",temp_str);
		act_structptr->resultstatus = check_value_str_cmp(strIP_Address,pExpIpAddrMax);
		return(0);
	}
// ok
// dummy vergleich
	act_structptr->resultstatus = check_value_str_cmp(strIP_Address,strIP_Address);
	return(0);
}



TESTFUNCTION SSH_PortConnect(struct struct_test *act_structptr)
{
int retry;
char strResultValue[80];
// @GC 12.10.2012 fuer RFP SL35
// ab der FW SIP-DECT-Lite_3.1RC3 muss SSH Enabled werden
//
// dieses enable bleibt dann bis factoryreset erhalten.
// wichtig fuer Final Test
	if ( ipRfp_rs232_enableSSH() ) 
	{
		act_structptr->resultstatus = check_value(1);
		return(0);
	}
//MessagePopup("STOP", "----");
 	set_hintwindow("systemPing",COL_NEUTRAL,(testflow->running_left ? LEFT : RIGHT));	  // LEFT, RIGHT     
// 5*1000 = 5Sec.
	retry=5;	
	while ( ! ipRfp_SystemPing (strIP_Address,1000) );      // 1000 ms
	{	
		retry--;
		if(retry == 0)
		{
			act_structptr->resultstatus = check_value(2);
			return(0);
		}
	}	
 	set_hintwindow("SSH",COL_NEUTRAL,(testflow->running_left ? LEFT : RIGHT));	  // LEFT, RIGHT     
LIBSSH_OPEN:	
	// use libssh @GC 13.01.2012
	if ( ipRfp_libssh_Open(strIP_Address) ) 
	{
//MessagePopup("WAIT","Warte nach ssh open mit Fehler");
//if( ConfirmPopup("Retry", "libssh_Open() fails Retry?" )) goto LIBSSH_OPEN;
		act_structptr->resultstatus = check_value(3);
		return(0);
	}
// ipAddr Kommando 	
	if ( ipRfp_libssh_getIpAddress(strResultValue) )
	{
		act_structptr->resultstatus = check_value(4);
		return(0);
	}
 	set_hintwindow("",COL_NEUTRAL,(testflow->running_left ? LEFT : RIGHT));	  // LEFT, RIGHT     
//	ipAddr vergleichen
	if ( strcmp(strResultValue,strIP_Address) != 0 )
	{	
		act_structptr->resultstatus = check_value(5);
		return(0);
	}
	act_structptr->resultstatus = check_value(0);
	return(0);   
}






TESTFUNCTION CheckLinkStatus(struct struct_test *act_structptr)  
{
char *pEthDev;
char *pExpected;
char linkStatus[256];

	pEthDev = act_structptr->string[0];             // eth0
	pExpected = act_structptr->string[1];           // eth0: link up, 100 Mb's, full duplex
	if(!strncmp(strSerial,"3C2",3))
	{ //RFP48_3C2, only WLAN have 1000MBiits
		pExpected = act_structptr->string[2];           // eth0: link up, 100 Mb's, full duplex
	}
	
	// Link Status abfragen 
	// eth0 ist extern Gigabit
	// eth1 ist intern 100mbit
	if ( ipRfp_libssh_getLinkStatus(pEthDev, linkStatus) )  return(0x050f);    		
	act_structptr->resultstatus = check_value_str_cmp(linkStatus,pExpected);
	return(0);
}



TESTFUNCTION CheckBMCStatus(struct struct_test *act_structptr)
{
int  retry;	
char bmcStatus[80];  
char bmcTestmode[80];  
//wait 10Sec 
	for ( retry=10 ; retry > 0 ; retry-- )
	{
		if ( ipRfp_libssh_getBMCStatus(bmcStatus,bmcTestmode,NULL) ) return(0x0510);
		if ( strcmp(bmcStatus,"RUNNING") == 0 )
			break;
		Sleep(1000); // 1 Sec
	}	
	// wenn BMC running, aber nicht im Testmode dann testmode on
	if ( strcmp(bmcStatus,"RUNNING") == 0 && strcmp(bmcTestmode,"(TESTMODE)") != 0)
	{	
		// fmanu testmode on
		if ( ipRfp_libssh_enableBMCTestmode() ) return(0x0510); 
		// etwa 6-10 sec auf RUNNING warten
		// BMC wird neu geladen
		// max wait 20 Sec
		for ( retry=20 ; retry > 0 ; retry-- )
		{
			if ( ipRfp_libssh_getBMCStatus(bmcStatus,bmcTestmode,NULL) ) return(0x0510);
			if ( strcmp(bmcStatus,"RUNNING") == 0 )
				break;
			Sleep(1000); // 1 Sec
		}	
	}
	act_structptr->resultstatus = check_value_str_cmp(bmcStatus,"RUNNING");
	return(0);   
}


TESTFUNCTION RS232_EnableRSX(struct struct_test *act_structptr)
{
int i;
int error;
int retry;
int ret;
char respBuffer[256];

	sw_stand[0]= '0';
	sw_stand[1]= '0';
	sw_stand[2]= '0';
	Sleep(3000);
 	set_hintwindow("enableRSX",COL_NEUTRAL,(testflow->running_left ? LEFT : RIGHT));	  // LEFT, RIGHT  
 	
//folgender Befehl nicht nötig, da rsx-tool auf dem Flachbaugruppentest automatisch enabled ist (nach TFTP...) 	
/*
	if ( ipRfp_rs232_enableRSX() != 0 )
	{	
		act_structptr->resultstatus = check_value(2);
		return(0);
	}
*/	
	Sleep(1000);   //500 ms need to start rsx deamon  //auf 1Sec erweitert 25.06.2014 @GC

 	set_hintwindow("openRSX",COL_NEUTRAL,(testflow->running_left ? LEFT : RIGHT));	  // LEFT, RIGHT     
	// open socket
	if ( ipRfp_rsx_Open(strIP_Address,38477))   // 10318   == 0x284e 
	{	
		act_structptr->resultstatus = check_value(3);   //fails if socket connection not properly closed before (reboot DUT)
		return(0);
	}
	
 	set_hintwindow("getSwVersion",COL_NEUTRAL,(testflow->running_left ? LEFT : RIGHT));	  // LEFT, RIGHT     
	if (  ipRfp_rsx_GetSoftwareVersion_Retry(respBuffer,5) == NULL )     // 5x Retry 
	{
		act_structptr->resultstatus = check_value(4);
		return(0);
	}	
	// hier nur auslesen und in globale Variable schreiben
	sw_stand[0]=respBuffer[0];
	sw_stand[1]=respBuffer[1];
	sw_stand[2]=respBuffer[2];
 	set_hintwindow("",COL_NEUTRAL,(testflow->running_left ? LEFT : RIGHT));	  // LEFT, RIGHT     
	act_structptr->resultstatus = check_value(5);
	return(0);
}




TESTFUNCTION IP_PortConnect(struct struct_test *act_structptr)
{
int i;
int error;
int retry;
int ret;
char respBuffer[256];

	sw_stand[0]= '0';
	sw_stand[1]= '0';
	sw_stand[2]= '0';
 	set_hintwindow("systemPing",COL_NEUTRAL,(testflow->running_left ? LEFT : RIGHT));	  // LEFT, RIGHT     
	// 5*1000 = 5Sec.
	retry=5;	
	while ( ! ipRfp_SystemPing (strIP_Address,1000) );      // 1000 ms
	{	
		retry--;
		if(retry == 0)
		{
			act_structptr->resultstatus = check_value(1);
			return(0);
		}
	}	
 	set_hintwindow("enableRSX",COL_NEUTRAL,(testflow->running_left ? LEFT : RIGHT));	  // LEFT, RIGHT     
	if ( ipRfp_rs232_enableRSX() != 0 )
	{	
		act_structptr->resultstatus = check_value(2);
		return(0);
	}
	Sleep(1000);   //500 ms need to start rsx deamon  //auf 1Sec erweitert 25.06.2014 @GC
 	set_hintwindow("openRSX",COL_NEUTRAL,(testflow->running_left ? LEFT : RIGHT));	  // LEFT, RIGHT     
	// open socket
	if ( ipRfp_rsx_Open(strIP_Address,38477) )
	{	
		act_structptr->resultstatus = check_value(3);
		return(0);
	}
 	set_hintwindow("getSwVersion",COL_NEUTRAL,(testflow->running_left ? LEFT : RIGHT));	  // LEFT, RIGHT     
	if (  ipRfp_rsx_GetSoftwareVersion_Retry(respBuffer,5) == NULL )     // 5x Retry 
	{
		act_structptr->resultstatus = check_value(4);
		return(0);
	}	
	// hier nur auslesen und in globale Variable schreiben
	sw_stand[0]=respBuffer[0];
	sw_stand[1]=respBuffer[1];
	sw_stand[2]=respBuffer[2];
 	set_hintwindow("",COL_NEUTRAL,(testflow->running_left ? LEFT : RIGHT));	  // LEFT, RIGHT     
	act_structptr->resultstatus = check_value(5);
	return(0);
}


TESTFUNCTION IP_EnterTestMode(struct struct_test *act_structptr)
{

 	set_hintwindow("EnterTestmode",COL_NEUTRAL,(testflow->running_left ? LEFT : RIGHT));	  // LEFT, RIGHT     
	if ( ipRfp_rsx_EnterTestmode() )
	{
		act_structptr->resultstatus = check_value(0);
		return(0);
	}
	// check enter testmode is ready
	// funct only in testmode
	// use default Freq Band
	if ( ipRfp_rsx_SetFrequencyBand((unsigned char)1) )       
	{
		act_structptr->resultstatus = check_value(1);
		return(0);
	}
 	set_hintwindow("",COL_NEUTRAL,(testflow->running_left ? LEFT : RIGHT));	  // LEFT, RIGHT     
	act_structptr->resultstatus = check_value(2);
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


TESTFUNCTION AdjustVcc1V8(struct struct_test *act_structptr)
{
int i;	
double volt=0;
double voltMiddle=0;
double voltDiff=0;
double voltDiffLast=0;
char cInfo[256];
 /*
TP525	// 	 SI_VDD  1v8
TP203   //	 GND 
*/

unsigned char DecValueMin;
unsigned char DecValueMax;
unsigned char DecValue;
unsigned char DecValueLast;

	DecValueMin=(unsigned char)act_structptr->intpara[0];
	DecValueMax=(unsigned char)act_structptr->intpara[1];
 	set_hintwindow("",COL_NEUTRAL,(testflow->running_left ? LEFT : RIGHT));	  // LEFT, RIGHT     
//	if(protest_set_relay2 (PROTEST_RELAY2, REL2_K27, 1)) return(0x110b); 
//	if(protest_adc_board_configure (PROTEST_ADC1,TP203,  TP525,ADC_DCDIVIDER2)) return(0x110c);
	if(ipRfp_rsx_ReadEepromBandgap (&DecValue))
		if(ipRfp_rsx_ReadEepromBandgap (&DecValue)) return(0x0519);
	if ( DecValue < DecValueMin || DecValue > DecValueMax )
	{
		DecValue=(DecValueMax+DecValueMin)/2;	   // DecValue falsch also mitte benutzen
	}	
	if ( ipRfp_rsx_SetBandgap (DecValue) ) 
		if ( ipRfp_rsx_SetBandgap (DecValue) ) return(0x051a);  
	Sleep(200);   //200ms
//setup MiniSys:
	if(MiniSys_ADC_RaSe(0, 2)) //setup DC Measurement, Range: 2V 
		return(0x1001);  
	if(MiniSys_ADC_InpS(1, 9)) //setup GND1, IN9 
		return(0x1002);  
	Sleep(100);   
		
	if(MiniSys_ADC_getVdc(&volt)) return(0x1003);
//	if(protest_adc_read (PROTEST_ADC1, &volt)) return(0x110d);
//		sprintf(cInfo,"before Adjust EepromValue=0x%02x Volt=%.4f",DecValue, volt);
    voltMiddle   = (act_structptr->lowerlimit + act_structptr->upperlimit) / 2;
    voltMiddle   = voltMiddle + 0.007;   //1.81 ist besser als 1.79
    voltDiff     = act_structptr->upperlimit;       // nur ein grosser DummyWert fuer die erste Abfrage
    voltDiffLast = act_structptr->upperlimit;       // nur ein grosser DummyWert fuer die erste Abfrage
    DecValueLast = DecValue;
	for ( i = (DecValueMax-DecValueMin+2) ; i > 0 ; i-- )
	{
		if ( ipRfp_rsx_SetBandgap (DecValue) ) 
			if ( ipRfp_rsx_SetBandgap (DecValue) ) return(0x051b);  
		Sleep(200);   //200ms
//		if(protest_adc_read (PROTEST_ADC1, &volt)) return(0x110e);
		if(MiniSys_ADC_getVdc(&volt)) return(0x1004);

// voltDiff muss immer positive sein
        if ( volt > voltMiddle )
            voltDiff = volt - voltMiddle;
        else
            voltDiff = voltMiddle - volt;
// the letzte VoltageDiff Wert besser
        if ( voltDiff > voltDiffLast )
		{
            voltDiff = voltDiffLast;
            DecValue = DecValueLast;
            break;                       // ende
        }
// save the last Value
        voltDiffLast = voltDiff;
        DecValueLast = DecValue;
// count up or down
       if ( volt > voltMiddle )
           DecValue--;
       else
           DecValue++;
        if ( DecValue < DecValueMin || DecValue > DecValueMax )
			break;
	}
	if ( ipRfp_rsx_SetBandgap (DecValue) ) 
		if ( ipRfp_rsx_SetBandgap (DecValue) ) return(0x051c);  
	Sleep(200);   //400ms
	if(MiniSys_ADC_getVdc(&volt)) return(0x1005);
// for later test
	gl_DecValue = DecValue;
// nur im Produktion Mode
	if( isInProductionMode() )
	{	
// wenn ok dann wegschreiben
		if ((volt >= act_structptr->lowerlimit) && (volt <= act_structptr->upperlimit))
		{
			if(ipRfp_rsx_WriteEepromBandgap(DecValue)) return(0x051d);
		}
	}
	act_structptr->resultstatus = check_value(volt);
	return(0);			   
}


TESTFUNCTION CheckVcc1V8Value(struct struct_test *act_structptr)
{
unsigned char value;
	value = gl_DecValue;
	act_structptr->resultstatus = check_value(value);
	return(0);
}

	

TESTFUNCTION AdjustBmcClock(struct struct_test *act_structptr)
{
int i;
int    steps=0;
double stepsdouble;
double step_width=0;
double dFreq, dFreqLast, dFreqDiff, dFreqDiffLast;
double dFreqMiddle;
double dBMC_Clock_min, dBMC_Clock_max;
unsigned char ClockValue,ClockValueLast;
char cInfo[256];


	if(ipRfp_rsx_ReadEepromClockFrequency (&ClockValue)) return(0x0521);  
// Soll 10368000 Hz
// RFP3Gen:
// Min  10367998 Hz   // - 0.2ppm
// Max  10368002 Hz   // + 0.2ppm
// Stepweite ist etwa 2Hz per Bit (laut Aastra)
// RFP4Gen:
// Min  10367995 Hz   // - 0.5ppm
// Max  10368005 Hz   // + 0.5ppm

	
// set OC7, Relay for 10MHz
	if(MiniSys_set_OC(7,1)) return(0x1000);
	if((CMD60_ConfAfmForm (gpib_handle.cmd60Cnt, LT30Mhz, RMS)) != 0 ) return(0x0204);
	if(ipRfp_rsx_SetClock (0x01)) return(0x051f);
	Sleep(200);  //200ms
	if(CMD60_ReadAfmCoun (gpib_handle.cmd60Cnt, &dBMC_Clock_min)) return(0x0205);

	if(ipRfp_rsx_SetClock (0xFF)) return(0x0520);
	Sleep(500);  //500ms
	if(CMD60_ReadAfmCoun (gpib_handle.cmd60Cnt, &dBMC_Clock_max)) return(0x0206);

	step_width = (dBMC_Clock_max - dBMC_Clock_min)/255;
	if(step_width < 0) step_width *= -1;         // nur positive
	if(step_width == 0) step_width = 1;
// get default Value	
	if(ipRfp_rsx_ReadEepromClockFrequency (&ClockValue)) return(0x0521); 
    dFreqMiddle = (act_structptr->lowerlimit + act_structptr->upperlimit) / 2;
	dFreqDiff     = act_structptr->upperlimit;       // nur ein grosser DummyWert fuer die erste Abfrage
    dFreqDiffLast = act_structptr->upperlimit;       // nur ein grosser DummyWert fuer die erste Abfrage
	dFreqLast = 0;
	dFreq = 0;
	steps = 0;
    ClockValueLast = ClockValue;

// nach etwa 8x sollte die Mitte gefunden sein
// zur Sicherheit 12x versuchen bis Timeout kommt
	for ( i = 0 ; i < 12 ; i++ )
	{
		if(ipRfp_rsx_SetClock (ClockValue)) return(0x0522);
		Sleep(300);  //500ms
#ifdef COUNT_LONG_WAY
		if(CMD60_addon_ReadAfmCoun (gpib_handle.cmd60Cnt, &dFreq)) return(0x0208);
#else
		if(CMD60_ReadAfmCoun (gpib_handle.cmd60Cnt, &dFreq)) return(0x0208);
#endif		
// dFreqdiff muss immer positive sein		
		dFreqDiff = dFreq - dFreqMiddle;		//dFrwqMiddle is the otimal Value (center Frequency, (lowerLimit+upperLimit)/2)
		if (dFreqDiff < 0) dFreqDiff *= -1;       //nur positive
		// Treffer genau die Mitte oder +- 1
		if ( dFreqDiff <= 1 )			  //CMD60: +-0.1ppm == +- 1Hz @ 10MHz
		{
			break;                       // ende
		}	
// kleiner 0 = Fehlmessung, Frequenze nicht stabil
		if ( dFreq < 0 )
		{
			break;                       // ende
		}	
		if ( i > 0 )
		{
			// step Weite neue berechnen
			step_width = (dFreqLast - dFreq)/steps;
			if(step_width < 0) step_width *= -1;         // nur positive
			if(step_width == 0) step_width=0.5;
		}
		steps = dFreqDiff/step_width;
		stepsdouble = dFreqDiff/step_width;
		if((stepsdouble-steps) > 0.5) steps++;
		
		if ( steps == 0 ) steps++;
// abfrage um Range erweitert 27.06.2014 @GC
// nicht der ertse durchlauf
// der letzte dFreqDiff Wert besser als dieser
// und letzte dFreq in Range
		if ( i > 0 && dFreqDiff > dFreqDiffLast && dFreqLast >= act_structptr->lowerlimit && dFreqLast <= act_structptr->upperlimit )
		{
			dFreqDiff = dFreqDiffLast;
			ClockValue = ClockValueLast;
			dFreq = dFreqLast;
			break;                       // ende
		}
// save the last Value
        dFreqDiffLast = dFreqDiff;
        ClockValueLast = ClockValue;
        dFreqLast = dFreq;
		// count up or down	
		if ( dFreq > dFreqMiddle )
		{
			ClockValue = ClockValue - steps;				
		}	
		else
		{
			ClockValue = ClockValue + steps;				
		}

	} // end if value corrected
	if(ipRfp_rsx_SetClock (ClockValue)) return(0x0523);
// for later check	
	gl_ClockValue = ClockValue;
	// nur im Produktion Mode
	if( isInProductionMode() )
	{	
		if(ipRfp_rsx_WriteEepromClockFrequency (ClockValue)) return(0x0524); 

	}
// read back BMC temperature:	
	iTemperatureBMC =  ip_struct.rx_buffer[5];
// reset OC7, Relay for 10MHz
	if(MiniSys_set_OC(7,0)) return(0x1000);
	act_structptr->resultstatus = check_value(dFreq);       // Freq absolute in Hz
	return(0);
}


TESTFUNCTION CheckBmcClockValue(struct struct_test *act_structptr)
{
unsigned char value;

	value = gl_ClockValue;
	act_structptr->resultstatus = check_value(value);
	return(0);
}


TESTFUNCTION LogBmcTemperature(struct struct_test *act_structptr)
{
unsigned char value;

	value = iTemperatureBMC;  // answer from if(ipRfp_rsx_WriteEepromClockFrequency (ClockValue))
	act_structptr->resultstatus = check_value(value);
	return(0);
}


TESTFUNCTION CheckTemperatureInternal(struct struct_test *act_structptr)
{
	
	ipRfp_rs232_getTemperature(&InternalTemp, &ExternalTemp);
	act_structptr->resultstatus = check_value(InternalTemp);
	return(0);   
}


TESTFUNCTION CheckTemperatureExternal(struct struct_test *act_structptr)
{
	
	act_structptr->resultstatus = check_value(ExternalTemp);
	return(0);   
}


TESTFUNCTION RFP_ip_TestEnterTestmode(struct struct_test *act_structptr)
{
// folgendes geht nur, wenn rsx_enable aufgerufen wurde	 (in rsx_enable wird auch der win-socket geöffnet)
// geht auch nur einmal
	if (ipRfp_rsx_EnterTestmode()) 
		return(0x0526);   
	act_structptr->resultstatus = check_value(0);
	return(0);
}



TESTFUNCTION RFP_CMD60_connect(struct struct_test *act_structptr)
{
char   szRFPI[20]  = "1000000000";
int    i;
int    rc;
long   iRetry;
char   *pMode;
char   cInfo[256];
char   cErrorInfo[256];
double dDeviation;
double dDeviationMiddle;
double dDeviationDiff;
double dDeviationDiffLast;
double dNTP;
double dDev_NEG;
double dDev_POS;
double dHfOffset;     
double dRfLevel; 
int    iFrequencyBand;
int    iPowerLevel;
int rfpart,radio;	
unsigned char readActiveRadio;            // read Active Radio from the connection, or the last connection
unsigned char DeviationAdj;
unsigned char DeviationAdjLast;
unsigned char DeviationAdjMin;
unsigned char DeviationAdjMax;

// ### between Murata connector and air antenna loss: 2dBm , cable = Relay ++ about 3dBm, 3dBm - 2 dBm = 1dBm overall for RSSI
// but for power output: there have to be 2 dB more !!! stupid !!!
//  setup in sequence :  act_structptr->doublepara[0],  act_structptr->doublepara[1]
// dummy release:
	CMD60_ProcConnRel (gpib_handle.cmd60);

//RF-Nail1
	dHFOffsetRF1 = act_structptr->doublepara[0] + act_structptr->system_offsets[HF_OFFSET];
//RF-Nail2
	dHFOffsetRF2 = act_structptr->doublepara[1] + act_structptr->system_offsets[HF_OFFSET];
	iTrafficcarrier = act_structptr->intpara[0]; 
//assign HF-offset, traficcarrier and -slot dependent from radio
	dHfOffset = dHFOffsetRF2;
	iTrafficslot = 6;		 // iTrafficslot gerade = gerades Radio (0)
	iTrafficcarrier = 0;    // channel ?
	dCarrierOffset = 0;
	radio=0;                 

	(void)CMD60_Reset(gpib_handle.cmd60);
//CMD-setup:	
// Set Fixed Part 
	if(CMD60_ConfTest(gpib_handle.cmd60, FP)) return(0x020b);
//set Antenna 0 (CMD send this to RFP)	
	if(CMD60_ConfFpAnt (gpib_handle.cmd60, 0)) return(0x20c);	
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
//set Antenna 0 (CMD send this to RFP)	
	if(CMD60_ConfFpAnt (gpib_handle.cmd60, 0)) return(0x20c);	
	
// set Bursts for NTP and MAX/MIN modulation (1..12000 default=4)
	if(CMD60_ConfAverBurs  (gpib_handle.cmd60, 4)) return(0x0211);
//set Bursts for Timing  (2..2000  default=32)
	if(CMD60_ConfTimAverBurs (gpib_handle.cmd60, 32)) return(0x0212);
//set packet type PP32Z (prolonged preamble)
	if( CMD60_addon_SetPacketTypePP32Z(gpib_handle.cmd60) != 0 ) return(0x0213); //[@JM080918]
//set Antenna   0 or 1	
	if(CMD60_ConfFpAnt(gpib_handle.cmd60, 0)) return(0x0214);   // default after reset is 0
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


//#ifdef POWER_WITHOUT_BREAK_CONNECTION
TESTFUNCTION PowerRF_J33(struct struct_test *act_structptr)
{
int rfpart;
double dNTP;


	if(MiniSys_set_OC(6,0)) return(0x1000);
	if(ipRfp_rsx_SetDiversity(0x01)) return(0x0525);	  //sollte J33 sein
	
// loop back data type : 00001111
//	if(CMD60_ConfModDataType (gpib_handle.cmd60, FIG2)) return(0x021a);
// set Bursts for NTP and MAX/MIN modulation (1..12000 default=4)
//	if(CMD60_ConfAverBurs  (gpib_handle.cmd60, 4)) return(0x021b);
//set Bursts for Timing  (2..2000  default=32)
//	if(CMD60_ConfTimAverBurs (gpib_handle.cmd60, 32)) return(0x021c);
	Sleep(200);  // 200 ms 
	if (CMD60_ReadNtp (gpib_handle.cmd60, &dNTP))
	{
// fail NaN
		dNTP = -200;                              // on error -200  
	}
	else
	{	
		dNTP += dHFOffsetRF1;
	}	
	act_structptr->resultstatus = check_value(dNTP);
	return(0);		
}


TESTFUNCTION RF_BER_J33(struct struct_test *act_structptr)
{

double dBerRfLevel, dBER, dFER;

	dBerRfLevel = -88.0 + dHFOffsetRF1;
// set Ber Eval Window: 100 Bursts 
	if(CMD60_ConfBerEvalWind (gpib_handle.cmd60, 100)) return(0x0241);
	if(CMD60_ConfBerRfLev (gpib_handle.cmd60,(dBerRfLevel))) return(0x0242);

	if(CMD60_ReadBerLter (gpib_handle.cmd60, &dBER, &dFER)) dBER = 9999;	 // bei NAN  dBERR auf 9999
    else changeToPPM(&dBER, &dFER); 	
	act_structptr->resultstatus = check_value(dBER);
	return(0);		
}



TESTFUNCTION PowerRF_J32(struct struct_test *act_structptr)
{
int rfpart;
double dNTP;

	if(MiniSys_set_OC(6,1)) return(0x1000); //set RF path

	if(ipRfp_rsx_SetDiversity(0x02)) return(0x0525);	  //sollte J32 sein
// loop back data type : 00001111
//	if(CMD60_ConfModDataType (gpib_handle.cmd60, FIG2)) return(0x021a);
// set Bursts for NTP and MAX/MIN modulation (1..12000 default=4)
//	if(CMD60_ConfAverBurs  (gpib_handle.cmd60, 4)) return(0x021b);
//set Bursts for Timing  (2..2000  default=32)
//	if(CMD60_ConfTimAverBurs (gpib_handle.cmd60, 32)) return(0x021c);
	Sleep(200);  // 200 ms 
	if (CMD60_ReadNtp (gpib_handle.cmd60, &dNTP))
	{
// fail NaN
		dNTP = -200;                              // on error -200  
	}
	else
	{	
		dNTP += dHFOffsetRF2;
	}	
	act_structptr->resultstatus = check_value(dNTP);
	return(0);		
}

TESTFUNCTION RF_BER_J32(struct struct_test *act_structptr)
{

double dBerRfLevel, dBER, dFER;

	dBerRfLevel = -88.0 + dHFOffsetRF1;
// set Ber Eval Window: 100 Bursts 
	if(CMD60_ConfBerEvalWind (gpib_handle.cmd60, 100)) return(0x0241);
	if(CMD60_ConfBerRfLev (gpib_handle.cmd60,(dBerRfLevel))) return(0x0242);

	if(CMD60_ReadBerLter (gpib_handle.cmd60, &dBER, &dFER)) dBER = 9999;	 // bei NAN  dBERR auf 9999
    else changeToPPM(&dBER, &dFER); 	
	act_structptr->resultstatus = check_value(dBER);

	if(MiniSys_set_OC(6,0)) return(0x1000); //set RF path
	return(0);		
}



/*
TESTFUNCTION PowerRF_J33(struct struct_test *act_structptr)		  //TC0 = TrafficCarrier 0    
{
int rfpart;
double dNTP;

char   szRFPI[20]  = "1000000000";

	if(MiniSys_set_OC(6,0)) return(0x1000);
	if(ipRfp_rsx_SetDiversity(0x01)) return(0x0525);	  //sollte J33 sein

	CMD60_ProcConnRel (gpib_handle.cmd60);
	iTrafficcarrier = act_structptr->intpara[0]; 
	if(CMD60_ConfFpChannels(gpib_handle.cmd60, iTrafficslot, iTrafficcarrier, dCarrierOffset)) return(0x020f);
//	if(CMD60_WriteInstrData (gpib_handle.cmd60, "CONF:FP:TRAF:CARR:5\n"))   return(0x021c);  

// loop back data type : 00001111 
//	if(CMD60_ConfModDataType(gpib_handle.cmd60,FIG2)) return(0x0210);
//set Antenna 0 (CMD send this to RFP)	
	if(CMD60_ConfFpAnt (gpib_handle.cmd60, 0)) return(0x20c);	
// set Bursts for NTP and MAX/MIN modulation (1..12000 default=4)
	if(CMD60_ConfAverBurs  (gpib_handle.cmd60, 4)) return(0x021b);
//set Bursts for Timing  (2..2000  default=32)
	if(CMD60_ConfTimAverBurs (gpib_handle.cmd60, 32)) return(0x021c);
	
	
	CMD60_ProcConnSet (gpib_handle.cmd60);

	Sleep(200);  // 200 ms 
	
	
	if (CMD60_ReadNtp (gpib_handle.cmd60, &dNTP))
	{
// fail NaN
		dNTP = -200;                              // on error -200  
	}
	else
	{	
		dNTP += dHFOffsetRF1;
	}	
	act_structptr->resultstatus = check_value(dNTP);
	return(0);		
}




TESTFUNCTION RF_BER_J33(struct struct_test *act_structptr)
{

double dBerRfLevel, dBER, dFER;

	dBerRfLevel = -88.0 + dHFOffsetRF1;
// set Ber Eval Window: 100 Bursts 
// loop back data type : 00001111 
//	if(CMD60_ConfModDataType(gpib_handle.cmd60,FIG2)) return(0x0210);
//	if(CMD60_ConfBerEvalWind (gpib_handle.cmd60, 100)) return(0x0241);
	if(CMD60_ConfBerRfLev (gpib_handle.cmd60,(dBerRfLevel))) return(0x0242);

	if(CMD60_ReadBerLter (gpib_handle.cmd60, &dBER, &dFER)) dBER = 9999;	 // bei NAN  dBERR auf 9999
    else changeToPPM(&dBER, &dFER); 	
	act_structptr->resultstatus = check_value(dBER);
	return(0);		
}



TESTFUNCTION PowerRF_J32(struct struct_test *act_structptr)
{
int rfpart;
double dNTP;
char   szRFPI[20]  = "1000000000";

	if(MiniSys_set_OC(6,1)) return(0x1000); //set RF path
	if(ipRfp_rsx_SetDiversity(0x02)) return(0x0525);	  //sollte J32 sein
	CMD60_ProcConnRel (gpib_handle.cmd60);
	iTrafficcarrier = act_structptr->intpara[0]; 
	if(CMD60_ConfFpChannels(gpib_handle.cmd60, iTrafficslot, iTrafficcarrier, dCarrierOffset)) return(0x020f);
//	if(CMD60_WriteInstrData (gpib_handle.cmd60, "CONF:FP:TRAF:CARR:5\n"))   return(0x021c);  
// loop back data type : 00001111 
	if(CMD60_ConfModDataType(gpib_handle.cmd60,FIG2)) return(0x0210);
//set Antenna 1 (CMD send this to RFP)	
	if(CMD60_ConfFpAnt (gpib_handle.cmd60, 1)) return(0x20c);	
// set Bursts for NTP and MAX/MIN modulation (1..12000 default=4)
	if(CMD60_ConfAverBurs  (gpib_handle.cmd60, 4)) return(0x021b);
//set Bursts for Timing  (2..2000  default=32)
	if(CMD60_ConfTimAverBurs (gpib_handle.cmd60, 32)) return(0x021c);
	CMD60_ProcConnSet (gpib_handle.cmd60);
	if(ipRfp_rsx_SetDiversity(0x02)) return(0x0525);	  //sollte J32 sein
	Sleep(200);  // 200 ms 
	if (CMD60_ReadNtp (gpib_handle.cmd60, &dNTP))
	{
// fail NaN
		dNTP = -200;                              // on error -200  
	}
	else
	{	
		dNTP += dHFOffsetRF2;
	}	
	act_structptr->resultstatus = check_value(dNTP);
	return(0);		
}

TESTFUNCTION RF_BER_J32(struct struct_test *act_structptr)
{

double dBerRfLevel, dBER, dFER;

	dBerRfLevel = -88.0 + dHFOffsetRF1;
// set Ber Eval Window: 100 Bursts 
// loop back data type : 00001111 
	if(CMD60_ConfModDataType(gpib_handle.cmd60,FIG2)) return(0x0210);
	if(CMD60_ConfBerEvalWind (gpib_handle.cmd60, 100)) return(0x0241);
	if(CMD60_ConfBerRfLev (gpib_handle.cmd60,(dBerRfLevel))) return(0x0242);

	if(CMD60_ReadBerLter (gpib_handle.cmd60, &dBER, &dFER)) dBER = 9999;	 // bei NAN  dBERR auf 9999
    else changeToPPM(&dBER, &dFER); 	
	act_structptr->resultstatus = check_value(dBER);

	if(MiniSys_set_OC(6,0)) return(0x1000); //set RF path
	return(0);		
}

*/


TESTFUNCTION RssiHighRF(struct struct_test *act_structptr)
{
int rfpart;
double dHfOffset; 
double dRfLevel;
double dRssiAttenuation;	   // Dämpfung
unsigned char value;

// ### between Murata connector and air antenna loss: 2dBm , cable = Relay ++ about 3dBm, 3dBm - 2 dBm = 1dBm overall for RSSI
// but for power output: there have to be 2 dB more !!! stupid !!!
//  setup in sequence :  act_structptr->doublepara[0],  act_structptr->doublepara[1]

// eMail Carten Becker 08.02.2012 (FCC-Zulassung)
// Die Messung findet am Messpunkt statt an der Antenne spielt sich dann die Realität ab.
// Hierzwischen liegen (Systembedingt) 2dB. 
// Insofern haben wir diesen Offset (neben den aufbaubedingten Anpassungen) so ermittelt (und auch in die neue TS eingesetzt): 
// Remark:
//     >>>>> Please implement Offset-value of 2dB in CMD-Settings (the measured NTP at Testpoint is 2dB higher than on antenna joint) 
//

// ##### add new 06/06/2018
	if(ipRfp_rsx_SetDiversity(0x01)) return(0x0525);	  //sollte J33 sein
	if(MiniSys_set_OC(6,0)) return(0x1000); //set RF path
//#### add end
//assign HF-offset, 
	dHfOffset = dHFOffsetRF1; // antenna 1 (J33)
 	set_hintwindow("",COL_NEUTRAL,(testflow->running_left ? LEFT : RIGHT));	  // LEFT, RIGHT     
//CMD60 - setup:
// loop back data type : 00001111
	if(CMD60_ConfModDataType (gpib_handle.cmd60, FIG2)) return(0x0223);
// set Bursts for NTP and MAX/MIN modulation (1..12000 default=4)
	if(CMD60_ConfAverBurs  (gpib_handle.cmd60, 4)) return(0x0224);
// RF-Level:
//RSSI Dämpfung
	dRssiAttenuation = -2.0;         //		experimental, 
	dRfLevel = -45.0 + dHfOffset + dRssiAttenuation;
	if(dRfLevel > -40)	dRfLevel = -40;
	if(CMD60_ConfFpRfLev (gpib_handle.cmd60, (dRfLevel))) return(0x0225);
//DUT-setup:	
	if(ipRfp_rsx_SetFrequencyBand (0x01)) return(0x0531);
//	CMD60_ProcConnSet (gpib_handle.cmd60);
	
	Sleep(500);   
	if(ipRfp_rsx_GetRSSI (&value)) return(0x0532);
	gl_iRssi45 = value; 
	act_structptr->resultstatus = check_value(gl_iRssi45);
	return(0);
}                                     


TESTFUNCTION RssiLowRF(struct struct_test *act_structptr)
{
int rfpart;
double dHfOffset;     
double dRfLevel;
double dRssiAttenuation;	   // Dämpfung
unsigned char value;
// eMail Carten Becker 08.02.2012 (FCC-Zulassung)
// Die Messung findet am Messpunkt statt an der Antenne spielt sich dann die Realität ab.
// Hierzwischen liegen (Systembedingt) 2dB. 
// Insofern haben wir diesen Offset (neben den aufbaubedingten Anpassungen) so ermittelt (und auch in die neue TS eingesetzt): 
// Remark:
//     >>>>> Please implement Offset-value of 2dB in CMD-Settings (the measured NTP at Testpoint is 2dB higher than on antenna joint) 
//assign HF-offset, traficcarrier and -slot dependent from radio
	dHfOffset = dHFOffsetRF1;

//CMD60 - setup:
// loop back data type : 00001111
//	if(CMD60_ConfModDataType (gpib_handle.cmd60, FIG2)) return(0x0227);
// set Bursts for NTP and MAX/MIN modulation (1..12000 default=4)
//	if(CMD60_ConfAverBurs  (gpib_handle.cmd60, 4)) return(0x0228);
// RF-Level:
//RSSI Dämpfung
	dRssiAttenuation = -2.0;         //		experimental, 
	dRfLevel = -75.0 + dHfOffset + dRssiAttenuation;
	if(dRfLevel > -40)	dRfLevel = -40;
	if(CMD60_ConfFpRfLev (gpib_handle.cmd60, ( dRfLevel ))) return(0x0229);
//DUT-setup:	
	Sleep(500);   
	if(ipRfp_rsx_GetRSSI (&value)) return(0x0533);
	gl_iRssi75 = value; 
	act_structptr->resultstatus = check_value(gl_iRssi75);
	return(0);                                            
}                                                         


TESTFUNCTION RssiKdecRF(struct struct_test *act_structptr)
{
int rfpart;	
int eeprom_addr;
double cFaktor;
unsigned char kdec;

//					          (-45dBm + 75dBm) *64.0
//CompFaktor = round(------------------------------------------)
//					   ADC(rssi= -45dBm) - ADC(rssi = -75dBm)
	cFaktor = (-45+75)*64;
	cFaktor = cFaktor/ (gl_iRssi45 - gl_iRssi75);
	cFaktor = cFaktor + 0.5;
	kdec = (unsigned char)cFaktor;
	eeprom_addr = 0x49;
	if(ipRfp_rsx_WriteEeprom(eeprom_addr, &kdec,1,0))	 // !!! habe das ruecklesen rausgenommen, geht nicht (  BIT_IP_RFP_35.c, Zeile 3207 : #ifdef READ_BACK
		return(0x0535);	 
	act_structptr->resultstatus = check_value(kdec);      
	return(0);                                            
} 


TESTFUNCTION RssiOffsetRF(struct struct_test *act_structptr)
{
int rfpart;	
int eeprom_addr;
double cOffset;
unsigned char kdec;    


//					  ADC(rssi= -45dBm) * (75 - 45)dBm
//CompOffset = round(------------------------------------------+45dBm )                                                         
//                       ADC(rssi= -45dBm) - ADC(rssi = -75dBm) 

	cOffset = gl_iRssi45 * (75-45);
	cOffset = cOffset/ (gl_iRssi45 - gl_iRssi75);
	cOffset = cOffset + 45;
	cOffset = cOffset + 0.5;
	kdec = (unsigned char)cOffset;
	eeprom_addr = 0x47;
	if(ipRfp_rsx_WriteEeprom(eeprom_addr, &kdec,1,0))return(0x0537);	 
	act_structptr->resultstatus = check_value(kdec);      
	return(0);                                            
}                                                         


TESTFUNCTION PowerRampRF(struct struct_test *act_structptr)
{
int rfpart;
double dHfOffset;  
double dRfLevel;     
int retry;
int i;
double dNTPDummy;
unsigned int verdictForPowerRamp;
unsigned int verdictForNTPDummy;
// Match the Power-Ramp Template ??
	//assign radio
	rfpart = act_structptr->intpara[0];
//falscher Parameter
	if ( rfpart < 1 || rfpart > 2 ) return(0x2010);
	//assign HF-offset, traficcarrier and -slot dependent from radio
	if (1 == rfpart)    //RF-Part1
	{
		dHfOffset = dHFOffsetRF1;
	}
	else		        //RF-Part2
	{
		dHfOffset = dHFOffsetRF2;
	}
//CMD60 - setup:
// RF-Level:
	dRfLevel = -45.0 + dHfOffset;
	if(dRfLevel > -40)	dRfLevel = -40;
	if(CMD60_ConfFpRfLev(gpib_handle.cmd60, dRfLevel)) return(0x022b);
//reset to default values for Power Ramp and NTP.
	if ( CMD60_CalcLimPowTranTempCle (gpib_handle.cmd60) )  return(0x022c);
/*
** Remark: Please configure CMD60/65 Escape-Sequence (via Config-Key) with Data: 7000000000 (Receiver)
** Configure Power-Ramp Packet Type: PP32 (Transmitter)
*/
//set packet type PP32Z (prolonged preamble)
	if( CMD60_addon_SetPacketTypePP32Z(gpib_handle.cmd60) != 0 ) return(0x022d); //[@JM080918]
#ifdef RFP35_BER_WITH_ESC_SEQUENCE
// muss nach connection
// activate SW-demod for BER
// CONF:MESS:ESC:MT "EE60010000"
	if ( (i=CMD60_addon_activate_SW_Demod(gpib_handle.cmd60))) return(i);
#endif //RFP35_BER_WITH_ESC_SEQUENCE
// loop back data type : 00001111
	if(CMD60_ConfModDataType (gpib_handle.cmd60, FIG2)) return(0x022e);
// set Bursts for NTP and MAX/MIN modulation (1..12000 default=4)
	if(CMD60_ConfAverBurs  (gpib_handle.cmd60, 4)) return(0x022f);
//set Bursts for Timing  (2..2000  default=32)
	if(CMD60_ConfTimAverBurs (gpib_handle.cmd60, 32)) return(0x0230);
// ntp min max Werte setzen sind nur Dummy Werte
	if(CMD60_CalcLimPowTranTempTol (gpib_handle.cmd60, 10.0 , 30.0 )) return(0x0231);  
	for ( retry=5, verdictForPowerRamp=9 ; verdictForPowerRamp != 0 && retry > 0 ; retry-- )
	{
// die ersten zwei Messungen werden benötigt dammit
// eine PowerRamp Messung erzeugt wird
//
// Normal ntp lesen
		//if(CMD60_addon_ReadNormalTransmitPower (gpib_handle.cmd60,&dNTPDummy)) return(0x0232);  
		if(CMD60_addon_ReadNormalTransmitPower (gpib_handle.cmd60,&dNTPDummy)) continue;  //fail NaN  [@GC 10.05.2012]
// Verdict NTP lesen
		if(CMD60_CalcLimPowTranTolMatc (gpib_handle.cmd60 , &verdictForNTPDummy)) return(0x0233);  
      	// <0> : MATC | measurement does not exceed the tolerance.
       	// <1> : NMAT | measurement exceeds the tolerance.
       	// <2> : INV  | no measurement result available.
		// dies ist die PowerRamp Auswertung
		if(CMD60_CalcLimPowTranMatc (gpib_handle.cmd60,&verdictForPowerRamp)) return(0x0234);
		//        <0> : MATC | signal matches template.
    	//        <1> : NMAT | signal does not match template.
    	//        <2> : INV  | no measurement result available.
	}
	act_structptr->resultstatus = check_value(verdictForPowerRamp);
	return(0);                                            
}                                                         

TESTFUNCTION Write_SerialandMAC(struct struct_test *act_structptr)
{
//	ipRfp_rsx_Close();         
	
	if(ipRfp_rs232_sendMACandSerial(strMAC, strSerial))
			act_structptr->resultstatus = check_value(-1);
		
	act_structptr->resultstatus = check_value(0);
	return(0); //ok
}


TESTFUNCTION CheckBootImage(struct struct_test *act_structptr)   
{
char mail_buffer[256];


char *pExpected;
char linkStatus[256];


	pExpected = act_structptr->string[0]; // 

	sprintf(mail_buffer,"bit\n");
	if(ipRfp_rs232_mail_out(mail_buffer,pExpected,NULL))    // waits 5 second for answer
		{
	 	set_hintwindow("Boot Image Error!",COL_NEUTRAL,(testflow->running_left ? LEFT : RIGHT));	  // LEFT, RIGHT     
		act_structptr->resultstatus = check_value(-1);
		}
	else
		{
	 	set_hintwindow("Boot Image OK!",COL_NEUTRAL,(testflow->running_left ? LEFT : RIGHT));	  // LEFT, RIGHT     
		act_structptr->resultstatus = check_value(1);
		}
		
	return(0);
}


TESTFUNCTION FinalizeEeprom(struct struct_test *act_structptr)
{
int	 retry;	
char cInfo[256];
char finalState[80];
char bmcCalibStatus[80];

	if(isInProductionMode())
	{
	 	set_hintwindow("Finalize Eeprom",COL_NEUTRAL,(testflow->running_left ? LEFT : RIGHT));	  // LEFT, RIGHT     
       	if ( ipRfp_rsx_FinalizeEeprom() != 0 )
       	{
           	act_structptr->resultstatus = check_value(1);
           	return(0);		// error
       	}
	}
	act_structptr->resultstatus = check_value(0);
	return(0); //ok
}


TESTFUNCTION ShutDownDUT(struct struct_test *act_structptr)
{
char cInfo[256];
// use libssh @GC 13.01.2012
	(void)ipRfp_libssh_Close();
// close socket connection
	(void)ipRfp_rsx_Close();         
// new @GC 14.09.2011
	if ( ipRfp_rs232_sendHalt() )
	{
		act_structptr->resultstatus = check_value(1);
		return(0);	  //error
	}
	act_structptr->resultstatus = check_value(0);
	return(0);
}	
	

TESTFUNCTION Disconnect(struct struct_test *act_structptr)
{
#ifdef USE_ONLY_PoE_INJECTOR

	if(MiniSys_set_OC(4, 0))   // switch power off via opto-switch
		return(0x1135);
	Sleep(200);   
#endif	
	if(testflow->running_left)
	{
		iBarCodeLeft = 0;
	}
	else
	{
		iBarCodeRight = 0;
	}
	act_structptr->resultstatus = check_value(0);
	return(0);
}








