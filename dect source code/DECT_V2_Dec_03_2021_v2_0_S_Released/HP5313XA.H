
/*= <HP 53131/2 Universal Counter > Include File ============================*/

/*===========================================================================*/
/*  Please do not use global variables or arrays in the include file of      */
/*  instrument drivers that will be submitted for inclusion into the         */
/*  LabWindows Instrument Driver Library.                                    */
/*===========================================================================*/
     
#ifndef __hp5313xa_HEADER
#define __hp5313xa_HEADER

#include <vpptype.h>

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif


/*****************************************************************************/
/*= Instrument Driver Specific Error/Warning Codes ==========================*/
/*****************************************************************************/
#define VI_ERROR_INSTR_FILE_OPEN                (_VI_ERROR+0x3FFC0800L)
#define VI_ERROR_INSTR_FILE_WRITE               (_VI_ERROR+0x3FFC0801L)
#define VI_ERROR_INSTR_INTERPRETING_RESPONSE    (_VI_ERROR+0x3FFC0803L)
#define VI_ERROR_INSTR_SYSTEM_ERROR             (_VI_ERROR+0x3FFC0804L)
#define VI_ERROR_INSTR_UPLOW_LIMIT              (_VI_ERROR+0x3FFC0805L)
#define VI_ERROR_INSTR_PARAMETER9               (_VI_ERROR+0x3FFC0809L)
#define VI_ERROR_INSTR_PARAMETER10              (_VI_ERROR+0x3FFC080AL)
#define VI_ERROR_INSTR_PARAMETER11              (_VI_ERROR+0x3FFC080BL)
#define VI_ERROR_INSTR_PARAMETER12              (_VI_ERROR+0x3FFC080CL)
#define VI_ERROR_INSTR_PARAMETER13              (_VI_ERROR+0x3FFC080DL)
#define VI_ERROR_INSTR_PARAMETER14              (_VI_ERROR+0x3FFC080EL)
#define VI_ERROR_INSTR_PARAMETER15              (_VI_ERROR+0x3FFC080FL)

/*****************************************************************************/
/*= Define Instrument Specific Error/Warning Codes Here =====================*/
/*****************************************************************************/
#define VI_WARNING_INSTR_OFFSET                           (0x3FFC0900L)
#define VI_ERROR_INSTR_OFFSET                   (_VI_ERROR+0x3FFC0900L)

/*****************************************************************************/
/*= GLOBAL USER-CALLABLE FUNCTION DECLARATIONS (Exportable Functions) =======*/
/*****************************************************************************/
ViStatus _VI_FUNC hp5313xa_init (ViRsrc resourceName, ViBoolean IDQuery,
                    ViBoolean resetDevice, ViInt16 instrumentModel,
                    ViPSession instrumentHandle);
ViStatus _VI_FUNC hp5313xa_inputCond (ViSession instrumentHandle, ViInt16 channel,
                    ViBoolean coupling, ViBoolean impedanceOhms,
                    ViBoolean attenuation, ViBoolean filter);
ViStatus _VI_FUNC hp5313xa_trigSens (ViSession instrumentHandle, ViInt16 channel,
                    ViBoolean triggerLevel, ViReal64 voltageLevel,
                    ViBoolean attenuation, ViBoolean triggerSlope,
                    ViInt16 sensitivity);
ViStatus _VI_FUNC hp5313xa_limit (ViSession instrumentHandle, ViBoolean limitTest,
                    ViBoolean onFail, ViBoolean show,
                    ViReal64 lowerLimit, ViReal64 upperLimit);
ViStatus _VI_FUNC hp5313xa_math (ViSession instrumentHandle, ViBoolean math,
                    ViReal64 scaleValue, ViReal64 offsetValue);
ViStatus _VI_FUNC hp5313xa_savRecSet (ViSession instrumentHandle,
                    ViBoolean saveRecall, ViInt16 memoryLocation);
ViStatus _VI_FUNC hp5313xa_freqPeriodRatio (ViSession instrumentHandle,
                    ViInt16 selectFunction,
                    ViInt16 selectArming,
                    ViReal64 timeDigits,
                    ViBoolean startSlope, ViInt16 stop);
ViStatus _VI_FUNC hp5313xa_phase (ViSession instrumentHandle,
                    ViBoolean selectArming, ViBoolean startSlope);
ViStatus _VI_FUNC hp5313xa_totalize (ViSession instrumentHandle,
                    ViInt16 selectArming, ViReal64 timeValue,
                    ViBoolean startSlope, ViInt16 stop);
