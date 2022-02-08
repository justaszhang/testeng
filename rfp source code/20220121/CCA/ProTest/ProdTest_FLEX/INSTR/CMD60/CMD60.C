/*
 *= Rohde&Schwarz CMD60 =======================================================
 *
 * Instrument Driver
 * Compiler name and release(s) : LabWindows/CVI Version 4.0.1.
 *                                Compatible with LabWindows/CVI Version 3.1.
 * Original Release : June, 1996
 *
 * By : ROHDE & SCHWARZ FRANCE - IS3i
 *      R&S CONTACTS - Hermann LANGER
 *              Phone : 00 33 1 41 36 11 17
 *              Fax   : 00 33 1 41 36 10 10
 *
 *      Author - Stephane BLANCHON
 *              Fax.  : 00 33 1 69 31 40 36
 *
 * Version : 2.0
 * 
 * ----------------------------------------------------------------------------
 *                         MODIFICATON HISTORY 
 * ----------------------------------------------------------------------------
 *  Date     | Author     | Society | Description
 * ----------------------------------------------------------------------------
 *  28.06.96 | S.BLANCHON | IS3i    | Creation (delivery version).
 *      V1.0 |            |         |   Version 1.0.
 * ----------------------------------------------------------------------------
 *  22.11.96 | S.BLANCHON | IS3i    | Correction:
 *      V1.1 |            |         |   Declare  'CMD60_ProcConnSet'  and 
 *           |            |         |   'CMD60_ProcConnRel' with _VI_FUNC.
 *           |            |         |   Remove compare string with 'NAN' in
 *           |            |         |   'CMD60_ReadAfmDist' function.
 *           |            |         |   Verify string length in :
 *           |            |         |     'CMD60_ConfPpRfpi'
 *           |            |         |     'CMD60_ConfPpQPacket'
 *           |            |         |     'CMD60_ConfFpRfpi'
 *           |            |         |     'CMD60_ConfFpPmid'
 *           |            |         |     'CMD60_ConfMessEscMT'
 *           |            |         |     'CMD60_ConfRfg'
 *           |            |         |     'CMD60_SourRfgSign'
 * ----------------------------------------------------------------------------
 *  25.11.96 | S.BLANCHON | IS3i    | Update for VISA 1.0 and CVI 4.0.1:
 *      V1.2 |            |         |   Remove calls functions 'viFindRsrc' and
 *           |            |         |   'viFindNext' in CMD60_Initialize func.
 * ----------------------------------------------------------------------------
 *  26.11.96 | S.BLANCHON | IS3i    | Modification:
 *      V1.3 |            |         |   Remote control on serial port via  VISA
 *           |            |         |   functions: change CMD60_Initialize func
 *           |            |         |   -tion (init serial port: baud rate ...)
 *           |            |         |   and change all functions using  ViWrite
 *           |            |         |   (append '\n' caracter  at  the  end  of
 *           |            |         |   string command send to the CMD60).
 * ----------------------------------------------------------------------------
 *  27.11.96 | S.BLANCHON | IS3i    | Addition:
 *      V1.4 |            |         |   'CMD60_ReadCurr' function.
 *           |            |         |   'CMD60_ReadVolt' function.
 * ----------------------------------------------------------------------------
 *  04.12.96 | S.BLANCHON | IS3i    | Modification:
 *      V1.5 |            |         | User defines are not compatible under 
 *           |            |         | LabWindows/CVI 3.1.
 * ----------------------------------------------------------------------------
 *  04.12.96 | S.BLANCHON | IS3i    | Make a delivery version: 
 *      V2.0 |            |         |   Version 2.0.
 *=============================================================================
 */

#include <utility.h>
#include <visa.h>
#include <ansi_c.h>
#include <string.h>
#include <formatio.h>
#include "cmd60.h"

#define CMD60_REVISION     "Rev 2.0, 12/96, CVI 4.0.1"    /* Revision driver */
#define BUFFER_SIZE         4000L /* 5000L */         	  /* File I/O buffer size [@KT250297]*/



/*****************************************************************************/
/*= INSTRUMENT-DEPENDENT STATUS/RANGE STRUCTURE  ============================*/
/*****************************************************************************/
/* CMD60_stringValPair is used in the CMD60_errorMessage function            */
/* CMD60_statusDataRanges is used to track session dependent status & ranges */
/*===========================================================================*/
typedef struct CMD60_stringValPair
{
	ViStatus stringVal;
	ViString stringName;
} CMD60_tStringValPair;
 
struct CMD60_statusDataRanges {
	ViInt16 triggerMode;
	ViInt16 val2;
	ViInt16 val3;
	ViChar  instrDriverRevision[256];
};

typedef struct CMD60_statusDataRanges *CMD60_instrRange;

/*****************************************************************************/
/*= UTILITY ROUTINE DECLARATIONS (Non-Exportable Functions) =================*/
/*****************************************************************************/

ViBoolean CMD60_invalidViBooleanRange (ViBoolean  val);

ViBoolean CMD60_invalidViInt16Range   (ViInt16  val,
                                       ViInt16  min, 
                                       ViInt16  max);

ViBoolean CMD60_invalidViInt32Range   (ViInt32  val, 
                                       ViInt32  min, 
                                       ViInt32  max);

ViBoolean CMD60_invalidViUInt16Range  (ViUInt16  val, 
                                       ViUInt16  min, 
                                       ViUInt16  max);

ViBoolean CMD60_invalidViUInt32Range  (ViUInt32  val, 
                                       ViUInt32  min, 
                                       ViUInt32  max);

ViBoolean CMD60_invalidViReal32Range  (ViReal32  val,
                                       ViReal32  min, 
                                       ViReal32  max);

ViBoolean CMD60_invalidViReal64Range  (ViReal64  val, 
                                       ViReal64  min, 
                                       ViReal64  max);

ViStatus  CMD60_initCleanUp           (ViSession   openRMSession, 
                                       ViPSession  openInstrSession, 
                                       ViStatus    currentStatus);

/*---------------------------------------------------------------------------*/
/*------------- INSERT INSTRUMENT-DEPENDENT ROUTINES HERE -------------------*/
/*---------------------------------------------------------------------------*/

/*= ROUTINES - NIVEAU 2 - ===================================================*/

/*===========================================================================*/
/* Function :
/*     CMD60_Register
/* Purpose :
/*     If the parameter buf is different to 0 and 16 then one error is coming.
/* Parameters :
/*     InstrSession 
/*         Session of the instrument driver.
/*     buf 
/*         STB value.
/*         Value range :
/*         String (ASCII format).
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_Register (ViSession       InstrSession, 
                                  ViChar _VI_FAR  buf[])
{
	ViStatus  CMD60_status = VI_SUCCESS;
	ViInt32   reg_STB;

	Scan (buf, "%s>%d", &reg_STB);
	
	if ((reg_STB != 0) && (reg_STB != 16))
		CMD60_status = VI_ERROR_REGISTER_STB;

	return CMD60_status;
}

/*= END of ROUTINES - NIVEAU 2 - ============================================*/


/*****************************************************************************/
/*====== USER-CALLABLE FUNCTIONS (Exportable Functions) =====================*/
/*****************************************************************************/

/*===========================================================================*/
/* Function :
/*     CMD60_Initialize
/* Purpose :
/*     This function performs the following initialization actions:
/*     - Opens a session to the Default Resource Manager resource and a session
/*       to the specified device using the interface and address specified in
/*       the resourceName variable.
/*     - Performs an identification query on the Instrument.
/*     - Resets the instrument to a known state.
/* Parameters :
/*     resourceName 
/*         Interface and address of the device.
/*     IDQuery
/*         A ID Query flag.
/*         Value range :
/*         0 - Skip Query.
/*         1 - Do Query.
/*     reset
/*         A reset flag.
/*         Value range :
/*         0 - Don't Reset.
/*         1 - Reset Device.
/*     InstrSession
/*         An Instrument Handle which is used to differentiate between 
/*         different sessions of this instrument driver.
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_Initialize (ViRsrc      resourceName, 
                                    ViBoolean   IDQuery,
                                    ViBoolean   reset, 
                                    ViPSession  InstrSession)
{
	ViStatus    CMD60_status = VI_SUCCESS;
	ViSession   rmSession = 0;
	ViUInt32    retCnt = 0;
	ViByte      rdBuffer[BUFFER_SIZE];
	int			PosIDN;

	/*- Check input parameter ranges ----------------------------------------*/

	if (CMD60_invalidViBooleanRange (IDQuery))
	    return VI_ERROR_PARAMETER2;
	
	if (CMD60_invalidViBooleanRange (reset))
		return VI_ERROR_PARAMETER3;


	/*- Open instrument session ---------------------------------------------*/

	if ((CMD60_status = viOpenDefaultRM (&rmSession)) < 0)
		return CMD60_status;

	if ((CMD60_status = viOpen (rmSession, 
	                            resourceName,
	                            VI_NULL,
	                            VI_NULL,
	                            InstrSession)) < 0) 
	{
		viClose (rmSession);
		return CMD60_status;
	}
	
	/*- Configure VISA Formatted I/O ----------------------------------------*/
	
	if ((CMD60_status = viSetAttribute (*InstrSession,
	                                    VI_ATTR_TMO_VALUE,
	                                    10000)) < 0)
		return CMD60_initCleanUp (rmSession, InstrSession, CMD60_status);

	if ((CMD60_status = viSetBuf (*InstrSession,
	                              VI_READ_BUF|VI_WRITE_BUF,
	                              4000)) < 0)
		return CMD60_initCleanUp (rmSession, InstrSession, CMD60_status);
	
	if ((CMD60_status = viSetAttribute (*InstrSession,
	                                    VI_ATTR_WR_BUF_OPER_MODE,
	                                    VI_FLUSH_ON_ACCESS)) < 0)
		return CMD60_initCleanUp (rmSession, InstrSession, CMD60_status);

	if ((CMD60_status = viSetAttribute (*InstrSession,
	                                    VI_ATTR_RD_BUF_OPER_MODE,
	                                    VI_FLUSH_ON_ACCESS)) < 0)
		return CMD60_initCleanUp (rmSession, InstrSession, CMD60_status);
	
#ifdef VI_ATTR_ASRL_DATA_BITS
	/* If Serial Interface defined */
	if (
	    (resourceName[0] == 'A') && 	 /* A */
	    (resourceName[1] == 'S') &&      /* S */
	    (resourceName[2] == 'R') &&      /* R */
	    (resourceName[3] == 'L')         /* L */
	   )
	{
		/* Serial Interface initialized to 8 databits, 1 stopbit, no parity, baud rate 2400 */
		if ((CMD60_status = viSetAttribute (*InstrSession,
		                                    VI_ATTR_ASRL_DATA_BITS,
		                                    8)) < 0)
			return CMD60_initCleanUp (rmSession, InstrSession, CMD60_status);
    	
		if ((CMD60_status = viSetAttribute (*InstrSession,
		                                    VI_ATTR_ASRL_STOP_BITS,
		                                    VI_ASRL_STOP_ONE)) < 0) 
			return CMD60_initCleanUp (rmSession, InstrSession, CMD60_status);
		
		if ((CMD60_status = viSetAttribute (*InstrSession,
		                                    VI_ATTR_ASRL_BAUD,
		                                    2400)) < 0)
			return CMD60_initCleanUp (rmSession, InstrSession, CMD60_status);
		
		if ((CMD60_status = viSetAttribute (*InstrSession,
		                                    VI_ATTR_ASRL_PARITY,
		                                    VI_ASRL_PAR_NONE)) < 0)
			return CMD60_initCleanUp (rmSession, InstrSession, CMD60_status);
		
		if ((CMD60_status = viSetAttribute (*InstrSession,
		                                    VI_ATTR_ASRL_FLOW_CNTRL,
		                                    VI_ASRL_FLOW_XON_XOFF)) < 0)
			return CMD60_initCleanUp (rmSession, InstrSession, CMD60_status);
	}
#endif

	/*- Setting of register of CMD60 ----------------------------------------*/
	
	if ((CMD60_status = viWrite (*InstrSession,
	                             "*SRE 0; *ESE 0\n",
	                             15,
	                             &retCnt)) < 0)
		return CMD60_initCleanUp (rmSession, InstrSession, CMD60_status);


	/*- Identification Query ------------------------------------------------*/
	if (IDQuery) 
	{
		if ((CMD60_status = viWrite (*InstrSession, "*IDN?\n", 6, &retCnt)) < 0)
			return CMD60_initCleanUp (rmSession, InstrSession, CMD60_status);
            
		if ((CMD60_status = viRead (*InstrSession, rdBuffer, BUFFER_SIZE, &retCnt)) < 0)
			return CMD60_status;
		
		PosIDN = FindPattern (rdBuffer, 0, -1, "ROHDE&SCHWARZ,CMD60", 1, 0);
		
		if (PosIDN == -1)
			{
			PosIDN = FindPattern (rdBuffer, 0, -1, "ROHDE&SCHWARZ,CMD55M", 1, 0);
			if (PosIDN == -1)
				return CMD60_initCleanUp (rmSession, InstrSession, VI_ERROR_FAIL_ID_QUERY);
			else
				{
				if (reset)  {
							if ((CMD60_status = CMD60_Reset (*InstrSession)) < 0)
								return CMD60_initCleanUp (rmSession, InstrSession, CMD60_status);
							reset = 0;
							}
				}
			}
	}
	
	if (reset) {
		if ((CMD60_status = CMD60_Reset (*InstrSession)) < 0)
			return CMD60_initCleanUp (rmSession, InstrSession, CMD60_status);
	}

	return CMD60_status;
}

