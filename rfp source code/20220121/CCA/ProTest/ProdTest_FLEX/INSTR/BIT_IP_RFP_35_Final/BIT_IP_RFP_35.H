#ifndef BIT_IP_RFP_35_H
#define BIT_IP_RFP_35_H


#include "osdefn_b.h"			//TASK-ID
#include "primitiv.h"		  	//SUB-ID for Task

#define     FP_RFP  0x10

#ifndef		MAX_MAIL_LENGTH
#define     MAX_MAIL_LENGTH     0x80
#endif

#ifndef STRUCT_RS232_RFP
#define STRUCT_RS232_RFP
struct rs232_RFP 
	{
	int		port;
	int 	baudrate;
	int		chk_ackn;
	unsigned char	tx_buffer[0x80];
	unsigned char	rx_buffer[0x1000];	  //back to 4096 @GC 18.08.2011
	char	tx_count;
	char	rx_count;
	char 	dut_type;
	char	mail_type;
	char	module_addr;
	char	protest_addr;
	};
#endif					

extern struct rs232_RFP ip_struct;
extern struct rs232_RFP rs232_RFP; 

#define NO_FLAGS_SET    0
#define NOT_CONNECTED   0
#define CONNECTED       1


////////////////////////////////////////////////////////////////////////////////

int 	ipRfp_clearTempFiles(void);

/////////////////////// OpenOCD ////////////////////////////////////////////////

int 	ipRfp_runOpenOCD(char *openocdScripts, char *respBuffer, int respSize);


/////////////////////// Network  ////////////////////////////////////////////////
int		ipRfp_SystemPing(char *ipAddr , int timeOut);
int 	ipRfp_getAdapterIpAddr(char *adapterName, char *adapterIpAddr);

/////////////////////// Socket /////////////////////////////////////////////////
int 	ipRfp_startWinsock(void);
int 	ipRfp_cleanupWinsock(void);



int 	ipRfp_rsx_Open(char *ipAddr, int port);
int 	ipRfp_rsx_Close(void);

int 	ipRfp_rsx_SendDataToRfp(int length, unsigned char *pBuffer);
int 	ipRfp_rsx_ReceiveDataFromIpRfp(int length, unsigned char *pBuffer);
int 	ipRfp_rsx_mail_out(int write_count, unsigned char *buffer, int read_count);


/////////////////////// RSX ///////////////////////////////////////////////////

int 	ipRfp_rsx_EnterTestmode(void);
int 	ipRfp_rsx_ExitTestmode(void);
char   *ipRfp_rsx_GetSoftwareVersion(char *buffer);
char   *ipRfp_rsx_GetSoftwareVersion_Retry(char *buffer, int retry);
char   *ipRfp_rsx_ReadEeprom(int address, char *buffer, int count);
int 	ipRfp_rsx_WriteEeprom(int address, char *buffer, int count,int convert);
int 	ipRfp_rsx_GetRSSI(unsigned char *value);
int 	ipRfp_rsx_SetupDummyBearer(unsigned char dummychannel, unsigned char dummyslot);
int 	ipRfp_rsx_SetDiversity(unsigned char value);
//int 	ipRfp_rsx_SetRxChannel(unsigned char channel,unsigned char radio);           // not in rfp35 @GC26.05.2011  
int 	ipRfp_rsx_SetDeviation(unsigned char value);
int 	ipRfp_rsx_ReadEepromDeviation(unsigned char *value, unsigned char *radio);   // new @GC26.05.2011
int 	ipRfp_rsx_WriteEepromDeviation(unsigned char value, int radio);
int 	ipRfp_rsx_SetB_fieldContent(unsigned char value1, unsigned char value2);
int 	ipRfp_rsx_SetClock(unsigned char value);
int 	ipRfp_rsx_ReadEepromClockFrequency(unsigned char *value);
int 	ipRfp_rsx_WriteEepromClockFrequency(unsigned char value);
int 	ipRfp_rsx_SetBandgap(unsigned char value);
int 	ipRfp_rsx_ReadEepromBandgap(unsigned char *value);
int 	ipRfp_rsx_WriteEepromBandgap(unsigned char value);
int 	ipRfp_rsx_FinalizeEeprom(void);
int     ipRfp_rsx_getEepromValues(unsigned char *eeprom_value);              // @GC07.06.2011
int     ipRfp_rsx_showEepromValues(unsigned char *eeprom_value_adjust,unsigned char *eeprom_value_reboot); // @GC07.06.2011