ViStatus _VI_FUNC hp5313xa_timWidDuty (ViSession instrumentHandle,
                    ViInt16 selectfunction,
                    ViBoolean selectArming, ViBoolean startSlope);
ViStatus _VI_FUNC hp5313xa_timeInterval (ViSession instrumentHandle,
                    ViBoolean startArming,
                    ViBoolean startSlope, ViBoolean stop,
                    ViReal64 timeValue, ViBoolean commonMode);
ViStatus _VI_FUNC hp5313xa_confblock (ViSession instrumentHandle,
                    ViBoolean statistics, ViInt32 ofMeasurements,
                    ViBoolean filterData, ViInt16 show,
                    ViReal64 lowerLimit, ViReal64 upperLimit);
ViStatus _VI_FUNC hp5313xa_confFunc (ViSession instrumentHandle,
                    ViInt16 selectFunction);
ViStatus _VI_FUNC hp5313xa_strStpMeas (ViSession instrumentHandle,
                    ViInt16 executionMode);
ViStatus _VI_FUNC hp5313xa_initReg (ViSession instrumentHandle);
ViStatus _VI_FUNC hp5313xa_setSrqCond (ViSession instrumentHandle,
                    ViInt16 SRQCondition);
ViStatus _VI_FUNC hp5313xa_waitSrq (ViSession instrumentHandle);
ViStatus _VI_FUNC hp5313xa_initMeas (ViSession instrumentHandle);
ViStatus _VI_FUNC hp5313xa_postProcess (ViSession instrumentHandle,
                    ViInt16 postProcess);
ViStatus _VI_FUNC hp5313xa_queryMeas (ViSession instrumentHandle,
                    ViInt16 selectFunction,
                    ViInt16 selectMeasMethod,
                    ViPReal64 measuredValue);
ViStatus _VI_FUNC hp5313xa_dataRes (ViSession instrumentHandle,
                    ViPReal64 measuredData);
ViStatus _VI_FUNC hp5313xa_mathRes (ViSession instrumentHandle,
                    ViPReal64 scaledOffsetData);
ViStatus _VI_FUNC hp5313xa_limitRes (ViSession instrumentHandle,
                    ViPInt16 dataWithinLimit, ViPInt16 failCount,
                    ViPInt16 passCount, ViPInt16 lowerFailures,
                    ViPInt16 upperFailures);
ViStatus _VI_FUNC hp5313xa_statisticsRes (ViSession instrumentHandle,
                    ViPReal64 mean, ViPReal64 stdDeviation,
                    ViPReal64 maximum, ViPReal64 minimum);
ViStatus _VI_FUNC hp5313xa_queryChan3 (ViSession instrumentHandle,
                    ViPBoolean inputCoupling,
                    ViPBoolean inputImpedance);
ViStatus _VI_FUNC hp5313xa_enaDisSet (ViSession instrumentHandle,
                    ViBoolean limitTest, ViBoolean math,
                    ViBoolean filterData, ViBoolean statistics);
ViStatus _VI_FUNC hp5313xa_writeInstrData (ViSession instrumentHandle,
                    ViString writeBuffer);
ViStatus _VI_FUNC hp5313xa_readInstrData (ViSession instrumentHandle,
                    ViInt16 numberBytesToRead,
                    ViChar _VI_FAR readBuffer[],
                    ViPInt32 numBytesRead);
ViStatus _VI_FUNC hp5313xa_reset (ViSession instrumentHandle);
ViStatus _VI_FUNC hp5313xa_selfTest (ViSession instrumentHandle,
                    ViPInt16 selfTestResult,
                    ViChar _VI_FAR selfTestMessage[]);
ViStatus _VI_FUNC hp5313xa_errorQuery (ViSession instrumentHandle,
                    ViPInt32 errorCode,
                    ViChar _VI_FAR errorMessage[]);
ViStatus _VI_FUNC hp5313xa_errorMessage (ViSession instrumentHandle,
                    ViStatus errorCode,
                    ViChar _VI_FAR errorMessage[]);
ViStatus _VI_FUNC hp5313xa_revisionQuery (ViSession instrumentHandle,
                    ViChar _VI_FAR instrumentDriverRevision[],
                    ViChar _VI_FAR firmwareRevision[]);
ViStatus _VI_FUNC hp5313xa_close (ViSession instrumentHandle);


#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif

/*****************************************************************************/
/*=== END INCLUDE FILE ======================================================*/
/*****************************************************************************/

#endif
