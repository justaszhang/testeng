#include <cvidef.h>












int __stdcall rtx2012_Write (char *command);	   
int __stdcall rtx2012_Query (char *command, char *output)     ;
int __stdcall rtx2012_Initialize (char *output);    

int __stdcall rtx2012_ProcConnSet (void);

int __stdcall rtx2012_ConfFpSignMode (int mode);

int __stdcall rtx2012_ConfFpAnt (int antennaNumber);

int __stdcall rtx2012_ConfFpRfLev (double RFLevel_dBm);

int __stdcall rtx2012_ConfFpChannels (int trafficSlotNumber,
                                      int trafficCarrierNumber,
                                      double carrierOffset);

int __stdcall rtx2012_ConfFpRfpi (char RFPI[]);

int __stdcall rtx2012_ConfBerEvalWind (int evaluationWindow);

int __stdcall rtx2012_ConfBerRfLev (double RFLevel_dBm);

int __stdcall rtx2012_ConfModDataType (int dataTypeInBField);

int __stdcall rtx2012_Reset (void);

int __stdcall rtx2012_ErrorQuery (int *Error_Code, char errorMessage[]);

int __stdcall rtx2012_SensSignStat (int *lockState);

int __stdcall rtx2012_SensDetRfpi (char RFPIValue[]);

int __stdcall rtx2012_CalcLimPowTranTolMatc (int *verdictForNTP);

int __stdcall rtx2012_CalcLimPowTranMatc (int *verdictForPowerRamp);

int __stdcall rtx2012_ReadNtp (double *NTPValue);

int __stdcall rtx2012_ReadBerLter (double *BERValue, double *FERValue);

int __stdcall rtx2012_Loopback_Mode_EU_Setting (void);   
int __stdcall rtx2012_Nonsignal_Connect (void); 

int __stdcall rtx2012_ConfMessEscMT (char MTEscapeSequence[]);

int __stdcall rtx2012_StatDev (int *currentInstrumentState);

int __stdcall rtx2012_ConfTimAverBurs (int numberOfBursts);

int __stdcall rtx2012_ConfAverBurs (int numberOfBursts);









int __stdcall rtx2012_ConfTest (int DUTType);

int __stdcall rtx2012_ProcSelTest (int mode);

int __stdcall rtx2012_CalcLimPowTranTempCle (void);

int __stdcall rtx2012_CalcLimPowTranTempTol (double lowerNTP_dBm,
                                             double upperNTP_dBm);

int __stdcall rtx2012_ProcConnRel (void);

int __stdcall rtx2012_Read_NTP (char *power);   

int __stdcall rtx2012_Read_FrequencyOffset (char *freqoffset);  

int __stdcall rtx2012_Read_Deviation (char *BField);

int __stdcall rtx2012_Read_FrequencyDriftSlot (char *freqDriftSlot);  

int __stdcall rtx2012_Read_TemplateMatch (char *matching);    

