/*= Rohde&Schwarz CMD60 Include File     ====================================*/

#ifndef __PREFIX_HEADER
#define __PREFIX_HEADER

#include <vpptype.h>
#endif

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

/*****************************************************************************/
/*= Define Instrument Specific Error Codes Here =============================*/
/*****************************************************************************/
//#define VI_ERROR_PARAMETER9               (_VI_ERROR+0x3FFC0009L)

#define VI_ERROR_REGISTER_STB             (_VI_ERROR+0x3FFC0800L)
#define VI_ERROR_INV_MEAS                 (_VI_ERROR+0x3FFC0801L)

#define CMD60_ERROR_OFFSET                (_VI_ERROR+0x3FFC0900L)
#define CMD60_ERROR_STRING_LENGTH    (CMD60_ERROR_OFFSET + 0x00L)

/*****************************************************************************/
/*= GLOBAL USER-CALLABLE FUNCTION DECLARATIONS (Exportable Functions) =======*/
/*****************************************************************************/

ViStatus _VI_FUNC CMD60_Initialize (ViRsrc     resourceName, 
                                    ViBoolean  IDQuery,
                                    ViBoolean  resetDevice, 
                                    ViSession *InstrumentHandle);
                                    
ViStatus _VI_FUNC CMD60_ConfTest (ViSession  InstrumentHandle, 
                                  ViInt32    DutType);

ViStatus _VI_FUNC CMD60_ProcSelTest (ViSession  InstrumentHandle, 
                                     ViInt32    mode);

ViStatus _VI_FUNC CMD60_ConfPpRfpi (ViSession        InstrumentHandle, 
                                    ViChar _VI_FAR   Rfpi[]);

ViStatus _VI_FUNC CMD60_ConfPpDummMode (ViSession  InstrumentHandle, 
                                        ViInt32    mode);

ViStatus _VI_FUNC CMD60_ConfPpChannels (ViSession  InstrumentHandle, 
                                        ViInt32    dummySlotNumber,
                                        ViInt32    dummyCarrierNumber,
                                        ViInt32    trafficSlotNumber,
                                        ViInt32    trafficCarrierNumber, 
                                        ViReal64   carrierOffset);

ViStatus _VI_FUNC CMD60_ConfPpSignMode (ViSession  InstrumentHandle, 
                                        ViInt32    mode);

ViStatus _VI_FUNC CMD60_ConfPpRfLev (ViSession  InstrumentHandle, 
                                     ViReal64   RFLevelIndBm);

ViStatus _VI_FUNC CMD60_ConfPpQPacket (ViSession       InstrumentHandle, 
                                       ViChar _VI_FAR  staticSystemInfo[],
                                       ViChar _VI_FAR  fixedPartCapabilities[],
                                       ViChar _VI_FAR  multiframeNumber[], 
                                       ViChar _VI_FAR  additionalQPacket[],
                                       ViChar _VI_FAR  QPacketMultiplexSequence[]);

ViStatus _VI_FUNC CMD60_ConfFpAnt (ViSession  InstrumentHandle, 
                                   ViInt32    antennaNumber);

ViStatus _VI_FUNC CMD60_ConfFpRfpi (ViSession       InstrumentHandle, 
                                    ViChar _VI_FAR  Rfpi[]);

ViStatus _VI_FUNC CMD60_ConfFpPmid (ViSession       InstrumentHandle, 
                                    ViChar _VI_FAR  Pmid[]);

ViStatus _VI_FUNC CMD60_ConfFpChannels (ViSession  InstrumentHandle,
                                        ViInt32    trafficSlotNumber,
                                        ViInt32    trafficCarrierNumber, 
                                        ViReal64   carrierOffset);

ViStatus _VI_FUNC CMD60_ConfFpSignMode (ViSession  InstrumentHandle, 
                                        ViInt32    mode);

ViStatus _VI_FUNC CMD60_ConfFpRfLev (ViSession  InstrumentHandle, 
                                     ViReal64   RFLevelIndBm);
                                 
ViStatus _VI_FUNC CMD60_SystCommGpibAddr (ViSession  InstrumentHandle,
                                          ViInt32    BusAddress);

ViStatus _VI_FUNC CMD60_ConfMessEscMT (ViSession       InstrumentHandle, 
                                       ViChar _VI_FAR  MTEscapeSequence[]);

ViStatus _VI_FUNC CMD60_CalcLimPowTranTempCle (ViSession  InstrumentHandle);