/*===========================================================================*/
/* Function : 
/*     CMD60_ConfTest
/* Purpose :
/*     This function permits to select the DUT (PP-TEST or FP-TEST).
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     Test
/*         The selected DUT to test.
/*         Value range :
/*         0 - Dect Fixed Part as DUT.
/*         1 - Dect Portable Part as DUT.
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_ConfTest (ViSession  InstrSession, 
                                  ViInt32    Test)
{
	ViStatus  CMD60_status = VI_SUCCESS;
	ViByte    wrtBuf[BUFFER_SIZE];
	ViByte    rdBuf[BUFFER_SIZE];
    ViUInt32  retCnt = 0;
	ViString  IeeeSyntax[] = {"DFP", "DPP"};
	
	if (CMD60_invalidViInt32Range (Test, 0, 1))
		return VI_ERROR_PARAMETER2;

	Fmt (wrtBuf, 
	     "%s<CONF:TEST %s;*STB?\n", 
	     IeeeSyntax[Test]);
	     
    if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
                                 &retCnt)) < 0)
		return CMD60_status;

    if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	if ((CMD60_status = CMD60_Register (InstrSession, rdBuf)) < 0)
		return CMD60_status;
		
	return CMD60_status;
}

/*===========================================================================*/
/* Function : 
/*      CMD60_ProcSelTest
/* Purpose :
/*     This function permits to select the type of the test.
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     SelTest
/*         The type of the test to activate.
/*         Value range :
/*         0 - No Test Mode.
/*         1 - Manual Test.
/*         2 - Burst Analysis.
/*         3 - RF Generator.
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_ProcSelTest (ViSession  InstrSession, 
                                     ViInt32    SelTest)
{
	ViStatus  CMD60_status = VI_SUCCESS;
	ViByte    wrtBuf[BUFFER_SIZE];
	ViByte    rdBuf[BUFFER_SIZE];
    ViUInt32  retCnt = 0;
	ViString  IeeeSyntax[] = {"NONE", "MAN", "BAN", "RFG"};
	
	if (CMD60_invalidViInt32Range (SelTest, 0, 3))
		return VI_ERROR_PARAMETER2;

	Fmt (wrtBuf, 
	     "%s<PROC:SEL %s;*STB?\n", 
	     IeeeSyntax[SelTest]);
	     
	if ((CMD60_status = viWrite (InstrSession, 
	                             wrtBuf, 
	                             NumFmtdBytes (), 
	                             &retCnt)) < 0)
		return CMD60_status;
		
	if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	if ((CMD60_status = CMD60_Register (InstrSession, rdBuf)) < 0)
		return CMD60_status;
	
	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_ConfPpRfpi
/* Purpose :
/*     This function permits to set the identification number of the Fixed 
/*     Part.
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     Rfpi
/*         The "Radio Fixed Part Identity".
/*         Value range :
/*         String (10 digits in hexadecimal format).
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_ConfPpRfpi (ViSession      InstrSession, 
                                    ViChar _VI_FAR Rfpi[])
{
	ViByte    wrtBuf[BUFFER_SIZE];
	ViByte    rdBuf[BUFFER_SIZE];
	ViUInt32  retCnt = 0;
	ViStatus  CMD60_status = VI_SUCCESS;
	
	if (StringLength (Rfpi) != 10)
		return CMD60_ERROR_STRING_LENGTH;

	Fmt (wrtBuf, 
	     "%s<CONF:PP:RFPI \"%s\";*STB?\n",
		 Rfpi);
		 
	if ((CMD60_status = viWrite (InstrSession, 
	                             wrtBuf, 
	                             NumFmtdBytes (), 
								 &retCnt)) < 0)
		return CMD60_status;
	    
	if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	if ((CMD60_status = CMD60_Register (InstrSession, rdBuf)) < 0)
		return CMD60_status;

	return CMD60_status;
}

/*===========================================================================*/
/* Function : 
/*     CMD60_ConfPpDummMode
/* Purpose :
/*     This function permits to determine whether the Dummy Bearer is to be 
/*     maintained after the call setup.
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     DummMode
/*         The mode of the Dummy Bearer after connection.
/*         Value range :
/*         0 - On  : Dummy is still available after connection.
/*         1 - Off : Dummy is switch off after connection.
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_ConfPpDummMode (ViSession  InstrSession, 
                                        ViInt32    DummMode)
{
	ViStatus    CMD60_status = VI_SUCCESS;
	ViByte  	wrtBuf[BUFFER_SIZE];
	ViByte      rdBuf[BUFFER_SIZE];
    ViUInt32	retCnt = 0;
	ViString    IeeeSyntax[] = {"ON", "OFF"};
		
	if (CMD60_invalidViInt32Range (DummMode, 0, 1))
		return VI_ERROR_PARAMETER2;
		
	Fmt (wrtBuf, 
	     "%s<CONF:PP:DUMM:MODE %s;*STB?\n", 
	     IeeeSyntax[DummMode]);
	     
    if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
                                 &retCnt)) < 0)
		return CMD60_status;

    if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	if ((CMD60_status = CMD60_Register (InstrSession, rdBuf)) < 0)
		return CMD60_status;
	
	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_ConfPpChannels
/* Purpose :
/*     This function permits to set channel of the Dummy Bearer and the Traffic
/*     Bearer on the selected mode PP-TEST.
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     DummySlotNumber
/*         The slot number of the Dummy Bearer to be transmitted.
/*         Value range :
/*         0 to 11
/*     DummyCarrierNumber
/*         The channel number of the Dummy Bearer to be transmitted.
/*         Value range :
/*         0 to 9
/*	   TrafficSlotNumber
/*	       This control specifies the slot number of the Traffic Bearer with
/*         call setup.
/*         Value range :
/*         0 to 11
/*     TrafficCarrierNumber
/*         The channel number of the Traffic Bearer to be transmitted.
/*         Value range :
/*         0 to 9
/*     CarrierOffset
/*	       The carrier offset to change the assignment of channel numbers
/*         to frequencies in half the DECT channel spacing.
/*         Value range :
/*         -3.0 to +3.0 in steps of 0.5
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_ConfPpChannels (ViSession  InstrSession,
                                        ViInt32    DummySlotNumber,
                                        ViInt32    DummyCarrierNumber,
                                        ViInt32    TrafficSlotNumber,
                                        ViInt32    TrafficCarrierNumber,
                                        ViReal64   CarrierOffset)
{
	ViByte    wrtBuf[BUFFER_SIZE];
	ViByte    rdBuf[BUFFER_SIZE];
    ViUInt32  retCnt;
    ViStatus  CMD60_status = VI_SUCCESS;

    if (CMD60_invalidViInt32Range (DummySlotNumber, 0, 11))
		return VI_ERROR_PARAMETER2;
		
    if (CMD60_invalidViInt32Range (DummyCarrierNumber, 0, 9))
		return VI_ERROR_PARAMETER3;
		
	if (CMD60_invalidViInt32Range (TrafficSlotNumber, 0, 11))
		return VI_ERROR_PARAMETER4;
		
    if (CMD60_invalidViInt32Range (TrafficCarrierNumber, 0, 9))
		return VI_ERROR_PARAMETER5;
		
    if (CMD60_invalidViReal64Range (CarrierOffset, -22.0, +3.0))
		return VI_ERROR_PARAMETER6;
	
	Fmt (wrtBuf, 
	     "%s<CONF:PP:DUMM:SLOT %d;:CONF:PP:DUMM:CARR %d;:CONF:PP:CARR:OFFS %f[p1];:CONF:PP:TRAF:SLOT %d;:CONF:PP:TRAF:CARR %d;*STB?\n", 
	     DummySlotNumber,
	     DummyCarrierNumber,
	     CarrierOffset,
	     TrafficSlotNumber,
	     TrafficCarrierNumber);
	
    if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
                                 &retCnt)) < 0)
		return CMD60_status;
	
    if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	if ((CMD60_status = CMD60_Register (InstrSession, rdBuf)) < 0)
		return CMD60_status;
	
	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_ConfPpSignMode
/* Purpose :
/*     This function permits to set the signalling mode on the selected mode 
/*     PP-TEST.
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     PpSignMode
/*         The mode of connection.
/*         Value range :
/*         0 - Normal
/*         1 - Loopback
/*         2 - Echo
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_ConfPpSignMode (ViSession  InstrSession, 
                                        ViInt32    PpSignMode)
{
	ViByte    wrtBuf[BUFFER_SIZE];
	ViByte    rdBuf[BUFFER_SIZE];
    ViUInt32  retCnt;
    ViStatus  CMD60_status = VI_SUCCESS;
    ViString  IeeeSyntax[] = {"NORM", "LOOP", "ECHO"};

    if (CMD60_invalidViInt32Range (PpSignMode, 0, 2))
		return VI_ERROR_PARAMETER2;

	Fmt (wrtBuf, 
	     "%s<CONF:PP:SIGN:MODE %s;*STB?\n", 
	     IeeeSyntax[PpSignMode]);
	     
    if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
                                 &retCnt)) < 0)
		return CMD60_status;

    if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	if ((CMD60_status = CMD60_Register (InstrSession, rdBuf)) < 0)
		return CMD60_status;
	
	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_ConfPpRfLev
/* Purpose :
/*     This function permits to set the RF Level on the selected mode PP-TEST.
/* Parameter :
/*     InstrSession
/*         Session of the instrument driver.
/*     RFLevelIndBm
/* 	       The transmit power level in dBm.
/*         Value range :
/*         The range of the RF level depends on the external
/*         attenuation and the selected RF connector as follows :
/*         When RF OUT selected :
/*              -100.0 - ext.att. ... -40.0 - ext.att. dBm
/*         When RF OUT2 and RF IN selected :
/*              -40.0 - ext.att.  ... +7.5 - ext.att. dBm
/*         When RF OUT2 and RF IN2 selected
/*              -20.0 - ext.att.  ... +7.5 - ext.att. dBm
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_ConfPpRfLev (ViSession  InstrSession, 
                                     ViReal64   RFLevelIndBm)
{
	ViByte    wrtBuf[BUFFER_SIZE];
	ViByte    rdBuf[BUFFER_SIZE];
    ViUInt32  retCnt;
    ViStatus  CMD60_status = VI_SUCCESS;
	
	if (CMD60_invalidViReal64Range (RFLevelIndBm, -200.0, +100.0))
		return VI_ERROR_PARAMETER2;
	
	Fmt (wrtBuf, 
	     "%s<CONF:PP:RF:LEV %f[p1] dBm;*STB?\n", 
	     RFLevelIndBm);
	     
    if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
                                 &retCnt)) < 0)
		return CMD60_status;

    if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	if ((CMD60_status = CMD60_Register (InstrSession, rdBuf)) < 0)
		return CMD60_status;
	
	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_ConfPpQPacket
/* Purpose :
/*     This function permits to set the Q-Packets used on the selected mode 
/*     PP-TEST.
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/* 	   StaticSystemInfo
/*         The prototype of the Q0 packet. In the Q0 packet, the Fixed
/*         Part transmits static system information.
/*         Value range :
/*         String (12 digits in hexadecimal format).
/*     FixedPartCapabilities
/*         The prototype of the Q3 packet. In the Q3 packet, the Fixed
/*         Part sends slot types and message types.
/*         Value range :
/*         String (12 digits in hexadecimal format).
/* 	   MultiframeNumber
/*	       The prototype of the Q6 packet. In the Q6 packet, the Fixed
/*         Part sends the number of the current multiframe.
/*         Value range :
/*         String (12 digits in hexadecimal format).
/*	   AdditionalQPacket
/*	       The prototype of any further Q packet. This packet (QE) can
/*         be freely selected.
/*         Value range :
/*         String (12 digits in hexadecimal format).
/*	   QPacketMultiplexSequence
/*	       The transmission sequence of the Q Packets used.
/*         Value range :
/*         String (8 digits in hexadecimal format).
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_ConfPpQPacket (ViSession       InstrSession,
                                       ViChar _VI_FAR  StaticSystemInfo[],
                                       ViChar _VI_FAR  FixedPartCapabilities[],
                                       ViChar _VI_FAR  MultiframeNumber[],
                                       ViChar _VI_FAR  AdditionalQPacket[],
                                       ViChar _VI_FAR  QPacketMultiplexSequence[])
{
	ViByte    wrtBuf[BUFFER_SIZE];
	ViByte    rdBuf[BUFFER_SIZE];
	ViUInt32  retCnt;
	ViStatus  CMD60_status = VI_SUCCESS;
	
	if (StringLength (StaticSystemInfo) != 12)
		return CMD60_ERROR_STRING_LENGTH;
	
	if (StringLength (FixedPartCapabilities) != 12)
		return CMD60_ERROR_STRING_LENGTH;
		
	if (StringLength (MultiframeNumber) != 12)
		return CMD60_ERROR_STRING_LENGTH;
	
	if (StringLength (AdditionalQPacket) != 12)
		return CMD60_ERROR_STRING_LENGTH;
	
	if (StringLength (QPacketMultiplexSequence) != 8)
		return CMD60_ERROR_STRING_LENGTH;
	
	Fmt (wrtBuf, 
	     "%s<CONF:PP:PROT:SYST:INFO \"%s\";:CONF:PP:PROT:FP:CAP \"%s\";:CONF:PP:PROT:MULT \"%s\";:CONF:PP:PROT:QPAC:ADD \"%s\";:CONF:PP:TABL:QPAC:SEQ \"%s\";*STB?\n",
		 StaticSystemInfo,
		 FixedPartCapabilities,
		 MultiframeNumber,
		 AdditionalQPacket,
		 QPacketMultiplexSequence);    
	
	if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
                                 &retCnt)) < 0)
		return CMD60_status;

    if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	if ((CMD60_status = CMD60_Register (InstrSession, rdBuf)) < 0)
		return CMD60_status;
	
	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_ConfFpAnt
/* Purpose :
/*     This function permits to set the antenna number to  be  used  by the
/*     Fixed Part under test.
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     FpAnt
/*         The antenna number to be used by the Fixed Part under test.
/*         Value range :
/*         0 to 7
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_ConfFpAnt (ViSession  InstrSession, 
                                   ViInt32    FpAnt)
{
	ViByte    wrtBuf[BUFFER_SIZE];
	ViByte    rdBuf[BUFFER_SIZE];
    ViUInt32  retCnt;
    ViStatus  CMD60_status = VI_SUCCESS;
    
    if (CMD60_invalidViInt32Range (FpAnt, 0, 7))
		return VI_ERROR_PARAMETER2;
    
	Fmt (wrtBuf, 
	     "%s<CONF:FP:ANT %d;*STB?\n",
		 FpAnt);    
	
	if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
                                 &retCnt)) < 0)
		return CMD60_status;

    if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	if ((CMD60_status = CMD60_Register (InstrSession, rdBuf)) < 0)
		return CMD60_status;
	
	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_ConfFpRfpi
/* Purpose :
/*     This function permits to set the "Radio Fixed Part Identity". This is an
/*     identification number of the fixed part under test.
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     Rfpi
/*         The "Radio Fixed Part Identity".
/*         Value range :
/*         String (10 digits in hexadecimal format).
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_ConfFpRfpi (ViSession       InstrSession, 
                                    ViChar _VI_FAR  Rfpi[])
{
	ViByte    wrtBuf[BUFFER_SIZE];
	ViByte    rdBuf[BUFFER_SIZE];
    ViUInt32  retCnt = 0;
    ViStatus  CMD60_status = VI_SUCCESS;
    
	if (StringLength (Rfpi) != 10)
		return CMD60_ERROR_STRING_LENGTH;

	Fmt (wrtBuf, 
	     "%s<CONF:FP:RFPI \"%s\";*STB?\n",
		 Rfpi);
		 
	if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
    							 &retCnt)) < 0)
		return CMD60_status;
        
	if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	if ((CMD60_status = CMD60_Register (InstrSession, rdBuf)) < 0)
		return CMD60_status;

	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_ConfFpPmid
/* Purpose :
/*     This function permits to set the "Portable Part MAC Identity". This is
/*     the identification number of the portable part to be simulated.
/* Parameters :
/*     InstrSession
/*	       Session of the instrument driver.
/*     Pmid
/*         The "Portable Part MAC Identity".
/*         Value range :
/*         String (5 digits in hexadecimal format).
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_ConfFpPmid (ViSession       InstrSession, 
                                    ViChar _VI_FAR  Pmid[])
{
	ViByte    wrtBuf[BUFFER_SIZE];
	ViByte    rdBuf[BUFFER_SIZE];
    ViUInt32  retCnt = 0;
    ViStatus  CMD60_status = VI_SUCCESS;
    
	if (StringLength (Pmid) != 5)
		return CMD60_ERROR_STRING_LENGTH;
    
    Fmt (wrtBuf, 
	     "%s<CONF:FP:PMID \"%s\";*STB?\n",
		 Pmid);
		 
	if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
    							 &retCnt)) < 0)
		return CMD60_status;
        
	if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	if ((CMD60_status = CMD60_Register (InstrSession, rdBuf)) < 0)
		return CMD60_status;

	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_ConfFpChannels
/* Purpose :
/*     This function permits to set the traffic bearer on call setup. 
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     TrafficSlotNumber
/*         Slot number of the traffic bearer.
/*         Value range :
/*         0 to 11.
/*     TrafficCarrierNumber
/*         Channel number of the traffic bearer.
/*         Value range :
/*         0 to 9.
/*     CarrierOffset
/*         Offset for extended channels.
/*         Value range :
/*         -3.0 to +3.0 in steps of 0.5.
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_ConfFpChannels (ViSession  InstrSession,
                                        ViInt32    TrafficSlotNumber,
                                        ViInt32    TrafficCarrierNumber,
                                        ViReal64   CarrierOffset)
{
	ViByte    wrtBuf[BUFFER_SIZE];
	ViByte    rdBuf[BUFFER_SIZE];
    ViUInt32  retCnt = 0;
    ViStatus  CMD60_status = VI_SUCCESS;
		
    if (CMD60_invalidViInt32Range (TrafficSlotNumber, 0, 11))
		return VI_ERROR_PARAMETER2;
		
    if (CMD60_invalidViInt32Range (TrafficCarrierNumber, 0, 9))
		return VI_ERROR_PARAMETER3;
		
    if (CMD60_invalidViReal64Range (CarrierOffset, -22.0, +3.0))
		return VI_ERROR_PARAMETER4;
	
	Fmt (wrtBuf, 
	     "%s<CONF:FP:CARR:OFFS %f[p1];:CONF:FP:TRAF:SLOT %d;:CONF:FP:TRAF:CARR %d;*STB?\n", 
	     CarrierOffset,
	     TrafficSlotNumber,
	     TrafficCarrierNumber);
		 
	if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
    							 &retCnt)) < 0)
		return CMD60_status;
        
	if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	if ((CMD60_status = CMD60_Register (InstrSession, rdBuf)) < 0)
		return CMD60_status;

	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_ConfFpSignMode
/* Purpose :
/*     This function permits to set the mode of connection.
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     FpSignMode
/*         Mode of connection.
/*         Value range :
/*         0 - Loopback.
/*         1 - Normal.
/*         2 - Echo.
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_ConfFpSignMode (ViSession  InstrSession, 
                                        ViInt32    FpSignMode)
{
	ViByte    wrtBuf[BUFFER_SIZE];
	ViByte    rdBuf[BUFFER_SIZE];
    ViUInt32  retCnt;
    ViStatus  CMD60_status = VI_SUCCESS;
    ViString  IeeeSyntax[] = {"NORM", "LOOP", "ECHO"};

    if (CMD60_invalidViInt32Range (FpSignMode, 0, 2))
		return VI_ERROR_PARAMETER2;

	Fmt (wrtBuf, 
	     "%s<CONF:FP:SIGN:MODE %s;*STB?\n", 
	     IeeeSyntax[FpSignMode]);
	     
    if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
                                 &retCnt)) < 0)
		return CMD60_status;

    if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	if ((CMD60_status = CMD60_Register (InstrSession, rdBuf)) < 0)
		return CMD60_status;
	
	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_ConfFpRfLev
/* Purpose :
/*     This function permits to set the transmit power.
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     RFLevelIndBm
/*         The RF level.
/*         Value range :
/*         -40 dBm to -100 dBm (referred to RF IN/OUT) in steps of 0.1 dB.
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_ConfFpRfLev (ViSession  InstrSession, 
                                     ViReal64   RFLevelIndBm)
{
	ViByte    wrtBuf[BUFFER_SIZE];
	ViByte    rdBuf[BUFFER_SIZE];
    ViUInt32  retCnt;
    ViStatus  CMD60_status = VI_SUCCESS;
	
	if (CMD60_invalidViReal64Range (RFLevelIndBm, -200.0, +100.0))
		return VI_ERROR_PARAMETER2;
	
	Fmt (wrtBuf, 
	     "%s<CONF:FP:RF:LEV %f[p1] dBm;*STB?\n", 
	     RFLevelIndBm);
	     
    if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
                                 &retCnt)) < 0)
		return CMD60_status;

    if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	if ((CMD60_status = CMD60_Register (InstrSession, rdBuf)) < 0)
		return CMD60_status;
	
	return CMD60_status;	
}

/*===========================================================================*/
/* Function :
/*     CMD60_SystCommGpibAddr
/* Purpose :
/*     This function permits to ...
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     BusAddress
/*         ...
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_SystCommGpibAddr (ViSession  InstrSession,
                                          ViInt32    BusAddress)
{
	ViByte    wrtBuf[BUFFER_SIZE];
	ViByte    rdBuf[BUFFER_SIZE];
    ViUInt32  retCnt;
    ViStatus  CMD60_status = VI_SUCCESS;
    
    if (CMD60_invalidViInt32Range (BusAddress, 0, 30))
		return VI_ERROR_PARAMETER2;
    
	Fmt (wrtBuf, 
	     "%s<SYST:COMM:GPIB:ADDR %d;*STB?\n",
		 BusAddress);    
	
	if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
                                 &retCnt)) < 0)
		return CMD60_status;

    if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	if ((CMD60_status = CMD60_Register (InstrSession, rdBuf)) < 0)
		return CMD60_status;
	
	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_ConfMessEscMT
/* Purpose :
/*     This function permits to set the MT escape sequence.
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     MTEscapeSequence
/*         The MT escape sequence.
/*         Value range :
/*         String (10 digits in hex format).
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_ConfMessEscMT (ViSession       InstrSession,
                                       ViChar _VI_FAR  MTEscapeSequence[])
{
	ViByte    wrtBuf[BUFFER_SIZE];
	ViByte    rdBuf[BUFFER_SIZE];
    ViUInt32  retCnt = 0;
    ViStatus  CMD60_status = VI_SUCCESS;
    
	if (StringLength (MTEscapeSequence) != 10)
		return CMD60_ERROR_STRING_LENGTH;
    
    Fmt (wrtBuf, 
	     "%s<CONF:MESS:ESC:MT \"%s\";*STB?\n",
		 MTEscapeSequence);
		 
	if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
    							 &retCnt)) < 0)
		return CMD60_status;
        
	if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	if ((CMD60_status = CMD60_Register (InstrSession, rdBuf)) < 0)
		return CMD60_status;

	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_CalcLimPowTranTempCle
/* Purpose :
/*     This function permits to reset to default values for Power Ramp and NTP.
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_CalcLimPowTranTempCle (ViSession  InstrSession)
{
	ViByte    wrtBuf[BUFFER_SIZE];
	ViByte    rdBuf[BUFFER_SIZE];
    ViUInt32  retCnt = 0;
    ViStatus  CMD60_status = VI_SUCCESS;
    
    Fmt (wrtBuf, 
	     "%s<CALC:LIM:POW:TRAN:TEMP:CLE;*STB?\n");
		 
	if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
    							 &retCnt)) < 0)
		return CMD60_status;
        
	if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	if ((CMD60_status = CMD60_Register (InstrSession, rdBuf)) < 0)
		return CMD60_status;

	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_CalcLimPowTranTempTol
/* Purpose :
/*     This function permits to set tolerance range for NTP.
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     LowerNTP
/*         Lower tolerance for NTP.
/*         Value range :
/*         0.0 to 30.0 dBm.
/*     UpperNTP
/*         Upper tolerance for NTP.
/*         Value range :
/*         0.0 to 30.0 dBm.
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_CalcLimPowTranTempTol (ViSession  InstrSession, 
                                               ViReal64   LowerNTP,
                                               ViReal64   UpperNTP)
{
	ViByte    wrtBuf[BUFFER_SIZE];
	ViByte    rdBuf[BUFFER_SIZE];
    ViUInt32  retCnt;
    ViStatus  CMD60_status = VI_SUCCESS;
	
	if (CMD60_invalidViReal64Range (LowerNTP, 0.0, 30.0))
		return VI_ERROR_PARAMETER2;
		
	if (CMD60_invalidViReal64Range (UpperNTP, 0.0, 30.0))
		return VI_ERROR_PARAMETER3;
	
	Fmt (wrtBuf, 
	     "%s<CALC:LIM:POW:TRAN:TEMP:TOL:LOW %f[p1];:CALC:LIM:POW:TRAN:TEMP:TOL:UPP %f[p1];*STB?\n", 
	     LowerNTP,
	     UpperNTP);
	     
    if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
                                 &retCnt)) < 0)
		return CMD60_status;

    if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	if ((CMD60_status = CMD60_Register (InstrSession, rdBuf)) < 0)
		return CMD60_status;
	
	return CMD60_status;	
}

/*===========================================================================*/
/* Function :
/*     CMD60_CalcLimPowTranTemp
/* Purpose :
/*     This function permits to set tolerance range for Power Ramp.
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     AbsoluteLevelAtA
/*         Absolute tolerance level at A.
/*         Value range :
/*         0.0 to 100.0 nW
/*     AbsoluteLevelAtB
/*         Absolute tolerance level at B.
/*         Value range :
/*         0.0 to 100.0 nW
/*     RelativeLevelAtC
/*         Relative tolerance level at C.
/*         Value range :
/*         0.0 to 10.0 dB
/*     RelativeLevelAtD
/*         Relative tolerance level at D.
/*         Value range :
/*         0.0 to 10.0 dB
/*     RelativeLevelAtE
/*         Relative tolerance level at E.
/*         Value range :
/*         -10.0 to 0.0 dB
/*     RelativeLevelAtF
/*         Relative tolerance level at F.
/*         Value range :
/*         -20.0 to 0.0 dB
/*                C ____
/*                 |    |___________________________  D
/*                 | E ________________________     |
/*                 |  |                        |_ F |
/*                 |  |                          |  |
/*                 |  |                          |  |
/*                 |  |                          |  |
/*                 |  |                          |  |
/*                 |  |                          |  |
/*                 |  |                          |  |
/*                 |  |                          |  |
/*                 |  |                          |  |
/*     		 B ____|  |                          |  |____ B
/*            |       |                          |       |
/*      A ____|       |                          |       |____ A
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_CalcLimPowTranTemp (ViSession  InstrSession, 
                                            ViReal64   AbsoluteLevelAtA,
                                            ViReal64   AbsoluteLevelAtB, 
                                            ViReal64   RelativeLevelAtC,
                                            ViReal64   RelativeLevelAtD, 
                                            ViReal64   RelativeLevelAtE,
                                            ViReal64   RelativeLevelAtF)
{
	ViByte    wrtBuf[BUFFER_SIZE];
	ViByte    rdBuf[BUFFER_SIZE];
    ViUInt32  retCnt;
    ViStatus  CMD60_status = VI_SUCCESS;
	
	if (CMD60_invalidViReal64Range (AbsoluteLevelAtA, 0.0, 100.0))
		return VI_ERROR_PARAMETER2;
		
	if (CMD60_invalidViReal64Range (AbsoluteLevelAtB, 0.0, 100.0))
		return VI_ERROR_PARAMETER3;
	
	if (CMD60_invalidViReal64Range (RelativeLevelAtC, 0.0, 10.0))
		return VI_ERROR_PARAMETER4;
	
	if (CMD60_invalidViReal64Range (RelativeLevelAtD, 0.0, 10.0))
		return VI_ERROR_PARAMETER5;
	
	if (CMD60_invalidViReal64Range (RelativeLevelAtE, -10.0, 0.0))
		return VI_ERROR_PARAMETER6;
	
	if (CMD60_invalidViReal64Range (RelativeLevelAtF, -20.0, 0.0))
		return VI_ERROR_PARAMETER7;
	
	Fmt (wrtBuf, 
	     "%s<CALC:LIM:POW:TRAN %f[p1]nW,%f[p1]uW,%f[p1]dB,%f[p1]dB,%f[p1]dB,%f[p1]dB;*STB?\n", 
	     AbsoluteLevelAtA,
	     AbsoluteLevelAtB,
	     RelativeLevelAtC,
	     RelativeLevelAtD,
	     RelativeLevelAtE,
	     RelativeLevelAtF);
	     
    if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
                                 &retCnt)) < 0)
		return CMD60_status;

    if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	if ((CMD60_status = CMD60_Register (InstrSession, rdBuf)) < 0)
		return CMD60_status;
	
	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_ConfPowDynRang
/* Purpose :
/*     This function permits to set the dynamic range.
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     PowDynRang
/*         The dynamic range.
/*         Value range :
/*         0 - High dynamic.
/*         1 - Low dynamic.
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_ConfPowDynRang (ViSession  InstrSession,
                                        ViInt32    PowDynRang)
{
	ViByte    wrtBuf[BUFFER_SIZE];
	ViByte    rdBuf[BUFFER_SIZE];
    ViUInt32  retCnt;
    ViStatus  CMD60_status = VI_SUCCESS;
    ViString  IeeeSyntax[] = {"HIGH", "LOW"};

    if (CMD60_invalidViInt32Range (PowDynRang, 0, 1))
		return VI_ERROR_PARAMETER2;

	Fmt (wrtBuf, 
	     "%s<CONF:POW:DYN:RANG %s;*STB?\n", 
	     IeeeSyntax[PowDynRang]);
	     
    if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
                                 &retCnt)) < 0)
		return CMD60_status;

    if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	if ((CMD60_status = CMD60_Register (InstrSession, rdBuf)) < 0)
		return CMD60_status;
	
	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_ConfAverBurs
/* Purpose :
/*     This function permits to set the number of bursts for NTP and MAX/MIN
/*     modulation.
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     NumberOfBursts
/*         Number of bursts.
/*         Value range :
/*         1 to 200.
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_ConfAverBurs (ViSession  InstrSession,
                                      ViInt32    NumberOfBursts)
{
	ViByte    wrtBuf[BUFFER_SIZE];
	ViByte    rdBuf[BUFFER_SIZE];
    ViUInt32  retCnt;
    ViStatus  CMD60_status = VI_SUCCESS;

    if (CMD60_invalidViInt32Range (NumberOfBursts, 1, 200))
		return VI_ERROR_PARAMETER2;

	Fmt (wrtBuf, 
	     "%s<CONF:AVER:BURS %d;*STB?\n", 
	     NumberOfBursts);
	     
    if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
                                 &retCnt)) < 0)
		return CMD60_status;

    if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	if ((CMD60_status = CMD60_Register (InstrSession, rdBuf)) < 0)
		return CMD60_status;
	
	return CMD60_status;
}

/*===========================================================================*/
/* Function : 
/*     CMD60_ConfPowTranOutpRang
/* Purpose :
/*     This function permits to select a part of the burst power array.
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     StartPosition
/*         The start position of a burst power.
/*         Value range :
/*         -38.0 to 462.0 (in steps of 1/6).
/*     NumberOfMeasurement
/*         The number of measurement values of a burst power.
/*         Value range :
/*         1 to 3000.
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_ConfPowTranOutpRang (ViSession  InstrSession,
                                             ViReal64   StartPosition,
                                             ViInt32    NumberOfMeasurement)
{
	ViByte    wrtBuf[BUFFER_SIZE];
	ViByte    rdBuf[BUFFER_SIZE];
    ViUInt32  retCnt;
    ViStatus  CMD60_status = VI_SUCCESS;

    if (CMD60_invalidViReal64Range (StartPosition, -38.0, 462.0))
		return VI_ERROR_PARAMETER3;
    
    if (CMD60_invalidViInt32Range (NumberOfMeasurement, 1, 3000))
		return VI_ERROR_PARAMETER3;

	Fmt (wrtBuf, 
	     "%s<CONF:POW:TRAN:OUTP:RANG %f[p1],%d;*STB?\n", 
	     StartPosition,
	     NumberOfMeasurement);
	     
    if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
                                 &retCnt)) < 0)
		return CMD60_status;

    if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	if ((CMD60_status = CMD60_Register (InstrSession, rdBuf)) < 0)
		return CMD60_status;
	
	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_CalcLimModCle
/* Purpose :
/*     This function permits to ...
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_CalcLimModCle (ViSession  InstrSession)
{
	ViByte    wrtBuf[BUFFER_SIZE];
	ViByte    rdBuf[BUFFER_SIZE];
    ViUInt32  retCnt;
    ViStatus  CMD60_status = VI_SUCCESS;

	Fmt (wrtBuf, 
	     "%s<CALC:LIM:MOD:CLE;*STB?\n");
	     
    if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
                                 &retCnt)) < 0)
		return CMD60_status;

    if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	if ((CMD60_status = CMD60_Register (InstrSession, rdBuf)) < 0)
		return CMD60_status;
	
	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_CalcLimMod
/* Purpose :
/*     This function permits to ...
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     frequencyOffsetInkHz
/*     BFieldLowerInkHz
/*     BFieldUpperInkHz
/*     syncLowerInkHz
/*     syncUpperInkHz
/*     frequencyDriftInHzperS
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_CalcLimMod (ViSession  InstrSession,
                                    ViReal64   frequencyOffsetInkHz,
                                    ViReal64   BFieldLowerInkHz, 
                                    ViReal64   BFieldUpperInkHz,
                                    ViReal64   syncLowerInkHz, 
                                    ViReal64   syncUpperInkHz,
                                    ViReal64   frequencyDriftInHzperS)
{
	ViByte    wrtBuf[BUFFER_SIZE];
	ViByte    rdBuf[BUFFER_SIZE];
    ViUInt32  retCnt;
    ViStatus  CMD60_status = VI_SUCCESS;
	
	if (CMD60_invalidViReal64Range (frequencyOffsetInkHz, 0.0, 500.0))
		return VI_ERROR_PARAMETER2;
		
	if (CMD60_invalidViReal64Range (BFieldLowerInkHz, 0.0, 500.0))
		return VI_ERROR_PARAMETER3;
	
	if (CMD60_invalidViReal64Range (BFieldUpperInkHz, 0.0, 500.0))
		return VI_ERROR_PARAMETER4;
	
	if (CMD60_invalidViReal64Range (syncLowerInkHz, 0.0, 500.0))
		return VI_ERROR_PARAMETER5;
	
	if (CMD60_invalidViReal64Range (syncUpperInkHz, 0.0, 500.0))
		return VI_ERROR_PARAMETER6;
	
	if (CMD60_invalidViReal64Range (frequencyDriftInHzperS, 0.0, 1.0e8))
		return VI_ERROR_PARAMETER7;
	
	Fmt (wrtBuf, 
	     "%s<CALC:LIM:FREQ:OFFS:TOL %f[p1] kHz;:CALC:LIM:BFI:TOL %f[p1] kHz,%f[p1] kHz;:CALC:LIM:SFI:TOL %f[p1] kHz,%f[p1] kHz;:CALC:LIM:FREQ:DRIF:TOL %f[p1] Hz/s;*STB?\n", 
	     frequencyOffsetInkHz,
	     BFieldLowerInkHz,
	     BFieldUpperInkHz,
	     syncLowerInkHz,
	     syncUpperInkHz,
	     frequencyDriftInHzperS);
	     
    if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
                                 &retCnt)) < 0)
		return CMD60_status;

    if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	if ((CMD60_status = CMD60_Register (InstrSession, rdBuf)) < 0)
		return CMD60_status;
	
	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_ConfModDataType
/* Purpose :
/*     This function permits to ...
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     dataTypeInBField
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_ConfModDataType (ViSession  InstrSession, 
                                         ViInt32    dataTypeInBField)
{
	ViByte    wrtBuf[BUFFER_SIZE];
	ViByte    rdBuf[BUFFER_SIZE];
    ViUInt32  retCnt;
    ViStatus  CMD60_status = VI_SUCCESS;
	ViString  DataTypeValue[] = {"PSRB", "BS55", "BS33", "BS0F", "F31"};

    if (CMD60_invalidViInt32Range (dataTypeInBField, 0, 4))
		return VI_ERROR_PARAMETER2;

	Fmt (wrtBuf, 
	     "%s<CONF:MOD:DATA:TYPE %s;*STB?\n", 
	     DataTypeValue[dataTypeInBField]);
	
	     
    if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
                                 &retCnt)) < 0)
		return CMD60_status;

    if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	if ((CMD60_status = CMD60_Register (InstrSession, rdBuf)) < 0)
		return CMD60_status;
	
	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_CalcLimTimCle
/* Purpose :
/*     This function permits to ...
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_CalcLimTimCle (ViSession  InstrSession)
{
	ViByte    wrtBuf[BUFFER_SIZE];
	ViByte    rdBuf[BUFFER_SIZE];
    ViUInt32  retCnt;
    ViStatus  CMD60_status = VI_SUCCESS;

	Fmt (wrtBuf, 
	     "%s<CALC:LIM:TIM:CLE;*STB?\n");
	     
    if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
                                 &retCnt)) < 0)
		return CMD60_status;

    if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	if ((CMD60_status = CMD60_Register (InstrSession, rdBuf)) < 0)
		return CMD60_status;
	
	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_ConfTimAverBurs
/* Purpose :
/*     This function permits to ...
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     numberOfBursts
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_ConfTimAverBurs (ViSession  InstrSession,
                                         ViInt32    numberOfBursts)
{
	ViByte    wrtBuf[BUFFER_SIZE];
	ViByte    rdBuf[BUFFER_SIZE];
    ViUInt32  retCnt;
    ViStatus  CMD60_status = VI_SUCCESS;

    if (CMD60_invalidViInt32Range (numberOfBursts, 2, 1000))
		return VI_ERROR_PARAMETER2;

	Fmt (wrtBuf, 
	     "%s<CONF:TIM:AVER:BURS %d;*STB?\n", 
	     numberOfBursts);
	     
    if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
                                 &retCnt)) < 0)
		return CMD60_status;

    if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	if ((CMD60_status = CMD60_Register (InstrSession, rdBuf)) < 0)
		return CMD60_status;
	
	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_CalcLimTime
/* Purpose :
/*     This function permits to ...
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     timeAccuracy
/*     TolForMaxNegJitter
/*     TolForMaxPosJitter
/*     TolForMinPacketDelay
/*     TolForMaxPacketDelay
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_CalcLimTime (ViSession  InstrSession, 
                                     ViReal64   timeAccuracy,
                                     ViReal64   TolForMaxNegJitter, 
                                     ViReal64   TolForMaxPosJitter,
                                     ViReal64   TolForMinPacketDelay, 
                                     ViReal64   TolForMaxPacketDelay)
{
	ViByte    wrtBuf[BUFFER_SIZE];
	ViByte    rdBuf[BUFFER_SIZE];
    ViUInt32  retCnt;
    ViStatus  CMD60_status = VI_SUCCESS;
	
	if (CMD60_invalidViReal64Range (timeAccuracy, 0.0, 100.0))
		return VI_ERROR_PARAMETER2;
		
	if (CMD60_invalidViReal64Range (TolForMaxNegJitter, -100.0, 0.0))
		return VI_ERROR_PARAMETER3;
	
	if (CMD60_invalidViReal64Range (TolForMaxPosJitter, 0.0, 100.0))
		return VI_ERROR_PARAMETER4;
	
	if (CMD60_invalidViReal64Range (TolForMinPacketDelay, -5.0, +5.0))
		return VI_ERROR_PARAMETER5;
	
	if (CMD60_invalidViReal64Range (TolForMaxPacketDelay, -5.0, +5.0))
		return VI_ERROR_PARAMETER6;
	
	Fmt (wrtBuf, 
	     "%s<CALC:LIM:TIME:ACC:TOL %f[p1];:CALC:LIM:JITT:TOL %f[p1]us,%f[p1]us;:CALC:LIM:PACK:DEL:TOL %f[p1]us,%f[p1]us;*STB?\n", 
	     timeAccuracy,
	     TolForMaxNegJitter,
	     TolForMaxPosJitter,
	     TolForMinPacketDelay,
	     TolForMaxPacketDelay);
	     
    if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
                                 &retCnt)) < 0)
		return CMD60_status;

    if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	if ((CMD60_status = CMD60_Register (InstrSession, rdBuf)) < 0)
		return CMD60_status;
	
	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_CalcLimBerLterCle
/* Purpose :
/*     This function permits to ...
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_CalcLimBerLterCle (ViSession  InstrSession)
{
	ViByte    wrtBuf[BUFFER_SIZE];
	ViByte    rdBuf[BUFFER_SIZE];
    ViUInt32  retCnt;
    ViStatus  CMD60_status = VI_SUCCESS;

	Fmt (wrtBuf, 
	     "%s<CALC:LIM:BER:LTER:CLE;*STB?\n");
	     
    if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
                                 &retCnt)) < 0)
		return CMD60_status;

    if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	if ((CMD60_status = CMD60_Register (InstrSession, rdBuf)) < 0)
		return CMD60_status;
	
	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_CalcLimBerLtermTol
/* Purpose :
/*     This function permits to ...
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     longTermBER
/*     longTermFER
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_CalcLimBerLtermTol (ViSession  InstrSession, 
                                            ViReal64   longTermBER,
                                            ViReal64   longTermFER)
{
	ViByte    wrtBuf[BUFFER_SIZE];
	ViByte    rdBuf[BUFFER_SIZE];
    ViUInt32  retCnt;
    ViStatus  CMD60_status = VI_SUCCESS;
	
	if (CMD60_invalidViReal64Range (longTermBER, 0.0, 10.0))
		return VI_ERROR_PARAMETER2;
		
	if (CMD60_invalidViReal64Range (longTermFER, 0.0, 10.0))
		return VI_ERROR_PARAMETER3;
	
	Fmt (wrtBuf, 
	     "%s<CALC:LIM:BER:LTERM:TOL %f[p1],%f[p1];*STB?\n", 
	     longTermBER,
	     longTermFER);
	     
    if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
                                 &retCnt)) < 0)
		return CMD60_status;

    if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	if ((CMD60_status = CMD60_Register (InstrSession, rdBuf)) < 0)
		return CMD60_status;
	
	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_ConfBerPpMbeClear
/* Purpose :
/*     This function permits to ...
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     multiBearerArray
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_ConfBerPpMbeClear (ViSession  InstrSession,
                                           ViInt32    multiBearerArray)
{
	ViByte    wrtBuf[BUFFER_SIZE];
	ViByte    rdBuf[BUFFER_SIZE];
    ViUInt32  retCnt;
    ViStatus  CMD60_status = VI_SUCCESS;

    if (CMD60_invalidViInt32Range (multiBearerArray, 0, 1))
		return VI_ERROR_PARAMETER2;

	if (multiBearerArray == 0) 
	    /* Portable-Part */
		Fmt (wrtBuf, 
	         "%s<CONF:BER:PP:MBE:CLEAR;*STB?\n");
	else
	    /* Fixed-Part */
		Fmt (wrtBuf, 
	         "%s<CONF:BER:FP:MBE:CLEAR;*STB?\n");
	     
    if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
                                 &retCnt)) < 0)
		return CMD60_status;

    if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	if ((CMD60_status = CMD60_Register (InstrSession, rdBuf)) < 0)
		return CMD60_status;
	
	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_ConfBerMbeSlot
