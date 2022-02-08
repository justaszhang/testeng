#ifndef CMD60_ADDON_H
#define CMD60_ADDON_H
//==============================================================================
//
// Title:       CMD60_addon.h
//
// Purpose:     CMD60 Add-on Funktionen
//
// Created on:  12.04.2012 by G.Cordes
// Copyright:   . All Rights Reserved.
//
//==============================================================================


int CMD60_addon_Register(int InstrSession, char buf[]);
int CMD60_addon_ClearError(int InstrSession);
int CMD60_addon_SetPacketTypePP32Z (int InstrSession);
int CMD60_addon_SendCommand(int InstrSession, unsigned char *cCmd);
int CMD60_addon_GetFirmwareVersion(int InstrSession,char *firmwareVersion);
int CMD60_addon_GetOption(int InstrSession,char *sysOption);
int CMD60_addon_ReadNormalTransmitPower (int InstrSession,double *transPower);
int CMD60_addon_ReadAfmCoun (int InstrSession,double *freq);
int CMD60_addon_activate_SW_Demod(int InstrSession);
int CMD60_addon_deactivate_SW_Demod(int InstrSession);
int CMD60_addon_ReadFpRfpi (int InstrSession, char RfpiValue[]);
int CMD60_addon_Connect(int InstrSession,int type,char *cErrorInfo);




#endif //CMD60_ADDON_H
