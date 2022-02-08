/** Cordless Technology A/S *************************
 * $Workfile:   bios.h  $
 * $Revision:   2.0  $
 *   $Author:   LM  $
 *****************************************************
 *      $Log:   K:\PVCS\VCS\TEST3G\DRIVERS\BIOS.H_V  $
 * 
 *    Rev 2.0   25 Feb 1997 14:12:16   LM
 * External release for R&S
 * 
 ******************************************************/

#ifndef _BIOS_H
#define _BIOS_H

//the globally defined types
#ifndef _GLOBAL_H
  typedef unsigned char BYTE8;
  typedef unsigned short WORD16;
  typedef short INT16;
  typedef int INT32;
  typedef unsigned int UINT32;
#endif

// the types we communicate with
#define FP  0
#define PP  1

//Keycodes used
#define HOOK_ON_KEY  3
#define HOOK_OFF_KEY 4

// The IIC-adresses for the devices
#define PPPROCESSORADR                  0x10
#define FPPROCESSORADR                  0x20
#define PPEEPROMADR                     0xA0
#define FPEEPROMADR                     0xA8


// the maixmum number of data in a mail structure
#define MAX_MAIL_LENGTH 100

WORD16 BIOS_GetBoardAdr (void);
void BIOS_SetBoardAdr (WORD16);
BYTE8 BIOS_GetDUT (void);
void BIOS_SetDUT (BYTE8);
BYTE8 BIOS_GetPID (void);
void BIOS_SetPID (BYTE8);
void BIOS_InitIICBoard(BYTE8,BYTE8);

INT32 BIOS_Open(BYTE8);
INT32 BIOS_Close(void);

INT32 BIOS_SendMail (BYTE8, WORD16, BYTE8 *,BYTE8);
INT32 BIOS_ReceiveMail (WORD16, BYTE8 *,BYTE8 *);

INT32 BIOS_WriteRAM (BYTE8, WORD16, BYTE8 *,BYTE8);
INT32 BIOS_WriteRAMByte (BYTE8, WORD16, BYTE8);
INT32 BIOS_ReadRAM (BYTE8, WORD16, BYTE8 *,BYTE8 );
INT32 BIOS_ReadRAMByte (BYTE8, WORD16, BYTE8 *);
INT32 BIOS_ModifyRAMByte (BYTE8, WORD16,BYTE8, BYTE8);

// EEPROM functions
INT32 BIOS_EEPROMProtected(void);
void BIOS_ProtectEEPROM(void);
void BIOS_UnprotectEEPROM(void);
INT32 BIOS_WriteEEPROM (WORD16, BYTE8 *,INT32);
INT32 BIOS_WriteEEPROMByte (WORD16, BYTE8);
INT32 BIOS_ReadEEPROM (WORD16, BYTE8 *,WORD16);
INT32 BIOS_ReadEEPROMByte (WORD16, BYTE8 *);

INT32 BIOS_SetEEPROMServiceBit(void);

INT32 BIOS_SWReset (void);
INT32 BIOS_EnterTestmode(void);
INT32 BIOS_ExitTestmode(void);
INT32 BIOS_GetUnitVersion(BYTE8 *);

INT32 BIOS_SetClock(BYTE8);

/************ Battery measurement ********/
INT32 BIOS_BattMeasVolt(BYTE8 *bResult);
INT32 BIOS_BattMeasTemp(BYTE8 *bResult);

/************ Audio Buzzer functions ********/
INT32 BIOS_StartTone(BYTE8);
INT32 BIOS_StopTone(void);
INT32 BIOS_StartTestRingTone(void);
INT32 BIOS_StopTestRingTone(void);

INT32 BIOS_StartBuzzerTone(void);
INT32 BIOS_StopBuzzerTone(void);
INT32 BIOS_MuteMic(void);
INT32 BIOS_UnMuteMic(void);
INT32 BIOS_OpenAudio(void);
INT32 BIOS_CloseAudio(void);

INT32 BIOS_OpenAudioLoopback(void);
INT32 BIOS_CloseAudioLoopback(void);

/*************** DSP functions **************/
INT32 BIOS_SetDSPIOMMode (void);

INT32 BIOS_WriteDSPRAM (BYTE8, BYTE8, WORD16);
INT32 BIOS_WriteLowerDSPRAM(BYTE8, WORD16);
INT32 BIOS_WriteUpperDSPRAM(BYTE8, WORD16);
INT32 BIOS_WriteYDSPRAM(BYTE8, WORD16);