/* Purpose :
/*     This function permits to ...
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     multiBearerArray
/*     additionalSlotNumber
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_ConfBerMbeSlot (ViSession InstrSession,
                                        ViInt32   multiBearerArray,
                                        ViInt32   additionalSlotNumber)
{
	ViByte    wrtBuf[BUFFER_SIZE];
	ViByte    rdBuf[BUFFER_SIZE];
    ViUInt32  retCnt;
    ViStatus  CMD60_status = VI_SUCCESS;

    if (CMD60_invalidViInt32Range (multiBearerArray, 0, 1))
		return VI_ERROR_PARAMETER2;
	
	if (CMD60_invalidViInt32Range (additionalSlotNumber, 0, 11))
		return VI_ERROR_PARAMETER3;

	if (multiBearerArray == 0) 
	    /* Portable-Part */
		Fmt (wrtBuf, 
	         "%s<CONF:BER:PP:MBE:SLOT %d;*STB?\n",
	         additionalSlotNumber);
	else
	    /* Fixed-Part */
		Fmt (wrtBuf, 
	         "%s<CONF:BER:FP:MBE:SLOT %d;*STB?\n",
	         additionalSlotNumber);
	     
    if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
                                 &retCnt)) < 0)
		return CMD60_status;

    if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	if ((CMD60_status = CMD60_Register (InstrSession, rdBuf)) < 0)
		return CMD60_status;
	
	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_ConfBerEvalWind
/* Purpose :
/*     This function permits to ...
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     evaluationWindow
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_ConfBerEvalWind (ViSession  InstrSession,
                                         ViInt32    evaluationWindow)
{
	ViByte    wrtBuf[BUFFER_SIZE];
	ViByte    rdBuf[BUFFER_SIZE];
    ViUInt32  retCnt;
    ViStatus  CMD60_status = VI_SUCCESS;

    if (CMD60_invalidViInt32Range (evaluationWindow, 1, 30e6))
		return VI_ERROR_PARAMETER2;
	
	Fmt (wrtBuf, 
	     "%s<CONF:BER:EVAL:WIND %d;*STB?\n",
	     evaluationWindow);
	     
    if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
                                 &retCnt)) < 0)
		return CMD60_status;

    if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	if ((CMD60_status = CMD60_Register (InstrSession, rdBuf)) < 0)
		return CMD60_status;
	
	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_ConfBerRfLev
/* Purpose :
/*     This function permits to ...
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     RfLevelIndBm
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_ConfBerRfLev (ViSession  InstrSession,
                                      ViReal64   RfLevelIndBm)
{
	ViByte    wrtBuf[BUFFER_SIZE];
	ViByte    rdBuf[BUFFER_SIZE];
    ViUInt32  retCnt;
    ViStatus  CMD60_status = VI_SUCCESS;

    if (CMD60_invalidViReal64Range (RfLevelIndBm, -200.0, +100.0))
		return VI_ERROR_PARAMETER2;
	
	Fmt (wrtBuf, 
	     "%s<CONF:BER:RF:LEV %f[p1];*STB?\n",
	     RfLevelIndBm);
	     
    if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
                                 &retCnt)) < 0)
		return CMD60_status;

    if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	if ((CMD60_status = CMD60_Register (InstrSession, rdBuf)) < 0)
		return CMD60_status;
	
	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_CorrLoss                                  
/* Purpose :
/*     This function permits to ...
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     source
/*     externalAttenuation
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_CorrLoss (ViSession  InstrSession,
                                  ViInt32    source,
                                  ViReal64   externalAttenuation)
{
	ViByte    wrtBuf[BUFFER_SIZE];
	ViByte    rdBuf[BUFFER_SIZE];
    ViUInt32  retCnt;
    ViStatus  CMD60_status = VI_SUCCESS;
    ViString  SourValue[] = {"SOUR1", "SENS2", "SOUR2"};

	if (CMD60_invalidViInt32Range (source, 0, 2))
		return VI_ERROR_PARAMETER2;
		
	if (source == 0)
	{
		/* Source RF In Out */
		if (CMD60_invalidViReal64Range (externalAttenuation, -40.0, 50.0))
			return VI_ERROR_PARAMETER3;
    }
    else
    {
    	/* Source RF In 2 or RF Out 2*/
    	if (CMD60_invalidViReal64Range (externalAttenuation, -40.0, 90.0))
			return VI_ERROR_PARAMETER3;
	}
	
	Fmt (wrtBuf, 
	     "%s<%s:CORR:LOSS %f[p1];*STB?\n",
		 SourValue[source], 
		 externalAttenuation);
		 
    if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
    							 &retCnt)) < 0)
        return CMD60_status;
        
	if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	if ((CMD60_status = CMD60_Register (InstrSession, rdBuf)) < 0)
		return CMD60_status;
	
	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_RoutIoc