ViStatus _VI_FUNC CMD60_CalcLimPowTranTempTol (ViSession  InstrumentHandle, 
                                               ViReal64   lowerNTP,
                                               ViReal64   upperNTP);

ViStatus _VI_FUNC CMD60_CalcLimPowTranTemp (ViSession  InstrumentHandle,
                                            ViReal64   absoluteLevelAtA,
                                            ViReal64   absoluteLevelAtB,
                                            ViReal64   relativeLevelAtC,
                                            ViReal64   relativeLevelAtD,
                                            ViReal64   relativeLevelAtE,
                                            ViReal64   relativeLevelAtF);

ViStatus _VI_FUNC CMD60_ConfPowDynRang (ViSession  InstrumentHandle, 
                                        ViInt32    dynamicRange);

ViStatus _VI_FUNC CMD60_ConfAverBurs (ViSession  InstrumentHandle, 
                                      ViInt32    numberOfBursts);

ViStatus _VI_FUNC CMD60_ConfPowTranOutpRang (ViSession  InstrumentHandle,
                                             ViReal64   startPosition,
                                             ViInt32    numberOfMeasurement);

ViStatus _VI_FUNC CMD60_CalcLimModCle (ViSession  InstrumentHandle);

ViStatus _VI_FUNC CMD60_CalcLimMod (ViSession  InstrumentHandle,
                                    ViReal64   frequencyOffsetInkHz, 
                                    ViReal64   BFieldLowerInkHz,
                                    ViReal64   BFieldUpperInkHz, 
                                    ViReal64   syncLowerInkHz,
                                    ViReal64   syncUpperInkHz, 
                                    ViReal64   frequencyDriftInHzperS);

ViStatus _VI_FUNC CMD60_ConfModDataType (ViSession  InstrumentHandle,
                                         ViInt32    dataTypeInBField);

ViStatus _VI_FUNC CMD60_CalcLimTimCle (ViSession  InstrumentHandle);

ViStatus _VI_FUNC CMD60_ConfTimAverBurs (ViSession  InstrumentHandle, 
                                         ViInt32    numberOfBursts);

ViStatus _VI_FUNC CMD60_CalcLimTime (ViSession  InstrumentHandle, 
                                     ViReal64   TimeAccuracy,
                                     ViReal64   TolForMaxNegJitter, 
                                     ViReal64   TolForMaxPosJitter,
                                     ViReal64   TolForMinPacketDelay, 
                                     ViReal64   TolForMaxPacketDelay);

ViStatus _VI_FUNC CMD60_CalcLimBerLterCle (ViSession  InstrumentHandle);

ViStatus _VI_FUNC CMD60_CalcLimBerLtermTol (ViSession  InstrumentHandle, 
                                            ViReal64   longTermBER,
                                            ViReal64   longTermFER);

ViStatus _VI_FUNC CMD60_ConfBerPpMbeClear (ViSession  InstrumentHandle,
                                           ViInt32    multiBearerArray);

ViStatus _VI_FUNC CMD60_ConfBerMbeSlot (ViSession  InstrumentHandle, 
                                        ViInt32    multiBearerArray,
                                        ViInt32    additionalSlotNumber);

ViStatus _VI_FUNC CMD60_ConfBerEvalWind (ViSession  InstrumentHandle,
                                         ViInt32    evaluationWindow);

ViStatus _VI_FUNC CMD60_ConfBerRfLev (ViSession  InstrumentHandle, 
                                      ViReal64   RFLevelIndBm);

ViStatus _VI_FUNC CMD60_CorrLoss (ViSession  InstrumentHandle, 
                                  ViInt32    source,
                                  ViReal64   externalAttenuation);

ViStatus _VI_FUNC CMD60_RoutIoc (ViSession  InstrumentHandle, 
                                 ViInt32    routeIO);

ViStatus _VI_FUNC CMD60_ConfSyncFreqRef (ViSession  InstrumentHandle, 
                                         ViInt32    frequency);

ViStatus _VI_FUNC CMD60_ConfSyncFreqOutpRef (ViSession  InstrumentHandle, 
                                             ViInt32    frequency);

ViStatus _VI_FUNC CMD60_ConfSyncPort (ViSession  InstrumentHandle, 
                                      ViInt32    port);

ViStatus _VI_FUNC CMD60_ConfAfg (ViSession  InstrumentHandle, 
                                 ViReal64   levelInVolt,
                                 ViReal64   frequencyInHz);

ViStatus _VI_FUNC CMD60_ConfAfgenAdpc (ViSession  InstrumentHandle, 
                                       ViInt32    speechCoder);

