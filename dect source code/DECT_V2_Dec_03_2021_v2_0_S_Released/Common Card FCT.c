
#include <gpib.h>
#include <cvintwrk.h>
#include <formatio.h>
#include "Serial Number pnl.h"
#include "MAC pnl.h"
#include <rs232.h>
#include <ansi_c.h>
#include <utility.h>
#include <cvirte.h>		
#include <userint.h>
#include "Front Panel1.h"
#include "Common Card FCT.h"
#include "FCT Functions.h"
#include "MSG OK.h"
#include "MSG YES.h"
#include "hp5313xa.h" 
#include "LabJackUD.h"
#include "AudzerLib.h"
#include "inifile.h"
#include "toolbox.h"
#include "rtx2012base.h"     

LJ_HANDLE LabJack_Handle = 0; 

ViSession  UCT_HANDLE = 0; 
ViSession  MT8852B_HANDLE = 0;
ViSession  CMD60_HANDLE = 0;
ViSession  AGILENT_HANDLE = 0;


//local functions
int Set_Serial_Number(int panel, int control);
int Set_MAC_Address(int panel,int control);  
int Set_Test_Number(int panel,int control);
int Test_Stage_Start(int panel,int control, int test_stage) ;
int Test_Stage_Finish(int panel,int control, int test_stage);


//global variables
int test_stage = 1;
int MSG_FLAG_1 = 0;
int MSG_YES_FLAG = 0;
int MSG_YES_PASS = 1;
int Printer_Port_On = 0;
int debug = 1;
int network_enable = 0;
int VT100_Filter = 1;
int apc_present_in = 0;
int apc_sata_act_in = 0; 
int kernel_silent = 0;
int android_time = 0;
int lid_position = 1;
int test_fixture = 1;
int Fixture_Read = 0;
int scan_serial_numbers = 0;



static int gThreadID;
static IniText g_myInifile = 0;
static char    g_fileName[MAX_PATHNAME_LEN];  
static double g_dSpkFreq;
static double g_dSpkAmp; 
static double g_dSpkThd; 

static double g_dRecFreq;
static double g_dRecAmp; 
static double g_dRecThd; 

static double g_dTremFreq;
static double g_dTremAmp; 
static double g_dTremThd; 



static double rfoffset; 
static double btoffset;


//enter TBR6 mode
unsigned char EnterTBR6Mode[15] = {0x0E, 0x10, 0x0C, 0x82, 0x00, 0xC3, 0xF1, 0xE0, 0x02, 0x54, 0x42, 0x52, 0x36, 0x00, 0x20};
char          EnterTBR6Mode_Response[12] = {0x0C, 0xE0,0x00,0xC4,0xF1,0x01,0x00,0x54,0x42,0x52,0x36,0x00};

//enter the test mode
unsigned char EnterTestMode[9] = {0x08, 0x10, 0x06, 0x82, 0x00, 0x04, 0xF0, 0xE6, 0x90};
char          EnterTestMode_Response[8] = {0x08, 0xE6, 0x00, 0x00, 0xF0, 0x84, 0x46, 0x22};


//enter the test mode
unsigned char EnterEnhancedTestMode[9] = {0x08, 0x10, 0x06, 0x82, 0x00, 0x00, 0xF7, 0xE6, 0x93};
char          EnterEnhancedTestMode_Response[5] = {0x05, 0xE6, 0x00, 0x01, 0xF7, 0xF6};

//write eeprom hw release
unsigned char WriteEepromHWRelease[13] = {0x0C, 0x10, 0x0A, 0x82, 0x00, 0x42, 0xF0, 0xEB, 0x0A, 0x00, 0x01, 0x00, 0xD0};
char 		  WriteEepromHWRelease_Response[7] = {0x07, 0xEB, 0x00, 0x43, 0xF0, 0x0A, 0x00}; 

//file system functionality
unsigned char FileSystemFunctionality[23] = {0x16, 0x10, 0x14, 0x82, 0x25, 0x04, 0x44, 0x06, 0x01, 0x30, 0x0A, 0x43, 0x3A, 0x2F, 0x48, 0x44, 0x46, 0x2E, 0x43, 0x46, 0x47, 0x00, 0xAA };
//SP 11 char          FileSystemFunctionality_Response[45] = {0x2D,0xE6,0x30,0x05,0x44,0x07,0x01,0x00,0x00,0x01,0x00,0x00,0xC2,0x01,0x00,0x00,0x00,0x80,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0xC2,0xDE,0x0B,0x00,0x00,0x00,0x4A,0xF5,0xDD,0x10,0x58,0xF5,0xDD,0x10,0x58,0xF5,0xDD,0x71};
char FileSystemFunctionality_Response[45] = {0x2D, 0xE6, 0x30, 0x05, 0x44, 0x07, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0xC2, 0x01, 0x00, 0x00, 0x00, 0x80, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0xC2, 0xDB, 0x0B, 0x00, 0x00, 0x00, 0x4A, 0xF5, 0xDD, 0x10, 0x58, 0xF5, 0xDD, 0x10, 0x58, 0xF5, 0xDD}; //7.2.1 

//reset EEPROM test flow
unsigned char ResetEepromTestflowData[13] = {0x0C, 0x10, 0x0A, 0x82, 0x00, 0x42, 0xF0, 0xEB, 0x01, 0x00, 0x01, 0x00, 0xDB};
char          ResetEepromTestflowData_Response[7] = {0x07, 0xEB, 0x00, 0x43, 0xF0, 0x01, 0x00};

//sd init test
unsigned char SDInit[9] = {0x08, 0x10, 0x06, 0x82, 0x00, 0xD0, 0xF1, 0xE6, 0x45};
char          SDInit_Response[6] = {0x06, 0xE6, 0x00, 0xD1, 0xF1, 0x00};

//write eeprom ipei
unsigned char WriteEepromIpei[17] = {0x10, 0x10, 0x0E, 0x82, 0x00, 0x42, 0xF0, 0xEB, 0x21, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF};
char 		  WriteEepromIpei_Response[7] = {0x07, 0xEB, 0x00, 0x43, 0xF0, 0x21, 0x00}; 

//write eeprom serial number
unsigned char WriteEepromSerialNumber[17] = {0x10, 0x10, 0x0E, 0x82, 0x00, 0x42, 0xF0, 0xEB, 0x05, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0xDB};
char 		  WriteEepromSerialNumber_Response[7] = {0x07, 0xEB, 0x00, 0x43, 0xF0, 0x05, 0x00}; 

//read in the mac address of the unit
unsigned char ReadEepromMacAddress[15] = {0x0E, 0x10, 0x0C, 0x82, 0x00, 0xA2, 0xF1, 0xE6, 0x06, 0x80, 0x00, 0x00, 0x08, 0x00, 0xB9};
char		  ReadEepromMacAddress_Response[11] = {0x14, 0xE6, 0x00, 0xA3, 0xF1, 0x06, 0x80, 0x00, 0x00, 0x08, 0x00};

//Branding Check
unsigned char BrandingVersionCheck[15] = {0x0E, 0x10, 0x0C, 0x82, 0x00, 0xA2, 0xF1, 0xE6, 0x17, 0x80, 0x00, 0x00, 0x03, 0x00, 0xA3};
char 		  BrandingVersionCheck_Response[15] = {0x0F, 0xE6, 0x00, 0xA3, 0xF1, 0x17, 0x80, 0x00, 0x00, 0x03, 0x00, 0x01, 0x0E, 0x0E, 0x00};

//Branding Check Proximus
unsigned char BrandingVersionCheckProximus[15] = {0x0E, 0x10, 0x0C, 0x82, 0x00, 0xA2, 0xF1, 0xE6, 0x17, 0x80, 0x00, 0x00, 0x06, 0x00, 0xA6};
char 		  BrandingVersionCheckProximus_Response[18] = {0x12, 0xE6, 0x00, 0xA3, 0xF1, 0x17, 0x80, 0x00, 0x00, 0x06, 0x00, 0x01, 0x80, 0x0E, 0x00, 0x00, 0x01, 0x00};

//Branding
unsigned char BrandingVersionSet[21] = {0x14, 0x10, 0x12, 0x82, 0x00, 0xA4, 0xF1, 0xE6, 0x17, 0x80, 0x00, 0x00, 0x03, 0x00, 0x01,0x0E,0x0E,0x00,0x00,0x00,0xA4};
char 		  BrandingVersionSet_Response[13] = {0x0E, 0xE6, 0x00, 0xA5, 0xF1, 0x17, 0x80, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00};  

//Branding Check Proximus 
unsigned char BrandingVersionSetProximus[21] = {0x14, 0x10, 0x12, 0x82, 0x00, 0xA4, 0xF1, 0xE6, 0x17, 0x80, 0x00, 0x00, 0x06, 0x00, 0x01,0x80,0x0E,0x00,0x00,0x01,0x2E}; 
char 		  BrandingVersionSetProximus_Response[13] = {0x0E, 0xE6, 0x00, 0xA5, 0xF1, 0x17, 0x80, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00};

//get extended sw version
unsigned char GetSwVersionString[9] = {0x08, 0x10, 0x06, 0x82, 0x00, 0x10, 0xF0, 0xE6, 0x84};
char		  GetSwVersionString_Response[76] = {0x4C,0xE6,0x00,0x11,0xF0,0x37,0x2E,0x30,0x2E,0x53,0x50,0x31,0x31,0x20,0x31,0x33,0x32,0x2E,0x37,0x30,0x2E,0x33,0x35,0x20,0x65,0x65,0x70,0x3A,0x39,0x37,
												0x2E,0x37,0x20,0x6D,0x6D,0x69,0x3A,0x38,0x30,0x30,0x45,0x30,0x30,0x30,0x30,0x30,0x31,0x20,0x68,0x77,0x3A,0x33,0x2E,0x30,0x2E,0x76,0x32,0x2E,0x36,0x2E,
												0x30,0x43,0x46,0x37,0x20,0x70,0x72,0x64,0x3A,0x33,0x47,0x4D,0x41,0x4B,0x53,0x00,0xEC,0x00};   //SP11
//dev 7.2.1	
//char		  GetSwVersionString_Response[57] = {0x38, 0xE6, 0x00, 0x11, 0xF0, 0x37, 0x2E, 0x32, 0x2E, 0x31, 0x20, 0x31, 0x33, 0x32, 0x2E, 0x37, 0x32, 0x2E, 0x38, 0x20, 0x65, 0x65, 0x70, 0x3A, 0x39, 0x37, 0x2E, 0x36, 0x32, 0x20, 0x68, 0x77, 0x3A, 0x33, 0x2E, 0x2A, 0x2E, 0x76, 0x32, 0x2E, 0x36, 0x2E, 0x32, 0x32, 0x20, 0x70, 0x72, 0x64, 0x3A, 0x33, 0x47, 0x4D, 0x41, 0x4B, 0x53, 0x00, 0xC4};//7.2.1

//parksearchcriteria, write 0x01 to location 0x0060
unsigned char ParkSearchCriteria[13] = {0x0C, 0x10, 0x0A, 0x82, 0x00, 0x42, 0xF0, 0xEB, 0x60, 0x00, 0x01, 0x01, 0xBB};
char		  ParkSearchCriteria_Response[7] = {0x07, 0xEB, 0x00, 0x43, 0xF0, 0x60, 0x00};


//eeprompli1, writes 0x25 to location 0x0093
unsigned char EepromPli1[13] = {0x0C, 0x10, 0x0A, 0x82, 0x00, 0x42, 0xF0, 0xEB, 0x93, 0x00, 0x01, 0x25, 0x6C};
char		  EepromPli1_Response[7] = {0x07, 0xEB, 0x00, 0x43, 0xF0, 0x93, 0x00};


// eeprompark1, writes 0x00 0x01 0x40 0x00 0x00 to location 0x0061
unsigned char EepromPark1[17] = {0x10, 0x10, 0x0E, 0x82, 0x00, 0x42, 0xF0, 0xEB, 0x61, 0x00, 0x05, 0x00, 0x01, 0x40, 0x00, 0x00, 0xFE}; 
char		  EepromPark1_Response[7] = {0x07, 0xEB, 0x00, 0x43, 0xF0, 0x61, 0x00}; 


//get service state
unsigned char GetServiceState[9] = {0x08, 0x10, 0x06, 0x82, 0x00, 0x52, 0xF1, 0xE6, 0xC7};
char          GetServiceState_Response[6] = {0x06, 0xE6, 0x00, 0x53, 0xF1, 0x01};


//eu frequency deviation band index 0 
unsigned char ReadEepromDeviationEU[10] = {0x09, 0x10, 0x07, 0x82, 0x00, 0x1C, 0xF1, 0xE6, 0x00, 0x89};
char          ReadEepromDeviationEU_Response[6] = {0x06, 0xE6, 0x00, 0x1D, 0xF1, 0x27};

//get rssi
unsigned char GetRssi[9] = {0x08, 0x10, 0x06, 0x82, 0x00, 0x0B, 0xF1, 0xE6, 0x9E};
char          GetRssi_Response[6] = {0x06, 0xE6, 0x00, 0x0C, 0xF1, 0x1D};


//eepromrssicompkfactor, writes 0x00 to location 0x004C
unsigned char EepromRssiCompKFactor_00[13] = {0x0C, 0x10, 0x0A, 0x82, 0x00, 0x42, 0xF0, 0xEB, 0x4C, 0x00, 0x01, 0x00, 0x96};
char		  EepromRssiCompKFactor_00_Response[7] = {0x07, 0xEB, 0x00, 0x43, 0xF0, 0x4C, 0x00};

//eepromrssicompOffset, writes 0x00 to location 0x004B
unsigned char EepromRssiCompOffset_00[13] = {0x0C, 0x10, 0x0A, 0x82, 0x00, 0x42, 0xF0, 0xEB, 0x4B, 0x00, 0x01, 0x00, 0x91};
char		  EepromRssiCompOffset_00_Response[7] = {0x07, 0xEB, 0x00, 0x43, 0xF0, 0x4B, 0x00};

//read VCC1
unsigned char ReadEepromVcc1[9] = {0x08, 0x10, 0x06, 0x82, 0x00, 0x67, 0xF1, 0xE6, 0xF2};
char          ReadEepromVcc1_Response[6] = {0x06, 0xE6, 0x00, 0x68, 0xF1, 0x00}; 

//set VCC1
unsigned char SetVcc1[10] = {0x09, 0x10, 0x07, 0x82, 0x00, 0xE0, 0xF1, 0xE6, 0x00, 0x75};
char          SetVcc1_Response[5] = {0x05, 0xE6, 0x00, 0x21, 0xF1}; 

//write VCC1
unsigned char WriteEepromVcc1[13] = {0x0C, 0x10, 0x0A, 0x82, 0x00, 0x42, 0xF0, 0xEB, 0x67, 0xF1, 0x01, 0x00, 0x4C};
char          WriteEepromVcc1_Response[7] = {0x07, 0xEB, 0x00, 0x43, 0xF0, 0x67, 0xF1};

//make backup
unsigned char MakeBackup[9] = {0x08, 0x10, 0x06, 0x82, 0x00, 0xA8, 0xF1, 0xE6, 0x3D};
char          MakeBackup_Response[8] = {0x07, 0xE6, 0x00, 0xA9, 0xF1, 0x02, 0x00, 0x00};

//enter Bluetooth test mode
unsigned char BtEnterDutMode[9] = {0x08, 0x10, 0x06, 0x82, 0x00, 0x92, 0xF1, 0xE6, 0x07};
char          BtEnterDutMode_Response[6] = {0x06, 0xE6, 0x00, 0x98, 0xF1, 0x00}; 

//Bluetooth Address
unsigned char BtSetBDAddr[20] = {0x13, 0x10, 0x11, 0x82, 0x00, 0x93, 0xF1, 0xEB, 0x01,0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C};
char          BtSetBDAddr_Response[8] = {0x08, 0xEB, 0x00, 0x99, 0xF1, 0x00, 0x01, 0x00};


//get blue tooth device clock synced with the bt tester
unsigned char BtSyncClock[9] = {0x08, 0x10, 0x06, 0x82, 0x00, 0x60, 0xF3, 0xE6, 0xF7};
char          BtSyncClock_Response[6] = {0x06, 0xE6, 0x00, 0x61, 0xF3, 0x00}; 

//get blue tooth analog frequency trim value
unsigned char BtGetAnaFtrim[9] = {0x08, 0x10, 0x06, 0x82, 0x00, 0x62, 0xF3, 0xE6, 0xF5};
char          BtGetAnaFtrim_Response[8] = {0x08, 0xE6, 0x00, 0x63, 0xF3, 0x00, 0x10, 0x00}; 
	
//bt eeprom write, to write bt clock adjustment value
unsigned char BtEepromWrite[16] = {0x0F, 0x10, 0x0D, 0x82, 0x00, 0xA4, 0xF1, 0xE6, 0x06, 0x80, 0x08, 0x00, 0x01, 0x00,0x00,0xBE};
char          BtEepromWrite_Response[15] = {0x0E, 0xE6, 0x00, 0xA5, 0xF1, 0x06, 0x80, 0x08, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00 }; 


//bt eeprom read, to read bt clock adjustment value		 BT_EEPROM_READ
unsigned char BtEepromRead[15] = {0x0E, 0x10, 0x0C, 0x82, 0x00, 0xA2, 0xF1, 0xE6, 0x06, 0x80, 0x08, 0x00, 0x01, 0x00, 0xB8};
char          BtEepromRead_Response[14] = {0x0D, 0xE6, 0x00, 0xA3, 0xF1, 0x06, 0x80, 0x08, 0x00, 0x01, 0x00, 0x00, 0x00, 0xFF}; 





//write eeprom LED
unsigned char WriteEepromLED[13] = {0x0C, 0x10, 0x0A, 0x82, 0x00, 0x42, 0xF0, 0xEB, 0x9B, 0x01, 0x01, 0x00, 0x40};
char 		  WriteEepromLED_Response[7] = {0x07, 0xEB, 0x00, 0x43, 0xF0, 0x9B, 0x01};

//write eeprom SOS
unsigned char WriteEepromSOS[13] = {0x0C, 0x10, 0x0A, 0x82, 0x00, 0x42, 0xF0, 0xEB, 0x95, 0x01, 0x01, 0x00, 0x4E};
char 		  WriteEepromSOS_Response[7] = {0x07, 0xEB, 0x00, 0x43, 0xF0, 0x95, 0x01};

//write eeprom HOTKEY
unsigned char WriteEepromHOTKEY[13] = {0x0C, 0x10, 0x0A, 0x82, 0x00, 0x42, 0xF0, 0xEB, 0x96, 0x01, 0x01, 0x00, 0x4D};
char 		  WriteEepromHOTKEY_Response[7] = {0x07, 0xEB, 0x00, 0x43, 0xF0, 0x96, 0x01};

//write eeprom VIBRATOR
unsigned char WriteEepromVIBRATOR[13] = {0x0C, 0x10, 0x0A, 0x82, 0x00, 0x42, 0xF0, 0xEB, 0x97, 0x01, 0x01, 0x01, 0x4D};
char 		  WriteEepromVIBRATOR_Response[7] = {0x07, 0xEB, 0x00, 0x43, 0xF0, 0x97, 0x01};

//write eeprom earpiece loudness
unsigned char WriteEepromEarpieceLoudness[13] = {0x0C, 0x10, 0x0A, 0x82, 0x00, 0x42, 0xF0, 0xEB, 0x2A, 0x00, 0x01, 0x25, 0xD5};
char 		  WriteEepromEarpieceLoudness_Response[7] = {0x07, 0xEB, 0x00, 0x43, 0xF0, 0x2A, 0x00}; 

//write eeprom func date		             
unsigned char WriteEepromFuncDate[15] = {0x0E, 0x10, 0x0C, 0x82, 0x00, 0x42, 0xF0, 0xEB, 0x11, 0x00, 0x03, 0x00, 0x00, 0x00, 0xC9};
char		  WriteEepromFuncDate_Response[7] = {0x07, 0xEB, 0x00, 0x43, 0xF0, 0x11, 0x00};

//write eeprom final date		             
unsigned char WriteEepromFinalDate[15] = {0x0E, 0x10, 0x0C, 0x82, 0x00, 0x42, 0xF0, 0xEB, 0x14, 0x00, 0x03, 0x00, 0x00, 0x00, 0xCC};
char		  WriteEepromFinalDate_Response[7] = {0x07, 0xEB, 0x00, 0x43, 0xF0, 0x14, 0x00};

//set the test flow to ft passed
unsigned char SetEepromTestflowDataToFTPassed[13] = {0x0C, 0x10, 0x0A, 0x82, 0x00, 0x42, 0xF0, 0xEB, 0x01, 0x00, 0x01, 0x10, 0xCB};
char          SetEepromTestflowDataToFTPassed_Response[7] = {0x07, 0xEB, 0x00, 0x43, 0xF0, 0x01, 0x00};


//set the test flow to Final passed
unsigned char SetEepromTestflowDataToFinalPassed[13] = {0x0C, 0x10, 0x0A, 0x82, 0x00, 0x42, 0xF0, 0xEB, 0x01, 0x00, 0x01, 0x30, 0xEB};
char          SetEepromTestflowDataToFinalPassed_Response[7] = {0x07, 0xEB, 0x00, 0x43, 0xF0, 0x01, 0x00}; 


//set the test flow to ft failed
unsigned char SetEepromTestflowDataToFTFailed[13] = {0x0C, 0x10, 0x0A, 0x82, 0x00, 0x42, 0xF0, 0xEB, 0x01, 0x00, 0x01, 0x01, 0xDA};
char          SetEepromTestflowDataToFTFailed_Response[7] = {0x07, 0xEB, 0x00, 0x43, 0xF0, 0x01, 0x00};


//set the test flow to final failed
unsigned char SetEepromTestflowDataToFinalFailed[13] = {0x0C, 0x10, 0x0A, 0x82, 0x00, 0x42, 0xF0, 0xEB, 0x01, 0x00, 0x01, 0x02, 0xD9};
char          SetEepromTestflowDataToFinalFailed_Response[7] = {0x07, 0xEB, 0x00, 0x43, 0xF0, 0x01, 0x00};


//read EEPROM Clock
unsigned char ReadEepromClock[9] = {0x08, 0x10, 0x06, 0x82, 0x00, 0x5D, 0xF1, 0xE6, 0xC8};
char          ReadEepromClock_Response[6] = {0x06, 0xE6, 0x00, 0x5E, 0xF1, 0x00}; 

//set reference clock      
unsigned char SetClock[9] = {0x08, 0x10, 0x06, 0x82, 0x00, 0x56, 0xF1, 0x00, 0x25};
char          SetClock_Response[5] = {0x05, 0xE6, 0x00, 0x21, 0xF1}; 

//write eeprom clock setting
unsigned char WriteEepromClock[10] = {0x09, 0x10, 0x07, 0x82, 0x00, 0x5F, 0xF1, 0xEB, 0x00, 0xC7};
char          WriteEepromClock_Response[5] = {0x05, 0xEB, 0x00, 0x60, 0xF1}; 

//power off test
unsigned char PowerOff[9] = {0x08, 0x10, 0x06, 0x82, 0x00, 0x16, 0xF0, 0xE6, 0x82};
char          PowerOff_Response[6] = {0x06, 0xE6, 0x00, 0x17, 0xF0, 0x02};

//read error counter
unsigned char ReadErrorCounter[12] = {0x0B, 0x10, 0x09, 0x82, 0x00, 0x40, 0xF0, 0xE6, 0xDF, 0x01, 0x05, 0x0F}; 
char ReadErrorCounter_Response[13] = {0x0D, 0xE6, 0x00, 0x41, 0xF0, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00}; 


//read vendor serial number
unsigned char ReadVendorSN[12] = {0x0B, 0x10, 0x09, 0x82, 0x00, 0x40, 0xF0, 0xE6, 0x05, 0x00, 0x05, 0xD4};
char ReadVendorSN_Response[13] = {0x0D, 0xE6, 0x00, 0x41, 0xF0, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00};




//read serial number string
unsigned char ReadSNString[15] = {0x0E, 0x10, 0x0C, 0x82, 0x00, 0xA2, 0xF1, 0xE6, 0x17, 0x80, 0x11, 0x00, 0x0F, 0x00, 0xBE};
char ReadSNString_Response[29] = {0x1B, 0xE6, 0x00, 0xA3, 0xF1, 0x17, 0x80, 0x11, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

//master reset
unsigned char MasterReset[9] = {0x08, 0x10, 0x06, 0x82, 0x00, 0x00, 0xF4, 0xE6, 0x90};
char          MasterReset_Response[5] = {0x05, 0xE6, 0x00, 0x01, 0xF4};


unsigned char TestTMS[10] = {0x09 ,0x10 ,0x07 ,0x82 ,0x00 ,0x70 ,0xF0 ,0xE6 ,0x30 ,0xD4};
char          TestTMS_Response[6] = {0x07, 0xE6, 0x00, 0x71, 0xF0, 0x00}; 


unsigned char TestTMS_V2[10] = {0x09 ,0x10 ,0x07 ,0x82 ,0x00 ,0x70 ,0xF0 ,0xE6 ,0x98 ,0x7C};
char          TestTMS_Response_V2[6] = {0x07, 0xE6, 0x00, 0x71, 0xF0, 0x00}; 

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//AUDIOSTARTTONE
unsigned char AudioStartTone[9] = {0x08, 0x10, 0x06, 0x82, 0x00, 0x43, 0xF1, 0xE6, 0xD6};
char AudioStartTone_Response[6] = {0x05, 0xE6, 0x00, 0xB2, 0xF1, 0x43};

//AUDIOSTOPTONE
unsigned char AudioStopTone[9] = {0x08, 0x10, 0x06, 0x82, 0x00, 0x44, 0xF1, 0xE6, 0xD1};
 char AudioStopTone_Response[6] = {0x05, 0xE6, 0x00, 0xB6, 0xF1, 0x47};

//AUDIOSTARTTONEEARPIECE
unsigned char AudioStartToneEarpiece[9] = {0x08, 0x10, 0x06, 0x82, 0x00, 0x82, 0xF1, 0xE6, 0x17};
 char AudioStartToneEarpiece_Response[6] = {0x05, 0xE6, 0x00, 0xB3, 0xF1, 0x42};


//AUDIOTREMBLERON
unsigned char AudioTremblerOn[9] = {0x08, 0x10, 0x06, 0x82, 0x00, 0x4D, 0xF1, 0xE6, 0xD8};
 char AudioTremblerOn_Response[6] = {0x05, 0xE6, 0x00, 0xBB, 0xF1, 0x4A};


//AUDIOTREMBLEROFF
unsigned char AudioTremblerOff[9] = {0x08, 0x10, 0x06, 0x82, 0x00, 0x4E, 0xF1, 0xE6, 0xDB};
 char AudioTremblerOff_Response[6] = {0x05, 0xE6, 0x00, 0xBC, 0xF1, 0x4D};



//Enhanced Tests
//AUDIO-SPK Test
unsigned char AudioSPK[20] = {0x13, 0x10, 0x11, 0x82, 0x00, 0xC3, 0xF1, 0xE6, 0x02, 0x41, 0x55, 0x44, 0x49, 0x4F, 0x2D, 0x53, 0x50, 0x4B, 0x00, 0x67};
char  AudioSPK_Response[23] = {0x16, 0xE6, 0x00, 0xC4, 0xF1, 0x02, 0xFF, 0x41, 0x55, 0x44, 0x49, 0x4F, 0x2D, 0x53, 0x50, 0x4B, 0x3A, 0x32, 0x42, 0x42, 0x39, 0x00, 0xCA}; 

//AUDIO-MIC
unsigned char AudioMIC[20] = {0x13, 0x10, 0x11, 0x82, 0x00, 0xC3, 0xF1, 0xE6, 0x02, 0x41, 0x55, 0x44, 0x49, 0x4F, 0x2D, 0x4D, 0x49, 0x43, 0x00, 0x68}; 
char  AudioMIC_Response[23] = {0x16, 0xE6, 0x00, 0xC4, 0xF1, 0x02, 0xFF, 0x41, 0x55, 0x44, 0x49, 0x4F, 0x2D, 0x4D, 0x49, 0x43, 0x3A, 0x35, 0x32, 0x39, 0x37, 0x00, 0xC7}; 

//AUDIO-EP
unsigned char AudioEP[19] = {0x12, 0x10, 0x10, 0x82, 0x00, 0xC3, 0xF1, 0xE6, 0x02, 0x41, 0x55, 0x44, 0x49, 0x4F, 0x2D, 0x45, 0x50, 0x00, 0x3A}; 
char  AudioEP_Response[22] = {0x15, 0xE6, 0x00, 0xC4, 0xF1, 0x02, 0xA3, 0x41, 0x55, 0x44, 0x49, 0x4F, 0x2D, 0x45, 0x50, 0x3A, 0x30, 0x36, 0x36, 0x33, 0x00, 0xC3}; 


//TREMBLER
unsigned char Trembler[19] = {0x12, 0x10, 0x10, 0x82, 0x00, 0xC3, 0xF1, 0xE6, 0x02, 0x54, 0x52, 0x45, 0x4D, 0x42, 0x4C, 0x45, 0x52, 0x00, 0x43}; 
char  Trembler_Response[22] = {0x15, 0xE6, 0x00, 0xC4, 0xF1, 0x02, 0xFF, 0x54, 0x52, 0x45, 0x4D, 0x42, 0x4C, 0x45, 0x52, 0x3A, 0x32, 0x32, 0x46, 0x34, 0x00, 0x97}; 


unsigned char SPKTestReset[12] = {0x0B, 0x10, 0x09, 0x82, 0x00, 0x2E, 0xF1, 0xE6, 0x43, 0xF1, 0x01, 0x08}; 
char  SPKTestReset_Response[9] = {0x08, 0xE6, 0x00, 0x2E, 0xF1, 0x43, 0xF1, 0x00, 0x6D}; 

unsigned char RECTestReset[12] = {0x0B, 0x10, 0x09, 0x82, 0x00, 0x2E, 0xF1, 0xE6, 0x82, 0xF1, 0x01, 0xC9}; 
char  RECTestReset_Response[9] = {0x08, 0xE6, 0x00, 0x2E, 0xF1, 0x82, 0xF1, 0x00, 0xAC};


unsigned char TremblerTestReset[12] = {0x0B, 0x10, 0x09, 0x82, 0x00, 0x2E, 0xF1, 0xE6, 0x4D, 0xF1, 0x01, 0x06}; 
char  TremblerTestReset_Response[9] = {0x08, 0xE6, 0x00, 0x2E, 0xF1, 0x4D, 0xF1, 0x00, 0x63}; 



unsigned char SPKTestPass[12] = {0x0B, 0x10, 0x09, 0x82, 0x00, 0x2E, 0xF1, 0xE6, 0x43, 0xF1, 0x00, 0x09}; 
char  SPKTestPass_Response[9] = {0x08, 0xE6, 0x00, 0x2E, 0xF1, 0x43, 0xF1, 0x00, 0x6D}; 

unsigned char RECTestPass[12] = {0x0B, 0x10, 0x09, 0x82, 0x00, 0x2E, 0xF1, 0xE6, 0x82, 0xF1, 0x00, 0xC8}; 
char  RECTestPass_Response[9] = {0x08, 0xE6, 0x00, 0x2E, 0xF1, 0x82, 0xF1, 0x00, 0xAC};


unsigned char TremblerTestPass[12] = {0x0B, 0x10, 0x09, 0x82, 0x00, 0x2E, 0xF1, 0xE6, 0x4D, 0xF1, 0x00, 0x07}; 
char  TremblerTestPass_Response[9] = {0x08, 0xE6, 0x00, 0x2E, 0xF1, 0x4D, 0xF1, 0x00, 0x63}; 





//HEADSET
//unsigned char Headset[18] = {0x11, 0x10, 0x0F, 0x82, 0x00, 0xC3, 0xF1, 0xE6, 0x02, 0x48, 0x45, 0x41, 0x44, 0x53, 0x45, 0x54, 0x00, 0x1E}; 
//I: 1F E6 00 C4 F1 81 00 48 45 41 44 53 45 54 3C 69 6E 73 65 72 74 2D 68 65 61 64 73 65 74 3E 00 AC 
unsigned char Headset[19] = {0x12, 0x10, 0x10, 0x82, 0x00, 0xC3, 0xF1, 0xE6, 0x02, 0x41, 0x55, 0x44, 0x49, 0x4F, 0x2D, 0x48, 0x53, 0x00, 0x34};
char  Headset_Response[21] ={0x14, 0xE6, 0x00, 0xC4, 0xF1, 0x02, 0x2D, 0x48, 0x45, 0x41, 0x44, 0x53, 0x45, 0x54, 0x3A, 0x30, 0x31, 0x43, 0x41, 0x00, 0x69}; 


//=============

unsigned char USB[14] = {0x0D, 0x10, 0x0B, 0x82, 0x00, 0xC3, 0xF1, 0xE6, 0x02, 0x55, 0x53, 0x42, 0x00, 0x10};
//char  USB_Response[16] = {0x0F, 0xE6, 0x00, 0xC4, 0xF1, 0x02, 0x01, 0x55, 0x53, 0x42, 0x3A, 0x63, 0x6F, 0x6E, 0x00, 0x2A}; 	
//char  USB_Response[12] = {0x0B, 0xE6, 0x00, 0xC4, 0xF1, 0x02, 0x00, 0x55, 0x53, 0x42, 0x00, 0x73}; 
char  USB_Response[11] = {0x0B, 0xE6, 0x00, 0xC5, 0xF1, 0x03, 0x00, 0x55, 0x53, 0x42, 0x00}; 

//KEY32 Example
unsigned char Key32[16] = {0x0F, 0x10, 0x0D, 0x82, 0x00, 0xC3, 0xF1, 0x0E, 0x02, 0x4B, 0x45, 0x59, 0x33, 0x32, 0x00, 0xEA};
char          Key32_Response[14] = {0x0D, 0x0E, 0x00, 0xC5, 0xF1, 0x03, 0x00, 0x4B, 0x45, 0x59, 0x33, 0x32, 0x00, 0x61};



//VERSION TESTS
unsigned char VERS_SW[18] = {0x11, 0x10, 0x0F, 0x82, 0x00, 0xC3, 0xF1, 0xE6, 0x02, 0x56, 0x45, 0x52, 0x53, 0x2D, 0x53, 0x57, 0x00, 0x6F}; 
char          VERS_SW_Response[22] = {0x15, 0xE6, 0x00, 0xC4, 0xF1, 0x02, 0x00, 0x56, 0x45, 0x52, 0x53, 0x2D, 0x53, 0x57, 0x3A, 0x37, 0x2E, 0x32, 0x2E, 0x31, 0x00, 0x02}; 



unsigned char VERS_LOADER[22] = {0x15, 0x10, 0x13, 0x82, 0x00, 0xC3, 0xF1, 0xE6, 0x02, 0x56, 0x45, 0x52, 0x53, 0x2D, 0x4C, 0x4F, 0x41, 0x44, 0x45, 0x52, 0x00, 0x7A}; 
char          VERS_LOADER_Response[25] =  {0x18, 0xE6, 0x00, 0xC4, 0xF1, 0x02, 0x00, 0x56, 0x45, 0x52, 0x53, 0x2D, 0x4C, 0x4F, 0x41, 0x44, 0x45, 0x52, 0x3A, 0x31, 0x2E, 0x30, 0x32, 0x00, 0x3E}; 



unsigned char VERS_HW[18] = {0x11, 0x10, 0x0F, 0x82, 0x00, 0xC3, 0xF1, 0xE6, 0x02, 0x56, 0x45, 0x52, 0x53, 0x2D, 0x48, 0x57, 0x00, 0x74}; 
char          VERS_HW_Response[28] = {0x1B, 0xE6, 0x00, 0xC4, 0xF1, 0x02, 0x00, 0x56, 0x45, 0x52, 0x53, 0x2D, 0x48, 0x57, 0x3A, 0x33, 0x2E, 0x2A, 0x2E, 0x56, 0x32, 0x2E, 0x36, 0x2E, 0x32, 0x32, 0x00, 0x66};


unsigned char VERS_BRAND[21] = {0x14, 0x10, 0x12, 0x82, 0x00, 0xC3, 0xF1, 0xE6, 0x02, 0x56, 0x45, 0x52, 0x53, 0x2D, 0x42, 0x52, 0x41, 0x4E, 0x44, 0x00, 0x30}; 
char          VERS_BRAND_Response[22] = {0x15, 0xE6, 0x00, 0xC4, 0xF1, 0x02, 0x00, 0x56, 0x45, 0x52, 0x53, 0x2D, 0x42, 0x52, 0x41, 0x4E, 0x44, 0x3A, 0x6E, 0x61, 0x00, 0x66}; 


unsigned char VERS_ATS[19] = {0x12, 0x10, 0x10, 0x82, 0x00, 0xC3, 0xF1, 0xE6, 0x02, 0x56, 0x45, 0x52, 0x53, 0x2D, 0x41, 0x54, 0x53, 0x00, 0x2D}; 
char          VERS_ATS_Response[21] = {0x14, 0xE6, 0x00, 0xC4, 0xF1, 0x02, 0x00, 0x56, 0x45, 0x52, 0x53, 0x2D, 0x41, 0x54, 0x53, 0x3A, 0x37, 0x2E, 0x32, 0x00, 0x5F}; 


unsigned char VERS_IP65[20] ={0x13, 0x10, 0x11, 0x82, 0x00, 0xC3, 0xF1, 0xE6, 0x02, 0x56, 0x45, 0x52, 0x53, 0x2D, 0x49, 0x50, 0x36, 0x35, 0x00, 0x71}; 
char          VERS_IP65_Response[21] = {0x14, 0xE6, 0x00, 0xC4, 0xF1, 0x02, 0x00, 0x56, 0x45, 0x52, 0x53, 0x2D, 0x49, 0x50, 0x36, 0x35, 0x3A, 0x6E, 0x73, 0x00, 0x35}; 



unsigned char VERS_SNO[19] = {0x12, 0x10, 0x10, 0x82, 0x00, 0xC3, 0xF1, 0xE6, 0x02, 0x56, 0x45, 0x52, 0x53, 0x2D, 0x53, 0x4E, 0x4F, 0x00, 0x39}; 
char          VERS_SNO_Response[20] = {0x13, 0xE6, 0x00, 0xC4, 0xF1, 0x02, 0x00, 0x56, 0x45, 0x52, 0x53, 0x2D, 0x53, 0x4E, 0x4F, 0x3A, 0x6E, 0x73, 0x00, 0x7D}; 


unsigned char VERS_FINAL[21] = {0x14, 0x10, 0x12, 0x82, 0x00, 0xC3, 0xF1, 0xE6, 0x02, 0x56, 0x45, 0x52, 0x53, 0x2D, 0x46, 0x49, 0x4E, 0x41, 0x4C, 0x00, 0x27}; 
char          VERS_FINAL_Response[30] = {0x1D, 0xE6, 0x00, 0xC4, 0xF1, 0x02, 0x00, 0x56, 0x45, 0x52, 0x53, 0x2D, 0x46, 0x49, 0x4E, 0x41, 0x4C, 0x3A, 0x5F, 0x5F, 0x5F, 0x5F, 0x2D, 0x5F, 0x5F, 0x2D, 0x5F, 0x5F, 0x00, 0x7E}; 


unsigned char VERS_IPEI[20] = {0x13, 0x10, 0x11, 0x82, 0x00, 0xC3, 0xF1, 0xE6, 0x02, 0x56, 0x45, 0x52, 0x53, 0x2D, 0x49, 0x50, 0x45, 0x49, 0x00, 0x7E}; 
char          VERS_IPEI_Response[34] = {0x21, 0xE6, 0x00, 0xC4, 0xF1, 0x02, 0x00, 0x56, 0x45, 0x52, 0x53, 0x2D, 0x49, 0x50, 0x45, 0x49, 0x3A, 0x31, 0x31, 0x30, 0x34, 0x31, 0x20, 0x30, 0x31, 0x37, 0x30, 0x39, 0x32, 0x39, 0x20, 0x2A, 0x00, 0x0C};


//////////////////////////////////////////////////////////////////////////////////////////
//User Tests
//LCD
unsigned char LCD_POS[18] = {0x11, 0x10, 0x0F, 0x82, 0x00, 0xC3, 0xF1, 0xE6, 0x02, 0x4C, 0x43, 0x44, 0x2D, 0x50, 0x4F, 0x53, 0x00, 0x7E}; 
char          LCD_POS_Response[31] = {0x1A, 0xE6, 0x00, 0xC4, 0xF1, 0x81, 0x00, 0x4C, 0x43, 0x44, 0x2D, 0x50, 0x4F, 0x53, 0x3C, 0x63, 0x68, 0x65, 0x63, 0x6B, 0x20, 0x70, 0x6F, 0x73, 0x3E, 0x00, 0xB6}; 
char          LCD_POS_Response_P2[20] = {0x13 , 0xE6 , 0x00 , 0xC4 , 0xF1 , 0x82 , 0x04 , 0x4C , 0x43 , 0x44 , 0x2D , 0x50 , 0x4F , 0x53 , 0x3A , 0x50 , 0x4F , 0x53 , 0x00 , 0xEF}; 


unsigned char LCD_RED[18] = {0x11, 0x10, 0x0F, 0x82, 0x00, 0xC3, 0xF1, 0xE6, 0x02, 0x4C, 0x43, 0x44, 0x2D, 0x52, 0x45, 0x44, 0x00, 0x61};
char          LCD_RED_Response[27] = {0x1A, 0xE6, 0x00, 0xC4, 0xF1, 0x81, 0x00, 0x4C, 0x43, 0x44, 0x2D, 0x52, 0x45, 0x44, 0x3C, 0x63, 0x68, 0x65, 0x63, 0x6B, 0x20, 0x72, 0x65, 0x64, 0x3E, 0x00, 0xB6}; 
char          LCD_RED_Response_P2[20] = {0x13, 0xE6, 0x00, 0xC4, 0xF1, 0x82, 0x05, 0x4C, 0x43, 0x44, 0x2D, 0x52, 0x45, 0x44, 0x3A, 0x52, 0x45, 0x44, 0x00, 0xEE};


unsigned char LCD_GREEN[20] = {0x13, 0x10, 0x11, 0x82, 0x00, 0xC3, 0xF1, 0xE6, 0x02, 0x4C, 0x43, 0x44, 0x2D, 0x47, 0x52, 0x45, 0x45, 0x4E, 0x00, 0x69}; 
char          LCD_GREEN_Response[31] = {0x1E, 0xE6, 0x00, 0xC4, 0xF1, 0x81, 0x00, 0x4C, 0x43, 0x44, 0x2D, 0x47, 0x52, 0x45, 0x45, 0x4E, 0x3C, 0x63, 0x68, 0x65, 0x63, 0x6B, 0x20, 0x67, 0x72, 0x65, 0x65, 0x6E, 0x3E, 0x00, 0xB6}; 
char          LCD_GREEN_Response_P2[24] = {0x17, 0xE6, 0x00, 0xC4, 0xF1, 0x82, 0x06, 0x4C, 0x43, 0x44, 0x2D, 0x47, 0x52, 0x45, 0x45, 0x4E, 0x3A, 0x47, 0x52, 0x45, 0x45, 0x4E, 0x00, 0xED};


unsigned char LCD_BLUE[19] =  {0x12, 0x10, 0x10, 0x82, 0x00, 0xC3, 0xF1, 0xE6, 0x02, 0x4C, 0x43, 0x44, 0x2D, 0x42, 0x4C, 0x55, 0x45, 0x00, 0x2C}; 
char          LCD_BLUE_Response[29] = {0x1C, 0xE6, 0x00, 0xC4, 0xF1, 0x81, 0x00, 0x4C, 0x43, 0x44, 0x2D, 0x42, 0x4C, 0x55, 0x45, 0x3C, 0x63, 0x68, 0x65, 0x63, 0x6B, 0x20, 0x62, 0x6C, 0x75, 0x65, 0x3E, 0x00, 0x96}; 
char          LCD_BLUE_Response_P2[22] = {0x15, 0xE6, 0x00, 0xC4, 0xF1, 0x82, 0x07, 0x4C, 0x43, 0x44, 0x2D, 0x42, 0x4C, 0x55, 0x45, 0x3A, 0x42, 0x4C, 0x55, 0x45, 0x00, 0xEC};


//LED
unsigned char LED_RED[18] = {0x11, 0x10, 0x0F, 0x82, 0x00, 0xC3, 0xF1, 0xE6, 0x02, 0x4C, 0x45, 0x44, 0x2D, 0x52, 0x45, 0x44, 0x00, 0x67}; 
char          LED_RED_Response[27] = {0x1A, 0xE6, 0x00, 0xC4, 0xF1, 0x81, 0x00, 0x4C, 0x45, 0x44, 0x2D, 0x52, 0x45, 0x44, 0x3C, 0x63, 0x68, 0x65, 0x63, 0x6B, 0x20, 0x72, 0x65, 0x64, 0x3E, 0x00, 0xB0}; 
char          LED_RED_Response_P2[20] = {0x13, 0xE6, 0x00, 0xC4, 0xF1, 0x82, 0x00, 0x4C, 0x45, 0x44, 0x2D, 0x52, 0x45, 0x44, 0x3A, 0x52, 0x65, 0x64, 0x00, 0xED};  


unsigned char LED_GREEN[20] = {0x13, 0x10, 0x11, 0x82, 0x00, 0xC3, 0xF1, 0xE6, 0x02, 0x4C, 0x45, 0x44, 0x2D, 0x47, 0x52, 0x45, 0x45, 0x4E, 0x00, 0x6F}; 
char          LED_GREEN_Response[31] = {0x1E, 0xE6, 0x00, 0xC4, 0xF1, 0x81, 0x00, 0x4C, 0x45, 0x44, 0x2D, 0x47, 0x52, 0x45, 0x45, 0x4E, 0x3C, 0x63, 0x68, 0x65, 0x63, 0x6B, 0x20, 0x67, 0x72, 0x65, 0x65, 0x6E, 0x3E, 0x00, 0xB0}; 
char          LED_GREEN_Response_P2[24] = {0x17, 0xE6, 0x00, 0xC4, 0xF1, 0x82, 0x00, 0x4C, 0x45, 0x44, 0x2D, 0x47, 0x52, 0x45, 0x45, 0x4E, 0x3A, 0x47, 0x72, 0x65, 0x65, 0x6E, 0x00, 0xED};


unsigned char LED_ORANGE[21] = {0x14, 0x10, 0x12, 0x82, 0x00, 0xC3, 0xF1, 0xE6, 0x02, 0x4C, 0x45, 0x44, 0x2D, 0x4F, 0x52, 0x41, 0x4E, 0x47, 0x45, 0x00, 0x24}; 
char          LED_ORANGE_Response[33] = {0x20, 0xE6, 0x00, 0xC4, 0xF1, 0x81, 0x00, 0x4C, 0x45, 0x44, 0x2D, 0x4F, 0x52, 0x41, 0x4E, 0x47, 0x45, 0x3C, 0x63, 0x68, 0x65, 0x63, 0x6B, 0x20, 0x6F, 0x72, 0x61, 0x6E, 0x67, 0x65, 0x3E, 0x00, 0x90}; 
char          LED_ORANGE_Response_P2[26] = {0x19, 0xE6, 0x00, 0xC4, 0xF1, 0x82, 0x00, 0x4C, 0x45, 0x44, 0x2D, 0x4F, 0x52, 0x41, 0x4E, 0x47, 0x45, 0x3A, 0x4F, 0x72, 0x61, 0x6E, 0x67, 0x65, 0x00, 0xCD};


unsigned char LED_KEYB[19] = {0x12, 0x10, 0x10, 0x82, 0x00, 0xC3, 0xF1, 0xE6, 0x02, 0x4C, 0x45, 0x44, 0x2D, 0x4B, 0x45, 0x59, 0x42, 0x00, 0x21}; 
char          LED_KEYB_Response[29] = {0x1C, 0xE6, 0x00, 0xC4, 0xF1, 0x81, 0x00, 0x4C, 0x45, 0x44, 0x2D, 0x4B, 0x45, 0x59, 0x42, 0x3C, 0x63, 0x68, 0x65, 0x63, 0x6B, 0x20, 0x6B, 0x65, 0x79, 0x62, 0x3E, 0x00, 0x90}; 
char          LED_KEYB_Response_P2[22] = {0x15 , 0xE6 , 0x00 , 0xC4 , 0xF1 , 0x82 , 0x00 , 0x4C , 0x45 , 0x44 , 0x2D , 0x4B , 0x45 , 0x59 , 0x42 , 0x3A , 0x4B , 0x65 , 0x79 , 0x62 , 0x00 , 0xCD}; 







//SDCARD
unsigned char SDCard[14] = {0x0D, 0x10, 0x0B, 0x82, 0x00, 0xC3, 0xF1, 0xE6, 0x02, 0x53, 0x44, 0x43, 0x00, 0x00}; 
char          SDCard_Response[15] = {0x0E, 0xE6, 0x00, 0xC4, 0xF1, 0x02, 0x00, 0x53, 0x44, 0x43, 0x3A, 0x61, 0x76, 0x00, 0x4E}; 


//KEYPAD

//key12
unsigned char KEY12[16] = {0x0F, 0x10, 0x0D, 0x82, 0x00, 0xC3, 0xF1, 0xE6, 0x02, 0x4B, 0x45, 0x59, 0x31, 0x32, 0x00, 0x00}; 
char          KEY12_Response[14] = {0x0D, 0xE6, 0x00, 0xC4, 0xF1, 0x81, 0x00, 0x4B, 0x45, 0x59, 0x31, 0x32, 0x00, 0xE0}; 

//key22
unsigned char KEY22[16] = {0x0F, 0x10, 0x0D, 0x82, 0x00, 0xC3, 0xF1, 0xE6, 0x02, 0x4B, 0x45, 0x59, 0x32, 0x32, 0x00, 0x03}; 
char          KEY22_Response[14] ={0x0D, 0xE6, 0x00, 0xC4, 0xF1, 0x81, 0x00, 0x4B, 0x45, 0x59, 0x32, 0x32, 0x00, 0xE3}; 

//key32
unsigned char KEY32[16] = {0x0F, 0x10, 0x0D, 0x82, 0x00, 0xC3, 0xF1, 0xE6, 0x02, 0x4B, 0x45, 0x59, 0x33, 0x32, 0x00, 0x02}; 
char          KEY32_Response[14] ={0x0D, 0xE6, 0x00, 0xC4, 0xF1, 0x81, 0x00, 0x4B, 0x45, 0x59, 0x33, 0x32, 0x00, 0xE2};


//disable Bluetooth
unsigned char BT_DISABLE[9] = {0x08, 0x10, 0x06, 0x82, 0x00, 0x64, 0xF3, 0xE6, 0xF3}; 
char BT_DISABLE_Response[6] = {0x05, 0xE6, 0x00, 0x65, 0xF3, 0x96}; 


//write serial number
unsigned char WriteSerialNumberString[30] = {0x1D, 0x10, 0x1B, 0x82, 0x00, 0xA4, 0xF1, 0xE6, 0x17, 0x80, 0x11, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xB8};
char 		  WriteSerialNumberString_Response[12] = {0x0E, 0xE6, 0x00, 0xA5, 0xF1, 0x17, 0x80, 0x11, 0x00, 0x03, 0x00, 0x00}; 


//write serial number
unsigned char WriteSerialNumberShortString[24] = {0x17, 0x10, 0x15, 0x82, 0x00, 0xA4, 0xF1, 0xE6, 0x17, 0x80, 0x11, 0x00, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xBE};
char 		  WriteSerialNumberShortString_Response[12] = {0x0E, 0xE6, 0x00, 0xA5, 0xF1, 0x17, 0x80, 0x11, 0x00, 0x03, 0x00, 0x00}; 


//write eeprom serial number
//unsigned char WriteSerialNumberString[21] = {0x14, 0x10, 0x0E, 0x82, 0x00, 0x42, 0xF0, 0xEB, 0x81, 0x01, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x52};
//char 		  WriteSerialNumberString_Response[7] = {0x07, 0xEB, 0x00, 0x43, 0xF0, 0x81, 0x01}; 




//Charger
unsigned char SetChargeCurrentMode_0[10] = {0x09, 0x10, 0x07, 0x82, 0x00, 0x87, 0xF1, 0xE6, 0x00, 0x12}; 
char SetChargeCurrentMode_0_Response[7] = {0x06, 0xE6, 0x00, 0x8C, 0xF1, 0x00, 0x7D};

unsigned char SetChargeCurrentMode_1[10] = {0x09, 0x10, 0x07, 0x82, 0x00, 0x87, 0xF1, 0xE6, 0x01, 0x13}; 
char SetChargeCurrentMode_1_Response[7] = {0x06, 0xE6, 0x00, 0x8C, 0xF1, 0x01, 0x7C};



unsigned char DisableCharger[9] = {0x08, 0x10, 0x06, 0x82, 0x00, 0x5C, 0xF1, 0xE6, 0xC9}; 
char DisableCharger_Response[7] = {0x06, 0xE6, 0x00, 0x8F, 0xF1, 0x00, 0x7E};



unsigned char GetChargerStatus[9] = {0x08, 0x10, 0x06, 0x82, 0x00, 0x86, 0xF1, 0xE6, 0x13}; 
char GetChargerStatus_Response[7] = {0x06, 0xE6, 0x00, 0x8B, 0xF1, 0x01, 0x7B };



static char *MitelAssy612d[6] = {"2GM","2GQ","2WD","3L1","3L4","END"};
static char *MitelAssy622d[6] = {"2GN","2GR","2WE","3L2","3L5","END"};
static char *MitelAssy632d[6] = {"2GP","2GS","2WF","3L3","3L6","END"};




/*****************************************/
/*
/*
/*****************************************/
void ErrorHandler (LJ_ERROR lngErrorcode, long lngLineNumber, long lngIteration)
{
	char err[255];

	if (lngErrorcode != LJE_NOERROR)
	{
		ErrorToString(lngErrorcode,err);
		printf("Error number = %d\n",lngErrorcode);
		printf("Error string = %s\n",err);
		printf("Source line number = %d\n",lngLineNumber);
		printf("Iteration = %d\n\n",lngIteration);
		if(lngErrorcode > LJE_MIN_GROUP_ERROR)
		{
			//Quit if this is a group error.
			getchar();
			exit(0);
		}
   }
}


/*****************************************/
/*
/*
/*****************************************/
int main (int argc, char *argv[])
{
	
	DisableBreakOnLibraryErrors ();
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((panelHandle = LoadPanel (0, "Front Panel1.uir", PANEL)) < 0)
		return -1;
	
	Init_Counter (panelHandle);
	
	Init_MT8852B (panelHandle);
	
	Init_CMD60 (panelHandle);
	
	DisplayPanel (panelHandle);
	save_panel(panelHandle) ;
 
	MakeDir( "CARD LOGS");
	MakeDir( "MITEL LOGS");

	Card_Index = 0;
	Build_Menu(panelHandle);
	Card_Index = 0;
			
	RunUserInterface ();
	
	DiscardPanel (panelHandle);
	return 0;
}

/*****************************************/
/*
/*
/*****************************************/
int CVICALLBACK Start (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
int i, q, Error_num,len,invalid_string, No_Power = 0, err,sn;
char msg[256],COM[10],Test_Sequence[512];
int test_stages = 1;
int status = 1;
int size;
int count = 1;
char temp_char;
int x;
int index = 0;
int temp_index = 0;
int test;
int sequence_complete = 0, stage_done = 0;
int errorcode = 0;
int debug_counter = 0;
int debug_num = 1;
int lid_counter = 0;
int activeCtrl = 0; 
int n = 0;
int prefix_found = 0;
int long_form_final = 0;
	
	switch (event)
		{
		case EVENT_COMMIT:
		{	
			//Setup_DAQ(panel);
			
			if(Test_In_Process) break;
			
			Get_Start_Time(panel);
			Stop_Test_Exe = 0;
			Stopped = 0;
			Stop_In_Process	= 0;
			Test_In_Process = 1;
			Terminated_Early = 0;
			*COM = '\x0';
			Serial_Num_Read = 0;
			MAC_Address_Read = 0;
			CARD_STATE = 0;
			test_stage = 1;
			MSG_FLAG_1 = 0;
			VT100_Filter = 1;
			debug = 1;
			FAIL_Continue = 0;
			COM_port = 0;  
			rfoffset = 0;
			btoffset = 0;
			
			
			//init LabJack
			if(/*(LabJack_Handle == 0)&&*/(Test_Type <= 2)){
				Setup_DAQ(panel);
			//	EIALL_Off();
			}
			
			memset(test_time_2, '\0', sizeof(test_time_2)); 
			memset( Serial_Global, '\0', sizeof(Serial_Global));
			memset( IPEI_Global, '\0', sizeof(IPEI_Global));
			memset( MAC_Global, '\0', sizeof(MAC_Global));
			memset( MAC_Global_2, '\0', sizeof(MAC_Global_2));
			memset( Product_Serial_Global, '\0', sizeof(Product_Serial_Global));
			
			SetCtrlAttribute(panel,PANEL_STATUS, ATTR_CMD_BUTTON_COLOR, VAL_GRAY);
			SetCtrlAttribute (panel, PANEL_STATUS, ATTR_LABEL_TEXT, "TEST STATUS");
			
			ProcessSystemEvents ();
			if (Stop_Test_Exe == 1) break; 




			//read loss
			char  cRfoffset[10] = {0};
			rfoffset_cfg_File_Handle =    OpenFile ("rf_offset.cfg", VAL_READ_ONLY, VAL_OPEN_AS_IS, VAL_ASCII);
			status = ReadLine(rfoffset_cfg_File_Handle,cRfoffset,9);
			CloseFile(rfoffset_cfg_File_Handle);
			rfoffset = atof(cRfoffset);

			sprintf(msg,"cable loss: %s,%f",cRfoffset,rfoffset);
			WriteToTerminal(panel, msg);



			//read loss
			char  cBtoffset[10] = {0};
			btoffset_cfg_File_Handle =    OpenFile ("bt_offset.cfg", VAL_READ_ONLY, VAL_OPEN_AS_IS, VAL_ASCII);
			status = ReadLine(btoffset_cfg_File_Handle,cBtoffset,9);
			CloseFile(btoffset_cfg_File_Handle);

			btoffset = atof(cBtoffset);

			sprintf(msg,"bt cable loss: %s,%f",cBtoffset,btoffset);
			WriteToTerminal(panel, msg);






			
			tester_cfg_File_Handle = OpenFile ("tester.cfg", VAL_READ_ONLY, VAL_OPEN_AS_IS, VAL_ASCII); 
			status = ReadLine(tester_cfg_File_Handle,Station_ID,127);
			SetCtrlVal (panel, PANEL_STATION_ID,Station_ID); 
			
			
		
			
			
			status = ReadLine(tester_cfg_File_Handle,Network_Path,255);
			if (status > 0)
				network_enable = 1;


			status = ReadLine(tester_cfg_File_Handle,rf_EquipType,127);   // rf_EquipType = 0 - cmd60; 1- RTX
			irfEquipmentType = atoi(rf_EquipType);
			
			
			status = ReadLine(tester_cfg_File_Handle,mandownsensor,127);   // rf_EquipType = 0 - cmd60; 1- RTX
			imandownsensor = atoi(mandownsensor);
			
			

			CloseFile(tester_cfg_File_Handle);
			
			if(test_stage == 1)
			{
				SetCtrlVal (panel, PANEL_Mitel_Serial_Number," ");
				SetCtrlVal (panel, PANEL_Log_File_String," ");   			
 				ResetTextBox(panel,PANEL_Status_Window, "New Test Started\n");
			}
			
			SetCtrlAttribute (panel, PANEL_RING, ATTR_DIMMED, 1);
			SetCtrlAttribute (panel, PANEL_RING_2, ATTR_DIMMED, 1);
			SetCtrlAttribute (panel, PANEL_COMMANDBUTTON_2, ATTR_DIMMED, 1); 
		    SetCtrlAttribute (panel, PANEL_COMMANDBUTTON, ATTR_DIMMED, 1);
			SetCtrlAttribute (panel, PANEL_PB_STOP, ATTR_DIMMED, 0); 
			
			ProcessSystemEvents ();
			if (Stop_Test_Exe == 1) break; 
			
			// Get the serial number from the operator
			if((Test_Type == 0)||(Test_Type == 1)||(Test_Type == 2)){
				DECT_Set_RF_Board_Serial_Number(panel,control);
			}
			else if((Test_Type == 3)||(Test_Type == 4)||(Test_Type == 5)||(Test_Type == 6)||(Test_Type == 7)||(Test_Type == 8)){
				DECT_Set_RF_Final_Serial_Number(panel,control);
			}
			
			
			if(Fixture_Read == 0){
				sprintf(Scan_Final_Fixture_Serial,"Mitel");
			}
			
			if (((Test_Type == 0)||(Test_Type == 1)||(Test_Type == 2)||(Test_Type == 3)||(Test_Type == 4)||(Test_Type == 5)||(Test_Type == 6)||(Test_Type == 7)||(Test_Type == 8))&&(Fixture_Read == 0)){
				memset(Scan_Final_Fixture_Serial, '\0', sizeof(Scan_Final_Fixture_Serial));
				scan_serial_numbers = 1;
				Serial_Num_Read = 0;
				Set_Serial_Number(panel,control);
				scan_serial_numbers = 0;
				Fixture_Read = 1;
			}
			
			SetCtrlVal(panel,PANEL_FIXTURE_ID,Scan_Final_Fixture_Serial);
			
			sprintf(GUI_Revision,"2.0S");  
			
			//////////////////////////////////////////////////////
			//Check Prefixes
			get_prefix_code(panel);
			prefix_found = 0; 
			int n = 0;

			//Check Prefixes
			//612d
			if(((Test_Type == 0)||(Test_Type == 3)||(Test_Type == 6))&&(CARD_STATE != 1)){ 
				while(strcmp(MitelAssy612d[n], "END")){
	    			if ( strstr(MitelAssy612d[n], Product_Prefix)) {
						prefix_found = 1;
	        			break;
	    			} 
					n++;
				} 
			}
			
			//622d
			if(((Test_Type == 1)||(Test_Type == 4)||(Test_Type == 7))&&(CARD_STATE != 1)){ 
				while(strcmp(MitelAssy622d[n], "END")){
	    			if ( strstr(MitelAssy622d[n], Product_Prefix)) {
						prefix_found = 1;
	        			break;
	    			} 
					n++;
				} 
			}
			
			//632d
			if(((Test_Type == 2)||(Test_Type == 5)||(Test_Type == 8))&&(CARD_STATE != 1)){
				while(strcmp(MitelAssy632d[n], "END")){
	    			if ( strstr(MitelAssy632d[n], Product_Prefix)) {
						prefix_found = 1;
	        			break;
	    			} 
					n++;
				} 
			}
			
			
			if(prefix_found != 1){
				sprintf(temp_string, "Wrong_App"); 
				CARD_STATE = FAIL;
				SetCtrlAttribute (panel, PANEL_STATUS, ATTR_LABEL_TEXT, temp_string);
				Test_In_Process = 0;
				SetCtrlAttribute (panel, PANEL_COMMANDBUTTON, ATTR_DIMMED, 0);
				ProcessSystemEvents ();
				Stop_Test_Exe = 1;
				SetCtrlAttribute(panel,PANEL_STATUS, ATTR_CMD_BUTTON_COLOR, VAL_RED);
				print_TRACS_results(panel);
				sprintf(msg,"Wrong Product Code for Selected Application.");
				WriteToTerminal(panel, msg); 
				Write_Log_File(temp_string,panel);
				Close_Data_Files(panel);
				break;
			
			}
		
			///////////////////////////////////////////////////////			
			ResetTextBox(panel,PANEL_Serial_Window_1, "\n");
			if (Stop_Test_Exe == 1) break;		

			SetCtrlAttribute(panel,PANEL_STATUS, ATTR_CMD_BUTTON_COLOR, VAL_GRAY);
			
			if (Test_Type == 0){
				SetCtrlAttribute (panel, PANEL_STATUS, ATTR_LABEL_TEXT, "DECT 612d CCA TEST STARTED");
			}
			else if (Test_Type == 1){
				SetCtrlAttribute (panel, PANEL_STATUS, ATTR_LABEL_TEXT, "DECT 622d CCA TEST STARTED");
			}
			else if (Test_Type == 2){
				SetCtrlAttribute (panel, PANEL_STATUS, ATTR_LABEL_TEXT, "DECT 632d CCA TEST STARTED");
			}
			else if (Test_Type == 3){
				SetCtrlAttribute (panel, PANEL_STATUS, ATTR_LABEL_TEXT, "DECT 612d FINAL TEST STARTED");
			}
			else if (Test_Type == 4){
				SetCtrlAttribute (panel, PANEL_STATUS, ATTR_LABEL_TEXT, "DECT 622d FINAL TEST STARTED");
			}
			else if (Test_Type == 5){
				SetCtrlAttribute (panel, PANEL_STATUS, ATTR_LABEL_TEXT, "DECT 632d FINAL TEST STARTED");
			}
			else if (Test_Type == 6){
				SetCtrlAttribute (panel, PANEL_STATUS, ATTR_LABEL_TEXT, "DECT 612d SN Fix Started");
			}
			else if (Test_Type == 7){
				SetCtrlAttribute (panel, PANEL_STATUS, ATTR_LABEL_TEXT, "DECT 622d SN Fix Started");
			}
			else if (Test_Type == 8){
				SetCtrlAttribute (panel, PANEL_STATUS, ATTR_LABEL_TEXT, "DECT 632d SN Fix Started");
			}
			else if (Test_Type == 9){
				SetCtrlAttribute (panel, PANEL_STATUS, ATTR_LABEL_TEXT, "DECT ENHANCED TESTS STARTED UART");
			}
			else if (Test_Type == 10){
				SetCtrlAttribute (panel, PANEL_STATUS, ATTR_LABEL_TEXT, "DECT ENHANCED USER TESTS STARTED UART");
			}
			else if (Test_Type == 11){
				SetCtrlAttribute (panel, PANEL_STATUS, ATTR_LABEL_TEXT, "DECT ENHANCED VERSION TESTS STARTED UART");
			}
			


			ProcessSystemEvents ();
			if (Stop_Test_Exe == 1) break;
			
			ProcessSystemEvents ();	
				
			SetCtrlVal(panel,PANEL_Status_Window, "Creating Data Files\n"); 				

			if(!(Open_Files_For_Write(panel)))
			{
				SetCtrlVal(panel,PANEL_Status_Window, "Unable to Open Data Files\n");
				SetCtrlVal(panel,PANEL_Status_Window, "Terminating Tests\n"); 
				Test_In_Process = 0; 
				CARD_STATE = FAIL;
				print_TRACS_results(panel);				
				
				return 0;
			}
			ProcessSystemEvents ();	
			if (Stop_Test_Exe == 1) break;
			
			//init variables
			sequence_complete = 0;
			stage_done = 0;
			index = 0;
			temp_index = 0;
			Test_File_Handle = 0; 
			memset( Part_Number, '\0', sizeof(Part_Number)); 
			
			
			COM_port = Serial_Port_Scan (panel);
			
			if(((Test_Type == 3)||(Test_Type == 4)||(Test_Type == 5))&&(COM_port == 1)){ 
				COM_port = 0;
				Delay(5);
				COM_port = Serial_Port_Scan (panel); 
			}
			
			for (q = 1; q <= test_stages; q++)
			{
			
				test_stage = q;	
				

				ProcessSystemEvents ();	
				if (Stop_Test_Exe == 1) break;
				
				if (Test_Type == 0){ 
					status = Open_COM_PORT_1(panel);    
					SetCtrlVal(panel,PANEL_Status_Window, "Starting DECT 612d CCA Tests\n");
					strcat (Part_Number,"DECT_612d_CCA");
				}
				else if (Test_Type == 1){ 
					status = Open_COM_PORT_1(panel);   
					SetCtrlVal(panel,PANEL_Status_Window, "Starting DECT 622d CCA Tests\n");
					strcat (Part_Number,"DECT_622d_CCA");
				}
				else if (Test_Type == 2){ 
					status = Open_COM_PORT_1(panel);    
					SetCtrlVal(panel,PANEL_Status_Window, "Starting DECT 632d CCA Tests\n");
					strcat (Part_Number,"DECT_632d_CCA");
				}
				else if (Test_Type == 3){ 
					status = Open_COM_PORT_1(panel);      
					SetCtrlVal(panel,PANEL_Status_Window, "Starting DECT 612d FINAL Tests\n");
					strcat (Part_Number,"DECT_612d_FINAL");
				}
				else if (Test_Type == 4){ 
					status = Open_COM_PORT_1(panel);    
					SetCtrlVal(panel,PANEL_Status_Window, "Starting DECT 622d FINAL Tests\n");
					strcat (Part_Number,"DECT_622d_FINAL");
				}
				else if (Test_Type == 5){ 
					status = Open_COM_PORT_1(panel);      
					SetCtrlVal(panel,PANEL_Status_Window, "Starting DECT 632d FINAL Tests\n");
					strcat (Part_Number,"DECT_632d_FINAL");
				}
				else if (Test_Type == 6){ 
					status = Open_COM_PORT_1(panel);
					SetCtrlVal(panel,PANEL_Status_Window, "Starting DECT 612d SN FIX Tests\n");
					strcat (Part_Number,"DECT_ENHANCED");
				}
				else if (Test_Type == 7){ 
					status = Open_COM_PORT_1(panel);
					SetCtrlVal(panel,PANEL_Status_Window, "Starting DECT 622d SN FIX  Tests\n");
					strcat (Part_Number,"DECT_ENHANCED_USER");
				}
				else if (Test_Type == 8){ 
					status = Open_COM_PORT_1(panel);
					SetCtrlVal(panel,PANEL_Status_Window, "Starting DECT 632d SN FIX  Tests\n");
					strcat (Part_Number,"DECT_ENHANCED_VERSION");
				}
				else if (Test_Type == 9){ 
					status = Open_COM_PORT_1(panel);  
					SetCtrlVal(panel,PANEL_Status_Window, "Starting DECT ENHANCED Tests UART\n");
					strcat (Part_Number,"DECT_ENHANCED");
				}
				else if (Test_Type == 10){ 
					status = Open_COM_PORT_1(panel);     
					SetCtrlVal(panel,PANEL_Status_Window, "Starting DECT ENHANCED USER Tests UART\n");
					strcat (Part_Number,"DECT_ENHANCED_USER");
				}
				else if (Test_Type == 11){ 
					status = Open_COM_PORT_1(panel); 
					SetCtrlVal(panel,PANEL_Status_Window, "Starting DECT ENHANCED VERSION Tests UART\n");
					strcat (Part_Number,"DECT_ENHANCED_VERSION");
				}
	
	
				count = 1;
				
				memset( data_string, '\0', sizeof(data_string)); 
				
				//DECT CCA Tests
				if((Test_Type == 0)&&(CARD_STATE != 1)) 
				{
					sprintf(msg,"Use lever to lower test probes so they contact the UUT.");
					Display_OK_MSG(msg); 
					if(!(DECT_612d_CCA_Test_Sequence(panel, temp_string))) 
					{
						if (Stop_Test_Exe == 1) break;  
						CARD_STATE = FAIL;
						SetCtrlAttribute(panel,PANEL_STATUS, ATTR_CMD_BUTTON_COLOR, VAL_RED);
						SetCtrlAttribute (panel, PANEL_STATUS, ATTR_LABEL_TEXT, "TEST FAIL");
						Stop_Test_Exe = 1;
						ProcessSystemEvents ();	
						break;
					}
					else
					{
						if (Stop_Test_Exe == 1) break;  
					 	CARD_STATE = PASS;
					}
						
				}
				
				if((Test_Type == 1)&&(CARD_STATE != 1)) 
				{
					sprintf(msg,"Use lever to lower test probes so they contact the UUT.");
					Display_OK_MSG(msg); 
					if(!(DECT_622d_CCA_Test_Sequence(panel, temp_string))) 
					{
						if (Stop_Test_Exe == 1) break;  
						CARD_STATE = FAIL;
						SetCtrlAttribute(panel,PANEL_STATUS, ATTR_CMD_BUTTON_COLOR, VAL_RED);
						SetCtrlAttribute (panel, PANEL_STATUS, ATTR_LABEL_TEXT, "TEST FAIL");
						Stop_Test_Exe = 1;
						ProcessSystemEvents ();	
						break;
					}
					else
					{
						if (Stop_Test_Exe == 1) break;  
					 	CARD_STATE = PASS;
					}
						
				}
				
				if((Test_Type == 2)&&(CARD_STATE != 1)) 
				{
					sprintf(msg,"Use lever to lower test probes so they contact the UUT.");
					Display_OK_MSG(msg); 
					if(!(DECT_632d_CCA_Test_Sequence(panel, temp_string))) 
					{
						if (Stop_Test_Exe == 1) break;  
						CARD_STATE = FAIL;
						SetCtrlAttribute(panel,PANEL_STATUS, ATTR_CMD_BUTTON_COLOR, VAL_RED);
						SetCtrlAttribute (panel, PANEL_STATUS, ATTR_LABEL_TEXT, "TEST FAIL");
						Stop_Test_Exe = 1;
						ProcessSystemEvents ();	
						break;
					}
					else
					{
						if (Stop_Test_Exe == 1) break;  
					 	CARD_STATE = PASS;
					}
						
				}
				
				
				//DECT 612d FINAL Tests
				if((Test_Type == 3)&&(CARD_STATE != 1)) 
				{
					if(!(DECT_612d_Final_Test_Sequence(panel, temp_string))) 
					{
						if (Stop_Test_Exe == 1) break;  
						CARD_STATE = FAIL;
						SetCtrlAttribute(panel,PANEL_STATUS, ATTR_CMD_BUTTON_COLOR, VAL_RED);
						SetCtrlAttribute (panel, PANEL_STATUS, ATTR_LABEL_TEXT, "TEST FAIL");
						Stop_Test_Exe = 1;
						ProcessSystemEvents ();	
						break;
					}
					else
					{
						if (Stop_Test_Exe == 1) break;  
					 	CARD_STATE = PASS;
					}
						
				}
				
				//DECT 622d FINAL Tests
				if((Test_Type == 4)&&(CARD_STATE != 1)) 
				{
					if(!(DECT_622d_Final_Test_Sequence(panel, temp_string))) 
					{
						if (Stop_Test_Exe == 1) break;  
						CARD_STATE = FAIL;
						SetCtrlAttribute(panel,PANEL_STATUS, ATTR_CMD_BUTTON_COLOR, VAL_RED);
						SetCtrlAttribute (panel, PANEL_STATUS, ATTR_LABEL_TEXT, "TEST FAIL");
						Stop_Test_Exe = 1;
						ProcessSystemEvents ();	
						break;
					}
					else
					{
						if (Stop_Test_Exe == 1) break;  
					 	CARD_STATE = PASS;
					}
						
				}
				
				
				//DECT 632d FINAL Tests
				if((Test_Type == 5)&&(CARD_STATE != 1)) 
				{
					if(!(DECT_632d_Final_Test_Sequence(panel, temp_string))) 
					{
						if (Stop_Test_Exe == 1) break;  
						CARD_STATE = FAIL;
						SetCtrlAttribute(panel,PANEL_STATUS, ATTR_CMD_BUTTON_COLOR, VAL_RED);
						SetCtrlAttribute (panel, PANEL_STATUS, ATTR_LABEL_TEXT, "TEST FAIL");
						Stop_Test_Exe = 1;
						ProcessSystemEvents ();	
						break;
					}
					else
					{
						if (Stop_Test_Exe == 1) break;  
					 	CARD_STATE = PASS;
					}
						
				}
				
				
				//DECT ENHANCED Tests
				if((Test_Type == 6)&&(CARD_STATE != 1)) 
				{
					if(!(DECT_612d_Final_Serial_Number_Sequence(panel, temp_string))) 
					{
						if (Stop_Test_Exe == 1) break;  
						CARD_STATE = FAIL;
						SetCtrlAttribute(panel,PANEL_STATUS, ATTR_CMD_BUTTON_COLOR, VAL_RED);
						SetCtrlAttribute (panel, PANEL_STATUS, ATTR_LABEL_TEXT, "TEST FAIL");
						Stop_Test_Exe = 1;
						ProcessSystemEvents ();	
						break;
					}
					else
					{
						if (Stop_Test_Exe == 1) break;  
					 	CARD_STATE = PASS;
					}
						
				}
				
				
				if((Test_Type == 7)&&(CARD_STATE != 1)) 
				{
					if(!(DECT_622d_Final_Serial_Number_Sequence(panel, temp_string))) 
					{
						if (Stop_Test_Exe == 1) break;  
						CARD_STATE = FAIL;
						SetCtrlAttribute(panel,PANEL_STATUS, ATTR_CMD_BUTTON_COLOR, VAL_RED);
						SetCtrlAttribute (panel, PANEL_STATUS, ATTR_LABEL_TEXT, "TEST FAIL");
						Stop_Test_Exe = 1;
						ProcessSystemEvents ();	
						break;
					}
					else
					{
						if (Stop_Test_Exe == 1) break;  
					 	CARD_STATE = PASS;
					}
						
				}
				
				
				if((Test_Type == 8)&&(CARD_STATE != 1)) 
				{
					if(!(DECT_632d_Final_Serial_Number_Sequence(panel, temp_string))) 
					{
						if (Stop_Test_Exe == 1) break;  
						CARD_STATE = FAIL;
						SetCtrlAttribute(panel,PANEL_STATUS, ATTR_CMD_BUTTON_COLOR, VAL_RED);
						SetCtrlAttribute (panel, PANEL_STATUS, ATTR_LABEL_TEXT, "TEST FAIL");
						Stop_Test_Exe = 1;
						ProcessSystemEvents ();	
						break;
					}
					else
					{
						if (Stop_Test_Exe == 1) break;  
					 	CARD_STATE = PASS;
					}
						
				}
				
				
				//DECT ENHANCED Tests
				if((Test_Type == 9)&&(CARD_STATE != 1)) 
				{
					if(!(DECT_Enhanced_Test_Sequence_2(panel, temp_string))) 
					{
						if (Stop_Test_Exe == 1) break;  
						CARD_STATE = FAIL;
						SetCtrlAttribute(panel,PANEL_STATUS, ATTR_CMD_BUTTON_COLOR, VAL_RED);
						SetCtrlAttribute (panel, PANEL_STATUS, ATTR_LABEL_TEXT, "TEST FAIL");
						Stop_Test_Exe = 1;
						ProcessSystemEvents ();	
						break;
					}
					else
					{
						if (Stop_Test_Exe == 1) break;  
					 	CARD_STATE = PASS;
					}
						
				}
				
				
				if((Test_Type == 10)&&(CARD_STATE != 1)) 
				{
					if(!(DECT_Enhanced_User_Test_Sequence_2(panel, temp_string))) 
					{
						if (Stop_Test_Exe == 1) break;  
						CARD_STATE = FAIL;
						SetCtrlAttribute(panel,PANEL_STATUS, ATTR_CMD_BUTTON_COLOR, VAL_RED);
						SetCtrlAttribute (panel, PANEL_STATUS, ATTR_LABEL_TEXT, "TEST FAIL");
						Stop_Test_Exe = 1;
						ProcessSystemEvents ();	
						break;
					}
					else
					{
						if (Stop_Test_Exe == 1) break;  
					 	CARD_STATE = PASS;
					}
						
				}
				
				
				ProcessSystemEvents ();	
				if (Stop_Test_Exe == 1) break;
				SetCtrlVal(panel,PANEL_Status_Window, "End of Test Stage\n");
				test_stage  = test_stage + 1;
				sequence_complete = 0;
				stage_done = 0;
				index = 0;
				temp_index = 0;
			}  
			
			CloseFile(Test_File_Handle);
			CloseFile(debug_File_Handle); 
		    Test_File_Handle = 0;
			
			SetCtrlVal(panel,PANEL_Status_Window, "END of TEST\n");
			
			if(CARD_STATE == FAIL){
				SetCtrlAttribute(panel,PANEL_STATUS, ATTR_CMD_BUTTON_COLOR, VAL_RED);
				SetCtrlAttribute (panel, PANEL_STATUS, ATTR_LABEL_TEXT, "TEST FAIL");
				sprintf(msg, "FAIL,%s\n",temp_string);
				Write_Log_File(msg,panel); 
			}
			else if ((CARD_STATE == PASS)&&(Stop_Test_Exe == 0)){
				SetCtrlAttribute(panel,PANEL_STATUS, ATTR_CMD_BUTTON_COLOR, VAL_GREEN);
				SetCtrlAttribute (panel, PANEL_STATUS, ATTR_LABEL_TEXT, "TEST PASS");
				sprintf(msg, "PASS\n");
				Write_Log_File(msg,panel);
				
			}
			else if(Stop_Test_Exe == 1){
				SetCtrlAttribute(panel,PANEL_STATUS, ATTR_CMD_BUTTON_COLOR, VAL_GRAY);
				SetCtrlAttribute (panel, PANEL_STATUS, ATTR_LABEL_TEXT, "TEST ABORTED");
				sprintf(msg, "TEST ABORTED\n");
				Write_Log_File(msg,panel);
			}
			
			print_TRACS_results(panel);
		
			Close_Data_Files(panel);		// end of tests, clean up.		;
			
			
			Close_COM_Port_Relays ();
			EIALL_Off();
			Power_OFF_DECT ();

		
			Test_In_Process = 0;
			break;
		}
		
		}		
		// ***************** Clean up if terminated early ****************************  
		
		if (Test_In_Process == 1 && Stop_Test_Exe ==1 && Stopped == 0)
		{
			Stopped = 1;
			SetCtrlVal(panel,PANEL_Status_Window, "Terminating Tests\n");		
			Stop_In_Process = 0;
			Terminated_Early = 1;
			SetCtrlAttribute(panel,PANEL_STATUS, ATTR_CMD_BUTTON_COLOR, VAL_RED);
			if (Test_Type == 0){
				SetCtrlAttribute (panel, PANEL_STATUS, ATTR_LABEL_TEXT, "612d CCA TEST TERMINATED");
			}
			else if (Test_Type == 1){
				SetCtrlAttribute (panel, PANEL_STATUS, ATTR_LABEL_TEXT, "622d CCA TEST TERMINATED");
			}
			else if (Test_Type == 2){
				SetCtrlAttribute (panel, PANEL_STATUS, ATTR_LABEL_TEXT, "632d CCA TEST TERMINATED");
			}
			else if (Test_Type == 3){
				SetCtrlAttribute (panel, PANEL_STATUS, ATTR_LABEL_TEXT, "612d FINAL TEST TERMINATED");
			}
			else if (Test_Type == 4){
				SetCtrlAttribute (panel, PANEL_STATUS, ATTR_LABEL_TEXT, "622d FINAL TEST TERMINATED");
			}
			else if (Test_Type == 5){
				SetCtrlAttribute (panel, PANEL_STATUS, ATTR_LABEL_TEXT, "632d FINAL TEST TERMINATED");
			}
			else if (Test_Type == 6){
				SetCtrlAttribute (panel, PANEL_STATUS, ATTR_LABEL_TEXT, "ENHANCED TEST TERMINATED");
			}
			else if (Test_Type == 7){
				SetCtrlAttribute (panel, PANEL_STATUS, ATTR_LABEL_TEXT, "ENHANCED USER TEST TERMINATED");
			}
			else if (Test_Type == 8){
				SetCtrlAttribute (panel, PANEL_STATUS, ATTR_LABEL_TEXT, "ENHANCED VERSION TEST TERMINATED");
			}
			else if (Test_Type == 9){
				SetCtrlAttribute (panel, PANEL_STATUS, ATTR_LABEL_TEXT, "ENHANCED TEST 2 TERMINATED");
			}
			else if (Test_Type == 10){
				SetCtrlAttribute (panel, PANEL_STATUS, ATTR_LABEL_TEXT, "ENHANCED USER TEST 2 TERMINATED");
			}
			else if (Test_Type == 11){
				SetCtrlAttribute (panel, PANEL_STATUS, ATTR_LABEL_TEXT, "ENHANCED VERSION TEST 2 TERMINATED");
			}
		
		
			sprintf(msg, "TEST TERMINATED.....TEST TERMINATED\n");

			Write_Log_File(msg,panel); 
			print_TRACS_results(panel);		
			Close_Data_Files(panel);		// end of tests, clean up.			
 			CloseCom( COM_port); 
			Test_In_Process = 0;

			
		}


	if(Test_In_Process == 0){ 	
		SetCtrlAttribute (panel, PANEL_COMMANDBUTTON, ATTR_DIMMED, 0);  
		SetCtrlAttribute (panel, PANEL_RING, ATTR_DIMMED, 0);
		SetCtrlAttribute (panel, PANEL_RING_2, ATTR_DIMMED, 0);
		SetCtrlAttribute (panel, PANEL_PB_STOP, ATTR_DIMMED, 1);
		SetCtrlAttribute (panel, PANEL_COMMANDBUTTON_2, ATTR_DIMMED, 0); 
		activeCtrl = GetActiveCtrl(panel); 
		if(activeCtrl != 3){
			SetActiveCtrl (panel, PANEL_COMMANDBUTTON);
		}
	}
	
	return 0;
}




/*****************************************/
/*
/*
/*****************************************/
int CVICALLBACK PB_Cancel_Quit (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
		{
		case EVENT_LEFT_CLICK:
			HidePanel (QuitHandle);

			break;
		}
	return 0;
}

/*****************************************/
/*
/*
/*****************************************/
int CVICALLBACK PB_Quit_App (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
		{
		case EVENT_LEFT_CLICK:
			Stop_Test_Exe = 1;
			QuitUserInterface (0);
			HidePanel (QuitHandle);       
			break;
		}
	return 0;
}


/*****************************************/
/*
/*
/*****************************************/
int CVICALLBACK Quit (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
		{
		case EVENT_COMMIT:
				if ((QuitHandle = LoadPanel (0, "QUIT BUTTON.uir", PANEL)) < 0)
					return -1;
					DisplayPanel (QuitHandle);
					Aquarius_DC_Off(); 
					ProcessSystemEvents ();   
					
		}
	return 0;
}

/*****************************************/
/*
/*
/*****************************************/
int CVICALLBACK CONF_COM1 (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
		{
		case EVENT_LEFT_CLICK:
			COM_port = 1;
			break;
		}
	return 0;
}

/*****************************************/
/*
/*
/*****************************************/
int CVICALLBACK CONF_COM2 (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
		{
		case EVENT_LEFT_CLICK:
		    COM_port = 2;
			break;
		}
	return 0;
}



/*****************************************/
/*
/*
/*****************************************/
int CVICALLBACK Read_Serial_Number (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	char *head;
	
	switch (event)
		{
		ProcessSystemEvents ();
		if (Stop_Test_Exe == 1) break; 					
		
		case EVENT_COMMIT:

			
			if(scan_serial_numbers != 1){
				GetCtrlVal(panel, control, Scan_Serial);
				head = Scan_Serial;
			}
			else if(scan_serial_numbers == 1){
				GetCtrlVal(panel, control, Scan_Final_Fixture_Serial); 
				head = Scan_Final_Fixture_Serial;
			}
			
  			while (*head)
  			{
				//toupper(head);
  				if (!((*head >= '0' && *head <= '9') || 
  				(*head >= 'A' && *head <= 'Z') ||
  				(*head >= 'a' && *head <= 'z') ||(*head >= ';' )||(*head >= '-' )||(*head >= '*' ))){
					SetCtrlVal(Scan_Serial_Handle, control, ""); 
					return 0; // invalid character
				}
  				if (*head >= 'a') *head = *head - 0x20; // change to upper case
  				head++;
  		 
  			}
  
			if(Stop_Test_Exe == 1)
			return 0;
			Serial_Num_Read = 1; 
			HidePanel (Scan_Serial_Handle); 
			DiscardPanel (Scan_Serial_Handle);  
			break;
		}
	return 0;
}


/*****************************************/
/*
/*
/*****************************************/
int CVICALLBACK Read_MAC_Address (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	char *head;
	
	switch (event)
		{
		ProcessSystemEvents ();
		if (Stop_Test_Exe == 1) break; 					
		
		case EVENT_COMMIT:
			GetCtrlVal(panel, control, Scan_MAC);
			
  			head = Scan_MAC;
			  
  			//08-00-0f-12-34-56
  			if(((Scan_MAC[2] != 0x2D)&&(Scan_MAC[5] != 0x2D)&&(Scan_MAC[8] != 0x2D)&&(Scan_MAC[11] != 0x2D)&&(Scan_MAC[14] != 0x2D))&&((Scan_MAC[0] != 0x30)||(Scan_MAC[1] != 0x38)||(Scan_MAC[2] != 0x30)||(Scan_MAC[3] != 0x30)
			||(Scan_MAC[4] != 0x30)||(Scan_MAC[5] != 0x46))){
					return 0;
  			}
			
			if(Stop_Test_Exe == 1)
			return 0;
			MAC_Address_Read = 1; 
			HidePanel (Scan_MAC_Handle); 
			break;
		}
	return 0;
}


/*****************************************/
/*
/*
/*****************************************/
int CVICALLBACK Read_Test_Number (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	char *head;
	
	switch (event)
		{
		ProcessSystemEvents ();
		if (Stop_Test_Exe == 1) break; 					
		
		case EVENT_COMMIT:
			GetCtrlVal(panel, control, Scan_Test);
			
  			head = Scan_Serial;
  
  			while (*head)
  			{
				
				
				//toupper(head);
  				if (!((*head >= '0' && *head <= '9') || 
  				(*head >= 'A' && *head <= 'Z') ||
  				(*head >= 'a' && *head <= 'z') )) return 0; // invalid character
  				if (*head >= 'a') *head = *head - 0x20; // change to upper case
  				head++;
  		 
  			}
  

			
			if(Stop_Test_Exe == 1)
			return 0;
			Test_Num_Read = 1; 
			HidePanel (Scan_Test_Handle); 
			break;
		}
	return 0;
}

/*****************************************/
/*
/*
/*****************************************/

int CVICALLBACK Stop (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{

	switch (event)
		{
		case EVENT_LEFT_CLICK:
		
				if(Test_In_Process ==1)
				{
			 		Stop_Test_Exe = 1;
			 		Terminated_Early = 1;
			 		Stopped = 0;
			 		
				
				}	
				break;
		}
	return 0;
}

/*****************************************/
/*
/*
/*****************************************/
int CVICALLBACK PB_Stop_Test (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
		{
		case EVENT_LEFT_CLICK:
			 Stop_Test_Exe = 1;
			 Stop_In_Process = 1;
			 HidePanel (StopHandle); 
			break;
		}
	return 0;
}

/*****************************************/
/*
/*
/*****************************************/
int CVICALLBACK PB_Cancel_Stop (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
		{
		case EVENT_LEFT_CLICK:
			HidePanel (StopHandle); 
			DiscardPanel (StopHandle);
			Stop_In_Process = 0; 
			
			break;
		}
	return 0;
}

/*****************************************/
/*
/*
/*****************************************/
int CVICALLBACK Set_Test_Type (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_MOUSE_WHEEL_SCROLL:
			return 1;
		case EVENT_COMMIT:
			GetCtrlIndex(panel,PANEL_RING,&Test_Type);
			Fixture_Read = 0; 
			break;
	}
	return 0;
}



//*************************************************************
//* 
//*
//*************************************************************
int DECT_Set_RF_Board_Serial_Number(int panel,int control)
{
	
	int len,invalid_string,sn;
	int i;
	int serial_num_length;
	
	invalid_string = 1;
	
			
	while(invalid_string)
	{

		if ((Scan_Serial_Handle = LoadPanel (0, "Serial Number pnl.uir", PANEL)) < 0)
			return -1;
		
		if((Test_Type == 0)||(Test_Type == 1)||(Test_Type == 2)||(Test_Type == 3)||(Test_Type == 4)||(Test_Type == 5)||(Test_Type == 6)||(Test_Type == 7)||(Test_Type == 8)||(Test_Type == 9)||(Test_Type == 10)||(Test_Type == 11)){
			SetCtrlAttribute (Scan_Serial_Handle, PANEL_Scan_Serial_Text, ATTR_LABEL_TEXT, "Scan Serial Number:");  
		}
		
		DisplayPanel (Scan_Serial_Handle);
 		SetCtrlVal(Scan_Serial_Handle, control, "");
	
	
		while(Serial_Num_Read == 0)
		{
			ProcessSystemEvents ();
			SetActiveCtrl (panel, PANEL_Mitel_Serial_Number); 
			if (Stop_Test_Exe == 1)
			{
				HidePanel(Scan_Serial_Handle);
				DiscardPanel(Scan_Serial_Handle);
				break; 					
			}
		}
		ProcessSystemEvents ();
		if (Stop_Test_Exe == 1)
		{
			HidePanel(Scan_Serial_Handle);
			DiscardPanel(Scan_Serial_Handle);
			break; 					
		}
		
		
		
		if((Test_Type == 0)||(Test_Type == 3)){
			if (strlen(Scan_Serial) != 37){
				SetCtrlVal(Scan_Serial_Handle, control, "");
				SetCtrlVal(panel,PANEL_Status_Window,"INVALID SERIAL\n");
				SetCtrlVal(Scan_Serial_Handle,PANEL_Scan_Serial_Text,"");
				Serial_Num_Read =0;
			}
		}
		else if ((Test_Type == 1)||(Test_Type == 2)||(Test_Type == 4)||(Test_Type == 5)||(Test_Type == 6)||(Test_Type == 7)||(Test_Type == 8)||(Test_Type == 9)||(Test_Type == 10)||(Test_Type == 11)){
			if (strlen(Scan_Serial) != 50){
				SetCtrlVal(Scan_Serial_Handle, control, "");
				SetCtrlVal(panel,PANEL_Status_Window,"INVALID SERIAL\n");
				SetCtrlVal(Scan_Serial_Handle,PANEL_Scan_Serial_Text,"");
				Serial_Num_Read =0;
			}
		}
		
		if(Serial_Num_Read == 1){
		
			i = 0;
		
			while(Scan_Serial[i] != 59){
				Serial_Global[i] = Scan_Serial[i];
				i++;	
			}
			SetCtrlVal (panel, PANEL_CM_Serial_Number,Serial_Global);
		
			i++;
			while(Scan_Serial[i] != 59){
				IPEI_Global[i-11] = Scan_Serial[i];
				i++;	
			}
			SetCtrlVal (panel, PANEL_IPEI_Number,IPEI_Global);
		
			if((Test_Type != 0)&&(Test_Type != 3)){
				i++;
				while(Scan_Serial[i] != 59){
					MAC_Global[i-22] = Scan_Serial[i];
					i++;	
				}
			
				SetCtrlVal (panel, PANEL_MAC_Address,MAC_Global); 
			
				i++;
				while(Scan_Serial[i] != NULL){
					Product_Serial_Global[i-35] = Scan_Serial[i];
					i++;	
				}
			
				SetCtrlVal (panel, PANEL_Mitel_Serial_Number,Product_Serial_Global); 
			
			}
			else if(Test_Type == 0){
				i++;
				while(Scan_Serial[i] != NULL){
					Product_Serial_Global[i-22] = Scan_Serial[i];
					i++;	
				}
			
				SetCtrlVal (panel, PANEL_Mitel_Serial_Number,Product_Serial_Global);
		
			}
			
			invalid_string = 0; 
		}
	
	}

return 1;

}



//*************************************************************
//* 
//*
//*************************************************************
int DECT_Set_RF_Final_Serial_Number(int panel,int control)
{
	
	int len,invalid_string,sn;
	int i;
	int serial_num_length;
	
	invalid_string = 1;
	
			
	while(invalid_string)
	{

		if ((Scan_Serial_Handle = LoadPanel (0, "Serial Number pnl.uir", PANEL)) < 0)
			return -1;
		
		if((Test_Type == 3)||(Test_Type == 4)||(Test_Type == 5)){
			SetCtrlAttribute (Scan_Serial_Handle, PANEL_Scan_Serial_Text, ATTR_LABEL_TEXT, "Scan Serial Number:");  
		}
		
		DisplayPanel (Scan_Serial_Handle);
 		SetCtrlVal(Scan_Serial_Handle, control, "");
	
	
		while(Serial_Num_Read == 0)
		{
			ProcessSystemEvents ();
			SetActiveCtrl (panel, PANEL_Mitel_Serial_Number); 
			if (Stop_Test_Exe == 1)
			{
				HidePanel(Scan_Serial_Handle);
				DiscardPanel(Scan_Serial_Handle);
				break; 					
			}
		}
		ProcessSystemEvents ();
		if (Stop_Test_Exe == 1)
		{
			HidePanel(Scan_Serial_Handle);
			DiscardPanel(Scan_Serial_Handle);
			break; 					
		}
		
		
		
		/*if((Test_Type == 0)||(Test_Type == 3)){
			if (strlen(Scan_Serial) != 37){
				SetCtrlVal(Scan_Serial_Handle, control, "");
				SetCtrlVal(panel,PANEL_Status_Window,"INVALID SERIAL\n");
				SetCtrlVal(Scan_Serial_Handle,PANEL_Scan_Serial_Text,"");
				Serial_Num_Read =0;
			}
		}*/
		
		if ((Test_Type == 3)||(Test_Type == 4)||(Test_Type == 5)||(Test_Type == 6)||(Test_Type == 7)||(Test_Type == 8)){
			if ((strlen(Scan_Serial) != 28)&&(strlen(Scan_Serial) != 55)&&(strlen(Scan_Serial) != 42)){
				SetCtrlVal(Scan_Serial_Handle, control, "");
				SetCtrlVal(panel,PANEL_Status_Window,"INVALID SERIAL\n");
				SetCtrlVal(Scan_Serial_Handle,PANEL_Scan_Serial_Text,"");
				Serial_Num_Read =0;
			}
		}
		
		
		if(strlen(Scan_Serial) == 28){
		
			if(Serial_Num_Read == 1){
		
				i = 0;
		
				while(Scan_Serial[i] != 59){
					Serial_Global[i] = Scan_Serial[i];
					i++;	
				}
				SetCtrlVal (panel, PANEL_CM_Serial_Number,Serial_Global);
		
				i++;
				while(Scan_Serial[i] != NULL){
					Product_Serial_Global[i-13] = Scan_Serial[i];
					i++;	
				}
			
				SetCtrlVal (panel, PANEL_Mitel_Serial_Number,Product_Serial_Global); 
			
				}
			
				invalid_string = 0; 
		}

		else if (strlen(Scan_Serial) == 55){
			long_form_final = 1;
			
			i = 0;
		
			while(Scan_Serial[i] != 59){
				Serial_Global[i] = Scan_Serial[i];
				i++;	
			}
			SetCtrlVal (panel, PANEL_CM_Serial_Number,Serial_Global);
		
			i++;
			while(Scan_Serial[i] != 59){
				IPEI_Global[i-13] = Scan_Serial[i];
				i++;	
			}
			SetCtrlVal (panel, PANEL_IPEI_Number,IPEI_Global);
		
			if((Test_Type != 0)&&(Test_Type != 3)){
				i++;
				while(Scan_Serial[i] != 59){
					MAC_Global[i-27] = Scan_Serial[i];
					i++;	
				}
			
				SetCtrlVal (panel, PANEL_MAC_Address,MAC_Global); 
			
				i++;
				while(Scan_Serial[i] != NULL){
					Product_Serial_Global[i-40] = Scan_Serial[i];
					i++;	
				}
			
				SetCtrlVal (panel, PANEL_Mitel_Serial_Number,Product_Serial_Global); 
			
			}
			/*else if((Test_Type == 3)){
				i++;
				while(Scan_Serial[i] != NULL){
					Product_Serial_Global[i-27] = Scan_Serial[i];
					i++;	
				}
			
				SetCtrlVal (panel, PANEL_Mitel_Serial_Number,Product_Serial_Global);
		
			}*/
			
			invalid_string = 0; 
			
			}
		else if (strlen(Scan_Serial) == 42){
			long_form_final = 1;
			
			i = 0;
		
			while(Scan_Serial[i] != 59){
				Serial_Global[i] = Scan_Serial[i];
				i++;	
			}
			SetCtrlVal (panel, PANEL_CM_Serial_Number,Serial_Global);
		
			i++;
			while(Scan_Serial[i] != 59){
				IPEI_Global[i-13] = Scan_Serial[i];
				i++;	
			}
			SetCtrlVal (panel, PANEL_IPEI_Number,IPEI_Global);
		
			i++;
			while(Scan_Serial[i] != NULL){
				Product_Serial_Global[i-27] = Scan_Serial[i];
				i++;	
			}
			
			SetCtrlVal (panel, PANEL_Mitel_Serial_Number,Product_Serial_Global); 
			
			invalid_string = 0; 
			
			}
			
			
		}
		
//63-001646-00;2GRUC18010090EW

return 1;

}


//*************************************************************
//* 
//*
//*************************************************************
/*int Set_Serial_Number(int panel,int control)
{
	
	int len,invalid_string,sn;
	
	invalid_string = 1;
			
	while(invalid_string)
	{
		if ((Scan_Serial_Handle = LoadPanel (0, "Serial Number pnl.uir", PANEL)) < 0)
			return -1;
		DisplayPanel (Scan_Serial_Handle);
 		SetCtrlVal(Scan_Serial_Handle, control, "");
				
		while(Serial_Num_Read == 0)
		{
			ProcessSystemEvents ();
			SetActiveCtrl (panel, PANEL_Mitel_Serial_Number); 
			if (Stop_Test_Exe == 1)
			{
				HidePanel(Scan_Serial_Handle);
				DiscardPanel(Scan_Serial_Handle);
				break; 					
			}
		}
		ProcessSystemEvents ();
		if (Stop_Test_Exe == 1)
		{
			HidePanel(Scan_Serial_Handle);
			DiscardPanel(Scan_Serial_Handle);
			break; 					
		}
				
		sn=0;
		GetCtrlVal(panel, PANEL_RING_2,&sn);
		if (((strlen(Scan_Serial) == 12)||(strlen(Scan_Serial) == 15))&&(Scan_Serial[0] != 0x32))
		{
			SetCtrlVal(Scan_Serial_Handle, control, "");
			SetCtrlVal(panel,PANEL_Status_Window,"INVALID SERIAL\n");
			SetCtrlVal(Scan_Serial_Handle,PANEL_Scan_Serial_Text,"");
			Serial_Num_Read =0;
		}
		else if(len = strlen(Scan_Serial) == sn)
		{
			invalid_string = 0;
			SetCtrlVal(panel,PANEL_Status_Window,"Serial Number Entered\n");
			SetCtrlVal(panel,PANEL_Mitel_Serial_Number,Scan_Serial);
			//Check for Branding and set Flag
			ProcessSystemEvents ();  
		}
		else
		{
			SetCtrlVal(Scan_Serial_Handle, control, "");
			SetCtrlVal(panel,PANEL_Status_Window,"INVALID SERIAL\n");
			SetCtrlVal(Scan_Serial_Handle,PANEL_Scan_Serial_Text,"");
			Serial_Num_Read =0;
		}
		
		ProcessSystemEvents (); 				
		if (Stop_Test_Exe == 1) break; 					
	}

return 1;

}*/


// *************************************************************
// int Set_Serial_Number(int panel,int control) 
// 
// *************************************************************
int Set_Serial_Number(int panel,int control)
{
	
	int len,invalid_string,sn;
	
			
	// Get the card serial number from the operator 
	invalid_string = 1;
			
	while(invalid_string)
	{
		if ((Scan_Serial_Handle = LoadPanel (0, "Serial Number pnl.uir", PANEL)) < 0)
			return -1;
		
		
		if(scan_serial_numbers == 0){
			SetCtrlAttribute (Scan_Serial_Handle, PANEL_Scan_Serial_Text, ATTR_LABEL_TEXT, "Enter Serial Number:");  
		}
		else if (scan_serial_numbers == 1){
			SetCtrlAttribute (Scan_Serial_Handle, PANEL_Scan_Serial_Text, ATTR_LABEL_TEXT, "Enter Fixture Number:"); 
		}
		
		
		DisplayPanel (Scan_Serial_Handle);
 		SetCtrlVal(Scan_Serial_Handle, control, "");
				
		while(Serial_Num_Read == 0)
		{
			ProcessSystemEvents ();
			//SetActiveCtrl (panel, PANEL_Serial_Number); 
			if (Stop_Test_Exe == 1)
			{
				HidePanel(Scan_Serial_Handle);
				DiscardPanel(Scan_Serial_Handle);
				break; 					
			}
		}
		ProcessSystemEvents ();
		if (Stop_Test_Exe == 1)
		{
			HidePanel(Scan_Serial_Handle);
			DiscardPanel(Scan_Serial_Handle);
			break; 					
		}
				
		sn=0;
		GetCtrlVal(panel, PANEL_RING_2,&sn);
		
		if (scan_serial_numbers == 1){
			//sn = 9;
			if ((Scan_Final_Fixture_Serial[0] != 67)&&(Scan_Final_Fixture_Serial[0] != 70)&&(Scan_Final_Fixture_Serial[0] != 77)&&(Scan_Final_Fixture_Serial[0] != 74))		 //Celestica = C //Flextronics = F  //Mitel = M   //Jabil = J
			{
				SetCtrlVal(Scan_Serial_Handle, control, "");
				SetCtrlVal(panel,PANEL_Status_Window,"INVALID SERIAL\n");
				SetCtrlVal(Scan_Serial_Handle,PANEL_Scan_Serial_Text,"");
				Serial_Num_Read =0;
			}
			else if((len = strlen(Scan_Final_Fixture_Serial) >= 5)||(len = strlen(Scan_Final_Fixture_Serial) <= 15))
			{
				invalid_string = 0;
				SetCtrlVal(panel,PANEL_Status_Window,"Serial Number Entered\n");
				SetCtrlVal(panel,PANEL_FIXTURE_ID,Scan_Final_Fixture_Serial);
				//Check for Branding and set Flag
				ProcessSystemEvents ();  
			}
			else
			{
				SetCtrlVal(Scan_Serial_Handle, control, "");
				SetCtrlVal(panel,PANEL_Status_Window,"INVALID SERIAL\n");
				SetCtrlVal(Scan_Serial_Handle,PANEL_Scan_Serial_Text,"");
				Serial_Num_Read =0;
			}
		}
	
	
		ProcessSystemEvents (); 				
		if (Stop_Test_Exe == 1) break; 					
	}

return 1;

}

/*****************************************/
/*
/*
/*****************************************/
int Set_MAC_Address(int panel,int control)
{
	
	int len,invalid_string,sn;
	
			
	/* Get the card serial number from the operator */
	invalid_string = 1;
			
	while(invalid_string)
	{
		if ((Scan_MAC_Handle = LoadPanel (0, "MAC pnl.uir", PANEL)) < 0)
			return -1;
		DisplayPanel (Scan_MAC_Handle);
 		SetCtrlVal(Scan_MAC_Handle, control, "");
								
	
		while(MAC_Address_Read == 0)		
		{
			ProcessSystemEvents ();
			if (Stop_Test_Exe == 1)
			{
				HidePanel(Scan_MAC_Handle);
				break; 					
			}
		}
		ProcessSystemEvents ();
		if (Stop_Test_Exe == 1)
		{
			HidePanel(Scan_MAC_Handle);
			break; 					
		}
				
		sn=0;
	
		if(len = strlen(Scan_MAC) == 17)
		{
			invalid_string = 0;
			SetCtrlVal(panel,PANEL_Status_Window,"MAC Address Entered\n");
			//SetCtrlVal(panel,PANEL_Mac_Address,Scan_MAC);
			ProcessSystemEvents ();  
		}
		else
		{
			SetCtrlVal(Scan_MAC_Handle, control, "");
			SetCtrlVal(panel,PANEL_Status_Window,"INVALID MAC\n");
			SetCtrlVal(Scan_MAC_Handle,PANEL_Scan_MAC_Text,"");
			MAC_Address_Read =0;
		}
		ProcessSystemEvents (); 				
		if (Stop_Test_Exe == 1) break; 					
	}

return 1;

}


/*****************************************/
/*
/*
/*****************************************/
int Set_Test_Number(int panel,int control)
{
	
	int len,invalid_string,sn;
	
			
	/* Get the card serial number from the operator */
	invalid_string = 1;
			
	while(invalid_string)
	{
		if ((Scan_Test_Handle = LoadPanel (0, "Test Number pnl.uir", PANEL)) < 0)
			return -1;
		DisplayPanel (Scan_Test_Handle);
 		SetCtrlVal(Scan_Test_Handle, control, "");
				
		while(Test_Num_Read == 0)
		{
			ProcessSystemEvents ();
			if (Stop_Test_Exe == 1)
			{
				HidePanel(Scan_Test_Handle);
				break; 					
			}
		}
		ProcessSystemEvents ();
		if (Stop_Test_Exe == 1)
		{
			HidePanel(Scan_Test_Handle);
			break; 					
		}
				
		invalid_string = 0;
		SetCtrlVal(panel,PANEL_Status_Window,"Test Number Entered\n");
		ProcessSystemEvents ();  
		if (Stop_Test_Exe == 1) break; 					
	}

return 1;

}


/*****************************************/
/*
/*
/*****************************************/
int Test_Stage_Start(int panel,int control, int test_stage)
{
	
	if (test_stage == 1)
	{
	    SetCtrlAttribute (panel, PANEL_RING, ATTR_DIMMED, 1);
	    SetCtrlAttribute (panel, PANEL_RING_2, ATTR_DIMMED, 1);
	}
	else if (test_stage == 2)
	{

	}
	else if (test_stage == 3)
	{		   
 
	}
	else if (test_stage == 4)
	{

	}

	return 1;
	
}


/*****************************************/
/*
/*
/*****************************************/
int Open_COM_PORT_1(int panel)
{

			if(COM_port == 1){
				if (!(OpenComConfig (1, "COM1", 115200, 0, 8, 1, 10*4096, 4096) == 0)){
					SetCtrlVal(panel,PANEL_Status_Window, "Unable to open COM port\n");
					Test_In_Process = 0;
					return 0;
				}
			}
			else if(COM_port == 2){
				if (!(OpenComConfig (2, "COM2", 115200, 0, 8, 1, 10*4096, 4096) == 0)){
					SetCtrlVal(panel,PANEL_Status_Window, "Unable to open COM port\n");
					Test_In_Process = 0;
					return 0;
				}
			}
			else if(COM_port == 3){
				if (!(OpenComConfig (3, "COM3", 115200, 0, 8, 1, 10*4096, 4096) == 0)){
					SetCtrlVal(panel,PANEL_Status_Window, "Unable to open COM port\n");
					Test_In_Process = 0;
					return 0;
				}
			}
			else if(COM_port == 4){
				if (!(OpenComConfig (4, "COM4", 115200, 0, 8, 1, 10*4096, 4096) == 0)){
					SetCtrlVal(panel,PANEL_Status_Window, "Unable to open COM port\n");
					Test_In_Process = 0;
					return 0;
				}
			}
			else if(COM_port == 5){
				if (!(OpenComConfig (5, "COM5", 115200, 0, 8, 1, 10*4096, 4096) == 0)){
					SetCtrlVal(panel,PANEL_Status_Window, "Unable to open COM port\n");
					Test_In_Process = 0;
					return 0;
				}
			}
			else if(COM_port == 6){
				if (!(OpenComConfig (6, "COM6", 115200, 0, 8, 1, 10*4096, 4096) == 0)){
					SetCtrlVal(panel,PANEL_Status_Window, "Unable to open COM port\n");
					Test_In_Process = 0;
					return 0;
				}
			}
			else if(COM_port == 7){
				if (!(OpenComConfig (7, "COM7", 115200, 0, 8, 1, 10*4096, 4096) == 0)){
					SetCtrlVal(panel,PANEL_Status_Window, "Unable to open COM port\n");
					Test_In_Process = 0;
					return 0;
				}
			}
			else if(COM_port == 8){
				if (!(OpenComConfig (8, "COM8", 115200, 0, 8, 1, 10*4096, 4096) == 0)){
					SetCtrlVal(panel,PANEL_Status_Window, "Unable to open COM port\n");
					Test_In_Process = 0;
					return 0;
				}
			}
			else if(COM_port == 9){
				if (!(OpenComConfig (9, "COM9", 115200, 0, 8, 1, 10*4096, 4096) == 0)){
					SetCtrlVal(panel,PANEL_Status_Window, "Unable to open COM port\n");
					Test_In_Process = 0;
					return 0;
				}
			}
			else if(COM_port == 10){
				if (!(OpenComConfig (10, "COM10", 115200, 0, 8, 1, 10*4096, 4096) == 0)){
					SetCtrlVal(panel,PANEL_Status_Window, "Unable to open COM port\n");
					Test_In_Process = 0;
					return 0;
				}
			}
			else if(COM_port == 11){
				if (!(OpenComConfig (11, "COM11", 115200, 0, 8, 1, 10*4096, 4096) == 0)){
					SetCtrlVal(panel,PANEL_Status_Window, "Unable to open COM port\n");
					Test_In_Process = 0;
					return 0;
				}
			}
			else if(COM_port == 12){
				if (!(OpenComConfig (12, "COM12", 115200, 0, 8, 1, 10*4096, 4096) == 0)){
					SetCtrlVal(panel,PANEL_Status_Window, "Unable to open COM port\n");
					Test_In_Process = 0;
					return 0;
				}
			}
			else if(COM_port == 13){
				if (!(OpenComConfig (13, "COM13", 115200, 0, 8, 1, 10*4096, 4096) == 0)){
					SetCtrlVal(panel,PANEL_Status_Window, "Unable to open COM port\n");
					Test_In_Process = 0;
					return 0;
				}
			}
			else if(COM_port == 14){
				if (!(OpenComConfig (14, "COM14", 115200, 0, 8, 1, 10*4096, 4096) == 0)){
					SetCtrlVal(panel,PANEL_Status_Window, "Unable to open COM port\n");
					Test_In_Process = 0;
					return 0;
				}
			}
			else if(COM_port == 15){
				if (!(OpenComConfig (15, "COM15", 115200, 0, 8, 1, 10*4096, 4096) == 0)){
					SetCtrlVal(panel,PANEL_Status_Window, "Unable to open COM port\n");
					Test_In_Process = 0;
					return 0;
				}
			}
			else if(COM_port == 16){
				if (!(OpenComConfig (16, "COM16", 115200, 0, 8, 1, 10*4096, 4096) == 0)){
					SetCtrlVal(panel,PANEL_Status_Window, "Unable to open COM port\n");
					Test_In_Process = 0;
					return 0;
				}
			}
			else if(COM_port == 17){
				if (!(OpenComConfig (17, "COM17", 115200, 0, 8, 1, 10*4096, 4096) == 0)){
					SetCtrlVal(panel,PANEL_Status_Window, "Unable to open COM port\n");
					Test_In_Process = 0;
					return 0;
				}
			}
			else if(COM_port == 18){
				if (!(OpenComConfig (18, "COM18", 115200, 0, 8, 1, 10*4096, 4096) == 0)){
					SetCtrlVal(panel,PANEL_Status_Window, "Unable to open COM port\n");
					Test_In_Process = 0;
					return 0;
				}
			}
			else if(COM_port == 19){
				if (!(OpenComConfig (19, "COM19", 115200, 0, 8, 1, 10*4096, 4096) == 0)){
					SetCtrlVal(panel,PANEL_Status_Window, "Unable to open COM port\n");
					Test_In_Process = 0;
					return 0;
				}
			}
			else if(COM_port == 20){
				if (!(OpenComConfig (20, "COM20", 115200, 0, 8, 1, 10*4096, 4096) == 0)){
					SetCtrlVal(panel,PANEL_Status_Window, "Unable to open COM port\n");
					Test_In_Process = 0;
					return 0;
				}
			}

			else if(COM_port == 0){
				return 1;
			}
			if (!(SetXMode (COM_port, 0)==0)){
				SetCtrlVal(panel,PANEL_Status_Window, "Unable to open COM port\n");
				Test_In_Process = 0;			
				return 0;
			}
			if(!(SetCTSMode (COM_port, LWRS_HWHANDSHAKE_OFF) == 0)){
				SetCtrlVal(panel,PANEL_Status_Window, "Unable to open COM port\n");
				Test_In_Process = 0;			
				return 0;
			}
			if(!( SetComTime (COM_port, 0.25) == 0)){
				SetCtrlVal(panel,PANEL_Status_Window, "Unable to open COM port\n");
				Test_In_Process = 0;			
				return 0;
			}

return 1;
}

/*****************************************/
/*
/*
/*****************************************/
int Open_COM_PORT_2(int panel)
{

			if(COM_port_2 == 1){
				if (!(OpenComConfig (1, "COM1", 115200, 0, 8, 1, 10*4096, 4096) == 0)){
					SetCtrlVal(panel,PANEL_Status_Window, "Unable to open COM port\n");
					Test_In_Process = 0;
					return 0;
				}
			}
			else if(COM_port_2 == 2){
				if (!(OpenComConfig (2, "COM2", 115200, 0, 8, 1, 10*4096, 4096) == 0)){
					SetCtrlVal(panel,PANEL_Status_Window, "Unable to open COM port\n");
					Test_In_Process = 0;
					return 0;
				}
			}
			else if(COM_port_2 == 3){
				if (!(OpenComConfig (3, "COM3", 115200, 0, 8, 1, 10*4096, 4096) == 0)){
					SetCtrlVal(panel,PANEL_Status_Window, "Unable to open COM port\n");
					Test_In_Process = 0;
					return 0;
				}
			}
			else if(COM_port_2 == 4){
				if (!(OpenComConfig (4, "COM4", 115200, 0, 8, 1, 10*4096, 4096) == 0)){
					SetCtrlVal(panel,PANEL_Status_Window, "Unable to open COM port\n");
					Test_In_Process = 0;
					return 0;
				}
			}
			else if(COM_port_2 == 5){
				if (!(OpenComConfig (5, "COM5", 115200, 0, 8, 1, 10*4096, 4096) == 0)){
					SetCtrlVal(panel,PANEL_Status_Window, "Unable to open COM port\n");
					Test_In_Process = 0;
					return 0;
				}
			}
			else if(COM_port_2 == 6){
				if (!(OpenComConfig (6, "COM6", 115200, 0, 8, 1, 10*4096, 4096) == 0)){
					SetCtrlVal(panel,PANEL_Status_Window, "Unable to open COM port\n");
					Test_In_Process = 0;
					return 0;
				}
			}
			else if(COM_port_2 == 7){
				if (!(OpenComConfig (7, "COM7", 115200, 0, 8, 1, 10*4096, 4096) == 0)){
					SetCtrlVal(panel,PANEL_Status_Window, "Unable to open COM port\n");
					Test_In_Process = 0;
					return 0;
				}
			}
			else if(COM_port_2 == 8){
				if (!(OpenComConfig (8, "COM8", 115200, 0, 8, 1, 10*4096, 4096) == 0)){
					SetCtrlVal(panel,PANEL_Status_Window, "Unable to open COM port\n");
					Test_In_Process = 0;
					return 0;
				}
			}
			else if(COM_port_2 == 9){
				if (!(OpenComConfig (9, "COM9", 115200, 0, 8, 1, 10*4096, 4096) == 0)){
					SetCtrlVal(panel,PANEL_Status_Window, "Unable to open COM port\n");
					Test_In_Process = 0;
					return 0;
				}
			}
			else if(COM_port_2 == 10){
				if (!(OpenComConfig (10, "COM10", 115200, 0, 8, 1, 10*4096, 4096) == 0)){
					SetCtrlVal(panel,PANEL_Status_Window, "Unable to open COM port\n");
					Test_In_Process = 0;
					return 0;
				}
			}
			else if(COM_port_2 == 11){
				if (!(OpenComConfig (11, "COM11", 115200, 0, 8, 1, 10*4096, 4096) == 0)){
					SetCtrlVal(panel,PANEL_Status_Window, "Unable to open COM port\n");
					Test_In_Process = 0;
					return 0;
				}
			}
			else if(COM_port_2 == 12){
				if (!(OpenComConfig (12, "COM12", 115200, 0, 8, 1, 10*4096, 4096) == 0)){
					SetCtrlVal(panel,PANEL_Status_Window, "Unable to open COM port\n");
					Test_In_Process = 0;
					return 0;
				}
			}
			else if(COM_port_2 == 13){
				if (!(OpenComConfig (13, "COM13", 115200, 0, 8, 1, 10*4096, 4096) == 0)){
					SetCtrlVal(panel,PANEL_Status_Window, "Unable to open COM port\n");
					Test_In_Process = 0;
					return 0;
				}
			}
			else if(COM_port_2 == 14){
				if (!(OpenComConfig (14, "COM14", 115200, 0, 8, 1, 10*4096, 4096) == 0)){
					SetCtrlVal(panel,PANEL_Status_Window, "Unable to open COM port\n");
					Test_In_Process = 0;
					return 0;
				}
			}
			else if(COM_port_2 == 15){
				if (!(OpenComConfig (15, "COM15", 115200, 0, 8, 1, 10*4096, 4096) == 0)){
					SetCtrlVal(panel,PANEL_Status_Window, "Unable to open COM port\n");
					Test_In_Process = 0;
					return 0;
				}
			}
			else if(COM_port_2 == 16){
				if (!(OpenComConfig (16, "COM16", 115200, 0, 8, 1, 10*4096, 4096) == 0)){
					SetCtrlVal(panel,PANEL_Status_Window, "Unable to open COM port\n");
					Test_In_Process = 0;
					return 0;
				}
			}
			else if(COM_port_2 == 17){
				if (!(OpenComConfig (17, "COM17", 115200, 0, 8, 1, 10*4096, 4096) == 0)){
					SetCtrlVal(panel,PANEL_Status_Window, "Unable to open COM port\n");
					Test_In_Process = 0;
					return 0;
				}
			}
			else if(COM_port_2 == 18){
				if (!(OpenComConfig (18, "COM18", 115200, 0, 8, 1, 10*4096, 4096) == 0)){
					SetCtrlVal(panel,PANEL_Status_Window, "Unable to open COM port\n");
					Test_In_Process = 0;
					return 0;
				}
			}
			else if(COM_port_2 == 19){
				if (!(OpenComConfig (19, "COM19", 115200, 0, 8, 1, 10*4096, 4096) == 0)){
					SetCtrlVal(panel,PANEL_Status_Window, "Unable to open COM port\n");
					Test_In_Process = 0;
					return 0;
				}
			}
			else if(COM_port_2 == 20){
				if (!(OpenComConfig (20, "COM20", 115200, 0, 8, 1, 10*4096, 4096) == 0)){
					SetCtrlVal(panel,PANEL_Status_Window, "Unable to open COM port\n");
					Test_In_Process = 0;
					return 0;
				}
			}
			else if(COM_port_2 == 0){
				return 1;
			}
			
			if (!(SetXMode (COM_port_2, 0)==0)){
				SetCtrlVal(panel,PANEL_Status_Window, "Unable to open COM port\n");
				Test_In_Process = 0;			
				return 0;
			}
			if(!(SetCTSMode (COM_port_2, LWRS_HWHANDSHAKE_OFF) == 0)){
				SetCtrlVal(panel,PANEL_Status_Window, "Unable to open COM port\n");
				Test_In_Process = 0;			
				return 0;
			}
			if(!( SetComTime (COM_port_2, 0.25) == 0)){
				SetCtrlVal(panel,PANEL_Status_Window, "Unable to open COM port\n");
				Test_In_Process = 0;			
				return 0;
			}

return 1;
}

/*****************************************/
/*
/*
/*****************************************/
void Display_OK_MSG(char *msg)
{

#define buff_size 160

int lpanelHandle,err, i, done, switch_state, start, click_len;
char buffer[buff_size];

	//SetDir (directory);
 	 ProcessSystemEvents(); 	
	lpanelHandle = LoadPanel (0, "MSG OK.uir", PANEL);
	done = 0;
	start = 1;
	MSG_FLAG_1 = 0;   // clear the flag before checking
	 ResetTextBox (lpanelHandle,PANEL_OK_TEXTBOX, "");
	 SetCtrlVal(lpanelHandle,PANEL_OK_TEXTBOX, msg);
	 ProcessSystemEvents();
	 DisplayPanel (lpanelHandle);
	 SetActiveCtrl (lpanelHandle,PANEL_PB_MSG_OK );
	while(MSG_FLAG_1 == 0) // set to 1 by push button
	{
		ProcessSystemEvents();
		if ((Stop_Test_Exe == 1) && (CARD_STATE == 0 ) && (Stopped == 1)) break;  
	}
	HidePanel (lpanelHandle);	   	// hide the panel
	DiscardPanel (lpanelHandle);
	ProcessSystemEvents();
	ProcessDrawEvents();
		
return;
}

/*****************************************/
/*
/*
/*****************************************/
int CVICALLBACK Prompt_OK (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
				MSG_FLAG_1 = 1; // use this to tear down the menu
			break;
	}
	return 0;
}


/*****************************************/
/*
/*
/*****************************************/
int CVICALLBACK Prompt_YES (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
				MSG_YES_FLAG = 1; // use this to tear down the menu
				MSG_YES_PASS = 1;
			break;
	}
	return 0;
}


/*****************************************/
/*
/*
/*****************************************/
int CVICALLBACK Prompt_NO (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
				MSG_YES_FLAG = 1; // use this to tear down the menu
				MSG_YES_PASS = 0;
			break;
	}
	return 0;
}



/*****************************************/
/*
/*
/*****************************************/
void Display_YES_MSG(char *msg)
{

#define buff_size 160

int lpanelHandle,err, i, done, switch_state, start, click_len;
char buffer[buff_size];

	//SetDir (directory);
 	 ProcessSystemEvents(); 	
	lpanelHandle = LoadPanel (0, "MSG YES.uir", PANEL);
	done = 0;
	start = 1;
	MSG_YES_FLAG = 0;   // clear the flag before checking
	 ResetTextBox (lpanelHandle,PANEL_YES_TEXTBOX, "");
	 SetCtrlVal(lpanelHandle,PANEL_YES_TEXTBOX, msg);
	 ProcessSystemEvents();
	 DisplayPanel (lpanelHandle);
	 SetActiveCtrl (lpanelHandle,PANEL_PB_MSG_YES );
	while(MSG_YES_FLAG == 0) // set to 1 by push button
	{
		ProcessSystemEvents();
		if ((Stop_Test_Exe == 1) && (CARD_STATE == 0 ) && (Stopped == 1)) break;  
	}
	HidePanel (lpanelHandle);	   	// hide the panel
	DiscardPanel (lpanelHandle);
	ProcessSystemEvents();
	ProcessDrawEvents();
		
return;
}



/*****************************************/
/*
/*
/*****************************************/
void Flush_COM_1 (void)  
{
	int input_queue_length = 0; 
	
		input_queue_length = GetInQLen (COM_port);
	    while((input_queue_length = GetInQLen (COM_port))> 0)
	    {
	    	//Delay(1);
	    	FlushInQ(COM_port);
			FlushOutQ(COM_port);
	    }
}





//CB37 to Mux80 Connections Chart	
//CB37		X2			X3			X4			X5	
//AIN0-13	*MUX120-127	MUX48-61	MUX72-85	MUX96-109	
//DAC0-1	DAC0-1		MUX62-63	MUX86-87	MUX110-111	
//FIO0-7	FIO0-7		MUX64-71	MUX88-95	MUX112-119	
//PIN2,20	PIN2,20		N/C			N/C			N/C	
//MIO0-2	MIO0-2		N/C			N/C			N/C	
//*Start at AIN4				

//Example
//x3
//0 = 48
//1 = 49
//etc


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


/*****************************************/
/*
/*
/*****************************************/
int Setup_DAQ(int panel)
{
	LJ_ERROR lngErrorcode;
	long lngGetNextIteration;
	long lngIOType=0, lngChannel=0;
	double dblValue=0;
	LJ_HANDLE lngHandle=0;

	
	lngErrorcode = OpenLabJack (LJ_dtU3, LJ_ctUSB, "0", 1, &LabJack_Handle);
	//pin assignments are in the factory default condition.
	lngErrorcode = ePut (LabJack_Handle, LJ_ioPIN_CONFIGURATION_RESET, 0, 0, 0);
	
	lngErrorcode = GoOne(LabJack_Handle);
	ErrorHandler(lngErrorcode, __LINE__, 0);
	
	
	ePut (LabJack_Handle, LJ_ioPUT_ANALOG_ENABLE_BIT, 0, 1, 0);		//FIO0 as Analog Input    


	//Get all the results just to check for errors.
	lngErrorcode = GetFirstResult(LabJack_Handle, &lngIOType, &lngChannel, &dblValue, 0, 0);
	ErrorHandler(lngErrorcode, __LINE__, 0);
	lngGetNextIteration=0;	//Used by the error handling function.
	while(lngErrorcode < LJE_MIN_GROUP_ERROR)
	{
		lngErrorcode = GetNextResult(LabJack_Handle, &lngIOType, &lngChannel, &dblValue, 0, 0);
		if(lngErrorcode != LJE_NO_MORE_DATA_AVAILABLE)
		{
			ErrorHandler(lngErrorcode, __LINE__, lngGetNextIteration);
		}

		lngGetNextIteration++;
	}
	
	return lngErrorcode; 
	
}






/*****************************************/
/*
/*
/*****************************************/
int Aquarius_DC_Off(void)
{
	int status = 0; 
	
	status = eDO(LabJack_Handle,0,1);   		// FIO0 High
	
	return status;  
}



/*****************************************/
/*
/*
/*****************************************/
int Aquarius_DC_On(void)
{
	int status = 0; 
	
	status = eDO(LabJack_Handle,0,0);   		// FIO0 Low
	
	return status;  
}

/*****************************************/
/*
/*
/*****************************************/
int FIO0_Off(void)
{
	int status = 0; 
	
	status = eDO(LabJack_Handle,0,1);   		// FIO0 High
	
	return status;  
}



/*****************************************/
/*
/*
/*****************************************/
int FIO0_On(void)
{
	int status = 0; 
	
	status = eDO(LabJack_Handle,0,0);   		// FIO0 Low
	
	return status;  
}


//EIO0 - 7 = 8 to 15
/*****************************************/
/*
/*
/*****************************************/
int EIO0_Off(void)
{
	int status = 0; 
	
	status = eDO(LabJack_Handle,8,1);   		// EIO0 High
	
	return status;  
}



/*****************************************/
/*
/*
/*****************************************/
int EIO0_On(void)
{
	int status = 0; 
	
	status = eDO(LabJack_Handle,8,0);   		// EIO0 Low
	
	return status;  
}


/*****************************************/
/*
/*
/*****************************************/
int EIO1_Off(void)
{
	int status = 0; 
	
	status = eDO(LabJack_Handle,9,1);   		// EIO1 High
	
	return status;  
}



/*****************************************/
/*
/*
/*****************************************/
int EIO1_On(void)
{
	int status = 0; 
	
	status = eDO(LabJack_Handle,9,0);   		// EIO1 Low
	
	return status;  
}


/*****************************************/
/*
/*
/*****************************************/
int EIO2_Off(void)
{
	int status = 0; 
	
	status = eDO(LabJack_Handle,10,1);   		// EIO2 High
	
	return status;  
}



/*****************************************/
/*
/*
/*****************************************/
int EIO2_On(void)
{
	int status = 0; 
	
	status = eDO(LabJack_Handle,10,0);   		// EIO2 Low
	
	return status;  
}


/*****************************************/
/*
/*
/*****************************************/
int EIO3_Off(void)
{
	int status = 0; 
	
	status = eDO(LabJack_Handle,11,1);   		// EIO3 High
	
	return status;  
}



/*****************************************/
/*
/*
/*****************************************/
int EIO3_On(void)
{
	int status = 0; 
	
	status = eDO(LabJack_Handle,11,0);   		// EIO3 Low
	
	return status;  
}


/*****************************************/
/*
/*
/*****************************************/
int EIO4_Off(void)
{
	int status = 0; 
	
	status = eDO(LabJack_Handle,12,1);   		// EIO4 High
	
	return status;  
}



/*****************************************/
/*
/*
/*****************************************/
int EIO4_On(void)
{
	int status = 0; 
	
	status = eDO(LabJack_Handle,12,0);   		// EIO4 Low
	
	return status;  
}


/*****************************************/
/*
/*
/*****************************************/
int EIO5_Off(void)
{
	int status = 0; 
	
	status = eDO(LabJack_Handle,13,1);   		// EIO5 High
	
	return status;  
}



/*****************************************/
/*
/*
/*****************************************/
int EIO5_On(void)
{
	int status = 0; 
	
	status = eDO(LabJack_Handle,13,0);   		// EIO5 Low
	
	return status;  
}


/*****************************************/
/*
/*
/*****************************************/
int EIO6_Off(void)
{
	int status = 0; 
	
	status = eDO(LabJack_Handle,14,1);   		// EIO6 High
	
	return status;  
}



/*****************************************/
/*
/*
/*****************************************/
int EIO6_On(void)
{
	int status = 0; 
	
	status = eDO(LabJack_Handle,14,0);   		// EIO6 Low
	
	return status;  
}


/*****************************************/
/*
/*
/*****************************************/
int EIO7_Off(void)
{
	int status = 0; 
	
	status = eDO(LabJack_Handle,15,1);   		// EIO7 High
	
	return status;  
}



/*****************************************/
/*
/*
/*****************************************/
int EIO7_On(void)
{
	int status = 0; 
	
	status = eDO(LabJack_Handle,15,0);   		// EIO7 Low
	
	return status;  
}


int EIALL_Off(void)
{
	int status = 0;

	status = eDO(LabJack_Handle,8,1);
		Delay(1);   
	status = eDO(LabJack_Handle,9,1);
		Delay(1);   
	status = eDO(LabJack_Handle,10,1);
		Delay(1);   
	status = eDO(LabJack_Handle,11,1);
		Delay(1);   
	status = eDO(LabJack_Handle,12,1);
		Delay(1);   
	/*status = eDO(LabJack_Handle,13,1);
		Delay(1);  		  */
	status = eDO(LabJack_Handle,14,1);
		Delay(1);   
	status = eDO(LabJack_Handle,15,1);
		Delay(1);      

	return status;
}

//*************************************************************
//*int Init_CMD60 (int panel) 	
//* 
//*************************************************************
int Init_CMD60 (int panel) {		
	
	char Temp_Buff[256]; 
	int status = 1;
	int numToRead; 

	
   //get handle for CMD60 instrument at CMD60_GPIB address, open the device
   CMD60_HANDLE = ibdev (0, CMD60_GPIB, NO_SAD, T10s, 1, 0);
   if (ibsta & 0x8000) status = 0;
   
   //to bring CMD60 to initial state after power on, you send the following   
   //send interface clear to the instrument
   ibwrt (CMD60_HANDLE, "IFC", strlen("IFC"));
   if (ibsta & 0x8000) status = 0;
   
   //send device clear to the instrunet
   ibwrt (CMD60_HANDLE, "DCL", strlen("DCL")); 
   if (ibsta & 0x8000) status = 0;
   
    //clear the device
   ibclr (CMD60_HANDLE);
   if (ibsta & 0x8000) status = 0;  
   
   //clear status of the instrument
   ibwrt (CMD60_HANDLE, "*CLS", strlen("*CLS"));
   if (ibsta & 0x8000) status = 0;
   
   //reset the instrunet
   ibwrt (CMD60_HANDLE, "*RST", strlen("*RST")); 
   if (ibsta & 0x8000) status = 0;   

   //write command to read the instrument id
   ibwrt (CMD60_HANDLE, "*IDN?", strlen("*IDN?")); 
   if (ibsta & 0x8000) status = 0;
   
   //read the instrument id
   numToRead = 200;
   ibrd (CMD60_HANDLE, Temp_Buff, numToRead);
   if (ibsta & 0x8000) status = 0;
   
   //to bring CMD60 to initial state after power on, you send the following   
   //send interface clear to the instrument
   ibwrt (CMD60_HANDLE, "IFC", strlen("IFC"));
   if (ibsta & 0x8000) status = 0;
   
   //send device clear to the instrunet
   ibwrt (CMD60_HANDLE, "DCL", strlen("DCL")); 
   if (ibsta & 0x8000) status = 0;
   
    //clear the device
   ibclr (CMD60_HANDLE);
   if (ibsta & 0x8000) status = 0;  
   
   //clear status of the instrument
   ibwrt (CMD60_HANDLE, "*CLS", strlen("*CLS"));
   if (ibsta & 0x8000) status = 0;
   
   //reset the instrunet
   ibwrt (CMD60_HANDLE, "*RST", strlen("*RST")); 
   if (ibsta & 0x8000) status = 0;
   
   
	return status;
   
}



//*************************************************************
//*int Set_CMD60_in_Loopback_Mode_EU_Setting(int panel) 	
//* 
//*************************************************************
int Set_CMD60_in_Loopback_Mode_EU_Setting(int panel) 
{		
	char Temp_Buff[256]; 
	int status = 1;
	int numToRead;
	char msg[256] = {'\0'};

	
	sprintf(msg,"DECT CMD60 Loopback Mode Test\n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg);
	WriteToTerminal(panel, msg); 
	
    
   //CMD60 setting for EU
   ibwrt (CMD60_HANDLE, "CONF:TEST:TYPE DPP", strlen("CONF:TEST:TYPE DPP"));
   if (ibsta & 0x8000) status = 0; 
   
   ibwrt (CMD60_HANDLE, "PROC:SEL:TEST MAN", strlen("PROC:SEL:TEST MAN"));
   if (ibsta & 0x8000) status = 0;
   
   ibwrt (CMD60_HANDLE, "CONF:PP:SIGN:MODE LOOP", strlen("CONF:PP:SIGN:MODE LOOP"));
   if (ibsta & 0x8000) status = 0; 
   
   ibwrt (CMD60_HANDLE, "CONF:POW:PACK PP32Z", strlen("CONF:POW:PACK PP32Z"));
   if (ibsta & 0x8000) status = 0;
   
   ibwrt (CMD60_HANDLE, "CONF:PP:RFPI '0001400000'", strlen("CONF:PP:RFPI '0001400000'"));
   if (ibsta & 0x8000) status = 0; 
   
   ibwrt (CMD60_HANDLE, "CONF:PP:RF:LEV -45.0dBm", strlen("CONF:PP:RF:LEV -45.0dBm"));
   if (ibsta & 0x8000) status = 0;
   
   ibwrt (CMD60_HANDLE, "CONF:PP:DUMM:SLOT 0", strlen("CONF:PP:DUMM:SLOT 0"));
   if (ibsta & 0x8000) status = 0; 
   
   ibwrt (CMD60_HANDLE, "CONF:PP:DUMM:CARR 5", strlen("CONF:PP:DUMM:CARR 5"));
   if (ibsta & 0x8000) status = 0;
   
   ibwrt (CMD60_HANDLE, "CONF:PP:TRAF:SLOT 2", strlen("CONF:PP:TRAF:SLOT 2"));
   if (ibsta & 0x8000) status = 0; 
   
   ibwrt (CMD60_HANDLE, "CONF:PP:TRAF:CARR 0", strlen("CONF:PP:TRAF:CARR 0"));	  	//conductive

   if (ibsta & 0x8000) status = 0;

   
   	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);	
	}
	
	return status;
}

//*************************************************************
//*int Set_RTX_in_Loopback_Mode_EU_Setting(int panel)
//*
//*************************************************************
int Set_RTX_in_Loopback_Mode_EU_Setting(int panel)
{
	char Temp_Buff[256];
	int status = 1;
	int numToRead;
	char msg[256] = {'\0'};


	sprintf(msg,"DECT RTX Loopback Mode Test\n");
	WriteToTerminal(panel, msg);


	//RTX setting for EU
	status = rtx2012_Loopback_Mode_EU_Setting();

	if(status == 1)
	{
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg);
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else
	{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg);
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);
	}

	return status;
}



//*************************************************************
//*int Set_RTX_in_Loopback_Mode_EU_Setting(int panel)
//*
//*******_******************************************************
int Reset_RTX(int panel)
{
		 
	rtx2012_Reset();   



	return 1;
}

//*************************************************************
//*int Set_CMD60_in_Loopback_Mode_Radiated_Setting(int panel) 	
//* 
//*************************************************************
int Set_CMD60_in_Loopback_Mode_Radiated_Setting(int panel) 
{		
	char Temp_Buff[256]; 
	int status = 1;
	int numToRead;
	char msg[256] = {'\0'};

	
	sprintf(msg,"DECT CMD60 Loopback Mode Test\n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg);
	WriteToTerminal(panel, msg); 
	
    
   //CMD60 setting for EU
   ibwrt (CMD60_HANDLE, "CONF:TEST:TYPE DPP", strlen("CONF:TEST:TYPE DPP"));
   if (ibsta & 0x8000) status = 0; 
   
   ibwrt (CMD60_HANDLE, "PROC:SEL:TEST MAN", strlen("PROC:SEL:TEST MAN"));
   if (ibsta & 0x8000) status = 0;
   
   ibwrt (CMD60_HANDLE, "CONF:PP:SIGN:MODE LOOP", strlen("CONF:PP:SIGN:MODE LOOP"));
   if (ibsta & 0x8000) status = 0; 
   
   ibwrt (CMD60_HANDLE, "CONF:POW:PACK PP32Z", strlen("CONF:POW:PACK PP32Z"));
   if (ibsta & 0x8000) status = 0;
   
   ibwrt (CMD60_HANDLE, "CONF:PP:RFPI '0001400000'", strlen("CONF:PP:RFPI '0001400000'"));
   if (ibsta & 0x8000) status = 0; 
   
   ibwrt (CMD60_HANDLE, "CONF:PP:RF:LEV -55.0dBm", strlen("CONF:PP:RF:LEV -55.0dBm"));
   if (ibsta & 0x8000) status = 0;
   
   ibwrt (CMD60_HANDLE, "CONF:PP:DUMM:SLOT 0", strlen("CONF:PP:DUMM:SLOT 0"));
   if (ibsta & 0x8000) status = 0; 
   
   ibwrt (CMD60_HANDLE, "CONF:PP:DUMM:CARR 5", strlen("CONF:PP:DUMM:CARR 5"));
   if (ibsta & 0x8000) status = 0;
   
   ibwrt (CMD60_HANDLE, "CONF:PP:TRAF:SLOT 2", strlen("CONF:PP:TRAF:SLOT 2"));
   if (ibsta & 0x8000) status = 0; 
   
   ibwrt (CMD60_HANDLE, "CONF:PP:TRAF:CARR 1", strlen("CONF:PP:TRAF:CARR 1"));	  	//radiated

   if (ibsta & 0x8000) status = 0;

   
   	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);	
	}
	
	return status;
}



//*************************************************************
//*int Set_CMD60_Mask_Setting(int panel) 	
//* 
//*************************************************************
int Set_CMD60_Mask_Setting(int panel) 
{		
	char temp_buff[256]; 
	int status = 1;
	int numToRead;
	char msg[256] = {'\0'};

	
	sprintf(msg,"DECT Setup Power Mask Test\n");
	WriteToTerminal(panel, msg); 

	
	ibwrt (CMD60_HANDLE, "READ:ARR:POW:TRAN?", strlen("READ:ARR:POW:TRAN?"));		
   	if (ibsta & 0x8000) status = 0; 
	
	
	numToRead = 200;
	ibrd (CMD60_HANDLE, temp_buff, numToRead);
	if (ibsta & 0x8000) status = 0;
    

	Delay(2);
	
   	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);	
	}
	
	return status;
}

//*************************************************************
//*int Set_CMD60_Mask_Setting(int panel)
//*
//*************************************************************
int Set_RTX_Mask_Setting(int panel)
{
	char temp_buff[256];
	int status = 1;
	int numToRead;
	char msg[256] = {'\0'};


	sprintf(msg,"DECT Setup Power Mask Test\n");
	WriteToTerminal(panel, msg);

	WriteToTerminal(panel, "Use the default setting");




	if(status == 1)
	{
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg);
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else
	{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg);
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);
	}

	return status;
}


//*************************************************************
//*int Open_CMD60 (int panel) 	
//* 
//*************************************************************
int Open_CMD60 (int panel) 
{

	int status = 1;
	char msg[256] = {'\0'};

	
	sprintf(msg,"DECT Open CMD60 Test\n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg); 
	WriteToTerminal(panel, msg); 
	
   //to bring CMD60 to initial state after power on, you send the following   
   //send interface clear to the instrument
   ibwrt (CMD60_HANDLE, "IFC", strlen("IFC"));
   if (ibsta & 0x8000) status = 0;
   
   //send device clear to the instrunet
   ibwrt (CMD60_HANDLE, "DCL", strlen("DCL")); 
   if (ibsta & 0x8000) status = 0;
   
    //clear the device
   ibclr (CMD60_HANDLE);
   if (ibsta & 0x8000) status = 0;  
   
   //clear status of the instrument
   ibwrt (CMD60_HANDLE, "*CLS", strlen("*CLS"));
   if (ibsta & 0x8000) status = 0;
   
   //reset the instrunet
   ibwrt (CMD60_HANDLE, "*RST", strlen("*RST")); 
   if (ibsta & 0x8000) status = 0;   

   
   	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);	
	}
	
	return status;
}

//*************************************************************
//*int Open_RTX (int panel)
//*
//*************************************************************
int Open_RTX (int panel)
{

	int status = 1;
	char msg[256] = {'\0'};


	sprintf(msg,"DECT Open RTX Test\n");
	WriteToTerminal(panel, msg);


	status = rtx2012_Init (msg);


	if(status == 1)
	{
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg);
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else
	{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg);
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);
	}

	return status;
}



//*************************************************************
//*int Setup_Connect_CMD60 (int panel)  	
//* 
//*************************************************************
int Setup_Connect_CMD60 (int panel) 
{

	int status = 1;
	char msg[256] = {'\0'};
	char temp_buff[24] = {'\0'}; 
	int numToRead;
	int counter = 0;

	
	sprintf(msg,"DECT Setup Connect CMD60 Test\n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg);
	WriteToTerminal(panel, msg); 
	
	while((counter < 10) && (!(strstr (temp_buff, "DCE")))){
		//set to establish connection with dut
		ibwrt (CMD60_HANDLE, "PROC:CONN:SET", strlen("PROC:CONN:SET"));
	   	if (ibsta & 0x8000) status = 0;
		
		Delay(1);
		
		ibwrt (CMD60_HANDLE, "STAT:DEV?", strlen("STAT:DEV?"));
	   	if (ibsta & 0x8000) status = 0;
   
		//read the instrument for DCE (DECT Connection Established)
	   	numToRead = 3;
	   	ibrd (CMD60_HANDLE, temp_buff, numToRead);
	   	if (ibsta & 0x8000) status = 0;
		
		Delay(1);
		
		counter++;
	}
				
	if (strstr (temp_buff, "DCE"))
	{
		status = 1;
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else
	{
		status = 0;
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg);
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);
	}
		
	
	return status;
}

//*************************************************************
//*int Setup_Connect_RTX (int panel)
//*
//*************************************************************
int Setup_Connect_RTX (int panel)
{

	int status = 1;
	char msg[256] = {'\0'};
	char temp_buff[24] = {'\0'};
	int numToRead;
	int counter = 0;


	sprintf(msg,"DECT Setup Connect RTX Test\n");
	WriteToTerminal(panel, msg);

	status = rtx2012_Nonsignal_Connect();


	if (status == 1)
	{
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg);
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else
	{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg);
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);
	}


	return status;
}



//*************************************************************
//*int Close_CMD60 (int panel) 	
//* 
//*************************************************************
int Close_CMD60 (int panel) 
{
	int status = 1;
	int CMD60_Error = 0; 
	char msg[256] = {'\0'};

	
	sprintf(msg,"DECT Close CMD60 Test\n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg); 
	WriteToTerminal(panel, msg); 
	
	
	
   //bring the device to power on state before closing it   
   //to bring CMD60 to initial state after power on, you send the following   
   //send interface clear to the instrument
   ibwrt (CMD60_HANDLE, "IFC", strlen("IFC"));
   if (ibsta & 0x8000) status = 0;
   
   //send device clear to the instrunet
   ibwrt (CMD60_HANDLE, "DCL", strlen("DCL")); 
   if (ibsta & 0x8000) status = 0;
   
    //clear the device
   ibclr (CMD60_HANDLE);
   if (ibsta & 0x8000) status = 0;  
   
   //clear status of the instrument
   ibwrt (CMD60_HANDLE, "*CLS", strlen("*CLS"));
   if (ibsta & 0x8000) status = 0;
   
   //reset the instrunet
   ibwrt (CMD60_HANDLE, "*RST", strlen("*RST")); 
   if (ibsta & 0x8000) status = 0;
   
   
   	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);	
	}
	
	return status;
   

}


//*************************************************************
//*int Close_RTX (int panel)
//*
//*************************************************************
int Close_RTX (int panel)
{
	int status = 1;
	int CMD60_Error = 0;
	char msg[256] = {'\0'};


	sprintf(msg,"DECT Close RTX Test\n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg);
	WriteToTerminal(panel, msg);


	rtx2012_Nosignal_Disconnect();
	
	rtx2012_Reset();




	if(status == 1)
	{
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg);
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else
	{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg);
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);
	}

	return status;


}

//*************************************************************
//*int Init_MT8852B (int panel) 	
//* 
//*************************************************************
int Init_MT8852B (int panel) {		
	
	char Temp_Buff[1024]; 
	int status = 1;
	int numToRead;  
	char msg[256] = {'\0'}; 
	
   //sprintf (Temp_Buff, "GPIB0::%d::INSTR", UCT_GPIB);
   
   
   	//sprintf(msg,"Init_MT8852B Test\n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg); 
	//WriteToTerminal(panel, msg); 
	
	
   //get handle for MT8852B instrument at MT8852B_GPIB address, open the device
   MT8852B_HANDLE = ibdev (0, MT8852B_GPIB, NO_SAD, T10s, 1, 0);
   if (ibsta & 0x8000) status = 0;
   
   //to bring CMD60 to initial state after power on, you send the following   
   //send interface clear to the instrument
   ibwrt (MT8852B_HANDLE, "IFC", strlen("IFC"));
   if (ibsta & 0x8000) status = 0; 
   
   //send device clear to the instrunet
   ibwrt (MT8852B_HANDLE, "DCL", strlen("DCL")); 
   if (ibsta & 0x8000) status = 0; 
   
    //clear the device
   ibclr (MT8852B_HANDLE);
   if (ibsta & 0x8000) status = 0;   
   
   //clear status of the instrument
   ibwrt (MT8852B_HANDLE, "*CLS", strlen("*CLS"));
   if (ibsta & 0x8000) status = 0; 
   
   //reset the instrument
   ibwrt (MT8852B_HANDLE, "*RST", strlen("*RST")); 
   if (ibsta & 0x8000) status = 0;    

   //write command to read the instrument id
   ibwrt (MT8852B_HANDLE, "*IDN?", strlen("*IDN?")); 
   if (ibsta & 0x8000) status = 0; 
   
   //read the instrument id
   numToRead = 200;
   ibrd (MT8852B_HANDLE, Temp_Buff, numToRead);
   if (ibsta & 0x8000) status = 0; 
   
   //to bring CMD60 to initial state after power on, you send the following   
   //send interface clear to the instrument
   ibwrt (MT8852B_HANDLE, "IFC", strlen("IFC"));
   if (ibsta & 0x8000) status = 0; 
   
   //send device clear to the instrunet
   ibwrt (MT8852B_HANDLE, "DCL", strlen("DCL")); 
   if (ibsta & 0x8000) status = 0; 
   
    //clear the device
   ibclr (MT8852B_HANDLE);
   if (ibsta & 0x8000) status = 0;   
   
   //clear status of the instrument
   ibwrt (MT8852B_HANDLE, "*CLS", strlen("*CLS"));
   if (ibsta & 0x8000) status = 0; 
   
   //reset the instrunet
   ibwrt (MT8852B_HANDLE, "*RST", strlen("*RST")); 
   if (ibsta & 0x8000) status = 0; 
   
   
   /*if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);	
	}*/
   
	return status;
   
}


//*************************************************************
//*int Open_MT8852B (int panel)  	
//* 
//*************************************************************
int Open_MT8852B (int panel)
{
	int status = 1;
	char msg[256] = {'\0'}; 
	
	sprintf(msg,"Open_MT8852B Test\n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg); 
	WriteToTerminal(panel, msg); 
	
   //to bring MT8852B to initial state after power on, you send the following   
   //send interface clear to the instrument
   ibwrt (MT8852B_HANDLE, "IFC", strlen("IFC"));
   if (ibsta & 0x8000) status = 0;
   
   //send device clear to the instrunet
   ibwrt (MT8852B_HANDLE, "DCL", strlen("DCL")); 
   if (ibsta & 0x8000) status = 0;
   
    //clear the device
   ibclr (MT8852B_HANDLE);
   if (ibsta & 0x8000) status = 0;  
   
   //clear status of the instrument
   ibwrt (MT8852B_HANDLE, "*CLS", strlen("*CLS"));
   if (ibsta & 0x8000) status = 0;
   
   //reset the instrunet
   ibwrt (MT8852B_HANDLE, "*RST", strlen("*RST")); 
   if (ibsta & 0x8000) status = 0; 
   
   
   if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);	
	}
   
	
	return status;
}



//*************************************************************
//*int Check_Sync_MT8852B (int panel)  	
//* 
//*************************************************************
int Check_Sync_MT8852B (int panel)  
{

	int status = 1;
	char msg[256] = {'\0'};
	int numToRead;
	int counter = 0;
	char returned_string[256] = {'\0'};
	
	
	sprintf(msg,"DECT Check Sync with MT8852B Test\n");
	WriteToTerminal(panel, msg); 
	
	while((counter < 10) && (!(strstr (returned_string, "!NO ERRORS!NO ERRORS!"))))
	{
			
   		ibwrt (MT8852B_HANDLE, "ERRLST", strlen("ERRLST"));
   		if (ibsta & 0x8000) status = 0;
	
		Delay (1);

		numToRead = 200;
		ibrd (MT8852B_HANDLE, returned_string, numToRead);
   		if (ibsta & 0x8000) status = 0; 
		
		//sprintf(msg,".");
		//WriteToTerminal(panel, msg); 
		ProcessSystemEvents (); 
			
		counter ++;

	} 
	
	
	if(strstr (returned_string, "!NO ERRORS!NO ERRORS!")){
		status = 1;
	}
	else{
		counter = 0;
		
		sprintf(msg,"Retry Sync\n");
		WriteToTerminal(panel, msg); 
		
		//to bring MT8852B to initial state after power on, you send the following   
	   	//send interface clear to the instrument
	   	ibwrt (MT8852B_HANDLE, "IFC", strlen("IFC"));
	  	if (ibsta & 0x8000) status = 0;
   
	   	//send device clear to the instrunet
	   	ibwrt (MT8852B_HANDLE, "DCL", strlen("DCL")); 
	   	if (ibsta & 0x8000) status = 0;
   
	    //clear the device
	   	ibclr (MT8852B_HANDLE);
	   	if (ibsta & 0x8000) status = 0;  
   
	   	//clear status of the instrument
	   	ibwrt (MT8852B_HANDLE, "*CLS", strlen("*CLS"));
	   	if (ibsta & 0x8000) status = 0;
   
	   	//reset the instrunet
	   	ibwrt (MT8852B_HANDLE, "*RST", strlen("*RST")); 
	   	if (ibsta & 0x8000) status = 0; 
		
		while((counter < 10) && (!(strstr (returned_string, "!NO ERRORS!NO ERRORS!"))))
		{
	   		ibwrt (MT8852B_HANDLE, "ERRLST", strlen("ERRLST"));
	   		if (ibsta & 0x8000) status = 0;
	
			Delay (1);

			numToRead = 200;
			ibrd (MT8852B_HANDLE, returned_string, numToRead);
	   		if (ibsta & 0x8000) status = 0; 
		
			//sprintf(msg,".");
			//WriteToTerminal(panel, msg); 
			ProcessSystemEvents (); 
			
			counter ++;
		}
		
		if(strstr (returned_string, "!NO ERRORS!NO ERRORS!")){
			status = 1;
		}
	}
	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);
	}
	
	return status;

}

//*************************************************************
//*int Check_Sync_MT8852B_2 (int panel)  	
//* 
//*************************************************************
int Check_Sync_MT8852B_2 (int panel)  
{

	int status = 1;
	char msg[256] = {'\0'};
	int numToRead;
	int counter = 0;
	char returned_string[256] = {'\0'};
	
	
	//sprintf(msg,"DECT Check Sync with MT8852B Test\n");
	//WriteToTerminal(panel, msg); 
	
	while((counter < 10) && (!(strstr (returned_string, "!NO ERRORS!NO ERRORS!"))))
	{
			
   		ibwrt (MT8852B_HANDLE, "ERRLST", strlen("ERRLST"));
   		if (ibsta & 0x8000) status = 0;
	
		Delay (1);

		numToRead = 200;
		ibrd (MT8852B_HANDLE, returned_string, numToRead);
   		if (ibsta & 0x8000) status = 0; 
		
		sprintf(msg,".");
		WriteToTerminal(panel, msg); 
		ProcessSystemEvents (); 
			
		counter ++;

	} 
	
	
	if(strstr (returned_string, "!NO ERRORS!NO ERRORS!")){
		status = 1;
	}
	else{
		counter = 0;
		
		sprintf(msg,"Retry Sync\n");
		WriteToTerminal(panel, msg); 
		
		//to bring MT8852B to initial state after power on, you send the following   
	   	//send interface clear to the instrument
	   	ibwrt (MT8852B_HANDLE, "IFC", strlen("IFC"));
	  	if (ibsta & 0x8000) status = 0;
   
	   	//send device clear to the instrunet
	   	ibwrt (MT8852B_HANDLE, "DCL", strlen("DCL")); 
	   	if (ibsta & 0x8000) status = 0;
   
	    //clear the device
	   	ibclr (MT8852B_HANDLE);
	   	if (ibsta & 0x8000) status = 0;  
   
	   	//clear status of the instrument
	   	ibwrt (MT8852B_HANDLE, "*CLS", strlen("*CLS"));
	   	if (ibsta & 0x8000) status = 0;
   
	   	//reset the instrunet
	   	ibwrt (MT8852B_HANDLE, "*RST", strlen("*RST")); 
	   	if (ibsta & 0x8000) status = 0; 
		
		while((counter < 10) && (!(strstr (returned_string, "!NO ERRORS!NO ERRORS!"))))
		{
	   		ibwrt (MT8852B_HANDLE, "ERRLST", strlen("ERRLST"));
	   		if (ibsta & 0x8000) status = 0;
	
			Delay (1);

			numToRead = 200;
			ibrd (MT8852B_HANDLE, returned_string, numToRead);
	   		if (ibsta & 0x8000) status = 0; 
		
			sprintf(msg,".");
			WriteToTerminal(panel, msg); 
			ProcessSystemEvents (); 
			
			counter ++;
		}
		
		if(strstr (returned_string, "!NO ERRORS!NO ERRORS!")){
			status = 1;
		}
	}
	
	/*if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);
	}*/
	
	return status;

}


//*************************************************************
//*int Set_MT8852B_Loopback (int panel)  	
//* 
//*************************************************************
int Set_MT8852B_Loopback_Mode (int panel) 
{

	int status = 1;
	char msg[256] = {'\0'};
	char temp_buff[256] = {'\0'}; 
	int numToRead;
	int counter = 0;
	char returned_string[256] = {'\0'};
	
	
	sprintf(msg,"DECT Set MT8852B in Loopback Mode\n");
	WriteToTerminal(panel, msg); 
	
   	//reset the instrunet
   	ibwrt (MT8852B_HANDLE, "*RST", strlen("*RST")); 
   	if (ibsta & 0x8000) status = 0;
   
   	//clear status of the instrument
   	ibwrt (MT8852B_HANDLE, "*CLS", strlen("*CLS"));
   	if (ibsta & 0x8000) status = 0;

   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "SCPTTSTGP 3,EDRTSTS,OFF", strlen("SCPTTSTGP 3,EDRTSTS,OFF"));
   	if (ibsta & 0x8000) status = 0;     

   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "SCPTTSTGP 3,STDTSTS,ON", strlen("SCPTTSTGP 3,STDTSTS,ON"));
   	if (ibsta & 0x8000) status = 0;   
  
   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "SCPTCFG 3,PC,OFF", strlen("SCPTCFG 3,PC,OFF"));
   	if (ibsta & 0x8000) status = 0;   

   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "SCPTCFG 3,MS,OFF", strlen("SCPTCFG 3,MS,OFF"));
   	if (ibsta & 0x8000) status = 0;    

   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "SCPTCFG 3,MP,OFF", strlen("SCPTCFG 3,MP,OFF"));
   	if (ibsta & 0x8000) status = 0;
   
   	//set operation mode to script mode
   	ibwrt (MT8852B_HANDLE, "OPMD SCRIPT", strlen("OPMD SCRIPT"));
   	if (ibsta & 0x8000) status = 0;
   
   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "SCPTSEL 3", strlen("SCPTSEL 3"));
   	if (ibsta & 0x8000) status = 0;   


////////////output power setting////////////////
   
   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "OPCFG 3,DEFAULT", strlen("OPCFG 3,DEFAULT"));
   	if (ibsta & 0x8000) status = 0;     

   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "OPCFG 3,AVGMXLIM,1.5", strlen("OPCFG 3,AVGMXLIM,1.5"));
   	if (ibsta & 0x8000) status = 0;   
   
   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "OPCFG 3,AVGMNLIM,-4.5", strlen("OPCFG 3,AVGMNLIM,-4.5"));
   	if (ibsta & 0x8000) status = 0;   

   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "OPCFG 3,PEAKLIM,1.5", strlen("OPCFG 3,PEAKLIM,1.5"));
   	if (ibsta & 0x8000) status = 0;    

   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "OPCFG 3,LFREQSEL,OFF", strlen("OPCFG 3,LFREQSEL,OFF"));
   	if (ibsta & 0x8000) status = 0;
   
   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "OPCFG 3,MFREQSEL,ON", strlen("OPCFG 3,MFREQSEL,ON"));
   	if (ibsta & 0x8000) status = 0; 
   
   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "OPCFG 3,HFREQSEL,OFF", strlen("OPCFG 3,HFREQSEL,OFF"));
   	if (ibsta & 0x8000) status = 0;   


////////////////modulation characteristics setting///////////////
   
   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "MICFG 3,DEFAULT", strlen("MICFG 3,DEFAULT"));
   	if (ibsta & 0x8000) status = 0;     

   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "MICFG 3,PKTTYPE,DH5", strlen("MICFG 3,PKTTYPE,DH5"));
   	if (ibsta & 0x8000) status = 0;   
   
   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "MICFG 3,F1AVGMIN,150kHz", strlen("MICFG 3,F1AVGMIN,150kHz"));
   	if (ibsta & 0x8000) status = 0;   

   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "MICFG 3,LFREQSEL,OFF", strlen("MICFG 3,LFREQSEL,OFF"));
   	if (ibsta & 0x8000) status = 0;    

   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "MICFG 3,MFREQSEL,ON", strlen("MICFG 3,MFREQSEL,ON"));
   	if (ibsta & 0x8000) status = 0;
   
   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "MICFG 3,HFREQSEL,OFF", strlen("MICFG 3,HFREQSEL,OFF"));
   	if (ibsta & 0x8000) status = 0;    
   

//////////////carrier frequency drift setting///////////////
   
   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "ICCFG 3,DEFAULT", strlen("ICCFG 3,DEFAULT"));
   	if (ibsta & 0x8000) status = 0;     

   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "ICCFG 3,HOPPING,HOPOFF", strlen("ICCFG 3,HOPPING,HOPOFF"));
   	if (ibsta & 0x8000) status = 0;   
   
   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "ICCFG 3,MXPOSLIM,50 kHz", strlen("ICCFG 3,MXPOSLIM,50 kHz"));
   	if (ibsta & 0x8000) status = 0;   

   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "ICCFG 3,MXNEGLIM,50 kHz", strlen("ICCFG 3,MXNEGLIM,50 kHz"));
   	if (ibsta & 0x8000) status = 0;    

   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "ICCFG 3,LFREQSEL,OFF", strlen("ICCFG 3,LFREQSEL,OFF"));
   	if (ibsta & 0x8000) status = 0;
   
   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "ICCFG 3,MFREQSEL,ON", strlen("ICCFG 3,MFREQSEL,ON"));
   	if (ibsta & 0x8000) status = 0; 
   
   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "ICCFG 3,HFREQSEL,OFF", strlen("ICCFG 3,HFREQSEL,OFF"));
   	if (ibsta & 0x8000) status = 0;   

   
////////////initial carrier frequency tolerance setting//////////////////
   
   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "CDCFG 3,DEFAULT", strlen("CDCFG 3,DEFAULT"));
   	if (ibsta & 0x8000) status = 0;     

   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "CDCFG 3,HOPPING,HOPOFF", strlen("CDCFG 3,HOPPING,HOPOFF"));
   	if (ibsta & 0x8000) status = 0;   
   
   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "CDCFG 3,PKTSIZE,FIVESLOT,TRUE", strlen("CDCFG 3,PKTSIZE,FIVESLOT,TRUE"));
   	if (ibsta & 0x8000) status = 0;   

   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "CDCFG 3,PKTSIZE,THREESLOT,FALSE", strlen("CDCFG 3,PKTSIZE,THREESLOT,FALSE"));
   	if (ibsta & 0x8000) status = 0;    

   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "CDCFG 3,PKTSIZE,ONESLOT,FALSE", strlen("CDCFG 3,PKTSIZE,ONESLOT,FALSE"));
   	if (ibsta & 0x8000) status = 0;
   
   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "CDCFG 3,LFREQSEL,OFF", strlen("CDCFG 3,LFREQSEL,OFF"));
   	if (ibsta & 0x8000) status = 0; 
   
   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "CDCFG 3,MFREQSEL,ON", strlen("CDCFG 3,MFREQSEL,ON"));
   	if (ibsta & 0x8000) status = 0;   

   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "CDCFG 3,HFREQSEL,OFF", strlen("CDCFG 3,HFREQSEL,OFF"));
   	if (ibsta & 0x8000) status = 0;   
   

/////////////sensitivity single slot packets setting/////////////////
   
   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "SSCFG 3,DEFAULT", strlen("SSCFG 3,DEFAULT"));
   	if (ibsta & 0x8000) status = 0;     

   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "SSCFG 3,HOPPING,HOPOFF", strlen("SSCFG 3,HOPPING,HOPOFF"));
   	if (ibsta & 0x8000) status = 0;   
   
   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "SSCFG 3,TXPWR,-76.0", strlen("SSCFG 3,TXPWR,-76.0"));
   	if (ibsta & 0x8000) status = 0;

   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "SSCFG 3,DIRTYTX,ON", strlen("SSCFG 3,DIRTYTX,ON"));
   	if (ibsta & 0x8000) status = 0;    

   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "SSCFG 3,LFREQSEL,OFF", strlen("SSCFG 3,LFREQSEL,OFF"));
   	if (ibsta & 0x8000) status = 0;
   
   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "SSCFG 3,MFREQSEL,ON", strlen("SSCFG 3,MFREQSEL,ON"));
   	if (ibsta & 0x8000) status = 0; 
   
   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "SSCFG 3,HFREQSEL,OFF", strlen("SSCFG 3,HFREQSEL,OFF"));
   	if (ibsta & 0x8000) status = 0; 
   
   	//write BERLIIM to the script to the instrument
   	ibwrt (MT8852B_HANDLE, "SSCFG 3,BERLIM,0.1", strlen("SSCFG 3,BERLIM,0.1")); 
   	if (ibsta & 0x8000) status = 0;


   
//////////system setting///////////////////
   
   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "DISCONNECT", strlen("DISCONNECT"));
   	if (ibsta & 0x8000) status = 0;     

   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "SYSCFG AUTH,STATE,OFF", strlen("SYSCFG AUTH,STATE,OFF"));
   	if (ibsta & 0x8000) status = 0;   
   
   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "OPMD SCRIPT", strlen("OPMD SCRIPT"));
   	if (ibsta & 0x8000) status = 0;   

   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "SCPTSEL 3", strlen("SCPTSEL 3"));
   	if (ibsta & 0x8000) status = 0;    

   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "TXPWR 3, -40", strlen("TXPWR 3, -40"));
   	if (ibsta & 0x8000) status = 0;
   
   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "PATHOFF 3, FIXED", strlen("PATHOFF 3, FIXED"));
   	if (ibsta & 0x8000) status = 0; 
   
   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "SYSCFG CONFIG, LKTIMO, 40", strlen("SYSCFG CONFIG, LKTIMO, 40"));
   	if (ibsta & 0x8000) status = 0;   

   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "SYSCFG EUTSRCE, MANUAL", strlen("SYSCFG EUTSRCE, MANUAL"));
   	if (ibsta & 0x8000) status = 0;   
 

   	sprintf (temp_buff, "SYSCFG EUTADDR, %s", MAC_Global_2);	 

   	ibwrt (MT8852B_HANDLE, temp_buff, strlen(temp_buff));

   	if (ibsta & 0x8000) status = 0;


	Delay(1);
	
	
	sprintf(msg,"bt cable loss: %f\n",btoffset);
	WriteToTerminal(panel, msg); 
	
	
	sprintf (temp_buff, "FIXEDOFF 3, -%.01fDB", btoffset);

 
   	ibwrt (MT8852B_HANDLE, temp_buff, strlen(temp_buff));
   	if (ibsta & 0x8000) status = 0;
   
   	Delay(1);
   
   	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);	
	}
	
	return status;


}


//*************************************************************
//*int Set_MT8852B_Loopback_2 (int panel)  	
//* 
//*************************************************************
int Set_MT8852B_Loopback_Mode_2 (int panel) 
{

	int status = 1;
	char msg[256] = {'\0'};
	char temp_buff[256] = {'\0'}; 
	int numToRead;
	int counter = 0;
	char returned_string[256] = {'\0'};
	
	
	//sprintf(msg,"DECT Set MT8852B in Loopback Mode\n");
	//WriteToTerminal(panel, msg); 
	
   	//reset the instrunet
   	ibwrt (MT8852B_HANDLE, "*RST", strlen("*RST")); 
   	if (ibsta & 0x8000) status = 0;
   
   	//clear status of the instrument
   	ibwrt (MT8852B_HANDLE, "*CLS", strlen("*CLS"));
   	if (ibsta & 0x8000) status = 0;

   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "SCPTTSTGP 3,EDRTSTS,OFF", strlen("SCPTTSTGP 3,EDRTSTS,OFF"));
   	if (ibsta & 0x8000) status = 0;     

   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "SCPTTSTGP 3,STDTSTS,ON", strlen("SCPTTSTGP 3,STDTSTS,ON"));
   	if (ibsta & 0x8000) status = 0;   
  
   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "SCPTCFG 3,PC,OFF", strlen("SCPTCFG 3,PC,OFF"));
   	if (ibsta & 0x8000) status = 0;   

   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "SCPTCFG 3,MS,OFF", strlen("SCPTCFG 3,MS,OFF"));
   	if (ibsta & 0x8000) status = 0;    

   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "SCPTCFG 3,MP,OFF", strlen("SCPTCFG 3,MP,OFF"));
   	if (ibsta & 0x8000) status = 0;
   
   	//set operation mode to script mode
   	ibwrt (MT8852B_HANDLE, "OPMD SCRIPT", strlen("OPMD SCRIPT"));
   	if (ibsta & 0x8000) status = 0;
   
   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "SCPTSEL 3", strlen("SCPTSEL 3"));
   	if (ibsta & 0x8000) status = 0;   


////////////output power setting////////////////
   
   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "OPCFG 3,DEFAULT", strlen("OPCFG 3,DEFAULT"));
   	if (ibsta & 0x8000) status = 0;     

   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "OPCFG 3,AVGMXLIM,1.5", strlen("OPCFG 3,AVGMXLIM,1.5"));
   	if (ibsta & 0x8000) status = 0;   
   
   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "OPCFG 3,AVGMNLIM,-4.5", strlen("OPCFG 3,AVGMNLIM,-4.5"));
   	if (ibsta & 0x8000) status = 0;   

   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "OPCFG 3,PEAKLIM,1.5", strlen("OPCFG 3,PEAKLIM,1.5"));
   	if (ibsta & 0x8000) status = 0;    

   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "OPCFG 3,LFREQSEL,OFF", strlen("OPCFG 3,LFREQSEL,OFF"));
   	if (ibsta & 0x8000) status = 0;
   
   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "OPCFG 3,MFREQSEL,ON", strlen("OPCFG 3,MFREQSEL,ON"));
   	if (ibsta & 0x8000) status = 0; 
   
   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "OPCFG 3,HFREQSEL,OFF", strlen("OPCFG 3,HFREQSEL,OFF"));
   	if (ibsta & 0x8000) status = 0;   


////////////////modulation characteristics setting///////////////
   
   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "MICFG 3,DEFAULT", strlen("MICFG 3,DEFAULT"));
   	if (ibsta & 0x8000) status = 0;     

   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "MICFG 3,PKTTYPE,DH5", strlen("MICFG 3,PKTTYPE,DH5"));
   	if (ibsta & 0x8000) status = 0;   
   
   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "MICFG 3,F1AVGMIN,150kHz", strlen("MICFG 3,F1AVGMIN,150kHz"));
   	if (ibsta & 0x8000) status = 0;   

   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "MICFG 3,LFREQSEL,OFF", strlen("MICFG 3,LFREQSEL,OFF"));
   	if (ibsta & 0x8000) status = 0;    

   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "MICFG 3,MFREQSEL,ON", strlen("MICFG 3,MFREQSEL,ON"));
   	if (ibsta & 0x8000) status = 0;
   
   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "MICFG 3,HFREQSEL,OFF", strlen("MICFG 3,HFREQSEL,OFF"));
   	if (ibsta & 0x8000) status = 0;    
   

//////////////carrier frequency drift setting///////////////
   
   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "ICCFG 3,DEFAULT", strlen("ICCFG 3,DEFAULT"));
   	if (ibsta & 0x8000) status = 0;     

   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "ICCFG 3,HOPPING,HOPOFF", strlen("ICCFG 3,HOPPING,HOPOFF"));
   	if (ibsta & 0x8000) status = 0;   
   
   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "ICCFG 3,MXPOSLIM,50 kHz", strlen("ICCFG 3,MXPOSLIM,50 kHz"));
   	if (ibsta & 0x8000) status = 0;   

   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "ICCFG 3,MXNEGLIM,50 kHz", strlen("ICCFG 3,MXNEGLIM,50 kHz"));
   	if (ibsta & 0x8000) status = 0;    

   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "ICCFG 3,LFREQSEL,OFF", strlen("ICCFG 3,LFREQSEL,OFF"));
   	if (ibsta & 0x8000) status = 0;
   
   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "ICCFG 3,MFREQSEL,ON", strlen("ICCFG 3,MFREQSEL,ON"));
   	if (ibsta & 0x8000) status = 0; 
   
   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "ICCFG 3,HFREQSEL,OFF", strlen("ICCFG 3,HFREQSEL,OFF"));
   	if (ibsta & 0x8000) status = 0;   

   
////////////initial carrier frequency tolerance setting//////////////////
   
   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "CDCFG 3,DEFAULT", strlen("CDCFG 3,DEFAULT"));
   	if (ibsta & 0x8000) status = 0;     

   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "CDCFG 3,HOPPING,HOPOFF", strlen("CDCFG 3,HOPPING,HOPOFF"));
   	if (ibsta & 0x8000) status = 0;   
   
   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "CDCFG 3,PKTSIZE,FIVESLOT,TRUE", strlen("CDCFG 3,PKTSIZE,FIVESLOT,TRUE"));
   	if (ibsta & 0x8000) status = 0;   

   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "CDCFG 3,PKTSIZE,THREESLOT,FALSE", strlen("CDCFG 3,PKTSIZE,THREESLOT,FALSE"));
   	if (ibsta & 0x8000) status = 0;    

   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "CDCFG 3,PKTSIZE,ONESLOT,FALSE", strlen("CDCFG 3,PKTSIZE,ONESLOT,FALSE"));
   	if (ibsta & 0x8000) status = 0;
   
   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "CDCFG 3,LFREQSEL,OFF", strlen("CDCFG 3,LFREQSEL,OFF"));
   	if (ibsta & 0x8000) status = 0; 
   
   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "CDCFG 3,MFREQSEL,ON", strlen("CDCFG 3,MFREQSEL,ON"));
   	if (ibsta & 0x8000) status = 0;   

   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "CDCFG 3,HFREQSEL,OFF", strlen("CDCFG 3,HFREQSEL,OFF"));
   	if (ibsta & 0x8000) status = 0;   
   

/////////////sensitivity single slot packets setting/////////////////
   
   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "SSCFG 3,DEFAULT", strlen("SSCFG 3,DEFAULT"));
   	if (ibsta & 0x8000) status = 0;     

   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "SSCFG 3,HOPPING,HOPOFF", strlen("SSCFG 3,HOPPING,HOPOFF"));
   	if (ibsta & 0x8000) status = 0;   
   
   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "SSCFG 3,TXPWR,-76.0", strlen("SSCFG 3,TXPWR,-76.0"));
   	if (ibsta & 0x8000) status = 0;

   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "SSCFG 3,DIRTYTX,ON", strlen("SSCFG 3,DIRTYTX,ON"));
   	if (ibsta & 0x8000) status = 0;    

   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "SSCFG 3,LFREQSEL,OFF", strlen("SSCFG 3,LFREQSEL,OFF"));
   	if (ibsta & 0x8000) status = 0;
   
   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "SSCFG 3,MFREQSEL,ON", strlen("SSCFG 3,MFREQSEL,ON"));
   	if (ibsta & 0x8000) status = 0; 
   
   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "SSCFG 3,HFREQSEL,OFF", strlen("SSCFG 3,HFREQSEL,OFF"));
   	if (ibsta & 0x8000) status = 0; 
   
   	//write BERLIIM to the script to the instrument
   	ibwrt (MT8852B_HANDLE, "SSCFG 3,BERLIM,0.1", strlen("SSCFG 3,BERLIM,0.1")); 
   	if (ibsta & 0x8000) status = 0;


   
//////////system setting///////////////////
   
   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "DISCONNECT", strlen("DISCONNECT"));
   	if (ibsta & 0x8000) status = 0;     

   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "SYSCFG AUTH,STATE,OFF", strlen("SYSCFG AUTH,STATE,OFF"));
   	if (ibsta & 0x8000) status = 0;   
   
   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "OPMD SCRIPT", strlen("OPMD SCRIPT"));
   	if (ibsta & 0x8000) status = 0;   

   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "SCPTSEL 3", strlen("SCPTSEL 3"));
   	if (ibsta & 0x8000) status = 0;    

   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "TXPWR 3, -40", strlen("TXPWR 3, -40"));
   	if (ibsta & 0x8000) status = 0;
   
   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "PATHOFF 3, FIXED", strlen("PATHOFF 3, FIXED"));
   	if (ibsta & 0x8000) status = 0; 
   
   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "SYSCFG CONFIG, LKTIMO, 40", strlen("SYSCFG CONFIG, LKTIMO, 40"));
   	if (ibsta & 0x8000) status = 0;   

   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "SYSCFG EUTSRCE, MANUAL", strlen("SYSCFG EUTSRCE, MANUAL"));
   	if (ibsta & 0x8000) status = 0;   
 

   	sprintf (temp_buff, "SYSCFG EUTADDR, %s", MAC_Global_2);	 

   	ibwrt (MT8852B_HANDLE, temp_buff, strlen(temp_buff));

   	if (ibsta & 0x8000) status = 0;


	Delay(1);
	
		sprintf(msg,"bt cable loss: %f\n",btoffset);
	WriteToTerminal(panel, msg); 
	
	sprintf (temp_buff, "FIXEDOFF 3, -%.01fDB", btoffset);     

 
   	ibwrt (MT8852B_HANDLE, temp_buff, strlen(temp_buff));
   	if (ibsta & 0x8000) status = 0;
   
   	Delay(1);
   
   	/*if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);	
	}*/
	
	return status;


}

//*************************************************************
//*int Set_MT8852B_Loopback_Mode_Radiated (int panel)  	
//* 
//*************************************************************
int Set_MT8852B_Loopback_Mode_Radiated (int panel) 
{

	int status = 1;
	char msg[256] = {'\0'};
	char temp_buff[256] = {'\0'}; 
	int numToRead;
	int counter = 0;
	char returned_string[256] = {'\0'};
	
	
	sprintf(msg,"DECT Set MT8852B in Loopback Mode\n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg);
	WriteToTerminal(panel, msg); 
	
   	//reset the instrunet
   	ibwrt (MT8852B_HANDLE, "*RST", strlen("*RST")); 
   	if (ibsta & 0x8000) status = 0;
   
   	//clear status of the instrument
   	ibwrt (MT8852B_HANDLE, "*CLS", strlen("*CLS"));
   	if (ibsta & 0x8000) status = 0;

   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "SCPTTSTGP 3,EDRTSTS,OFF", strlen("SCPTTSTGP 3,EDRTSTS,OFF"));
   	if (ibsta & 0x8000) status = 0;     

   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "SCPTTSTGP 3,STDTSTS,ON", strlen("SCPTTSTGP 3,STDTSTS,ON"));
   	if (ibsta & 0x8000) status = 0;   
  
   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "SCPTCFG 3,PC,OFF", strlen("SCPTCFG 3,PC,OFF"));
   	if (ibsta & 0x8000) status = 0;   

   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "SCPTCFG 3,MS,OFF", strlen("SCPTCFG 3,MS,OFF"));
   	if (ibsta & 0x8000) status = 0;    

   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "SCPTCFG 3,MP,OFF", strlen("SCPTCFG 3,MP,OFF"));
   	if (ibsta & 0x8000) status = 0;
   
   	//set operation mode to script mode
   	ibwrt (MT8852B_HANDLE, "OPMD SCRIPT", strlen("OPMD SCRIPT"));
   	if (ibsta & 0x8000) status = 0;
   
   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "SCPTSEL 3", strlen("SCPTSEL 3"));
   	if (ibsta & 0x8000) status = 0;   


////////////output power setting////////////////
   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "OPCFG 3,DEFAULT", strlen("OPCFG 3,DEFAULT"));
   	if (ibsta & 0x8000) status = 0;     

   	//write script to the instrument
	ibwrt (MT8852B_HANDLE, "OPCFG 3,AVGMXLIM,-10", strlen("OPCFG 3,AVGMXLIM,-10")); 
   	if (ibsta & 0x8000) status = 0;   
   
   	//write script to the instrument
	ibwrt (MT8852B_HANDLE, "OPCFG 3,AVGMNLIM,-30", strlen("OPCFG 3,AVGMNLIM,-30")); 
   	if (ibsta & 0x8000) status = 0;   

   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "OPCFG 3,PEAKLIM,-10", strlen("OPCFG 3,PEAKLIM,-10"));
   	if (ibsta & 0x8000) status = 0;    

   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "OPCFG 3,LFREQSEL,OFF", strlen("OPCFG 3,LFREQSEL,OFF"));
   	if (ibsta & 0x8000) status = 0;
   
   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "OPCFG 3,MFREQSEL,ON", strlen("OPCFG 3,MFREQSEL,ON"));
   	if (ibsta & 0x8000) status = 0; 
   
   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "OPCFG 3,HFREQSEL,OFF", strlen("OPCFG 3,HFREQSEL,OFF"));
   	if (ibsta & 0x8000) status = 0;   


/////////////////////////////		//NEW
   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "PATHOFF 3, FIXED", strlen("PATHOFF 3, FIXED"));
   	if (ibsta & 0x8000) status = 0; 
	
	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "FIXEDOFF 3, -10.00DB", strlen("FIXEDOFF 3, -10.00DB"));
   	if (ibsta & 0x8000) status = 0; 
/////////////////////////

   	sprintf (temp_buff, "SYSCFG EUTADDR, %s", MAC_Global_2);	 

   	ibwrt (MT8852B_HANDLE, temp_buff, strlen(temp_buff));

   	if (ibsta & 0x8000) status = 0;


	Delay(1);

 
   	//ibwrt (MT8852B_HANDLE, "FIXEDOFF 3, -5.00DB", strlen("FIXEDOFF 3, -5.00DB"));
  // 	if (ibsta & 0x8000) status = 0;
   
   	Delay(1);
   
   	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);	
	}
	
	return status;


}

//*************************************************************
//*int Set_MT8852B_Loopback_Mode_Radiated_2 (int panel)  	
//* 
//*************************************************************
int Set_MT8852B_Loopback_Mode_Radiated_2 (int panel) 
{

	int status = 1;
	char msg[256] = {'\0'};
	char temp_buff[256] = {'\0'}; 
	int numToRead;
	int counter = 0;
	char returned_string[256] = {'\0'};
	
	
   	//reset the instrunet
   	ibwrt (MT8852B_HANDLE, "*RST", strlen("*RST")); 
   	if (ibsta & 0x8000) status = 0;
   
   	//clear status of the instrument
   	ibwrt (MT8852B_HANDLE, "*CLS", strlen("*CLS"));
   	if (ibsta & 0x8000) status = 0;

   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "SCPTTSTGP 3,EDRTSTS,OFF", strlen("SCPTTSTGP 3,EDRTSTS,OFF"));
   	if (ibsta & 0x8000) status = 0;     

   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "SCPTTSTGP 3,STDTSTS,ON", strlen("SCPTTSTGP 3,STDTSTS,ON"));
   	if (ibsta & 0x8000) status = 0;   
  
   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "SCPTCFG 3,PC,OFF", strlen("SCPTCFG 3,PC,OFF"));
   	if (ibsta & 0x8000) status = 0;   

   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "SCPTCFG 3,MS,OFF", strlen("SCPTCFG 3,MS,OFF"));
   	if (ibsta & 0x8000) status = 0;    

   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "SCPTCFG 3,MP,OFF", strlen("SCPTCFG 3,MP,OFF"));
   	if (ibsta & 0x8000) status = 0;
   
   	//set operation mode to script mode
   	ibwrt (MT8852B_HANDLE, "OPMD SCRIPT", strlen("OPMD SCRIPT"));
   	if (ibsta & 0x8000) status = 0;
   
   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "SCPTSEL 3", strlen("SCPTSEL 3"));
   	if (ibsta & 0x8000) status = 0;   


////////////output power setting////////////////
   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "OPCFG 3,DEFAULT", strlen("OPCFG 3,DEFAULT"));
   	if (ibsta & 0x8000) status = 0;     

   	//write script to the instrument
	ibwrt (MT8852B_HANDLE, "OPCFG 3,AVGMXLIM,-10", strlen("OPCFG 3,AVGMXLIM,-10")); 
   	if (ibsta & 0x8000) status = 0;   
   
   	//write script to the instrument
	ibwrt (MT8852B_HANDLE, "OPCFG 3,AVGMNLIM,-30", strlen("OPCFG 3,AVGMNLIM,-30")); 
   	if (ibsta & 0x8000) status = 0;   

   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "OPCFG 3,PEAKLIM,-10", strlen("OPCFG 3,PEAKLIM,-10"));
   	if (ibsta & 0x8000) status = 0;    

   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "OPCFG 3,LFREQSEL,OFF", strlen("OPCFG 3,LFREQSEL,OFF"));
   	if (ibsta & 0x8000) status = 0;
   
   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "OPCFG 3,MFREQSEL,ON", strlen("OPCFG 3,MFREQSEL,ON"));
   	if (ibsta & 0x8000) status = 0; 
   
   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "OPCFG 3,HFREQSEL,OFF", strlen("OPCFG 3,HFREQSEL,OFF"));
   	if (ibsta & 0x8000) status = 0;   
	
	
/////////////////////////////		//NEW
   	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "PATHOFF 3, FIXED", strlen("PATHOFF 3, FIXED"));
   	if (ibsta & 0x8000) status = 0; 
	
	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "FIXEDOFF 3, -10.00DB", strlen("FIXEDOFF 3, -10.00DB"));
   	if (ibsta & 0x8000) status = 0; 
   
/////////////////////////

   	sprintf (temp_buff, "SYSCFG EUTADDR, %s", MAC_Global_2);	 

   	ibwrt (MT8852B_HANDLE, temp_buff, strlen(temp_buff));

   	if (ibsta & 0x8000) status = 0;


	Delay(1);

 
   //	ibwrt (MT8852B_HANDLE, "FIXEDOFF 3, -5.00DB", strlen("FIXEDOFF 3, -5.00DB"));
   	//if (ibsta & 0x8000) status = 0;
   
   	Delay(1);
	
	return status;


}
//*************************************************************
//*int Run_Scripts_MT8852B (int panel)  	
//* 
//*************************************************************
int Run_Scripts_MT8852B (int panel)
{

	int status = 1;
	char msg[256] = {'\0'};
	char temp_buff[240] = {'\0'};
	char temp_buff_2[240] = {'\0'};
	int numToRead;
	int counter = 0;
	int counter_2 = 0;
	int INS_Returned_Value = 0;
	int INS_Returned_Value_2 = 0;
	int INS_Returned_Value_3 = 0;
	char returned_string[256] = {'\0'};
	int retry_flag = 0;
	int counter_3 = 0;

	sprintf(msg,"DECT Connect and Run Bluetooth Scripts MT8852B Test\n");
	WriteToTerminal(panel, msg);
	ibwrt (MT8852B_HANDLE, "EUTMAXPWR 10,ON", strlen("EUTMAXPWR 10,ON"));   //new
	if (ibsta & 0x8000) status = 0;


	//write to the instrument
	ibwrt (MT8852B_HANDLE, "RUN", strlen("RUN"));
	if (ibsta & 0x8000) status = 0;

	sprintf(msg,".");
	WriteToTerminal(panel, msg);
	ProcessSystemEvents ();

	while((counter < 30) && (INS_Returned_Value_2 != 13))
	{
		ibwrt (MT8852B_HANDLE, "*INS?", strlen("*INS?"));
		if (ibsta & 0x8000) status = 0;

		//read the instrument
		numToRead = 8;
		ibrd (MT8852B_HANDLE, temp_buff, numToRead);
		if (ibsta & 0x8000) status = 0;

		Delay(2);

		INS_Returned_Value = atof (temp_buff);
		INS_Returned_Value_2 = (INS_Returned_Value & 0x0f);

		sprintf(msg,".");
		WriteToTerminal(panel, msg);
		ProcessSystemEvents ();

		counter++;
	}

	sprintf(msg,"%d,",counter); 	//running logfile
	strcat(data_string,msg);


	if(((Test_Type == 1)||(Test_Type == 2)||(Test_Type == 4)||(Test_Type == 5))&&(((counter <= 5)||(counter >= 30))))  //fail retry
	{

		sprintf(msg,"\nRetry run Scripts\n");
		WriteToTerminal(panel, msg);
		ProcessSystemEvents ();
		retry_flag = 1;

		sprintf(msg,"关闭MT8852B蓝牙测试仪然后按OK.\n按下OK后程序还会等待45秒.\n\nPower off, power on MT8852B then press OK");
		Display_OK_MSG(msg);

		//Init BT Tester
		//Delay(45);

		MT8852B_HANDLE = 0;


		MT8852B_HANDLE = ibdev (0, MT8852B_GPIB, NO_SAD, T10s, 1, 0);
		if (ibsta & 0x8000) status = 0;


		ibwrt (MT8852B_HANDLE, "BOOTSTATUS?", strlen("BOOTSTATUS?"));  //new
		if (ibsta & 0x8000) status = 0;

		//read the instrument Boot status
		numToRead = 1;
		ibrd (MT8852B_HANDLE, temp_buff, numToRead);
		if (ibsta & 0x8000) status = 0;

		ibwrt (MT8852B_HANDLE, "ERRLST", strlen("ERRLST"));
		if (ibsta & 0x8000) status = 0;

		//read the instrument Error List
		numToRead = 50;
		ibrd (MT8852B_HANDLE, temp_buff, numToRead);
		if (ibsta & 0x8000) status = 0;

		ibwrt (MT8852B_HANDLE, "IFC", strlen("IFC"));
		if (ibsta & 0x8000) status = 0;

		//send device clear to the instrunet
		ibwrt (MT8852B_HANDLE, "DCL", strlen("DCL"));
		if (ibsta & 0x8000) status = 0;

		//clear the device
		ibclr (MT8852B_HANDLE);
		if (ibsta & 0x8000) status = 0;

		//clear status of the instrument
		ibwrt (MT8852B_HANDLE, "*CLS", strlen("*CLS"));
		if (ibsta & 0x8000) status = 0;

		//reset the instrument
		ibwrt (MT8852B_HANDLE, "*RST", strlen("*RST"));
		if (ibsta & 0x8000) status = 0;

		//write command to read the instrument id
		ibwrt (MT8852B_HANDLE, "*IDN?", strlen("*IDN?"));
		if (ibsta & 0x8000) status = 0;

		//read the instrument id
		numToRead = 200;
		ibrd (MT8852B_HANDLE, temp_buff, numToRead);
		if (ibsta & 0x8000) status = 0;

		//to bring CMD60 to initial state after power on, you send the following
		//send interface clear to the instrument
		ibwrt (MT8852B_HANDLE, "IFC", strlen("IFC"));
		if (ibsta & 0x8000) status = 0;

		//send device clear to the instrunet
		ibwrt (MT8852B_HANDLE, "DCL", strlen("DCL"));
		if (ibsta & 0x8000) status = 0;

		//clear the device
		ibclr (MT8852B_HANDLE);
		if (ibsta & 0x8000) status = 0;

		//clear status of the instrument
		ibwrt (MT8852B_HANDLE, "*CLS", strlen("*CLS"));
		if (ibsta & 0x8000) status = 0;

		//reset the instrunet
		ibwrt (MT8852B_HANDLE, "*RST", strlen("*RST"));
		if (ibsta & 0x8000) status = 0;


		ibwrt (MT8852B_HANDLE, "EUTMAXPWR 10,ON", strlen("EUTMAXPWR 10,ON"));   //new
		if (ibsta & 0x8000) status = 0;

		//////////////////////////////////////////////////
		//re-enter BT Mode
		status = 0;
		counter_2 = 0;
		while((counter_2 < 10) && (!(status == 1)))
		{
			status = DECT_Send_String_USB (panel,EnterTestMode,EnterTestMode_Response,returned_string,6,1,1);	  //needs to be 6 to support 7.2
			Delay(1);
			counter_2++;
		}

		status = 0;
		counter_2 = 0;
		while((counter_2 < 10) && (!(status == 1)))
		{
			status = DECT_Send_String_USB (panel,BtEnterDutMode,BtEnterDutMode_Response,returned_string,0,1,1);
			Delay(1);
			counter_2 ++;
		}

		status = 0;
		counter_2 = 0;
		while((counter_2 < 10) && (!(status == 1)))
		{
			status = DECT_Send_String_USB (panel,BtEnterDutMode,BtEnterDutMode_Response,returned_string,0,1,1);
			Delay(1);
			counter_2 ++;
		}

		Delay(10);

		////////////////////////////////////////////
		status = Check_Sync_MT8852B_2(panel);

		Delay(5);

		if((Test_Type == 1)||(Test_Type == 2))
		{
			Set_MT8852B_Loopback_Mode_2 (panel);
			Delay(5);
		}
		else if((Test_Type == 4)||(Test_Type == 5))
		{
			Set_MT8852B_Loopback_Mode_Radiated_2(panel);
			Delay(5);
		}

		//write to the instrument

		ibwrt (MT8852B_HANDLE, "RUN", strlen("RUN"));
		if (ibsta & 0x8000) status = 0;

		sprintf(msg,".");
		WriteToTerminal(panel, msg);
		ProcessSystemEvents ();

		counter = 0;
		INS_Returned_Value_3 = 0;

		while((counter < 30) && (INS_Returned_Value_3 != 13))
		{
			ibwrt (MT8852B_HANDLE, "*INS?", strlen("*INS?"));
			if (ibsta & 0x8000) status = 0;

			//read the instrument
			numToRead = 8;
			ibrd (MT8852B_HANDLE, temp_buff_2, numToRead);
			if (ibsta & 0x8000) status = 0;

			Delay(2);

			INS_Returned_Value = atof (temp_buff_2);
			INS_Returned_Value_3 = (INS_Returned_Value & 0x0f);

			sprintf(msg,".");
			WriteToTerminal(panel, msg);
			ProcessSystemEvents ();

			counter++;
		}

	}


	if(retry_flag == 1)
	{
		sprintf(msg,"\nMT8852B Returned Value: %d", INS_Returned_Value_3);
		WriteToTerminal(panel, msg);
	}


	if (((INS_Returned_Value_2 == 13)||(INS_Returned_Value_3 == 13))&&(counter!= 0))
	{
		sprintf(msg,"\nTest Pass\n\n");
		WriteToTerminal(panel, msg);
		status = 1;
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else
	{
		sprintf(msg,"\nTest Fail\n\n");
		WriteToTerminal(panel, msg);
		status = 0;
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);
	}

	return status;
}


//*************************************************************
//*int Measure_Output_Power_MT8852B (int panel)  	
//* 
//*************************************************************
int Measure_Output_Power_MT8852B (int panel)  
{

	int status = 1;
	char msg[256] = {'\0'};
	char temp_buff[24] = {'\0'}; 
	int numToRead;
	int counter = 0;
	char returned_string[256] = {'\0'};
	char temp_array[256]  = {'\0'}; 
	int i = 0;
	
	
	sprintf(msg,"DECT Measure Output Power MT8852B Test\n");
	WriteToTerminal(panel, msg); 
	
	
	for (i = 0; i <= 5; i++){
		//write script to the instrument
	   	ibwrt (MT8852B_HANDLE, "XRESULT OP, HOPONM", strlen("XRESULT OP, HOPONM"));
	   	if (ibsta & 0x8000) status =0; 
	
		Delay(1);

	   	numToRead = 200;
		ibrd (MT8852B_HANDLE, returned_string, numToRead);
	   	if (ibsta & 0x8000) status = 0; 
	}		
	
	
	sprintf (msg, "%s\n", returned_string); 
	WriteToTerminal(panel, returned_string); 
	
	if (strstr (returned_string, "PASS")){
		status = 1;
	}
	else{
	   	status = 0;
	}
	
	
	i = 0;
	
	while((returned_string[i] != 10)&&(i < 255)){
		
		if(returned_string[i] != 44){
			temp_array[i] = returned_string[i];
		}
		else if(returned_string[i] == 44){
			temp_array[i] = 58;	
		}
						
		i++;				
	}
	
	sprintf(msg,"%s,",temp_array);
	strcat(data_string,msg);
	
	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		//SetCtrlVal (panel, PANEL_Serial_Window_1, "Test Pass\n\n");
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		//SetCtrlVal (panel, PANEL_Serial_Window_1, "Test Fail\n\n");	
	}
	
	return status;

}




/*

 // output power//////////////////////////////////////////////////////////////////////////////////  
   //write script to the instrument
   ibwrt (MT8852B_HANDLE, "XRESULT OP, HOPONM", strlen("XRESULT OP, HOPONM"));
   if (ibsta & 0x8000) MT8852B_Error = TRUE; 

   numToRead = 200;
   ibrd (MT8852B_HANDLE, OP_Buff, numToRead);
   CHANNEL_RESULTS[1][0].BT_Output_Power =  atof (OP_Buff);
   
   if (ibsta & 0x8000) 
   {
	   
	MT8852B_Error = TRUE;
	CHANNEL_RESULTS[1][0].BT_Output_Power_Test = FALSE;
	   
   }
   	else 
	{
		
	CHANNEL_RESULTS[1][0].BT_Output_Power_Test = TRUE;
		
	}
	
    pt = strtok (OP_Buff,",");
	
	Counter = 0;
	
	while (pt != NULL) {
		
		OP_Buff[0] = NULL;
		strcpy (OP_Data[Counter], pt);
		pt = strtok (NULL, ",");
		Counter = Counter + 1;
		
	}

	//copy the bt tester's response to bt variables
	strcpy (CHANNEL_RESULTS[1][0].BT_Output_Power_Valid_Stage, OP_Data[1]);
	strcpy (CHANNEL_RESULTS[1][0].BT_Output_Power_Results_Valid, OP_Data[2]);
	CHANNEL_RESULTS[1][0].BT_Output_Power_Max =  atof (OP_Data[3]);
	CHANNEL_RESULTS[1][0].BT_Output_Power_Min =  atof (OP_Data[4]);
	CHANNEL_RESULTS[1][0].BT_Output_Power_Max_Step =  atof (OP_Data[5]);
	CHANNEL_RESULTS[1][0].BT_Output_Power_Min_Step =  atof (OP_Data[6]);
	CHANNEL_RESULTS[1][0].BT_Output_Power_Failed =  atoi (OP_Data[7]);
	CHANNEL_RESULTS[1][0].BT_Output_Power_Tested =  atoi (OP_Data[8]);
	strcpy (CHANNEL_RESULTS[1][0].BT_Output_Power_State, OP_Data[9]);
	
	Comp_Error_Num = COMPARE_STRINGS (4, "PASS", CHANNEL_RESULTS[1][0].BT_Output_Power_State);
	if (Comp_Error_Num)  MT8852B_Error = TRUE;
	
*/

//*************************************************************
//*int Measure_Modulation_Index_MT8852B (int panel)   	
//* 
//*************************************************************
int Measure_Modulation_Index_MT8852B (int panel)  
{

	int status = 1;
	char msg[256] = {'\0'};
	char temp_buff[24] = {'\0'}; 
	int numToRead;
	int counter = 0;
	char returned_string[256] = {'\0'};
	char temp_array[256]  = {'\0'}; 
	int i = 0;
	
	
	
	sprintf(msg,"DECT Measure Modulation Index MT8852B Test\n");
	WriteToTerminal(panel, msg); 
	

	//write script to the instrument
	for (i = 0; i <= 5; i++){ 
	   	ibwrt (MT8852B_HANDLE, "XRESULT MI, HOPOFFM", strlen("XRESULT MI, HOPOFFM"));  
	   	if (ibsta & 0x8000) status =0; 
	
		Delay(1); 

	   	numToRead = 200;
		ibrd (MT8852B_HANDLE, returned_string, numToRead);
	   	if (ibsta & 0x8000) status = 0; 
	}		
	sprintf (msg, "%s\n", returned_string); 
	WriteToTerminal(panel, returned_string);
	
	
	if (strstr (returned_string, "PASS")){
		status = 1;
	}
	else{
	   	status = 0;
	}
	
	
	i = 0;
	
	while((returned_string[i] != 10)&&(i < 255)){
		
		if(returned_string[i] != 44){
			temp_array[i] = returned_string[i];
		}
		else if(returned_string[i] == 44){
			temp_array[i] = 58;	
		}
						
		i++;				
	}
	
	sprintf(msg,"%s,",temp_array);
	strcat(data_string,msg);
	
	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		//SetCtrlVal (panel, PANEL_Serial_Window_1, "Test Pass\n\n");
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		//SetCtrlVal (panel, PANEL_Serial_Window_1, "Test Fail\n\n");	
	}
	
	return status;

}



/*//modulation index (characteristics)//////////////////////////////////////////////////////////////////////////////////
   //write script to the instrument
   ibwrt (MT8852B_HANDLE, "XRESULT MI, HOPOFFM", strlen("XRESULT MI, HOPOFFM"));
   if (ibsta & 0x8000) MT8852B_Error = TRUE; 
   
   numToRead = 200;
   ibrd (MT8852B_HANDLE, MI_Buff, numToRead);
   CHANNEL_RESULTS[1][0].BT_Modulation_Characteristics =  atof (MI_Buff);
   
   if (ibsta & 0x8000) 
   {
	   
	MT8852B_Error = TRUE;
	CHANNEL_RESULTS[1][0].BT_Modulation_Characteristics_Test = FALSE;
	   
   }
   	else 
	{
		
	CHANNEL_RESULTS[1][0].BT_Modulation_Characteristics_Test = TRUE;
		
	}
	
    pt = strtok (MI_Buff,",");
	
	Counter = 0;
	
	while (pt != NULL) {
		
		MI_Buff[0] = NULL;
		strcpy (MI_Data[Counter], pt);
		pt = strtok (NULL, ",");
		Counter = Counter + 1;
		
	}
	
	//copy the bt tester's response to bt variables
	strcpy (CHANNEL_RESULTS[1][0].BT_Modulation_Characteristics_Valid_Stage, MI_Data[1]);
	strcpy (CHANNEL_RESULTS[1][0].BT_Modulation_Characteristics_Results_Valid, MI_Data[2]);
	CHANNEL_RESULTS[1][0].BT_Modulation_Characteristics_F1_Max =  atof (MI_Data[3]);
	CHANNEL_RESULTS[1][0].BT_Modulation_Characteristics_F1_Avg =  atof (MI_Data[4]);
	CHANNEL_RESULTS[1][0].BT_Modulation_Characteristics_F2_Max =  atof (MI_Data[5]);
	CHANNEL_RESULTS[1][0].BT_Modulation_Characteristics_F2_Avg =  atof (MI_Data[6]);
	CHANNEL_RESULTS[1][0].BT_Modulation_Characteristics_F2_AvgOverF1_Avg =  atoi (MI_Data[7]);
	CHANNEL_RESULTS[1][0].BT_Modulation_Characteristics_F2_Max_Failed =  atoi (MI_Data[8]);
	CHANNEL_RESULTS[1][0].BT_Modulation_Characteristics_F2_Max_Count =  atoi (MI_Data[9]);
	CHANNEL_RESULTS[1][0].BT_Modulation_Characteristics_Failed =  atoi (MI_Data[10]);
	CHANNEL_RESULTS[1][0].BT_Modulation_Characteristics_Tested =  atoi (MI_Data[11]);
	strcpy (CHANNEL_RESULTS[1][0].BT_Modulation_Characteristics_State, MI_Data[12]);
	
	Comp_Error_Num = COMPARE_STRINGS (4, "PASS", CHANNEL_RESULTS[1][0].BT_Modulation_Characteristics_State);
	if (Comp_Error_Num)  MT8852B_Error = TRUE;	
	
*/




//*************************************************************
//*int Measure_Carrier_Frequency_Tolerance_MT8852B (int panel)   	
//* 
//*************************************************************
int Measure_Carrier_Frequency_Tolerance_MT8852B (int panel) 
{

	int status = 1;
	char msg[256] = {'\0'};
	char temp_buff[24] = {'\0'}; 
	int numToRead;
	int counter = 0;
	char returned_string[256] = {'\0'};
	char temp_array[256]  = {'\0'}; 
	int i = 0;
	
	
	sprintf(msg,"DECT Measure Carrier Frequency Tolerance MT8852B Test\n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg);
	WriteToTerminal(panel, msg); 
	

	//write script to the instrument
	for (i = 0; i <= 5; i++){
	   	ibwrt (MT8852B_HANDLE, "XRESULT IC, HOPOFFM", strlen("XRESULT IC, HOPOFFM")); 
	   	if (ibsta & 0x8000) status =0; 
	
		Delay(1); 

	   	numToRead = 200;
		ibrd (MT8852B_HANDLE, returned_string, numToRead);
	   	if (ibsta & 0x8000) status = 0; 
	}	
	
	sprintf (msg, "%s\n", returned_string); 
	WriteToTerminal(panel, returned_string); 
	
	
	if (strstr (returned_string, "PASS")){
		status = 1;
	}
	else{
	   	status = 0;
	}
	
	
	i = 0;
	
	while((returned_string[i] != 10)&&(i < 255)){
		
		if(returned_string[i] != 44){
			temp_array[i] = returned_string[i];
		}
		else if(returned_string[i] == 44){
			temp_array[i] = 58;	
		}
						
		i++;				
	}
	
	sprintf(msg,"%s,",temp_array);
	strcat(data_string,msg);
	
	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		//SetCtrlVal (panel, PANEL_Serial_Window_1, "Test Pass\n\n");
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		//SetCtrlVal (panel, PANEL_Serial_Window_1, "Test Fail\n\n");	
	}
	
	return status;

}

/*//intial carrier frequency tolerance//////////////////////////////////////////////////////////////////////////////////   
   //write script to the instrument
   ibwrt (MT8852B_HANDLE, "XRESULT IC, HOPOFFM", strlen("XRESULT IC, HOPOFFM"));
   if (ibsta & 0x8000) MT8852B_Error = TRUE;
   
   numToRead = 200;
   ibrd (MT8852B_HANDLE, IC_Buff, numToRead);
   CHANNEL_RESULTS[1][0].BT_Initial_Carrier_Frequency_Tolerance =  atof (IC_Buff);
   if (ibsta & 0x8000) 
   {
	   
   MT8852B_Error = TRUE;
   CHANNEL_RESULTS[1][0].BT_Initial_Carrier_Frequency_Tolerance_Test = FALSE;
	   
   }
   	else 
	{
		
	CHANNEL_RESULTS[1][0].BT_Initial_Carrier_Frequency_Tolerance_Test = TRUE;
		
	}
	
    pt = strtok (IC_Buff,",");
	
	Counter = 0;
	
	while (pt != NULL) {
		
		IC_Buff[0] = NULL;
		strcpy (IC_Data[Counter], pt);
		pt = strtok (NULL, ",");
		Counter = Counter + 1;
		
	}

	//copy the bt tester's response to bt variables
	strcpy (CHANNEL_RESULTS[1][0].BT_Initial_Carrier_Frequency_Tolerance_Valid_Stage, IC_Data[1]);
	strcpy (CHANNEL_RESULTS[1][0].BT_Initial_Carrier_Frequency_Tolerance_Results_Valid, IC_Data[2]);
	CHANNEL_RESULTS[1][0].BT_Initial_Carrier_Frequency_Tolerance_Avg_OffSet =  atof (IC_Data[3]);
	CHANNEL_RESULTS[1][0].BT_Initial_Carrier_Frequency_Tolerance_Max_Pos_OffSet =  atof (IC_Data[4]);
	CHANNEL_RESULTS[1][0].BT_Initial_Carrier_Frequency_Tolerance_Max_Neg_OffSet =  atof (IC_Data[5]);
	CHANNEL_RESULTS[1][0].BT_Initial_Carrier_Frequency_Tolerance_Failed =  atoi (IC_Data[6]);
	CHANNEL_RESULTS[1][0].BT_Initial_Carrier_Frequency_Tolerance_Tested =  atoi (IC_Data[7]);
	strcpy (CHANNEL_RESULTS[1][0].BT_Initial_Carrier_Frequency_Tolerance_State, IC_Data[8]);
	
	Comp_Error_Num = COMPARE_STRINGS (4, "PASS", CHANNEL_RESULTS[1][0].BT_Initial_Carrier_Frequency_Tolerance_State);
	if (Comp_Error_Num)  MT8852B_Error = TRUE;	
*/


//*************************************************************
//*int Measure_Carrier_Frequency_Drift_MT8852B (int panel)   	
//* 
//*************************************************************
int Measure_Carrier_Frequency_Drift_MT8852B (int panel) 
{

	int status = 1;
	char msg[256] = {'\0'};
	char temp_buff[24] = {'\0'}; 
	int numToRead;
	int counter = 0;
	char returned_string[256] = {'\0'};
	char temp_array[256]  = {'\0'}; 
	int i = 0;
	
	sprintf(msg,"DECT Measure Carrier Frequency Drift MT8852B Test\n");
	WriteToTerminal(panel, msg); 
	

	//write script to the instrument
	for (i = 0; i <= 5; i++){ 
	   	ibwrt (MT8852B_HANDLE, "XRESULT CD, HOPOFFM", strlen("XRESULT CD, HOPOFFM")); 
	   	if (ibsta & 0x8000) status =0; 
	
		Delay(1); 

	   	numToRead = 200;
		ibrd (MT8852B_HANDLE, returned_string, numToRead);
	   	if (ibsta & 0x8000) status = 0; 
	}		
	
	sprintf (msg, "%s\n", returned_string); 
	WriteToTerminal(panel, returned_string);

	
	if (strstr (returned_string, "PASS")){
		status = 1;
	}
	else{
	   	status = 0;
	}
	
	
	i = 0;
	
	while((returned_string[i] != 10)&&(i < 255)){
		
		if(returned_string[i] != 44){
			temp_array[i] = returned_string[i];
		}
		else if(returned_string[i] == 44){
			temp_array[i] = 58;	
		}
						
		i++;				
	}
	
	sprintf(msg,"%s,",temp_array);
	strcat(data_string,msg);
	
	
	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		//SetCtrlVal (panel, PANEL_Serial_Window_1, "Test Pass\n\n");
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		//SetCtrlVal (panel, PANEL_Serial_Window_1, "Test Fail\n\n");	
	}
	
	return status;

}



/*//carrier frequency drift//////////////////////////////////////////////////////////////////////////////////	
   //write script to the instrument
   ibwrt (MT8852B_HANDLE, "XRESULT CD, HOPOFFM", strlen("XRESULT CD, HOPOFFM"));
   if (ibsta & 0x8000) MT8852B_Error = TRUE;
   
   
   numToRead = 200;
   ibrd (MT8852B_HANDLE, CD_Buff, numToRead);
   CHANNEL_RESULTS[1][0].BT_Carrier_Frequency_Drift =  atof (CD_Buff);
   if (ibsta & 0x8000) 
   {
   MT8852B_Error = TRUE;
   CHANNEL_RESULTS[1][0].BT_Carrier_Frequency_Drift_Test = FALSE;
	   
   }
   	else 
	{
		
	CHANNEL_RESULTS[1][0].BT_Carrier_Frequency_Drift_Test = TRUE;
		
	}
   
    pt = strtok (CD_Buff,",");
	
	Counter = 0;
	
	while (pt != NULL) {
		
		CD_Buff[0] = NULL;
		strcpy (CD_Data[Counter], pt);
		pt = strtok (NULL, ",");
		Counter = Counter + 1;
		
	}
	

	//copy the bt tester's response to bt variables
	strcpy (CHANNEL_RESULTS[1][0].BT_Carrier_Frequency_Drift_Valid_Stage, CD_Data[1]);
	strcpy (CHANNEL_RESULTS[1][0].BT_Carrier_Frequency_Drift_DH1_Results_Valid, CD_Data[2]);
	CHANNEL_RESULTS[1][0].BT_Carrier_Frequency_Drift_Max_Rate_DH1 =  atof (CD_Data[3]);
	CHANNEL_RESULTS[1][0].BT_Carrier_Frequency_Drift_Max_Drift_DH1 =  atof (CD_Data[4]);
	CHANNEL_RESULTS[1][0].BT_Carrier_Frequency_Drift_Avg_Drift_DH1 =  atof (CD_Data[5]);
	CHANNEL_RESULTS[1][0].BT_Carrier_Frequency_Drift_DH1_Failed =  atoi (CD_Data[6]);
	CHANNEL_RESULTS[1][0].BT_Carrier_Frequency_Drift_DH1_Tested =  atoi (CD_Data[7]);
	strcpy (CHANNEL_RESULTS[1][0].BT_Carrier_Frequency_Drift_DH1_State, CD_Data[8]);
	strcpy (CHANNEL_RESULTS[1][0].BT_Carrier_Frequency_Drift_DH3_Results_Valid, CD_Data[9]);
	CHANNEL_RESULTS[1][0].BT_Carrier_Frequency_Drift_Max_Rate_DH3 =  atof (CD_Data[10]);
	CHANNEL_RESULTS[1][0].BT_Carrier_Frequency_Drift_Max_Drift_DH3 =  atof (CD_Data[11]);
	CHANNEL_RESULTS[1][0].BT_Carrier_Frequency_Drift_Avg_Drift_DH3 =  atof (CD_Data[12]);
	CHANNEL_RESULTS[1][0].BT_Carrier_Frequency_Drift_DH3_Failed =  atoi (CD_Data[13]);
	CHANNEL_RESULTS[1][0].BT_Carrier_Frequency_Drift_DH3_Tested =  atoi (CD_Data[14]);
	strcpy (CHANNEL_RESULTS[1][0].BT_Carrier_Frequency_Drift_DH3_State, CD_Data[15]);	
	strcpy (CHANNEL_RESULTS[1][0].BT_Carrier_Frequency_Drift_DH5_Results_Valid, CD_Data[16]);
	CHANNEL_RESULTS[1][0].BT_Carrier_Frequency_Drift_Max_Rate_DH5 =  atof (CD_Data[17]);
	CHANNEL_RESULTS[1][0].BT_Carrier_Frequency_Drift_Max_Drift_DH5 =  atof (CD_Data[18]);
	CHANNEL_RESULTS[1][0].BT_Carrier_Frequency_Drift_Avg_Drift_DH5 =  atof (CD_Data[19]);
	CHANNEL_RESULTS[1][0].BT_Carrier_Frequency_Drift_DH5_Failed =  atoi (CD_Data[20]);
	CHANNEL_RESULTS[1][0].BT_Carrier_Frequency_Drift_DH5_Tested =  atoi (CD_Data[21]);
	strcpy (CHANNEL_RESULTS[1][0].BT_Carrier_Frequency_Drift_DH5_State, CD_Data[22]);

	Comp_Error_Num = COMPARE_STRINGS (4, "PASS", CHANNEL_RESULTS[1][0].BT_Carrier_Frequency_Drift_DH5_State);
	if (Comp_Error_Num)  MT8852B_Error = TRUE;		
*/	
	
	
//*************************************************************
//*int Measure_Sensitive_Single_Slot_Packets_MT8852B (int panel)   	
//* 
//*************************************************************
int Measure_Sensitive_Single_Slot_Packets_MT8852B (int panel) 
{

	int status = 1;
	char msg[256] = {'\0'};
	char temp_buff[24] = {'\0'}; 
	int numToRead;
	int counter = 0;
	char returned_string[256] = {'\0'};
	char temp_array[256]  = {'\0'}; 
	int i = 0;
	
	
	sprintf(msg,"DECT Measure Sensitive Single Slot Packets MT8852B Test\n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg);
	WriteToTerminal(panel, msg); 
	

	//write script to the instrument
	for (i = 0; i <= 5; i++){  
	   	ibwrt (MT8852B_HANDLE, "XRESULT SS, HOPOFFM", strlen("XRESULT SS, HOPOFFM"));
	   	if (ibsta & 0x8000) status =0; 
	
		Delay(1); 

	   	numToRead = 200;
		ibrd (MT8852B_HANDLE, returned_string, numToRead);
	   	if (ibsta & 0x8000) status = 0; 
	}
		
	sprintf (msg, "%s\n", returned_string); 
	WriteToTerminal(panel, returned_string); 

	
	if (strstr (returned_string, "PASS")){
		status = 1;
	}
	else{
	   	status = 0;
	}
	
	i = 0;
	
	while((returned_string[i] != 10)&&(i < 255)){
		
		if(returned_string[i] != 44){
			temp_array[i] = returned_string[i];
		}
		else if(returned_string[i] == 44){
			temp_array[i] = 58;	
		}
						
		i++;				
	}
	
	sprintf(msg,"%s,",temp_array);
	strcat(data_string,msg);
	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		//SetCtrlVal (panel, PANEL_Serial_Window_1, "Test Pass\n\n");
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		//SetCtrlVal (panel, PANEL_Serial_Window_1, "Test Fail\n\n");	
	}
	
	return status;

}

/*//sensetive single-slot packet//////////////////////////////////////////////////////////////////////////////////	
   //write script to the instrument
   ibwrt (MT8852B_HANDLE, "XRESULT SS, HOPOFFM", strlen("XRESULT SS, HOPOFFM"));
   if (ibsta & 0x8000) MT8852B_Error = TRUE;   

   
   numToRead = 200;
   ibrd (MT8852B_HANDLE, SS_Buff, numToRead);
   CHANNEL_RESULTS[1][0].BT_Sensitive_Single_Slot_Packets =  atof (SS_Buff);
   if (ibsta & 0x8000) 
   {
   MT8852B_Error = TRUE;
   CHANNEL_RESULTS[1][0].BT_Sensitive_Single_Slot_Packets_Test = FALSE;
	   
   }
   	else 
	{
		
	CHANNEL_RESULTS[1][0].BT_Sensitive_Single_Slot_Packets_Test = TRUE;
		
	}
 
    pt = strtok (SS_Buff,",");
	
	Counter = 0;
	
	while (pt != NULL) {
		
		SS_Buff[0] = NULL;
		strcpy (SS_Data[Counter], pt);
		pt = strtok (NULL, ",");
		Counter = Counter + 1;
		
	}
	

	strcpy (CHANNEL_RESULTS[1][0].BT_Sensitive_Single_Slot_Packets_Valid_Stage, SS_Data[1]);
	strcpy (CHANNEL_RESULTS[1][0].BT_Sensitive_Single_Slot_Packets_Results_Valid, SS_Data[2]);
	CHANNEL_RESULTS[1][0].BT_Sensitive_Single_Slot_Packets_Overall_BER =  atof (SS_Data[3]);
	CHANNEL_RESULTS[1][0].BT_Sensitive_Single_Slot_Packets_Overall_FER =  atof (SS_Data[4]);
	strcpy (CHANNEL_RESULTS[1][0].BT_Sensitive_Single_Slot_Packets_State, SS_Data[5]);
	CHANNEL_RESULTS[1][0].BT_Sensitive_Single_Slot_Packets_FER_CRC =  atoi (SS_Data[6]);
	CHANNEL_RESULTS[1][0].BT_Sensitive_Single_Slot_Packets_FER_Length =  atoi (SS_Data[7]);
	CHANNEL_RESULTS[1][0].BT_Sensitive_Single_Slot_Packets_FER_Lost =  atoi (SS_Data[8]);
	CHANNEL_RESULTS[1][0].BT_Sensitive_Single_Slot_Packets_Packets_Received =  atoi (SS_Data[9]);
	CHANNEL_RESULTS[1][0].BT_Sensitive_Single_Slot_Packets_Bit_Error =  atoi (SS_Data[10]);
	CHANNEL_RESULTS[1][0].BT_Sensitive_Single_Slot_Packets_Frame_Error =  atoi (SS_Data[11]);
	CHANNEL_RESULTS[1][0].BT_Sensitive_Single_Slot_Packets_Packets_Sent =  atoi (SS_Data[12]);
	
	Comp_Error_Num = COMPARE_STRINGS (4, "PASS", CHANNEL_RESULTS[1][0].BT_Sensitive_Single_Slot_Packets_State);
	if (Comp_Error_Num)  MT8852B_Error = TRUE;	
*/


//*************************************************************
//*int Check_BTADDR_MT8852B (int panel)   	
//* 
//*************************************************************
int Check_BTADDR_MT8852B (int panel) 
{

	int status = 1;
	char msg[256] = {'\0'};
	char temp_buff[24] = {'\0'}; 
	int numToRead;
	int counter = 0;
	char returned_string[256] = {'\0'};
	int i = 0;
	
	sprintf(msg,"DECT Check BTADDR Test\n");
	WriteToTerminal(panel, msg); 

	//write script to the instrument
	for (i = 0; i <= 3; i++){  
	   	ibwrt (MT8852B_HANDLE, "SYSCFG? BTADDR", strlen("SYSCFG? BTADDR")); 
	   	if (ibsta & 0x8000) status =0; 
	
		Delay(1); 

	   	numToRead = 200;
		ibrd (MT8852B_HANDLE, returned_string, numToRead);
	   	if (ibsta & 0x8000) status = 0; 
	}
	
	sprintf (msg, "%s\n", returned_string); 
	WriteToTerminal(panel, returned_string);

	
	if (strstr (returned_string, "PASS")){
		status = 1;
	}
	else{
	   	status = 0;
	}
	
	sprintf(msg,"%s,",returned_string);
	strcat(data_string,msg);
	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
	}
	
	return status;

}



//*************************************************************
//*int Check_EUTADDR_MT8852B (int panel)   	
//* 
//*************************************************************
int Check_EUTADDR_MT8852B (int panel) 
{

	int status = 1;
	char msg[256] = {'\0'};
	char temp_buff[24] = {'\0'}; 
	int numToRead;
	int counter = 0;
	char returned_string[256] = {'\0'};
	
	
	sprintf(msg,"DECT Check EUTADDR MT8852B Test\n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg);
	WriteToTerminal(panel, msg); 


	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "SYSCFG? EUTADDR", strlen("SYSCFG? EUTADDR")); 
   	if (ibsta & 0x8000) status =0; 
	
	Delay(1); 

   	numToRead = 200;
	ibrd (MT8852B_HANDLE, returned_string, numToRead);
   	if (ibsta & 0x8000) status = 0; 
	
	
	sprintf (msg, "%s\n", returned_string); 
	//SetCtrlVal (panel, PANEL_Serial_Window_1, returned_string);
	WriteToTerminal(panel, returned_string); 
	
	
	sprintf(msg,"%s,",returned_string);
	strcat(data_string,msg);
	

	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		//SetCtrlVal (panel, PANEL_Serial_Window_1, "Test Pass\n\n");
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		//SetCtrlVal (panel, PANEL_Serial_Window_1, "Test Fail\n\n");	
	}
	
	return status;

}



   /*//write script to the instrument
   ibwrt (MT8852B_HANDLE, "SYSCFG? BTADDR", strlen("SYSCFG? BTADDR"));
   if (ibsta & 0x8000) MT8852B_Error = TRUE; 
   
   numToRead = 200;
   ibrd (MT8852B_HANDLE, BTADDR_Buff, numToRead);
   if (ibsta & 0x8000) MT8852B_Error = TRUE;
   
   ibwrt (MT8852B_HANDLE, "SYSCFG? EUTADDR", strlen("SYSCFG? EUTADDR"));
   if (ibsta & 0x8000) MT8852B_Error = TRUE;   
   
   numToRead = 200;
   ibrd (MT8852B_HANDLE, EUTADDR_Buff, numToRead);
   if (ibsta & 0x8000) MT8852B_Error = TRUE;
     */
   
   
   
//*************************************************************
//*int Check_BTADDR_MT8852B_2 (int panel)   	
//* 
//*************************************************************
int Check_BTADDR_MT8852B_2 (int panel) 
{

	int status = 1;
	char msg[256] = {'\0'};
	char temp_buff[24] = {'\0'}; 
	int numToRead;
	int counter = 0;
	char returned_string[256] = {'\0'};
	
	
	sprintf(msg,"DECT Check BTADDR and EUTADDR MT8852B Test\n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg);
	WriteToTerminal(panel, msg); 

	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "SYSCFG? BTADDR", strlen("SYSCFG? BTADDR")); 
   	if (ibsta & 0x8000) status =0; 
	
	Delay(1); 

   	numToRead = 200;
	ibrd (MT8852B_HANDLE, returned_string, numToRead);
   	if (ibsta & 0x8000) status = 0; 
	
	
	sprintf (msg, "%s\n", returned_string); 
	//SetCtrlVal (panel, PANEL_Serial_Window_1, returned_string);
	WriteToTerminal(panel, returned_string);

			
	
	//write script to the instrument
   	ibwrt (MT8852B_HANDLE, "SYSCFG? EUTADDR", strlen("SYSCFG? EUTADDR")); 
   	if (ibsta & 0x8000) status =0; 
	
	Delay(1); 

   	numToRead = 200;
	ibrd (MT8852B_HANDLE, returned_string, numToRead);
   	if (ibsta & 0x8000) status = 0; 
	
	
	sprintf (msg, "%s\n", returned_string); 
	//SetCtrlVal (panel, PANEL_Serial_Window_1, returned_string);
	WriteToTerminal(panel, returned_string); 
	
	
	sprintf(msg,"%s,",returned_string);
	strcat(data_string,msg);
	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		//SetCtrlVal (panel, PANEL_Serial_Window_1, "Test Pass\n\n");
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		//SetCtrlVal (panel, PANEL_Serial_Window_1, "Test Fail\n\n");	
	}
	
	return status;

}
 
   

//*************************************************************
//*int Close_MT8852B(int panel)  	
//* 
//*************************************************************
int Close_MT8852B(int panel)
{
	
	int status = 1; 
	char msg[256] = {'\0'}; 
	
	sprintf(msg,"Close MT8852B Test\n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg); 
	WriteToTerminal(panel, msg); 

   //bring the device to power on state before closing it   
   //to bring MT8852B to initial state after power on, you send the following   
   //send interface clear to the instrument
   ibwrt (MT8852B_HANDLE, "IFC", strlen("IFC"));
   if (ibsta & 0x8000) status = 0;
   
   //send device clear to the instrunet
   ibwrt (MT8852B_HANDLE, "DCL", strlen("DCL")); 
   if (ibsta & 0x8000) status = 0;
   
    //clear the device
   ibclr (MT8852B_HANDLE);
   if (ibsta & 0x8000) status = 0;  
   
   //clear status of the instrument
   ibwrt (MT8852B_HANDLE, "*CLS", strlen("*CLS"));
   if (ibsta & 0x8000) status = 0;
   
   //reset the instrunet
   ibwrt (MT8852B_HANDLE, "*RST", strlen("*RST")); 
   if (ibsta & 0x8000) status = 0;

   
   if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);	
	}
   
	
	return status;
}




//*************************************************************
//* Init_Counter(int panel); 	
//* 
//*************************************************************
int Init_Counter (int panel) {		
	
	char Temp_Buff[1024]; 
	int status = 1;
	
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
int Frequency_Measurement(int panel) {		
	
	char Temp_Buff[1024]; 
	int status = 0;
	double measured_frequency = 0.0;
	int numToRead;
	char msg[128]; 
	
	sprintf(msg,"CCA Frequency_TP122 Test\n");						 
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg);
	WriteToTerminal(panel, msg); 
	
	memset( Temp_Buff, '\0', sizeof(Temp_Buff));  
	
	status = TRIGGER_HP53131A_TO_MEASURE (); 
	
    //read the instrument id
    numToRead = 200;
    ibrd (UCT_HANDLE, Temp_Buff, numToRead);
    if (ibsta & 0x8000) status = 1;
	

    measured_frequency =  atof (Temp_Buff);
   
   	sprintf(msg,"Frequency_TP122 = %fHz\n",measured_frequency); 				
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg);
	WriteToTerminal(panel, msg);
	
	
	/*if ((measured_frequency > 32400)||(measured_frequency < 33000)){
		SetCtrlVal (panel, PANEL_Serial_Window_1, "Frequency_TP122 Test Pass\n\n");		 
		SetCtrlVal (panel, PANEL_Status_Window, "Frequency_TP122 Test Passed\n"); 			
		status = 1;
	}
	else{
		SetCtrlVal (panel, PANEL_Serial_Window_1, "Frequency_TP122 Test Fail\n\n");		 
		SetCtrlVal (panel, PANEL_Status_Window, "Frequency_TP122 Test Failed\n"); 		
		status = 0;
	}*/
	
	if (status == 1)
		return 1;
	else
		return 0;
   
}




//*************************************************************
//* 	
//* 
//*************************************************************
int TRIGGER_HP53131A_TO_MEASURE(void)
{
 
	int status = 1;
	char Temp_Buff[1024];

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
	

    return status;
}




//*************************************************************
//* 	
//* 
//*************************************************************
int Power_ON_DECT(void)
{
 
	int status = 0;

	//EIO3 Low
	//Toggle EIO4 Low then High
	
	
	//EIO3_On();
	
	//Delay(2);
	
	EIO4_On();
	
	Delay(2);
	
	EIO4_Off();
	
	Delay(5);
    
	return status;
}


//*************************************************************
//* 	
//* 
//*************************************************************
int Switch_DECT_USB_COM_ON(void)
{
 
	int status = 0;

	//EIO3 Low
	//Toggle EIO4 Low then High
	
	
	EIO0_On();
	//EIO1_On();
	//EIO2_On();
	
	Delay(5);
    
	return status;
}



//*************************************************************
//* 	
//* 
//*************************************************************
int Switch_DECT_USB_COM_OFF(void)
{
 
	int status = 0;

	//EIO3 Low
	//Toggle EIO4 Low then High
	
	
	EIO0_Off();
	//EIO1_Off();
	//EIO2_Off();
	
	Delay(5);
    
	return status;
}




//*************************************************************
//* 	
//* 
//*************************************************************
int Power_OFF_DECT(void)
{
 
	int status = 0;

	//EIO3 high

	//EIO3_Off();
	
	//EIO4_Off();
	
    return status;
}




//*************************************************************
//* 	
//* 
//*************************************************************
int Open_COM_Port_Relays(void)
{
 
	int status = 0;
	//EIO6 and EIO7

	EIO6_On();
	EIO7_On();
			
	Delay(1);
			
    return status;
}



//*************************************************************
//* 	
//* 
//*************************************************************
int Close_COM_Port_Relays(void)
{
 
	int status = 0;
	//EIO6 and EIO7

	EIO6_Off();
	EIO7_Off();
			
	Delay(1);
			
    return status;
}





//*************************************************************
//* 	
//* 
//*************************************************************
void print_hex_memory(int panel, void *mem, int size) 
{
	char temp_char[12];  
	int i;
  	unsigned char *p = (unsigned char *)mem;
  
	for (i=0;i<size;i++) {
		sprintf(temp_char,"0x%02X ",p[i]);
		//SetCtrlVal (panel, PANEL_Serial_Window_1, temp_char); 
		WriteToTerminal(panel, temp_char);

    	/*if ((i%16==0) && i){
			sprintf(temp_char,"\n");
			SetCtrlVal (panel, PANEL_Serial_Window_1, temp_char); 
    
		}*/
  	}
  
	sprintf(temp_char,"\n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, temp_char); 
	WriteToTerminal(panel, temp_char);

}


//*************************************************************
//* 
//*
//*************************************************************
int Serial_Port_Scan (int panel){

	int NUMBER_OF_SERIAL_PORTS = 0;
	int y = 0;
	int MAX_NUMBER_OF_SERIAL_PORTS = 256;
	int x = 0;
	int SERIAL_PORTS [256] = {'\0'};
	int NUMBER_OF_SERIAL_FAILURES [256] = {'\0'}; 
	char Temp_Buff[1024] = {'\0'};
	int Error_Num;
	int Com_Error;
	int SERIAL_COM_PORT; 
	
    for (x=0; x < MAX_NUMBER_OF_SERIAL_PORTS; x++)
		{
        NUMBER_OF_SERIAL_FAILURES[x] = 0;
		SERIAL_PORTS [x] = 0; 
	
		/* DERIVE NAME OF THIS COM PORT */
        sprintf (Temp_Buff,"com%d", x);
        
        /* OPEN COM PORT */
        Error_Num = OpenCom (x, Temp_Buff);
        if (Error_Num) 
			{
			Com_Error = 1;
			Error_Num = CloseCom (x);
			NUMBER_OF_SERIAL_FAILURES[x] = 1;
			SERIAL_PORTS [x] = 0;
			}
		else
			{
			SERIAL_PORTS [y] = x;
			NUMBER_OF_SERIAL_FAILURES[x] = 0;
			NUMBER_OF_SERIAL_PORTS++;
			y++;
			}
		}
		
		
	if (NUMBER_OF_SERIAL_PORTS == 1){ 
		SERIAL_COM_PORT = SERIAL_PORTS [0];
	}
	else{
		SERIAL_COM_PORT = SERIAL_PORTS [NUMBER_OF_SERIAL_PORTS - 1]; 
	}
	
	return SERIAL_COM_PORT;


}


//DECT USB Tests
//*************************************************************
//* 
//*
//*************************************************************
int DECT_Connect_USB_Cable(int panel)
{

	int status = 0;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};    

	
	sprintf(msg,"Switching to USB\n");
	WriteToTerminal(panel, msg); 
	
	CloseCom(COM_port);
	Close_COM_Port_Relays ();
	
	if((Test_Type == 4)||(Test_Type == 5)){
	//if((Test_Type == 1)||(Test_Type == 2)||(Test_Type == 4)||(Test_Type == 5)){ 	
		sprintf(msg,"连接USB，按下OK\n\nConnect USB Cable, then press OK.");
		Display_OK_MSG(msg);
	}
	
	//sprintf(msg,"Connect USB Cable.");
	//Display_OK_MSG(msg);
	
	
	Switch_DECT_USB_COM_ON(); 
	
	Delay(5);

	COM_port_2 = 4;
	
	Open_COM_PORT_2(panel);
	
	
	return status;
}


//*************************************************************
//* 
//*
//*************************************************************
int DECT_Disconnect_USB_Cable(int panel)
{

	int status = 0;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};    

	
	sprintf(msg,"DECT Disconnect USB Cable Test\n\n");
	WriteToTerminal(panel, msg); 
	
	CloseCom(COM_port_2); 
	
	
	if((Test_Type == 4)||(Test_Type == 5)){  
	//if((Test_Type == 1)||(Test_Type == 2)||(Test_Type == 4)||(Test_Type == 5)){  
		sprintf(msg,"拔下USB，按下OK\n\nDisconnect USB Cable, then Press OK.");
		Display_OK_MSG(msg);
	}
	
	if((Test_Type == 1)||(Test_Type == 2)){
		Switch_DECT_USB_COM_OFF(); 
	
	}	
	
	Delay(3);
	
	return status;
}



//*************************************************************
//* 
//*
//*************************************************************
int DECT_Enter_Test_Mode_USB(int panel)
{

	int status = 0;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'}; 
	int counter = 0;

	
	sprintf(msg,"DECT Enter Test Mode USB Test\n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg); 
	WriteToTerminal(panel, msg); 
	
	
	while((counter < 10) && (!(status == 1))){  
		status = DECT_Send_String_USB (panel,EnterTestMode,EnterTestMode_Response,returned_string,6,1,1);	  //needs to be 6 to support 7.2
		Delay(1);
		counter++;
	}
	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);	
	}
	
	return status;
}



//*************************************************************
//* 
//*
//*************************************************************
int DECT_Enter_Bluetooth_Test_Mode_USB(int panel)
{

	int status = 0;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'}; 
	int counter = 0;

	
	sprintf(msg,"DECT Enter Bluetooth Test Mode USB Test\n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg); 
	WriteToTerminal(panel, msg); 
	
	while((counter < 10) && (!(status == 1))){ 
		status = DECT_Send_String_USB (panel,BtEnterDutMode,BtEnterDutMode_Response,returned_string,0,1,1);
		Delay(10);  
		counter ++; 
	}
	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);	
	}
	
	return status;
}



//*************************************************************
//* 
//*
//*************************************************************
int DECT_Get_SW_Version_USB_Test (int panel)
{

	int status = 1;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	int SW_Revision_File_Handle = -1;
	char SW_Revision[256] = {'\0'}; 
	char temp_buff[512] = {'\0'};
	char temp_buff_2[512] = {'\0'};
	int i;
	int counter = 0;
	
	sprintf(msg,"DECT Get SW Version USB Test\n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg); 
	WriteToTerminal(panel, msg); 
	
	//Get expected version
	SW_Revision_File_Handle = OpenFile ("sw_version.cfg", VAL_READ_ONLY, VAL_OPEN_AS_IS, VAL_ASCII);
	status = ReadLine(SW_Revision_File_Handle,SW_Revision,24);
	CloseFile(SW_Revision_File_Handle);
	
	
	while((counter < 10) && (!(status == 1))){  
		status = DECT_Send_String_USB(panel,GetSwVersionString,GetSwVersionString_Response,returned_string,-1,1,2);
		//status = DECT_Send_String_USB(panel,GetSwVersionString,GetSwVersionString_Response,returned_string,14,1,2); 
		Delay(1);
		counter++;
	}

	if(((Product_Serial_Global[0] == 50 )&&(Product_Serial_Global[1] == 87)&&(Product_Serial_Global[2] == 68))||((Product_Serial_Global[0] == 50 )&&(Product_Serial_Global[1] == 87)&&(Product_Serial_Global[2] == 69))||((Product_Serial_Global[0] == 50 )&&(Product_Serial_Global[1] == 87)&&(Product_Serial_Global[2] == 70))){	                   	
		Fmt (temp_buff, "%s<%s[i5w69]\n", returned_string);
	}
	else{
		Fmt (temp_buff, "%s<%s[i5w53]\n", returned_string);
	}
	
	//SetCtrlVal (panel, PANEL_Serial_Window_1, temp_buff);
	WriteToTerminal(panel, temp_buff);
	
	
	for (i = 0; i <= 13; i++){
		if(temp_buff[i] != SW_Revision[i]){
			status = 0;
			sprintf (temp_buff_2, "Expected SW Revision: %s \n", SW_Revision);	
    		//SetCtrlVal (panel, PANEL_Serial_Window_1, temp_buff_2);
			WriteToTerminal(panel, temp_buff_2);
			break;
		}
	}
	
	sprintf(msg,"%s,",SW_Revision);
	strcat(data_string,msg);
	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		//SetCtrlVal (panel, PANEL_Serial_Window_1, "Test Pass\n\n");
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		//SetCtrlVal (panel, PANEL_Serial_Window_1, "Test Fail\n\n");	
	}
	
	return status;
	
}	



//*************************************************************
//* 
//*
//*************************************************************
int DECT_Bluetooth_Sync_Clock_USB_Test(int panel)
{

	int status = 0;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'}; 
	int counter = 0;

	
	sprintf(msg,"DECT Bluetooth Sync Clock USB Test\n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg);
	WriteToTerminal(panel, msg); 
	
	while((counter < 20) && (!(status == 1))){ 
		status = DECT_Send_String_USB (panel,BtSyncClock,BtSyncClock_Response,returned_string,0,1,1);
		Delay(1);
		counter ++;
	}
	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);	
	}
	
	return status;
}





//*************************************************************
//* 
//*
//*************************************************************
int DECT_Bluetooth_Get_Analog_Trim_USB_Test(int panel)
{

	int status = 0;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'}; 
	int counter = 0;
	char temp_buff[256] = {'\0'}; 
	
	memset( Bluetooth_Analog_Trim, '\0', sizeof(Bluetooth_Analog_Trim));
	
	sprintf(msg,"DECT Bluetooth Get Analog Trim USB Test\n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg);
	WriteToTerminal(panel, msg); 
	
	while((counter < 10) && (!(status == 1))){ 
		status = DECT_Send_String_USB (panel,BtGetAnaFtrim,BtGetAnaFtrim_Response,returned_string,5,1,1);
		Delay(1);
		counter ++; 
	}
	
	Bluetooth_Analog_Trim[0] = returned_string[6];
	
	sprintf (temp_buff, "0x%X", Bluetooth_Analog_Trim[0]);
	sprintf(msg,"%s,",temp_buff);
	strcat(data_string,msg);
	
	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		//SetCtrlVal (panel, PANEL_Serial_Window_1, "Test Pass\n\n");
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		//SetCtrlVal (panel, PANEL_Serial_Window_1, "Test Fail\n\n");	
	}
	
	return status;
}





//*************************************************************
//* 
//*
//*************************************************************
int DECT_Bluetooth_Write_Analog_Trim_USB_Test(int panel)
{

	int status = 0;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'}; 
	int counter = 0;
	unsigned char BT_Eeprom_Write[16] = {'\0'};
	int x;
	
	sprintf(msg,"DECT Bluetooth Write Analog Trim USB Test\n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg);
	WriteToTerminal(panel, msg); 
	
	
	for (x=0; x < 16; x++){
		BT_Eeprom_Write[x] = BtEepromWrite[x];
	}
	

	BT_Eeprom_Write[14] = Bluetooth_Analog_Trim[0];

	//xor bytes 15, 14 to write BT Trim
	BT_Eeprom_Write[15] =  BT_Eeprom_Write[15] ^ BT_Eeprom_Write[14];
	
	
	while((counter < 10) && (!(status == 1))){  
		status = DECT_Send_String_USB (panel,BT_Eeprom_Write,BtEepromWrite_Response,returned_string,10,1,1);  
		Delay(1);
		counter++;
	}
	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);	
	}
	
	return status;
}


//*************************************************************
//* 
//* This EEPROM read command is used to get out the value for Bluetooth clock adjustment from EEPROM.
//*************************************************************
int DECT_Bluetooth_Read_ClockAdjust_USB_Test(int panel)  
{

	int status = 0;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'}; 
	int counter = 0;
	char temp_buff[256] = {'\0'}; 

	
	sprintf(msg,"DECT Bluetooth Read ClockAdjust USB Test\n");
	WriteToTerminal(panel, msg); 
	
	while((counter < 10) && (!(status == 1))){ 
		status = DECT_Send_String_USB_V2 (panel,BtEepromRead,BtEepromRead_Response,returned_string,-1,1,1);
		Delay(1);
		counter ++; 
	}
	
	sprintf (temp_buff, "0x%X", returned_string[11]);
	sprintf(msg,"%s,",temp_buff);
	strcat(data_string,msg);
	
	
	if(status == 1 && (Bluetooth_Analog_Trim[0] == returned_string[11])){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		//SetCtrlVal (panel, PANEL_Serial_Window_1, "Test Pass\n\n");
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		//SetCtrlVal (panel, PANEL_Serial_Window_1, "Test Fail\n\n");	
	}
	
	return status;
}


//*************************************************************
//* 
//*
//*************************************************************
int DECT_EEPROM_LED_USB_Test(int panel)
{

	int status = 0;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	int counter = 0;
	
	sprintf(msg,"DECT EEPROM LED USB Test\n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg);
	WriteToTerminal(panel, msg); 
	
	
	while((counter < 10) && (!(status == 1))){  
		status = DECT_Send_String_USB (panel,WriteEepromLED,WriteEepromLED_Response,returned_string,0,1,1);  
		Delay(1);
		counter++;
	}
	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);	
	}
	
	return status;
}


//*************************************************************
//* 
//*
//*************************************************************
int DECT_EEPROM_LED_Test(int panel)
{

	int status = 0;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	int counter = 0;
	
	sprintf(msg,"DECT EEPROM LED Test\n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg);
	WriteToTerminal(panel, msg); 
	
	
	while((counter < 10) && (!(status == 1))){  
		status = DECT_Send_String (panel,WriteEepromLED,WriteEepromLED_Response,returned_string,0,1,1);  
		Delay(1);
		counter++;
	}
	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);	
	}
	
	return status;
}


//*************************************************************
//* 
//*
//*************************************************************
int DECT_EEPROM_SOS_USB_Test(int panel)
{

	int status = 0;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	unsigned char EepromSOS[13] = {'\0'}; 
	int x;
	int counter = 0;
	
	//MODEL_632D_V2 Only
	sprintf(msg,"DECT EEPROM SOS USB Test\n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg);
	WriteToTerminal(panel, msg); 
	
	for (x=0; x < 13; x++)
    {
        EepromSOS[x] = WriteEepromSOS[x];
    }
		
	if (Test_Type == 2){ //632d
		EepromSOS[11] = 0x01;
		EepromSOS[12] =  EepromSOS[12] ^ EepromSOS[11];  
	}

	
	while((counter < 10) && (!(status == 1))){  
		status = DECT_Send_String_USB (panel,EepromSOS,WriteEepromSOS_Response,returned_string,0,1,1);   
		Delay(1);
		counter++;
	}
	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);	
	}
	
	return status;
}



//*************************************************************
//* 
//*
//*************************************************************
int DECT_EEPROM_SOS_Test(int panel)
{

	int status = 0;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	unsigned char EepromSOS[13] = {'\0'}; 
	int x;
	int counter = 0;
	

	sprintf(msg,"DECT EEPROM SOS Test\n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg);
	WriteToTerminal(panel, msg); 
	
	for (x=0; x < 13; x++)
    {
        EepromSOS[x] = WriteEepromSOS[x];
    }
		
	if (Test_Type == 2){ //632d
		EepromSOS[11] = 0x01;
		EepromSOS[12] =  EepromSOS[12] ^ EepromSOS[11];  
	}
	
	while((counter < 10) && (!(status == 1))){  
		status = DECT_Send_String (panel,EepromSOS,WriteEepromSOS_Response,returned_string,0,1,1);   
		Delay(1);
		counter++;
	}
	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);	
	}
	
	return status;
}


//*************************************************************
//* 
//*
//*************************************************************
int DECT_EEPROM_HOTKEY_USB_Test(int panel)
{

	int status = 0;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	unsigned char EepromHOTKEY[13] = {'\0'}; 
	int x;  
	int counter = 0;

	// MODEL_622D_V2 MODEL_632D_V2 Only
	sprintf(msg,"DECT EEPROM Hotkey USB Test\n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg);
	WriteToTerminal(panel, msg); 
	
	for (x=0; x < 13; x++)
    {
        EepromHOTKEY[x] = WriteEepromHOTKEY[x];
    }
		
	
	if ((Test_Type == 1) || (Test_Type == 2)){
		EepromHOTKEY[11] = 0x01;
		EepromHOTKEY[12] =  EepromHOTKEY[12] ^ EepromHOTKEY[11];
	}

	
	while((counter < 10) && (!(status == 1))){  
		status = DECT_Send_String_USB (panel,EepromHOTKEY,WriteEepromHOTKEY_Response,returned_string,0,1,1);   
		Delay(1);
		counter++;
	}
	

	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);	
	}
	
	return status;
}


//*************************************************************
//* 
//*
//*************************************************************
int DECT_EEPROM_HOTKEY_Test(int panel)
{

	int status = 0;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	unsigned char EepromHOTKEY[13] = {'\0'}; 
	int x;  
	int counter = 0;

	// MODEL_622D_V2 MODEL_632D_V2 Only
	sprintf(msg,"DECT EEPROM Hotkey Test\n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg);
	WriteToTerminal(panel, msg); 
	
	for (x=0; x < 13; x++)
    {
        EepromHOTKEY[x] = WriteEepromHOTKEY[x];
    }
		
	
	if ((Test_Type == 1) || (Test_Type == 2)){
		EepromHOTKEY[11] = 0x01;
		EepromHOTKEY[12] =  EepromHOTKEY[12] ^ EepromHOTKEY[11];
	}

	
	while((counter < 10) && (!(status == 1))){  
		status = DECT_Send_String (panel,EepromHOTKEY,WriteEepromHOTKEY_Response,returned_string,0,1,1);   
		Delay(1);
		counter++;
	}
	

	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);	
	}
	
	return status;
}


//*************************************************************
//* 
//*
//*************************************************************
int DECT_EEPROM_VIBRATOR_USB_Test(int panel)
{

	int status = 0;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	int counter = 0;

	// MODEL_622D_V2 MODEL_632D_V2 Only
	sprintf(msg,"DECT EEPROM Vibrator USB Test\n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg);
	WriteToTerminal(panel, msg); 
	
	while((counter < 10) && (!(status == 1))){  
		status = DECT_Send_String_USB (panel,WriteEepromVIBRATOR,WriteEepromVIBRATOR_Response,returned_string,0,1,1);   
		Delay(1);
		counter++;
	}
	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);	
	}
	
	return status;
}


//*************************************************************
//* 
//*
//*************************************************************
int DECT_EEPROM_LOUDNESS_USB_Test(int panel)
{

	int status = 0;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	int counter = 0;

	// MODEL_632D_V2 Only
	sprintf(msg,"DECT EEPROM Loudness USB Test\n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg);
	WriteToTerminal(panel, msg); 
	
	while((counter < 10) && (!(status == 1))){  
		status = DECT_Send_String_USB (panel,WriteEepromEarpieceLoudness,WriteEepromEarpieceLoudness_Response,returned_string,0,1,1);   
		Delay(1);
		counter++;
	}
	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);	
	}
	
	return status;
}



//*************************************************************
//* 
//*
//*************************************************************
int DECT_EEPROM_WRITE_DATE_USB_Test(int panel)
{

	int status = 0;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	unsigned char EepromFuncDate[15] = {'\0'};
	unsigned int year, month, day, hour, min, sec, weekDay; 
	int x;
	int counter = 0;
	
	CVIAbsoluteTime absTime; 

	sprintf(msg,"DECT EEPROM Write Date USB Test\n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg);
	WriteToTerminal(panel, msg); 
	
	for (x=0; x < 15; x++) 
    {
        EepromFuncDate[x] = WriteEepromFuncDate[x];
    }		 


	GetCurrentCVIAbsoluteTime(&absTime);

	CVIAbsoluteTimeToLocalCalendar(absTime, &year, &month, &day, &hour, &min, &sec, 0, &weekDay);
	

	EepromFuncDate[11] = (char)(year - 2000);
	EepromFuncDate[12] = (char) month;
	EepromFuncDate[13] = (char) day;

	EepromFuncDate[14] =  EepromFuncDate[14] ^ EepromFuncDate[13] ^ EepromFuncDate[12] ^ EepromFuncDate[11];
	
	
	while((counter < 10) && (!(status == 1))){  
		status = DECT_Send_String_USB (panel,EepromFuncDate,WriteEepromFuncDate_Response,returned_string,0,1,1);   
		Delay(1);
		counter++;
	}

	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);	
	}
	
	return status;
}



//*************************************************************
//* 
//*
//*************************************************************
int DECT_EEPROM_WRITE_DATE_Test(int panel)
{

	int status = 0;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	unsigned char EepromFuncDate[15] = {'\0'};
	unsigned int year, month, day, hour, min, sec, weekDay; 
	int x;
	int counter = 0;
	
	CVIAbsoluteTime absTime; 

	sprintf(msg,"DECT EEPROM Write Date Test\n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg);
	WriteToTerminal(panel, msg); 
	
	for (x=0; x < 15; x++) 
    {
        EepromFuncDate[x] = WriteEepromFuncDate[x];
    }		 


	GetCurrentCVIAbsoluteTime(&absTime);

	CVIAbsoluteTimeToLocalCalendar(absTime, &year, &month, &day, &hour, &min, &sec, 0, &weekDay);
	

	EepromFuncDate[11] = (char)(year - 2000);
	EepromFuncDate[12] = (char) month;
	EepromFuncDate[13] = (char) day;

	EepromFuncDate[14] =  EepromFuncDate[14] ^ EepromFuncDate[13] ^ EepromFuncDate[12] ^ EepromFuncDate[11];
	
	
	while((counter < 10) && (!(status == 1))){  
		status = DECT_Send_String (panel,EepromFuncDate,WriteEepromFuncDate_Response,returned_string,0,1,1);   
		Delay(1);
		counter++;
	}

	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);	
	}
	
	return status;
}




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//unsigned char WriteEepromFinalDate[15] = {0x0E, 0x10, 0x0C, 0x82, 0x00, 0x42, 0xF0, 0xEB, 0x14, 0x00, 0x03, 0x00, 0x00, 0x00, 0xCC};
//char		  WriteEepromFinalDate_Response[7] = {0x07, 0xEB, 0x00, 0x43, 0xF0, 0x11, 0x00};

//*************************************************************
//* 
//*
//*************************************************************
int DECT_EEPROM_WRITE_FINAL_DATE_USB_Test(int panel)
{

	int status = 0;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	unsigned char EepromFinalDate[15] = {'\0'};
	unsigned int year, month, day, hour, min, sec, weekDay; 
	int x;
	int counter = 0;
	
	CVIAbsoluteTime absTime; 

	sprintf(msg,"DECT EEPROM Write Final Date USB Test\n");
	WriteToTerminal(panel, msg); 
	
	for (x=0; x < 15; x++) 
    {
        EepromFinalDate[x] = WriteEepromFinalDate[x];
    }		 


	GetCurrentCVIAbsoluteTime(&absTime);

	CVIAbsoluteTimeToLocalCalendar(absTime, &year, &month, &day, &hour, &min, &sec, 0, &weekDay);
	

	EepromFinalDate[11] = (char)(year - 2000);
	EepromFinalDate[12] = (char) month;
	EepromFinalDate[13] = (char) day;

	EepromFinalDate[14] =  EepromFinalDate[14] ^ EepromFinalDate[13] ^ EepromFinalDate[12] ^ EepromFinalDate[11];
	
	
	while((counter < 10) && (!(status == 1))){  
		status = DECT_Send_String_USB (panel,EepromFinalDate,WriteEepromFinalDate_Response,returned_string,0,1,1);   
		Delay(1);
		counter++;
	}

	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);	
	}
	
	return status;
}



//*************************************************************
//* 
//*
//*************************************************************
int DECT_EEPROM_WRITE_FINAL_DATE_Test(int panel)
{

	int status = 0;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	unsigned char EepromFinalDate[15] = {'\0'};
	unsigned int year, month, day, hour, min, sec, weekDay; 
	int x;
	int counter = 0;
	
	CVIAbsoluteTime absTime; 

	sprintf(msg,"DECT EEPROM Write Final Date Test\n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg);
	WriteToTerminal(panel, msg); 
	
	for (x=0; x < 15; x++) 
    {
        EepromFinalDate[x] = WriteEepromFinalDate[x];
    }		 


	GetCurrentCVIAbsoluteTime(&absTime);

	CVIAbsoluteTimeToLocalCalendar(absTime, &year, &month, &day, &hour, &min, &sec, 0, &weekDay);
	

	EepromFinalDate[11] = (char)(year - 2000);
	EepromFinalDate[12] = (char) month;
	EepromFinalDate[13] = (char) day;

	EepromFinalDate[14] =  EepromFinalDate[14] ^ EepromFinalDate[13] ^ EepromFinalDate[12] ^ EepromFinalDate[11];
	
	
	while((counter < 10) && (!(status == 1))){  
		status = DECT_Send_String (panel,EepromFinalDate,WriteEepromFinalDate_Response,returned_string,0,1,1);    
		Delay(1);
		counter++;
	}

	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);	
	}
	
	return status;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//*************************************************************
//* 
//*
//*************************************************************
int DECT_EEPROM_WRITE_PASS_USB_Test(int panel)
{

	int status = 0;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	int counter = 0;

	sprintf(msg,"DECT EEPROM Write Pass USB Test\n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg); 
	WriteToTerminal(panel, msg); 

	while((counter < 10) && (!(status == 1))){  
		status = DECT_Send_String_USB (panel,SetEepromTestflowDataToFTPassed,SetEepromTestflowDataToFTPassed_Response,returned_string,0,1,1);    
		Delay(1);
		counter++;
	}
	

	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);	
	}
	
	return status;
}



//*************************************************************
//* 
//*
//*************************************************************
int DECT_EEPROM_WRITE_PASS_USB_Final_Test(int panel)
{

	int status = 0;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	int counter = 0;

	sprintf(msg,"DECT EEPROM Write Pass USB Final Test\n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg); 
	WriteToTerminal(panel, msg); 

	while((counter < 10) && (!(status == 1))){  
		status = DECT_Send_String_USB (panel,SetEepromTestflowDataToFinalPassed,SetEepromTestflowDataToFinalPassed_Response,returned_string,0,1,1);    
		Delay(1);
		counter++;
	}
	

	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);	
	}
	
	return status;
}



//*************************************************************
//* 
//*
//*************************************************************
int DECT_EEPROM_WRITE_FAIL_USB_Test(int panel)
{

	int status = 0;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	int counter = 0;


	sprintf(msg,"DECT EEPROM Write Fail USB Test\n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg); 
	WriteToTerminal(panel, msg); 
	
	if((Test_Type == 0)||(Test_Type == 1)||(Test_Type == 2)){
		while((counter < 10) && (!(status == 1))){  
			status = DECT_Send_String_USB (panel,SetEepromTestflowDataToFTFailed,SetEepromTestflowDataToFTFailed_Response,returned_string,0,1,1);     
			Delay(1);
			counter++;
		}
	}
	else if ((Test_Type == 3)||(Test_Type == 4)||(Test_Type == 5)){
		while((counter < 10) && (!(status == 1))){  
			status = DECT_Send_String_USB (panel,SetEepromTestflowDataToFinalFailed,SetEepromTestflowDataToFinalFailed_Response,returned_string,0,1,1);     
			Delay(1);
			counter++;
		}
	}
	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		//SetCtrlVal (panel, PANEL_Serial_Window_1, "Test Pass\n\n");
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		//SetCtrlVal (panel, PANEL_Serial_Window_1, "Test Fail\n\n");	
	}
	
	return status;
}



//*************************************************************
//* 
//*
//*************************************************************
int DECT_Make_Backup_USB_Test(int panel)
{

	int status = 0;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	int counter = 0;

	sprintf(msg,"DECT Make Backup USB Test\n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg); 
	WriteToTerminal(panel, msg); 
	
	while((counter < 10) && (!(status == 1))){  
		status = DECT_Send_String_USB (panel,MakeBackup,MakeBackup_Response,returned_string,5,1,1);     
		Delay(1);
		counter++;
	}
	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);	
	}
	
	return status;
}


//*************************************************************
//* 
//*
//*************************************************************
int DECT_Power_Off_USB_Test(int panel)
{

	int status = 0;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	int counter = 0;

	
	sprintf(msg,"DECT Power Off USB Test\n");
	WriteToTerminal(panel, msg); 
	
	while((counter < 10) && (!(status == 1))){  
		status = DECT_Send_String_USB (panel,PowerOff,PowerOff_Response,returned_string,0,1,1);      
		Delay(1);
		counter++;
	}
	
	Delay(3);
	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);	
	}
	
	return status;
}

//END DECT USB TESTS



//*************************************************************
//* 
//*
//*************************************************************
int DECT_Enter_Test_Mode(int panel)
{

	int status = 0;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'}; 
	int counter = 0;

	
	sprintf(msg,"DECT Enter Test Mode Test\n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg); 
	WriteToTerminal(panel, msg); 
	
	while((counter < 10) && (!(status == 1))){
		status = DECT_Send_String (panel,EnterTestMode,EnterTestMode_Response,returned_string,6,1,1);	  //6 required for new development version from Stephan
		counter ++;
	}
	

	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);

	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg);
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);
	
	}
	
	return status;
}


//*************************************************************
//* 
//*
//*************************************************************
int DECT_Write_HW_Release_Test (int panel)
{

	int status = 0;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	int counter = 0;
	
	sprintf(msg,"DECT Write HW Release Test\n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg); 
	WriteToTerminal(panel, msg); 
	
	while((counter < 10) && (!(status == 1))){  
		status = DECT_Send_String (panel,WriteEepromHWRelease,WriteEepromHWRelease_Response,returned_string,0,1,1);      
		Delay(1);
		counter++;
	}

	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);
	}
	
	return status;
}




//*************************************************************
//* 
//*
//*************************************************************
int DECT_EEPROM_WRITE_PASS_Test(int panel)
{

	int status = 0;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	int counter = 0;

	sprintf(msg,"DECT EEPROM Write Pass Test\n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg); 
	WriteToTerminal(panel, msg); 

	while((counter < 10) && (!(status == 1))){  
		status = DECT_Send_String (panel,SetEepromTestflowDataToFTPassed,SetEepromTestflowDataToFTPassed_Response,returned_string,0,1,1);    
		Delay(1);
		counter++;
	}
	

	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);	
	}
	
	return status;
}




//*************************************************************
//* 
//*
//*************************************************************
int DECT_EEPROM_WRITE_PASS_Final_Test(int panel)
{

	int status = 0;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	int counter = 0;

	sprintf(msg,"DECT EEPROM Write Pass Final Test\n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg); 
	WriteToTerminal(panel, msg); 

	while((counter < 10) && (!(status == 1))){  
		status = DECT_Send_String (panel,SetEepromTestflowDataToFinalPassed,SetEepromTestflowDataToFinalPassed_Response,returned_string,0,1,1);    
		Delay(1);
		counter++;
	}
	

	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);	
	}
	
	return status;
}


//*************************************************************
//* 
//*
//*************************************************************
int DECT_EEPROM_WRITE_FAIL_Test(int panel)
{

	int status = 0;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	int counter = 0;


	sprintf(msg,"DECT EEPROM Write Fail Test\n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg); 
	WriteToTerminal(panel, msg); 
	
	if((Test_Type == 0)||(Test_Type == 1)||(Test_Type == 2)){
		while((counter < 10) && (!(status == 1))){  
			status = DECT_Send_String(panel,SetEepromTestflowDataToFTFailed,SetEepromTestflowDataToFTFailed_Response,returned_string,0,1,1);     
			Delay(1);
			counter++;
		}
	}
	else if ((Test_Type == 3)||(Test_Type == 4)||(Test_Type == 5)){
		while((counter < 10) && (!(status == 1))){  
			status = DECT_Send_String(panel,SetEepromTestflowDataToFinalFailed,SetEepromTestflowDataToFinalFailed_Response,returned_string,0,1,1);     
			Delay(1);
			counter++;
		}
	}
	
	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		//SetCtrlVal (panel, PANEL_Serial_Window_1, "Test Pass\n\n");
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		//SetCtrlVal (panel, PANEL_Serial_Window_1, "Test Fail\n\n");	
	}
	
	return status;
}



//*************************************************************
//* 
//*
//*************************************************************
int DECT_Get_SW_Version_Test_612 (int panel)
{

	int status = 0;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	int SW_Revision_File_Handle = -1;
	char SW_Revision[256] = {'\0'}; 
	char temp_buff[512] = {'\0'};
	char temp_buff_2[512] = {'\0'};
	int i;
	int counter = 0;
	
	sprintf(msg,"DECT Get SW Version Test\n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg); 
	WriteToTerminal(panel, msg); 
	
	//Get expected version
	SW_Revision_File_Handle = OpenFile ("sw_version_612.cfg", VAL_READ_ONLY, VAL_OPEN_AS_IS, VAL_ASCII);
	status = ReadLine(SW_Revision_File_Handle,SW_Revision,24);
	CloseFile(SW_Revision_File_Handle);

	
	while((counter < 10) && (!(status == 1))){  
		status = DECT_Send_String (panel,GetSwVersionString,GetSwVersionString_Response,returned_string,-1,1,1);       
		Delay(1);
		counter++;
	}
	
	if(((Product_Serial_Global[0] == 50 )&&(Product_Serial_Global[1] == 87)&&(Product_Serial_Global[2] == 68))||((Product_Serial_Global[0] == 50 )&&(Product_Serial_Global[1] == 87)&&(Product_Serial_Global[2] == 69))||((Product_Serial_Global[0] == 50 )&&(Product_Serial_Global[1] == 87)&&(Product_Serial_Global[2] == 70))){	                   	
		Fmt (temp_buff, "%s<%s[i5w69]\n", returned_string);
	}
	else{
		Fmt (temp_buff, "%s<%s[i5w53]\n", returned_string);
	}
	
	//SetCtrlVal (panel, PANEL_Serial_Window_1, temp_buff);
	WriteToTerminal(panel, temp_buff);
	
	
	for (i = 0; i < strlen(SW_Revision); i++){
		if(temp_buff[i] != SW_Revision[i]){
			status = 0;
			sprintf (temp_buff_2, "Expected SW Revision: %s \n", SW_Revision);	
    		//SetCtrlVal (panel, PANEL_Serial_Window_1, temp_buff_2);
			WriteToTerminal(panel, temp_buff_2);
			break;
		}
	}
	
	
	sprintf(msg,"%s,",SW_Revision);
	strcat(data_string,msg);
	
	//status = 1; //debug
	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		//SetCtrlVal (panel, PANEL_Serial_Window_1, "Test Pass\n\n");
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		//SetCtrlVal (panel, PANEL_Serial_Window_1, "Test Fail\n\n");	
	}
	
	return status;
	
}	



//*************************************************************
//* 
//*
//*************************************************************
int DECT_Get_SW_Version_Test_622 (int panel)
{

	int status = 0;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	int SW_Revision_File_Handle = -1;
	char SW_Revision[256] = {'\0'}; 
	char temp_buff[512] = {'\0'};
	char temp_buff_2[512] = {'\0'};
	int i;
	int counter = 0;
	
	sprintf(msg,"DECT Get SW Version Test\n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg); 
	WriteToTerminal(panel, msg); 
	
	//Get expected version
	SW_Revision_File_Handle = OpenFile ("sw_version_622.cfg", VAL_READ_ONLY, VAL_OPEN_AS_IS, VAL_ASCII);
	status = ReadLine(SW_Revision_File_Handle,SW_Revision,24);
	CloseFile(SW_Revision_File_Handle);

	
	while((counter < 10) && (!(status == 1))){  
		status = DECT_Send_String (panel,GetSwVersionString,GetSwVersionString_Response,returned_string,-1,1,1);       
		Delay(1);
		counter++;
	}
	
	if(((Product_Serial_Global[0] == 50 )&&(Product_Serial_Global[1] == 87)&&(Product_Serial_Global[2] == 68))||((Product_Serial_Global[0] == 50 )&&(Product_Serial_Global[1] == 87)&&(Product_Serial_Global[2] == 69))||((Product_Serial_Global[0] == 50 )&&(Product_Serial_Global[1] == 87)&&(Product_Serial_Global[2] == 70))){	                   	
		Fmt (temp_buff, "%s<%s[i5w69]\n", returned_string);
	}
	else{
		Fmt (temp_buff, "%s<%s[i5w53]\n", returned_string);
	}
	
	//SetCtrlVal (panel, PANEL_Serial_Window_1, temp_buff);
	WriteToTerminal(panel, temp_buff);
	
	
	for (i = 0; i < strlen(SW_Revision); i++){
		if(temp_buff[i] != SW_Revision[i]){
			status = 0;
			sprintf (temp_buff_2, "Expected SW Revision: %s \n", SW_Revision);	
    		//SetCtrlVal (panel, PANEL_Serial_Window_1, temp_buff_2);
			WriteToTerminal(panel, temp_buff_2);
			break;
		}
	}
	
	
	sprintf(msg,"%s,",SW_Revision);
	strcat(data_string,msg);
	
	//status = 1; //debug
	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		//SetCtrlVal (panel, PANEL_Serial_Window_1, "Test Pass\n\n");
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		//SetCtrlVal (panel, PANEL_Serial_Window_1, "Test Fail\n\n");	
	}
	
	return status;
	
}	

//*************************************************************
//* 
//*
//*************************************************************
int DECT_Get_SW_Version_Test_632 (int panel)
{

	int status = 0;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	int SW_Revision_File_Handle = -1;
	char SW_Revision[256] = {'\0'}; 
	char temp_buff[512] = {'\0'};
	char temp_buff_2[512] = {'\0'};
	int i;
	int counter = 0;
	
	sprintf(msg,"DECT Get SW Version Test\n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg); 
	WriteToTerminal(panel, msg); 
	
	//Get expected version
	SW_Revision_File_Handle = OpenFile ("sw_version_632.cfg", VAL_READ_ONLY, VAL_OPEN_AS_IS, VAL_ASCII);
	status = ReadLine(SW_Revision_File_Handle,SW_Revision,24);
	CloseFile(SW_Revision_File_Handle);

	
	while((counter < 10) && (!(status == 1))){  
		status = DECT_Send_String (panel,GetSwVersionString,GetSwVersionString_Response,returned_string,-1,1,1);       
		Delay(1);
		counter++;
	}
	
	if(((Product_Serial_Global[0] == 50 )&&(Product_Serial_Global[1] == 87)&&(Product_Serial_Global[2] == 68))||((Product_Serial_Global[0] == 50 )&&(Product_Serial_Global[1] == 87)&&(Product_Serial_Global[2] == 69))||((Product_Serial_Global[0] == 50 )&&(Product_Serial_Global[1] == 87)&&(Product_Serial_Global[2] == 70))){	                   	
		Fmt (temp_buff, "%s<%s[i5w69]\n", returned_string);
	}
	else{
		Fmt (temp_buff, "%s<%s[i5w53]\n", returned_string);
	}
	
	//SetCtrlVal (panel, PANEL_Serial_Window_1, temp_buff);
	WriteToTerminal(panel, temp_buff);
	
	
	for (i = 0; i < strlen(SW_Revision); i++){
		if(temp_buff[i] != SW_Revision[i]){
			status = 0;
			sprintf (temp_buff_2, "Expected SW Revision: %s \n", SW_Revision);	
    		//SetCtrlVal (panel, PANEL_Serial_Window_1, temp_buff_2);
			WriteToTerminal(panel, temp_buff_2);
			break;
		}
	}
	
	
	sprintf(msg,"%s,",SW_Revision);
	strcat(data_string,msg);
	
	//status = 1; //debug
	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		//SetCtrlVal (panel, PANEL_Serial_Window_1, "Test Pass\n\n");
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		//SetCtrlVal (panel, PANEL_Serial_Window_1, "Test Fail\n\n");	
	}
	
	return status;
	
}	



//*************************************************************
//* 
//*
//*************************************************************
int DECT_Get_HW_Version_Test (int panel, int iType)
{

	int status = 0;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};

	char HW_Revision[256] = {'\0'}; 
	char temp_buff[512] = {'\0'};
	char temp_buff_2[512] = {'\0'};
	int i;
	int counter = 0;
	
	sprintf(msg,"DECT Get HW Version Test\n");
	WriteToTerminal(panel, msg); 
	
	
	if(iType == 0)		  //612
		strcpy(HW_Revision,"hw:4.0.v2.6.12 prd:3G");
	else if(iType == 1)   //622
		strcpy(HW_Revision,"hw:4.0.v2.6.22 prd:3G"); 
	else 				  //632
		strcpy(HW_Revision,"hw:4.0.v2.6.32 prd:3G"); 
	
	
	while((counter < 10) && (!(status == 1))){  
		status = DECT_Send_String (panel,GetSwVersionString,GetSwVersionString_Response,returned_string,-1,1,1);       
		Delay(1);
		counter++;
	}
	
	if(((Product_Serial_Global[0] == 50 )&&(Product_Serial_Global[1] == 87)&&(Product_Serial_Global[2] == 68))||((Product_Serial_Global[0] == 50 )&&(Product_Serial_Global[1] == 87)&&(Product_Serial_Global[2] == 69))||((Product_Serial_Global[0] == 50 )&&(Product_Serial_Global[1] == 87)&&(Product_Serial_Global[2] == 70))){	                   	
		Fmt (temp_buff, "%s<%s[i5w69]\n", returned_string);
	}
	else{
		Fmt (temp_buff, "%s<%s[i5w53]\n", returned_string);
	}
	
	WriteToTerminal(panel, temp_buff);
	
	
	if(NULL == strstr(temp_buff,HW_Revision))
	{

		status = 0;
		sprintf (temp_buff_2, "Expected HW Revision: %s \n", HW_Revision);

		WriteToTerminal(panel, temp_buff_2);


	}
	
	
	sprintf(msg,"%s,",HW_Revision);
	strcat(data_string,msg);
	
	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
	}
	
	return status;
	
}	



//*************************************************************
//* 
//*
//*************************************************************
int DECT_Park_Search_Criteria_Test (int panel)
{
	
	int status = 0;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	int counter = 0;
	
	
	sprintf(msg,"DECT Park Search Criteria Test\n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg); 
	WriteToTerminal(panel, msg); 
	
	while((counter < 10) && (!(status == 1))){  
		status = DECT_Send_String (panel, ParkSearchCriteria, ParkSearchCriteria_Response,returned_string,0,1,1);        
		Delay(1);
		counter++;
	}
	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);	
	}
	
	return status;

}
	

//*************************************************************
//* 
//*
//*************************************************************
int DECT_EEPROM_PLI_1_Test (int panel)
{
	
	int status = 0;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	int counter = 0;
	
	sprintf(msg,"DECT EEPROM PLI 1 Test\n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg); 
	WriteToTerminal(panel, msg); 
	
	while((counter < 10) && (!(status == 1))){  
		status = DECT_Send_String (panel, EepromPli1, EepromPli1_Response,returned_string,0,1,1);         
		Delay(1);
		counter++;
	}
	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);	
	}
	
	return status;

}



//*************************************************************
//* 
//*
//*************************************************************
int DECT_EEPROM_Park_1_Test (int panel)
{
	
	int status = 0;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	int counter = 0;
	
	
	sprintf(msg,"DECT EEPROM Park 1 Test\n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg); 
	WriteToTerminal(panel, msg); 
	
	
	while((counter < 10) && (!(status == 1))){  
		status = DECT_Send_String (panel, EepromPark1, EepromPark1_Response,returned_string,0,1,1);          
		Delay(1);
		counter++;
	}
	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);	
	}
	
	return status;

}



//*************************************************************
//* 
//*
//*************************************************************
int DECT_Get_Service_State_Test (int panel)
{
	
	int status = 0;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	int counter = 0;
	
	sprintf(msg,"DECT Get Service State Test\n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg); 
	WriteToTerminal(panel, msg); 
	
	
	while((counter < 20) && (!(status == 1))){  
		status = DECT_Send_String (panel, GetServiceState, GetServiceState_Response,returned_string,0,1,2);
		Delay(1);
		counter++;
	}
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg);
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);
	
	}
	
	return status;

}



//*************************************************************
//* 
//*
//*************************************************************
int DECT_Measure_NTP_Power_Test (int panel)
{
	int status = 0;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	double  Measured_Value = 0; 
	char temp_buff[256] = {'\0'};
	int numToRead;
	int counter = 0;
	int counter2 = 0;
	
	sprintf(msg,"DECT Measure NTP Power Test\n");
	WriteToTerminal(panel, msg); 
	
   	//CMD60 modulation setting data setting of 00001111
   	ibwrt (CMD60_HANDLE, "CONF:MOD:DATA BS0F", strlen("CONF:MOD:DATA BS0F"));
   	if (ibsta & 0x8000) status = 0;  
   
   	ibwrt (CMD60_HANDLE, "READ:NTP?", strlen("READ:NTP?"));
   	if (ibsta & 0x8000) status = 0;     

   	numToRead = 12;
   	ibrd (CMD60_HANDLE, temp_buff, numToRead);
   	if (ibsta & 0x8000) status = 0;
	
	sprintf (msg, "cable loss: %f \n", rfoffset);	
	WriteToTerminal(panel, msg);

   	Measured_Value = atof ( temp_buff ) + (rfoffset);
	
	sprintf (temp_buff, "NTP: %.4f \n", Measured_Value);	
	WriteToTerminal(panel, temp_buff);
	
	while((Measured_Value <20.5)&&(counter2 < 3)){
		if(Measured_Value < 20.5){
			Measured_Value = 0;	
		
			sprintf (temp_buff, "Retry NTP Measurement\n");	
			WriteToTerminal(panel, temp_buff);
		
			//Open CMD60
			ibwrt (CMD60_HANDLE, "IFC", strlen("IFC"));
	   		if (ibsta & 0x8000) status = 0;
   
	   		//send device clear to the instrunet
	   		ibwrt (CMD60_HANDLE, "DCL", strlen("DCL")); 
	   		if (ibsta & 0x8000) status = 0;
   
	    	//clear the device
	   		ibclr (CMD60_HANDLE);
	   		if (ibsta & 0x8000) status = 0;  
   
	   		//clear status of the instrument
	   		ibwrt (CMD60_HANDLE, "*CLS", strlen("*CLS"));
	   		if (ibsta & 0x8000) status = 0;
   
	   		//reset the instrunet
	   		ibwrt (CMD60_HANDLE, "*RST", strlen("*RST")); 
	   		if (ibsta & 0x8000) status = 0;
		
			//DECT Loopback EU
			ibwrt (CMD60_HANDLE, "CONF:TEST:TYPE DPP", strlen("CONF:TEST:TYPE DPP"));
			if (ibsta & 0x8000) status = 0; 
   
			ibwrt (CMD60_HANDLE, "PROC:SEL:TEST MAN", strlen("PROC:SEL:TEST MAN"));
			if (ibsta & 0x8000) status = 0;
   
			ibwrt (CMD60_HANDLE, "CONF:PP:SIGN:MODE LOOP", strlen("CONF:PP:SIGN:MODE LOOP"));
			if (ibsta & 0x8000) status = 0; 
   
			ibwrt (CMD60_HANDLE, "CONF:POW:PACK PP32Z", strlen("CONF:POW:PACK PP32Z"));
			if (ibsta & 0x8000) status = 0;
   
			ibwrt (CMD60_HANDLE, "CONF:PP:RFPI '0001400000'", strlen("CONF:PP:RFPI '0001400000'"));
			if (ibsta & 0x8000) status = 0; 
   
			ibwrt (CMD60_HANDLE, "CONF:PP:RF:LEV -45.0dBm", strlen("CONF:PP:RF:LEV -45.0dBm"));
			if (ibsta & 0x8000) status = 0;
   
			ibwrt (CMD60_HANDLE, "CONF:PP:DUMM:SLOT 0", strlen("CONF:PP:DUMM:SLOT 0"));
			if (ibsta & 0x8000) status = 0; 
   
			ibwrt (CMD60_HANDLE, "CONF:PP:DUMM:CARR 5", strlen("CONF:PP:DUMM:CARR 5"));
			if (ibsta & 0x8000) status = 0;
   
			ibwrt (CMD60_HANDLE, "CONF:PP:TRAF:SLOT 2", strlen("CONF:PP:TRAF:SLOT 2"));
			if (ibsta & 0x8000) status = 0; 
   
			ibwrt (CMD60_HANDLE, "CONF:PP:TRAF:CARR 0", strlen("CONF:PP:TRAF:CARR 0"));	  	//conductive
			if (ibsta & 0x8000) status = 0;
		
		
			counter = 0; 
		
			//Get Service State
			while((counter < 10) && (!(status == 1))){  
				status = DECT_Send_String (panel, GetServiceState, GetServiceState_Response,returned_string,0,1,2);
				Delay(1);
				counter++;
			}
		
			counter = 0;
			
			memset(temp_buff, '\0', sizeof(temp_buff));
		
			//Connect CMD60
			while((counter < 10) && (!(strstr (temp_buff, "DCE")))){
				//set to establish connection with dut
				ibwrt (CMD60_HANDLE, "PROC:CONN:SET", strlen("PROC:CONN:SET"));
			   	if (ibsta & 0x8000) status = 0;
		
				Delay(1);
		
				ibwrt (CMD60_HANDLE, "STAT:DEV?", strlen("STAT:DEV?"));
			   	if (ibsta & 0x8000) status = 0;
   
				//read the instrument for DCE (DECT Connection Established)
			   	numToRead = 3;
			   	ibrd (CMD60_HANDLE, temp_buff, numToRead);
			   	if (ibsta & 0x8000) status = 0;
		
				Delay(1);
		
				counter++;
			}
			
			memset(temp_buff, '\0', sizeof(temp_buff));
		
			//Read NTP
	   		ibwrt (CMD60_HANDLE, "CONF:MOD:DATA BS0F", strlen("CONF:MOD:DATA BS0F"));
		   	if (ibsta & 0x8000) status = 0;  
   
		   	ibwrt (CMD60_HANDLE, "READ:NTP?", strlen("READ:NTP?"));
		   	if (ibsta & 0x8000) status = 0;     

		   	numToRead = 12;
		   	ibrd (CMD60_HANDLE, temp_buff, numToRead);
		   	if (ibsta & 0x8000) status = 0;
			
			sprintf (msg, "cable loss: %f \n", rfoffset);	
			WriteToTerminal(panel, msg);

		   	Measured_Value = atof ( temp_buff ) +(rfoffset); 
		
			sprintf (temp_buff, "NTP: %.4f \n", Measured_Value);	
			WriteToTerminal(panel, temp_buff);
		
		}
		counter2++;
	}
	
	sprintf(msg,"%f,",Measured_Value);
	strcat(data_string,msg);
	
	if ((Measured_Value >= 20.5) && (Measured_Value <= 25.5)){  
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		//SetCtrlVal (panel, PANEL_Serial_Window_1, "Test Pass\n\n");
		status = 1;
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		//SetCtrlVal (panel, PANEL_Serial_Window_1, "Test Fail\n\n");
		status = 0;
	}
	
	return status;

}

//*************************************************************
//*
//*
//*************************************************************
int DECT_Measure_RTX_NTP_Power_Test (int panel)
{
	int status = 0;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	double  Measured_Value = 0;
	char temp_buff[256] = {'\0'};
	int numToRead;
	int counter = 0;
	int counter2 = 0;

	sprintf(msg,"DECT Measure NTP Power Test\n");

	WriteToTerminal(panel, msg);


	rtx2012_Read_NTP(temp_buff);  //if execute fail, should return -999
	
	sprintf (msg, "cable loss: %f \n", rfoffset);	
	WriteToTerminal(panel, msg);


	Measured_Value = atof(temp_buff) + (rfoffset);

	sprintf (temp_buff, "NTP: %.4f \n", Measured_Value);
	WriteToTerminal(panel, temp_buff);


	while((Measured_Value <20.5)&&(counter2 < 3))
	{
		if(Measured_Value < 20.5)
		{
			Measured_Value = 0;

			sprintf (temp_buff, "Retry NTP Measurement\n");
			WriteToTerminal(panel, temp_buff);

			Open_RTX(panel);
			Set_RTX_in_Loopback_Mode_EU_Setting(panel);


			counter = 0;

			//Get Service State
			while((counter < 10) && (!(status == 1)))
			{
				status = DECT_Send_String (panel, GetServiceState, GetServiceState_Response,returned_string,0,1,2);
				Delay(1);
				counter++;
			}

			counter = 0;

			memset(temp_buff, '\0', sizeof(temp_buff));

			//Connect RTX
			Setup_Connect_RTX(panel);

			memset(temp_buff, '\0', sizeof(temp_buff));

			//Read NTP
			rtx2012_Read_NTP(&Measured_Value);  //if execute fail, should return -999


			sprintf (temp_buff, "NTP: %.4f \n", Measured_Value);
			WriteToTerminal(panel, temp_buff);

		}
		counter2++;
	}

	sprintf(msg,"%f,",Measured_Value);
	strcat(data_string,msg);

	if ((Measured_Value >= 20.5) && (Measured_Value <= 25.5))
	{
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg);
		status = 1;
	}
	else
	{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg);
		status = 0;
	}

	return status;

}



//*************************************************************
//* 
//*
//*************************************************************
int DECT_Measure_NTP_Power_Radiated_Test (int panel)
{
	int status = 0;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	double  Measured_Value; 
	char temp_buff[256] = {'\0'};
	int numToRead;
	int i;
	int counter = 0;
	int counter2 = 0;
	
	sprintf(msg,"DECT Measure NTP Power Radiated Test\n");
	WriteToTerminal(panel, msg); 
	
	//CMD60 modulation setting data setting of 00001111
   	ibwrt (CMD60_HANDLE, "CONF:MOD:DATA BS0F", strlen("CONF:MOD:DATA BS0F"));
   	if (ibsta & 0x8000) status = 0;  
   
   	ibwrt (CMD60_HANDLE, "READ:NTP?", strlen("READ:NTP?"));
   	if (ibsta & 0x8000) status = 0;     

   	numToRead = 12;
   	ibrd (CMD60_HANDLE, temp_buff, numToRead);
   	if (ibsta & 0x8000) status = 0;
	
		sprintf (msg, "cable loss: %f \n", rfoffset);	
	WriteToTerminal(panel, msg);

   	Measured_Value = atof ( temp_buff ) + (rfoffset); ;
	
	sprintf (temp_buff, "NTP: %.4f \n", Measured_Value);	
	WriteToTerminal(panel, temp_buff);
	
	while((Measured_Value <= 0)&&(counter2 < 3)){
		if(Measured_Value <= 0){
			Measured_Value = 0;	
		
			sprintf (temp_buff, "Retry NTP Measurement\n");	
			WriteToTerminal(panel, temp_buff);
		
			//Open CMD60
			ibwrt (CMD60_HANDLE, "IFC", strlen("IFC"));
	   		if (ibsta & 0x8000) status = 0;
   
	   		//send device clear to the instrunet
	   		ibwrt (CMD60_HANDLE, "DCL", strlen("DCL")); 
	   		if (ibsta & 0x8000) status = 0;
   
	    	//clear the device
	   		ibclr (CMD60_HANDLE);
	   		if (ibsta & 0x8000) status = 0;  
   
	   		//clear status of the instrument
	   		ibwrt (CMD60_HANDLE, "*CLS", strlen("*CLS"));
	   		if (ibsta & 0x8000) status = 0;
   
	   		//reset the instrunet
	   		ibwrt (CMD60_HANDLE, "*RST", strlen("*RST")); 
	   		if (ibsta & 0x8000) status = 0;
		
			//DECT Loopback EU
			ibwrt (CMD60_HANDLE, "CONF:TEST:TYPE DPP", strlen("CONF:TEST:TYPE DPP"));
			if (ibsta & 0x8000) status = 0; 
   
			ibwrt (CMD60_HANDLE, "PROC:SEL:TEST MAN", strlen("PROC:SEL:TEST MAN"));
			if (ibsta & 0x8000) status = 0;
   
			ibwrt (CMD60_HANDLE, "CONF:PP:SIGN:MODE LOOP", strlen("CONF:PP:SIGN:MODE LOOP"));
			if (ibsta & 0x8000) status = 0; 
   
			ibwrt (CMD60_HANDLE, "CONF:POW:PACK PP32Z", strlen("CONF:POW:PACK PP32Z"));
			if (ibsta & 0x8000) status = 0;
   
			ibwrt (CMD60_HANDLE, "CONF:PP:RFPI '0001400000'", strlen("CONF:PP:RFPI '0001400000'"));
			if (ibsta & 0x8000) status = 0; 
   
			ibwrt (CMD60_HANDLE, "CONF:PP:RF:LEV -45.0dBm", strlen("CONF:PP:RF:LEV -45.0dBm"));
			if (ibsta & 0x8000) status = 0;
   
			ibwrt (CMD60_HANDLE, "CONF:PP:DUMM:SLOT 0", strlen("CONF:PP:DUMM:SLOT 0"));
			if (ibsta & 0x8000) status = 0; 
   
			ibwrt (CMD60_HANDLE, "CONF:PP:DUMM:CARR 5", strlen("CONF:PP:DUMM:CARR 5"));
			if (ibsta & 0x8000) status = 0;
   
			ibwrt (CMD60_HANDLE, "CONF:PP:TRAF:SLOT 2", strlen("CONF:PP:TRAF:SLOT 2"));
			if (ibsta & 0x8000) status = 0; 
   
			ibwrt (CMD60_HANDLE, "CONF:PP:TRAF:CARR 0", strlen("CONF:PP:TRAF:CARR 0"));	  	//conductive
			if (ibsta & 0x8000) status = 0;
		
		
			counter = 0; 
		
			//Get Service State
			while((counter < 10) && (!(status == 1))){  
				status = DECT_Send_String (panel, GetServiceState, GetServiceState_Response,returned_string,0,1,2);
				Delay(1);
				counter++;
			}
		
			counter = 0;
			
			memset(temp_buff, '\0', sizeof(temp_buff));
		
			//Connect CMD60
			while((counter < 10) && (!(strstr (temp_buff, "DCE")))){
				//set to establish connection with dut
				ibwrt (CMD60_HANDLE, "PROC:CONN:SET", strlen("PROC:CONN:SET"));
			   	if (ibsta & 0x8000) status = 0;
		
				Delay(1);
		
				ibwrt (CMD60_HANDLE, "STAT:DEV?", strlen("STAT:DEV?"));
			   	if (ibsta & 0x8000) status = 0;
   
				//read the instrument for DCE (DECT Connection Established)
			   	numToRead = 3;
			   	ibrd (CMD60_HANDLE, temp_buff, numToRead);
			   	if (ibsta & 0x8000) status = 0;
		
				Delay(1);
		
				counter++;
			}
			
			memset(temp_buff, '\0', sizeof(temp_buff));
		
			//Read NTP
	   		ibwrt (CMD60_HANDLE, "CONF:MOD:DATA BS0F", strlen("CONF:MOD:DATA BS0F"));
		   	if (ibsta & 0x8000) status = 0;  
   
		   	ibwrt (CMD60_HANDLE, "READ:NTP?", strlen("READ:NTP?"));
		   	if (ibsta & 0x8000) status = 0;     

		   	numToRead = 12;
		   	ibrd (CMD60_HANDLE, temp_buff, numToRead);
		   	if (ibsta & 0x8000) status = 0;
			
				sprintf (msg, "cable loss: %f \n", rfoffset);	
	WriteToTerminal(panel, msg);

		   	Measured_Value = atof ( temp_buff ) + (rfoffset); ;
		
			sprintf (temp_buff, "NTP: %.4f \n", Measured_Value);	
			WriteToTerminal(panel, temp_buff);
		
		}
		counter2++;
	}
	
	sprintf(msg,"%f,",Measured_Value);
	strcat(data_string,msg);
	
	if ((Measured_Value > 0) && (Measured_Value <= 25)){  
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		//SetCtrlVal (panel, PANEL_Serial_Window_1, "Test Pass\n\n");
		status = 1;
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		//SetCtrlVal (panel, PANEL_Serial_Window_1, "Test Fail\n\n");
		status = 0;
	}
	
	return status;

}


//*************************************************************
//* 
//*
//*************************************************************
int DECT_Measure_EU_Deviation_Test (int panel)
{
	int status = 0;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	double  Measured_Value [2] = {'\0'}; 
	char temp_buff[256] = {'\0'};
	int numToRead;
	int	EU_Deviation_Value;
	int x;
	char *temp_pointer;
	int	Measure_Deviation;
	double  Positive_Deviation;
	double  Negative_Deviation;
	int counter = 0;
	
	sprintf(msg,"DECT Measure Deviation Test\n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg); 
	WriteToTerminal(panel, msg); 
	
	
	while((counter < 10) && (!(status == 1))){  
		status = DECT_Send_String (panel, ReadEepromDeviationEU, ReadEepromDeviationEU_Response,returned_string,0,1,1);           
		Delay(1);
		counter++;
	}
	
	EU_Deviation_Value = returned_string[5];
	
	
	//CMD60 modulation setting data setting of 00001111
   	ibwrt (CMD60_HANDLE, "CONF:MOD:DATA BS0F", strlen("CONF:MOD:DATA BS0F"));
   	if (ibsta & 0x8000) status = 0;  
 
   	//we may have to loop around to get right output from this measurment
   	ibwrt (CMD60_HANDLE, "READ:BFIeld?", strlen("READ:BFIeld?"));
   	if (ibsta & 0x8000) status = 0;     

   	numToRead = 26;
   	ibrd (CMD60_HANDLE, temp_buff, numToRead);
   	if (ibsta & 0x8000) status = 0;

   	temp_pointer = strtok (temp_buff, ",");
   
   	x = 0;
   
   	while (temp_pointer != NULL)
   	{
		 Measured_Value [x] = atof (temp_pointer);
		 temp_pointer = strtok (NULL, ","); 
		 x = x + 1; 
   	}

	//+ve deviation
   	Positive_Deviation = (double) (Measured_Value [0] / 1000);
	//-ve deviation
   	Negative_Deviation = (double) (Measured_Value [1] / 1000); 
	
	sprintf (temp_buff, "Positive Deviation: %.4f kHz\n", Positive_Deviation);	
    //SetCtrlVal (panel, PANEL_Serial_Window_1, temp_buff);
	WriteToTerminal(panel, temp_buff);
	sprintf(msg,"%f,",Positive_Deviation);
	strcat(data_string,msg);
	
	
	sprintf (temp_buff, "Negative Deviation: %.4f kHz\n", Negative_Deviation);	
    //SetCtrlVal (panel, PANEL_Serial_Window_1, temp_buff);
	WriteToTerminal(panel, temp_buff);
	sprintf(msg,"%f,",Negative_Deviation);
	strcat(data_string,msg);
	
	if (((Positive_Deviation >= 360) && (Positive_Deviation <= 425))&&((Negative_Deviation >= -425) && (Negative_Deviation <= -360))){ 
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		//SetCtrlVal (panel, PANEL_Serial_Window_1, "Test Pass\n\n");
		status = 1;
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		//SetCtrlVal (panel, PANEL_Serial_Window_1, "Test Fail\n\n");
		status = 0;
	}
	
	return status;

}

//*************************************************************
//*
//*
//*************************************************************
int DECT_Measure_RTX_EU_Deviation_Test (int panel)
{
	int status = 0;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	double  Measured_Value [2] = {'\0'};
	char temp_buff[256] = {'\0'};
	int numToRead;
	int	EU_Deviation_Value;
	int x;
	char *temp_pointer;
	int	Measure_Deviation;
	double  Positive_Deviation;
	double  Negative_Deviation;
	int counter = 0;

	sprintf(msg,"DECT Measure Deviation Test\n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg);
	WriteToTerminal(panel, msg);


	while((counter < 10) && (!(status == 1)))
	{
		status = DECT_Send_String (panel, ReadEepromDeviationEU, ReadEepromDeviationEU_Response,returned_string,0,1,1);
		Delay(1);
		counter++;
	}

	EU_Deviation_Value = returned_string[5];


	if(rtx2012_Read_Deviation(temp_buff)!= 1)
	{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg);
		return 0;
	}

	temp_pointer = strtok (temp_buff, ",");

	x = 0;

	while (temp_pointer != NULL)
	{
		Measured_Value [x] = atof (temp_pointer);
		temp_pointer = strtok (NULL, ",");
		x = x + 1;
	}

	//+ve deviation
	Positive_Deviation = (double) (Measured_Value [0] / 1000);
	//-ve deviation
	Negative_Deviation = (double) (Measured_Value [1] / 1000);

	sprintf (temp_buff, "Positive Deviation: %.4f kHz\n", Positive_Deviation);
	//SetCtrlVal (panel, PANEL_Serial_Window_1, temp_buff);
	WriteToTerminal(panel, temp_buff);
	sprintf(msg,"%f,",Positive_Deviation);
	strcat(data_string,msg);


	sprintf (temp_buff, "Negative Deviation: %.4f kHz\n", Negative_Deviation);
	//SetCtrlVal (panel, PANEL_Serial_Window_1, temp_buff);
	WriteToTerminal(panel, temp_buff);
	sprintf(msg,"%f,",Negative_Deviation);
	strcat(data_string,msg);

	if (((Positive_Deviation >= 360) && (Positive_Deviation <= 425))&&((Negative_Deviation >= -425) && (Negative_Deviation <= -360)))
	{
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg);
		//SetCtrlVal (panel, PANEL_Serial_Window_1, "Test Pass\n\n");
		status = 1;
	}
	else
	{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg);
		//SetCtrlVal (panel, PANEL_Serial_Window_1, "Test Fail\n\n");
		status = 0;
	}

	return status;

}




//*************************************************************
//* 
//*
//*************************************************************
int DECT_Measure_Frequency_Offset_Test (int panel)
{
	int status = 0;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	double  Measured_Value; 
	char temp_buff[256] = {'\0'};
	int numToRead;
	double	Frequency_Offset; 
	
	sprintf(msg,"DECT Measure Frequency Offset Test\n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg); 
	WriteToTerminal(panel, msg); 
	
   	//CMD60 modulation setting data setting of 00001111
   	ibwrt (CMD60_HANDLE, "CONF:MOD:DATA BS0F", strlen("CONF:MOD:DATA BS0F"));
   	if (ibsta & 0x8000) status = 0;  
 
   	//we may have to loop around to get right output from this measurment
   	ibwrt (CMD60_HANDLE, "READ:FREQ:OFFS?", strlen("READ:FREQ:OFFS?"));
   	if (ibsta & 0x8000) status = 0;     

   	numToRead = 26;
   	ibrd (CMD60_HANDLE, temp_buff, numToRead);
   	if (ibsta & 0x8000) status = 0;

   	Measured_Value = (atof (temp_buff))/1000;
	
	sprintf (temp_buff, "Frequency Offset: %.4fkHz \n", Measured_Value);	
    //SetCtrlVal (panel, PANEL_Serial_Window_1, temp_buff);
	WriteToTerminal(panel, temp_buff);
	
	sprintf(msg,"%f,",Measured_Value);
	strcat(data_string,msg);
	
	if ((Measured_Value >= -30) && (Measured_Value <= 30)){  
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		//SetCtrlVal (panel, PANEL_Serial_Window_1, "Test Pass\n\n");
		status = 1;
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		//SetCtrlVal (panel, PANEL_Serial_Window_1, "Test Fail\n\n");
		status = 0;
	}
	
	return status;

}


//*************************************************************
//*
//*
//*************************************************************
int DECT_Measure_RTX_Frequency_Offset_Test (int panel)
{
	int status = 0;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	double  Measured_Value;
	char temp_buff[256] = {'\0'};
	int numToRead;
	double	Frequency_Offset;

	sprintf(msg,"DECT Measure Frequency Offset Test\n");
	WriteToTerminal(panel, msg);

	if(rtx2012_Read_FrequencyOffset(temp_buff)!= 1)
	{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg);
		return 0;
	}

	Measured_Value = (atof (temp_buff))/1000;

	sprintf (temp_buff, "Frequency Offset: %.4fkHz \n", Measured_Value);
	//SetCtrlVal (panel, PANEL_Serial_Window_1, temp_buff);
	WriteToTerminal(panel, temp_buff);

	sprintf(msg,"%f,",Measured_Value);
	strcat(data_string,msg);

	if ((Measured_Value >= -30) && (Measured_Value <= 30))
	{
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg);
		//SetCtrlVal (panel, PANEL_Serial_Window_1, "Test Pass\n\n");
		status = 1;
	}
	else
	{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg);
		//SetCtrlVal (panel, PANEL_Serial_Window_1, "Test Fail\n\n");
		status = 0;
	}

	return status;

}



//*************************************************************
//* 
//*
//*************************************************************
int DECT_Measure_Frequency_Drift_Test (int panel)
{
	int status = 0;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	double  Measured_Value; 
	char temp_buff[256] = {'\0'};
	int numToRead;
	double	Frequency_Offset; 
	
	sprintf(msg,"DECT Measure Frequency Drift Test\n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg); 
	WriteToTerminal(panel, msg); 
	
   //CMD60 modulation setting data setting of 01010101
   ibwrt (CMD60_HANDLE, "CONF:MOD:DATA BS55", strlen("CONF:MOD:DATA BS55"));
   if (ibsta & 0x8000) status = 0;  
   
   //configure cmd60 for drift unit / slot
   ibwrt (CMD60_HANDLE, "CONF:MOD:DRIF:UNIT SLOT", strlen("CONF:MOD:DRIF:UNIT SLOT"));
   if (ibsta & 0x8000) status = 0;
 
   //we may have to loop around to get right output from this measurment
   ibwrt (CMD60_HANDLE, "READ:FREQ:DRIF:SLOT?", strlen("READ:FREQ:DRIF:SLOT?"));
   if (ibsta & 0x8000) status = 0;     

   numToRead = 26;
   ibrd (CMD60_HANDLE, temp_buff, numToRead);
   if (ibsta & 0x8000) status = 0;

   Measured_Value = (atof (temp_buff))/1000;  			 //no limit
	
	sprintf (temp_buff, "Frequency Drift: %.4fkHz \n", Measured_Value);	
    //SetCtrlVal (panel, PANEL_Serial_Window_1, temp_buff);
	WriteToTerminal(panel, temp_buff);
	
	sprintf(msg,"%f,",Measured_Value);
	strcat(data_string,msg);
	
	if ((Measured_Value >= -5) && (Measured_Value <= 5)){ 
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		//SetCtrlVal (panel, PANEL_Serial_Window_1, "Test Pass\n\n");
		status = 1;
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		//SetCtrlVal (panel, PANEL_Serial_Window_1, "Test Fail\n\n");
		status = 0;
	}
	
	return status;

}

//*************************************************************
//*
//*
//*************************************************************
int DECT_Measure_RTX_Frequency_Drift_Test (int panel)
{
	int status = 0;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	double  Measured_Value;
	char temp_buff[256] = {'\0'};
	int numToRead;
	double	Frequency_Offset;

	sprintf(msg,"DECT Measure Frequency Drift Test\n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg);
	WriteToTerminal(panel, msg);


	if(rtx2012_Read_FrequencyDriftSlot(temp_buff) != 1)
	{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg);
		return 0;
	}



	Measured_Value = (atof (temp_buff))/1000000;		 // rtx feedback the unit is Hz, not kHz. 		 actually no limit for this item   according to spec

	sprintf (temp_buff, "Frequency Drift: %.4fkHz \n", Measured_Value);
	//SetCtrlVal (panel, PANEL_Serial_Window_1, temp_buff);
	WriteToTerminal(panel, temp_buff);

	sprintf(msg,"%f,",Measured_Value);
	strcat(data_string,msg);

	if ((Measured_Value >= -5) && (Measured_Value <= 5))
	{
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg);
		//SetCtrlVal (panel, PANEL_Serial_Window_1, "Test Pass\n\n");
		status = 1;
	}
	else
	{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg);
		//SetCtrlVal (panel, PANEL_Serial_Window_1, "Test Fail\n\n");
		status = 0;
	}

	return status;

}



//*************************************************************
//* 
//*
//*************************************************************
int DECT_Measure_RSSI_45dBm_Test (int panel)
{
	int status = 0;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	int Measured_Value; 
	char temp_buff[256] = {'\0'};
	int numToRead;
	double Frequency_Offset; 
	int counter = 0;
	
	sprintf(msg,"DECT Measure RSSI -45dBm Test\n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg);
	WriteToTerminal(panel, msg); 
	
	
	RSSI_45dBm = 0;

   	//CMD60 modulation setting data setting of 00001111
   	ibwrt (CMD60_HANDLE, "CONF:MOD:DATA BS0F", strlen("CONF:MOD:DATA BS0F"));
   	if (ibsta & 0x8000) status = 0;  
   
	
	//set the power to -75dBm
   	ibwrt (CMD60_HANDLE, "CONF:PP:RF:LEV -45.0dBm", strlen("CONF:PP:RF:LEV -45.0dBm"));
   	if (ibsta & 0x8000) status = 0;
	
	Delay(3);
   
	status = DECT_Send_String (panel, GetRssi, GetRssi_Response,returned_string,0,1,1);           

   	Measured_Value = returned_string[5];
	
	RSSI_45dBm = Measured_Value;
	
	sprintf (temp_buff, "RSSI at -45dBm: %d \n", Measured_Value);	
    //SetCtrlVal (panel, PANEL_Serial_Window_1, temp_buff);
	WriteToTerminal(panel, temp_buff);
	
	sprintf(msg,"%d,",Measured_Value);
	strcat(data_string,msg);
	
	if ((Measured_Value >= 38) && (Measured_Value <= 50)){  
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		//SetCtrlVal (panel, PANEL_Serial_Window_1, "Test Pass\n\n");
		status = 1;
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		//SetCtrlVal (panel, PANEL_Serial_Window_1, "Test Fail\n\n");
		status = 0;
	}
	
	return status;

}



//*************************************************************
//*
//*
//*************************************************************
int DECT_Measure_RTX_RSSI_45dBm_Test (int panel)
{
	int status = 0;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	int Measured_Value;
	char temp_buff[256] = {'\0'};
	int numToRead;
	double Frequency_Offset;
	int counter = 0;

	sprintf(msg,"DECT Measure RSSI -45dBm Test\n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg);
	WriteToTerminal(panel, msg);


	RSSI_45dBm = 0;

	//CMD60 modulation setting data setting of 00001111
	int i = rtx2012_Write("CONF:BER:DATA BS0F");
	if(i != 1)
		return 0;

	i = rtx2012_Write("CONF:PP:RF:LEV -45.0");
	if(i != 1)
		return 0;


	Delay(3);

	status = DECT_Send_String (panel, GetRssi, GetRssi_Response,returned_string,0,1,1);

	Measured_Value = returned_string[5];

	RSSI_45dBm = Measured_Value;

	sprintf (temp_buff, "RSSI at -45dBm: %d \n", Measured_Value);
	//SetCtrlVal (panel, PANEL_Serial_Window_1, temp_buff);
	WriteToTerminal(panel, temp_buff);

	sprintf(msg,"%d,",Measured_Value);
	strcat(data_string,msg);

	if ((Measured_Value >= 38) && (Measured_Value <= 50))
	{
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg);
		//SetCtrlVal (panel, PANEL_Serial_Window_1, "Test Pass\n\n");
		status = 1;
	}
	else
	{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg);
		//SetCtrlVal (panel, PANEL_Serial_Window_1, "Test Fail\n\n");
		status = 0;
	}

	return status;

}



//*************************************************************
//* 
//*
//*************************************************************
int DECT_Measure_RSSI_75dBm_Test (int panel)
{
	int status = 0;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	int Measured_Value; 
	char temp_buff[256] = {'\0'};
	int numToRead;
	double Frequency_Offset;
	int counter = 0;
	
	RSSI_75dBm = 0;
	
	sprintf(msg,"DECT Measure RSSI -75dBm Test\n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg); 
	WriteToTerminal(panel, msg); 
	
   	//CMD60 modulation setting data setting of 00001111
   	ibwrt (CMD60_HANDLE, "CONF:MOD:DATA BS0F", strlen("CONF:MOD:DATA BS0F"));
   	if (ibsta & 0x8000) status = 0;  
   
	
	//set the power to -75dBm
   	ibwrt (CMD60_HANDLE, "CONF:PP:RF:LEV -75.0dBm", strlen("CONF:PP:RF:LEV -75.0dBm"));  
   	if (ibsta & 0x8000) status = 0;   
	
	Delay(3);
   
	status = DECT_Send_String (panel, GetRssi, GetRssi_Response,returned_string,0,1,1);           

   	Measured_Value = returned_string[5];
	
	RSSI_75dBm = Measured_Value; 
	
	sprintf (temp_buff, "RSSI at -75dBm: %d \n", Measured_Value);	
    //SetCtrlVal (panel, PANEL_Serial_Window_1, temp_buff);
	WriteToTerminal(panel, temp_buff);
	
	sprintf(msg,"%d,",Measured_Value);
	strcat(data_string,msg);
	
	if ((Measured_Value >= 22) && (Measured_Value <= 38)){  
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		//SetCtrlVal (panel, PANEL_Serial_Window_1, "Test Pass\n\n");
		status = 1;
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		//SetCtrlVal (panel, PANEL_Serial_Window_1, "Test Fail\n\n");
		status = 0;
	}
	
	return status;

}

//*************************************************************
//*
//*
//*************************************************************
int DECT_Measure_RTX_RSSI_75dBm_Test (int panel)
{
	int status = 0;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	int Measured_Value;
	char temp_buff[256] = {'\0'};
	int numToRead;
	double Frequency_Offset;
	int counter = 0;

	RSSI_75dBm = 0;

	sprintf(msg,"DECT Measure RSSI -75dBm Test\n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg);
	WriteToTerminal(panel, msg);


	//rtx2012_Write("CONF:MOD:DATA BS0F");

	int i = rtx2012_Write("CONF:BER:DATA BS0F");
	if(i != 1)
		return 0;

	i = rtx2012_Write("CONF:PP:RF:LEV -75.0");
	if(i != 1)
		return 0;


	Delay(3);

	status = DECT_Send_String (panel, GetRssi, GetRssi_Response,returned_string,0,1,1);

	Measured_Value = returned_string[5];

	RSSI_75dBm = Measured_Value;

	sprintf (temp_buff, "RSSI at -75dBm: %d \n", Measured_Value);
	//SetCtrlVal (panel, PANEL_Serial_Window_1, temp_buff);
	WriteToTerminal(panel, temp_buff);

	sprintf(msg,"%d,",Measured_Value);
	strcat(data_string,msg);

	if ((Measured_Value >= 22) && (Measured_Value <= 38))
	{
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg);
		//SetCtrlVal (panel, PANEL_Serial_Window_1, "Test Pass\n\n");
		status = 1;
	}
	else
	{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg);
		//SetCtrlVal (panel, PANEL_Serial_Window_1, "Test Fail\n\n");
		status = 0;
	}

	return status;

}


//*************************************************************
//* 
//*
//*************************************************************
int DECT_Program_RSSI_K_Factor_Test (int panel)
{
	int status = 0;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	int Measured_Value; 
	char temp_buff[256] = {'\0'};
	int numToRead;
	double Frequency_Offset; 
	float M_DEC, X_DEC;
	int	Eeprom_Rssi_Comp_K_Factor;
	int	Eeprom_Rssi_Comp_Offset;
	unsigned char EepromRssiCompKFactor[13];
	unsigned char EepromRssiCompOffset[13];
	int x;
	int counter = 0;
	
	sprintf(msg,"DECT Program RSSI K Factor Test\n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg); 
	WriteToTerminal(panel, msg); 
	
	
	M_DEC = (float) (RSSI_45dBm - RSSI_75dBm) / 30.0;
	X_DEC = (float) ((-45) - (RSSI_45dBm/M_DEC)); 
	
	Eeprom_Rssi_Comp_K_Factor = (RoundRealToNearestInteger ((1.0/M_DEC)*16.0));
	Eeprom_Rssi_Comp_Offset = ((-1.0)*RoundRealToNearestInteger (X_DEC));
	
	
	//copy EepromRssiCompKFactor_00 to EepromRssiCompKFactor
	for (x = 0; x < 13; x++){
		EepromRssiCompKFactor[x] = EepromRssiCompKFactor_00[x];	
	}
	
	//write the value to 11th byte first
	EepromRssiCompKFactor [11] = (char) Eeprom_Rssi_Comp_K_Factor;
	
	//to generate correct command to write the rssi comp k factor
	//xor the last byte of the EepromRssiCompKFactor_00, which is 0x96
	//with the Eeprom_Rssi_Comp_K_Factor_TP501
	x = (0x96 ^ Eeprom_Rssi_Comp_K_Factor);

	//write the result as the last byte
	EepromRssiCompKFactor [12] = (char) x;
	
//	Delay(3);
   
   	while((counter < 10) && (!(status == 1))){  
		status = DECT_Send_String (panel, EepromRssiCompKFactor, EepromRssiCompKFactor_00_Response,returned_string,0,1,1);            
		Delay(1);
		counter++;
	}

	sprintf (temp_buff, "RSSI K Factor: 0x%X \n", EepromRssiCompKFactor [11]);	
    //SetCtrlVal (panel, PANEL_Serial_Window_1, temp_buff);
	WriteToTerminal(panel, temp_buff);
	
	sprintf (temp_buff, "0x%X", EepromRssiCompKFactor [11]);
	sprintf(msg,"%s,",temp_buff);
	strcat(data_string,msg);
	

	if ((Eeprom_Rssi_Comp_K_Factor >= 19) && (Eeprom_Rssi_Comp_K_Factor <= 60)){ 
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		//SetCtrlVal (panel, PANEL_Serial_Window_1, "Test Pass\n\n");
		status = 1;
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		//SetCtrlVal (panel, PANEL_Serial_Window_1, "Test Fail\n\n");	
		status = 0;
	}
	
	return status;

}



//*************************************************************
//* 
//*
//*************************************************************
int DECT_Program_RSSI_Offset_Test (int panel)
{
	int status = 0;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	int Measured_Value; 
	char temp_buff[256] = {'\0'};
	int numToRead;
	double Frequency_Offset; 
	float M_DEC, X_DEC;
	int	Eeprom_Rssi_Comp_K_Factor;
	int	Eeprom_Rssi_Comp_Offset;
	int x;
	unsigned char EepromRssiCompKFactor[13];
	unsigned char EepromRssiCompOffset[13];
	int counter = 0;
	
	sprintf(msg,"DECT Program RSSI Offset Test\n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg); 
	WriteToTerminal(panel, msg); 
	
	
	M_DEC = (float) (RSSI_45dBm - RSSI_75dBm) / 30.0;
	X_DEC = (float) ((-45) - (RSSI_45dBm/M_DEC)); 
	
	Eeprom_Rssi_Comp_K_Factor = (RoundRealToNearestInteger ((1.0/M_DEC)*16.0));
	Eeprom_Rssi_Comp_Offset = ((-1.0)*RoundRealToNearestInteger (X_DEC));
	
	
	//copy EepromRssiCompOffset_00 to EepromRssiCompOffset
	for (x = 0; x < 13; x++){
		EepromRssiCompOffset[x] = EepromRssiCompOffset_00[x];
	}

	//write the value to 11th byte first
	EepromRssiCompOffset [11] = (char) Eeprom_Rssi_Comp_Offset;
	
	//to generate correct command to write the rssi comp k factor
	//xor the last byte of the EepromRssiCompOffset_00, which is 0x91
	//with the CHANNEL_RESULTS[1][0].Eeprom_Rssi_Comp_Offset_TP501
	x = (0x91 ^ Eeprom_Rssi_Comp_Offset);

	//write the result as last the byte
	EepromRssiCompOffset [12] = (char) x; 
	
	
	while((counter < 10) && (!(status == 1))){  
		status = DECT_Send_String (panel, EepromRssiCompOffset, EepromRssiCompOffset_00_Response,returned_string,0,1,1);            
		Delay(1);
		counter++;
	}
   
	
	sprintf (temp_buff, "RSSI Offset: 0x%X \n", EepromRssiCompOffset [11]);	
    //SetCtrlVal (panel, PANEL_Serial_Window_1, temp_buff);
	WriteToTerminal(panel, temp_buff);
	
	sprintf (temp_buff, "0x%X", EepromRssiCompOffset [11]);
	sprintf(msg,"%s,",temp_buff);
	strcat(data_string,msg);
	

	if ((Eeprom_Rssi_Comp_Offset >= 101) && (Eeprom_Rssi_Comp_Offset <= 188)){ 
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		//SetCtrlVal (panel, PANEL_Serial_Window_1, "Test Pass\n\n");
		status = 1;
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		//SetCtrlVal (panel, PANEL_Serial_Window_1, "Test Fail\n\n");	
		status = 0;
	}
	
	return status;

}



//*************************************************************
//*
//*
//*************************************************************
int DECT_Measure_Receiver_Sensitivity_Test (int panel)
{
	int status = 0;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	double  Measured_Value [2] = {'\0'};
	char temp_buff[256] = {'\0'};
	int numToRead;
	double Receiver_Sensivity;
	char *temp_pointer;
	int x;


	sprintf(msg,"DECT Measure Receiver Sensitivity Test\n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg);
	WriteToTerminal(panel, msg);

	//set the modulation data to psrbs
	ibwrt (CMD60_HANDLE, "CONF:MOD:DATA PSRB", strlen("CONF:MOD:DATA PSRB"));
	if (ibsta & 0x8000) status = 0;

	//set the power to -45dBm
	
	sprintf (msg, "cable loss: %f \n", rfoffset);	
	WriteToTerminal(panel, msg);
	
		char cmd[1024] = {'\0'};
	sprintf(cmd,"CONF:BER:RF:LEV %fdBm",(-88.0 + (rfoffset)));
	ibwrt (CMD60_HANDLE, cmd, strlen(cmd));
	
	//ibwrt (CMD60_HANDLE, "CONF:BER:RF:LEV -88.0dBm", strlen("CONF:BER:RF:LEV -88.0dBm"));
	if (ibsta & 0x8000) status = 0;

	Delay (3);

	//read the response
	ibwrt (CMD60_HANDLE, "READ:BER:LTER?", strlen("READ:BER:LTER?"));
	if (ibsta & 0x8000) status = 0;

	Delay (2);

	//read the instrument for rx sensitivity
	numToRead = 26;
	ibrd (CMD60_HANDLE, temp_buff, numToRead);
	if (ibsta & 0x8000) status = 0;

	//Measured_Value = atof (temp_buff);
	
	temp_pointer = strtok (temp_buff, ",");

	x = 0;

	while (temp_pointer != NULL)
	{
		Measured_Value [x] = atof (temp_pointer);
		temp_pointer = strtok (NULL, ",");
		x = x + 1;
	}
	
	
	

	Receiver_Sensivity = (double) (Measured_Value[0]) * 1000000;

	sprintf (temp_buff, "Receiver_Sensivity: %.4fppm \n", Receiver_Sensivity);
	//SetCtrlVal (panel, PANEL_Serial_Window_1, temp_buff);
	WriteToTerminal(panel, temp_buff);

	sprintf(msg,"%f,",Receiver_Sensivity);
	strcat(data_string,msg);

	if ((Receiver_Sensivity >= 0)&&(Receiver_Sensivity < 1000))
	{
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg);
		//SetCtrlVal (panel, PANEL_Serial_Window_1, "Test Pass\n\n");
		status = 1;
	}
	else
	{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg);
		//SetCtrlVal (panel, PANEL_Serial_Window_1, "Test Fail\n\n");
		status = 0;
	}

	return status;

}

//*************************************************************
//*
//*
//*************************************************************
int DECT_Measure_RTX_Receiver_Sensitivity_Test (int panel)
{
	int status = 0;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	char temp_buff[256] = {'\0'};
	int numToRead;
	double Receiver_Sensivity;
	char *temp_pointer;
	int x;
	double  Measured_Value [2] = {'\0'};
	double dBer,dFer;


	sprintf(msg,"DECT Measure Receiver Sensitivity Test\n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg);
	WriteToTerminal(panel, msg);

	sprintf (msg, "cable loss: %f \n", rfoffset);
	WriteToTerminal(panel, msg);



	if(rtx2012_Read_ReceiverSensitivity(temp_buff,(-88.0 + (rfoffset)))!= 1)
	{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg);
		return 0;
	}

	WriteToTerminal(panel, temp_buff);
	WriteToTerminal(panel, "\n\n");


	temp_pointer = strtok (temp_buff, ",");

	x = 0;

	while (temp_pointer != NULL)
	{
		Measured_Value [x] = atof (temp_pointer);
		temp_pointer = strtok (NULL, ",");
		x = x + 1;
	}

	Receiver_Sensivity = (double) (Measured_Value[0]) * 1000000;

	sprintf (temp_buff, "Receiver_Sensivity: %.4fppm \n", Receiver_Sensivity);
	//SetCtrlVal (panel, PANEL_Serial_Window_1, temp_buff);
	WriteToTerminal(panel, temp_buff);

	sprintf(msg,"%f,",Receiver_Sensivity);
	strcat(data_string,msg);

	if ((Receiver_Sensivity >= 0)&&(Receiver_Sensivity < 1000))
	{
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg);   
		status = 1;
	}
	else
	{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg);
		status = 0;
	}




	return status;

}




//*************************************************************
//* 
//*
//*************************************************************
int DECT_Power_Template_Match_Test(int panel) 
{

	int status = 1;
	char msg[256] = {'\0'};
	char temp_buff[24] = {'\0'}; 
	int numToRead;
	int counter = 0;

	
	sprintf(msg,"DECT Power Template Match Test\n");
	WriteToTerminal(panel, msg); 
	
	
	while((counter < 10) && (!(strstr (temp_buff, "MATC")))){
   		//CMD60 modulation setting data setting of 00001111
   		ibwrt (CMD60_HANDLE, "CONF:MOD:DATA BS0F", strlen("CONF:MOD:DATA BS0F"));
   		if (ibsta & 0x8000) status = 0; 	
	
		//set the power to -45dBm
   		ibwrt (CMD60_HANDLE, "CONF:PP:RF:LEV -45.0dBm", strlen("CONF:PP:RF:LEV -45.0dBm"));
   		if (ibsta & 0x8000) status = 0;
   
   		Delay (3);

   		//read the response
   		ibwrt (CMD60_HANDLE, "STAT:DEV?", strlen("STAT:DEV?"));
   		if (ibsta & 0x8000) status = 0;
   
   		//CMD60 power template match
   		ibwrt (CMD60_HANDLE, "CALC:LIM:POW:TRAN:MATC?", strlen("CALC:LIM:POW:TRAN:MATC?"));
   		if (ibsta & 0x8000) status = 0;  
		
		
		//read the instrument for MATC
	   	numToRead = 4;
	   	ibrd (CMD60_HANDLE, temp_buff, numToRead);
	   	if (ibsta & 0x8000) status = 0;
		
		Delay(2);
		
		counter++;
	}
				
	if (strstr (temp_buff, "MATC"))
	{
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
		status = 1;
	}
	else
	{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);	 
		status = 0;
	}
		
	
	return status;
}

//*************************************************************
//*
//*
//*************************************************************
int DECT_RTX_Power_Template_Match_Test(int panel)
{

	int status = 1;
	char msg[256] = {'\0'};
	char temp_buff[24] = {'\0'};
	int numToRead;
	int counter = 0;


	sprintf(msg,"DECT Power Template Match Test\n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg);
	WriteToTerminal(panel, msg);

	if(rtx2012_Read_TemplateMatch(temp_buff) ==1 )
	{
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg);
			sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
		status = 1;
	}
	else
	{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg);
			sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
		status = 0;
	}

	return status;
}


//*************************************************************
//* 
//*
//*************************************************************
int DECT_Reset_EEPROM_Test_Flow_Test (int panel)
{

	int status = 0;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};  
	int counter = 0;

	
	sprintf(msg,"DECT Reset EEPROM Test Flow Test \n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg); 
	WriteToTerminal(panel, msg); 
	
	
	while((counter < 10) && (!(status == 1))){  
		status = DECT_Send_String (panel,ResetEepromTestflowData,ResetEepromTestflowData_Response,returned_string,0,1,1);
		Delay(1);
		counter++;
	}
	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);
	}
	
	return status;
}


//*************************************************************
//* 
//*
//*************************************************************
int DECT_File_System_Test (int panel)
{

	int status = 0;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	int counter = 0;
	
	sprintf(msg,"DECT File System Test\n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg); 
	WriteToTerminal(panel, msg); 
	
	
	while((counter < 10) && (!(status == 1))){  
		status = DECT_Send_String (panel,FileSystemFunctionality,FileSystemFunctionality_Response,returned_string,9,1,1); 
		Delay(1);
		counter++;
	}
	
	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);	
	}
	
	return status;
}


//*************************************************************
//* 
//*
//*************************************************************
int DECT_SD_Card_Test(int panel)
{

	int status = 0;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	int counter = 0;
	
	sprintf(msg,"DECT SD Card Test\n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg); 
	WriteToTerminal(panel, msg); 
	
	while((counter < 10) && (!(status == 1))){  
		status = DECT_Send_String (panel,SDInit,SDInit_Response,returned_string,0,1,1); 
		Delay(1);
		counter++;
	}
	
	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);
	}
	
	return status;
}



//*************************************************************
//* 
//*
//*************************************************************
int DECT_Make_Backup_Test(int panel)
{

	int status = 0;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	int counter = 0;

	sprintf(msg,"DECT Make Backup Test\n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg); 
	WriteToTerminal(panel, msg); 	
	
	while((counter < 10) && (!(status == 1))){  
		status = DECT_Send_String (panel,MakeBackup,MakeBackup_Response,returned_string,5,1,1); 
		Delay(1);
		counter++;
	}
	
	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);	
	}
	
	return status;
}


//*************************************************************
//* 
//*
//*************************************************************
int DECT_Power_Off_Test(int panel)
{

	int status = 0;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	int counter = 0;

	
	sprintf(msg,"DECT Power Off Test\n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg); 
	WriteToTerminal(panel, msg); 
	
	while((counter < 10) && (!(status == 1))){  
		status = DECT_Send_String (panel,PowerOff,PowerOff_Response,returned_string,0,1,1);
		Delay(1);
		counter++;
	}
	
	Delay(3);
	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);	
	}
	
	return status;
}



//*************************************************************
//* 
//*
//*************************************************************
/*int DECT_Parse_SN(int panel)
{
	char *pt; 
	int counter;
	char temp_buff[512] = {'\0'};
	
	
	memset( Serial_Global, '\0', sizeof(Serial_Global));
	memset( IPEI_Global, '\0', sizeof(IPEI_Global));
	memset( MAC_Global, '\0', sizeof(MAC_Global));
	memset( Product_Serial_Global, '\0', sizeof(Product_Serial_Global));
	
	
	
	sprintf(Serial_Global,"5543303041"); 
	sprintf(IPEI_Global,"02B2123253");
	sprintf(MAC_Global,"00304225AF17");
	sprintf(Product_Serial_Global,"2GNUC1730A07008"); 	//Mitel
	//sprintf(Product_Serial_Global,"2Y9NUC1730A07008");	//Diatonis 
	//sprintf(Product_Serial_Global,"2WDNUC1730A07008");		//Proximus 2WD, 2WE. 2WF
	

	return 1;
	//5543303041;02B2123254;00304225AF17;2GNUC1730A07008 

}

*/


/*****************************************/
/*
/*
/*****************************************/
int get_prefix_code(int panel)
{
	int status = 0;
	int i;
	
	
	memset(Product_Prefix, '\0', sizeof(Product_Prefix));     
	
	strncpy(Product_Prefix, Product_Serial_Global, 3);
	
	Product_Prefix[3] = '\0';

//	for(i = 0; i<=2; i++){
//		Product_Prefix[i] = Scan_Serial[i];
//	}

	return 1;
}


//*************************************************************
//* 
//*
//*************************************************************
int DECT_Send_String(int panel,unsigned char *string_to_send, unsigned char *expected_string, char *read_string, int check_length, int verbose, int delay)
{
	int status = 0;
	int x;
	int string_length;
	int string_length_2; 
	int input_queue_length = 0;  
	char msg[256] = {'\0'};
	char serial_input[1024] = {'\0'};
	char temp_char[12];
	
	
	//SetCtrlVal (panel, PANEL_Serial_Window_1, test_name);
	
	status = FlushInQ (COM_port);
	status = FlushOutQ (COM_port);
	
	string_length = (string_to_send[0] + 1);
	
	print_hex_memory(panel, string_to_send, string_length);
	
    for (x=0; x < string_length; x++)
    {
		status = ComWrtByte (COM_port, string_to_send[x]);
	}
	
	Delay(delay);
	
	input_queue_length = GetInQLen (COM_port);
	
    for (x = 0; x < input_queue_length; x++)
	{
    	serial_input[x] = (char) ComRdByte (COM_port);
	}

    serial_input[x] = 0;            			//Add Null to terminate string 
	
	
	
	for (x = 0; x < input_queue_length; x++)
	{
    	read_string [x] = serial_input[x];
	}

	
	if(check_length == 0){
		string_length = expected_string[0];	   	//number of bytes in message indicated in first byte
	}
	else if (check_length == -1){
	
	}
	else
	{
		string_length = check_length;
	}
	
	
	string_length_2 = expected_string[0];
	
	
	if((verbose == 1)&&(check_length != -1)){
		print_hex_memory(panel, serial_input, string_length_2);
	}
	else if((verbose == 1)&&(check_length == -1)){
		print_hex_memory(panel, read_string, input_queue_length);
	}
	
	//compare expected string with received string
	if(check_length != -1){
		for (x = 0;x < string_length;x++){
			if (serial_input[x] != (char) expected_string[x]){
				sprintf(msg,"Expected: "); 
				if(verbose == 1){
					print_hex_memory(panel, expected_string, string_length);
				}
				//SetCtrlVal (panel, PANEL_Serial_Window_1, "Test Fail\n\n");
				return 0;
			}
		}
	}
	status = 1;
	
	//SetCtrlVal (panel, PANEL_Serial_Window_1, "Test Pass\n\n");		 

	return status;

}






//*************************************************************
//* 
//*
//*************************************************************
int DECT_Read_String(int panel, int minlen,char *read_string, int * readlen,int verbose, int timeout)
{
	int status = 0;
	int x;
	int string_length; 
	int string_length_2; 
	int input_queue_length = 0;  
	char msg[256] = {'\0'};
	char serial_input[1024] = {'\0'};
	char temp_char[12];
	

	
	
	double starttime;
	GetCurrentDateTime (&starttime);
	int position = 0;
	status = 0;
	while(status != 1)
	{
		Delay(0.1);
		double endtime;
		GetCurrentDateTime (&endtime);
		
		if((endtime - starttime) >= (double)timeout)
		{
			status = 0;  
			break;
		}




		input_queue_length = GetInQLen (COM_port);	//get buffer length
		if(0 == input_queue_length)
		{
			status = 0;
			continue;
		}


		for (x = 0; x < input_queue_length; x++)	//read all buffer  data
		{
			serial_input[x+position] = (char) ComRdByte (COM_port);
		}

		serial_input[x+position] = 0;            			//Add Null to terminate string




		for (x = 0; x < input_queue_length; x++)	//copy data to readstring
		{
			read_string [x+position] = serial_input[x+position];
		}
		
		
		position += input_queue_length;
		
		*readlen =    position;
		
		
		

		
	
		
		
		if(position >= minlen)
			status = 1;
		
	}
		print_hex_memory(panel, read_string, position);               


	return status;

}




//*************************************************************
//* 
//*
//*************************************************************
int DECT_Send_String_USB(int panel,unsigned char *string_to_send, unsigned char *expected_string, char *read_string, int check_length, int verbose, int delay)
{
	int status = 0;
	int x;
	int string_length;
	int input_queue_length = 0;  
	char msg[256] = {'\0'};
	char serial_input[1024] = {'\0'};
	char temp_char[12];
	
	
	//SetCtrlVal (panel, PANEL_Serial_Window_1, test_name);
	
	status = FlushInQ (COM_port_2);
	status = FlushOutQ (COM_port_2);
	
	string_length = (string_to_send[0] + 1);
	
	print_hex_memory(panel, string_to_send, string_length);
	
    for (x=0; x < string_length; x++)
    {
		status = ComWrtByte (COM_port_2, string_to_send[x]);
	}
	
	Delay(delay);
	
	input_queue_length = GetInQLen (COM_port_2);
	
    for (x = 0; x < input_queue_length; x++)
	{
    	serial_input[x] = (char) ComRdByte (COM_port_2);
	}

    serial_input[x] = 0;            			//Add Null to terminate string 
	
	
	
	for (x = 0; x < input_queue_length; x++)
	{
    	read_string [x] = serial_input[x];
	}

	
	if(check_length == 0){
		string_length = expected_string[0];	   	//number of bytes in message indicated in first byte
	}
	else
	{
		string_length = check_length;
	}
	
	if(verbose == 1){
		print_hex_memory(panel, serial_input, string_length);
	}
	
	//compare expected string with received string
	for (x = 0;x < string_length;x++){
		if (serial_input[x] != (char) expected_string[x]){
			sprintf(msg,"Expected: "); 
			if(verbose == 1){
				print_hex_memory(panel, expected_string, string_length);
			}
			//SetCtrlVal (panel, PANEL_Serial_Window_1, "Test Fail\n\n");
			return 0;
		}
	}

	status = 1;
	
	//SetCtrlVal (panel, PANEL_Serial_Window_1, "Test Pass\n\n");		 

	return status;

}




//*************************************************************
//* 
//*This is small change with DECT_Send_String_USB, print logs while not check return value
//*************************************************************
int DECT_Send_String_USB_V2(int panel,unsigned char *string_to_send, unsigned char *expected_string, char *read_string, int check_length, int verbose, int delay)
{
	int status = 0;
	int x;
	int string_length;
	int string_length_2; 
	int input_queue_length = 0;  
	char msg[256] = {'\0'};
	char serial_input[1024] = {'\0'};
	char temp_char[12];
	
	
	//SetCtrlVal (panel, PANEL_Serial_Window_1, test_name);
	
	status = FlushInQ (COM_port_2);
	status = FlushOutQ (COM_port_2);
	
	string_length = (string_to_send[0] + 1);
	
	print_hex_memory(panel, string_to_send, string_length);
	
    for (x=0; x < string_length; x++)
    {
		status = ComWrtByte (COM_port_2, string_to_send[x]);
	}
	
	Delay(delay);
	
	input_queue_length = GetInQLen (COM_port_2);
	
    for (x = 0; x < input_queue_length; x++)
	{
    	serial_input[x] = (char) ComRdByte (COM_port_2);
	}

    serial_input[x] = 0;            			//Add Null to terminate string 
	
	
	
	for (x = 0; x < input_queue_length; x++)
	{
    	read_string [x] = serial_input[x];
	}

	
	if(check_length == 0){
		string_length = expected_string[0];	   	//number of bytes in message indicated in first byte
	}
	else if (check_length == -1){
	
	}
	else
	{
		string_length = check_length;
	}
	
	
	string_length_2 = expected_string[0];
	
	
	if((verbose == 1)&&(check_length != -1)){
		print_hex_memory(panel, serial_input, string_length_2);
	}
	else if((verbose == 1)&&(check_length == -1)){
		print_hex_memory(panel, read_string, input_queue_length);
	}
	
	//compare expected string with received string
	if(check_length != -1){
		for (x = 0;x < string_length;x++){
			if (serial_input[x] != (char) expected_string[x]){
				sprintf(msg,"Expected: "); 
				if(verbose == 1){
					print_hex_memory(panel, expected_string, string_length);
				}
				//SetCtrlVal (panel, PANEL_Serial_Window_1, "Test Fail\n\n");
				return 0;
			}
		}
	}
	status = 1;
	
	//SetCtrlVal (panel, PANEL_Serial_Window_1, "Test Pass\n\n");		 

	return status;

}



//*************************************************************
//* 
//*
//*************************************************************
int DECT_Read_Error_Counter_Test(int panel) 
{
	int status = 0;
	char temp_buff[256] = {'\0'};
	int numToRead = 0;
	//double Measured_Frequency;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	int counter = 0;
	
	
	sprintf(msg,"DECT Read Error Counter Test\n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg); 
	WriteToTerminal(panel, msg); 
	
	while((counter < 10) && (!(status == 1))){  
		status = DECT_Send_String (panel,ReadErrorCounter,ReadErrorCounter_Response,returned_string,5,1,1);  
		Delay(1);
		counter++;
	}
	
	sprintf (temp_buff, "Cumulative Error Counter: %x \n", returned_string[8]);	
    //SetCtrlVal (panel, PANEL_Serial_Window_1, temp_buff); 
	WriteToTerminal(panel, temp_buff); 
	
	
	Error_Counter = returned_string[8];
	
	
	sprintf(msg,"%i,",Error_Counter);
	strcat(data_string,msg);
	
	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		//SetCtrlVal (panel, PANEL_Serial_Window_1, "Test Pass\n\n");
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		//SetCtrlVal (panel, PANEL_Serial_Window_1, "Test Fail\n\n");	
	}


	return status;
}



//*************************************************************
//* 
//*
//*************************************************************
int DECT_MAN_DOWN_SENSOR_Test(int panel) 
{
	int status = 0;
	char temp_buff[256] = {'\0'};
	int numToRead = 0;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	int counter = 0;
	
	
	sprintf(msg,"DECT Man Down Sensor Test\n");
	WriteToTerminal(panel, msg); 
	

	if(imandownsensor == 0)
	{
		while((counter < 10) && (!(status == 1)))
		{
			status = DECT_Send_String (panel,TestTMS,TestTMS_Response,returned_string,6,1,1);
			Delay(1);
			counter++;
		}
	}
	else
	{
			while((counter < 10) && (!(status == 1)))
		{
			status = DECT_Send_String (panel,TestTMS_V2,TestTMS_Response_V2,returned_string,6,1,1);
			Delay(1);
			counter++;
		}
	}
	

	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);	
	}


	return status;
}



//*************************************************************
//* 
//*
//*************************************************************
int DECT_Master_Reset_Test(int panel) 
{
	int status = 0;
	char temp_buff[256] = {'\0'};
	int numToRead = 0;
	double Measured_Frequency;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	int counter = 0;  
	
	
	sprintf(msg,"DECT Master Reset Test\n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg); 
	WriteToTerminal(panel, msg); 
	
	while((counter < 10) && (!(status == 1))){ 
		status = DECT_Send_String (panel,MasterReset,MasterReset_Response,returned_string,0,1,1); 
		Delay(1);
		counter++;
	}	
	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		//SetCtrlVal (panel, PANEL_Serial_Window_1, "Test Fail\n\n");	
	}


	return status;
}



//*************************************************************
//* 
//*
//*************************************************************
int DECT_Master_Reset_USB_Test(int panel) 
{
	int status = 0;
	char temp_buff[256] = {'\0'};
	int numToRead = 0;
	double Measured_Frequency;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	int counter = 0;  
	
	sprintf(msg,"DECT Master Reset USB Test\n");
	WriteToTerminal(panel, msg); 
	
	while((counter < 10) && (!(status == 1))){  
		status = DECT_Send_String_USB (panel,MasterReset,MasterReset_Response,returned_string,0,1,1); 
		Delay(1);
		counter++;
	}

	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);	
	}


	return status;
}


//*************************************************************
//* 
//*
//*************************************************************
int DECT_Write_IPEI_Test(int panel) 
{

	unsigned char EepromIpei[17] = {'\0'}; 
	char nible1;
	char nible2; 
	int temp_int, temp_int_2;
	int status = 0;
	char msg[256];
	char temp_char[24] = {'\0'};
	char temp_char_2[24] = {'\0'};
	int x;
	char returned_string[256];
	int counter = 0; 
	
	sprintf(msg,"DECT Write IPEI Test\n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg); 
	WriteToTerminal(panel, msg); 
	
	
	//load default array
	for (x=0; x < 17; x++)
    {
        EepromIpei[x] = WriteEepromIpei[x];
    }		 
	
	//1st byte 
	nible1 = IPEI_Global [0];
	if (nible1 > '9') nible1 += 9;
    nible1 &= 0x0F;

 	nible2 = IPEI_Global [1];
	if (nible2 > '9') nible2 += 9;
    nible2 &= 0x0F;
	
	EepromIpei[11] =  ((nible1 * 16) +  nible2);
	
		 
	//2nd byte 
	nible1 = IPEI_Global [2];
	if (nible1 > '9') nible1 += 9;
    nible1 &= 0x0F;

 	nible2 = IPEI_Global [3];
	if (nible2 > '9') nible2 += 9;
    nible2 &= 0x0F;
	
	EepromIpei[12] =  ((nible1 * 16) +  nible2);	
	
	//3rd byte 
	nible1 = IPEI_Global [4];
	if (nible1 > '9') nible1 += 9;
    nible1 &= 0x0F;

 	nible2 = IPEI_Global [5];
	if (nible2 > '9') nible2 += 9;
    nible2 &= 0x0F;
	
	EepromIpei[13] =  ((nible1 * 16) +  nible2);
	
	//4th byte 
	nible1 = IPEI_Global [6];
	if (nible1 > '9') nible1 += 9;
    nible1 &= 0x0F;

 	nible2 = IPEI_Global [7];
	if (nible2 > '9') nible2 += 9;
    nible2 &= 0x0F;
	
	EepromIpei[14] =  ((nible1 * 16) +  nible2);	
	
	//5th byte 
	nible1 = IPEI_Global [8];
	if (nible1 > '9') nible1 += 9;
    nible1 &= 0x0F;

 	nible2 = IPEI_Global [9];
	if (nible2 > '9') nible2 += 9;
    nible2 &= 0x0F;
	
	EepromIpei[15] =  ((nible1 * 16) +  nible2);	
	
	if((IPEI_Global [0] == 49) && (IPEI_Global [1] == 49) && (IPEI_Global [2] == 48) && (IPEI_Global [3] == 52)){
		sprintf(temp_char,"0%c%c%c%c%c", IPEI_Global [0],IPEI_Global [1],IPEI_Global [2],IPEI_Global [3],IPEI_Global [4]);
		sprintf(temp_char_2,"%c%c%c%c%c%c%c", IPEI_Global [5],IPEI_Global [6],IPEI_Global [7],IPEI_Global [8],IPEI_Global [9],IPEI_Global [10],IPEI_Global [11]);

		temp_int = atoi(temp_char); 
		temp_int_2 = atoi(temp_char_2); 
	
		EepromIpei[11] = (char)((temp_int >> 12) & 0x000000FF);  
		EepromIpei[12] = (char)((temp_int >>  4) & 0x000000FF);
		EepromIpei[13] = (char)(((temp_int << 4) & 0x000000F0)|((temp_int_2 >> 16) & 0x0000000F));
		EepromIpei[14] = (char)((temp_int_2 >> 8) & 0x000000FF);
		EepromIpei[15] = (char)((temp_int_2) & 0x000000FF); 
	}
	
	//populate Global array for checking later
	//for(int i = 0;i < 5; i ++){
	//	EepromIpei_Global[i] = EepromIpei[i + 11]; 	
	//}
	
	//xor bytes 16, 16, 15, 14, 13, 12, and 11 to generate complete command to write the ipei
	EepromIpei[16] =  EepromIpei[16] ^ EepromIpei[15] ^ EepromIpei[14] ^ EepromIpei[13] ^ EepromIpei[12] ^ EepromIpei[11];
	
	while((counter < 10) && (!(status == 1))){  
		status = DECT_Send_String (panel,EepromIpei,WriteEepromIpei_Response,returned_string,0,1,1); 
		Delay(1);
		counter++;
	}
	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);	
	}
	
	return status;
	

}


//*************************************************************
//* 
//*
//*************************************************************
int DECT_Write_IPEI_USB_Test(int panel) 
{

	unsigned char EepromIpei[17] = {'\0'}; 
	char nible1;
	char nible2; 
	int temp_int, temp_int_2;
	int status = 0;
	char msg[256];
	char temp_char[24] = {'\0'};
	char temp_char_2[24] = {'\0'};
	int x;
	char returned_string[256];

	
	sprintf(msg,"DECT Write IPEI Test\n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg); 
	WriteToTerminal(panel, msg); 
	
	
	//load default array
	for (x=0; x < 17; x++)
    {
        EepromIpei[x] = WriteEepromIpei[x];
    }		 
	
	//1st byte 
	nible1 = IPEI_Global [0];
	if (nible1 > '9') nible1 += 9;
    nible1 &= 0x0F;

 	nible2 = IPEI_Global [1];
	if (nible2 > '9') nible2 += 9;
    nible2 &= 0x0F;
	
	EepromIpei[11] =  ((nible1 * 16) +  nible2);
	
		 
	//2nd byte 
	nible1 = IPEI_Global [2];
	if (nible1 > '9') nible1 += 9;
    nible1 &= 0x0F;

 	nible2 = IPEI_Global [3];
	if (nible2 > '9') nible2 += 9;
    nible2 &= 0x0F;
	
	EepromIpei[12] =  ((nible1 * 16) +  nible2);	
	
	//3rd byte 
	nible1 = IPEI_Global [4];
	if (nible1 > '9') nible1 += 9;
    nible1 &= 0x0F;

 	nible2 = IPEI_Global [5];
	if (nible2 > '9') nible2 += 9;
    nible2 &= 0x0F;
	
	EepromIpei[13] =  ((nible1 * 16) +  nible2);
	
	//4th byte 
	nible1 = IPEI_Global [6];
	if (nible1 > '9') nible1 += 9;
    nible1 &= 0x0F;

 	nible2 = IPEI_Global [7];
	if (nible2 > '9') nible2 += 9;
    nible2 &= 0x0F;
	
	EepromIpei[14] =  ((nible1 * 16) +  nible2);	
	
	//5th byte 
	nible1 = IPEI_Global [8];
	if (nible1 > '9') nible1 += 9;
    nible1 &= 0x0F;

 	nible2 = IPEI_Global [9];
	if (nible2 > '9') nible2 += 9;
    nible2 &= 0x0F;
	
	EepromIpei[15] =  ((nible1 * 16) +  nible2);	
	
	if((IPEI_Global [0] == 49) && (IPEI_Global [1] == 49) && (IPEI_Global [2] == 48) && (IPEI_Global [3] == 52)){
		sprintf(temp_char,"0%c%c%c%c%c", IPEI_Global [0],IPEI_Global [1],IPEI_Global [2],IPEI_Global [3],IPEI_Global [4]);
		sprintf(temp_char_2,"%c%c%c%c%c%c%c", IPEI_Global [5],IPEI_Global [6],IPEI_Global [7],IPEI_Global [8],IPEI_Global [9],IPEI_Global [10],IPEI_Global [11]);

		temp_int = atoi(temp_char); 
		temp_int_2 = atoi(temp_char_2); 
	
		EepromIpei[11] = (char)((temp_int >> 12) & 0x000000FF);  
		EepromIpei[12] = (char)((temp_int >>  4) & 0x000000FF);
		EepromIpei[13] = (char)(((temp_int << 4) & 0x000000F0)|((temp_int_2 >> 16) & 0x0000000F));
		EepromIpei[14] = (char)((temp_int_2 >> 8) & 0x000000FF);
		EepromIpei[15] = (char)((temp_int_2) & 0x000000FF); 
	}
	
	//populate Global array for checking later
	//for(int i = 0;i < 5; i ++){
	//	EepromIpei_Global[i] = EepromIpei[i + 11]; 	
	//}
	
	//xor bytes 16, 16, 15, 14, 13, 12, and 11 to generate complete command to write the ipei
	EepromIpei[16] =  EepromIpei[16] ^ EepromIpei[15] ^ EepromIpei[14] ^ EepromIpei[13] ^ EepromIpei[12] ^ EepromIpei[11];
	
	
	status = DECT_Send_String (panel,EepromIpei,WriteEepromIpei_Response,returned_string,0,1,1); 
	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		//SetCtrlVal (panel, PANEL_Serial_Window_1, "Test Pass\n\n");
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		//SetCtrlVal (panel, PANEL_Serial_Window_1, "Test Fail\n\n");	
	}
	
	return status;
	

}


//*************************************************************
//* 
//*
//*************************************************************
int DECT_Write_Serial_Number_Test(int panel) 
{
	unsigned char EepromSerialNumber[17] = {'\0'}; 
	char nible1;
	char nible2; 
	int temp_int, temp_int_2;
	int status = 0;
	char msg[256];
	char temp_char[24] = {'\0'};
	char temp_char_2[24] = {'\0'};
	int x;
	char returned_string[256] = {'\0'};
	int counter = 0;
	
	
	sprintf(msg,"DECT Write Serial Number Test\n"); 
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg); 
	WriteToTerminal(panel, msg); 
	
	for (x=0; x < 17; x++)
    {
    	EepromSerialNumber[x] = WriteEepromSerialNumber[x];
    }
	
	//1st byte 
	nible1 = Serial_Global [0];
	if (nible1 > '9') nible1 += 9;
    nible1 &= 0x0F;

 	nible2 = Serial_Global [1];
	if (nible2 > '9') nible2 += 9;
    nible2 &= 0x0F;
	
	EepromSerialNumber[11] =  ((nible1 * 16) +  nible2);
	
		 
	//2nd byte 
	nible1 = Serial_Global [2];
	if (nible1 > '9') nible1 += 9;
    nible1 &= 0x0F;

 	nible2 = Serial_Global [3];
	if (nible2 > '9') nible2 += 9;
    nible2 &= 0x0F;
	
	EepromSerialNumber[12] =  ((nible1 * 16) +  nible2);	
	
	//3rd byte 
	nible1 = Serial_Global [4];
	if (nible1 > '9') nible1 += 9;
    nible1 &= 0x0F;

 	nible2 = Serial_Global [5];
	if (nible2 > '9') nible2 += 9;
    nible2 &= 0x0F;
	
	EepromSerialNumber[13] =  ((nible1 * 16) +  nible2);
	
	//4th byte 
	nible1 = Serial_Global [6];
	if (nible1 > '9') nible1 += 9;
    nible1 &= 0x0F;

 	nible2 = Serial_Global [7];
	if (nible2 > '9') nible2 += 9;
    nible2 &= 0x0F;
	
	EepromSerialNumber[14] =  ((nible1 * 16) +  nible2);	
	
	//5th byte 
	nible1 = Serial_Global [8];
	if (nible1 > '9') nible1 += 9;
    nible1 &= 0x0F;

 	nible2 = Serial_Global [9];
	if (nible2 > '9') nible2 += 9;
    nible2 &= 0x0F;
	
	EepromSerialNumber[15] =  ((nible1 * 16) +  nible2);	
	
	
	//xor bytes 16, 16, 15, 14, 13, 12, and 11 to generate complete command to write the serial number
	EepromSerialNumber[16] =  EepromSerialNumber[16] ^ EepromSerialNumber[15] ^ EepromSerialNumber[14] ^ EepromSerialNumber[13] ^ EepromSerialNumber[12] ^ EepromSerialNumber[11];
	
	
	while((counter < 10) && (!(status == 1))){  
		status = DECT_Send_String (panel,EepromSerialNumber,WriteEepromSerialNumber_Response,returned_string,0,1,1); 
		Delay(1);
		counter++;
	}
	
	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);	
	}
	
	return status;
}



//*************************************************************
//* 
//*
//*************************************************************
int DECT_Verify_MAC_Address_Test (int panel)
{
	
	int status = 0;
	char temp_buff[256] = {'\0'}; 
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	int counter = 0;
	int i = 0;
	int n = 0;
	char local_MAC[24] = {'\0'};
	
	sprintf(msg,"DECT Verify MAC Address Test\n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg); 
	WriteToTerminal(panel, msg); 
	
	
	while((counter < 10) && (!(status == 1))){  
		status = DECT_Send_String (panel,ReadEepromMacAddress,ReadEepromMacAddress_Response,returned_string,10,1,1); 
		Delay(1);
		counter++;
	}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	//there are 6 bytes in the mac address
	//8 bytes after the data length (08 00) is the mac address data
	//the last byte of the data length is at location 10th of the returned_string
	//based on the table on the test specs:
	//byte 18th of the returned_string [18] -> the 6th byte of the MAC address
	
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	//returned bytes are containing 2 digits per byte. need to expand each nible (digit) of the byte to a byte.
	//to do so, the high nible is anded with 0xf0 and shifted to the right.
	//to trnsform this to ascii charater, add 0x30 to it. The low nible is anded with 0x0f. 
	//there is no need to shift it, just add 0x30 to transform it to ascii. 
	
	//MAC_Global_2 [0][0] = (((returned_string [18] & 0xf0) >> 4) + 0x30);
	
	//1st digit, if greater than 9 then it is a hex number and 0x41 has to be added
	MAC_Global_2 [0] = ((returned_string [18] & 0xf0) >> 4);
	if (MAC_Global_2 [0] > 9) (MAC_Global_2 [0] = MAC_Global_2 [0] + 0x41 - 0x0A);
	else (MAC_Global_2 [0] = MAC_Global_2 [0] + 0x30);
	
	//2nd digit
	MAC_Global_2 [1] = (returned_string [18] & 0x0f);
	if (MAC_Global_2 [1] > 9) (MAC_Global_2 [1] = MAC_Global_2 [1] + 0x41 - 0x0A);
	else (MAC_Global_2 [1] = MAC_Global_2 [1] + 0x30);
	
	//3rd digit
	MAC_Global_2 [2] = ((returned_string [17] & 0xf0) >> 4);
	if (MAC_Global_2 [2] > 9) (MAC_Global_2 [2] = MAC_Global_2 [2] + 0x41 - 0x0A);
	else (MAC_Global_2 [2] = MAC_Global_2 [2] + 0x30);
	
	//4th digit
	MAC_Global_2 [3] = (returned_string [17] & 0x0f);
	if (MAC_Global_2 [3] > 9) (MAC_Global_2 [3] = MAC_Global_2 [3] + 0x41 - 0x0A);
	else (MAC_Global_2 [3] = MAC_Global_2 [3] + 0x30);
	
	//5th digit
	MAC_Global_2 [4] = ((returned_string [15] & 0xf0) >> 4);
	if (MAC_Global_2 [4] > 9) (MAC_Global_2 [4] = MAC_Global_2 [4] + 0x41 - 0x0A);
	else (MAC_Global_2 [4] = MAC_Global_2 [4] + 0x30);	
	
	//6th digit
	MAC_Global_2 [5] = (returned_string [15] & 0x0f);
	if (MAC_Global_2 [5] > 9) (MAC_Global_2 [5] = MAC_Global_2 [5] + 0x41 - 0x0A);
	else (MAC_Global_2 [5] = MAC_Global_2 [5] + 0x30);	
	
	//7th digit
	MAC_Global_2 [6] = ((returned_string [11] & 0xf0) >> 4);
	if (MAC_Global_2 [6] > 9) (MAC_Global_2 [6] = MAC_Global_2 [6] + 0x41 - 0x0A);
	else (MAC_Global_2 [6] = MAC_Global_2 [6] + 0x30);	
	
	//8th digit
	MAC_Global_2 [7] = (returned_string [11] & 0x0f);
	if (MAC_Global_2 [7] > 9) (MAC_Global_2 [7] = MAC_Global_2 [7] + 0x41 - 0x0A);
	else (MAC_Global_2 [7] = MAC_Global_2 [7] + 0x30);	
	
	//9th digit
	MAC_Global_2 [8] = ((returned_string [14] & 0xf0) >> 4);
	if (MAC_Global_2 [8] > 9) (MAC_Global_2 [8] = MAC_Global_2 [8] + 0x41 - 0x0A);
	else (MAC_Global_2 [8] = MAC_Global_2 [8] + 0x30);	
	
	//10th digit
	MAC_Global_2 [9] = (returned_string [14] & 0x0f);
	if (MAC_Global_2 [9] > 9) (MAC_Global_2 [9] = MAC_Global_2 [9] + 0x41 - 0x0A);
	else (MAC_Global_2 [9] = MAC_Global_2 [9] + 0x30);	
	
	//11th digit
	MAC_Global_2 [10] = ((returned_string [13] & 0xf0) >> 4);
	if (MAC_Global_2 [10] > 9) (MAC_Global_2 [10] = MAC_Global_2 [10] + 0x41 - 0x0A);
	else (MAC_Global_2 [10] = MAC_Global_2 [10] + 0x30);	
	
	//12th digit
	MAC_Global_2 [11] = (returned_string [13] & 0x0f);
	if (MAC_Global_2 [11] > 9) (MAC_Global_2 [11] = MAC_Global_2 [11] + 0x41 - 0x0A);
	else (MAC_Global_2 [11] = MAC_Global_2 [11] + 0x30);	
	
	
	//print_hex_memory(panel, MAC_Global_2, 12); 
	
	
	sprintf(msg,"MAC Address Read: %c%c%c%c%c%c%c%c%c%c%c%c\n", MAC_Global_2 [0],MAC_Global_2 [1],MAC_Global_2 [2],MAC_Global_2 [3],MAC_Global_2 [4],MAC_Global_2 [5],MAC_Global_2 [6],MAC_Global_2 [7],MAC_Global_2 [8],MAC_Global_2 [9],MAC_Global_2 [10],MAC_Global_2 [11]);
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg);
	WriteToTerminal(panel, msg); 
	
/////////////////////////////////////////////////	
	
	//n = 0;
	
	if(long_form_final == 1){ 
	
		for (i = 0;i <= 12; i++){
			local_MAC[i] = MAC_Global[i];
		}
	
		if(long_form_final == 1){ 
			status = 1;

			for(i = 0;i <= 12; i++){
				if((local_MAC[i] != MAC_Global_2[i])){
					status = 0;
				}
	
			}
		
		sprintf(temp_buff,"MAC Address Expected: %c%c%c%c%c%c%c%c%c%c%c%c\n", MAC_Global[0],MAC_Global[1],MAC_Global[2],MAC_Global[3],MAC_Global[4],MAC_Global[5],MAC_Global[6],MAC_Global[7],MAC_Global[8],MAC_Global[9],MAC_Global[10],MAC_Global[11]);
		WriteToTerminal(panel, temp_buff);
	
		}
	
	}
	
	
////////////////////////////////////////	
	sprintf(msg,"%s,",MAC_Global_2);
	strcat(data_string,msg);
	
 
		
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);	
	}
	
	return status;

}




//*************************************************************
//* 
//*
//*************************************************************
int DECT_Read_Device_MAC_Address_Test (int panel)
{
	
	int status = 0;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	int counter = 0;
	
	sprintf(msg,"DECT Read Device MAC Address Test\n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg); 
	WriteToTerminal(panel, msg); 
	
	
	while((counter < 10) && (!(status == 1))){  
		status = DECT_Send_String (panel,ReadEepromMacAddress,ReadEepromMacAddress_Response,returned_string,10,1,1); 
		Delay(1);
		counter++;
	}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	//there are 6 bytes in the mac address
	//8 bytes after the data length (08 00) is the mac address data
	//the last byte of the data length is at location 10th of the returned_string
	//based on the table on the test specs:
	//byte 18th of the returned_string [18] -> the 6th byte of the MAC address
	
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	//returned bytes are containing 2 digits per byte. need to expand each nible (digit) of the byte to a byte.
	//to do so, the high nible is anded with 0xf0 and shifted to the right.
	//to trnsform this to ascii charater, add 0x30 to it. The low nible is anded with 0x0f. 
	//there is no need to shift it, just add 0x30 to transform it to ascii. 
	
	//MAC_Global_2 [0][0] = (((returned_string [18] & 0xf0) >> 4) + 0x30);
	
	//1st digit, if greater than 9 then it is a hex number and 0x41 has to be added
	MAC_Global_2 [0] = ((returned_string [18] & 0xf0) >> 4);
	if (MAC_Global_2 [0] > 9) (MAC_Global_2 [0] = MAC_Global_2 [0] + 0x41 - 0x0A);
	else (MAC_Global_2 [0] = MAC_Global_2 [0] + 0x30);
	
	//2nd digit
	MAC_Global_2 [1] = (returned_string [18] & 0x0f);
	if (MAC_Global_2 [1] > 9) (MAC_Global_2 [1] = MAC_Global_2 [1] + 0x41 - 0x0A);
	else (MAC_Global_2 [1] = MAC_Global_2 [1] + 0x30);
	
	//3rd digit
	MAC_Global_2 [2] = ((returned_string [17] & 0xf0) >> 4);
	if (MAC_Global_2 [2] > 9) (MAC_Global_2 [2] = MAC_Global_2 [2] + 0x41 - 0x0A);
	else (MAC_Global_2 [2] = MAC_Global_2 [2] + 0x30);
	
	//4th digit
	MAC_Global_2 [3] = (returned_string [17] & 0x0f);
	if (MAC_Global_2 [3] > 9) (MAC_Global_2 [3] = MAC_Global_2 [3] + 0x41 - 0x0A);
	else (MAC_Global_2 [3] = MAC_Global_2 [3] + 0x30);
	
	//5th digit
	MAC_Global_2 [4] = ((returned_string [15] & 0xf0) >> 4);
	if (MAC_Global_2 [4] > 9) (MAC_Global_2 [4] = MAC_Global_2 [4] + 0x41 - 0x0A);
	else (MAC_Global_2 [4] = MAC_Global_2 [4] + 0x30);	
	
	//6th digit
	MAC_Global_2 [5] = (returned_string [15] & 0x0f);
	if (MAC_Global_2 [5] > 9) (MAC_Global_2 [5] = MAC_Global_2 [5] + 0x41 - 0x0A);
	else (MAC_Global_2 [5] = MAC_Global_2 [5] + 0x30);	
	
	//7th digit
	MAC_Global_2 [6] = ((returned_string [11] & 0xf0) >> 4);
	if (MAC_Global_2 [6] > 9) (MAC_Global_2 [6] = MAC_Global_2 [6] + 0x41 - 0x0A);
	else (MAC_Global_2 [6] = MAC_Global_2 [6] + 0x30);	
	
	//8th digit
	MAC_Global_2 [7] = (returned_string [11] & 0x0f);
	if (MAC_Global_2 [7] > 9) (MAC_Global_2 [7] = MAC_Global_2 [7] + 0x41 - 0x0A);
	else (MAC_Global_2 [7] = MAC_Global_2 [7] + 0x30);	
	
	//9th digit
	MAC_Global_2 [8] = ((returned_string [14] & 0xf0) >> 4);
	if (MAC_Global_2 [8] > 9) (MAC_Global_2 [8] = MAC_Global_2 [8] + 0x41 - 0x0A);
	else (MAC_Global_2 [8] = MAC_Global_2 [8] + 0x30);	
	
	//10th digit
	MAC_Global_2 [9] = (returned_string [14] & 0x0f);
	if (MAC_Global_2 [9] > 9) (MAC_Global_2 [9] = MAC_Global_2 [9] + 0x41 - 0x0A);
	else (MAC_Global_2 [9] = MAC_Global_2 [9] + 0x30);	
	
	//11th digit
	MAC_Global_2 [10] = ((returned_string [13] & 0xf0) >> 4);
	if (MAC_Global_2 [10] > 9) (MAC_Global_2 [10] = MAC_Global_2 [10] + 0x41 - 0x0A);
	else (MAC_Global_2 [10] = MAC_Global_2 [10] + 0x30);	
	
	//12th digit
	MAC_Global_2 [11] = (returned_string [13] & 0x0f);
	if (MAC_Global_2 [11] > 9) (MAC_Global_2 [11] = MAC_Global_2 [11] + 0x41 - 0x0A);
	else (MAC_Global_2 [11] = MAC_Global_2 [11] + 0x30);	
	
	
	//print_hex_memory(panel, MAC_Global_2, 12); 
	
	
	sprintf(msg,"MAC Address: %c%c%c%c%c%c%c%c%c%c%c%c\n", MAC_Global_2 [0],MAC_Global_2 [1],MAC_Global_2 [2],MAC_Global_2 [3],MAC_Global_2 [4],MAC_Global_2 [5],MAC_Global_2 [6],MAC_Global_2 [7],MAC_Global_2 [8],MAC_Global_2 [9],MAC_Global_2 [10],MAC_Global_2 [11]);
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg);
	WriteToTerminal(panel, msg); 
 
		
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);	
	}
	
	return status;

}


//*************************************************************
//* 
//*
//*************************************************************
int DECT_Write_MAC_Address_USB_Test(int panel) 
{
	
	unsigned char SetBDAddr[20] = {'\0'}; 
	char nible1;
	char nible2; 
	int temp_int, temp_int_2;
	int status = 0;
	char msg[256];
	char temp_char[24] = {'\0'};
	char temp_char_2[24] = {'\0'};
	int x;
	char returned_string[256] = {'\0'};
	
	
	sprintf(msg,"DECT Write MAC Address USB Test\n"); 
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg); 
	WriteToTerminal(panel, msg); 
	
	
	for (x=0; x < 20; x++)
        {
        SetBDAddr[x] = BtSetBDAddr[x];
    }
	
	//1st byte 
	nible1 = MAC_Global [0];
	if (nible1 > '9') nible1 += 9;
    nible1 &= 0x0F;

 	nible2 = MAC_Global [1];
	if (nible2 > '9') nible2 += 9;
    nible2 &= 0x0F;
	
	SetBDAddr[18] =  ((nible1 * 16) +  nible2);
	
		 
	//2nd byte 
	nible1 = MAC_Global [2];
	if (nible1 > '9') nible1 += 9;
    nible1 &= 0x0F;

 	nible2 = MAC_Global [3];
	if (nible2 > '9') nible2 += 9;
    nible2 &= 0x0F;
	
	SetBDAddr[17] =  ((nible1 * 16) +  nible2);	
	
	//3rd byte 
	nible1 = MAC_Global [4];
	if (nible1 > '9') nible1 += 9;
    nible1 &= 0x0F;

 	nible2 = MAC_Global [5];
	if (nible2 > '9') nible2 += 9;
    nible2 &= 0x0F;
	
	SetBDAddr[15] =  ((nible1 * 16) +  nible2);
	
	//4th byte 
	nible1 = MAC_Global [6];
	if (nible1 > '9') nible1 += 9;
    nible1 &= 0x0F;

 	nible2 = MAC_Global [7];
	if (nible2 > '9') nible2 += 9;
    nible2 &= 0x0F;
	
	SetBDAddr[11] =  ((nible1 * 16) +  nible2);	
	
	//5th byte 
	nible1 = MAC_Global [8];
	if (nible1 > '9') nible1 += 9;
    nible1 &= 0x0F;

 	nible2 = MAC_Global [9];
	if (nible2 > '9') nible2 += 9;
    nible2 &= 0x0F;
	
	SetBDAddr[14] =  ((nible1 * 16) +  nible2);	
	
	//6th byte 
	nible1 = MAC_Global [10];
	if (nible1 > '9') nible1 += 9;
    nible1 &= 0x0F;

 	nible2 = MAC_Global [11];
	if (nible2 > '9') nible2 += 9;
    nible2 &= 0x0F;
	
	SetBDAddr[13] =  ((nible1 * 16) +  nible2);	
	
	//xor bytes 19, 18, 17, 15, 14, 13, and 11 to generate complete command to write the mac address
	SetBDAddr[19] =  SetBDAddr[19] ^ SetBDAddr[18] ^ SetBDAddr[17] ^ SetBDAddr[15] ^ SetBDAddr[14] ^ SetBDAddr[13] ^ SetBDAddr[11];
	
	status = DECT_Send_String_USB (panel,SetBDAddr,BtSetBDAddr_Response,returned_string,0,1,1); 
	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);	
	}
	
	return status;


}



//*************************************************************
//* 
//*
//*************************************************************
int DECT_Branding_Check_Test(int panel) 
{
	
	int status = 0;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	int counter = 0; 

	
	sprintf(msg,"DECT Branding Check Test\n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg); 
	WriteToTerminal(panel, msg); 
	
	
	if(((Product_Serial_Global[0] == 50 )&&(Product_Serial_Global[1] == 87)&&(Product_Serial_Global[2] == 68))||((Product_Serial_Global[0] == 50 )&&(Product_Serial_Global[1] == 87)&&(Product_Serial_Global[2] == 69))||((Product_Serial_Global[0] == 50 )&&(Product_Serial_Global[1] == 87)&&(Product_Serial_Global[2] == 70))){	
		 while((counter < 15) && (!(status == 1))){ 
			status = DECT_Send_String (panel,BrandingVersionCheckProximus,BrandingVersionCheckProximus_Response,returned_string,0,1,2);  
			Delay(1);
			counter++;
		}	
	
	}
	else if (((Product_Serial_Global[0] == 50 )&&(Product_Serial_Global[1] == 71))||	 //Mitel 2Gx
			 ((Product_Serial_Global[0] == 51 )&&(Product_Serial_Global[1] == 76))||	 //Mitel 3Lx   
				((Product_Serial_Global[0] == 50 )&&(Product_Serial_Global[1] == 89)&&(Product_Serial_Global[2] == 57))||	   	//Diatonis 2Y9
					((Product_Serial_Global[0] == 50 )&&(Product_Serial_Global[1] == 89)&&(Product_Serial_Global[2] == 65))){	//Diatonis 2YA
		while((counter < 15) && (!(status == 1))){  
			status = DECT_Send_String (panel,BrandingVersionCheck,BrandingVersionCheck_Response,returned_string,0,1,2); 
			Delay(1);
			counter++;
		}
	
	}
	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);
	}
	
	return status;

}




//*************************************************************
//* 
//*
//*************************************************************
int DECT_Branding_Set_Test(int panel) 
{
	
	int status = 0;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	int counter = 0;
	
	
	sprintf(msg,"DECT Branding Set Test\n"); 
	WriteToTerminal(panel, msg); 
	
	
	//Proximus	
	if(((Product_Serial_Global[0] == 50 )&&(Product_Serial_Global[1] == 87)&&(Product_Serial_Global[2] == 68))||((Product_Serial_Global[0] == 50 )&&(Product_Serial_Global[1] == 87)&&(Product_Serial_Global[2] == 69))||((Product_Serial_Global[0] == 50 )&&(Product_Serial_Global[1] == 87)&&(Product_Serial_Global[2] == 70))){	                   	
		while((counter < 15) && (!(status == 1))){  
			status = DECT_Send_String (panel,BrandingVersionSetProximus,BrandingVersionSetProximus_Response,returned_string,13,1,2);  
			Delay(1);
			counter++;
		}
	}
	else if (((Product_Serial_Global[0] == 50 )&&(Product_Serial_Global[1] == 71))||	 //Mitel 2Gx
			 ((Product_Serial_Global[0] == 51 )&&(Product_Serial_Global[1] == 76))||	 //Mitel 3Lx
				((Product_Serial_Global[0] == 50 )&&(Product_Serial_Global[1] == 89)&&(Product_Serial_Global[2] == 57))||	   	//Diatonis 2Y9
					((Product_Serial_Global[0] == 50 )&&(Product_Serial_Global[1] == 89)&&(Product_Serial_Global[2] == 65))){	//Diatonis 2YA
		while((counter < 15) && (!(status == 1))){  
			status = DECT_Send_String (panel,BrandingVersionSet,BrandingVersionSet_Response,returned_string,13,1,2);   
			Delay(1);
			counter++;
		}

	}
	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);
	}

	return status;
}


//*************************************************************
//* 
//*
//*************************************************************
int DECT_P1V8_Voltage_Adjustment_Test(int panel)
{
	
	int status = 0;
	char msg[256] = {'\0'};
	char returned_string[256];
	char previous_value = 0x00; 
	char temp_buff[256] = {'\0'};
	
	int i,j,x;
	int voltage_cal = -1;
	int counter = 0;
	int repeat_cycle_counter = 0;
	double voltage = 0;
	double voltage_1 = 0;
	double upper_limit = 1.81;
	double lower_limit = 1.79;
	double avg_voltage_temp = 0;
	double avg_voltage = 0; 
	double temp_voltage = 0;
	int cal_factor = 0;
	double dblValue=0;
	long lngIOType=0, lngChannel=0; 

	unsigned char Set_VCC1[10] = {'\0'};
	unsigned char Write_VCC1[13] = {'\0'};
	
	
	//ePut (LabJack_Handle, LJ_ioPUT_ANALOG_ENABLE_BIT, 0, 1, 0);		//FIO0 as Analog Input
	ePut (LabJack_Handle, LJ_ioPUT_ANALOG_ENABLE_BIT, 13, 1, 0);	//EIO5 as Analog Input  
	
	
	sprintf(msg,"DECT P1V8 Voltage Adjustment Test\n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg); 
	WriteToTerminal(panel, msg); 
	
	//Read Existing Value 
	status = DECT_Send_String (panel,ReadEepromVcc1,ReadEepromVcc1_Response,returned_string,5,0,1); 
	
	
	previous_value = returned_string[5]; 
	sprintf (temp_buff, "Old Calibration Factor: %d \n", previous_value);	
    //SetCtrlVal (panel, PANEL_Serial_Window_1, temp_buff);
	WriteToTerminal(panel, temp_buff); 
	//////////////////
	
	////
	
	for(i = 0; i<=5; i++){
		//read FIO0  
		AddRequest (LabJack_Handle, LJ_ioGET_AIN, 13, 0, 0, 0);
		GoOne(LabJack_Handle);
		lngChannel = 13;
		GetFirstResult(LabJack_Handle, &LJ_ioANALOG_INPUT, &lngChannel, &dblValue, 0, 0);
		
		voltage = dblValue;

		if ((voltage >= lower_limit ) && (voltage <= upper_limit)){
			//calc_avg
			for(j=1;j<=50;j++){
				AddRequest (LabJack_Handle, LJ_ioGET_AIN, 13, 0, 0, 0);
				GoOne(LabJack_Handle);
				lngChannel = 13;
				GetFirstResult(LabJack_Handle, &LJ_ioANALOG_INPUT, &lngChannel, &dblValue, 0, 0);
				avg_voltage_temp = dblValue;
				avg_voltage = avg_voltage + avg_voltage_temp;
			}
					
			avg_voltage = avg_voltage/50;
			temp_voltage = avg_voltage;
					
			if((avg_voltage >= lower_limit ) && (avg_voltage <= upper_limit)){
				sprintf (temp_buff, "Reference Voltage(1.8V): %f \n", voltage);	
    			//SetCtrlVal (panel, PANEL_Serial_Window_1, temp_buff); 
				WriteToTerminal(panel, temp_buff);  
				//voltage_cal = 1; 
				break;
			}
					
			avg_voltage_temp = 0;
			avg_voltage = 0;
		}
	}	

	
	//always recalibrate
	//voltage_cal = -1;
	
	if(voltage_cal != 1){
		//////////////////////
		//Set VCC1
		//////////////////////
			
	//while((voltage_cal == -1)&&(counter <= 5)){	
		
		while((voltage_cal == -1)&&(counter <= 15)){
			for (x=0; x < 10; x++)
		    {
		        Set_VCC1[x] = SetVcc1[x];
		    }
		
			Set_VCC1[8] = (char) counter;
			Set_VCC1[9] =  Set_VCC1[8] ^ Set_VCC1[9];  
			
			
			status = DECT_Send_String (panel,Set_VCC1,SetVcc1_Response,returned_string,0,0,1); 
			
			Delay(2);

	
			for(i = 1; i<=5; i++){
				//read EIO5  
				AddRequest (LabJack_Handle, LJ_ioGET_AIN, 13, 0, 0, 0);
				GoOne(LabJack_Handle);
				lngChannel = 13;
				GetFirstResult(LabJack_Handle, &LJ_ioANALOG_INPUT, &lngChannel, &dblValue, 0, 0);
			
				voltage = dblValue;
				sprintf (temp_buff, "Temp Voltage(1.8V): %f \n", voltage);	
    			WriteToTerminal(panel, temp_buff);
				ProcessSystemEvents (); 
		
				if ((voltage >= lower_limit ) && (voltage <= upper_limit)){
					//calc_avg
					for(j=1;j<=50;j++){
						AddRequest (LabJack_Handle, LJ_ioGET_AIN, 13, 0, 0, 0);
						GoOne(LabJack_Handle);
						lngChannel = 13;
						GetFirstResult(LabJack_Handle, &LJ_ioANALOG_INPUT, &lngChannel, &dblValue, 0, 0);
						avg_voltage_temp = dblValue;
						avg_voltage = avg_voltage + avg_voltage_temp;
						
					}
					
					avg_voltage = avg_voltage/50;
					temp_voltage = avg_voltage;
					
					//debug
					//avg_voltage = 2;
					
					if((avg_voltage >= lower_limit ) && (avg_voltage <= upper_limit)){
						voltage_cal = 1;
						//Overall_Fail = FALSE;																			// Overall_Fail = FALSE;
						sprintf (temp_buff, "Reference Voltage(1.8V): %f \n", voltage);	
    					//SetCtrlVal (panel, PANEL_Serial_Window_1, temp_buff);
						WriteToTerminal(panel, temp_buff); 
						cal_factor = Set_VCC1[8];
						sprintf (temp_buff, "New Calibration Factor: %d \n", cal_factor);	
    					//SetCtrlVal (panel, PANEL_Serial_Window_1, temp_buff);
						WriteToTerminal(panel, temp_buff); 
						break;
					}
					
					avg_voltage_temp = 0;
					avg_voltage = 0;
				}
	
			}
		
			counter = counter + 1;
		
			if((temp_voltage > 1.82)&&(counter >= 2)&&(counter < 7)){
				counter = counter - 2;
			}
			else if((temp_voltage > 1.82)&&(counter >= 7)&&(counter <= 15)){
				counter = counter - 4;
			}
			
			temp_voltage = 0;
			
		}
		
		repeat_cycle_counter ++;
		counter = 0;
	//}	
		//////////////////////
		//Write New Value if voltage_cal = 1 else fail
		//////////////////////
		if(voltage_cal == 1){
		
			for (x=0; x < 13; x++)
		    {
		        Write_VCC1[x] = WriteEepromVcc1[x];
		    }
		
			Write_VCC1[11] = Set_VCC1[8];
			Write_VCC1[12] = Write_VCC1[11] ^ Write_VCC1[12]; 
			
			status = DECT_Send_String (panel,Write_VCC1,WriteEepromVcc1_Response,returned_string,0,1,1);  
		}
	}
	
	
	sprintf(msg,"%d,",cal_factor);
	strcat(data_string,msg);
	
	sprintf(msg,"%f,",voltage);
	strcat(data_string,msg);

	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		//SetCtrlVal (panel, PANEL_Serial_Window_1, "Test Pass\n\n");
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		//SetCtrlVal (panel, PANEL_Serial_Window_1, "Test Fail\n\n");	
	}

	return status;

}	   



//*************************************************************
//*
//*
//*************************************************************
int DECT_P3V_Voltage_Measure_Test(int panel)
{

	int status = 0;
	char msg[256] = {'\0'};

	int i;
	double voltage = 0;
	double upper_limit = 3.2;
	double lower_limit = 2.9;
	double dblValue=0;
	long lngIOType=0, lngChannel=0;


	ePut (LabJack_Handle, LJ_ioPUT_ANALOG_ENABLE_BIT, 9, 1, 0);	//EIO1 as Analog Input


	sprintf(msg,"DECT P3V Voltage Measure Test\n");
	WriteToTerminal(panel, msg);



	for(i = 0; i<=5; i++)
	{
		//read EIO
		AddRequest (LabJack_Handle, LJ_ioGET_AIN_DIFF, 9, 0, 32, 0);
		Delay(1);   
		GoOne(LabJack_Handle);
		Delay(2);
		lngChannel = 9;
		GetFirstResult(LabJack_Handle, &LJ_ioANALOG_INPUT, &lngChannel, &dblValue, 0, 0);
	//	GetFirstResult(LabJack_Handle, 0, 0, &dblValue, 0, 0);          
		voltage = dblValue;

		if ((voltage >= lower_limit ) && (voltage <= upper_limit))
		{
			  status = 1;
			  break;
		}
	}

	sprintf(msg,"measure voltage : %f \n\n",voltage); 
	WriteToTerminal(panel, msg);    

	sprintf(msg,"%f,",voltage);
	strcat(data_string,msg);

	if(status == 1)
	{
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg);
	}
	else
	{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg);
	}

	return status;

}

//*************************************************************
//* 
//*
//*************************************************************
int DECT_Activate_Ref_Clk_Test(int panel) 
{
	int status = 0;
	char temp_buff[256] = {'\0'};
	int numToRead = 0;
	double Measured_Frequency;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	int counter = 0;
	
	
	sprintf(msg,"DECT Activate Ref CLK Test\n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg); 
	WriteToTerminal(panel, msg); 
	
	
	while((counter < 15) && (!(status == 1))){ 
		status = DECT_Send_String (panel,ReadEepromClock,ReadEepromClock_Response,returned_string,5,1,1);
		Delay(1);
		counter++;
	}
	
	Delay(0.1);
	//read freq
	status = TRIGGER_HP53131A_TO_MEASURE (); 
	
    //read the instrument id
    numToRead = 200;
    ibrd (UCT_HANDLE, temp_buff, numToRead);
    if (ibsta & 0x8000) status = 0;

    Measured_Frequency =  atof (temp_buff);
	
	sprintf (temp_buff, "Measured Frequency: %f Hz\n", Measured_Frequency);	
    //SetCtrlVal (panel, PANEL_Serial_Window_1, temp_buff);
	WriteToTerminal(panel, temp_buff); 
	
	//debug
	//status = 1;
	
	sprintf(msg,"%f,",Measured_Frequency);
	strcat(data_string,msg);	
	
	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
	}


	return status;
}




//*************************************************************
//* 
//*
//*************************************************************
int DECT_Set_Clk_Test(int panel)
{
	int status = 0;

	char New_SetClock[24] = {'\0'};
	char New_WriteEepromClock[24] = {'\0'};
 	char Hex_Read_Bytes[24] = {'\0'};
	char temp_buff[256] = {'\0'};
	char returned_string[256] = {'\0'}; 
	
	int	Bandgap_Frequency_Setting = 0;
	int Previous_Frequency_Setting = 0;
	int Check_Sum = 0;
	int New_Value = 0;
	int numToRead = 0;
	int x;
	int counter = 0;
	
	double Measured_Frequency; 
	
	char msg[256] = {'\0'}; 
	

	sprintf(msg,"DECT Reference Frequency Adjust Test\n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg);
	WriteToTerminal(panel, msg); 


	{

		for (int iLoop = 0; iLoop < 10; iLoop++)
		{
			//new value
			New_Value = 0x00;

			//copy SetClock to New_SetClock
			for (x = 0; x < 9; x++)
				New_SetClock[x] = (char) SetClock[x];

			New_SetClock[7] = (char) New_Value;
			Check_Sum =  SetClock[8];
			Check_Sum = Check_Sum ^ New_Value;
			New_SetClock[8] =  (char) Check_Sum;


			//send
			DECT_Send_String (panel,New_SetClock,SetClock_Response,returned_string,0,0,1);

			Delay(0.5);

			TRIGGER_HP53131A_TO_MEASURE ();

			//read the instrument id
			numToRead = 200;
			ibrd (UCT_HANDLE, temp_buff, numToRead);
			if (ibsta & 0x8000) status = 0;

			Measured_Frequency =  atof (temp_buff);

			sprintf (temp_buff, "\nReference Clock Setting: 0x00\n");
			//SetCtrlVal (panel, PANEL_Serial_Window_1, temp_buff);
			WriteToTerminal(panel, temp_buff);
			sprintf (temp_buff, "Measured Reference Clock Frequency: %.4f Hz\n", Measured_Frequency);
			//SetCtrlVal (panel, PANEL_Serial_Window_1, temp_buff);
			WriteToTerminal(panel, temp_buff);
			
			if(Measured_Frequency <= 10367900)
			{
				break;
			}


			if(Measured_Frequency > 10367900 && 9 == iLoop)
			{
				sprintf(msg,"Test Fail\n\n");
				WriteToTerminal(panel, msg);
				return 0 ;
			}

		}




	}

	{//new value
	

	for (int iLoop = 0; iLoop < 10; iLoop++)
	{

		New_Value = 0xDF;

		//copy SetClock to New_SetClock
		for (x = 0; x < 9; x++)
			New_SetClock[x] = (char) SetClock[x];

		New_SetClock[7] = (char) New_Value;
		Check_Sum =  SetClock[8];
		Check_Sum = Check_Sum ^ New_Value;
		New_SetClock[8] =  (char) Check_Sum;


		//send
		DECT_Send_String (panel,New_SetClock,SetClock_Response,returned_string,0,0,1);

		Delay(0.5);


		TRIGGER_HP53131A_TO_MEASURE ();

		//read the instrument id
		numToRead = 200;
		ibrd (UCT_HANDLE, temp_buff, numToRead);
		if (ibsta & 0x8000) status = 0;

		Measured_Frequency =  atof (temp_buff);

		sprintf (temp_buff, "\nReference Clock Setting: 0xDF\n");
		//SetCtrlVal (panel, PANEL_Serial_Window_1, temp_buff);
		WriteToTerminal(panel, temp_buff);
		sprintf (temp_buff, "Measured Reference Clock Frequency: %.4f Hz\n", Measured_Frequency);
		//SetCtrlVal (panel, PANEL_Serial_Window_1, temp_buff);
		WriteToTerminal(panel, temp_buff);
		
		
		if(Measured_Frequency >= 10368200)
		{
			break;
		}


		if(Measured_Frequency < 10368200 && 9 == iLoop)
		{
			sprintf(msg,"Test Fail\n\n");
			WriteToTerminal(panel, msg);
			return 0 ;
		}

	}


		
	}
	
	New_Value = 223 - (((Measured_Frequency - 10368000)/5));
	
	if (New_Value <= 0x00 || New_Value > 0xdf) New_Value = 125;   

		//calculate new value		
		while (!(status)&&(counter <= 20))	
		{
			for (x = 0; x < 9; x++){
				New_SetClock[x] = SetClock[x];
			}

			New_SetClock[7] = (char) New_Value;
			Check_Sum =  SetClock[8];
			Check_Sum = Check_Sum ^ New_Value; 
			New_SetClock[8] =  (char) Check_Sum;
		
			//send 
			DECT_Send_String (panel,New_SetClock,SetClock_Response,returned_string,0,0,1);
			
			Delay(0.5);
			
			TRIGGER_HP53131A_TO_MEASURE (); 
		    numToRead = 200;
		    ibrd (UCT_HANDLE, temp_buff, numToRead);
		    if (ibsta & 0x8000) status = 0;

		    Measured_Frequency =  atof (temp_buff);
			
			sprintf (temp_buff, "\nReference Clock Setting: %d \n", New_Value);	
    		//SetCtrlVal (panel, PANEL_Serial_Window_1, temp_buff);
			WriteToTerminal(panel, temp_buff);
			sprintf (temp_buff, "Measured Reference Clock Frequency: %.4f Hz\n", Measured_Frequency);	
    		//SetCtrlVal (panel, PANEL_Serial_Window_1, temp_buff);
			WriteToTerminal(panel, temp_buff); 
			
   
		    if ((Measured_Frequency >= 10367995) && (Measured_Frequency <= 10368005)) 
			{

				for (x = 0; x < 10; x++){
					New_WriteEepromClock[x] = WriteEepromClock[x];
				}
				
				New_WriteEepromClock[8] = (char) New_Value;
				Check_Sum =  New_WriteEepromClock[9];
				Check_Sum = Check_Sum ^ New_Value;
				New_WriteEepromClock[9] =  (char) Check_Sum;
				
				DECT_Send_String (panel,New_WriteEepromClock,WriteEepromClock_Response,returned_string,0,0,1);  

			 	status = 1;
			
		    }
		    else
			{
		        //retry
		        Delay (0.05);
				Previous_Frequency_Setting = New_Value;
				
		    }
				
				
			if(Measured_Frequency > 10368075){
				New_Value =  Previous_Frequency_Setting - 20; 
			}
			else if(Measured_Frequency < 10367925){ 
					New_Value =  Previous_Frequency_Setting + 20;
			}	
			else if((Measured_Frequency >= 10368030)&&(Measured_Frequency <= 10368075)){
				New_Value =  Previous_Frequency_Setting - 12; 
			}
			else if((Measured_Frequency >= 10367925)&&(Measured_Frequency <= 10367970)){ 
					New_Value =  Previous_Frequency_Setting + 12;
			}
			else if((Measured_Frequency >= 10368010)&&(Measured_Frequency < 10368030)){
				New_Value =  Previous_Frequency_Setting - 3; 
			}
			else if((Measured_Frequency > 10367970)&&(Measured_Frequency <= 10367990)){ 
					New_Value =  Previous_Frequency_Setting + 3;
			}
			else if((Measured_Frequency > 10368005)&&(Measured_Frequency < 10368010)){
				New_Value =  Previous_Frequency_Setting - 1; 
			}
			else if((Measured_Frequency > 10367990)&&(Measured_Frequency < 10367995)){ 
					New_Value =  Previous_Frequency_Setting + 1;
			}
			
			if (New_Value <= 0x00 || New_Value > 0xdf) New_Value = 125;

			counter ++;

		} //while
		
		
	sprintf(msg,"%d,",New_Value);
	strcat(data_string,msg);	
		
	sprintf(msg,"%f,",Measured_Frequency);
	strcat(data_string,msg);	
	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		//SetCtrlVal (panel, PANEL_Serial_Window_1, "Test Pass\n\n");
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		//SetCtrlVal (panel, PANEL_Serial_Window_1, "Test Fail\n\n");	
	}

	return status;
}	





/*****************************************/
/*
/*
/*****************************************/
int WriteToTerminal(int panel, char *buffer)
{
	int buffer_size = 0;
	
	SetCtrlVal (panel, PANEL_Serial_Window_1,buffer);  
	
	if(debug == 1){
		buffer_size = strlen(buffer);		
		WriteFile (debug_File_Handle, buffer, buffer_size);
	}
	
	return 1;
}



/*==============================================================================================================*/
//New Tests
/*==============================================================================================================*/



//*************************************************************
//* 
//*
//*************************************************************
int DECT_Enter_Enhanced_Test_Mode(int panel)
{

	int status = 0;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'}; 
	int counter = 0;

	
	sprintf(msg,"DECT Enter Enhanced Test Mode Test\n");
	WriteToTerminal(panel, msg); 
	
	Delay(5);
	
	while((counter < 20) && (!(status == 1))){
		status = DECT_Send_String (panel,EnterEnhancedTestMode,EnterEnhancedTestMode_Response,returned_string,0,1,1);	 
		counter ++;
	}
	
	counter = 0;
	
	if(status == 0){
		sprintf(msg,"请确认手机完全开机并完成初始化，再按OK\nPlease confirm phone is fully booted, and passed the INIT screen, then press OK\n");
		Display_OK_MSG(msg);
		
		CloseCom(COM_port); 
		Delay(1);
		COM_port = Serial_Port_Scan (panel);
		if(((Test_Type == 3)||(Test_Type == 4)||(Test_Type == 5))&&(COM_port == 1)){ 
			COM_port = 0;
			Delay(5);
			COM_port = Serial_Port_Scan (panel); 
		}
		Delay(1);
		Open_COM_PORT_1(panel);
		Delay(2);
		
		while((counter < 10) && (!(status == 1))){
			status = DECT_Send_String (panel,EnterEnhancedTestMode,EnterEnhancedTestMode_Response,returned_string,0,1,1);	 
			counter ++;
		}
	}
	
	sprintf(msg,"COM Port: %d\n",COM_port);
	WriteToTerminal(panel, msg);
	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);	
	}
	
	return status;
}



//*************************************************************
//* 
//*
//*************************************************************
int DECT_Audio_Speaker_Test(int panel) 
{
	int status = 0;
	char temp_buff[256] = {'\0'};
	int numToRead = 0;
	double Measured_Frequency;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	int counter = 0;
	unsigned char convert_rx_data[4] = {'\0'};
	unsigned short data = 0;
	int i = 0;
	int x = 0;
	
	
	sprintf(msg,"DECT Audio Speaker Test\n");
	WriteToTerminal(panel, msg); 
	
	while((counter < 10) && (!(status == 1))){  
		status = DECT_Send_String (panel,AudioSPK,AudioSPK_Response,returned_string,6,1,10);  
		Delay(1);
		counter++;
	}
	
	for (i = 17; i <= 20; i++){
		if ((returned_string[i] >= '0' && returned_string[i] <= '9'))
		{
			convert_rx_data[x] = returned_string[i] - '0';
		}
		else if (returned_string[i] >= 'A' && returned_string[i] <= 'F')
		{
			convert_rx_data[x] = returned_string[i] - 'A' + 10;
		}
		else if (returned_string[i] >= 'a' && returned_string[i] <= 'f') // Convert upper case
		{
			convert_rx_data[x] = returned_string[i] - 'a' + 10;           
		}
		x++;
	}
	data = (convert_rx_data[0] << 12) | (convert_rx_data[1] << 8) | (convert_rx_data[2] << 4) | (convert_rx_data[3]);
	
	sprintf(temp_buff,"AUDIO-SPK = %d\n", data);
	WriteToTerminal(panel, temp_buff);

	
	sprintf(msg,"%d,",data);
	strcat(data_string,msg);
	
	

	if((data >= 4096)&&(data <= 32767)){
		status = 1;
	}
	else{
		status = 0;
	}
	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
	}


	return status;
}



//*************************************************************
//* 
//*
//*************************************************************
int DECT_Audio_MIC_Test(int panel) 
{
	int status = 0;
	char temp_buff[256] = {'\0'};
	int numToRead = 0;
	double Measured_Frequency;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	int counter = 0;
	unsigned char convert_rx_data[4] = {'\0'};
	unsigned short data = 0;
	int i = 0;
	int x = 0;
	
	sprintf(msg,"DECT Audio MIC Test\n");
	WriteToTerminal(panel, msg); 
	
	while((counter < 10) && (!(status == 1))){  
		status = DECT_Send_String (panel,AudioMIC,AudioMIC_Response,returned_string,6,1,10);  
		Delay(1);
		counter++;
	}
	
	
	for (i = 17; i <= 20; i++){
		if ((returned_string[i] >= '0' && returned_string[i] <= '9'))
		{
			convert_rx_data[x] = returned_string[i] - '0';
		}
		else if (returned_string[i] >= 'A' && returned_string[i] <= 'F')
		{
			convert_rx_data[x] = returned_string[i] - 'A' + 10;
		}
		else if (returned_string[i] >= 'a' && returned_string[i] <= 'f') // Convert upper case
		{
			convert_rx_data[x] = returned_string[i] - 'a' + 10;           
		}
		x++;
	}
	data = (convert_rx_data[0] << 12) | (convert_rx_data[1] << 8) | (convert_rx_data[2] << 4) | (convert_rx_data[3]);
	
	
	sprintf(temp_buff,"AUDIO-MIC = %d\n", data);
	WriteToTerminal(panel, temp_buff);
	
	
	sprintf(msg,"%d,",data);
	strcat(data_string,msg);
	
	
	if((data >= 4096)&&(data <= 32767)){
		status = 1;
	}
	else{
		status = 0;
	}
	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
	}


	return status;
}



//*************************************************************
//* 
//*
//*************************************************************
int DECT_Audio_EP_Test(int panel) 
{
	int status = 0;
	char temp_buff[256] = {'\0'};
	int numToRead = 0;
	double Measured_Frequency;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	int counter = 0;
	unsigned char convert_rx_data[4] = {'\0'};
	unsigned short data = 0;
	int i = 0;
	int x = 0;
	
	sprintf(msg,"DECT Audio EP Test\n");
	WriteToTerminal(panel, msg); 
	
	while((counter < 10) && (!(status == 1))){  
		status = DECT_Send_String (panel,AudioEP,AudioEP_Response,returned_string,6,1,10);  
		Delay(1);
		counter++;
	}
	
	for (i = 16; i <= 19; i++){
		if ((returned_string[i] >= '0' && returned_string[i] <= '9'))
		{
			convert_rx_data[x] = returned_string[i] - '0';
		}
		else if (returned_string[i] >= 'A' && returned_string[i] <= 'F')
		{
			convert_rx_data[x] = returned_string[i] - 'A' + 10;
		}
		else if (returned_string[i] >= 'a' && returned_string[i] <= 'f') // Convert upper case
		{
			convert_rx_data[x] = returned_string[i] - 'a' + 10;           
		}
		x++;
	}
	data = (convert_rx_data[0] << 12) | (convert_rx_data[1] << 8) | (convert_rx_data[2] << 4) | (convert_rx_data[3]);
	
	sprintf(temp_buff,"AUDIO-EP = %d\n", data);
	WriteToTerminal(panel, temp_buff);
	
	
	sprintf(msg,"%d,",data);
	strcat(data_string,msg);
	

	if((data >= 256)&&(data <= 10000)){
		status = 1;
	}
	else{
		status = 0;
	}
	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
	}


	return status;
}



//*************************************************************
//* 
//*
//*************************************************************
int DECT_Audio_Trembler_Test(int panel) 
{
	int status = 0;
	char temp_buff[256] = {'\0'};
	int numToRead = 0;
	double Measured_Frequency;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	int counter = 0;
	unsigned char convert_rx_data[4] = {'\0'};
	unsigned short data = 0;
	int i = 0;
	int x = 0;
	
	
	sprintf(msg,"DECT Audio Trembler Test\n");
	WriteToTerminal(panel, msg); 
	
	while((counter < 10) && (!(status == 1))){  
		status = DECT_Send_String (panel,Trembler,Trembler_Response,returned_string,6,1,10);  
		Delay(1);
		counter++;
	}
	
	
	for (i = 16; i <= 19; i++){
		if ((returned_string[i] >= '0' && returned_string[i] <= '9'))
		{
			convert_rx_data[x] = returned_string[i] - '0';
		}
		else if (returned_string[i] >= 'A' && returned_string[i] <= 'F')
		{
			convert_rx_data[x] = returned_string[i] - 'A' + 10;
		}
		else if (returned_string[i] >= 'a' && returned_string[i] <= 'f') // Convert upper case
		{
			convert_rx_data[x] = returned_string[i] - 'a' + 10;           
		}
		x++;
	}
	data = (convert_rx_data[0] << 12) | (convert_rx_data[1] << 8) | (convert_rx_data[2] << 4) | (convert_rx_data[3]);
	
	sprintf(temp_buff,"AUDIO-TREMBLER = %d\n", data);
	WriteToTerminal(panel, temp_buff);
	
	sprintf(msg,"%d,",data);
	strcat(data_string,msg);
	

	if((data >= 1000)&&(data <= 32767)){
		status = 1;
	}
	else{
		status = 0;
	}
	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
	}


	return status;
}



//*************************************************************
//* 
//* 
//*************************************************************
int DECT_Audio_Headset_Test(int panel, int iRepeat) 
{
	int status = 0;
	char temp_buff[256] = {'\0'};
	int numToRead = 0;
	double Measured_Frequency;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	int counter = 0;
	char temp_val[12] = {'\0'};
	char temp_val_2 = 0x0000;
	//int headset_mode = 0;
	unsigned long HexNumber = 0;
	int short_string = 0;
	unsigned char convert_rx_data[4] = {'\0'};
	unsigned short data = 0;
	int i = 0;
	int x = 0;
 
	sprintf(msg,"DECT Audio Headset Test\n");
	WriteToTerminal(panel, msg); 
	
	sprintf(msg,"插入耳机\n\nPlug in Headset Loopback.");
	Display_OK_MSG(msg); 
	
	while((counter < 10) && (!(status == 1))){  
		status = DECT_Send_String (panel,Headset,Headset_Response,returned_string,-1,1,10);  
		Delay(1);
		counter++;
	}
	

	HexNumber = ((returned_string [49] << 24) | (returned_string [50] << 16) | (returned_string [51] << 8) | (returned_string [52]));
	
	if(HexNumber !=0){
		for (i = 49; i <= 52; i++){
			if ((returned_string[i] >= '0' && returned_string[i] <= '9'))
			{
				convert_rx_data[x] = returned_string[i] - '0';
			}
			else if (returned_string[i] >= 'A' && returned_string[i] <= 'F')
			{
				convert_rx_data[x] = returned_string[i] - 'A' + 10;
			}
			else if (returned_string[i] >= 'a' && returned_string[i] <= 'f') // Convert upper case
			{
				convert_rx_data[x] = returned_string[i] - 'a' + 10;           
			}
			x++;
		}
		data = (convert_rx_data[0] << 12) | (convert_rx_data[1] << 8) | (convert_rx_data[2] << 4) | (convert_rx_data[3]);
	}

	
	if(HexNumber != 0){
		sprintf(temp_buff,"HEADSET = %d\n", data);
		WriteToTerminal(panel, temp_buff);
	}
	
	if(HexNumber == 0){
		short_string = 1;
		x = 0;
		for (i = 16; i <= 19; i++){
			if ((returned_string[i] >= '0' && returned_string[i] <= '9'))
			{
				convert_rx_data[x] = returned_string[i] - '0';
			}
			else if (returned_string[i] >= 'A' && returned_string[i] <= 'F')
			{
				convert_rx_data[x] = returned_string[i] - 'A' + 10;
			}
			else if (returned_string[i] >= 'a' && returned_string[i] <= 'f') // Convert upper case
			{
				convert_rx_data[x] = returned_string[i] - 'a' + 10;           
			}
			x++;
		}
		
		data = (convert_rx_data[0] << 12) | (convert_rx_data[1] << 8) | (convert_rx_data[2] << 4) | (convert_rx_data[3]);
	
		sprintf(temp_buff,"HEADSET = %d\n", data);
		WriteToTerminal(panel, temp_buff);
	}
	
	

	
	
	sprintf(msg,"移除耳机\n\nRemove Headset Loopback.");
	Display_OK_MSG(msg); 
	
	
	//Limits = 0100..7FFF Hex
	//Limits = 256 .. 32767 Decimal
	if((data >= 8000)&&(data <= 12000)){
		status = 1;
	}
	else
	{
		status = 0;
	}

	if(status == 1)
	{
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg);

		sprintf(msg,"%d,",data);
		strcat(data_string,msg);
	}
	else
	{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg);


		if(iRepeat == 2)
		{
			sprintf(msg,"%d,",data);
			strcat(data_string,msg);
		}

	}



	return status;
}



////////////////////////////////////////////////////////
///////////////////////////////////////////////////////


//calibration MIC, wait for support
double Get_Mic_sensitivity()
{
	//IniText iniText;
	
	/* Create a new Inifile object and read it from a file */
	if (!(g_myInifile = Ini_New (0)))
	{
		MessagePopup("Inifile","Error allocating memory for Inifile");
		return 0;
	}

	strcpy(g_fileName,"C:\\audio analysisl\\data.ini");
	if (Ini_ReadFromFile (g_myInifile, g_fileName))
	{
		MessagePopup("Inifile","Error reading Inifile");
		return 0;
	}
	
	double dMicSenVal;
	if( Ini_GetDouble (g_myInifile, "DECT_BASE", "Microphone sensitivity", &dMicSenVal) <=0)
	{
		MessagePopup("Inifile","Error reading Inifile");
		return 0;
	}
	
	double dSoundMaxInputVolVal;
	if( Ini_GetDouble (g_myInifile, "DECT_BASE", "Sound card maximum input voltage RMS", &dSoundMaxInputVolVal) <=0)
	{
		MessagePopup("Inifile","Error reading Inifile");
		return 0;
	}

	if (g_myInifile)
	{
		Ini_Dispose (g_myInifile);
		g_myInifile = 0;
	}
	//double dVal =   dMicSenVal/dSoundMaxInputVolVal;
	//return dVal/1000;
	return		 dMicSenVal;
}



 int CVICALLBACK Thread_Single_tone_capture (void *functionData)
 {
	//MessagePopup("thread","start");
	int   status = 0;   
	 
	uint32_t deviceID = 0;   //will replace to auto detect
	int32_t bitsPerSample = 16;
	double sampleRate = 44100;
	int32_t numberOfChannels = 2;   
	

	

	status = Single_tone_capture(deviceID,sampleRate,numberOfChannels,bitsPerSample,0,0);  
	
	 if(status == 0)
		 return 1;
	 
	 else return 0;

 }



int DECT_Audio_TestReset_Test(int panel)
{
	int status = 0;
	
	int status1 = 0;     
	int status2 = 0;     
    
	
	char temp_buff[256] = {'\0'};
	int numToRead = 0;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	int counter = 0;

	
	sprintf(msg,"DECT Audio Test Result Reset Test\n");
	WriteToTerminal(panel, msg); 
	
	while((counter < 10) && (!(status1 == 1))){  
		status1 = DECT_Send_String (panel,SPKTestReset,SPKTestReset_Response,returned_string,9,1,1);  
		Delay(1);
		counter++;
	}  
	
	
	while((counter < 10) && (!(status2 == 1)))
	{
		status2 = DECT_Send_String (panel,RECTestReset,RECTestReset_Response,returned_string,9,1,1);
		Delay(1);
		counter++;
	}
	
	

	
	status = status1&status2;
	
	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
			sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);
	}

	return status;
}


int DECT_Audio_TestPass_Test(int panel)
{
	int status = 0;
	int status1 = 0;
	int status2 = 0;

	char temp_buff[256] = {'\0'};
	int numToRead = 0;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	int counter = 0;

	
	sprintf(msg,"DECT Audio Test Result Pass Test\n");
	WriteToTerminal(panel, msg); 
	
	while((counter < 10) && (!(status1 == 1))){  
		status1 = DECT_Send_String (panel,SPKTestPass,SPKTestPass_Response,returned_string,9,1,1);  
		Delay(1);
		counter++;
	}  
	
	
	while((counter < 10) && (!(status2 == 1)))
	{
		status2 = DECT_Send_String (panel,RECTestPass,RECTestPass_Response,returned_string,9,1,1);
		Delay(1);
		counter++;
	}
	
	

	
	status = status1&status2;
	
	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
			sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);
	}


	return status;
}


int DECT_Trembler_TestReset_Test(int panel)
{
	int status = 0;
	
	char temp_buff[256] = {'\0'};
	int numToRead = 0;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	int counter = 0;

	
	sprintf(msg,"DECT Trembler Test Result Reset Test\n");
	WriteToTerminal(panel, msg); 
	
	
	while((counter < 10) && (!(status == 1)))
	{
		status = DECT_Send_String (panel,TremblerTestReset,TremblerTestReset_Response,returned_string,9,1,1);
		Delay(1);
		counter++;
	}
	
	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
			sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);
	}

	return status;
}


int DECT_Trembler_TestPass_Test(int panel)
{
	int status = 0;
	char temp_buff[256] = {'\0'};
	int numToRead = 0;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	int counter = 0;

	
	sprintf(msg,"DECT Trembler Test Result Pass Test\n");
	WriteToTerminal(panel, msg); 
	
	
	while((counter < 10) && (!(status == 1)))
	{
		status = DECT_Send_String (panel,TremblerTestPass,TremblerTestPass_Response,returned_string,9,1,1);
		Delay(1);
		counter++;
	}
	
	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
			sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);
	}


	return status;
}




int DECT_Audio_Speaker_Test_Ex(int panel, int iRepeat) 
{
	int status = 0;   
	char msg[256] = {'\0'};  
	char returned_string[256] = {'\0'};    
	
	
	sprintf(msg,"DECT Audio Speaker Test Ex\n");
	WriteToTerminal(panel, msg); 
	
	//start tone
	status = DECT_Send_String (panel,AudioStartTone,AudioStartTone_Response,returned_string,6,1,1);  
	
	if(status != 1)
	{
		sprintf(msg,"Send start tone Fail\n\n");
		WriteToTerminal(panel, msg); 
		return    0;
	}

	ProcessSystemEvents (); 

	CmtScheduleThreadPoolFunction (DEFAULT_THREAD_POOL_HANDLE, Thread_Single_tone_capture, 0, &gThreadID);   


	Delay(2);
	
	Single_tone_capture_stop(); 
	
	CmtWaitForThreadPoolFunctionCompletion (DEFAULT_THREAD_POOL_HANDLE, gThreadID, OPT_TP_PROCESS_EVENTS_WHILE_WAITING);
	
	CmtReleaseThreadPoolFunctionID(DEFAULT_THREAD_POOL_HANDLE, gThreadID);	   
	

	int sec,min,hour,year,month,day;
	GetSystemTime(&hour,&min,&sec);
	GetSystemDate(&month,&day,&year);
	
	unsigned char cFileName[MAX_PATHNAME_LEN] = {0};
	unsigned char cPathName[MAX_PATHNAME_LEN] = {0};     
	char cTemp[MAX_PATHNAME_LEN] = {0};
	
	
	
	sprintf(cTemp,"SoundRecord");

	GetFullPathFromProject (cTemp,cPathName );
	if(FileExists (cPathName, NULL)==0)
		MakeDir(cPathName);


	sprintf(cTemp,"SoundRecord\\%04d%02d%02d",year,month,day);

	GetFullPathFromProject (cTemp,cPathName );
	if(FileExists (cPathName, NULL)==0)
		MakeDir(cPathName);



	sprintf(cFileName,"\\%s_Spk_%04d%02d%02d_%02d%02d%02d.wav","SN",year,month,day,hour,min,sec);


	
	Single_tone_save(cPathName,cFileName,2,44100,16);
	 
	 

	
	uint8_t analysis_channel = 0; 
	double mic_sensetivity = Get_Mic_sensitivity(); 
	double sampleRate = 44100;  

  
	
	double detectedFrequency, detectedAmplitude, detectedTHD; 
		

		
	status = Single_tone_analysis(analysis_channel,mic_sensetivity, sampleRate,  &detectedFrequency, &detectedAmplitude,&detectedTHD);
	
	

	
	if(status != 0)
	{
		sprintf(msg,"AudioAnalysis Fail\n\n");
		WriteToTerminal(panel, msg);

		//stop tone
		status = DECT_Send_String (panel,AudioStopTone,AudioStopTone_Response,returned_string,6,1,1);

		if(status != 1)
		{
			sprintf(msg,"Send stop tone Fail\n\n");
			WriteToTerminal(panel, msg);
			return    0;
		}

		return    0;
	}

	//stop tone
	status = DECT_Send_String (panel,AudioStopTone,AudioStopTone_Response,returned_string,6,1,1);  
	
	if(status != 1)
	{
		sprintf(msg,"Send stop tone Fail\n\n");
		WriteToTerminal(panel, msg); 
		return    0;
	}
	
	
	g_dSpkFreq =   detectedFrequency;
	g_dSpkAmp =   detectedAmplitude;  
	g_dSpkThd =   detectedTHD;  
	
	sprintf(msg,"Speaker Test finish. detectedFrequency %f; detectedAmplitude %f; detectedTHD %f\n\n",detectedFrequency,detectedAmplitude,detectedTHD); 
	WriteToTerminal(panel, msg);  
	
	
	if(strstr(msg,"NaN") != NULL)
	{
		sprintf(msg,"detected NaN\n\n");
		WriteToTerminal(panel, msg);

		if(iRepeat == 2)
		{
			sprintf(msg,"%f; %f; %f,",detectedFrequency,detectedAmplitude,detectedTHD);

			strcat(data_string,msg);
		}


		return    0;
	}

	
	
	if((detectedFrequency >= 1049||detectedFrequency <= 1045)){
		sprintf(msg,"detectedFrequency %f is out of range,limit 1045 to 1049\n\n",detectedFrequency);
		WriteToTerminal(panel, msg); 
		
		

		if(iRepeat == 2)
		{
			sprintf(msg,"%f; %f; %f,",detectedFrequency,detectedAmplitude,detectedTHD);

			strcat(data_string,msg);
		}
		
		
		return    0;
	}
	
	//if((detectedAmplitude <= 31||detectedAmplitude >= 43)){
//	if((detectedAmplitude <= 90||detectedAmplitude >= 126)){   	 	  //data set by fpe1
	if((detectedAmplitude <= 90||detectedAmplitude >= 126)){   	 	  //data set by MP    
		sprintf(msg,"detectedAmplitude %f is out of range, limit 90 to 126\n\n",detectedAmplitude);
		WriteToTerminal(panel, msg);


		if(iRepeat == 2)
		{
			sprintf(msg,"%f; %f; %f,",detectedFrequency,detectedAmplitude,detectedTHD);

			strcat(data_string,msg);
		}
		return    0;
	}
	
	if((detectedTHD >= 5))
	{
		sprintf(msg,"detectedTHD %f is out of range, limit < 5\n\n",detectedTHD);
		WriteToTerminal(panel, msg);


		if(iRepeat == 2)
		{
			sprintf(msg,"%f; %f; %f,",detectedFrequency,detectedAmplitude,detectedTHD);

			strcat(data_string,msg);
		}
		return    0;
	}

	
	
	sprintf(msg,"%f; %f; %f,",detectedFrequency,detectedAmplitude,detectedTHD); 
	
	strcat(data_string,msg);  

	

	sprintf(msg,"Speaker Test Pass. detectedFrequency %f; detectedAmplitude %f; detectedTHD %f\n\n",detectedFrequency,detectedAmplitude,detectedTHD); 
	WriteToTerminal(panel, msg);   
	return 1;
	

}

int DECT_Audio_EarpieceTest_Test_Ex(int panel, int iRepeat) 
{
		int status = 0;   
	char msg[256] = {'\0'};  
	char returned_string[256] = {'\0'};    
	
	
	sprintf(msg,"DECT Audio Earpiece Test Ex\n");
	WriteToTerminal(panel, msg); 
	
	//start tone
	status = DECT_Send_String (panel,AudioStartToneEarpiece,AudioStartToneEarpiece_Response,returned_string,6,1,1);  
	
	if(status != 1)
	{
		sprintf(msg,"Send start tone Fail\n\n");
		WriteToTerminal(panel, msg); 
		return    0;
	}

	ProcessSystemEvents (); 

	CmtScheduleThreadPoolFunction (DEFAULT_THREAD_POOL_HANDLE, Thread_Single_tone_capture, 0, &gThreadID);   


	Delay(2);
	
	Single_tone_capture_stop(); 
	
	CmtWaitForThreadPoolFunctionCompletion (DEFAULT_THREAD_POOL_HANDLE, gThreadID, OPT_TP_PROCESS_EVENTS_WHILE_WAITING);
	
	CmtReleaseThreadPoolFunctionID(DEFAULT_THREAD_POOL_HANDLE, gThreadID);	   
	

	int sec,min,hour,year,month,day;
	GetSystemTime(&hour,&min,&sec);
	GetSystemDate(&month,&day,&year);
	
	unsigned char cFileName[MAX_PATHNAME_LEN] = {0};
	unsigned char cPathName[MAX_PATHNAME_LEN] = {0};     
	char cTemp[MAX_PATHNAME_LEN] = {0};
	
	
	
	sprintf(cTemp,"SoundRecord");

	GetFullPathFromProject (cTemp,cPathName );
	if(FileExists (cPathName, NULL)==0)
		MakeDir(cPathName);


	sprintf(cTemp,"SoundRecord\\%04d%02d%02d",year,month,day);

	GetFullPathFromProject (cTemp,cPathName );
	if(FileExists (cPathName, NULL)==0)
		MakeDir(cPathName);



	sprintf(cFileName,"\\%s_Rec_%04d%02d%02d_%02d%02d%02d.wav","SN",year,month,day,hour,min,sec);


	
	Single_tone_save(cPathName,cFileName,2,44100,16);
	 
	 

	
	uint8_t analysis_channel = 0; 
	double mic_sensetivity = Get_Mic_sensitivity(); 
	double sampleRate = 44100;  

  
	
	double detectedFrequency, detectedAmplitude, detectedTHD; 
		

		
	status = Single_tone_analysis(analysis_channel,mic_sensetivity, sampleRate,  &detectedFrequency, &detectedAmplitude,&detectedTHD);
	
	

	
	if(status != 0)
	{
		sprintf(msg,"AudioAnalysis Fail\n\n");
		WriteToTerminal(panel, msg);

		//stop tone
		status = DECT_Send_String (panel,AudioStopTone,AudioStopTone_Response,returned_string,6,1,1);

		if(status != 1)
		{
			sprintf(msg,"Send stop tone Fail\n\n");
			WriteToTerminal(panel, msg);
			return    0;
		}

		return    0;
	}

	//stop tone
	status = DECT_Send_String (panel,AudioStopTone,AudioStopTone_Response,returned_string,6,1,1);  
	
	if(status != 1)
	{
		sprintf(msg,"Send stop tone Fail\n\n");
		WriteToTerminal(panel, msg); 
		return    0;
	}

	
	g_dRecFreq =   detectedFrequency;
	g_dRecAmp =   detectedAmplitude;  
	g_dRecThd =   detectedTHD;  
	
	sprintf(msg,"Earpiece Test finish. detectedFrequency %f; detectedAmplitude %f; detectedTHD %f\n\n",detectedFrequency,detectedAmplitude,detectedTHD); 
	WriteToTerminal(panel, msg);
	
	
	
	if(strstr(msg,"NaN") != NULL)
	{
		sprintf(msg,"detected NaN\n\n");
		WriteToTerminal(panel, msg);

		if(iRepeat == 2)
		{
			sprintf(msg,"%f; %f; %f,",detectedFrequency,detectedAmplitude,detectedTHD);

			strcat(data_string,msg);
		}


		return    0;
	}

  
	
	
	if((detectedFrequency >= 1049||detectedFrequency <= 1045)){
		sprintf(msg,"detectedFrequency %f is out of range,limit 1045 to 1049\n\n",detectedFrequency);
		WriteToTerminal(panel, msg); 
		return    0;
	}
	
//	if((detectedAmplitude <= 14||detectedAmplitude >= 26)){
//	if((detectedAmplitude <= 63||detectedAmplitude >= 103)){ 
	if((detectedAmplitude <= 70||detectedAmplitude >= 94)){  // data set by MP       
	 //for collect data   
		sprintf(msg,"detectedAmplitude %f is out of range, limit 63 to 103\n\n",detectedAmplitude);
		WriteToTerminal(panel, msg);


		if(iRepeat == 2)
		{
			sprintf(msg,"%f; %f; %f,",detectedFrequency,detectedAmplitude,detectedTHD);

			strcat(data_string,msg);
		}
		return    0;
	}

	if((detectedTHD >= 5))
	{
		sprintf(msg,"detectedTHD %f is out of range, limit < 5\n\n",detectedTHD);
		WriteToTerminal(panel, msg);

		if(iRepeat == 2)
		{
			sprintf(msg,"%f; %f; %f,",detectedFrequency,detectedAmplitude,detectedTHD);

			strcat(data_string,msg);
		}
		return    0;
	}

	sprintf(msg,"%f; %f; %f,",detectedFrequency,detectedAmplitude,detectedTHD);
	
	strcat(data_string,msg);   
	

	sprintf(msg,"Earpiece Test Pass. detectedFrequency %f; detectedAmplitude %f; detectedTHD %f\n\n",detectedFrequency,detectedAmplitude,detectedTHD); 
	WriteToTerminal(panel, msg);   
	return 1;
	

}

int DECT_Audio_TremblerTest_Test_Ex(int panel, int iRepeat) 
{

	
		int status = 0;   
	char msg[256] = {'\0'};  
	char returned_string[256] = {'\0'};    
	
	
	sprintf(msg,"DECT Audio Trembler Test Ex\n");
	WriteToTerminal(panel, msg); 
	
	//start tone
	status = DECT_Send_String (panel,AudioTremblerOn,AudioTremblerOn_Response,returned_string,6,1,1);  
	
	if(status != 1)
	{
		sprintf(msg,"Send start tone Fail\n\n");
		WriteToTerminal(panel, msg); 
		return    0;
	}

	ProcessSystemEvents (); 

	CmtScheduleThreadPoolFunction (DEFAULT_THREAD_POOL_HANDLE, Thread_Single_tone_capture, 0, &gThreadID);   


	Delay(2);
	
	Single_tone_capture_stop(); 
	
	CmtWaitForThreadPoolFunctionCompletion (DEFAULT_THREAD_POOL_HANDLE, gThreadID, OPT_TP_PROCESS_EVENTS_WHILE_WAITING);
	
	CmtReleaseThreadPoolFunctionID(DEFAULT_THREAD_POOL_HANDLE, gThreadID);	   
	

	int sec,min,hour,year,month,day;
	GetSystemTime(&hour,&min,&sec);
	GetSystemDate(&month,&day,&year);
	
	unsigned char cFileName[MAX_PATHNAME_LEN] = {0};
	unsigned char cPathName[MAX_PATHNAME_LEN] = {0};     
	char cTemp[MAX_PATHNAME_LEN] = {0};
	
	
	
	sprintf(cTemp,"SoundRecord");

	GetFullPathFromProject (cTemp,cPathName );
	if(FileExists (cPathName, NULL)==0)
		MakeDir(cPathName);


	sprintf(cTemp,"SoundRecord\\%04d%02d%02d",year,month,day);

	GetFullPathFromProject (cTemp,cPathName );
	if(FileExists (cPathName, NULL)==0)
		MakeDir(cPathName);



	sprintf(cFileName,"\\%s_Trem_%04d%02d%02d_%02d%02d%02d.wav","SN",year,month,day,hour,min,sec);


	
	Single_tone_save(cPathName,cFileName,2,44100,16);
	 
	 

	
	uint8_t analysis_channel = 0; 
	double mic_sensetivity = Get_Mic_sensitivity(); 
	double sampleRate = 44100;  

  
	
	double detectedFrequency, detectedAmplitude, detectedTHD; 
		

		
	status = Single_tone_analysis(analysis_channel,mic_sensetivity, sampleRate,  &detectedFrequency, &detectedAmplitude,&detectedTHD);
	
	

	
	if(status != 0)
	{
		sprintf(msg,"AudioAnalysis Fail\n\n");
		WriteToTerminal(panel, msg);

		//stop tone
		status = DECT_Send_String (panel,AudioTremblerOff,AudioTremblerOff_Response,returned_string,6,1,1);

		if(status != 1)
		{
			sprintf(msg,"Send stop tone Fail\n\n");
			WriteToTerminal(panel, msg);
			return    0;
		}

		return    0;
	}

	//stop tone
	status = DECT_Send_String (panel,AudioTremblerOff,AudioTremblerOff_Response,returned_string,6,1,1);  
	
	if(status != 1)
	{
		sprintf(msg,"Send stop tone Fail\n\n");
		WriteToTerminal(panel, msg); 
		return    0;
	}
	
	g_dTremFreq =   detectedFrequency;
	g_dTremAmp =   detectedAmplitude;  
	g_dTremThd =   detectedTHD;  
		
		sprintf(msg,"Trembler Test finish. detectedFrequency %f; detectedAmplitude %f; detectedTHD %f\n\n",detectedFrequency,detectedAmplitude,detectedTHD); 
	WriteToTerminal(panel, msg);  


	if(strstr(msg,"NaN") != NULL)
	{
		sprintf(msg,"detected NaN\n\n");
		WriteToTerminal(panel, msg);

		if(iRepeat == 2)
		{
			sprintf(msg,"%f; %f; %f,",detectedFrequency,detectedAmplitude,detectedTHD);

			strcat(data_string,msg);
		}


		return    0;
	}



	if((detectedFrequency >= 350||detectedFrequency <= 120))
	{
		sprintf(msg,"detectedFrequency %f is out of range,limit 120 to 350\n\n",detectedFrequency);
		WriteToTerminal(panel, msg);

		if(iRepeat == 2)
		{
			sprintf(msg,"%f; %f; %f,",detectedFrequency,detectedAmplitude,detectedTHD);

			strcat(data_string,msg);
		}
		return    0;
	}
	
//	if((detectedAmplitude <= 4||detectedAmplitude >= 44)){
	//	if((detectedAmplitude <= 52||detectedAmplitude >= 135)){      //data set from fpe1
		if((detectedAmplitude <= 52||detectedAmplitude >= 135)){      //data set from MP  
			sprintf(msg,"detectedAmplitude %f is out of range, limit 52 to 135\n\n",detectedAmplitude);
			WriteToTerminal(panel, msg);

			if(iRepeat == 2)
			{
				sprintf(msg,"%f; %f; %f,",detectedFrequency,detectedAmplitude,detectedTHD);

				strcat(data_string,msg);
			}
			return    0;
		}
	
	/*if((detectedTHD >= 5)){
		sprintf(msg,"detectedTHD %f is out of range, limit < 5\n\n",detectedTHD);
		WriteToTerminal(panel, msg); 
		return    0;
	}  */

	
		sprintf(msg,"%f; %f; %f,",detectedFrequency,detectedAmplitude,detectedTHD); 
	
	strcat(data_string,msg);    

	

	sprintf(msg,"Trembler Test Pass. detectedFrequency %f; detectedAmplitude %f; detectedTHD %f\n\n",detectedFrequency,detectedAmplitude,detectedTHD); 
	WriteToTerminal(panel, msg);   
	return 1;
	
	

}


////////////////////////////////////////////////////////////////
//ENHANCED USER TESTS
////////////////////////////////////////////////////////////////

//*************************************************************
//* 
//*
//*************************************************************
int DECT_USB_Test(int panel) 
{
	int status = 0;
	char temp_buff[256] = {'\0'};
	int numToRead = 0;
	double Measured_Frequency;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	int counter = 0;
	
	
	sprintf(msg,"DECT USB Test\n");
	WriteToTerminal(panel, msg); 
	

	while((counter < 10) && (!(status == 1))){  
		status = DECT_Send_String (panel,USB,USB_Response,returned_string,0,1,5); 
		Delay(1);
		counter++;
	}
	

	sprintf(msg,"Remove USB Cable. Press OK when complete");
	Display_OK_MSG(msg); 
	Delay(1);
	
	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
	}


	return status;
}

//*************************************************************
//* 
//*
//*************************************************************
int DECT_LCD_RED_Test(int panel)
{
	int checkpass = 0;
	char temp_buff[256] = {'\0'};
	int numToRead = 0;
//	double Measured_Frequency;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	int counter = 0;


	sprintf(msg,"DECT LCD_RED Test\n");
	WriteToTerminal(panel, msg);



	while((counter < 3) && (!(checkpass == 1)))
	{
		sprintf(msg,"如果显示红色，按下PASS，否则按FAIL。\n\nPress PASS if the LCD screen turns RED, otherwise press FAIL.");
		Display_OK_MSG(msg);





		if( 0 == DECT_Send_String (panel,LCD_RED,LCD_RED_Response,returned_string,(LCD_RED_Response[0] + 1),1,1))
		{
			Delay(1);
			counter++;
			continue;
		}


		if((returned_string [(LCD_RED_Response[0] + 1)+15] == 82)&&(returned_string [(LCD_RED_Response[0] + 1)+16] == 69)&&(returned_string [(LCD_RED_Response[0] + 1)+17] == 68))   // found
		{
			sprintf(temp_buff,"LCD_RED = %c%c%c\n", returned_string [(LCD_RED_Response[0] + 1)+15],returned_string [(LCD_RED_Response[0] + 1)+16],returned_string [(LCD_RED_Response[0] + 1)+17]);
			WriteToTerminal(panel, temp_buff);
			checkpass = 1;
		}
		else
		{
			strcpy(returned_string,"");
			if(0 == DECT_Read_String (panel,(LCD_RED_Response_P2[0]+1),returned_string,&numToRead,1,20))
			{
				Delay(1);
				counter++;
				continue;
			}

			sprintf(temp_buff,"LCD_RED = %c%c%c\n", returned_string [15],returned_string [16],returned_string [17]);
			WriteToTerminal(panel, temp_buff);

			if((returned_string [15] == 82)&&(returned_string [16] == 69)&&(returned_string [17] == 68))
			{
				checkpass = 1;
			}
			else
			{
				checkpass = 0;
			}


		}




	



	



		counter++;
	}



	if(checkpass == 1)
	{
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg);
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else
	{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg);
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);
	}


	return checkpass;
}

   
   
//*************************************************************
//* 
//*
//*************************************************************
int DECT_LCD_GREEN_Test(int panel)
{
	int checkpass = 0;
	char temp_buff[256] = {'\0'};
	int numToRead = 0;
//	double Measured_Frequency;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	int counter = 0;


	sprintf(msg,"DECT LCD_GREEN Test\n");
	WriteToTerminal(panel, msg);



	while((counter < 3) && (!(checkpass == 1)))
	{

		sprintf(msg,"如果显示绿色，按下PASS，否则按FAIL。\n\nPress PASS if the LCD screen turns GREEN, otherwise press FAIL.");
		Display_OK_MSG(msg);



		if( 0 == DECT_Send_String (panel,LCD_GREEN,LCD_GREEN_Response,returned_string,(LCD_GREEN_Response[0] + 1),1,1))
		{
			Delay(1);
			counter++;
			continue;
		}


		if((returned_string [(LCD_GREEN_Response[0] + 1)+17] == 71)&&(returned_string [(LCD_GREEN_Response[0] + 1)+18] == 82)&&(returned_string [(LCD_GREEN_Response[0] + 1)+19] == 69)&&(returned_string [(LCD_GREEN_Response[0] + 1)+20] == 69)&&(returned_string [(LCD_GREEN_Response[0] + 1)+21] == 78))   // found
		{
			sprintf(temp_buff,"LCD_GREEN = %c%c%c%c%c\n", returned_string [(LCD_GREEN_Response[0] + 1)+17],returned_string [(LCD_GREEN_Response[0] + 1)+18],returned_string [(LCD_GREEN_Response[0] + 1)+19],returned_string [(LCD_GREEN_Response[0] + 1)+20],returned_string [(LCD_GREEN_Response[0] + 1)+21]);
			WriteToTerminal(panel, temp_buff);
			checkpass = 1;
		}
		else
		{
			strcpy(returned_string,"");
			if(0 == DECT_Read_String (panel,(LCD_GREEN_Response_P2[0]+1),returned_string,&numToRead,1,20))
			{
				Delay(1);
				counter++;
				continue;
			}


			sprintf(temp_buff,"LCD_GREEN = %c%c%c%c%c\n", returned_string [17],returned_string [18],returned_string [19],returned_string [20],returned_string [21]);
			WriteToTerminal(panel, temp_buff);


			if((returned_string [17] == 71)&&(returned_string [18] == 82)&&(returned_string [19] == 69)&&(returned_string [20] == 69)&&(returned_string [21] == 78))
			{
				checkpass = 1;
			}
			else
			{
				checkpass = 0;
			}

		}


	


		counter++;
	}




	if(checkpass == 1)
	{
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg);
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else
	{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg);
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);
	}


	return checkpass;
}



//*************************************************************
//*
//*
//*************************************************************
int DECT_LCD_BLUE_Test(int panel)
{
	int checkpass = 0;
	char temp_buff[256] = {'\0'};
	int numToRead = 0;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	int counter = 0;


	sprintf(msg,"DECT LCD_BLUE Test\n");
	WriteToTerminal(panel, msg);


	while((counter < 3) && (!(checkpass == 1)))
	{
		sprintf(msg,"如果显示蓝色，按下PASS，否则按FAIL。\n\nPress PASS if the LCD screen turns BLUE, otherwise press FAIL.");
		Display_OK_MSG(msg);


		if( 0 == DECT_Send_String (panel,LCD_BLUE,LCD_BLUE_Response,returned_string,(LCD_BLUE_Response[0]+1),1,1))
		{
			Delay(1);
			counter++;
			continue;
		}

		if((returned_string [(LCD_BLUE_Response[0]+1)+16] == 66)&&(returned_string [(LCD_BLUE_Response[0]+1)+17] == 76)&&(returned_string [(LCD_BLUE_Response[0]+1)+18] == 85)&&(returned_string [(LCD_BLUE_Response[0]+1)+19] == 69))   // found
		{
			sprintf(temp_buff,"LCD_BLUE = %c%c%c%c\n", returned_string [(LCD_BLUE_Response[0]+1)+16],returned_string [(LCD_BLUE_Response[0]+1)+17],returned_string [(LCD_BLUE_Response[0]+1)+18],returned_string [(LCD_BLUE_Response[0]+1)+19]);
			WriteToTerminal(panel, temp_buff);
			checkpass = 1;    
		}
		else
		{
			strcpy(returned_string,"");
			if(0 == DECT_Read_String (panel,(LCD_BLUE_Response_P2[0]+1),returned_string,&numToRead,1,20))
			{
				Delay(1);
				counter++;
				continue;
			}



			sprintf(temp_buff,"LCD_BLUE = %c%c%c%c\n", returned_string [16],returned_string [17],returned_string [18],returned_string [19]);
			WriteToTerminal(panel, temp_buff);


			if((returned_string [16] == 66)&&(returned_string [17] == 76)&&(returned_string [18] == 85)&&(returned_string [19] == 69))
			{
				checkpass = 1;
			}
			else
			{
				checkpass = 0;
			}
		}






		counter++;



	}


	if(checkpass == 1)
	{
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg);
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else
	{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg);
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);
	}


	return checkpass;
}



//*************************************************************
//* 
//*
//*************************************************************
int DECT_LCD_POS_Test(int panel)
{
	int checkpass = 0;
	char temp_buff[256] = {'\0'};
	int numToRead = 0;
//	double Measured_Frequency;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	int counter = 0;


	sprintf(msg,"DECT LCD_POS Test\n");
	WriteToTerminal(panel, msg);



	while((counter < 3) && (!(checkpass == 1)))
	{
		sprintf(msg,"如果居中显示，按下PASS，否则按FAIL。\n\nPress PASS if the LCD screen appears centered, otherwise press FAIL.");
		Display_OK_MSG(msg);
		
		
		if( 0 == DECT_Send_String (panel,LCD_POS,LCD_POS_Response,returned_string,(LCD_POS_Response[0] + 1),1,1))
		{
			Delay(1);
			counter++;
			continue;
		}

		if((returned_string [(LCD_POS_Response[0] + 1)+15] == 80)&&(returned_string [(LCD_POS_Response[0] + 1)+16] == 79)&&(returned_string [(LCD_POS_Response[0] + 1)+17] == 83))   // found
		{
				sprintf(temp_buff,"LCD_POS = %c%c%c\n", returned_string [(LCD_POS_Response[0] + 1)+15],returned_string [(LCD_POS_Response[0] + 1)+16],returned_string [(LCD_POS_Response[0] + 1)+17]);
			WriteToTerminal(panel, temp_buff);
			checkpass = 1;
		}
		else
		{
			strcpy(returned_string,"");
			if(0 == DECT_Read_String (panel,(LCD_POS_Response_P2[0]+1),returned_string,&numToRead,1,20))   //13 E6 00 C4 F1 82 04 4C 43 44 2D 50 4F 53 3A 50 4F 53 00 EF 0F E6 00 C4 F1 01 00 4C 43 44 2D 50 4F 53 00 1E
			{
				Delay(1);
				counter++;
				continue;
			}


			sprintf(temp_buff,"LCD_POS = %c%c%c\n", returned_string [15],returned_string [16],returned_string [17]);
			WriteToTerminal(panel, temp_buff);

			if((returned_string [15] == 80)&&(returned_string [16] == 79)&&(returned_string [17] == 83))
			{
				checkpass = 1;
			}
			else
			{
				checkpass = 0;
			}
		}


	

		counter++;
	}



	if(checkpass == 1)   {
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg);
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else
	{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg);
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);
	}


	return checkpass;
}




//*************************************************************
//* 
//*
//*************************************************************
int DECT_LED_RED_Test(int panel) 
{
	int checkpass = 0;
	char temp_buff[256] = {'\0'};
	int numToRead = 0;
//	double Measured_Frequency;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	int counter = 0;
	
	
	sprintf(msg,"DECT LED_RED Test\n");
	WriteToTerminal(panel, msg); 
	
	
	
	while((counter < 3) && (!(checkpass == 1))){ 
		
		sprintf(msg,"如果红灯亮，按下PASS，否则按FAIL。\n\nPress PASS if the RED LED turns on, otherwise press FAIL.");
	Display_OK_MSG(msg); 
	

		
		
			if( 0 == DECT_Send_String (panel,LED_RED,LED_RED_Response,returned_string,(LED_RED_Response[0]+1),1,1))
		{
			Delay(1);
			counter++;
			continue;
		}



		if((returned_string [(LED_RED_Response[0]+1)+15] == 82)&&(returned_string [(LED_RED_Response[0]+1)+16] == 101)&&(returned_string [(LED_RED_Response[0]+1)+17] == 100))   // found
		{
			sprintf(temp_buff,"LED_RED = %c%c%c\n", returned_string [(LED_RED_Response[0]+1)+15],returned_string [(LED_RED_Response[0]+1)+16],returned_string [(LED_RED_Response[0]+1)+17]);
			WriteToTerminal(panel, temp_buff);
			checkpass = 1;
		}
		else
		{
			strcpy(returned_string,"");
			if(0 == DECT_Read_String (panel,(LED_RED_Response_P2[0]+1),returned_string,&numToRead,1,20))
			{
				Delay(1);
				counter++;
				continue;
			}
			


			sprintf(temp_buff,"LED_RED = %c%c%c\n", returned_string [15],returned_string [16],returned_string [17]);
			WriteToTerminal(panel, temp_buff);


			if((returned_string [15] == 82)&&(returned_string [16] == 101)&&(returned_string [17] == 100))
			{
				checkpass = 1;
			}
			else
			{
				checkpass = 0;
			}
		}






		counter++;
	}
	

	if(checkpass == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg);
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);
	}


	return checkpass;
}



//*************************************************************
//* 
//*
//*************************************************************
int DECT_LED_GREEN_Test(int panel)
{
	int checkpass = 0;
	char temp_buff[256] = {'\0'};
	int numToRead = 0;
//	double Measured_Frequency;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	int counter = 0;


	sprintf(msg,"DECT LED_GREEN Test\n");
	WriteToTerminal(panel, msg);



	while((counter < 3) && (!(checkpass == 1)))
	{

		sprintf(msg,"如果绿灯亮，按下PASS，否则按FAIL。\n\nPress PASS if the GREEN LED turns on, otherwise press FAIL.");
		Display_OK_MSG(msg);



		if( 0 == DECT_Send_String (panel,LED_GREEN,LED_GREEN_Response,returned_string,(LED_GREEN_Response[0] + 1),1,1))
		{
			Delay(1);
			counter++;
			continue;
		}

		if((returned_string [(LED_GREEN_Response[0] + 1)+17] == 71)&&(returned_string [(LED_GREEN_Response[0] + 1)+18] == 114)&&(returned_string [(LED_GREEN_Response[0] + 1)+19] == 101)&&(returned_string [(LED_GREEN_Response[0] + 1)+20] == 101)&&(returned_string [(LED_GREEN_Response[0] + 1)+21] == 110))   // found
		{
				sprintf(temp_buff,"LED_GREEN = %c%c%c%c%c\n", returned_string [(LED_GREEN_Response[0] + 1)+17],returned_string [(LED_GREEN_Response[0] + 1)+18],returned_string [(LED_GREEN_Response[0] + 1)+19],returned_string [(LED_GREEN_Response[0] + 1)+20],returned_string [(LED_GREEN_Response[0] + 1)+21]);
			WriteToTerminal(panel, temp_buff);
			checkpass = 1;
		}
		else
		{
			strcpy(returned_string,"");
			if(0 == DECT_Read_String (panel,(LED_GREEN_Response_P2[0]+1),returned_string,&numToRead,1,20))
			{
				Delay(1);
				counter++;
				continue;
			}


			sprintf(temp_buff,"LED_GREEN = %c%c%c%c%c\n", returned_string [17],returned_string [18],returned_string [19],returned_string [20],returned_string [21]);
			WriteToTerminal(panel, temp_buff);


			if((returned_string [17] == 71)&&(returned_string [18] == 114)&&(returned_string [19] == 101)&&(returned_string [20] == 101)&&(returned_string [21] == 110))
			{
				checkpass = 1;
			}
			else
			{
				checkpass = 0;
			}

		}


	
		counter++;
	}



	if(checkpass == 1)
	{
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg);
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else
	{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg);
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);
	}


	return checkpass;
}



//*************************************************************
//* 
//*
//*************************************************************
int DECT_LED_ORANGE_Test(int panel)
{
	int checkpass = 0;
	char temp_buff[256] = {'\0'};
	int numToRead = 0;
//	double Measured_Frequency;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	int counter = 0;


	sprintf(msg,"DECT LED_ORANGE Test\n");
	WriteToTerminal(panel, msg);



	while((counter < 3) && (!(checkpass == 1)))
	{
		sprintf(msg,"如果橙灯亮，按下PASS，否则按FAIL。\n\nPress PASS if the ORANGE LED turns on, otherwise press FAIL.");
		Display_OK_MSG(msg);



		if( 0 == DECT_Send_String (panel,LED_ORANGE ,LED_ORANGE_Response,returned_string,(LED_ORANGE_Response[0]+1),1,1))
		{
			Delay(1);
			counter++;
			continue;
		}


		if((returned_string [(LED_ORANGE_Response[0]+1)+18] == 79)&&(returned_string [(LED_ORANGE_Response[0]+1)+19] == 114)&&(returned_string [(LED_ORANGE_Response[0]+1)+20] == 97)&&(returned_string [(LED_ORANGE_Response[0]+1)+21] == 110)&&(returned_string [(LED_ORANGE_Response[0]+1)+22] == 103)&&(returned_string [(LED_ORANGE_Response[0]+1)+23] == 101))   // found
		{
				sprintf(temp_buff,"LED_ORANGE = %c%c%c%c%c%c\n", returned_string [(LED_ORANGE_Response[0]+1)+18],returned_string [(LED_ORANGE_Response[0]+1)+19],returned_string [(LED_ORANGE_Response[0]+1)+20],returned_string [(LED_ORANGE_Response[0]+1)+21],returned_string [(LED_ORANGE_Response[0]+1)+22],returned_string [(LED_ORANGE_Response[0]+1)+23]);
			WriteToTerminal(panel, temp_buff);

			checkpass = 1;
		}
		else
		{
			strcpy(returned_string,"");
			if(0 == DECT_Read_String (panel,(LED_ORANGE_Response_P2[0]+1),returned_string,&numToRead,1,20))
			{
				Delay(1);
				counter++;
				continue;
			}


			sprintf(temp_buff,"LED_ORANGE = %c%c%c%c%c%c\n", returned_string [18],returned_string [19],returned_string [20],returned_string [21],returned_string [22],returned_string [23]);
			WriteToTerminal(panel, temp_buff);



			if((returned_string [18] == 79)&&(returned_string [19] == 114)&&(returned_string [20] == 97)&&(returned_string [21] == 110)&&(returned_string [22] == 103)&&(returned_string [23] == 101))
			{
				checkpass = 1;
			}
			else
			{
				checkpass = 0;
			}
		}





		counter++;
	}



	if(checkpass == 1)
	{
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg);
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else
	{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg);
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);
	}


	return checkpass;
}


//*************************************************************
//* 
//*
//*************************************************************
int DECT_LED_KEYB_Test(int panel) 
{
	int checkpass = 0;
	char temp_buff[256] = {'\0'};
	int numToRead = 0;
//	double Measured_Frequency;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	int counter = 0;
	
	
	
	sprintf(msg,"DECT LED_KEYB Test\n");
	WriteToTerminal(panel, msg); 
	
	

	while((counter < 3) && (!(checkpass == 1)))
	{
		sprintf(msg,"如果按键灯亮，按PASS，否则按FAIL。\n\nPress PASS if the KEYBOARD LED turns on, otherwise press FAIL.");
		Display_OK_MSG(msg);
		if(0 == DECT_Send_String (panel,LED_KEYB,LED_KEYB_Response,returned_string,(LED_KEYB_Response[0] +1),1,1))
		{
			Delay(1);
			counter++;
			continue;
		}

		if((returned_string [(LED_KEYB_Response[0] +1)+16] == 75)&&(returned_string [(LED_KEYB_Response[0] +1)+17] == 101)&&(returned_string [(LED_KEYB_Response[0] +1)+18] == 121)&&(returned_string [(LED_KEYB_Response[0] +1)+19] == 98))   // found
		{
				sprintf(temp_buff,"LED_KEYB = %c%c%c%c\n", returned_string [(LED_KEYB_Response[0] +1)+16],returned_string [(LED_KEYB_Response[0] +1)+17],returned_string [(LED_KEYB_Response[0] +1)+18],returned_string [(LED_KEYB_Response[0] +1)+19]);
			WriteToTerminal(panel, temp_buff);
			checkpass = 1;
		}
		else
		{
			strcpy(returned_string,"");
			if(0 == DECT_Read_String (panel,(LED_KEYB_Response_P2[0] + 1),returned_string,&numToRead,1,20)) //15 E6 00 C4 F1 82 00 4C 45 44 2D 4B 45 59 42 3A 4B 65 79 62 00 CD 10 E6 00 C4 F1 02 00 4C 45 44 2D 4B 45 59 42 00 42
			{
				Delay(1);
				counter++;
				continue;
			}


			sprintf(temp_buff,"LED_KEYB = %c%c%c%c\n", returned_string [16],returned_string [17],returned_string [18],returned_string [19]);
			WriteToTerminal(panel, temp_buff);


			if((returned_string [16] == 75)&&(returned_string [17] == 101)&&(returned_string [18] == 121)&&(returned_string [19] == 98))
			{
				checkpass = 1;
			}
			else
			{
				checkpass = 0;
			}
		}


		counter++;
	}

	
	

	
	
	if(checkpass == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);
	}


	return checkpass;
}


//*************************************************************
//* 
//*
//*************************************************************
int DECT_SDCARD_Test(int panel) 
{
	int status = 0;
	char temp_buff[256] = {'\0'};
	int numToRead = 0;
	double Measured_Frequency;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	int counter = 0;
	
	
	sprintf(msg,"DECT SD Card Test\n");
	WriteToTerminal(panel, msg); 
	
	//sprintf(msg,"SD Card is required to be installed before the phone is powered on.");
	//Display_OK_MSG(msg); 
	
	while((counter < 10) && (!(status == 1))){  
		status = DECT_Send_String (panel,SDCard,SDCard_Response,returned_string,-1,1,5);  
		Delay(1);
		counter++;
	}
	
	sprintf(temp_buff,"SDC = %c%c\n", returned_string [11],returned_string [12]);
	WriteToTerminal(panel, temp_buff);
	
	if((returned_string [11] == 97)&&(returned_string [12] == 118)){
		status = 1;
	}
	else{
		status = 0;
	}
	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg);
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);
	}


	return status;
}

//*************************************************************
//* 
//*
//*************************************************************
int DECT_KEY12_Test(int panel) 
{
	int checkpass = 0;
	char temp_buff[256] = {'\0'};
	int numToRead = 0;
//	double Measured_Frequency;
	char msg[256] = {'\0'};
	char returned_string[512] = {'\0'};
	int counter = 0;
	
	
	sprintf(msg,"DECT KEY12 Test\n");
	WriteToTerminal(panel, msg); 
	

	
	while((counter < 10) && (!(checkpass == 1)))
	{
		sprintf(msg,"按下LCD上面显示的所有按键。\n\nPress all KEYS as shown on the LCD display.");
		Display_OK_MSG(msg); 
		
		
		
			if( 0 == DECT_Send_String (panel,KEY12,KEY12_Response,returned_string,(KEY12_Response[0]+1),1,1))
		{
			Delay(1);
			counter++;
			continue;
		}


		strcpy(returned_string,"");  
		if(0 == DECT_Read_String (panel,14*26,returned_string,&numToRead,1,40))
		{
			Delay(1);
			counter++;
			continue;
		}

		if(((returned_string [355] == 0x02)&&(returned_string [356] == 0x12))||((returned_string [369] == 0x02)&&(returned_string [370] == 0x12))||((returned_string [383] == 0x02)&&(returned_string [384] == 0x12)))
		{
			checkpass = 1;
			if((returned_string [355] == 0x02)&&(returned_string [356] == 0x12))
			{
				sprintf(temp_buff,"KEY12 = 0x%x, 0x%x\n", returned_string [355],returned_string [356]);
				WriteToTerminal(panel, temp_buff);
			}
			else if((returned_string [369] == 0x02)&&(returned_string [370] == 0x12))
			{
				sprintf(temp_buff,"KEY12 = 0x%x, 0x%x\n", returned_string [369],returned_string [370]);
				WriteToTerminal(panel, temp_buff);
			}
			else if((returned_string [383] == 0x02)&&(returned_string [384] == 0x12))
			{
				sprintf(temp_buff,"KEY12 = 0x%x, 0x%x\n", returned_string [383],returned_string [384]);
				WriteToTerminal(panel, temp_buff);
			}

		}
	
		counter++;   
	}
	

	
	if(checkpass == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg);
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);
	}


	return checkpass;
}



//*************************************************************
//*
//*
//*************************************************************
int DECT_KEY22_Test(int panel)
{
	int checkpass = 0;
	char temp_buff[256] = {'\0'};
	int numToRead = 0;
//	double Measured_Frequency;
	char msg[256] = {'\0'};
	char returned_string[512] = {'\0'};
	int counter = 0;


	sprintf(msg,"DECT KEY22 Test\n");
	WriteToTerminal(panel, msg);



	while((counter < 3) && (!(checkpass == 1)))
	{
		sprintf(msg,"按下LCD上面显示的所有按键。\n\nPress all KEYS as shown on the LCD display.");
		Display_OK_MSG(msg);


		if( 0 == DECT_Send_String (panel,KEY22,KEY22_Response,returned_string,(KEY22_Response[0]+1),1,1))
		{
			Delay(1);
			counter++;
			continue;
		}


		strcpy(returned_string,"");  
		if(0 == DECT_Read_String (panel,14*28,returned_string,&numToRead,1,40))
		{
			Delay(1);
			counter++;
			continue;
		}
		
			if(((returned_string [383] == 0x02)&&(returned_string [384] == 0x22))||((returned_string [397] == 0x02)&&(returned_string [398] == 0x22))||((returned_string [411] == 0x02)&&(returned_string [412] == 0x22)))
	{
		checkpass = 1;
		if((returned_string [383] == 0x02)&&(returned_string [384] == 0x22))
		{
			sprintf(temp_buff,"KEY22 = 0x%x, 0x%x\n", returned_string [383],returned_string [384]);
			WriteToTerminal(panel, temp_buff);
		}
		else if((returned_string [397] == 0x02)&&(returned_string [398] == 0x22))
		{
			sprintf(temp_buff,"KEY22 = 0x%x, 0x%x\n", returned_string [397],returned_string [398]);
			WriteToTerminal(panel, temp_buff);
		}
		else if((returned_string [411] == 0x02)&&(returned_string [412] == 0x22))
		{
			sprintf(temp_buff,"KEY22 = 0x%x, 0x%x\n", returned_string [411],returned_string [412]);
			WriteToTerminal(panel, temp_buff);
		}
	}

		counter++;
	}



	

	if(checkpass == 1)
	{
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg);
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else
	{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg);
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);
	}


	return checkpass;
}



//*************************************************************
//* 
//*
//*************************************************************
int DECT_KEY32_Test(int panel)
{
	int checkpass = 0;
	char temp_buff[256] = {'\0'};
	int numToRead = 0;
	//double Measured_Frequency;
	char msg[256] = {'\0'};
	char returned_string[512] = {'\0'};
	int counter = 0;
	


	sprintf(msg,"DECT KEY32 Test\n");
	WriteToTerminal(panel, msg);




	while((counter < 3) && (!(checkpass == 1)))
	{
		sprintf(msg,"按下LCD上面显示的所有按键。\n\nPress all KEYS as shown on the LCD display.");
		Display_OK_MSG(msg);
	

		if( 0 == DECT_Send_String (panel,KEY32,KEY32_Response,returned_string,(KEY32_Response[0]+1),1,1))
		{
			Delay(1);
			counter++;
			continue;
		}


		strcpy(returned_string,"");  
		if(0 == DECT_Read_String (panel,14*29,returned_string,&numToRead,1,40))
		{
			Delay(1);
			counter++;
			continue;
		}

		if(((returned_string [397] == 0x02)&&(returned_string [398] == 0x32))||((returned_string [411] == 0x02)&&(returned_string [412] == 0x32))||((returned_string [425] == 0x02)&&(returned_string [426] == 0x32)))
		{
			checkpass = 1;
			if((returned_string [397] == 0x02)&&(returned_string [398] == 0x32))
			{
				sprintf(temp_buff,"KEY32 = 0x%x, 0x%x\n", returned_string [397],returned_string [398]);
				WriteToTerminal(panel, temp_buff);
			}
			else if((returned_string [411] == 0x02)&&(returned_string [412] == 0x32))
			{
				sprintf(temp_buff,"KEY32 = 0x%x, 0x%x\n", returned_string [411],returned_string [412]);
				WriteToTerminal(panel, temp_buff);
			}
			else if((returned_string [425] == 0x02)&&(returned_string [426] == 0x32))
			{
				sprintf(temp_buff,"KEY32 = 0x%x, 0x%x\n", returned_string [425],returned_string [426]);
				WriteToTerminal(panel, temp_buff);
			}

		}
	
		counter++;    


	}


	if(checkpass == 1)
	{
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg);
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else
	{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg);
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);
	}


	return checkpass;
}


////////////////////////////////////////////////////////////////
//ENHANCED VERSION TESTS
////////////////////////////////////////////////////////////////


//*************************************************************
//* 
//*
//*************************************************************
int DECT_VERS_SW_Test(int panel) 
{
	int status = 0;
	char temp_buff[256] = {'\0'};
	int numToRead = 0;
	double Measured_Frequency;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	int counter = 0;
	
	
	sprintf(msg,"DECT VERS_SW Test\n");
	WriteToTerminal(panel, msg); 
	
	while((counter < 10) && (!(status == 1))){  
		status = DECT_Send_String (panel,VERS_SW,VERS_SW_Response,returned_string,0,1,1);  
		Delay(1);
		counter++;
	}
	
	sprintf(temp_buff,"VERS_SW = %c%c%c%c%c\n", returned_string [15],returned_string [16],returned_string [17],returned_string [18],returned_string [19]);
	WriteToTerminal(panel, temp_buff);
	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
	}


	return status;
}


//*************************************************************
//* 
//*
//*************************************************************
int DECT_VERS_LOADER_Test(int panel) 
{
	int status = 0;
	char temp_buff[256] = {'\0'};
	int numToRead = 0;
	double Measured_Frequency;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	int counter = 0;
	
	
	sprintf(msg,"DECT VERS_LOADER Test\n");
	WriteToTerminal(panel, msg); 
	
	while((counter < 10) && (!(status == 1))){  
		status = DECT_Send_String (panel,VERS_LOADER,VERS_LOADER_Response ,returned_string,0,1,1);  
		Delay(1);
		counter++;
	}
	
	sprintf(temp_buff,"VERS_LOADER = %c%c%c%c\n", returned_string [19],returned_string [20],returned_string [21],returned_string [22]);
	WriteToTerminal(panel, temp_buff);
	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
	}


	return status;
}



//*************************************************************
//* 
//*
//*************************************************************
int DECT_VERS_HW_Test(int panel) 
{
	int status = 0;
	char temp_buff[256] = {'\0'};
	int numToRead = 0;
	double Measured_Frequency;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	int counter = 0;
	
	
	sprintf(msg,"DECT VERS_HW Test\n");
	WriteToTerminal(panel, msg); 
	
	while((counter < 10) && (!(status == 1))){  
		status = DECT_Send_String (panel,VERS_HW,VERS_HW_Response ,returned_string,0,1,1);  
		Delay(1);
		counter++;
	}
	
	sprintf(temp_buff,"HEADSET = %c%c%c%c%c%c%c%c%c%c%c\n", returned_string [15],returned_string [16],returned_string [17],returned_string [18],returned_string [19],returned_string [20],returned_string [21],returned_string [22],returned_string [23],returned_string [24],returned_string [25]);
	WriteToTerminal(panel, temp_buff);
	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
	}


	return status;
}


//*************************************************************
//* 
//*
//*************************************************************
int DECT_VERS_BRAND_Test(int panel) 
{
	int status = 0;
	char temp_buff[256] = {'\0'};
	int numToRead = 0;
	double Measured_Frequency;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	int counter = 0;
	
	
	sprintf(msg,"DECT VERS_BRAND Test\n");
	WriteToTerminal(panel, msg); 
	
	while((counter < 10) && (!(status == 1))){  
		status = DECT_Send_String (panel,VERS_BRAND,VERS_BRAND_Response ,returned_string,0,1,1);  
		Delay(1);
		counter++;
	}
	
	sprintf(temp_buff,"VERS_BRAND = %c%c\n", returned_string [18],returned_string [19]);
	WriteToTerminal(panel, temp_buff);
	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
	}


	return status;
}



//*************************************************************
//* 
//*
//*************************************************************
int DECT_VERS_ATS_Test(int panel) 
{
	int status = 0;
	char temp_buff[256] = {'\0'};
	int numToRead = 0;
	double Measured_Frequency;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	int counter = 0;
	
	
	sprintf(msg,"DECT VERS_ATS Test\n");
	WriteToTerminal(panel, msg); 
	
	while((counter < 10) && (!(status == 1))){  
		status = DECT_Send_String (panel,VERS_ATS,VERS_ATS_Response ,returned_string,0,1,1);  
		Delay(1);
		counter++;
	}
	
	sprintf(temp_buff,"VERS_ATS = %c%c%c\n", returned_string [16],returned_string [17],returned_string [18]);
	WriteToTerminal(panel, temp_buff);
	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
	}


	return status;
}




//*************************************************************
//* 
//*
//*************************************************************
int DECT_VERS_IP65_Test(int panel) 
{
	int status = 0;
	char temp_buff[256] = {'\0'};
	int numToRead = 0;
	double Measured_Frequency;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	int counter = 0;
	
	
	sprintf(msg,"DECT VERS_IP65 Test\n");
	WriteToTerminal(panel, msg); 
	
	while((counter < 10) && (!(status == 1))){  
		status = DECT_Send_String (panel,VERS_IP65,VERS_IP65_Response ,returned_string,0,1,1);  
		Delay(1);
		counter++;
	}
	
	sprintf(temp_buff,"VERS_IP65 = %c%c\n", returned_string [17],returned_string [18]);
	WriteToTerminal(panel, temp_buff);
	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
	}


	return status;
}



//*************************************************************
//* 
//*
//*************************************************************
int DECT_VERS_SNO_Test(int panel) 
{
	int status = 0;
	char temp_buff[256] = {'\0'};
	int numToRead = 0;
	double Measured_Frequency;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	int counter = 0;
	
	
	sprintf(msg,"DECT VERS_SNO Test\n");
	WriteToTerminal(panel, msg); 
	
	//while((counter < 10) && (!(status == 1))){  
	//	status = DECT_Send_String (panel,VERS_SNO,VERS_SNO_Response ,returned_string,-1,1,1);  
	//	Delay(1);
	//	counter++;
	//}
	
	while((counter < 10) && (!(status == 1))){  
		status = DECT_Send_String (panel,ReadVendorSN,ReadVendorSN_Response,returned_string,-1,1,1);  
		Delay(1);
		counter++;
	}
	
	
	/*sprintf(temp_buff,"VERS_SNO = %c%c%c%c%c\n", returned_string [16],returned_string [17],returned_string [25],returned_string [26],returned_string [27]);
	WriteToTerminal(panel, temp_buff);
	
	if ((returned_string [16] == 74)&&(returned_string [17] == 87)&&(returned_string [25] == Product_Serial_Global [12])&&(returned_string [26] == Product_Serial_Global [13])&&(returned_string [27] == Product_Serial_Global [14])){
	 status = 1;
	}
	else{
		status = 0;
	}*/
	
	
	
	sprintf(temp_buff,"VERS_SNO = %c%c%c%c%c\n", returned_string [8],returned_string [9],returned_string [10],returned_string [11],returned_string [12]);
	WriteToTerminal(panel, temp_buff);
	
	if ((returned_string [8] == 74)&&(returned_string [9] == 87)&&(returned_string [10] == Product_Serial_Global [12])&&(returned_string [11] == Product_Serial_Global [13])&&(returned_string [12] == Product_Serial_Global [14])){
	 status = 1;
	}
	else{
		status = 0;
		sprintf(temp_buff,"Expected_SNO = JW%c%c%c\n", Product_Serial_Global [12], Product_Serial_Global [13], Product_Serial_Global [14]);
		WriteToTerminal(panel, temp_buff);
	}
	
	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
	}


	return status;
}



//*************************************************************
//* 
//*
//*************************************************************
int DECT_VERS_FINAL_Test(int panel) 
{
	int status = 0;
	char temp_buff[256] = {'\0'};
	int numToRead = 0;
	double Measured_Frequency;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	int counter = 0;
	
	
	sprintf(msg,"DECT VERS_FINAL Test\n");
	WriteToTerminal(panel, msg); 
	
	while((counter < 10) && (!(status == 1))){  
		status = DECT_Send_String (panel,VERS_FINAL,VERS_FINAL_Response ,returned_string,0,1,1);  
		Delay(1);
		counter++;
	}
	
	sprintf(temp_buff,"VERS_FINAL = %c%c%c%c%c%c%c%c%c%c\n", returned_string [18],returned_string [19],returned_string [20],returned_string [21],returned_string [22],returned_string [23],returned_string [24],returned_string [25],returned_string [26],returned_string [27]);
	WriteToTerminal(panel, temp_buff);
	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
	}


	return status;
}



//*************************************************************
//* 
//*
//*************************************************************
int DECT_VERS_IPEI_Test(int panel) 
{
	int status = 0;
	char temp_buff[256] = {'\0'};
	int numToRead = 0;
	double Measured_Frequency;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	int counter = 0;
	int i = 0;
	int n = 0;
	char local_IPEI[24] = {'\0'};
	
	
	sprintf(msg,"DECT Verify IPEI Test\n");
	WriteToTerminal(panel, msg); 
	
	while((counter < 10) && (!(status == 1))){  
		status = DECT_Send_String (panel,VERS_IPEI,VERS_IPEI_Response ,returned_string,-1,1,1);  
		Delay(1);
		counter++;
	}
	
	sprintf(temp_buff,"READ IPEI = %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n", returned_string [17],returned_string [18],returned_string [19],returned_string [20],returned_string [21],returned_string [22],returned_string [23],returned_string [24],returned_string [25],returned_string [26],returned_string [27],returned_string [28],returned_string [29],returned_string [30],returned_string [31]);
	WriteToTerminal(panel, temp_buff);
	
	
	if(long_form_final == 1){  
	
	n = 0;
	
	for (i = 17;i <= 31; i++){
		if(returned_string [i] != 32){
			local_IPEI[n] = returned_string [i];
			n++;
		}
	}
	

		status = 1;

		for(i = 0;i <= 12; i++){
			if((local_IPEI[i] != IPEI_Global[i])){
				status = 0;
			}
	
		}
		
	sprintf(temp_buff,"EXPECTED IPEI = %c%c%c%c%c %c%c%c%c%c%c%c %c\n", IPEI_Global [0],IPEI_Global [1],IPEI_Global[2],IPEI_Global[3],IPEI_Global[4],IPEI_Global[5],IPEI_Global[6],IPEI_Global[7],IPEI_Global[8],IPEI_Global[9],IPEI_Global[10],IPEI_Global[11],IPEI_Global[12]);
	WriteToTerminal(panel, temp_buff);
	
	}
	
	
	//sprintf(msg,"%c%c%c%c%c%c%c%c%c%c%c%c%c,",local_IPEI[0],local_IPEI[1],local_IPEI[2],local_IPEI[3],local_IPEI[4],local_IPEI[5],local_IPEI[6],local_IPEI[7],local_IPEI[8],local_IPEI[9],local_IPEI[10],local_IPEI[11],local_IPEI[12]);
	//strcat(data_string,msg);
	
	sprintf(msg,"%s,",local_IPEI);
	strcat(data_string,msg);
	
	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);
	}


	return status;
}


////
//BT Stuff
//*************************************************************
//* 
//*
//*************************************************************

int Clear_BT_Tester(void)
{
   int status; 
   
   //to bring MT8852B to initial state after power on, you send the following   
   //send interface clear to the instrument
   ibwrt (MT8852B_HANDLE, "IFC", strlen("IFC"));
   if (ibsta & 0x8000) status = 0;
   
   //send device clear to the instrunet
   ibwrt (MT8852B_HANDLE, "DCL", strlen("DCL")); 
   if (ibsta & 0x8000) status = 0;
   
    //clear the device
   ibclr (MT8852B_HANDLE);
   if (ibsta & 0x8000) status = 0;  
   
   //clear status of the instrument
   ibwrt (MT8852B_HANDLE, "*CLS", strlen("*CLS"));
   if (ibsta & 0x8000) status = 0;
   
   //reset the instrunet
   ibwrt (MT8852B_HANDLE, "*RST", strlen("*RST")); 
   if (ibsta & 0x8000) status = 0; 
   
   return 1;
	
}

//*************************************************************
//* 
//*
//*************************************************************

int Check_Sync_BT_Tester(void)
{
	int counter = 0;
	char temp_buff[24] = {'\0'}; 
	int status = 0;
	int numToRead;
	char returned_string[256] = {'\0'}; 
	
	while((counter < 10) && (!(strstr (temp_buff, "!NO ERRORS!NO ERRORS!"))))
	{
			
   		ibwrt (MT8852B_HANDLE, "ERRLST", strlen("ERRLST"));
   		if (ibsta & 0x8000) status = 0;
	
		Delay (1);

		numToRead = 200;
		ibrd (MT8852B_HANDLE, returned_string, numToRead);
   		if (ibsta & 0x8000) status = 0; 
		
		//sprintf(msg,".");
		//WriteToTerminal(panel, msg); 
		ProcessSystemEvents (); 
			
		counter ++;

	} 
	
	if(strstr (temp_buff, "!NO ERRORS!NO ERRORS!")){
		status = 1;
	}


	return 1;
}


//*************************************************************
//* 
//*
//*************************************************************
int DECT_Disable_Bluetooth_USB_Test(int panel)
{

	int status = 0;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'}; 
	int counter = 0;

	
	sprintf(msg,"DECT Disable Bluetooth Test\n");
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg); 
	WriteToTerminal(panel, msg); 
	
	while((counter < 20) && (!(status == 1))){
		status = DECT_Send_String_USB (panel,BT_DISABLE,BT_DISABLE_Response,returned_string,0,1,1);	 
		counter ++;
	}
	
	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);	
	}
	
	return status;
}



///////////////////////////////////////////////////////////////////

//*************************************************************
//* 
//*
//*************************************************************
int DECT_Write_Serial_Number_Final_Test(int panel) 
{
	unsigned char EepromSerialNumber[17] = {'\0'}; 
	char nible1;
	char nible2; 
	int temp_int, temp_int_2;
	int status = 0;
	char msg[256];
	char temp_char[24] = {'\0'};
	char temp_char_2[24] = {'\0'};
	int x;
	char returned_string[256] = {'\0'};
	int counter = 0;
	
	
	sprintf(msg,"DECT Write Serial Number Test\n"); 
	//SetCtrlVal (panel, PANEL_Serial_Window_1, msg); 
	WriteToTerminal(panel, msg); 
	
	for (x=0; x < 17; x++)
    {
    	EepromSerialNumber[x] = WriteEepromSerialNumber[x];
    }
	

	EepromSerialNumber[11] =  0x4A;  
	
	EepromSerialNumber[12] =  0x57;
	
	EepromSerialNumber[13] =  Product_Serial_Global [12]; 
	
	EepromSerialNumber[14] =  Product_Serial_Global [13];	
	
	EepromSerialNumber[15] =  Product_Serial_Global [14];	
	
	
	//xor bytes 16, 16, 15, 14, 13, 12, and 11 to generate complete command to write the serial number
	EepromSerialNumber[16] =  EepromSerialNumber[16] ^ EepromSerialNumber[15] ^ EepromSerialNumber[14] ^ EepromSerialNumber[13] ^ EepromSerialNumber[12] ^ EepromSerialNumber[11];

	while((counter < 10) && (!(status == 1))){  
		status = DECT_Send_String (panel,EepromSerialNumber,WriteEepromSerialNumber_Response,returned_string,0,1,1);
		Delay(1);
		counter++;
	}
	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);	
	}
	
	return status;
}


//*************************************************************
//* 
//*
//*************************************************************
int DECT_Vendor_SN_Test(int panel) 
{
	int status = 0;
	char temp_buff[256] = {'\0'};
	int numToRead = 0;
	double Measured_Frequency;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	int counter = 0;
	char read_SN[12] = {'\0'};
	
	
	sprintf(msg,"DECT Serial Number Verification Test\n");
	WriteToTerminal(panel, msg); 
	
	while((counter < 10) && (!(status == 1))){  
		status = DECT_Send_String (panel,ReadVendorSN,ReadVendorSN_Response,returned_string,-1,1,1);  
		Delay(1);
		counter++;
	}

	sprintf(read_SN,"%c%c%c%c%c,", returned_string [8],returned_string [9],returned_string [10],returned_string [11],returned_string [12]); 
	
	sprintf(temp_buff,"VERS_SNO = %c%c%c%c%c\n", returned_string [8],returned_string [9],returned_string [10],returned_string [11],returned_string [12]);
	WriteToTerminal(panel, temp_buff);
	
	if (((returned_string [8] == 74)||(returned_string [8] == 48)||(returned_string [8] == 87))&&((returned_string [9] == 87)||(returned_string [9] == 76)||(returned_string [9] == 78))&&(returned_string [10] == Product_Serial_Global [12])&&(returned_string [11] == Product_Serial_Global [13])&&(returned_string [12] == Product_Serial_Global [14])){
	 status = 1;
	}
	else{
		status = 0;
		sprintf(temp_buff,"Expected_SNO = JW%c%c%c OR Expected_SNO = 0L%c%c%c OR Expected_SNO = 0L%c%c%c\n", Product_Serial_Global [12], Product_Serial_Global [13], Product_Serial_Global [14],Product_Serial_Global [12], Product_Serial_Global [13], Product_Serial_Global [14],Product_Serial_Global [12], Product_Serial_Global [13], Product_Serial_Global [14]);
		WriteToTerminal(panel, temp_buff);
	}
	
	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		strcat(data_string,read_SN);
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		strcat(data_string,read_SN);
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);
	}


	return status;
}



//*************************************************************
//* 
//*
//*************************************************************
int DECT_Serial_Number_String_Test(int panel)
{
	unsigned char EepromSerialNumber[30] = {'\0'}; 
	char nible1;
	char nible2; 
	int temp_int, temp_int_2;
	int status = 0;
	char msg[256];
	char temp_char[24] = {'\0'};
	char temp_char_2[24] = {'\0'};
	int x;
	char returned_string[256] = {'\0'};
	int counter = 0;
	
	
	sprintf(msg,"DECT Write Mitel Serial Number Test\n"); 
	WriteToTerminal(panel, msg); 
	
	for (x=0; x < 30; x++)
    {
    	EepromSerialNumber[x] = WriteSerialNumberString[x];
    }
	
	
	EepromSerialNumber[14] =  Product_Serial_Global [0];  
	
	EepromSerialNumber[15] =  Product_Serial_Global [1]; 
	
	EepromSerialNumber[16] =  Product_Serial_Global [2]; 
	
	EepromSerialNumber[17] =  Product_Serial_Global [3];	
	
	EepromSerialNumber[18] =  Product_Serial_Global [4];	
	
	EepromSerialNumber[19] =  Product_Serial_Global [5]; 
	
	EepromSerialNumber[20] =  Product_Serial_Global [6];	
	
	EepromSerialNumber[21] =  Product_Serial_Global [7];
	
	EepromSerialNumber[22] =  Product_Serial_Global [8]; 
	
	
	EepromSerialNumber[23] =  Product_Serial_Global [9]; 
	
	EepromSerialNumber[24] =  Product_Serial_Global [10];	
	
	EepromSerialNumber[25] =  Product_Serial_Global [11];	
	
	EepromSerialNumber[26] =  Product_Serial_Global [12]; 
	
	EepromSerialNumber[27] =  Product_Serial_Global [13];	
	
	EepromSerialNumber[28] =  Product_Serial_Global [14];
	
	
	
	//xor bytes 29, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, and 14 to generate complete command to write the serial number
	EepromSerialNumber[29] =  EepromSerialNumber[29] ^ EepromSerialNumber[28] ^ EepromSerialNumber[27] ^ EepromSerialNumber[26] ^ EepromSerialNumber[25] ^ EepromSerialNumber[24] ^ EepromSerialNumber[23] ^ EepromSerialNumber[22] ^ EepromSerialNumber[21] ^ EepromSerialNumber[20]^ EepromSerialNumber[19] ^ EepromSerialNumber[18] ^ EepromSerialNumber[17] ^ EepromSerialNumber[16] ^ EepromSerialNumber[15] ^ EepromSerialNumber[14];
	
	
	
	while((counter < 10) && (!(status == 1))){  
		status = DECT_Send_String (panel,EepromSerialNumber,WriteSerialNumberString_Response,returned_string,-1,1,1);
		Delay(1);
		counter++;
	}
	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);	
	}
	
	return status;
}



//*************************************************************
//* 
//*
//*************************************************************
int DECT_Check_Serial_Number_String_Test(int panel) 
{
	int status = 0;
	char temp_buff[256] = {'\0'};
	int numToRead = 0;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'};
	int counter = 0;
	char read_SN[32] = {'\0'};
	
	
	sprintf(msg,"DECT Serial Number String Verification Test\n");
	WriteToTerminal(panel, msg); 
	
	while((counter < 10) && (!(status == 1))){  
		status = DECT_Send_String (panel,ReadSNString,ReadSNString_Response,returned_string,-1,1,1);  
		Delay(1);
		counter++;
	}

	sprintf(read_SN,"%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", returned_string [11],returned_string [12],returned_string [13],returned_string [14],returned_string [15],returned_string [16],returned_string [17],returned_string [18],returned_string [19],returned_string [20],
		 returned_string [21],returned_string [22],returned_string [23],returned_string [24],returned_string [25]); 
	
	sprintf(temp_buff,"SN String = %s\n", read_SN);
	WriteToTerminal(panel, temp_buff);
	
	if (strcmp(read_SN,Product_Serial_Global) == 0){
	 status = 1;
	}
	else{
		status = 0;
		sprintf(temp_buff,"read sn %s is not match with label %s\n", read_SN,Product_Serial_Global);
		WriteToTerminal(panel, temp_buff);
	}
	
	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		strcat(data_string,read_SN);
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		strcat(data_string,read_SN);
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);
	}


	return status;
}





//*************************************************************
//* 
//*
//*************************************************************
int DECT_Serial_Number_String_Test_2(int panel)
{
	unsigned char EepromSerialNumber[24] = {'\0'}; 
	char nible1;
	char nible2; 
	int temp_int, temp_int_2;
	int status = 0;
	char msg[256];
	char temp_char[24] = {'\0'};
	char temp_char_2[24] = {'\0'};
	int x;
	char returned_string[256] = {'\0'};
	int counter = 0;
	
	
	sprintf(msg,"DECT Write Mitel Short String Serial Number Test\n"); 
	WriteToTerminal(panel, msg); 
	
	for (x=0; x < 24; x++)
    {
    	EepromSerialNumber[x] = WriteSerialNumberShortString[x];
    }
	
	
	EepromSerialNumber[14] =  Product_Serial_Global [3];  
	
	EepromSerialNumber[15] =  Product_Serial_Global [4]; 
	
	EepromSerialNumber[16] =  Product_Serial_Global [5]; 
	
	EepromSerialNumber[17] =  Product_Serial_Global [6];	
	
	EepromSerialNumber[18] =  Product_Serial_Global [7];	
	
	EepromSerialNumber[19] =  Product_Serial_Global [8]; 
	
	EepromSerialNumber[20] =  Product_Serial_Global [12];	
	
	EepromSerialNumber[21] =  Product_Serial_Global [13];
	
	EepromSerialNumber[22] =  Product_Serial_Global [14]; 
	
	
	
	//xor bytes 23, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 19, 18, 17, 16, 15, and 14 to generate complete command to write the serial number
	EepromSerialNumber[23] =  EepromSerialNumber[23] ^ EepromSerialNumber[22] ^ EepromSerialNumber[21] ^ EepromSerialNumber[20] ^ EepromSerialNumber[19] ^ EepromSerialNumber[18] ^ EepromSerialNumber[17] ^ EepromSerialNumber[16] ^ EepromSerialNumber[15] ^ EepromSerialNumber[14];
	
	
	
	while((counter < 10) && (!(status == 1))){  
		status = DECT_Send_String (panel,EepromSerialNumber,WriteSerialNumberString_Response,returned_string,-1,1,1);
		Delay(1);
		counter++;
	}
	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		strcat(data_string,msg);
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"FAIL,"); 	//running logfile
		strcat(data_string,msg);	
	}
	
	return status;
}





//*************************************************************
//* 
//*
//*************************************************************
int DECT_Check_Final_Fixture_Status_Test(int panel) 
{

	int status = 0;
	char msg[256];
	char temp_char[24] = {'\0'};
//	char temp_char_2[24] = {'\0'};
	int x;
//	char returned_string[256] = {'\0'};
//	int counter = 0;
	char serial_input[MAXLINE+1] = {'\0'};  
	double delay_time = 600;
	int input_queue_length = 0; 
	int Fixture_COM = 1;
	

	if (!(OpenComConfig (Fixture_COM, "COM1", 9600, 0, 8, 1, 4096, 4096) == 0)){ 
		SetCtrlVal(panel,PANEL_Status_Window, "Unable to open COM port\n");
		Test_In_Process = 0;
		return 0;
	}
			
	if (!(SetXMode (Fixture_COM, 0)==0)){
		SetCtrlVal(panel,PANEL_Status_Window, "Unable to open COM port\n");
		Test_In_Process = 0;			
		return 0;
	}
	if(!(SetCTSMode (Fixture_COM, LWRS_HWHANDSHAKE_OFF) == 0)){
		SetCtrlVal(panel,PANEL_Status_Window, "Unable to open COM port\n");
		Test_In_Process = 0;			
		return 0;
	}
	if(!( SetComTime (Fixture_COM, 0.25) == 0)){
		SetCtrlVal(panel,PANEL_Status_Window, "Unable to open COM port\n");
		Test_In_Process = 0;			
		return 0;
	}
		
	
	FlushInQ (Fixture_COM);
	FlushOutQ (Fixture_COM);
	
	sprintf(msg,"STATUS");
	status = ComWrt(Fixture_COM,"STATUS",6);
	
	Delay(1);
	
	while(GetInQLen(Fixture_COM) > 0)
	{
		input_queue_length = GetInQLen (Fixture_COM);     
    		for (x = 0; x < input_queue_length; x++){
    			serial_input[x] = (char) ComRdByte (Fixture_COM);
			sprintf(temp_char,"%c",serial_input[x]);
			WriteToTerminal(panel, temp_char);
		}
	}
	
	CloseCom (Fixture_COM);
	
	
	if (strstr (serial_input, "OPEN")){
		status = 0;
	}
	if (strstr (serial_input, "CLOSE")){
		status = 1;
	}
	
	return status;
	
}


///////////////////////////////////////////////////////////////
//*************************************************************
//*int Init_Agilent (int panel)	
//* 
//*************************************************************
int Init_Agilent (int panel) {		
	
	char Temp_Buff[256]; 
	int status = 1;
	int numToRead; 


	
   //get handle for Agilent instrument at Agilent_GPIB address, open the device
   AGILENT_HANDLE = ibdev (0, AGILENT_SIM_GPIB, NO_SAD, T10s, 1, 0);
   if (ibsta & 0x8000) status = 0;
   
   //to bring Agilent to initial state after power on, you send the following   
   //send interface clear to the instrument
   ibwrt (AGILENT_HANDLE, "IFC", strlen("IFC"));
   if (ibsta & 0x8000) status = 0;
   
   //send device clear to the instrunet
   ibwrt (AGILENT_HANDLE, "DCL", strlen("DCL")); 
   if (ibsta & 0x8000) status = 0;
   
    //clear the device
   ibclr (AGILENT_HANDLE);
   if (ibsta & 0x8000) status = 0;  
   
   //clear status of the instrument
   ibwrt (AGILENT_HANDLE, "*CLS", strlen("*CLS"));
   if (ibsta & 0x8000) status = 0;
   
   //reset the instrunet
   ibwrt (AGILENT_HANDLE, "*RST", strlen("*RST")); 
   if (ibsta & 0x8000) status = 0;   

   //write command to read the instrument id
   ibwrt (AGILENT_HANDLE, "*IDN?", strlen("*IDN?")); 
   if (ibsta & 0x8000) status = 0;
   
   //read the instrument id
   numToRead = 200;
   ibrd (AGILENT_HANDLE, Temp_Buff, numToRead);
   if (ibsta & 0x8000) status = 0;
   
   //to bring Agilent to initial state after power on, you send the following   
   //send interface clear to the instrument
   ibwrt (AGILENT_HANDLE, "IFC", strlen("IFC"));
   if (ibsta & 0x8000) status = 0;
   
   //send device clear to the instrunet
   ibwrt (AGILENT_HANDLE, "DCL", strlen("DCL")); 
   if (ibsta & 0x8000) status = 0;
   
    //clear the device
   ibclr (AGILENT_HANDLE);
   if (ibsta & 0x8000) status = 0;  
   
   //clear status of the instrument
   ibwrt (AGILENT_HANDLE, "*CLS", strlen("*CLS"));
   if (ibsta & 0x8000) status = 0;
   
   //reset the instrunet
   ibwrt (AGILENT_HANDLE, "*RST", strlen("*RST")); 
   if (ibsta & 0x8000) status = 0;
   
   
   if(0 == status)
	   WriteToTerminal(panel, "Initial Agilent Fail.\r\n");  
   else
	   WriteToTerminal(panel, "Initial Agilent Pass.\r\n");     
   
   
	return status;
   
}
//*************************************************************
//*int Set_Agilent_ForLeakCurr(int panel)  	
//* 
//*************************************************************
int Set_Agilent_LeakCurr(int panel) 
{

	int status = 1;
	char msg[256] = {'\0'};
	char temp_buff[24] = {'\0'};
	int numToRead;
	int counter = 0;


	sprintf(msg,"DECT Set Agilent for leakage current test\n");
	WriteToTerminal(panel, msg);


	//Set channel 1 4V
	ibwrt (AGILENT_HANDLE, "SOUR:VOLT 4.0", strlen("SOUR:VOLT 4.0"));
	if (ibsta & 0x8000) status = 0;
	//Set channel 1 current limit 3
	ibwrt (AGILENT_HANDLE, "SOUR:CURR 3", strlen("SOUR:CURR 3"));
	if (ibsta & 0x8000) status = 0;

	//Set channel 2 off   
	ibwrt (AGILENT_HANDLE, "SOUR:VOLT2 0", strlen("SOUR:VOLT2 0"));
	if (ibsta & 0x8000) status = 0;

	ibwrt (AGILENT_HANDLE, "SOUR:CURR2 0.1", strlen("SOUR:CURR2 0.1"));
	if (ibsta & 0x8000) status = 0;

	Delay(1);
	//set output
	ibwrt (AGILENT_HANDLE, "OUTP:STAT ON", strlen("OUTP:STAT ON"));
	if (ibsta & 0x8000) status = 0;

	status = 1;		//debug

	if (status)
	{
		status = 1;
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg);
		sprintf(msg,"PASS,"); 	//running logfile
		//strcat(data_string,msg);
	}
	else
	{
		status = 0;
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg);
		sprintf(msg,"FAIL,"); 	//running logfile
		//strcat(data_string,msg);
	}

	
	return status;
}
//*************************************************************
//*int Meas_Agilent_InstantCurrent(int panel)  	
//* 
//*************************************************************
int Measure_Agilent_InsCurr(int panel) 
{

	int status = 1;
	char msg[256] = {'\0'};
	char temp_buff[1024] = {'\0'}; 
	int numToRead;
	int counter = 0;

	
	sprintf(msg,"DECT measure Agilent instant current test\n");
	WriteToTerminal(panel, msg); 
	
	ibwrt (AGILENT_HANDLE, "SENSe:CURR:RANG 3", strlen("SENSe:CURR:RANG 3"));
	if (ibsta & 0x8000) status = 0;
	
	Delay(1);
		
	ibwrt (AGILENT_HANDLE, "MEAS:CURR?", strlen("MEAS:CURR?"));
	if (ibsta & 0x8000) status = 0;
   
	//read the instrument
	numToRead = DEFAULT_READLEN;
	ibrd (AGILENT_HANDLE, temp_buff, numToRead);
	if (ibsta & 0x8000) status = 0;
	
	
	float fMeasure_value =   atof(temp_buff) * 1000;
	
	sprintf(msg,"Agilent 663xx Returned Value: %fmA\n", fMeasure_value);
	WriteToTerminal(panel, msg);
	
		sprintf(msg,"%f,",fMeasure_value); 	//running logfile
		strcat(data_string,msg);
	
	
	if(fMeasure_value>5)
	{
		status = 0; 
		Set_Agilent_Off(panel); //power off because detect unusural current
		
	}
		
		
	//status = 1;		//debug
				
	if (status)
	{
		status = 1;
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		//strcat(data_string,msg);
	}
	else
	{
		status = 0;
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg);
		sprintf(msg,"FAIL,"); 	//running logfile
		//strcat(data_string,msg);
	}
		
	
	return status;
}
//*************************************************************
//*int Meas_Agilent_InstantCurrent(int panel)  	
//* 
//*************************************************************
int Measure_Agilent_LeakCurr(int panel) 
{

	int status = 1;
	char msg[256] = {'\0'};
	char temp_buff[1024] = {'\0'}; 
	int numToRead;
	int counter = 0;

	
	sprintf(msg,"DECT measure Agilent leakage current test\n");
	WriteToTerminal(panel, msg); 
	
	
	ibwrt (AGILENT_HANDLE, "SENSe:CURR:RANG 0.02", strlen("SENSe:CURR:RANG 0.02"));
	if (ibsta & 0x8000) status = 0;
	
	Delay(1);
	float fMeasure_value = 0;
	int iLoop = 0;
	for(iLoop = 0; iLoop< 10 ; iLoop++)
	{
		ibwrt (AGILENT_HANDLE, "MEAS:CURR?", strlen("MEAS:CURR?"));
		if (ibsta & 0x8000) status = 0;

		//read the instrument
		numToRead = DEFAULT_READLEN;
		ibrd (AGILENT_HANDLE, temp_buff, numToRead);
		if (ibsta & 0x8000) status = 0;
		
		fMeasure_value =   atof(temp_buff) * 1000;  
		sprintf(msg,"Agilent 663xx Returned Value: %fmA\n", fMeasure_value);
		WriteToTerminal(panel, msg);
		
		
		if(fMeasure_value>1)  
		{
			status = 0; 
		}
		else 
		{
			status = 1;
			break;
		}

		Delay(1);
	}
	if(iLoop == 10)   	status = 0; 
	
	
	
	
	ibwrt (AGILENT_HANDLE, "SENSe:CURR:RANG 3", strlen("SENSe:CURR:RANG 3"));
	if (ibsta & 0x8000) status = 0;

	Delay(1);
		sprintf(msg,"%f,",fMeasure_value); 	//running logfile
		strcat(data_string,msg);
		
	//status = 1;		//debug
				
	if (status)
	{
		status = 1;
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		//strcat(data_string,msg);
	}
	else
	{
		status = 0;
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg);
		sprintf(msg,"FAIL,"); 	//running logfile
		//strcat(data_string,msg);
	}
		
	
	return status;
}

//*************************************************************
//*int Set_Agilent_ForChargingCurr(int panel)  	
//* 
//*************************************************************
int Set_Agilent_CharCurr(int panel) 
{

	int status = 1;
	char msg[256] = {'\0'};
	char temp_buff[24] = {'\0'};
	int numToRead;
	int counter = 0;


	sprintf(msg,"DECT Set Agilent for charging current test\n");
	WriteToTerminal(panel, msg);
	

	
	ibwrt (AGILENT_HANDLE, "SENSe:CURR:RANG 3", strlen("SENSe:CURR:RANG 3"));
	if (ibsta & 0x8000) status = 0;
	

	//Set channel 1 4V
	ibwrt (AGILENT_HANDLE, "SOUR:VOLT 3.7", strlen("SOUR:VOLT 3.7"));
	if (ibsta & 0x8000) status = 0;
	//Set channel 1 current limit 3
	ibwrt (AGILENT_HANDLE, "SOUR:CURR 3", strlen("SOUR:CURR 3"));
	if (ibsta & 0x8000) status = 0;

	//Set channel 2 on   
	ibwrt (AGILENT_HANDLE, "SOUR:VOLT2 5", strlen("SOUR:VOLT2 5"));
	if (ibsta & 0x8000) status = 0;

	ibwrt (AGILENT_HANDLE, "SOUR:CURR2 1.5", strlen("SOUR:CURR2 1.5"));
	if (ibsta & 0x8000) status = 0;

	Delay(1);
	//set output
	ibwrt (AGILENT_HANDLE, "OUTP2:STAT ON", strlen("OUTP2:STAT ON"));
	if (ibsta & 0x8000) status = 0;

	status = 1;		//debug

	if (status)
	{
		status = 1;
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg);
		sprintf(msg,"PASS,"); 	//running logfile
	//	strcat(data_string,msg);
	}
	else
	{
		status = 0;
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg);
		sprintf(msg,"FAIL,"); 	//running logfile
	//	strcat(data_string,msg);
	}

	
	return status;
}
//*************************************************************
//*int Meas_Agilent_ChargingCurrent(int panel)  	
//* 
//*************************************************************
int Measure_Agilent_CharCurr(int mode, int panel) 
{

	int status = 1;
	char msg[256] = {'\0'};
	char temp_buff[1024] = {'\0'}; 
	int numToRead;
	int counter = 0;
	float fMeasure_value = 0;

	
	sprintf(msg,"DECT measure Agilent charging current test\n");
	WriteToTerminal(panel, msg); 
		
	int iLoop = 0;
	for(iLoop = 0; iLoop< 10 ; iLoop++)
	{
		ibwrt (AGILENT_HANDLE, "MEAS:CURR2?", strlen("MEAS:CURR2?"));
		if (ibsta & 0x8000) status = 0;

		//read the instrument
		numToRead = DEFAULT_READLEN;
		ibrd (AGILENT_HANDLE, temp_buff, numToRead);
		if (ibsta & 0x8000) status = 0;
		
		fMeasure_value =   atof(temp_buff) * 1000;  
		sprintf(msg,"Agilent 663xx Returned Value: %fmA\n", fMeasure_value);
		WriteToTerminal(panel, msg);

		if(mode == 0)// disable charging
		{
			if(fMeasure_value>15 || fMeasure_value < 0)
			{
				status = 0;
			}
			else
			{
				status = 1;
				break;
			}
		}
		else if(mode == 1)	   //low charging
		{
			if(fMeasure_value>800 || fMeasure_value < 80)
			{
				status = 0;
			}
			else
			{
				status = 1;
				break;
			}
		}
		else if(mode == 2)	   //high charging
		{
			if(fMeasure_value>850 || fMeasure_value < 400)
			{
				status = 0;
			}
			else
			{
				status = 1;
				break;
			}
		}   
		else //(mode == 3)	   //DC charging
		{
			if(fMeasure_value>1200 || fMeasure_value < 600)
			{
				status = 0;
			}
			else
			{
				status = 1;
				break;
			}
		}   

	

		Delay(1);
	}
	if(iLoop == 10)   	status = 0; 
	
		sprintf(msg,"%f,",fMeasure_value); 	//running logfile
		strcat(data_string,msg);
		
	//status = 1;		//debug
				
	if (status)
	{
		status = 1;
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		//strcat(data_string,msg);
	}
	else
	{
		status = 0;
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg);
		sprintf(msg,"FAIL,"); 	//running logfile
		//strcat(data_string,msg);
	}
		
	
	return status;
}
//*************************************************************
//*int Set_Agilent_Off(int panel)  	
//* 
//*************************************************************
int Set_Agilent_Off(int panel) 
{

	int status = 1;
	char msg[256] = {'\0'};
	char temp_buff[24] = {'\0'};
	int numToRead;
	int counter = 0;


	sprintf(msg,"DECT Set Agilent off\n");
	WriteToTerminal(panel, msg);
	
	//Set channel 1 off   
	ibwrt (AGILENT_HANDLE, "SOUR:VOLT 0", strlen("SOUR:VOLT2 0"));
	if (ibsta & 0x8000) status = 0;

	ibwrt (AGILENT_HANDLE, "SOUR:CURR 0.1", strlen("SOUR:CURR2 0.1"));
	if (ibsta & 0x8000) status = 0;


	//Set channel 2 off   
	ibwrt (AGILENT_HANDLE, "SOUR:VOLT2 0", strlen("SOUR:VOLT2 0"));
	if (ibsta & 0x8000) status = 0;

	ibwrt (AGILENT_HANDLE, "SOUR:CURR2 0.1", strlen("SOUR:CURR2 0.1"));
	if (ibsta & 0x8000) status = 0;


	//set output
	ibwrt (AGILENT_HANDLE, "OUTP:STAT OFF", strlen("OUTP:STAT OFF"));
	if (ibsta & 0x8000) status = 0;
	
	ibwrt (AGILENT_HANDLE, "OUTP2:STAT OFF", strlen("OUTP2:STAT OFF"));
	if (ibsta & 0x8000) status = 0;

	status = 1;		//debug

	if (status)
	{
		status = 1;
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg);
		sprintf(msg,"PASS,"); 	//running logfile
		//strcat(data_string,msg);
	}
	else
	{
		status = 0;
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg);
		sprintf(msg,"FAIL,"); 	//running logfile
		//strcat(data_string,msg);
	}

	
	return status;
}

//*************************************************************
//*int Set_Agilent_ForChargingCurr(int panel)  	
//* 
//*************************************************************
int Set_Agilent_NormalWorkVolt(int panel) 
{

	int status = 1;
	char msg[256] = {'\0'};
	char temp_buff[24] = {'\0'};
	int numToRead;
	int counter = 0;


	sprintf(msg,"DECT Set Agilent for normal test\n");
	WriteToTerminal(panel, msg);
	

	
	ibwrt (AGILENT_HANDLE, "SENSe:CURR:RANG 3", strlen("SENSe:CURR:RANG 3"));
	if (ibsta & 0x8000) status = 0;
	

	//Set channel 1 4V
	ibwrt (AGILENT_HANDLE, "SOUR:VOLT 4.0", strlen("SOUR:VOLT 4.0"));
	if (ibsta & 0x8000) status = 0;
	//Set channel 1 current limit 3
	ibwrt (AGILENT_HANDLE, "SOUR:CURR 3", strlen("SOUR:CURR 3"));
	if (ibsta & 0x8000) status = 0;


	status = 1;		//debug

	if (status)
	{
		status = 1;
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg);
		sprintf(msg,"PASS,"); 	//running logfile
	//	strcat(data_string,msg);
	}
	else
	{
		status = 0;
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg);
		sprintf(msg,"FAIL,"); 	//running logfile
	//	strcat(data_string,msg);
	}

	
	return status;
}


//*************************************************************
//*int Meas_Agilent_PowerOnCurrent(int panel)  	
//* 
//*************************************************************
int Measure_Agilent_PwrOnCurr(int panel) 
{

	int status = 1;
	char msg[256] = {'\0'};
	char temp_buff[1024] = {'\0'}; 
	int numToRead;
	int counter = 0;

	
	sprintf(msg,"DECT measure Agilent power on current test\n");
	WriteToTerminal(panel, msg);   

	
	ibwrt (AGILENT_HANDLE, "SENSe:CURR:RANG 3", strlen("SENSe:CURR:RANG 3"));
	if (ibsta & 0x8000) status = 0;
	
	Delay(1);
	float fMeasure_value = 0;
	int iLoop = 0;
	for(iLoop = 0; iLoop< 10 ; iLoop++)
	{
		ibwrt (AGILENT_HANDLE, "MEAS:CURR?", strlen("MEAS:CURR?"));
		if (ibsta & 0x8000) status = 0;

		//read the instrument
		numToRead = DEFAULT_READLEN;
		ibrd (AGILENT_HANDLE, temp_buff, numToRead);
		if (ibsta & 0x8000) status = 0;
		
		fMeasure_value =   atof(temp_buff) * 1000;  
		sprintf(msg,"Agilent 663xx Returned Value: %fmA\n", fMeasure_value);
		WriteToTerminal(panel, msg);
		
		
		if(fMeasure_value<5 || fMeasure_value > 350)   //adjust the same with spec
		{
			status = 0; 
		}
		else 
		{
			status = 1;
			break;
		}

		Delay(1);
	}
	if(iLoop == 10)   	status = 0; 
	
	
	ibwrt (AGILENT_HANDLE, "SENSe:CURR:RANG 3", strlen("SENSe:CURR:RANG 3"));
	if (ibsta & 0x8000) status = 0;

		
	//status = 1;		//debug
	
		sprintf(msg,"%f,",fMeasure_value); 	//running logfile
		strcat(data_string,msg);
				
	if (status)
	{
		status = 1;
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		//strcat(data_string,msg);
	}
	else
	{
		status = 0;
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg);
		sprintf(msg,"FAIL,"); 	//running logfile
		//strcat(data_string,msg);
	}
		
	
	return status;
}
//*************************************************************
//* 
//*
//*************************************************************
int DECT_SetChargeCurrentMode_0_USB(int panel)
{

	int status = 0;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'}; 
	int counter = 0;

	
	sprintf(msg,"DECT SetChargeCurrentMode_0\n");
	WriteToTerminal(panel, msg); 
	
	
	while((counter < 10) && (!(status == 1))){  
		status = DECT_Send_String_USB (panel,SetChargeCurrentMode_0,SetChargeCurrentMode_0_Response,returned_string,6,1,1);	  //needs to be 6 to support 7.2
		Delay(1);
		counter++;
	}
	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
	//	strcat(data_string,msg);
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"FAIL,"); 	//running logfile
	//	strcat(data_string,msg);	
	}
	
	return status;
}


//*************************************************************
//* 
//*
//*************************************************************
int DECT_SetChargeCurrentMode_1_USB(int panel)
{

	int status = 0;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'}; 
	int counter = 0;

	
	sprintf(msg,"DECT SetChargeCurrentMode_1\n");
	WriteToTerminal(panel, msg); 
	
	
	while((counter < 10) && (!(status == 1))){  
		status = DECT_Send_String_USB (panel,SetChargeCurrentMode_1,SetChargeCurrentMode_1_Response,returned_string,6,1,1);	  //needs to be 6 to support 7.2
		Delay(1);
		counter++;
	}
	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
	//	strcat(data_string,msg);
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"FAIL,"); 	//running logfile
	//	strcat(data_string,msg);	
	}
	
	return status;
}  
//*************************************************************
//* 
//*
//*************************************************************
int DECT_DisableCharger_USB(int panel)
{

	int status = 0;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'}; 
	int counter = 0;

	
	sprintf(msg,"DECT DisableCharger\n");
	WriteToTerminal(panel, msg); 
	
	
	while((counter < 10) && (!(status == 1))){  
		status = DECT_Send_String_USB (panel,DisableCharger,DisableCharger_Response,returned_string,6,1,1);	  //needs to be 6 to support 7.2
		Delay(1);
		counter++;
	}
	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		//strcat(data_string,msg);
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"FAIL,"); 	//running logfile
		//strcat(data_string,msg);	
	}
	
	return status;
}

//*************************************************************
//* 
//*
//*************************************************************
int DECT_GetChargerStatus_USB(int panel)
{

	int status = 0;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'}; 
	int counter = 0;

	
	sprintf(msg,"DECT GetChargerStatus\n");
	WriteToTerminal(panel, msg); 
	
	
	while((counter < 10) && (!(status == 1))){  
		status = DECT_Send_String_USB (panel,GetChargerStatus,GetChargerStatus_Response,returned_string,6,1,1);
		Delay(1);
		counter++;
	}
	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		//strcat(data_string,msg);
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"FAIL,"); 	//running logfile
		//strcat(data_string,msg);	
	}
	
	return status;
}


//*************************************************************
//* 
//*
//*************************************************************
int DECT_GetChargerStatus(int panel)
{

	int status = 0;
	char msg[256] = {'\0'};
	char returned_string[256] = {'\0'}; 
	int counter = 0;

	
	sprintf(msg,"DECT GetChargerStatus\n");
	WriteToTerminal(panel, msg); 
	
	
	while((counter < 10) && (!(status == 1))){  
		status = DECT_Send_String (panel,GetChargerStatus,GetChargerStatus_Response,returned_string,5,1,1);
		Delay(1);
		counter++;
	}
	
	if(status == 1){
		sprintf(msg,"Test Pass\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"PASS,"); 	//running logfile
		//strcat(data_string,msg);
	}
	else{
		sprintf(msg,"Test Fail\n\n");
		WriteToTerminal(panel, msg); 
		sprintf(msg,"FAIL,"); 	//running logfile
		//strcat(data_string,msg);	
	}
	
	return status;
}





/////////////////////////////////////////////////////////////////////////////////////////////