/* Purpose :
/*     This function permits to set up the input/output of the CMD.
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     routeIO
/*         Input/Output of the CMD.
/*	       Value range :
/*         0 - RF In/Out
/*         1 - RF In1 / RF Out2
/*         2 - RF In2 / RF Out1
/*         3 - RF In2 / RF Out2
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_RoutIoc (ViSession  InstrSession, 
                                 ViInt32    routeIO)
{
	ViByte    wrtBuf[BUFFER_SIZE];
	ViByte    rdBuf[BUFFER_SIZE];
    ViUInt32  retCnt;
    ViStatus  CMD60_status = VI_SUCCESS;
    ViString  IocValue[] = {"I1O1", "I1O2", "I2O1", "I2O2"};

	if (CMD60_invalidViInt32Range (routeIO, 0, 3))
		return VI_ERROR_PARAMETER2;

	Fmt (wrtBuf, 
	     "%s<ROUT:IOC %s;*STB?\n",
		 IocValue[routeIO]);
		 
    if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
    							 &retCnt)) < 0)
        return CMD60_status;
        
	if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	if ((CMD60_status = CMD60_Register (InstrSession, rdBuf)) < 0)
		return CMD60_status;
	
	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_ConfSyncFreqRef
/* Purpose :
/*     This function permits to ...
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     frequency
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_ConfSyncFreqRef (ViSession  InstrSession, 
                                         ViInt32    frequency)
{
	ViByte    wrtBuf[BUFFER_SIZE];
	ViByte    rdBuf[BUFFER_SIZE];
    ViUInt32  retCnt;
    ViStatus  CMD60_status = VI_SUCCESS;
    ViString  FreqRefValue[] = {"E10M", "I10M"};

	if (CMD60_invalidViInt32Range (frequency, 0, 1))
		return VI_ERROR_PARAMETER2;

	Fmt (wrtBuf, 
	     "%s<CONF:SYNC:FREQ:REF %s;*STB?\n",
		 FreqRefValue[frequency]);
		 
    if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
    							 &retCnt)) < 0)
        return CMD60_status;
        
	if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	if ((CMD60_status = CMD60_Register (InstrSession, rdBuf)) < 0)
		return CMD60_status;
	
	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_ConfSyncFreqOutpRef
/* Purpose :
/*     This function permits to ...
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     frequency
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_ConfSyncFreqOutpRef (ViSession  InstrSession, 
                                             ViInt32    frequency)
{
	ViByte    wrtBuf[BUFFER_SIZE];
	ViByte    rdBuf[BUFFER_SIZE];
    ViUInt32  retCnt;
    ViStatus  CMD60_status = VI_SUCCESS;
    ViString  FreqOutpRefValue[] = {"OS1", "OS2", "OS4", "OS12", "OS24", "OS48", "OS96"};

	if (CMD60_invalidViInt32Range (frequency, 0, 7))
		return VI_ERROR_PARAMETER2;

	Fmt (wrtBuf, 
	     "%s<CONF:SYNC:FREQ:OUTP:REF %s;*STB?\n",
		 FreqOutpRefValue[frequency]);
		 
    if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
    							 &retCnt)) < 0)
        return CMD60_status;
        
	if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	if ((CMD60_status = CMD60_Register (InstrSession, rdBuf)) < 0)
		return CMD60_status;
	
	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_ConfSyncPort
/* Purpose :
/*     This function permits to ...
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     port
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_ConfSyncPort (ViSession  InstrSession, 
                                      ViInt32    port)
{
	ViByte    wrtBuf[BUFFER_SIZE];
	ViByte    rdBuf[BUFFER_SIZE];
    ViUInt32  retCnt;
    ViStatus  CMD60_status = VI_SUCCESS;
    ViString  SyncPortValue[] = {"MAST", "SLAV"};

	if (CMD60_invalidViInt32Range (port, 0, 1))
		return VI_ERROR_PARAMETER2;

	Fmt (wrtBuf, 
	     "%s<CONF:SYNC:PORT %s;*STB?\n",
		 SyncPortValue[port]);
		 
    if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
    							 &retCnt)) < 0)
        return CMD60_status;
        
	if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	if ((CMD60_status = CMD60_Register (InstrSession, rdBuf)) < 0)
		return CMD60_status;
	
	return CMD60_status;
}

/*****************************************************************************/
/*                   AF Generator and Measurements                           */
/*****************************************************************************/

/*===========================================================================*/
/* Function :
/*     CMD60_ConfAfg
/* Purpose :
/*     This function sets up the parameters of AF Generator.
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     levelInVolt
/*         Voltage level.
/*         Value range :
/*         10.0 uv to 5.0 v.
/*     frequencyInHz
/*         Frequency.
/*         Value range :
/*         20.0 to 10000.0 Hz
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_ConfAfg (ViSession  InstrSession,
                                 ViReal64   levelInVolt,
                                 ViReal64   frequencyInHz)
{
	ViByte    wrtBuf[BUFFER_SIZE];
	ViByte    rdBuf[BUFFER_SIZE];
    ViUInt32  retCnt;
    ViStatus  CMD60_status = VI_SUCCESS;
	
	if (CMD60_invalidViReal64Range (levelInVolt, 10.0e-6, 5.0))
		return VI_ERROR_PARAMETER2;
		
	if (CMD60_invalidViReal64Range (frequencyInHz, 20.0, 10000.0))
		return VI_ERROR_PARAMETER3;
	
	Fmt (wrtBuf, 
	     "%s<CONF:AFG:VOLT %f[p5];FREQ %f[p1];*STB?\n", 
	     levelInVolt,
	     frequencyInHz);
	     
    if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
                                 &retCnt)) < 0)
		return CMD60_status;

    if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	if ((CMD60_status = CMD60_Register (InstrSession, rdBuf)) < 0)
		return CMD60_status;
	
	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_ConfAfgenAdpc
/* Purpose :
/*     This function sets up the speech coder of AFG.
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     speechCoder
/*         The source of the speech coder. 
/*         Value range :
/*         0 - On  : The source of speech coder is the AF generator.
/*         1 - Off : The  source of the speech coder is the multifunction
/*                   connector.
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_ConfAfgenAdpc (ViSession  InstrSession, 
                                       ViInt32    speechCoder)
{
	ViByte    wrtBuf[BUFFER_SIZE];
	ViByte    rdBuf[BUFFER_SIZE];
    ViUInt32  retCnt;
    ViStatus  CMD60_status = VI_SUCCESS;
    ViString  AfgenAdpcValue[] = {"ON", "OFF"};

	if (CMD60_invalidViInt32Range (speechCoder, 0, 1))
		return VI_ERROR_PARAMETER2;
	
	Fmt (wrtBuf, 
	     "%s<CONF:AFGEN:ADPC %s;*STB?\n", 
	     AfgenAdpcValue[speechCoder]);
	     
    if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
                                 &retCnt)) < 0)
		return CMD60_status;

    if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	if ((CMD60_status = CMD60_Register (InstrSession, rdBuf)) < 0)
		return CMD60_status;
	
	return CMD60_status;
}

/*===========================================================================*/
/* Function : 
/*     CMD60_SourAfgStat
/* Purpose :
/*     This function permits to switch generator on and off.
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     switchingGenerator
/*         The switching of the AF generator.
/*         Value range :
/*         0 - On  : Switch generator on.
/*         1 - Off : Switch generator off.
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_SourAfgStat (ViSession  InstrSession,
                                     ViInt32    switchingGenerator)
{
	ViByte    wrtBuf[BUFFER_SIZE];
	ViByte    rdBuf[BUFFER_SIZE];
    ViUInt32  retCnt;
    ViStatus  CMD60_status = VI_SUCCESS;
    ViString  AfgStatValue[] = {"ON", "OFF"};

	if (CMD60_invalidViInt32Range (switchingGenerator, 0, 1))
		return VI_ERROR_PARAMETER2;
	
	Fmt (wrtBuf, 
	     "%s<SOUR:AFG:STAT %s;*STB?\n", 
	     AfgStatValue[switchingGenerator]);
	     
    if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
                                 &retCnt)) < 0)
		return CMD60_status;

    if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	if ((CMD60_status = CMD60_Register (InstrSession, rdBuf)) < 0)
		return CMD60_status;
	
	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_ConfAfmFreq
/* Purpose :
/*     This function sets up the others parametres of AFG.
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     distortionFrequencyInHz
/*         ...
/*         Value range :
/*         50.0 to 5000.0 Hz.
/*     lowerLimitFrequencyInHz
/*         ...
/*         Value range :
/*         10 to 1000 Hz.
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_ConfAfmFreq (ViSession  InstrSession,
                                     ViReal64   distortionFrequencyInHz,
                                     ViReal64   lowerLimitFrequencyInHz)
{
	ViByte    wrtBuf[BUFFER_SIZE];
	ViByte    rdBuf[BUFFER_SIZE];
    ViUInt32  retCnt;
    ViStatus  CMD60_status = VI_SUCCESS;

	if (CMD60_invalidViReal64Range (distortionFrequencyInHz, 50.0, 5000.0))
		return VI_ERROR_PARAMETER2;
		
	if (CMD60_invalidViReal64Range (lowerLimitFrequencyInHz, 10.0, 1000.0))
		return VI_ERROR_PARAMETER3;
		
	Fmt (wrtBuf, 
	     "%s<CONF:AFM:FREQ:DIST %f[p1];MIN %f[p1];*STB?\n", 
	     distortionFrequencyInHz,
	     lowerLimitFrequencyInHz);
	     
    if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
                                 &retCnt)) < 0)
		return CMD60_status;

    if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	if ((CMD60_status = CMD60_Register (InstrSession, rdBuf)) < 0)
		return CMD60_status;
	
	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_ConfAfmForm
/* Purpose :
/*     This function modifies the display format of AF counter and voltmeter.
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*	   displayFormatOfAFCounter
/*         Format of AF counter.
/*         Value range :
/*         0 - Frequency to measure < 10 kHz.
/*         1 - Frequency to measure < 30 MHz.
/*         2 - Frequency to measure < 60 MHz.
/*         3 - ?
/*	   displayFormatOfVoltmeter
/*         Format of voltmeter.
/*         Value range :
/*         0 - Rms format.
/*         1 - Sqrt format.
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_ConfAfmForm (ViSession  InstrSession,
                                     ViInt32    displayFormatOfAFCounter,
                                     ViInt32    displayFormatOfVoltmeter)
{
	ViByte    wrtBuf[BUFFER_SIZE];
	ViByte    rdBuf[BUFFER_SIZE];
    ViUInt32  retCnt;
    ViStatus  CMD60_status = VI_SUCCESS;
    ViString  AfmCounFormValue[] = {"LT10", "LT30", "LT60", "GT10"};
    ViString  AfmVoltFormValue[] = {"RMS", "SQRT"};
    
	if (CMD60_invalidViInt32Range (displayFormatOfAFCounter, 0, 3))
		return VI_ERROR_PARAMETER2;
		
	if (CMD60_invalidViInt32Range (displayFormatOfVoltmeter, 0, 1))
		return VI_ERROR_PARAMETER3;
		
	Fmt (wrtBuf, 
	     "%s<CONF:AFM:COUN:FORM %s;:CONF:AFM:VOLT:FORM %s;*STB?\n", 
	     AfmCounFormValue[displayFormatOfAFCounter],
	     AfmVoltFormValue[displayFormatOfVoltmeter]);
	     
    if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
                                 &retCnt)) < 0)
		return CMD60_status;

    if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	if ((CMD60_status = CMD60_Register (InstrSession, rdBuf)) < 0)
		return CMD60_status;
	
	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_ConfAfmInp
/* Purpose :
/*     This function permits to set the input of the AF voltmeter.
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     input 
/*         The input of the AF voltmeter.
/*         Value range :
/*         0 - AF Voltmeter : Connector AF voltmeter.
/*         1 - ADPCM        : Apply the input of the AF voltmeter
/*                            directly to the output of the ADPCM
/*                            (signal of the speech decoder).
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_ConfAfmInp (ViSession  InstrSession, 
                                    ViInt32    input)
{
	ViByte    wrtBuf[BUFFER_SIZE];
	ViByte    rdBuf[BUFFER_SIZE];
    ViUInt32  retCnt;
    ViStatus  CMD60_status = VI_SUCCESS;
    ViString  AfmInpValue[] = {"AFV", "ADPC"};

	if (CMD60_invalidViInt32Range (input, 0, 1))
		return VI_ERROR_PARAMETER2;
	
	Fmt (wrtBuf, 
	     "%s<CONF:AFM:INP %s;*STB?\n", 
	     AfmInpValue[input]);
	     
    if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
                                 &retCnt)) < 0)
		return CMD60_status;

    if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	if ((CMD60_status = CMD60_Register (InstrSession, rdBuf)) < 0)
		return CMD60_status;
	
	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_ConfAfmVoltBpas
/* Purpose :
/*     This function permits to set the parameters of the bandpass filter.
/*     - the operating mode (Activate (ON) or Deactivate (OFF)).
/*     - the bandwidth of the bandpass filter.
/*     - the center frequency of the bandpass filter.
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     operatingMode
/*         The mode of the bandpass filter.
/*         Value range :
/*         0 - On  : The bandpass filter is activated.
/*         1 - Off : The bandpass filter is deactivated.
/*     bandwidthInHz
/*         The bandwidth of the bandpass filter.
/*         Value range :
/*         10.0 to 1000.0 Hz
/*     centerFrequencyInHz
/*         The center frequency of the bandpass filter.
/*         Value range :
/*         500.0 to 5000.0 Hz
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_ConfAfmVoltBpas (ViSession  InstrSession,
                                         ViInt32    operatingMode,
                                         ViReal64   bandwidthInHz,
                                         ViReal64   centerFrequencyInHz)
{
	ViByte    wrtBuf[BUFFER_SIZE];
	ViByte    rdBuf[BUFFER_SIZE];
    ViUInt32  retCnt;
    ViStatus  CMD60_status = VI_SUCCESS;
    
    if (CMD60_invalidViInt32Range (operatingMode, 0, 1))
		return VI_ERROR_PARAMETER2;

	if (CMD60_invalidViReal64Range (bandwidthInHz, 10.0, 1000.0))
		return VI_ERROR_PARAMETER3;
		
	if (CMD60_invalidViReal64Range (centerFrequencyInHz, 500.0, 5000.0))
		return VI_ERROR_PARAMETER4;
		
	/* Set up the bandpass filter of the AF-Voltmeter  */
	if (operatingMode == 1)
	{
		/* Deactivate the bandpass filter */
		Fmt (wrtBuf, 
	         "%s<CONF:AFM:VOLT:BPAS:FREQ OFF;*STB?\n");
	}
	else
	{
		/* Activate the bandpass filter */
		Fmt (wrtBuf, 
	         "%s<CONF:AFM:VOLT:BPAS:WIDT %f[p1];:CONF:AFM:VOLT:BPAS:FREQ %f[p1];*STB?\n", 
	         bandwidthInHz,
	         centerFrequencyInHz);
	}
	
    if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
                                 &retCnt)) < 0)
		return CMD60_status;

    if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	if ((CMD60_status = CMD60_Register (InstrSession, rdBuf)) < 0)
		return CMD60_status;
	
	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_ConfBanPowTranNormExp
