#ifndef TEST_H
#define TEST_H

#define RFP35_PROJECT			      // wird in tools.c benoetigt  [@GC03052011]

#define RFP_FUNCTIONAL_TEST			// DR, 20160610,   libssh login needs some changes in  FinalTest

#undef   RFP35_BER_TEMP_BS33			// 17.09.12   (11.10.2012 erneut aktiviert @GC und am 23.11.2012 wieder deaktiviert @GC)

#undef  RFP35_BER_WITH_ESC_SEQUENCE      // ohne ESC Sequence 17.06.2011

#define RFP35_DEBUG_COPY_RS232_LOG "G:\\Protest\\tmp\\rs232_logs"   // save,remove all rs232 Logs see Test Disconnect()

#include <windows.h>				// [GC@25052011]    need for Sleep
#include <utility.h>
#include <ansi_c.h>
#include <userint.h>
#include <rs232.h>
#include <string.h>
#include <formatio.h>
#include <tcpsupp.h>    

#include "rtepubl.h"
#include "device.h"
#include "CMD60.h"
#include "CMD.h"
#include "CMD60_addon.h"
#include "tool.h"
#include "flex.h"  
#include "rte_tool.h"
#include "rfp35_eeprom_01.h"


#include "BIT_IP_RFP_35.H"
#include "uselibssh.h"


//#define 	MHz_EXTERN		     //CMD60 external Clock 

#define		HOODLEFT			0x01   //PA0 Bit0
#define		HOODRIGHT			0x02   //PA1 Bit1


#ifdef USE_QPDS
struct structC structQPDS;
#endif

struct structDutParameter
		{
		char 	strExID[80];
		char 	strERP_PartNo[80];
		char	strCustSn[80];
		char    strMacAddr[40];
		char    strParkNr[40];
		char	strHwBranding[10];  // strHwBranding (ffsip|ffsip_sc|none)
		char    strHwType[10];	   // strHwType     (RFP35|RFP36|RFP37|RFP43)
		}DutParameter;
		
/////für globale Steuerung der CMD60 Einstellungen////////////////

int 	iDummyslot;
int 	iDummycarrier;
int 	iTrafficslot;
int 	iTrafficcarrier;
int     iTemperatureBMC;
double dCarrierOffset;

///////////////////////////////////////////////////////////////

int		gl_iNeedRebootTime;  	 // Flag ob Reboot nach Flash benoetigt wird, oder nicht
time_t  gl_timePowerOn;          // Zeitstempel PowerOn fuer Test Clock_TP331_32KHz

int	ichoice;
int Global_imainpanel;

unsigned char eeprom_compare_value[80];

char 	softw_release_left[80],
		softw_release_right[80];
char	toggle;
char 	sw_stand[3];
char 	strSoftwareVersion_left[80];
char 	strSoftwareVersion_right[80];
char 	strTftpDaemon[256];       // command tftpd starter 
char 	strOpenOCDScriptPath[256];	  // openOCD Script path
char 	strManuSoftwarePath[256];	  // manu Software path


#ifndef STRUCT_RS232
#define STRUCT_RS232
struct rs232
	{
	int		port;
	int		chk_ackn;
	char	tx_buffer[80];
	char	rx_buffer[82];
	char	tx_count;
	char	rx_count;
	char 	dut_type;
	char	mail_type;
	char	protest_addr;
	};
#endif

struct rs232     rs232;        // for Protest
struct rs232_RFP rs232_RFP;    // rs232 for DUT 
struct rs232_RFP ip_struct;    // ip for DUT


char 	strBarcode_right[MAXPATHNAMELEN];
char 	strBarcode_left[MAXPATHNAMELEN];  

char 	strSerial[MAXPATHNAMELEN];
char 	strMAC[MAXPATHNAMELEN];


char 	strFlexUir[MAXPATHNAMELEN];  
int iBarCodeLeft;
int iBarCodeRight;
int BarCode_Label;
int 	iCheckOkay;

int Conect_counter;

//*****global variables needed for splitting functions:
int gl_iRssi45,gl_iRssi75;
unsigned char gl_ClockValue;			 //@GC 17.06.2011
unsigned char gl_DecValue;				 //@GC 17.06.2011
unsigned char gl_DeviationAdj;           //@GC 17.06.2011

double	gl_dDeviationNeg;
int     gl_berRetry;             //@GC 31.05.2011

char strIP_Address[30];


int 	gDebugMiniSys;
int 	gDebugWlanEmbedded;

// RFP35 hat nur noch eine Antenne und kein Links oder Rechts
double dHFOffsetRF1;           // for Radio1
double dHFOffsetRF2;           // for Radio0

