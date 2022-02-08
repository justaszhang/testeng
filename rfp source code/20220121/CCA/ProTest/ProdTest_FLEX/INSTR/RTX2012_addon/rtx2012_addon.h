#ifndef __rtx2012_addon_H__
#define __rtx2012_addon_H__

#ifdef __cplusplus
    extern "C" {
#endif
		
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

#include "cvidef.h"


//int CMD60_addon_Register(int InstrSession, char buf[]);
int rtx2012_addon_ClearError (void);  
//int rtx2012_addon_SendCommand(unsigned char *cCmd);
//int CMD60_addon_GetFirmwareVersion(int InstrSession,char *firmwareVersion);
//int CMD60_addon_GetOption(int InstrSession,char *sysOption);
int rtx2012_addon_ReadNormalTransmitPower (double *transPower);
//int CMD60_addon_ReadAfmCoun (int InstrSession,double *freq);
int rtx2012_addon_activate_SW_Demod();
int rtx2012_addon_deactivate_SW_Demod();
int rtx2012_addon_Connect (int type, char *cErrorInfo);   
int rtx2012_addon_SetPacketTypePP32Z (void);   
int rtx2012_addon_ReadFpRfpi (char *rfpiValue); 



		
		
		
		
		
		
		
		

#ifdef __cplusplus
    }
#endif

#endif  /* ndef __rtx2012_addon_H__ */