/* Purpose :
/*     This function permits to set of the maximum expected power of the MS.
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     expectedPowerOfMSIndBm 
/*         ...
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_ConfBanPowTranNormExp (ViSession  InstrSession,
                                               ViReal64   expectedPowerOfMSIndBm)
{
	ViByte    wrtBuf[BUFFER_SIZE];
	ViByte    rdBuf[BUFFER_SIZE];
    ViUInt32  retCnt;
    ViStatus  CMD60_status = VI_SUCCESS;

	if (CMD60_invalidViReal64Range (expectedPowerOfMSIndBm, -30.0, +30.0))
		return VI_ERROR_PARAMETER2;
		
	Fmt (wrtBuf, 
	     "%s<CONF:BAN:POW:TRAN:NORM:EXP %f[p1]dBm;*STB?\n", 
	     expectedPowerOfMSIndBm);
	     
    if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
                                 &retCnt)) < 0)
		return CMD60_status;

    if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	if ((CMD60_status = CMD60_Register (InstrSession, rdBuf)) < 0)
		return CMD60_status;
	
	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_ConfBanFreq
/* Purpose :
/*     This function permits to set of the frequency of the signal to be 
/*     received.
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     frequencyInMHz
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_ConfBanFreq (ViSession  InstrSession,
                                     ViReal64   frequencyInMHz)
{
	ViByte    wrtBuf[BUFFER_SIZE];
	ViByte    rdBuf[BUFFER_SIZE];
    ViUInt32  retCnt;
    ViStatus  CMD60_status = VI_SUCCESS;

	if (CMD60_invalidViReal64Range (frequencyInMHz, 1876.608, 1935.360))
		return VI_ERROR_PARAMETER2;
		
	Fmt (wrtBuf, 
	     "%s<CONF:BAN:FREQ %f[p3]MHz;*STB?\n", 
	     frequencyInMHz);
	     
    if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
                                 &retCnt)) < 0)
		return CMD60_status;

    if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	if ((CMD60_status = CMD60_Register (InstrSession, rdBuf)) < 0)
		return CMD60_status;
	
	return CMD60_status;
}
                                 
/*===========================================================================*/
/* Function :
/*     CMD60_ConfBanMeasWind
/* Purpose :
/*     This function permits to set of different discrete values for the 
/*     measuring window between 25탎 and 1000탎.
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     Window 
/*         Index of the measuring window array "MeasWindValue".
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_ConfBanMeasWind (ViSession  InstrSession, 
                                         ViInt32    Window)
{
	ViByte    wrtBuf[BUFFER_SIZE];
	ViByte    rdBuf[BUFFER_SIZE];
    ViUInt32  retCnt;
    ViStatus  CMD60_status = VI_SUCCESS;
    ViString  MeasWindValue[] = { "T25",  "T50",  "T75",
                                  "T83", "T100", "T150",
                                 "T156", "T200", "T250",
                                 "T300", "T350", "T364",
                                 "T400", "T450", "T500",
                                 "T750", "T781", "T1K"};
    
    if (CMD60_invalidViInt32Range (Window, 0, 17))
		return VI_ERROR_PARAMETER2;
	
	Fmt (wrtBuf, 
	     "%s<CONF:BAN:MEAS:WIND %s;*STB?\n", 
	     MeasWindValue[Window]);
	     
    if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
                                 &retCnt)) < 0)
		return CMD60_status;

    if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	if ((CMD60_status = CMD60_Register (InstrSession, rdBuf)) < 0)
		return CMD60_status;
	
	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_ConfBanTrig
/* Purpose :
/*     This function permits to set the trigger mode and the delay for the 
/*     measurement after the trigger event from 0 to 10ms in steps of 1탎.
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     trigger 
/*         Index of the trigger mode array.
/*     delay
/*         Delay in 탎.
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_ConfBanTrig (ViSession  InstrSession,
                                     ViInt32    trigger, 
                                     ViInt32    delay)
{
	ViByte    wrtBuf[BUFFER_SIZE];
	ViByte    rdBuf[BUFFER_SIZE];
    ViUInt32  retCnt;
    ViStatus  CMD60_status = VI_SUCCESS;
    ViString  TrigValue[] = { "EXT", "RSL", "FRE"};
    
    if (CMD60_invalidViInt32Range (trigger, 0, 2))
		return VI_ERROR_PARAMETER2;
		
	if (CMD60_invalidViInt32Range (delay, 0, 10000))
		return VI_ERROR_PARAMETER3;
	
	Fmt (wrtBuf, 
	     "%s<CONF:BAN:TRIG %s;:CONF:BAN:TRIG:DEL %f[p6];*STB?\n", 
	     TrigValue[trigger],
	     delay/1e6);
	     
    if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
                                 &retCnt)) < 0)
		return CMD60_status;

    if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	if ((CMD60_status = CMD60_Register (InstrSession, rdBuf)) < 0)
		return CMD60_status;
	
	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_ConfRfg
/* Purpose :
/*     This function permits to ...
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     Setting
/*     FrequencyInMHz
/*     DeviationInkHz
/*     Signal
/*     RFLevelIndBm
/*     SignalPattern1to20
/*     SignalPattern21to40
/*     SignalPattern41to60
/*     SignalPattern61to80
/*     SignalPattern81to100
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_ConfRfg (ViSession       InstrSession, 
                                 ViInt32         Setting,
                                 ViReal64        FrequencyInMHz, 
                                 ViReal64        DeviationInkHz,
                                 ViInt32         Signal, 
                                 ViReal64        RFLevelIndBm,
                                 ViChar _VI_FAR  SignalPattern1to20[],
                                 ViChar _VI_FAR  SignalPattern21to40[],
                                 ViChar _VI_FAR  SignalPattern41to60[],
                                 ViChar _VI_FAR  SignalPattern61to80[],
                                 ViChar _VI_FAR  SignalPattern81to100[])
{
	ViByte    wrtBuf[BUFFER_SIZE];
	ViByte    rdBuf[BUFFER_SIZE];
    ViUInt32  retCnt;
    ViStatus  CMD60_status = VI_SUCCESS;
    ViString  SettValue[] = { "SET1", "SET2", "SET3", "SET4", "SET5", "SET6", "SET7"};
    ViString  SignValue[] = {  "B55",  "B33",  "B0F",  "CE55", "CE33", "CE0F", 
                              "DB55",  "DB33", "DB0F", "DBFIG31", "DBPR", "DBUS" };

	if (CMD60_invalidViInt32Range (Setting, 0, 6))
		return VI_ERROR_PARAMETER2;
	
	if (CMD60_invalidViReal64Range (FrequencyInMHz, 1876.608, 1902.528))
		return VI_ERROR_PARAMETER3;
	
	if (CMD60_invalidViReal64Range (DeviationInkHz, 0.0, 510.0))
		return VI_ERROR_PARAMETER4;
	
	if (CMD60_invalidViInt32Range (Signal, 0, 11))
		return VI_ERROR_PARAMETER5;
	
	if (CMD60_invalidViReal64Range (RFLevelIndBm, -100.0, -40.0))
		return VI_ERROR_PARAMETER6;
		
	if (StringLength (SignalPattern1to20) != 20)
		return CMD60_ERROR_STRING_LENGTH;
		
	if (StringLength (SignalPattern21to40) != 20)
		return CMD60_ERROR_STRING_LENGTH;
		
	if (StringLength (SignalPattern41to60) != 20)
		return CMD60_ERROR_STRING_LENGTH;
	
	if (StringLength (SignalPattern61to80) != 20)
		return CMD60_ERROR_STRING_LENGTH;
		
	if (StringLength (SignalPattern81to100) != 20)
		return CMD60_ERROR_STRING_LENGTH;
	
	Fmt (wrtBuf, 
	     "%s<CONF:RFG:SEL:SETT %s;:CONF:RFG:FREQ:OUTP %f[p3] MHz;:CONF:RFG:FREQ:DEV %f[p1] kHz;:CONF:RFG:SIGN %s;:CONF:RFG:RF:LEV %f[p1] dBm;*STB?\n", 
	     SettValue[Setting],
	     FrequencyInMHz,
	     DeviationInkHz,
	     SignValue[Signal],
	     RFLevelIndBm);
	     
    if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
                                 &retCnt)) < 0)
		return CMD60_status;

    if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	if ((CMD60_status = CMD60_Register (InstrSession, rdBuf)) < 0)
		return CMD60_status;
	
	if (Signal == 11)
	{
		Fmt (wrtBuf, 
	         "%s<CONF:RFG:SIGN:PATT \"%s%s%s%s%s\";*STB?\n", 
	         SignalPattern1to20,
	         SignalPattern21to40,
	         SignalPattern41to60,
	         SignalPattern61to80,
	         SignalPattern81to100);
	     
	    if ((CMD60_status = viWrite (InstrSession, 
                                     wrtBuf, 
                                     NumFmtdBytes (), 
                                     &retCnt)) < 0)
			return CMD60_status;

    	if ((CMD60_status = viRead (InstrSession, 
	                                rdBuf, 
								    BUFFER_SIZE, 
								    &retCnt)) < 0)
			return CMD60_status;
		
		if ((CMD60_status = CMD60_Register (InstrSession, rdBuf)) < 0)
			return CMD60_status;
	}
	
	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_SourRfgSelSett
/* Purpose :
/*     This function permits to ...
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     SettingNumber
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_SourRfgSelSett (ViSession  InstrSession, 
                                        ViInt32    SettingNumber)
{
	ViByte    wrtBuf[BUFFER_SIZE];
	ViByte    rdBuf[BUFFER_SIZE];
    ViUInt32  retCnt;
    ViStatus  CMD60_status = VI_SUCCESS;
    ViString  SettValue[] = { "SET1", "SET2", "SET3", "SET4", "SET5", "SET6", "SET7", "NONE"};
    
	if (CMD60_invalidViInt32Range (SettingNumber, 0, 7))
		return VI_ERROR_PARAMETER2;
	
	Fmt (wrtBuf, 
	     "%s<SOUR:RFG:SEL:SETT %s;*STB?\n", 
	     SettValue[SettingNumber]);
	     
    if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
                                 &retCnt)) < 0)
		return CMD60_status;

    if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	if ((CMD60_status = CMD60_Register (InstrSession, rdBuf)) < 0)
		return CMD60_status;
	
	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_SourRfgFreqOutp
/* Purpose :
/*     This function permits to ...
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     FrequencyInMHz
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_SourRfgFreqOutp (ViSession  InstrSession, 
                                         ViReal64   FrequencyInMHz)
{
	ViByte    wrtBuf[BUFFER_SIZE];
	ViByte    rdBuf[BUFFER_SIZE];
    ViUInt32  retCnt;
    ViStatus  CMD60_status = VI_SUCCESS;
    
    if (CMD60_invalidViReal64Range (FrequencyInMHz, 1876.608, 1935.360))
		return VI_ERROR_PARAMETER2;
	
	Fmt (wrtBuf, 
	     "%s<SOUR:RFG:FREQ:OUTP %f[p3] MHz;*STB?\n", 
	     FrequencyInMHz);
	     
    if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
                                 &retCnt)) < 0)
		return CMD60_status;

    if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	if ((CMD60_status = CMD60_Register (InstrSession, rdBuf)) < 0)
		return CMD60_status;
	
	return CMD60_status;
}
                                         
/*===========================================================================*/
/* Function :
/*     CMD60_SourRfgFreqDev
/* Purpose :
/*     This function permits to ...
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     DeviationInkHz
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_SourRfgFreqDev (ViSession  InstrSession, 
                                        ViReal64   DeviationInkHz)
{
	ViByte    wrtBuf[BUFFER_SIZE];
	ViByte    rdBuf[BUFFER_SIZE];
    ViUInt32  retCnt;
    ViStatus  CMD60_status = VI_SUCCESS;
    
	if (CMD60_invalidViReal64Range (DeviationInkHz, 0.0, 510.0))
		return VI_ERROR_PARAMETER2;
	
	Fmt (wrtBuf, 
	     "%s<SOUR:RFG:FREQ:DEV %f[p1] kHz;*STB?\n", 
	     DeviationInkHz);
	     
    if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
                                 &retCnt)) < 0)
		return CMD60_status;

    if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	if ((CMD60_status = CMD60_Register (InstrSession, rdBuf)) < 0)
		return CMD60_status;
	
	return CMD60_status;
}
                                        
/*===========================================================================*/
/* Function :
/*     CMD60_SourRfgRfLev
/* Purpose :
/*     This function permits to ...
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     RFLevelIndBm
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_SourRfgRfLev (ViSession  InstrSession, 
                                      ViReal64   RFLevelIndBm)
{
	ViByte    wrtBuf[BUFFER_SIZE];
	ViByte    rdBuf[BUFFER_SIZE];
    ViUInt32  retCnt;
    ViStatus  CMD60_status = VI_SUCCESS;
    
	if (CMD60_invalidViReal64Range (RFLevelIndBm, -100.0, -40.0))
		return VI_ERROR_PARAMETER2;
	
	Fmt (wrtBuf, 
	     "%s<SOUR:RFG:RF:LEV %f[p1] dBm;*STB?\n", 
	     RFLevelIndBm);
	     
    if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
                                 &retCnt)) < 0)
		return CMD60_status;

    if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	if ((CMD60_status = CMD60_Register (InstrSession, rdBuf)) < 0)
		return CMD60_status;
	
	return CMD60_status;
}
                                      
/*===========================================================================*/
/* Function :
/*     CMD60_SourRfgSign
/* Purpose :
/*     This function permits to ...
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     Signal
/*     SignalPattern1to20
/*     SignalPattern21to40
/*     SignalPattern41to60
/*     SignalPattern61to80
/*     SignalPattern81to100
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_SourRfgSign (ViSession       InstrSession, 
                                     ViInt32         Signal,
                                     ViChar _VI_FAR  SignalPattern1to20[],
                                     ViChar _VI_FAR  SignalPattern21to40[],
                                     ViChar _VI_FAR  SignalPattern41to60[],
                                     ViChar _VI_FAR  SignalPattern61to80[],
                                     ViChar _VI_FAR  SignalPattern81to100[])
{
	ViByte    wrtBuf[BUFFER_SIZE];
	ViByte    rdBuf[BUFFER_SIZE];
    ViUInt32  retCnt;
    ViStatus  CMD60_status = VI_SUCCESS;
    ViString  SignValue[] = {  "B55",  "B33",  "B0F",  "CE55", "CE33", "CE0F", 
                              "DB55",  "DB33", "DB0F", "DBFIG31", "DBPR", "DBUS" };

	if (CMD60_invalidViInt32Range (Signal, 0, 11))
		return VI_ERROR_PARAMETER2;
	
	if (StringLength (SignalPattern1to20) != 20)
		return CMD60_ERROR_STRING_LENGTH;
		
	if (StringLength (SignalPattern21to40) != 20)
		return CMD60_ERROR_STRING_LENGTH;
		
	if (StringLength (SignalPattern41to60) != 20)
		return CMD60_ERROR_STRING_LENGTH;
	
	if (StringLength (SignalPattern61to80) != 20)
		return CMD60_ERROR_STRING_LENGTH;
		
	if (StringLength (SignalPattern81to100) != 20)
		return CMD60_ERROR_STRING_LENGTH;
		
	Fmt (wrtBuf, 
	     "%s<SOUR:RFG:SIGN %s;*STB?\n", 
	     SignValue[Signal]);
	     
    if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
                                 &retCnt)) < 0)
		return CMD60_status;

    if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	if ((CMD60_status = CMD60_Register (InstrSession, rdBuf)) < 0)
		return CMD60_status;
	
	if (Signal == 11)
	{
		Fmt (wrtBuf, 
	         "%s<SOUR:RFG:SIGN:PATT \"%s%s%s%s%s\";*STB?\n", 
	         SignalPattern1to20,
	         SignalPattern21to40,
	         SignalPattern41to60,
	         SignalPattern61to80,
	         SignalPattern81to100);
	     
	    if ((CMD60_status = viWrite (InstrSession, 
                                     wrtBuf, 
                                     NumFmtdBytes (), 
                                     &retCnt)) < 0)
			return CMD60_status;

    	if ((CMD60_status = viRead (InstrSession, 
	                                rdBuf, 
								    BUFFER_SIZE, 
								    &retCnt)) < 0)
			return CMD60_status;
		
		if ((CMD60_status = CMD60_Register (InstrSession, rdBuf)) < 0)
			return CMD60_status;
	}
	
	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_ProcConnSet
/* Purpose :
/*     This function permits to ...
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_ProcConnSet (ViSession  InstrSession)
{
	ViByte    wrtBuf[BUFFER_SIZE];
	ViByte    rdBuf[BUFFER_SIZE];
    ViUInt32  retCnt;
    ViStatus  CMD60_status = VI_SUCCESS;
    
	Fmt (wrtBuf, 
	     "%s<PROC:CONN:SET;*STB?\n");
	     
    if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
                                 &retCnt)) < 0)
		return CMD60_status;

    if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	if ((CMD60_status = CMD60_Register (InstrSession, rdBuf)) < 0)
		return CMD60_status;
	
	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_ProcConnRel
/* Purpose :
/*     This function permits to ...
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_ProcConnRel (ViSession  InstrSession)
{
	ViByte    wrtBuf[BUFFER_SIZE];
	ViByte    rdBuf[BUFFER_SIZE];
    ViUInt32  retCnt;
    ViStatus  CMD60_status = VI_SUCCESS;
    
	Fmt (wrtBuf, 
	     "%s<PROC:CONN:REL;*STB?\n");
	     
    if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
                                 &retCnt)) < 0)
		return CMD60_status;

    if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	if ((CMD60_status = CMD60_Register (InstrSession, rdBuf)) < 0)
		return CMD60_status;
	
	return CMD60_status;
}

/*
///////////////////////////////////////////////////////////////////////////////
//              A C T I O N / S T A T U S    F U N C T I O N S
///////////////////////////////////////////////////////////////////////////////
*/

/*===========================================================================*/
/* Function :
/*     CMD60_StatDev
/* Purpose :
/*     This function permits to ...
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     StatDev
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_StatDev (ViSession         InstrSession,
                                 ViInt32 _VI_FAR  *StatDev)
{
	ViByte  	wrtBuf[BUFFER_SIZE];
	ViByte      rdBuf[BUFFER_SIZE];
	ViByte		Buf[256];
	ViByte      Result[256];
	ViUInt32	retCnt;
	ViStatus  	CMD60_status = VI_SUCCESS;

	Fmt (wrtBuf, 
	     "%s<STAT:DEV?;*STB?\n");
	
	if ((CMD60_status = viWrite (InstrSession, 
	                             wrtBuf, 
	                             NumFmtdBytes (), 
    							 &retCnt)) < 0)
		return CMD60_status;
        
	if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;

	Scan (rdBuf, "%s>%s[t59x]%s", Result, Buf);
	
	if ((CMD60_status = CMD60_Register (InstrSession, Buf)) < 0)
		return CMD60_status;

    if (CompareStrings ("IDLE", 0, Result, 0, 1) == 0)		
		*StatDev  = 1;
    else if (CompareStrings ("DFP", 0, Result, 0, 1) == 0)
    	*StatDev = 2;
    else if (CompareStrings ("DPP", 0, Result, 0, 1) == 0)
    	*StatDev = 3;
    else if (CompareStrings ("DCE", 0, Result, 0, 1) == 0)
    	*StatDev = 4;
    else if (CompareStrings ("DBAN", 0, Result, 0, 1) == 0)
    	*StatDev = 5;
    else if (CompareStrings ("DRFG", 0, Result, 0, 1) == 0)
    	*StatDev = 6;

	return CMD60_status;
}

/*
///////////////////////////////////////////////////////////////////////////////
//                        D A T A    F U N C T I O N S
///////////////////////////////////////////////////////////////////////////////
*/