ViStatus _VI_FUNC CMD60_SourAfgStat (ViSession  InstrumentHandle, 
                                     ViInt32    switchingGenerator);

ViStatus _VI_FUNC CMD60_ConfAfmFreq (ViSession  InstrumentHandle,
                                     ViReal64   distortionFrequencyInHz,
                                     ViReal64   lowerLimitFrequencyInHz);

ViStatus _VI_FUNC CMD60_ConfAfmForm (ViSession  InstrumentHandle,
                                     ViInt32    displayFormatOfAFCounter,
                                     ViInt32    displayFormatOfVoltmeter);

ViStatus _VI_FUNC CMD60_ConfAfmInp (ViSession   InstrumentHandle, 
                                    ViInt32     input);

ViStatus _VI_FUNC CMD60_ConfAfmVoltBpas (ViSession  InstrumentHandle, 
                                         ViInt32    operatingMode,
                                         ViReal64   bandwidthInHz,
                                         ViReal64   centerFrequencyInHz);

ViStatus _VI_FUNC CMD60_ConfBanPowTranNormExp (ViSession  InstrumentHandle,
                                               ViReal64   expectedPowerOfMSIndBm);

ViStatus _VI_FUNC CMD60_ConfBanFreq (ViSession  InstrumentHandle, 
                                     ViReal64   frequencyInMHz);

ViStatus _VI_FUNC CMD60_ConfBanMeasWind (ViSession  InstrumentHandle, 
                                         ViInt32    window);

ViStatus _VI_FUNC CMD60_ConfBanTrig (ViSession  InstrumentHandle, 
                                     ViInt32    trigger,
                                     ViInt32    delayIns);

ViStatus _VI_FUNC CMD60_ConfRfg (ViSession       InstrumentHandle, 
                                 ViInt32         settingNumber,
                                 ViReal64        frequencyInMHz,
                                 ViReal64        deviationInkHz,
                                 ViInt32         signal, 
                                 ViReal64        RFLevelIndBm,
                                 ViChar _VI_FAR  signalPattern1to20[], 
                                 ViChar _VI_FAR  signalPattern21to40[],
                                 ViChar _VI_FAR  signalPattern41to60[], 
                                 ViChar _VI_FAR  signalPattern61to80[],
                                 ViChar _VI_FAR  signalPattern81to100[]);

ViStatus _VI_FUNC CMD60_SourRfgSelSett (ViSession  InstrumentHandle, 
                                        ViInt32    SettingNumber);
                                        
ViStatus _VI_FUNC CMD60_SourRfgFreqOutp (ViSession  InstrumentHandle, 
                                         ViReal64   FrequencyInMHz);
                                         
ViStatus _VI_FUNC CMD60_SourRfgFreqDev (ViSession  InstrumentHandle, 
                                        ViReal64   DeviationInkHz);
                                        
ViStatus _VI_FUNC CMD60_SourRfgRfLev (ViSession  InstrumentHandle, 
                                      ViReal64   RFLevelIndBm);
                                      
ViStatus _VI_FUNC CMD60_SourRfgSign (ViSession       InstrumentHandle, 
                                     ViInt32         Signal,
                                     ViChar _VI_FAR  SignalPattern1to20[],
                                     ViChar _VI_FAR  SignalPattern21to40[],
                                     ViChar _VI_FAR  SignalPattern41to60[],
                                     ViChar _VI_FAR  SignalPattern61to80[],
                                     ViChar _VI_FAR  SignalPattern81to100[]);

ViStatus _VI_FUNC CMD60_ProcConnSet (ViSession  InstrumentHandle);

ViStatus _VI_FUNC CMD60_ProcConnRel (ViSession  InstrumentHandle);

ViStatus _VI_FUNC CMD60_StatDev (ViSession        InstrumentHandle, 
                                 ViInt32 _VI_FAR *CurrentInstrumentState);



ViStatus _VI_FUNC CMD60_SensSignStat (ViSession        InstrumentHandle, 
                                      ViInt32 _VI_FAR *LockState);

ViStatus _VI_FUNC CMD60_SensDetDummCarr (ViSession        InstrumentHandle,
                                         ViInt32 _VI_FAR *DummyCarrierValue);

ViStatus _VI_FUNC CMD60_SensDetDummSlot (ViSession        InstrumentHandle, 
                                         ViInt32 _VI_FAR *DummySlotValue);