INT32 BIOS_ReadDSPRAM (BYTE8, BYTE8, WORD16 *);
INT32 BIOS_ReadLowerDSPRAM(BYTE8, WORD16 *);
INT32 BIOS_ReadUpperDSPRAM(BYTE8, WORD16 *);
INT32 BIOS_ReadYDSPRAM(BYTE8, WORD16 *);

INT32 BIOS_SetTAMSwitch (BYTE8);
INT32 BIOS_GetAudioState(BYTE8*);

/************* Display Task functions *******/
INT32 BIOS_WriteChar(BYTE8);
INT32 BIOS_ClearDisplay(void);
INT32 BIOS_StopBlink(void);
INT32 BIOS_StopSymbolBlink(void);
INT32 BIOS_DisableDisplay(void);

/************* Keydriver Task functions *******/
INT32 BIOS_ActivateKey(BYTE8);
INT32 BIOS_GetKeypressed(BYTE8*);


/************* FP Line Task functions ********/
INT32 BIOS_LineAGCEnable(void);
INT32 BIOS_LineAGCDisable(void);
INT32 BIOS_EnableLineShort(void);
INT32 BIOS_DisableLineShort(void);
INT32 BIOS_ConnectEToGND(void);
INT32 BIOS_DisconnectEToGND(void);
INT32 BIOS_EnableLineBreak(void);
INT32 BIOS_DisableLineBreak(void);
INT32 BIOS_HookOn(void);
INT32 BIOS_HookOff(void);
INT32 BIOS_ReadRingDetector(BYTE8*);


/************* BMC and HF functions *********/
INT32 BIOS_EnterLowlevelTestmode(void);
INT32 BIOS_ExitLowlevelTestmode(void);

INT32 BIOS_BMCSetAfieldContent(BYTE8);
INT32 BIOS_BMCSetBfieldContent(BYTE8);
INT32 BIOS_BMCSetPseudoRandomBfield(void);
INT32 BIOS_BMCSetConstantBfield(void);

INT32 BIOS_TxChannel(BYTE8);
INT32 BIOS_RxChannel(BYTE8);

INT32 BIOS_BMCSetTxChannel(BYTE8);
INT32 BIOS_BMCChangeTXChannel (BYTE8);
INT32 BIOS_BMCSetRxChannel(BYTE8);
INT32 BIOS_BMCChangeRXChannel (BYTE8);
INT32 BIOS_BMCSwitchAntenna(BYTE8);
INT32 BIOS_BMCSetQuadrature(BYTE8);
INT32 BIOS_BMCInitQuad(void);
INT32 BIOS_BMCSetQuadDCOffset(BYTE8,BYTE8);
INT32 BIOS_BMCSetRGMSK(BYTE8);
INT32 BIOS_BMCSetInternalVREF(BYTE8);
INT32 BIOS_BMCSetPolarity(BYTE8 bPol1 ,BYTE8 bPol2);
INT32 BIOS_BMCGetPolarity(BYTE8 *,BYTE8 *);
INT32 BIOS_BMCChipEnable(BYTE8);

INT32 BIOS_EnableREFClk(void);
INT32 BIOS_DisableREFClk(void);

INT32 BIOS_EnableCharger(void);
INT32 BIOS_DisableCharger(void);

INT32 BIOS_InitRSSI(BYTE8);
INT32 BIOS_GetRSSI(BYTE8*);
INT32 BIOS_SetupDummyBearer(BYTE8,BYTE8);
INT32 BIOS_BMCSetDiversity(WORD16,BYTE8);
INT32 BIOS_GetServiceState (BYTE8*);

/*Compound sequences for HF Development*/
/*Do NOT use these for production*/
INT32 BIOS_HF_DefaultRX(BYTE8);
INT32 BIOS_HF_DefaultTX(BYTE8);
INT32 BIOS_HF_BMCSetTxChannel(BYTE8 bChannel);
INT32 BIOS_HF_BMCSetRxChannel(BYTE8 bChannel);

/*** Compound sequences for Production***/
INT32 BIOS_DefaultRF(void);
INT32 BIOS_DefaultRX(BYTE8);


#ifndef COPYRIGHT
  static const char COPYRIGHT[] = "[Copyright Cordless Technology A/S]"; 
  #define COPYRIGHT
#endif

#ifndef __bios_h__
static const char _bios_h_[]  = "[$Workfile::   bios.h  $ - $Revision::   2.0   $]";
  #define __bios_h__
#endif

#endif
