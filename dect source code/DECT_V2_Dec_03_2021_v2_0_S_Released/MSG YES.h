/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/* Copyright (c) National Instruments 2007. All Rights Reserved.          */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  PANEL                           1
#define  PANEL_YES_TEXTBOX               2
#define  PANEL_TOP_STRING                3
#define  PANEL_PB_MSG_NO                 4       /* callback function: Prompt_NO */
#define  PANEL_PB_MSG_YES                5       /* callback function: Prompt_YES */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */ 

int  CVICALLBACK Prompt_NO(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Prompt_YES(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
