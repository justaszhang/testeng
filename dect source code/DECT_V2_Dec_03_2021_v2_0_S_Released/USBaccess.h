// DLL class definitions for access to USB HID devices
//
// (C) 2001-2003 Copyright Cleware GmbH
// All rights reserved
//
// History:
// 05.01.2001	ws	Initial coding
// 17.07.2001	ws	cleanup interface
// 10.12.2001	ws	cleanup interface again, basic class to hide implementation details
// 13.12.2001	ws	introduced versionNumber and virtual destructor
// 23.05.2002	ws	added switch access
// ...
// 03.02.2003	ws	added switch version 10 
// 04.08.2003	ws	added humidity 
// 21.01.2004	ws	fixed some humidity problems 
//		 2004	ws	added contact + io16
// 05.02.2005	ws	added ADC08-Support (330)
// 17.08.2006	ws	added Humi22-Support (340)
// 16.01.2007	ws	tolerance humi 22 degree or 5%RH (343)
// 23.01.2007	ws	added check humi validity not for subdevices (344)
// 25.01.2007	ws	indication of timeout with humi22 (-200.) missing (345)



// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the USBACCESS_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// USBACCESS_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.


#ifndef __USBACCESS_H__
#define __USBACCESS_H__

#ifdef USBACCESS_EXPORTS
#define USBACCESS_API __declspec(dllexport)
#else
#define USBACCESS_API __declspec(dllimport)
#endif

const int USBaccessVersion = 345 ;


typedef unsigned long int CUSBaccess ;
// typedef void * HANDLE ;	// defined in windows.h
enum FCWUSBactions {		LEDs=0, EEwrite=1, EEread=2, Reset=3, KeepCalm=4, GetInfo=5, StartMeasuring=6 } ;
enum FCWLED_IDs {			LED_0=0, LED_1=1, LED_2=2, LED_3=3 } ;
enum FCWSWITCH_IDs {		SWITCH_0=0x10, SWITCH_1=0x11, SWITCH_2=0x12, SWITCH_3=0x13 } ;
enum FCWUSBtype_enum {		ILLEGAL_DEVICE=0,
							LED_DEVICE=0x01,
							WATCHDOG_DEVICE=0x05,
							AUTORESET_DEVICE=0x06,
							SWITCH1_DEVICE=0x08,
							TEMPERATURE_DEVICE=0x10, 
							TEMPERATURE2_DEVICE=0x11,
							TEMPERATURE5_DEVICE=0x15, 
							HUMIDITY1_DEVICE=0x20,
							CONTACT00_DEVICE=0x30
							} ;


	USBACCESS_API CUSBaccess * _stdcall	FCWInitObject() ;
	USBACCESS_API void _stdcall			FCWUnInitObject(CUSBaccess *obj) ;
	USBACCESS_API int _stdcall			FCWOpenCleware(CUSBaccess *obj) ;
	USBACCESS_API int _stdcall			FCWCloseCleware(CUSBaccess *obj) ;
	USBACCESS_API int _stdcall			FCWRecover(CUSBaccess *obj, int deviceNo) ;
	USBACCESS_API HANDLE _stdcall		FCWGetHandle(CUSBaccess *obj, int deviceNo) ;
	USBACCESS_API int _stdcall			FCWGetValue(CUSBaccess *obj, int deviceNo, unsigned char *buf, int bufsize) ;
	USBACCESS_API int _stdcall			FCWSetValue(CUSBaccess *obj, int deviceNo, unsigned char *buf, int bufsize) ;
	//USBACCESS_API int _stdcall			FCWSetLED(CUSBaccess *obj, int deviceNo, enum LED_IDs Led, int value) ;	// value: 0=off 7=medium 15=highlight
	//USBACCESS_API int _stdcall			FCWSetSwitch(CUSBaccess *obj, int deviceNo, enum SWITCH_IDs Switch, int On) ;	//	On: 0=off, 1=on
	//USBACCESS_API int _stdcall			FCWGetSwitch(CUSBaccess *obj, int deviceNo, enum SWITCH_IDs Switch) ;			//	On: 0=off, 1=on, -1=error
	//USBACCESS_API int _stdcall			FCWGetSeqSwitch(CUSBaccess *obj, int deviceNo, enum SWITCH_IDs Switch, int seqNum) ;			//	On: 0=off, 1=on, -1=error
	USBACCESS_API int _stdcall			FCWGetSwitchConfig(CUSBaccess *obj, int deviceNo, int *switchCount, int *buttonAvailable) ;	
	USBACCESS_API int _stdcall			FCWSetTempOffset(CUSBaccess *obj, int deviceNo, double Sollwert, double Istwert) ;
	USBACCESS_API int _stdcall			FCWGetTemperature(CUSBaccess *obj, int deviceNo, double *Temperature, int *timeID) ;
	USBACCESS_API int _stdcall			FCWGetHumidity(CUSBaccess *obj, int deviceNo, double *Humidity, int *timeID) ;
	USBACCESS_API int _stdcall			FCWResetDevice(CUSBaccess *obj, int deviceNo) ;
	USBACCESS_API int _stdcall			FCWStartDevice(CUSBaccess *obj, int deviceNo) ;
	USBACCESS_API int _stdcall			FCWCalmWatchdog(CUSBaccess *obj, int deviceNo, int minutes, int minutes2restart) ;
	USBACCESS_API int _stdcall			FCWGetVersion(CUSBaccess *obj, int deviceNo) ;
	USBACCESS_API int _stdcall			FCWGetUSBType(CUSBaccess *obj, int deviceNo) ;
	USBACCESS_API int _stdcall			FCWGetSerialNumber(CUSBaccess *obj, int deviceNo) ;
	USBACCESS_API int _stdcall			FCWGetDLLVersion() ;
	USBACCESS_API int _stdcall			FCWGetManualOnCount(CUSBaccess *obj, int deviceNo) ;
	USBACCESS_API int _stdcall			FCWGetManualOnTime(CUSBaccess *obj, int deviceNo) ;	
	USBACCESS_API int _stdcall			FCWGetOnlineOnCount(CUSBaccess *obj, int deviceNo) ;
	USBACCESS_API int _stdcall			FCWGetOnlineOnTime(CUSBaccess *obj, int deviceNo) ;	
	USBACCESS_API int _stdcall			FCWGetMultiSwitch(CUSBaccess *obj, int deviceNo, unsigned long int *mask, unsigned long int *value, int seqNumber) ;
	USBACCESS_API int _stdcall			FCWSetMultiSwitch(CUSBaccess *obj, int deviceNo, unsigned long int value) ;
	USBACCESS_API int _stdcall			FCWSetMultiConfig(CUSBaccess *obj, int deviceNo, unsigned long int directions) ;
	USBACCESS_API int _stdcall			FCWGetCounter(CUSBaccess *obj, int deviceNo, int counter) ;
	USBACCESS_API int _stdcall			FCWSyncDevice(CUSBaccess *obj, int deviceNo, unsigned long int mask) ;
	USBACCESS_API void _stdcall		FCWDebugWrite(CUSBaccess *obj, char *s) ;

#endif // __USBACCESS_H__