ViStatus _VI_FUNC CMD60_SensDetRfpi (ViSession       InstrumentHandle, 
                                     ViChar _VI_FAR  RfpiValue[]);

ViStatus _VI_FUNC CMD60_SensDetPmid (ViSession       InstrumentHandle, 
                                     ViChar _VI_FAR  PmidValue[]);

ViStatus _VI_FUNC CMD60_ReadAfmCoun (ViSession         InstrumentHandle, 
                                     ViReal64 _VI_FAR *AFCounter);

ViStatus _VI_FUNC CMD60_ReadAfmVolt (ViSession         InstrumentHandle, 
                                     ViReal64 _VI_FAR *AFVoltage);

ViStatus _VI_FUNC CMD60_ReadAfmDist (ViSession         InstrumentHandle, 
                                     ViReal64 _VI_FAR *AfDistortion);
                                    
ViStatus _VI_FUNC CMD60_ReadCurr (ViSession         InstrumentHandle, 
                                  ViReal64 _VI_FAR *CurrentDc,
                                  ViReal64 _VI_FAR *CurrentDcMax,
                                  ViReal64 _VI_FAR *CurrentDcMin);

ViStatus _VI_FUNC CMD60_ReadVolt (ViSession         InstrumentHandle, 
                                  ViReal64 _VI_FAR *VoltDc);

ViStatus _VI_FUNC CMD60_CalcLimPowTranTolMatc (ViSession        InstrumentHandle,
                                               ViInt32 _VI_FAR *VerdictForNTP);

ViStatus _VI_FUNC CMD60_CalcLimPowTranMatc (ViSession        InstrumentHandle,
                                            ViInt32 _VI_FAR *VerdictForPowerRamp);

ViStatus _VI_FUNC CMD60_ReadNtp (ViSession         InstrumentHandle, 
                                 ViReal64 _VI_FAR *NTPValue);

ViStatus _VI_FUNC CMD60_ReadArrPowTran (ViSession        InstrumentHandle, 
                                        ViReal64 _VI_FAR PowerValues[]);

ViStatus _VI_FUNC CMD60_CalcLimFreqOffsTolMatc (ViSession        InstrumentHandle,
                                                ViInt32 _VI_FAR *VerdictForFrequencyOffset);

ViStatus _VI_FUNC CMD60_CalcLimBfiTolMatc (ViSession        InstrumentHandle,
                                           ViInt32 _VI_FAR *VerdictForMaxPosBField,
                                           ViInt32 _VI_FAR *VerdictForMaxNegBField);

ViStatus _VI_FUNC CMD60_CalcLimSfiTolMatc (ViSession        InstrumentHandle,
                                           ViInt32 _VI_FAR *VerdictForMaxPosSyncField,
                                           ViInt32 _VI_FAR *VerdictForMaxNegSyncField);

ViStatus _VI_FUNC CMD60_CalcLimFreqDrifTolMatc (ViSession        InstrumentHandle,
                                                ViInt32 _VI_FAR *VerdictForFrequencyDrift);

ViStatus _VI_FUNC CMD60_ReadFreqOffs (ViSession         InstrumentHandle,
                                      ViReal64 _VI_FAR *FrequencyOffsetValue);

ViStatus _VI_FUNC CMD60_ReadBfi (ViSession         InstrumentHandle, 
                                 ViReal64 _VI_FAR *MaxPosBFieldValue,
                                 ViReal64 _VI_FAR *MaxNegBFieldValue);

ViStatus _VI_FUNC CMD60_ReadSfi (ViSession         InstrumentHandle, 
                                 ViReal64 _VI_FAR *MaxPosSFieldValue,
                                 ViReal64 _VI_FAR *MaxNegSFieldValue);

ViStatus _VI_FUNC CMD60_ReadFreqDrif (ViSession         InstrumentHandle,
                                      ViReal64 _VI_FAR *FrequencyDriftValue);

ViStatus _VI_FUNC CMD60_ReadArrFreqDev (ViSession         InstrumentHandle,
                                        ViReal64 _VI_FAR  FrequencyValues[]);

ViStatus _VI_FUNC CMD60_CalcLimTimeAccTolMatc (ViSession        InstrumentHandle,
                                               ViInt32 _VI_FAR *VerdictForTimeAccuracy);

ViStatus _VI_FUNC CMD60_CalcLimJittTolMatc (ViSession        InstrumentHandle,
                                            ViInt32 _VI_FAR *VerdictForMaxPosJitter,
                                            ViInt32 _VI_FAR *VerdictForMaxNegJitter);

