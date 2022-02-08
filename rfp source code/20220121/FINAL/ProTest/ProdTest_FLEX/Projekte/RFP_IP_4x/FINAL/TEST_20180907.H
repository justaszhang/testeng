#ifndef TEST_H
#define TEST_H

#define RFP35_PROJECT			//wird in tools.c benoetigt  [@GC03052011]


#undef  CMD60_SETTINGS_DECT     // @GC 22.07.2011  
#undef  USE_BARTENDER            
#define PRINT_LABEL				// use in Final-Test



#define RFP35_DEBUG_COPY_RS232_LOG "G:\\Protest\\tmp\\rs232_logs"   // save,remove all rs232 Logs see Test Disconnect()
																	// in Functiontest save and in Final test Remove	


#include <windows.h>				// [GC@25052011]    need for Sleep
									// #ifdef _WINDOWS_  
									// #endif
#include <utility.h>
#include <ansi_c.h>
#include <userint.h>
#include <rs232.h>
#include <string.h>
#include <formatio.h>
#include <tcpsupp.h> 

#include "minisystem.h"
#include "WLanEmbedded.h"
#include "rtepubl.h"
#include "device.h"
#include "Cmd60.h"
#include "cmd.h"
#include "CMD60_addon.h"
#include "tool.h"
#include "rte_tool.h"
#include "rfp35_eeprom_01.h"
#include "flex.h" 
#include "break.h" 

#include "BIT_IP_RFP_35.H"
#include "uselibssh.h"
#include "useftpd.h"

//*******************************************************************************************  
#define		HOODLEFT			1   //portc1(0x0)
#define		HOODRIGHT			2   //portc2(0x1)
#define 	REFERENCE			"Reference"

#undef 	MHz_EXTERN		     //CMD60 external Clock 

//*******************************************************************************************  
//struct structQPDS structQPDS;
/////für globale Steuerung der CMD60 Einstellungen////////////////
//int  			iZeroCounter;
int 			iDummyslot;
int 			iDummycarrier;
int 			iTrafficslot;
int 			iTrafficcarrier;


time_t  gl_timePowerOn;          // Zeitstempel PowerOn fuer Test CheckRTC


//constants & variables for label:
char 			*labelname_1;           
char 			labelname_production[MAXPATHNAMELEN];
char 			labelname_repair[MAXPATHNAMELEN];

int BarCode_Label;
int PanelBreak;
int 	iCheckOkay;
 /////////////////////////////////////////////////////////////// 

int	ichoice;
int Global_imainpanel;
int iReference;


//ProductInfo/////////////////////////////////////////////////

char 	softw_release_left[80],
		softw_release_right[80];
char 	sw_stand[3];
		

char    toggle;
		

char 	strSoftwareVersion_left[80];
char 	strSoftwareVersion_right[80];

char 	strTftpDaemon[256];       // command tftpd starter 

//char 	strOpenOCDScriptPath[256];	  // openOCD Script path
//char 	strManuSoftwarePath[256];	  // manu Software path


char 	strHwType[30];		   // strHwType     (RFP35|RFP36|RFP43)
char 	strHwBranding[30];	   // strHwBranding (none|ffsip)



//struct rs232     rs232;        // for Protest
struct rs232_RFP rs232_RFP;    // rs232 for DUT 
struct rs232_RFP ip_struct;    // ip for DUT



int Conect_counter;



char strIP_Address[40];
//*******************************************************************************************  


// DECT HF Offset and WLAN Power Offset
double dHFOffsetDECT_ANT0;           // for DECT ANT0 Left
double dHFOffsetDECT_ANT1;           // for DECT ANT1 Right

double dHFOffsetWLAN_ANT1;           // for WLAN ANT1/Left
double dHFOffsetWLAN_ANT2;           // for WLAN ANT2/Right
double dHFOffsetWLAN_ANT3;           // for WLAN ANT3/Middle

struct structDutParameter
		{
		char 	strExID[80];
		char 	strBgName[80];
		char	strCustSn[80];
		char    strMacAddr[40];
		char    strParkNr[40];
		char	strHwBranding[20];  // strHwBranding (ffsip|ffsip_sc|none)
		char    strHwType[20];	   // strHwType     (RFP35|RFP36|RFP37|RFP43)
		}DutParameter;


struct gpib
	{
	int	cmd60;		//cmd60-handle
	int	rs_nrvs;    //R&S NRVS
	int	hp8594e;    //hp8594e-handle   (WLAN-Antenna)
	int	hp8595a;    //hp8595a-handle   (Harmonic) 
	}gpib_handle;
	
char gMAC_DUT[0x20];
char gSerialNr_DUT[0x20];


char gMAC_Scanned[0x20];
char gSerialNr_Scanned[0x20];

int 	iBarCodeRight;  
int 	gDebugMiniSys;
int 	gDebugWlanEmbedded;

/*** Menue Function ******************************************/
/*** Menue Function ******************************************/
//int dut_set_diversity0(void);
int dut_set_diversity1(void);
int dut_set_diversity2(void);
int MiniSysSetOC5(void); //OC5    
int MiniSysReSetOC5(void); //OC5    
int MiniSysSetOC7(void); //OC7    
int MiniSysReSetOC7(void); //OC7    


/*** System Function ******************************************/
int GetBarCode(char* strMAC, char* strSerial);
void disconnect(int mode);
int start_up (struct rte_globals *global_sets);
int clean_up(int mode);
int flow_control(struct test_flow *test_flow);
int io_error(struct test_flow *test_flow);
void LogShortResult(int mode);
int dut_error (struct test_flow *test_flow);
char file_get_group(struct struct_file *file_group, FILE *fa_r,char *group);
char file_getpara(struct struct_file *file_group, char *tag,void *parameter, int type);


/*** Test Function ******************************************/
TESTFUNCTION PowerOn(struct struct_test *act_structptr);		         //@GC 18.09.2012
TESTFUNCTION SupplyCurrentPSU(struct struct_test *act_structptr);        //@GC 18.09.2012
TESTFUNCTION SSH_PortConnect(struct struct_test *act_structptr);         //@GC 18.09.2012

TESTFUNCTION Check_MAC_Serial(struct struct_test *act_structptr);
TESTFUNCTION LogDUTSerialNumber(struct struct_test *act_structptr);
TESTFUNCTION LogScannedSerialNumber(struct struct_test *act_structptr);
TESTFUNCTION LogMAC(struct struct_test *act_structptr);

TESTFUNCTION CheckLED_red(struct struct_test *act_structptr);
TESTFUNCTION CheckLED_green(struct struct_test *act_structptr);
TESTFUNCTION CheckLED_blue(struct struct_test *act_structptr);
TESTFUNCTION Check_button(struct struct_test *act_structptr);
TESTFUNCTION IP_PortConnect(struct struct_test *act_structptr);

TESTFUNCTION RFP_CMD60_connect(struct struct_test *act_structptr);
TESTFUNCTION PowerRF_J33(struct struct_test *act_structptr);
TESTFUNCTION PowerRF_J32(struct struct_test *act_structptr);

TESTFUNCTION WLanTxPower_Ant1(struct struct_test *act_structptr);
TESTFUNCTION WLanTxPower_Ant2(struct struct_test *act_structptr);
TESTFUNCTION WLanTxPower_Ant3(struct struct_test *act_structptr);


TESTFUNCTION FactoryDeliveryState(struct struct_test *act_structptr);        // @GC 12.10.2012 ersetzt Factory Reset 
TESTFUNCTION Disconnect(struct struct_test *act_structptr);


#endif  //TEST_H
