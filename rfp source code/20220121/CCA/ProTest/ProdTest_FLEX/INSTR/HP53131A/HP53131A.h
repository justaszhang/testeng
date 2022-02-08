//==============================================================================
//
// Title:		HP53131A.h
// Purpose:		A short description of the interface.
//
// Created on:	8/2/2021 at 2:04:09 PM by Mitel-Justas.
// Copyright:	. All Rights Reserved.
//
//==============================================================================

#ifndef __HP53131A_H__
#define __HP53131A_H__

#ifdef __cplusplus
    extern "C" {
#endif

//==============================================================================
// Include files

#include "cvidef.h"
#include <visa.h>     

//==============================================================================
// Constants

//==============================================================================
// Types

//==============================================================================
// External variables

//==============================================================================
// Global functions



int Init_Counter () ;

int TRIGGER_HP53131A_TO_MEASURE(double * freq) ;
int TRIGGER_HP53131A_TO_MEASURE_addon (double *freq);

#ifdef __cplusplus
    }
#endif

#endif  /* ndef __HP53131A_H__ */