/*===========================================================================*/
/* Function :
/*     CMD60_SensSignStat
/* Purpose :
/*     This function permits to ...
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     LockState
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_SensSignStat (ViSession        InstrSession, 
                                      ViInt32 _VI_FAR *LockState)
{
	ViByte  	wrtBuf[BUFFER_SIZE]; 
	ViByte      rdBuf[BUFFER_SIZE];
	ViByte		Buf[256]; 
	ViByte      Result[256];
	ViUInt32	retCnt;
	ViStatus  	CMD60_status = VI_SUCCESS;

	Fmt (wrtBuf, 
	     "%s<SENS:SIGN:STAT?;*STB?\n");
	
	if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
    							 &retCnt)) < 0)
		return CMD60_status;
        
	if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	Scan (rdBuf, "%s>%s[t59x]%s", Result, Buf);
	
	if ((CMD60_status = CMD60_Register (InstrSession, Buf)) < 0)
		return CMD60_status;
		
	if (CompareStrings ("LOCK", 0, Result, 0, 1) == 0)
		*LockState = 0;
	else if (CompareStrings ("UNL", 0, Result, 0, 1) == 0)
		*LockState = 1;

	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_SensDetDummCarr
/* Purpose :
/*     This function permits to ...
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     DummyCarrierValue
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_SensDetDummCarr (ViSession        InstrSession,
                                         ViInt32 _VI_FAR *DummyCarrierValue)
{
	ViByte  	wrtBuf[BUFFER_SIZE]; 
	ViByte      rdBuf[BUFFER_SIZE];
	ViByte		Buf[256]; 
	ViByte      Result[256];
	ViUInt32	retCnt;
	ViStatus  	CMD60_status = VI_SUCCESS;

	Fmt (wrtBuf, 
	     "%s<SENS:DET:DUMM:CARR?;*STB?\n");
	
	if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
    							 &retCnt)) < 0)
		return CMD60_status;
        
	if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	Scan (rdBuf, "%s>%s[t59x]%s", Result, Buf);
	
	if ((CMD60_status = CMD60_Register (InstrSession, Buf)) < 0)
		return CMD60_status;
		
	if (Scan (Result, "%s>%d", DummyCarrierValue) == 0)
		return VI_ERROR_INV_MEAS;

	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_SensDetDummSlot
/* Purpose :
/*     This function permits to ...
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     DummySlotValue
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_SensDetDummSlot (ViSession        InstrSession, 
                                         ViInt32 _VI_FAR *DummySlotValue)
{
	ViByte  	wrtBuf[BUFFER_SIZE]; 
	ViByte      rdBuf[BUFFER_SIZE];
	ViByte		Buf[256]; 
	ViByte      Result[256];
	ViUInt32	retCnt;
	ViStatus  	CMD60_status = VI_SUCCESS;

	Fmt (wrtBuf, 
	     "%s<SENS:DET:DUMM:SLOT?;*STB?\n");
	
	if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
    							 &retCnt)) < 0)
		return CMD60_status;
        
	if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	Scan (rdBuf, "%s>%s[t59x]%s", Result, Buf);
	
	if ((CMD60_status = CMD60_Register (InstrSession, Buf)) < 0)
		return CMD60_status;
		
	if (Scan (Result, "%s>%d", DummySlotValue) == 0)
		return VI_ERROR_INV_MEAS;

	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_SensDetRfpi
/* Purpose :
/*     This function permits to ...
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     RFPIValue
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_SensDetRfpi (ViSession       InstrSession, 
                                     ViChar _VI_FAR  RfpiValue[])
{
	ViByte  	wrtBuf[BUFFER_SIZE]; 
	ViByte      rdBuf[BUFFER_SIZE];
	ViByte		Buf[256]; 
	ViByte      Result[256];
	ViUInt32	retCnt;
	ViStatus  	CMD60_status = VI_SUCCESS;
	
	Fmt (wrtBuf, 
	     "%s<SENS:DET:RFPI?;*STB?\n");
	
	if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
    							 &retCnt)) < 0)
		return CMD60_status;
        
	if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	Scan (rdBuf, "%s>%s[t59x]%s", Result, Buf);
	
	if ((CMD60_status = CMD60_Register (InstrSession, Buf)) < 0)
		return CMD60_status;
		
	if (Scan (Result, "%s>%s", RfpiValue) == 0)
		return VI_ERROR_INV_MEAS;

	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_SensDetPmid
/* Purpose :
/*     This function permits to ...
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     PMIDValue
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_SensDetPmid (ViSession       InstrSession, 
                                     ViChar _VI_FAR  PmidValue[])
{
	ViByte  	wrtBuf[BUFFER_SIZE]; 
	ViByte      rdBuf[BUFFER_SIZE];
	ViByte		Buf[256]; 
	ViByte      Result[256];
	ViUInt32	retCnt;
	ViStatus  	CMD60_status = VI_SUCCESS;
	
	Fmt (wrtBuf, 
	     "%s<SENS:DET:PMID?;*STB?\n");
	
	if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
    							 &retCnt)) < 0)
		return CMD60_status;
        
	if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	Scan (rdBuf, "%s>%s[t59x]%s", Result, Buf);
	
	if ((CMD60_status = CMD60_Register (InstrSession, Buf)) < 0)
		return CMD60_status;
		
	if (Scan (Result, "%s>%s", PmidValue) == 0)
		return VI_ERROR_INV_MEAS;

	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_ReadAfmCoun
/* Purpose :
/*     This function permits to measure the AF counter.
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     AFCounter
/*         The AF counter measured.
/*         Return :
/*         <value> (Unit: Hz).
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_ReadAfmCoun (ViSession         InstrSession, 
                                     ViReal64 _VI_FAR *AFCounter)
{
	ViByte  	wrtBuf[BUFFER_SIZE]; 
	ViByte      rdBuf[BUFFER_SIZE];
	ViByte		Buf[256]; 
	ViByte      Result[256];
	ViUInt32	retCnt;
	ViStatus  	CMD60_status = VI_SUCCESS;

	Fmt (wrtBuf, 
	     "%s<READ:AFM:COUN?;*STB?\n");
	
	if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
    							 &retCnt)) < 0)
		return CMD60_status;
        
	if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	Scan (rdBuf, "%s>%s[t59x]%s", Result, Buf);
	
	if ((CMD60_status = CMD60_Register (InstrSession, Buf)) < 0)
		return CMD60_status;
		
	if (Scan (Result, "%s>%f", AFCounter) == 0)
		return VI_ERROR_INV_MEAS;

	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_ReadAfmVolt
/* Purpose :
/*     This function permits to measure the AF voltage.
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     AFVoltage
/*         The AF voltage measured.
/*         Return :
/*         <value> (Unit: V).
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_ReadAfmVolt (ViSession         InstrSession, 
                                     ViReal64 _VI_FAR *AFVoltage)
{
	ViByte  	wrtBuf[BUFFER_SIZE]; 
	ViByte      rdBuf[BUFFER_SIZE];
	ViByte		Buf[256]; 
	ViByte      Result[256];
	ViUInt32	retCnt;
	ViStatus  	CMD60_status = VI_SUCCESS;

	Fmt (wrtBuf, 
	     "%s<READ:AFM:VOLT?;*STB?\n");
	
	if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
    							 &retCnt)) < 0)
		return CMD60_status;
        
	if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	Scan (rdBuf, "%s>%s[t59x]%s", Result, Buf);
	
	if ((CMD60_status = CMD60_Register (InstrSession, Buf)) < 0)
		return CMD60_status;
		
	if (Scan (Result, "%s>%f", AFVoltage) == 0)
		return VI_ERROR_INV_MEAS;

	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_ReadAfmDist
/* Purpose :
/*     This function permits to measure the AF distortion.
/* Parameters :
/*      InstrSession
/*Session of the instrument driver.
/*      AfDistortion
/*          The AF distorsion measured.
/*          Return :
/*          <value> (Unit: %).
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_ReadAfmDist (ViSession         InstrSession, 
                                     ViReal64 _VI_FAR *AfDistortion)
{

	ViByte  	wrtBuf[BUFFER_SIZE]; 
	ViByte      rdBuf[BUFFER_SIZE];
	ViByte		Buf[256]; 
	ViByte      Result[256];
	ViUInt32	retCnt;
	ViStatus  	CMD60_status = VI_SUCCESS;

	Fmt (wrtBuf, 
	     "%s<READ:AFM:DIST?;*STB?\n");
	
	if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
    							 &retCnt)) < 0)
		return CMD60_status;
        
	if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	Scan (rdBuf, "%s>%s[t59x]%s", Result, Buf);
	
	if ((CMD60_status = CMD60_Register (InstrSession, Buf)) < 0)
		return CMD60_status;
	
	if (Scan (Result, "%s>%f", AfDistortion) == 0)
		return VI_ERROR_INV_MEAS;

	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_ReadCurr
/* Purpose :
/*     This function permits to measure the current.
/* Parameters :
/*      InstrSession
/*          Session of the instrument driver.
/*      CurrentDc
/*          The average current measured.
/*          Return :
/*          <value> (Unit: A).
/*      CurrentDcMax
/*          The maximum current measured.
/*          Return :
/*          <value> (Unit: A).
/*      CurrentDcMin
/*          The minimum current measured.
/*          Return :
/*          <value> (Unit: A).
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_ReadCurr (ViSession         InstrSession, 
                                  ViReal64 _VI_FAR *CurrentDc,
                                  ViReal64 _VI_FAR *CurrentDcMax,
                                  ViReal64 _VI_FAR *CurrentDcMin)
{

	ViByte  	wrtBuf[BUFFER_SIZE]; 
	ViByte      rdBuf[BUFFER_SIZE];
	ViByte		Buf[256]; 
	ViByte      Result[256];
	ViUInt32	retCnt;
	ViStatus  	CMD60_status = VI_SUCCESS;

	/* New average current measurement */
	Fmt (wrtBuf, 
	     "%s<READ:CURR?;*STB?\n");
	
	if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
    							 &retCnt)) < 0)
		return CMD60_status;
        
	if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	Scan (rdBuf, "%s>%s[t59x]%s", Result, Buf);
	
	if ((CMD60_status = CMD60_Register (InstrSession, Buf)) < 0)
		return CMD60_status;
	
	if (Scan (Result, "%s>%f", CurrentDc) == 0)
		return VI_ERROR_INV_MEAS;
		
	/* Read only maximum current measurement */
		
	Fmt (wrtBuf, 
	     "%s<FETC:CURR:MAX?;*STB?\n");
	
	if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
    							 &retCnt)) < 0)
		return CMD60_status;
        
	if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	Scan (rdBuf, "%s>%s[t59x]%s", Result, Buf);
	
	if ((CMD60_status = CMD60_Register (InstrSession, Buf)) < 0)
		return CMD60_status;
	
	if (Scan (Result, "%s>%f", CurrentDcMax) == 0)
		return VI_ERROR_INV_MEAS;
	
	/* Read only minimum current measurement */
	
	Fmt (wrtBuf, 
	     "%s<FETC:CURR:MIN?;*STB?\n");
	
	if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
    							 &retCnt)) < 0)
		return CMD60_status;
        
	if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	Scan (rdBuf, "%s>%s[t59x]%s", Result, Buf);
	
	if ((CMD60_status = CMD60_Register (InstrSession, Buf)) < 0)
		return CMD60_status;
	
	if (Scan (Result, "%s>%f", CurrentDcMin) == 0)
		return VI_ERROR_INV_MEAS;
	
	
	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_ReadVolt
/* Purpose :
/*     This function permits to measure the voltage.
/* Parameters :
/*      InstrSession
/*          Session of the instrument driver.
/*      VoltDc
/*          The voltage measured.
/*          Return :
/*          <value> (Unit: V).
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_ReadVolt (ViSession         InstrSession, 
                                  ViReal64 _VI_FAR *VoltDc)
{

	ViByte  	wrtBuf[BUFFER_SIZE]; 
	ViByte      rdBuf[BUFFER_SIZE];
	ViByte		Buf[256]; 
	ViByte      Result[256];
	ViUInt32	retCnt;
	ViStatus  	CMD60_status = VI_SUCCESS;

	/* New voltage measurement */
	Fmt (wrtBuf, 
	     "%s<READ:VOLT?;*STB?\n");
	
	if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
    							 &retCnt)) < 0)
		return CMD60_status;
        
	if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	Scan (rdBuf, "%s>%s[t59x]%s", Result, Buf);
	
	if ((CMD60_status = CMD60_Register (InstrSession, Buf)) < 0)
		return CMD60_status;
	
	if (Scan (Result, "%s>%f", VoltDc) == 0)
		return VI_ERROR_INV_MEAS;
		
	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_CalcLimPowTranTolMatc