int 	ipRfp_rsx_SetFrequencyBand(unsigned char value);
int 	ipRfp_rsx_SetTransmitPower(unsigned char value);  

 
/////////////////////// SSH per libssh /////GC13.01.2012 ///////////////////////
int		ipRfp_libssh_Open(char *ipaddr);
int		ipRfp_libssh_Reopen(void);
int		ipRfp_libssh_Close(void);
int		ipRfp_libssh_WriteAndRead(char *wBuffer, char *rBuffer, int maxread, char *wildString, double timeoutSec );
int		ipRfp_libssh_changeToRootUser(void);
int		ipRfp_libssh_sendShellCmd(char *strCmd);
int		ipRfp_libssh_getIpAddress(char *respBuffer);
int 	ipRfp_libssh_getMacAddress(char *respBuffer);
int		ipRfp_libssh_getManuVersion(char *pManuVersionFW,char *pManuVersionSW);
int 	ipRfp_libssh_getUsbDev(char *usbdevStr);                                        // usbStick or "NOT FOUND"
int 	ipRfp_libssh_getTemperature(double *temperature);
int 	ipRfp_libssh_getLinkStatus(char *ethDev, char *linkStatus);						// ethDev = eth0, eth1
int 	ipRfp_libssh_getRTCStatus(char *rtcStatus, char *ntpStatus, char *dateString);  // OK OK 2011-11-11T09:02:39
int     ipRfp_libssh_getBMCStatus(char *bmcStatus,char *bmcTestmode, char *bmcCalibStatus);		    // RUNNING or LOADING  // OK
int		ipRfp_libssh_enableBMCTestmode(void);
int 	ipRfp_libssh_getWLanStatus(char *respBuffer);
int 	ipRfp_libssh_checkBootDevice(char *bootDevice);         // "USBStick" "Bootimage 0" "Bootimage 1"
int 	ipRfp_libssh_setLED(char *ledCmd);						// on,red,green,off,system
int		ipRfp_libssh_setHeating(char *sCmd);					// on,off
int 	ipRfp_libssh_writeConfig(char *mac, char *hwType, char *branding, char *sernr, char *exid);
int 	ipRfp_libssh_readConfig(char *mac, char *hwType, char *branding, char *sernr, char *exid);
int 	ipRfp_libssh_setWLAN(char *wlanMode);                                           // "off", "2.4 g on", "2.4 n on", "5.18 n on" 
int 	ipRfp_libssh_enableRSX(void);
int 	ipRfp_libssh_FactoryReset(void);           // PARK.xml wird auch geloescht
int		ipRfp_libssh_FactoryCleanup(void);		   // PARK.xml bleibt erhalten


int		ipRfp_libssh_FtpGet(char *hostIpAddr, char *srcPath, char *srcFileName);
int     ipRfp_libssh_deleteFilesFromDut(void);        


// @GC 10.09.2013
int 	ipRfp_libssh_writePARK(char *strParkHexNumber, char *strDECTrDomain, char *strWLANrDomain);
// @GC 22.10.2012
int 	ipRfp_libssh_readPARK(char *strParkHexNumber);					
int 	ipRfp_libssh_readPARKxml(char *strParkHexNumber, char *strParkOctNumber, char *strDECTrDomain, char *strWLANrDomain);



/////////////////////// RS232 ////////////////////////////////////////////////////

int 	ipRfp_rs232_Open (int ComPort,int BaudRate, int TimeOut);
void 	ipRfp_rs232_Close (void);
int 	ipRfp_rs232_set_timeout(int TimeOut);
int		ipRfp_rs232_mail_out (char *cmdBuffer,char *bufferAnswerA,char *bufferAnswerB);
int 	ipRfp_rs232_mail_out_timeout (char *cmdBuffer,char *bufferAnswerA,char *bufferAnswerB,int timeOutSec);

int 	ipRfp_rs232_WaitForBootUp(int timeOut, char* bootstringA,char* bootstringB,char* bootstringC,char* bootstringD,char* bootstringE);

int		ipRfp_rs232_activate(void);
int 	ipRfp_rs232_sendShellCmd(char *strCmd);
int     ipRfp_rs232_getIpAddress(char *ipstr);
int 	ipRfp_rs232_setIpAddress(char *ipstr);
int     ipRfp_rs232_getUsbDev(char *usbdevStr);
int		ipRfp_rs232_getBMCStatus(char *bmcStatus,char *bmcTestmode,char *bmcCalibStatus);
int     ipRfp_rs232_getLinkStatus(char *ethDev,  char *linkStatus);
int 	ipRfp_rs232_enableSSH(void);
int 	ipRfp_rs232_enableRSX(void);
int 	ipRfp_rs232_sendHalt(void);
int 	ipRfp_rs232_writeConfig(char *mac, char *hwType, char *branding, char *sernr, char *exid);
int 	ipRfp_rs232_readConfig(char *mac, char *hwType, char *branding, char *sernr, char *exid);

int 	ipRfp_rs232_getFinalizeState(char *respBuffer);
int 	ipRfp_rs232_setFinalizeState(void);                             // @GC 13.06.2011 only for Debug

int     ipRfp_rs232_CheckOTParea(void);
int 	ipRfp_rs232_checkBootImage(char *errBuffer);					// @GC 05.10.2011
int		ipRfp_rs232_CheckUBIerror(char *errBuffer);						// @GC 31.10.2011
int 	ipRfp_rs232_flashErase(void);			 						// @GC 05.10.2011
int 	ipRfp_rs232_sync(void);			 				 				// @GC 05.10.2011


// @GC 22.09.2012
int 	ipRfp_rs232_writePARK(char *strParkHexNumber, char *strDECTrDomain, char *strWLANrDomain);
int 	ipRfp_rs232_readPARK(char *strParkHexNumber);					
int 	ipRfp_rs232_readPARKxml(char *strParkHexNumber, char *strParkOctNumber, char *strDECTrDomain, char *strWLANrDomain);






/////////////////////////////////////////////////////////////////////////////////

#endif //BIT_IP_RFP_35_H