ViStatus _VI_FUNC CMD60_CalcLimPackDelTolMatc (ViSession        InstrumentHandle,
                                               ViInt32 _VI_FAR *VerdictForMinPacketDelay,
                                               ViInt32 _VI_FAR *VerdictForMaxPacketDelay);

ViStatus _VI_FUNC CMD60_ReadTimeAcc (ViSession         InstrumentHandle, 
                                     ViReal64 _VI_FAR *TimeAccuracyValue);

ViStatus _VI_FUNC CMD60_ReadJitt (ViSession         InstrumentHandle, 
                                  ViReal64 _VI_FAR *MaxPosJitterValue,
                                  ViReal64 _VI_FAR *MaxNegJitterValue);

ViStatus _VI_FUNC CMD60_ReadPackDel (ViSession         InstrumentHandle,
                                     ViReal64 _VI_FAR *MinPacketDelayValue,
                                     ViReal64 _VI_FAR *MaxPacketDelayValue);

ViStatus _VI_FUNC CMD60_CalcLimBerLtermTolMatc (ViSession        InstrumentHandle,
                                                ViInt32 _VI_FAR *VerdictForBER,
                                                ViInt32 _VI_FAR *VerdictForFER);

ViStatus _VI_FUNC CMD60_ReadBerLter (ViSession         InstrumentHandle, 
                                     ViReal64 _VI_FAR *BERValue,
                                     ViReal64 _VI_FAR *FERValue);

ViStatus _VI_FUNC CMD60_ReadArrBerMbe (ViSession         InstrumentHandle,
                                       ViReal64 _VI_FAR  MultiBearerSlots[]);

ViStatus _VI_FUNC CMD60_CalcBanLimPowTranTolMatc (ViSession        InstrumentHandle,
                                                  ViInt32 _VI_FAR *VerdictForNTP);

ViStatus _VI_FUNC CMD60_ReadArrBanFreqDev (ViSession         InstrumentHandle,
                                           ViReal64 _VI_FAR  FrequencyValues[]);

ViStatus _VI_FUNC CMD60_ReadBanPowTranNorm (ViSession         InstrumentHandle, 
                                            ViReal64 _VI_FAR *NTPValue);

ViStatus _VI_FUNC CMD60_ReadBanFreqOffs (ViSession         InstrumentHandle,
                                         ViReal64 _VI_FAR *FrequencyOffsetValue);

ViStatus _VI_FUNC CMD60_ReadBanMod (ViSession         InstrumentHandle,
                                    ViReal64 _VI_FAR *MaxPosModulationValue,
                                    ViReal64 _VI_FAR *MaxNegModulationValue);

ViStatus _VI_FUNC CMD60_ReadArrBanPowTran (ViSession         InstrumentHandle,
                                           ViReal64 _VI_FAR  PowerValues[]);
                                  
/** INSERT USER-CALLABLE INSTRUMENT-DEPENDENT FUNCTION DECLARATIONS HERE **/

ViStatus _VI_FUNC CMD60_WriteInstrData (ViSession  InstrumentHandle, 
                                        ViString   WriteBuffer);

ViStatus _VI_FUNC CMD60_ReadInstrData (ViSession        InstrumentHandle, 
                                       ViInt16          NumberBytesToRead,
                                       ViChar _VI_FAR   ReadBuffer[], 
                                       ViInt32 _VI_FAR *NumBytesRead);

ViStatus _VI_FUNC CMD60_Reset (ViSession  InstrumentHandle);

ViStatus _VI_FUNC CMD60_SelfTest (ViSession       InstrumentHandle,
                                  ViPInt16        Result);

ViStatus _VI_FUNC CMD60_ErrorQuery (ViSession        InstrumentHandle, 
                                    ViInt32 _VI_FAR *ErrorCode,
                                    ViChar _VI_FAR   ErrorMessage[]);

ViStatus _VI_FUNC CMD60_ErrorMessage (ViSession       InstrumentHandle, 
                                      ViStatus        ErrorCode,
                                      ViChar _VI_FAR  ErrorMessage[]);

ViStatus _VI_FUNC CMD60_RevisionQuery (ViSession       InstrumentHandle,
                                       ViChar _VI_FAR  InstrumentDriverRevision[],
                                       ViChar _VI_FAR  FirmwareRevision[]);

ViStatus _VI_FUNC CMD60_Close (ViSession  InstrumentHandle);

#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif

/*****************************************************************************/
/*=== END INCLUDE FILE ======================================================*/
/*****************************************************************************/