/* Purpose :
/*     This function permits to query the verdict of the measurement of the
/*     NTP according the tolerances setting.
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     VerdictForNTP
/*         The verdict of the measured NTP according the tolerances setting.
/*         Return :
/*         <0> : MATC | measurement does not exceed the tolerance.
/*         <1> : NMAT | measurement exceeds the tolerance.
/*         <2> : INV  | no measurement result available.
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_CalcLimPowTranTolMatc (ViSession        InstrSession, 
	                                           ViInt32 _VI_FAR *VerdictForNTP)
{
	ViByte  	wrtBuf[BUFFER_SIZE]; 
	ViByte      rdBuf[BUFFER_SIZE];
	ViByte		Buf[256]; 
	ViByte      Result[256];
	ViUInt32	retCnt;
	ViStatus  	CMD60_status = VI_SUCCESS;

	Fmt (wrtBuf, 
	     "%s<CALC:LIM:POW:TRAN:TOL:MATC?;*STB?\n");
	
	if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
    							 &retCnt)) < 0)
		return CMD60_status;
        
	if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	Scan (rdBuf, "%s>%s[t59x]%s", Result, Buf);
	
	if ((CMD60_status = CMD60_Register (InstrSession, Buf)) < 0)
		return CMD60_status;
		
	if (CompareStrings ("MATC", 0, Result, 0, 1) == 0)
		*VerdictForNTP = 0;
	else if (CompareStrings ("NMAT", 0, Result, 0, 1) == 0)
		*VerdictForNTP = 1;
	else if (CompareStrings ("INV", 0, Result, 0, 1) == 0)
		*VerdictForNTP = 2;

	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_CalcLimPowTranMatc
/* Purpose :
/*     This function permits to query the verdict of the measurement of the
/*     power ramp versus time according the tolerances setting.
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     VerdictForPowerRamp
/*         The verdict of the measured power according the tolerances setting.
/*         Return :
/*         <0> : MATC | signal matches template.
/*         <1> : NMAT | signal does not match template.
/*         <2> : INV  | no measurement result available.
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_CalcLimPowTranMatc (ViSession        InstrSession, 
	                                        ViInt32 _VI_FAR *VerdictForPowerRamp)
{
	ViByte  	wrtBuf[BUFFER_SIZE]; 
	ViByte      rdBuf[BUFFER_SIZE];
	ViByte		Buf[256]; 
	ViByte      Result[256];
	ViUInt32	retCnt;
	ViStatus  	CMD60_status = VI_SUCCESS;

	Fmt (wrtBuf, 
	     "%s<CALC:LIM:POW:TRAN:MATC?;*STB?\n");
	
	if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
    							 &retCnt)) < 0)
		return CMD60_status;
        
	if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	Scan (rdBuf, "%s>%s[t59x]%s", Result, Buf);
	
	if ((CMD60_status = CMD60_Register (InstrSession, Buf)) < 0)
		return CMD60_status;
		
	if (CompareStrings ("MATC", 0, Result, 0, 1) == 0)
		*VerdictForPowerRamp = 0;
	else if (CompareStrings ("NMAT", 0, Result, 0, 1) == 0)
		*VerdictForPowerRamp = 1;
	else if (CompareStrings ("INV", 0, Result, 0, 1) == 0)
		*VerdictForPowerRamp = 2;

	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_ReadNtp
/* Purpose :
/*     This function permits to measure the average power of the burst.
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     NTPValue
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_ReadNtp (ViSession         InstrSession, 
	                             ViReal64 _VI_FAR *NTPValue)
{
	ViByte  	wrtBuf[256];
	ViByte      rdBuf[BUFFER_SIZE];
	ViByte		Buf[256];
	ViByte      Result[BUFFER_SIZE];
	ViUInt32	retCnt;
	ViStatus  	CMD60_status = VI_SUCCESS;
    
	Fmt (wrtBuf, 
	     "READ:NTP?;*STB?\n");
	
	if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
    							 &retCnt)) < 0)
		return CMD60_status;
		
	if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	Scan (rdBuf, "%s>%s[t59x]%s", Result, Buf);
	
	if ((CMD60_status = CMD60_Register (InstrSession, Buf)) < 0)
		return CMD60_status;
		
	if (Scan (Result, "%s>%f", NTPValue) == 0)
		return VI_ERROR_INV_MEAS;

	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_ReadArrPowTran
/* Purpose :
/*     This function permits to ...
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     ArrPow
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_ReadArrPowTran (ViSession         InstrSession,
                                        ViReal64 _VI_FAR  ArrPow[])
{
	ViByte  	wrtBuf[256];
	ViByte      rdBuf[18000];
	ViByte		Buf[256];
	ViByte      Result[18000];
	ViUInt32	retCnt;
	ViStatus  	CMD60_status = VI_SUCCESS;

	Fmt (wrtBuf, 
	     "%s<READ:ARR:POW:TRAN?;*STB?\n");
	
	if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
    							 &retCnt)) < 0)
		return CMD60_status;
		
	if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								18000, 
								&retCnt)) < 0)
		return CMD60_status;
	
	Scan (rdBuf, "%s>%s[xt59]%s", Result, Buf);
	
	if ((CMD60_status = CMD60_Register (InstrSession, Buf)) < 0)
		return CMD60_status;

	Scan (Result, "%s>%3000f[x]", ArrPow);

	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_CalcLimFreqOffsTolMatc
/* Purpose :
/*     This function permits to ...
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     VerdictForFrequencyOffset
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_CalcLimFreqOffsTolMatc (ViSession        InstrSession,
                                                ViInt32 _VI_FAR *VerdictForFrequencyOffset)                               
                               
{
	ViByte  	wrtBuf[BUFFER_SIZE]; 
	ViByte      rdBuf[BUFFER_SIZE];
	ViByte		Buf[256]; 
	ViByte      Result[256];
	ViUInt32	retCnt;
	ViStatus  	CMD60_status = VI_SUCCESS;

	Fmt (wrtBuf, 
	     "%s<CALC:LIM:FREQ:OFFS:TOL:MATC?;*STB?\n");
	
	if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
    							 &retCnt)) < 0)
		return CMD60_status;
        
	if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	Scan (rdBuf, "%s>%s[t59x]%s", Result, Buf);
	
	if ((CMD60_status = CMD60_Register (InstrSession, Buf)) < 0)
		return CMD60_status;
		
	if (CompareStrings ("MATC", 0, Result, 0, 1) == 0)
		*VerdictForFrequencyOffset = 0;
	else if (CompareStrings ("NMAT", 0, Result, 0, 1) == 0)
		*VerdictForFrequencyOffset = 1;
	else if (CompareStrings ("INV", 0, Result, 0, 1) == 0)
		*VerdictForFrequencyOffset = 2;

	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_CalcLimBfiTolMatc
/* Purpose :
/*     This function permits to ...
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     VerdictForMaxPosBField
/*     VerdictForMaxNegBField
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_CalcLimBfiTolMatc (ViSession        InstrSession,
                                           ViInt32 _VI_FAR *VerdictForMaxPosBField,
                                           ViInt32 _VI_FAR *VerdictForMaxNegBField)
{
	ViByte  	wrtBuf[BUFFER_SIZE]; 
	ViByte      rdBuf[BUFFER_SIZE];
	ViByte		Buf[256]; 
	ViByte      Result[256];
	ViByte	 	Result1[255];
	ViByte	 	Result2[255];
	ViUInt32	retCnt;
	ViStatus  	CMD60_status = VI_SUCCESS;

	Fmt (wrtBuf, 
	     "%s<CALC:LIM:BFI:TOL:MATC?;*STB?\n");
	
	if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
    							 &retCnt)) < 0)
		return CMD60_status;
        
	if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	Scan (rdBuf, "%s>%s[xt59]%s", Result, Buf);
	
	if ((CMD60_status = CMD60_Register (InstrSession, Buf)) < 0)
		return CMD60_status;
		
	Scan (Result, "%s>%s[xt44]%s", Result1, Result2);
		
	if (CompareStrings ("MATC", 0, Result1, 0, 1) == 0)
		*VerdictForMaxPosBField = 0;
	else if (CompareStrings ("NMAT", 0, Result1, 0, 1) == 0)
		*VerdictForMaxPosBField = 1;
	else if (CompareStrings ("INV", 0, Result1, 0, 1) == 0)
		*VerdictForMaxPosBField = 2;
		
	if (CompareStrings ("MATC", 0, Result2, 0, 1) == 0)
		*VerdictForMaxNegBField = 0;
	else if (CompareStrings ("NMAT", 0, Result2, 0, 1) == 0)
		*VerdictForMaxNegBField = 1;
	else if (CompareStrings ("INV", 0, Result2, 0, 1) == 0)
		*VerdictForMaxNegBField = 2;

	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_CalcLimSfiTolMatc
/* Purpose :
/*     This function permits to ...
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     VerdictForMaxPosSyncField
/*     VerdictForMaxNegSyncField
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_CalcLimSfiTolMatc (ViSession        InstrSession,
                                           ViInt32 _VI_FAR *VerdictForMaxPosSyncField,
                                           ViInt32 _VI_FAR *VerdictForMaxNegSyncField)
{
	ViByte  	wrtBuf[BUFFER_SIZE]; 
	ViByte      rdBuf[BUFFER_SIZE];
	ViByte		Buf[256]; 
	ViByte      Result[256];
	ViByte	 	Result1[255];
	ViByte	 	Result2[255];
	ViUInt32	retCnt;
	ViStatus  	CMD60_status = VI_SUCCESS;

	Fmt (wrtBuf, 
	     "%s<CALC:LIM:SFI:TOL:MATC?;*STB?\n");
	
	if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
    							 &retCnt)) < 0)
		return CMD60_status;
        
	if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	Scan (rdBuf, "%s>%s[xt59]%s", Result, Buf);
	
	if ((CMD60_status = CMD60_Register (InstrSession, Buf)) < 0)
		return CMD60_status;
		
	Scan (Result, "%s>%s[xt44]%s", Result1, Result2);
		
	if (CompareStrings ("MATC", 0, Result1, 0, 1) == 0)
		*VerdictForMaxPosSyncField = 0;
	else if (CompareStrings ("NMAT", 0, Result1, 0, 1) == 0)
		*VerdictForMaxPosSyncField = 1;
	else if (CompareStrings ("INV", 0, Result1, 0, 1) == 0)
		*VerdictForMaxPosSyncField = 2;
		
	if (CompareStrings ("MATC", 0, Result2, 0, 1) == 0)
		*VerdictForMaxNegSyncField = 0;
	else if (CompareStrings ("NMAT", 0, Result2, 0, 1) == 0)
		*VerdictForMaxNegSyncField = 1;
	else if (CompareStrings ("INV", 0, Result2, 0, 1) == 0)
		*VerdictForMaxNegSyncField = 2;

	return CMD60_status;
}                                           

/*===========================================================================*/
/* Function :
/*     CMD60_CalcLimFreqDrifTolMatc
/* Purpose :
/*     This function permits to ...
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     VerdictForFrequencyDrift
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_CalcLimFreqDrifTolMatc (ViSession        InstrSession,
                                                ViInt32 _VI_FAR *VerdictForFrequencyDrift)
{
	ViByte  	wrtBuf[BUFFER_SIZE]; 
	ViByte      rdBuf[BUFFER_SIZE];
	ViByte		Buf[256]; 
	ViByte      Result[256];
	ViUInt32	retCnt;
	ViStatus  	CMD60_status = VI_SUCCESS;

	Fmt (wrtBuf, 
	     "%s<CALC:LIM:FREQ:DRIF:TOL:MATC?;*STB?\n");
	
	if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
    							 &retCnt)) < 0)
		return CMD60_status;
        
	if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	Scan (rdBuf, "%s>%s[t59x]%s", Result, Buf);
	
	if ((CMD60_status = CMD60_Register (InstrSession, Buf)) < 0)
		return CMD60_status;
		
	if (CompareStrings ("MATC", 0, Result, 0, 1) == 0)
		*VerdictForFrequencyDrift = 0;
	else if (CompareStrings ("NMAT", 0, Result, 0, 1) == 0)
		*VerdictForFrequencyDrift = 1;
	else if (CompareStrings ("INV", 0, Result, 0, 1) == 0)
		*VerdictForFrequencyDrift = 2;

	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_ReadFreqOffs
/* Purpose :
/*     This function permits to ...
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     FreqOffs
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_ReadFreqOffs (ViSession         InstrSession,
                                      ViReal64 _VI_FAR *FreqOffs)
{
	ViByte  	wrtBuf[256];
	ViByte      rdBuf[BUFFER_SIZE];
	ViByte		Buf[256];
	ViByte      Result[BUFFER_SIZE];
	ViUInt32	retCnt;
	ViStatus  	CMD60_status = VI_SUCCESS;
    
	Fmt (wrtBuf, 
	     "READ:FREQ:OFFS?;*STB?\n");
	
	if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
    							 &retCnt)) < 0)
		return CMD60_status;
		
	if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	Scan (rdBuf, "%s>%s[xt59]%s", Result, Buf);
	
	if ((CMD60_status = CMD60_Register (InstrSession, Buf)) < 0)
		return CMD60_status;
		
	if (Scan (Result, "%s>%f", FreqOffs) == 0)
		return VI_ERROR_INV_MEAS;

	return CMD60_status;
}
                             
/*===========================================================================*/
/* Function :
/*     CMD60_ReadBfi
/* Purpose :
/*     This function permits to ...
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     MaxPosBFieldValue
/*     MaxNegBFieldValue
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_ReadBfi (ViSession         InstrSession, 
                                 ViReal64 _VI_FAR *MaxPosBFieldValue,
                                 ViReal64 _VI_FAR *MaxNegBFieldValue)
{
	ViByte  	wrtBuf[256];
	ViByte      rdBuf[BUFFER_SIZE];
	ViByte		Buf[256];
	ViByte      Result[BUFFER_SIZE];
	ViByte	 	Result1[255];
	ViByte	 	Result2[255];
	ViUInt32	retCnt;
	ViStatus  	CMD60_status = VI_SUCCESS;
    
	Fmt (wrtBuf, 
	     "READ:BFI?;*STB?\n");
	
	if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
    							 &retCnt)) < 0)
		return CMD60_status;
		
	if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	Scan (rdBuf, "%s>%s[xt59]%s", Result, Buf);
	
	if ((CMD60_status = CMD60_Register (InstrSession, Buf)) < 0)
		return CMD60_status;
	
	Scan (Result, "%s>%s[xt44]%s", Result1, Result2);
		
	if (Scan (Result1, "%s>%f", MaxPosBFieldValue) == 0)
		return VI_ERROR_INV_MEAS;
	
	if (Scan (Result2, "%s>%f", MaxNegBFieldValue) == 0)
		return VI_ERROR_INV_MEAS;

	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_ReadSfi
/* Purpose :
/*     This function permits to ...
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     MaxPosSFieldValue
/*     MaxNegSFieldValue
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_ReadSfi (ViSession         InstrSession, 
                                 ViReal64 _VI_FAR *MaxPosSFieldValue,
                                 ViReal64 _VI_FAR *MaxNegSFieldValue)
{
	ViByte  	wrtBuf[256];
	ViByte      rdBuf[BUFFER_SIZE];
	ViByte		Buf[256];
	ViByte      Result[BUFFER_SIZE];
	ViByte	 	Result1[255];
	ViByte	 	Result2[255];
	ViUInt32	retCnt;
	ViStatus  	CMD60_status = VI_SUCCESS;
    
	Fmt (wrtBuf, 
	     "READ:SFI?;*STB?\n");
	
	if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
    							 &retCnt)) < 0)
		return CMD60_status;
		
	if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	Scan (rdBuf, "%s>%s[xt59]%s", Result, Buf);
	
	if ((CMD60_status = CMD60_Register (InstrSession, Buf)) < 0)
		return CMD60_status;
	
	Scan (Result, "%s>%s[xt44]%s", Result1, Result2);
		
	if (Scan (Result1, "%s>%f", MaxPosSFieldValue) == 0)
		return VI_ERROR_INV_MEAS;
	
	if (Scan (Result2, "%s>%f", MaxNegSFieldValue) == 0)
		return VI_ERROR_INV_MEAS;

	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_ReadFreqDrif
/* Purpose :
/*     This function permits to ...
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     FrequencyDriftValue
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_ReadFreqDrif (ViSession         InstrSession,
                                      ViReal64 _VI_FAR *FrequencyDriftValue)
{
	ViByte  	wrtBuf[256];
	ViByte      rdBuf[BUFFER_SIZE];
	ViByte		Buf[256];
	ViByte      Result[BUFFER_SIZE];
	ViUInt32	retCnt;
	ViStatus  	CMD60_status = VI_SUCCESS;
    
	Fmt (wrtBuf, 
	     "READ:FREQ:DRIF?;*STB?\n");
	
	if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
    							 &retCnt)) < 0)
		return CMD60_status;
		
	if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	Scan (rdBuf, "%s>%s[xt59]%s", Result, Buf);
	
	if ((CMD60_status = CMD60_Register (InstrSession, Buf)) < 0)
		return CMD60_status;
		
	if (Scan (Result, "%s>%f", FrequencyDriftValue) == 0)
		return VI_ERROR_INV_MEAS;

	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_ReadArrFreqDev
/* Purpose :
/*     This function permits to ...
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     ArrFreq
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_ReadArrFreqDev (ViSession         InstrSession,
                                        ViReal64 _VI_FAR  ArrFreq[])
{
	ViByte  	wrtBuf[256];
	ViByte      rdBuf[18000];
	ViByte		Buf[256];
	ViByte      Result[18000];
	ViUInt32	retCnt;
	ViStatus  	CMD60_status = VI_SUCCESS;

	Fmt (wrtBuf, 
	     "%s<READ:ARR:FREQ:DEV?;*STB?\n");
	
	if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
    							 &retCnt)) < 0)
		return CMD60_status;
		
	if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								18000, 
								&retCnt)) < 0)
		return CMD60_status;
		
	Scan (rdBuf, "%s>%s[xt59]%s", Result, Buf);
	
	if ((CMD60_status = CMD60_Register (InstrSession, Buf)) < 0)
		return CMD60_status;
		
	Scan (Result, "%s>%669f[x]", ArrFreq);

	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_CalcLimTimeAccTolMatc
/* Purpose :
/*     This function permits to ...
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     VerdictForTimeAccuracy
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_CalcLimTimeAccTolMatc (ViSession        InstrSession,
                                               ViInt32 _VI_FAR *VerdictForTimeAccuracy)
{
	ViByte  	wrtBuf[BUFFER_SIZE]; 
	ViByte      rdBuf[BUFFER_SIZE];
	ViByte		Buf[256]; 
	ViByte      Result[256];
	ViUInt32	retCnt;
	ViStatus  	CMD60_status = VI_SUCCESS;

	Fmt (wrtBuf, 
	     "%s<CALC:LIM:TIME:ACC:TOL:MATC?;*STB?\n");
	
	if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
    							 &retCnt)) < 0)
		return CMD60_status;
        
	if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	Scan (rdBuf, "%s>%s[t59x]%s", Result, Buf);
	
	if ((CMD60_status = CMD60_Register (InstrSession, Buf)) < 0)
		return CMD60_status;
		
	if (CompareStrings ("MATC", 0, Result, 0, 1) == 0)
		*VerdictForTimeAccuracy = 0;
	else if (CompareStrings ("NMAT", 0, Result, 0, 1) == 0)
		*VerdictForTimeAccuracy = 1;
	else if (CompareStrings ("INV", 0, Result, 0, 1) == 0)
		*VerdictForTimeAccuracy = 2;

	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_CalcLimJittTolMatc
/* Purpose :
/*     This function permits to ...
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     VerdictForMaxPosJitter
/*     VerdictForMaxNegJitter
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_CalcLimJittTolMatc (ViSession        InstrSession,
                                            ViInt32 _VI_FAR *VerdictForMaxPosJitter,
                                            ViInt32 _VI_FAR *VerdictForMaxNegJitter)
{
	ViByte  	wrtBuf[BUFFER_SIZE]; 
	ViByte      rdBuf[BUFFER_SIZE];
	ViByte		Buf[256]; 
	ViByte      Result[256];
	ViByte	 	Result1[255];
	ViByte	 	Result2[255];
	ViUInt32	retCnt;
	ViStatus  	CMD60_status = VI_SUCCESS;

	Fmt (wrtBuf, 
	     "%s<CALC:LIM:JITT:TOL:MATC?;*STB?\n");
	
	if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
    							 &retCnt)) < 0)
		return CMD60_status;
        
	if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	Scan (rdBuf, "%s>%s[xt59]%s", Result, Buf);
	
	if ((CMD60_status = CMD60_Register (InstrSession, Buf)) < 0)
		return CMD60_status;
		
	Scan (Result, "%s>%s[xt44]%s", Result1, Result2);
		
	if (CompareStrings ("MATC", 0, Result1, 0, 1) == 0)
		*VerdictForMaxPosJitter = 0;
	else if (CompareStrings ("NMAT", 0, Result1, 0, 1) == 0)
		*VerdictForMaxPosJitter = 1;
	else if (CompareStrings ("INV", 0, Result1, 0, 1) == 0)
		*VerdictForMaxPosJitter = 2;
		
	if (CompareStrings ("MATC", 0, Result2, 0, 1) == 0)
		*VerdictForMaxNegJitter = 0;
	else if (CompareStrings ("NMAT", 0, Result2, 0, 1) == 0)
		*VerdictForMaxNegJitter = 1;
	else if (CompareStrings ("INV", 0, Result2, 0, 1) == 0)
		*VerdictForMaxNegJitter = 2;

	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_CalcLimPackDelTolMatc
/* Purpose :
/*     This function permits to ...
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     VerdictForMinPacketDelay
/*     VerdictForMaxPacketDelay
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_CalcLimPackDelTolMatc (ViSession        InstrSession,
                                               ViInt32 _VI_FAR *VerdictForMinPacketDelay,
                                               ViInt32 _VI_FAR *VerdictForMaxPacketDelay)
{
	ViByte  	wrtBuf[BUFFER_SIZE]; 
	ViByte      rdBuf[BUFFER_SIZE];
	ViByte		Buf[256]; 
	ViByte      Result[256];
	ViByte	 	Result1[255];
	ViByte	 	Result2[255];
	ViUInt32	retCnt;
	ViStatus  	CMD60_status = VI_SUCCESS;

	Fmt (wrtBuf, 
	     "%s<CALC:LIM:PACK:DEL:TOL:MATC?;*STB?\n");
	
	if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
    							 &retCnt)) < 0)
		return CMD60_status;
        
	if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	Scan (rdBuf, "%s>%s[xt59]%s", Result, Buf);
	
	if ((CMD60_status = CMD60_Register (InstrSession, Buf)) < 0)
		return CMD60_status;
		
	Scan (Result, "%s>%s[xt44]%s", Result1, Result2);
		
	if (CompareStrings ("MATC", 0, Result1, 0, 1) == 0)
		*VerdictForMinPacketDelay = 0;
	else if (CompareStrings ("NMAT", 0, Result1, 0, 1) == 0)
		*VerdictForMinPacketDelay = 1;
	else if (CompareStrings ("INV", 0, Result1, 0, 1) == 0)
		*VerdictForMinPacketDelay = 2;
		
	if (CompareStrings ("MATC", 0, Result2, 0, 1) == 0)
		*VerdictForMaxPacketDelay = 0;
	else if (CompareStrings ("NMAT", 0, Result2, 0, 1) == 0)
		*VerdictForMaxPacketDelay = 1;
	else if (CompareStrings ("INV", 0, Result2, 0, 1) == 0)
		*VerdictForMaxPacketDelay = 2;

	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_ReadTimeAcc
/* Purpose :
/*     This function permits to read the time accuracy.
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     TimeAccuracyValue
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_ReadTimeAcc (ViSession         InstrSession, 
                                     ViReal64 _VI_FAR *TimeAccuracyValue)
{
	ViByte  	wrtBuf[256];
	ViByte      rdBuf[BUFFER_SIZE];
	ViByte		Buf[256];
	ViByte      Result[BUFFER_SIZE];
	ViUInt32	retCnt;
	ViStatus  	CMD60_status = VI_SUCCESS;
    
	Fmt (wrtBuf, 
	     "READ:TIME:ACC?;*STB?\n");
	
	if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
    							 &retCnt)) < 0)
		return CMD60_status;
		
	if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	Scan (rdBuf, "%s>%s[xt59]%s", Result, Buf);
	
	if ((CMD60_status = CMD60_Register (InstrSession, Buf)) < 0)
		return CMD60_status;
		
	if (Scan (Result, "%s>%f", TimeAccuracyValue) == 0)
		return VI_ERROR_INV_MEAS;

	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_ReadJitt
/* Purpose :
/*     This function permits to read the maximally and minimally time interval
/*     between two succesive bursts minus the average time period.
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     MaxPosJitterValue (maximally time interval)
/*     MaxNegJitterValue (minimally time interval)
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_ReadJitt (ViSession         InstrSession, 
                                  ViReal64 _VI_FAR *MaxPosJitterValue,
                                  ViReal64 _VI_FAR *MaxNegJitterValue)
{
	ViByte  	wrtBuf[256];
	ViByte      rdBuf[BUFFER_SIZE];
	ViByte		Buf[256];
	ViByte      Result[BUFFER_SIZE];
	ViByte	 	Result1[255];
	ViByte	 	Result2[255];
	ViUInt32	retCnt;
	ViStatus  	CMD60_status = VI_SUCCESS;
    
	Fmt (wrtBuf, 
	     "READ:JITT?;*STB?\n");
	
	if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
    							 &retCnt)) < 0)
		return CMD60_status;
		
	if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	Scan (rdBuf, "%s>%s[xt59]%s", Result, Buf);
	
	if ((CMD60_status = CMD60_Register (InstrSession, Buf)) < 0)
		return CMD60_status;
	
	Scan (Result, "%s>%s[xt44]%s", Result1, Result2);
		
	if (Scan (Result1, "%s>%f", MaxPosJitterValue) == 0)
		return VI_ERROR_INV_MEAS;
	
	if (Scan (Result2, "%s>%f", MaxNegJitterValue) == 0)
		return VI_ERROR_INV_MEAS;

	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_ReadPackDel
/* Purpose :
/*     This function permits to read the maximally and minimally time interval
/*     between two bursts of the CMD60 as FP and bursts of the PP under test
/*     minus 5ms.
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     MaxPacketDelayValue (maximally time interval)
/*     MinPacketDelayValue (minimally time interval)
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_ReadPackDel (ViSession         InstrSession,
                                     ViReal64 _VI_FAR *MinPacketDelayValue,
                                     ViReal64 _VI_FAR *MaxPacketDelayValue)
{
	ViByte  	wrtBuf[256];
	ViByte      rdBuf[BUFFER_SIZE];
	ViByte		Buf[256];
	ViByte      Result[BUFFER_SIZE];
	ViByte	 	Result1[255];
	ViByte	 	Result2[255];
	ViUInt32	retCnt;
	ViStatus  	CMD60_status = VI_SUCCESS;
    
	Fmt (wrtBuf, 
	     "READ:PACK:DEL?;*STB?\n");
	
	if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
    							 &retCnt)) < 0)
		return CMD60_status;
		
	if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	Scan (rdBuf, "%s>%s[xt59]%s", Result, Buf);
	
	if ((CMD60_status = CMD60_Register (InstrSession, Buf)) < 0)
		return CMD60_status;
	
	Scan (Result, "%s>%s[xt44]%s", Result1, Result2);
		
	if (Scan (Result1, "%s>%f", MaxPacketDelayValue) == 0)
		return VI_ERROR_INV_MEAS;
	
	if (Scan (Result2, "%s>%f", MinPacketDelayValue) == 0)
		return VI_ERROR_INV_MEAS;

	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_CalcLimBerLtermTolMatc
/* Purpose :
/*     This function permits to read the results of the tolerance query for the
/*     "Longterm BER" and the "Longterm FER".
/*     The "Longterm BER" and the "Longterm FER" are averaged over the time win
/*     -dow (Evaluation Window) to be set in the function "ConfBerEvalWind ()".
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     VerdictForBER 
/*         Verdict of the tolerance query for the "Lterm BER".
/* 	   VerdictForFER 
/*         Verdict of the tolerance query for the "Lterm FER".
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_CalcLimBerLtermTolMatc (ViSession        InstrSession,
                                                ViInt32 _VI_FAR *VerdictForBER,
                                                ViInt32 _VI_FAR *VerdictForFER)
{
	ViByte  	wrtBuf[BUFFER_SIZE]; 
	ViByte      rdBuf[BUFFER_SIZE];
	ViByte		Buf[256]; 
	ViByte      Result[256];
	ViByte	 	Result1[255];
	ViByte	 	Result2[255];
	ViUInt32	retCnt;
	ViStatus  	CMD60_status = VI_SUCCESS;

	Fmt (wrtBuf, 
	     "%s<CALC:LIM:BER:LTERM:TOL:MATC?;*STB?\n");
	
	if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
    							 &retCnt)) < 0)
		return CMD60_status;
        
	if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	Scan (rdBuf, "%s>%s[xt59]%s", Result, Buf);
	
	if ((CMD60_status = CMD60_Register (InstrSession, Buf)) < 0)
		return CMD60_status;
		
	Scan (Result, "%s>%s[xt44]%s", Result1, Result2);
		
	if (CompareStrings ("MATC", 0, Result1, 0, 1) == 0)
		*VerdictForBER = 0;
	else if (CompareStrings ("NMAT", 0, Result1, 0, 1) == 0)
		*VerdictForBER = 1;
	else if (CompareStrings ("INV", 0, Result1, 0, 1) == 0)
		*VerdictForBER = 2;
		
	if (CompareStrings ("MATC", 0, Result2, 0, 1) == 0)
		*VerdictForFER = 0;
	else if (CompareStrings ("NMAT", 0, Result2, 0, 1) == 0)
		*VerdictForFER = 1;
	else if (CompareStrings ("INV", 0, Result2, 0, 1) == 0)
		*VerdictForFER = 2;

	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_ReadBerLter
/* Purpose :
/*     This function permits to read the results of the "Longterm BER" and the
/*     "Longterm FER" measurement.
/*     The "Longterm BER" and the "Longterm FER" are averaged over the time win
/*     -dow (Evaluation Window) to be set in the function "ConfBerEvalWind ()".
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     BERValue 
/*         Longterm BER value.
/*	   FERValue
/*         Longterm FER value.
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_ReadBerLter (ViSession         InstrSession, 
                                     ViReal64 _VI_FAR *BERValue,
                                     ViReal64 _VI_FAR *FERValue)
{
	ViByte  	wrtBuf[256];
	ViByte      rdBuf[BUFFER_SIZE];
	ViByte		Buf[256];
	ViByte      Result[BUFFER_SIZE];
	ViByte	 	Result1[255];
	ViByte	 	Result2[255];
	ViUInt32	retCnt;
	ViStatus  	CMD60_status = VI_SUCCESS;
    
	Fmt (wrtBuf, 
	     "READ:BER:LTER?;*STB?\n");
	
	if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
    							 &retCnt)) < 0)
		return CMD60_status;
		
	if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	Scan (rdBuf, "%s>%s[xt59]%s", Result, Buf);
	
	if ((CMD60_status = CMD60_Register (InstrSession, Buf)) < 0)
		return CMD60_status;
	
	Scan (Result, "%s>%s[xt44]%s", Result1, Result2);
	
	if (Scan (Result1, "%s>%f", BERValue) == 0)
		return VI_ERROR_INV_MEAS;
	
	if (Scan (Result2, "%s>%f", FERValue) == 0)
		return VI_ERROR_INV_MEAS;

	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_ReadArrBerMbe
/* Purpose :
/*     This function permits to read the slots involved in the bit error rate 
/*     measurement.
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     MultiBearerSlots (array of slots)
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_ReadArrBerMbe (ViSession         InstrSession,
                                       ViReal64 _VI_FAR  MultiBearerSlots[])
{
	ViByte  	wrtBuf[256];
	ViByte      rdBuf[BUFFER_SIZE];
	ViByte		Buf[256];
	ViByte      Result[BUFFER_SIZE];
	ViUInt32	retCnt;
	ViStatus  	CMD60_status = VI_SUCCESS;

	Fmt (wrtBuf, 
	     "%s<READ:ARR:BER:MBE?;*STB?\n");
	
	if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
    							 &retCnt)) < 0)
		return CMD60_status;
		
	if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	Scan (rdBuf, "%s>%s[xt59]%s", Result, Buf);
	
	if ((CMD60_status = CMD60_Register (InstrSession, Buf)) < 0)
		return CMD60_status;
		
	Scan (Result, "%s>%669f[x]", MultiBearerSlots);

	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_CalcBanLimPowTranTolMatc
/* Purpose :
/*     This function permits to read the results of the tolerance query for the
/*     average power (NTP) over the time window (Measuring Window) to be set in
/*     the function "ConfBanMeasWind ()".
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     VerdictForNTP
/*         Verdict of the tolerance query for the NTP.
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_CalcBanLimPowTranTolMatc (ViSession        InstrSession,
                                                  ViInt32 _VI_FAR *VerdictForNTP)
{
	ViByte  	wrtBuf[BUFFER_SIZE]; 
	ViByte      rdBuf[BUFFER_SIZE];
	ViByte		Buf[256]; 
	ViByte      Result[256];
	ViUInt32	retCnt;
	ViStatus  	CMD60_status = VI_SUCCESS;

	Fmt (wrtBuf, 
	     "%s<CALC:BAN:LIM:POW:TRAN:TOL:MATC?;*STB?\n");
	
	if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
    							 &retCnt)) < 0)
		return CMD60_status;
        
	if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	Scan (rdBuf, "%s>%s[t59x]%s", Result, Buf);
	
	if ((CMD60_status = CMD60_Register (InstrSession, Buf)) < 0)
		return CMD60_status;
		
	if (CompareStrings ("MATC", 0, Result, 0, 1) == 0)
		*VerdictForNTP = 0;
	else if (CompareStrings ("NMAT", 0, Result, 0, 1) == 0)
		*VerdictForNTP = 1;
	else if (CompareStrings ("INV", 0, Result, 0, 1) == 0)
		*VerdictForNTP = 2;

	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_ReadArrBanFreqDev
/* Purpose :
/*     This function permits to read the frequencies in kHz of the demodulated
/*     signal versus time.
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     FrequencyValues (array of frequency)
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_ReadArrBanFreqDev (ViSession         InstrSession,
                                           ViReal64 _VI_FAR  FrequencyValues[])
{
	ViByte  	wrtBuf[256];
	ViByte      rdBuf[BUFFER_SIZE];
	ViByte		Buf[256];
	ViByte      Result[BUFFER_SIZE];
	ViUInt32	retCnt;
	ViStatus  	CMD60_status = VI_SUCCESS;

	Fmt (wrtBuf, 
	     "%s<READ:ARR:BAN:FREQ:DEV?;*STB?\n");
	
	if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
    							 &retCnt)) < 0)
		return CMD60_status;
		
	if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	Scan (rdBuf, "%s>%s[xt59]%s", Result, Buf);
	
	if ((CMD60_status = CMD60_Register (InstrSession, Buf)) < 0)
		return CMD60_status;
		
	Scan (Result, "%s>%669f[x]", FrequencyValues);

	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_ReadBanPowTranNorm
/* Purpose :
/*     This function permits to read the average power in dBm over the time win
/*     -dow (Measuring Window) to be set in the function "ConfBanMeasWind ()".
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     NTPValue 
/*         Normal Transmit Power value.
/*         Return :
/*         <value> (Unit: dBm)
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_ReadBanPowTranNorm (ViSession         InstrSession, 
                                            ViReal64 _VI_FAR *NTPValue)
{
	ViByte  	wrtBuf[256];
	ViByte      rdBuf[BUFFER_SIZE];
	ViByte		Buf[256];
	ViByte      Result[BUFFER_SIZE];
	ViUInt32	retCnt;
	ViStatus  	CMD60_status = VI_SUCCESS;
    
	Fmt (wrtBuf, 
	     "READ:BAN:POW:TRAN:NORM?;*STB?\n");
	
	if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
    							 &retCnt)) < 0)
		return CMD60_status;
		
	if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	Scan (rdBuf, "%s>%s[xt59]%s", Result, Buf);
	
	if ((CMD60_status = CMD60_Register (InstrSession, Buf)) < 0)
		return CMD60_status;
		
	if (Scan (Result, "%s>%f", NTPValue) == 0)
		return VI_ERROR_INV_MEAS;

	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_ReadBanFreqOffs
/* Purpose :
/*     This function permits to read the average value in kHz of the demodula-
/*     -ted signal over the time window (Measuring Window) to be set in the 
/*     function "ConfBanMeasWind ()".
/*     If the number of modulated "0" and "1" is the same, the measured value
/*     corresponds to the frequency offset.
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     FrequencyOffsetValue 
/*         The measured frequency offset (average value).
/*         Return :
/*         <value> (Unit: kHz)
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_ReadBanFreqOffs (ViSession         InstrSession,
                                         ViReal64 _VI_FAR *FrequencyOffsetValue)
{
	ViByte  	wrtBuf[256];
	ViByte      rdBuf[BUFFER_SIZE];
	ViByte		Buf[256];
	ViByte      Result[BUFFER_SIZE];
	ViUInt32	retCnt;
	ViStatus  	CMD60_status = VI_SUCCESS;
    
	Fmt (wrtBuf, 
	     "READ:BAN:FREQ:OFFS?;*STB?\n");
	
	if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
    							 &retCnt)) < 0)
		return CMD60_status;
		
	if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	Scan (rdBuf, "%s>%s[xt59]%s", Result, Buf);
	
	if ((CMD60_status = CMD60_Register (InstrSession, Buf)) < 0)
		return CMD60_status;
		
	if (Scan (Result, "%s>%f", FrequencyOffsetValue) == 0)
		return VI_ERROR_INV_MEAS;

	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_ReadBanMod
/* Purpose :
/*     This function permits to read the positive and negative peak values of
/*     the demodulated signal compared with the frequency offset.
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     MaxPosModulationValue (positive peak value)
/*	   MaxNegModulationValue (negative peak value)
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_ReadBanMod (ViSession         InstrSession,
                                    ViReal64 _VI_FAR *MaxPosModulationValue,
                                    ViReal64 _VI_FAR *MaxNegModulationValue)
{
	ViByte  	wrtBuf[256];
	ViByte      rdBuf[BUFFER_SIZE];
	ViByte		Buf[256];
	ViByte      Result[BUFFER_SIZE];
	ViByte	 	Result1[255];
	ViByte	 	Result2[255];
	ViUInt32	retCnt;
	ViStatus  	CMD60_status = VI_SUCCESS;
    
	Fmt (wrtBuf, 
	     "READ:BAN:MOD?;*STB?\n");
	
	if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
    							 &retCnt)) < 0)
		return CMD60_status;
		
	if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								BUFFER_SIZE, 
								&retCnt)) < 0)
		return CMD60_status;
		
	Scan (rdBuf, "%s>%s[xt59]%s", Result, Buf);
	
	if ((CMD60_status = CMD60_Register (InstrSession, Buf)) < 0)
		return CMD60_status;
	
	Scan (Result, "%s>%s[xt44]%s", Result1, Result2);
		
	if (Scan (Result1, "%s>%f", MaxPosModulationValue) == 0)
		return VI_ERROR_INV_MEAS;
	
	if (Scan (Result2, "%s>%f", MaxNegModulationValue) == 0)
		return VI_ERROR_INV_MEAS;

	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_ReadArrBanPowTran
/* Purpose :
/*     Read the power values in dBm of the power ramp versus time.
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     ArrPow
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_ReadArrBanPowTran (ViSession         InstrSession,
                                           ViReal64 _VI_FAR  ArrPow[])
                        
{
	ViByte  	wrtBuf[256];
	ViByte      rdBuf[18000];
	ViByte		Buf[256];
	ViByte      Result[18000];
	ViUInt32	retCnt;
	ViStatus  	CMD60_status = VI_SUCCESS;

	Fmt (wrtBuf, 
	     "%s<READ:ARR:BAN:POW:TRAN?;*STB?\n");
	
	if ((CMD60_status = viWrite (InstrSession, 
                                 wrtBuf, 
                                 NumFmtdBytes (), 
    							 &retCnt)) < 0)
		return CMD60_status;
		
	if ((CMD60_status = viRead (InstrSession, 
	                            rdBuf, 
								18000, 
								&retCnt)) < 0)
		return CMD60_status;
		
	Scan (rdBuf, "%s>%s[xt59]%s", Result, Buf);
	
	if ((CMD60_status = CMD60_Register (InstrSession, Buf)) < 0)
		return CMD60_status;
		
	Scan (Result, "%s>%669f[x]", ArrPow);

	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_WriteInstrData
/* Purpose :
/*     This function writes a command string to the instrument.
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     writeBuffer
/*         Buffer to write any valid command to the instrument.
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_WriteInstrData (ViSession  InstrSession, 
                                        ViString   writeBuffer)
{
	ViStatus CMD60_status = VI_SUCCESS;
    
	if ((CMD60_status = viPrintf (InstrSession, "%s", writeBuffer)) < 0)
		return CMD60_status;

	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_ReadInstrData
/* Purpose :
/*     This function reads the output buffer of the instrument.
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     numBytes
/*         The number of bytes to read.
/*     rdBuf
/*         The incoming data from the instrument.
/*         Return :
/*         <String> (ASCII format).
/*	   bytesRead
/*         Number of bytes transferred from the instrument.
/*         Return :
/*         <Value> (Unit: None).
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_ReadInstrData (ViSession       InstrSession, 
                                       ViInt16         numBytes,
                                       ViChar _VI_FAR  rdBuf[], 
                                       ViPInt32        bytesRead)
{
	ViStatus CMD60_status = VI_SUCCESS;
	*bytesRead = 0L;
        
	if ((CMD60_status = viRead (InstrSession, rdBuf, numBytes, bytesRead)) < 0)
		return CMD60_status;

	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_Reset
/* Purpose :
/*     This function resets the instrument.
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_Reset (ViSession  InstrSession)
{
	ViUInt32  retCnt;
	ViStatus  CMD60_status = VI_SUCCESS;
	ViByte      rdBuffer[BUFFER_SIZE];
	int			PosIDN;


    CMD60_status = viWrite (InstrSession, 
	                        "*CLS;*RST;STAT:PRES\n", 
	                        20, 
		                    &retCnt);
		
	CMD60_status = viWrite (InstrSession, "*IDN?\n", 6, &retCnt);
            
	CMD60_status = viRead (InstrSession, rdBuffer, BUFFER_SIZE, &retCnt);
		
	PosIDN = FindPattern (rdBuffer, 0, -1, "ROHDE&SCHWARZ,CMD55M", 1, 0);

    if(PosIDN == 0)
		   CMD60_status = viWrite (InstrSession, 
	                        "CONF:NETW DECT\n", 
	                        15, 
		                    &retCnt);
		
		
    return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_SelfTest
/* Purpose :
/*     This function executes the instrument self test and returns the result.
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     Result
/*         Self-test result.
/*         Return :
/*         <value 0 to 255> (No error return 0).
/* Return Value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_SelfTest (ViSession       InstrSession,
                                  ViPInt16        Result)
{
	ViUInt32  retCnt;
	ViStatus  CMD60_status = VI_SUCCESS;

	if ((CMD60_status = viWrite (InstrSession, 
	                             "*TST?\n", 
	                             6, 
	                             &retCnt)) < 0)
        return CMD60_status;

    if ((CMD60_status = viScanf (InstrSession, 
                                 "%d", 
                                 Result)) < 0)
        return CMD60_status;

    return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_ErrorQuery
/* Purpose :
/*     This function queries the instrument error queue.
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/*     ErrorCode 
/*         ...
/*     ErrorMessage
/*         ...
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_ErrorQuery (ViSession       InstrSession, 
                                    ViInt32        *ErrorCode,
                                    ViChar _VI_FAR  ErrorMessage[])
{
	ViUInt32  retCnt = 0;
	ViStatus  CMD60_status = VI_SUCCESS;

	if ((CMD60_status = viWrite (InstrSession, 
	                             ":SYST:ERR?\n", 
	                             11, 
	                             &retCnt)) < 0)
		return CMD60_status;

	if ((CMD60_status = viScanf (InstrSession, 
	                             "%d,\"%[^\"]", 
	                             ErrorCode, 
	                             ErrorMessage)) < 0)
		return CMD60_status;

	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_ErrorMessage
/* Purpose :
/*     This function translates the error return value from the instrument
/*     driver into a user-readable string.
/* Parameter :
/*     InstrSession
/*         Session of the instrument driver.
/*     ErrorCode 
/*         ...
/*     ErrorMessage
/*         ...
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_ErrorMessage (ViSession       InstrSession, 
                                      ViStatus        ErrorCode,
                                      ViChar _VI_FAR  ErrorMessage[])
{
	ViStatus  CMD60_status = VI_SUCCESS;
	ViInt16   i;
	static CMD60_tStringValPair statusDescArray[] = {
        {VI_ERROR_PARAMETER1,         "ERROR: Parameter 1 out of range"},
        {VI_ERROR_PARAMETER2,         "ERROR: Parameter 2 out of range"},
        {VI_ERROR_PARAMETER3,         "ERROR: Parameter 3 out of range"},
        {VI_ERROR_PARAMETER4,         "ERROR: Parameter 4 out of range"},
        {VI_ERROR_PARAMETER5,         "ERROR: Parameter 5 out of range"},
        {VI_ERROR_PARAMETER6,         "ERROR: Parameter 6 out of range"},
        {VI_ERROR_PARAMETER7,         "ERROR: Parameter 7 out of range"},
        {VI_ERROR_PARAMETER8,         "ERROR: Parameter 8 out of range"},
        {VI_ERROR_FAIL_ID_QUERY,      "ERROR: Identification query failed"},
        {VI_ERROR_REGISTER_STB,	      "ERROR: Register STB <> 0"},       
		{VI_ERROR_INV_MEAS,		      "ERROR: Invalid measurement"},
		
		{CMD60_ERROR_STRING_LENGTH,   "ERROR: String length out of range"},
		
        {VI_NULL, VI_NULL}
    };

    CMD60_status = viStatusDesc (InstrSession, ErrorCode, ErrorMessage);
    
    if (CMD60_status == VI_WARN_UNKNOWN_STATUS) 
    {
		for (i=0; statusDescArray[i].stringName; i++) 
		{
            if (statusDescArray[i].stringVal == ErrorCode) 
            {
                strcpy (ErrorMessage, statusDescArray[i].stringName);
                return (VI_SUCCESS);
            }
        }
        sprintf (ErrorMessage, "Unknown Error 0x%08lX", ErrorCode);
    }
    
    return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_RevisionQuery
/* Purpose :
/*     This function returns the driver and instrument revisions.
/* Parameters :
/*     InstrSession
/*Session of the instrument driver.
/*     DriverRev
/*          Revision of the instrument driver.
/*     InstrRev
/*          Revision of the instrument.
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_RevisionQuery (ViSession       InstrSession,
                                       ViChar _VI_FAR  DriverRev[], 
                                       ViChar _VI_FAR  InstrRev[])
{
	ViUInt32  retCnt = 0;
	ViStatus  CMD60_status = VI_SUCCESS;

	if ((CMD60_status = viWrite (InstrSession, 
	                             "*IDN?\n", 
	                             6, 
	                             &retCnt)) < 0)
		return CMD60_status;

	if ((CMD60_status = viScanf (InstrSession, 
	                             "%*[^,],%*[^,],%*[^,],%[^\n]", 
	                             InstrRev)) < 0)
		return CMD60_status;

	strcpy (DriverRev, CMD60_REVISION);
    
	return CMD60_status;
}

/*===========================================================================*/
/* Function :
/*     CMD60_Close
/* Purpose :
/*     This function closes the instrument.
/* Parameters :
/*     InstrSession
/*         Session of the instrument driver.
/* Return value :
/*     Status of the operation.
/*===========================================================================*/
ViStatus _VI_FUNC CMD60_Close (ViSession  InstrSession)
{
    CMD60_instrRange  instrPtr;
    ViSession         rmSession;
    ViUInt32          retCnt = 0;
    ViStatus          CMD60_status = VI_SUCCESS;

    if ((CMD60_status = viGetAttribute (InstrSession, 
                                        VI_ATTR_RM_SESSION, 
                                        &rmSession)) < 0)
        return CMD60_status;
        
    if ((CMD60_status = viGetAttribute (InstrSession, 
                                        VI_ATTR_USER_DATA, 
                                        &instrPtr)) < 0)
        return CMD60_status;
    
    if ((CMD60_status = viWrite (InstrSession, 
                                 "PROC:SEL NONE\n", 
                                 14, 
                                 &retCnt)) < 0)
        return CMD60_status;

    free (instrPtr);
    
    CMD60_status = viClose (InstrSession);
    
    viClose (rmSession);

    return CMD60_status;
}

