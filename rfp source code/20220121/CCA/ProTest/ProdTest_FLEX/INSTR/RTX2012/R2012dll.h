/****************************************************************************
*  Program/file: R2011dll.h
*
*  Copyright (C) by RTX TELECOM A/S, Denmark.
*  These computer program listings and specifications, are the property of
*  RTX TELECOM A/S, Denmark and shall not be reproduced or copied or used in
*  whole or in part without written permission from RTX TELECOM A/S, Denmark.
*
*  Programmer:
*
*  MODULE:
*  CONTROLLING DOCUMENT:
*  SYSTEM DEPENDENCIES:
*
*
*  DESCRIPTION: This headerfile defines the interface functions of the
*               RTX2011 DLL (R2011.dll).
*
*
****************************************************************************/

/****************************************************************************
*                                  PVCS info
*****************************************************************************

$Author:   lgj  $
$Date:   06 Jun 2013 14:18:46  $
$Revision:   1.2  $
$Modtime:   04 Mar 2013 08:27:46  $

*/

#ifndef _2011_DLL_H
#define _2011_DLL_H

/****************************************************************************
*                               Include files
****************************************************************************/
#define WIN32_LEAN_AND_MEAN   // Exclude rarely-used stuff from Windows headers

//#include <windows.h>
#include "RtxDef.h"

/****************************************************************************
*                              Macro definitions
****************************************************************************/

// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the PPDRV_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// PPDRV_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef CMD_2011_EXPORTS
 #define CMD_2011_API __declspec(dllexport)
#else
 #define CMD_2011_API __declspec(dllimport)
#endif


/****************************************************************************
*                     Enumerations/Type definitions/Structs
****************************************************************************/


/****************************************************************************
*                     Variable decaration
****************************************************************************/

/****************************************************************************
*                             Function prototypes
****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------------------------------------------
;* DESCRIPTION: Send a SCPI compatible command to test quipment
;* INPUT:  Pointer to a zero-terminated ASCII string
;* OUTPUT: Number of error encountered in SCPI passing or communication
;*----------------------------------------------------------------------------*/
CMD_2011_API int16 _stdcall RtxWrt(int8 *Cmd);
CMD_2011_API int16 _stdcall vbRtxWrt(int8 *Cmd);

/*------------------------------------------------------------------------------
;* DESCRIPTION: Read response from test equipment to a SCPI query
;* INPUT:  Pointer to destionation for a zero-terminated ASCII string
;* OUTPUT: Number characters in return string
;*----------------------------------------------------------------------------*/
CMD_2011_API int16 _stdcall RtxRd(int8 far *Dest);
CMD_2011_API int16 _stdcall vbRtxRd(PUCHAR Buffer);

/*-----------------------------------------------------------------------------
;* Functions to retrieve record samples of NTP and demodulated data
;*
-------------------------------------------------------------------------------*/
CMD_2011_API int16 _stdcall ReadRxValue(uint16 Position);
CMD_2011_API int16 _stdcall ReadPwrValue(uint16 Position);
CMD_2011_API int16 _stdcall vbReadRxValue(uint16 Position);
CMD_2011_API int16 _stdcall vbReadPwrValue(uint16 Position);
CMD_2011_API int16 _stdcall ReadRxValueBlock(uint16 Position, uint16 Len, int16* far BlockPtr);

/*-----------------------------------------------------------------------------
;* Function to force (trig) sampling of a single burst.
/*-----------------------------------------------------------------------------*/
CMD_2011_API void _stdcall ForceADCSample(void);
CMD_2011_API uint16 _stdcall vbForceADCSample(uint16 Test);

#ifdef __cplusplus
}
#endif

#endif // _2011_DLL_H