//*******************************
struct gpib
	{
	int	cmd60;		//cmd60-handle
	int	cmd60Cnt;		//cmd60-handle
	int	rs_nrvs;    //R&S NRVS
	int	hp8594e;    //hp8594e-handle   (WLAN-Antenna)
	int	hp8595a;    //hp8595a-handle   (Harmonic) 
	}gpib_handle;

//----------------------------------------------

/*** Menue Function ******************************************/
int dut_set_diversity1(void);
int dut_set_diversity2(void);
int dut_OpenCOM1(void);
int dut_CloseCOM1 (void);
int dut_PowerOn (void);
int dut_PowerOff(void);
int dut_OpenLibssh(void);



/*** System Function ******************************************/
int start_up (struct rte_globals *global_sets);
int clean_up(int mode);
int flow_control(struct test_flow *test_flow);
int io_error(struct test_flow *test_flow);
int dut_error (struct test_flow *test_flow);
void disconnect(int mode);
int io_error(struct test_flow *test_flow);
void LogShortResult(int mode);


char file_get_group(struct struct_file *file_group, FILE *fa_r,char *group);
char file_getpara(struct struct_file *file_group, char *tag,void *parameter, int type);
int  GetBarCode(char* strBarCode, char* strSerial,int site);




/*** Test Function ******************************************/
TESTFUNCTION PowerOn(struct struct_test *act_structptr);  
TESTFUNCTION PowerOnCheckTFTP(struct struct_test *act_structptr);						// @GC18.09.2012  
TESTFUNCTION SupplyCurrentPSU(struct struct_test *act_structptr); 


TESTFUNCTION CheckIprfp4gDnld(struct struct_test *act_structptr);


TESTFUNCTION LogSerialNumber(struct struct_test *act_structptr);
TESTFUNCTION LogMAC(struct struct_test *act_structptr);
TESTFUNCTION Clock_25MHz(struct struct_test *act_structptr);

TESTFUNCTION RFP_ip_TestEnterTestmode(struct struct_test *act_structptr);

TESTFUNCTION RS232_EnableRSX(struct struct_test *act_structptr);
TESTFUNCTION RS232_CheckLinkStatus(struct struct_test *act_structptr);  
TESTFUNCTION RS232_getIpAddr(struct struct_test *act_structptr);  

TESTFUNCTION SSH_PortConnect(struct struct_test *act_structptr);
TESTFUNCTION CheckTemperature(struct struct_test *act_structptr);
TESTFUNCTION CheckBMCStatus(struct struct_test *act_structptr);
TESTFUNCTION CheckLinkStatus(struct struct_test *act_structptr);  

TESTFUNCTION IP_PortConnect(struct struct_test *act_structptr);
TESTFUNCTION IP_EnterTestMode(struct struct_test *act_structptr);

TESTFUNCTION CheckDECTSwVersion(struct struct_test *act_structptr);

TESTFUNCTION AdjustVcc1V8(struct struct_test *act_structptr);
TESTFUNCTION CheckVcc1V8Value(struct struct_test *act_structptr);
TESTFUNCTION AdjustBmcClock(struct struct_test *act_structptr);
TESTFUNCTION CheckBmcClockValue(struct struct_test *act_structptr);
TESTFUNCTION LogBmcTemperature(struct struct_test *act_structptr);
TESTFUNCTION CheckTemperatureInternal(struct struct_test *act_structptr);
TESTFUNCTION CheckTemperatureExternal(struct struct_test *act_structptr);


TESTFUNCTION RFP_CMD60_connect(struct struct_test *act_structptr);
TESTFUNCTION PowerRF_J33(struct struct_test *act_structptr);
TESTFUNCTION PowerRF_J32(struct struct_test *act_structptr);

TESTFUNCTION RF_BER_J32(struct struct_test *act_structptr);
TESTFUNCTION RF_BER_J33(struct struct_test *act_structptr);




TESTFUNCTION RssiHighRF(struct struct_test *act_structptr);
TESTFUNCTION RssiLowRF(struct struct_test *act_structptr);				//@JM20110502
TESTFUNCTION RssiKdecRF(struct struct_test *act_structptr);				//@JM20110502
TESTFUNCTION RssiOffsetRF(struct struct_test *act_structptr);			//@JM20110502

TESTFUNCTION PowerRampRF(struct struct_test *act_structptr);
TESTFUNCTION CheckBootImage(struct struct_test *act_structptr);
TESTFUNCTION CheckBootFlashImage(struct struct_test *act_structptr);
TESTFUNCTION Check_TestSoftwareVersion(struct struct_test *act_structptr); 


TESTFUNCTION Write_SerialandMAC(struct struct_test *act_structptr);  
TESTFUNCTION FinalizeEeprom(struct struct_test *act_structptr);

TESTFUNCTION Disconnect_V2(struct struct_test *act_structptr);
TESTFUNCTION ShutDownDUT(struct struct_test *act_structptr);


#endif   //test_header