/*****************************************************************************/
/*= UTILITY ROUTINES (Non-Exportable Functions) =============================*/
/*****************************************************************************/

/*===========================================================================*/
/* Function: Boolean Value Out Of Range - ViBoolean                          */
/* Purpose:  This function checks a Boolean to see if it is equal to VI_TRUE */
/*           or VI_FALSE. If the value is out of range, the return value is  */
/*           VI_TRUE, otherwise the return value is VI_FALSE.                */
/*===========================================================================*/
ViBoolean CMD60_invalidViBooleanRange (ViBoolean val)
{
    return ((val != VI_FALSE && val != VI_TRUE) ? VI_TRUE : VI_FALSE);
}

/*===========================================================================*/
/* Function: Short Signed Integer Value Out Of Range - ViInt16               */
/* Purpose:  This function checks a short signed integer value to see if it  */  
/*           lies between a minimum and maximum value.  If the value is out  */
/*           of range, the return value is VI_TRUE, otherwise the return     */
/*           value is VI_FALSE.                                              */
/*===========================================================================*/
ViBoolean CMD60_invalidViInt16Range (ViInt16 val, ViInt16 min, ViInt16 max)
{
    return ((val < min || val > max) ? VI_TRUE : VI_FALSE);
}

/*===========================================================================*/
/* Function: Long Signed Integer Value Out Of Range - ViInt32                */
/* Purpose:  This function checks a long signed integer value to see if it   */  
/*           lies between a minimum and maximum value.  If the value is out  */
/*           of range,  the return value is VI_TRUE, otherwise the return    */
/*           value is VI_FALSE.                                              */
/*===========================================================================*/
ViBoolean CMD60_invalidViInt32Range  (ViInt32 val, ViInt32 min, ViInt32 max)
{
    return ((val < min || val > max) ? VI_TRUE : VI_FALSE);
}

/*===========================================================================*/
/* Function: Short Unsigned Integer Value Out Of Range - ViUInt16            */
/* Purpose:  This function checks a short unsigned integer value to see if it*/  
/*           lies between a minimum and maximum value.  If the value is out  */
/*           of range,  the return value is VI_TRUE, otherwise the return    */
/*           value is VI_FALSE.                                              */
/*===========================================================================*/
ViBoolean CMD60_invalidViUInt16Range  (ViUInt16 val, ViUInt16 min, ViUInt16 max)
{
    return ((val < min || val > max) ? VI_TRUE : VI_FALSE);
}

/*===========================================================================*/
/* Function: Long Unsigned Integer Value Out Of Range - ViUInt32             */
/* Purpose:  This function checks a long unsigned integer value to see if it */  
/*           lies between a minimum and maximum value.  If the value is out  */
/*           of range,  the return value is VI_TRUE, otherwise the return    */
/*           value is VI_FALSE.                                              */
/*===========================================================================*/
ViBoolean CMD60_invalidViUInt32Range  (ViUInt32 val, ViUInt32 min, ViUInt32 max)
{
    return ((val < min || val > max) ? VI_TRUE : VI_FALSE);
}

/*===========================================================================*/
/* Function: Real (Float) Value Out Of Range - ViReal32                      */
/* Purpose:  This function checks a real (float) value to see if it lies     */  
/*           between a minimum and maximum value.  If the value is out of    */
/*           range, the return value is VI_TRUE, otherwise the return value  */
/*           is VI_FALSE.                                                    */
/*===========================================================================*/
ViBoolean CMD60_invalidViReal32Range  (ViReal32 val, ViReal32 min, ViReal32 max)
{
    return ((val < min || val > max) ? VI_TRUE : VI_FALSE);
}

/*===========================================================================*/
/* Function: Real (Double) Value Out Of Range - ViReal64                     */
/* Purpose:  This function checks a real (double) value to see if it lies    */  
/*           between a minimum and maximum value.  If the value is out of    */
/*           range, the return value is VI_TRUE, otherwise the return value  */
/*           is VI_FALSE.                                                    */
/*===========================================================================*/
ViBoolean CMD60_invalidViReal64Range  (ViReal64 val, ViReal64 min, ViReal64 max)
{
    return ((val < min || val > max) ? VI_TRUE : VI_FALSE);
}

/*===========================================================================*/
/* Function : 
/*      CMD60_initCleanUp                                            
/* Purpose :
/*      This function is used only by the CMD60_Initialize function. When an
/*      error is detected this function is called to close the open resource
/*      manager and instrument object sessions and to set the InstrSession
/*      that is returned from CMD60_Initialize to VI_NULL.
/* Parameters :
/*      openRMSession
/*      openInstrSession
/*		currentStatus
/* Return value :
/*      Status of the operation.
/*===========================================================================*/
ViStatus CMD60_initCleanUp (ViSession   openRMSession,
                            ViPSession  openInstrSession, 
                            ViStatus    currentStatus)
{
    viClose (*openInstrSession);
    viClose (openRMSession);
    *openInstrSession = VI_NULL;
    
    return currentStatus;
}
