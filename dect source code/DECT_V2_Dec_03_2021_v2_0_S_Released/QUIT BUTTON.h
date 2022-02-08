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
#define  PANEL_Quit_PB_TEXTMSG           2
#define  PANEL_PB_Quit                   3       /* callback function: PB_Quit_App */
#define  PANEL_PB_Quit_Cancel            4       /* callback function: PB_Cancel_Quit */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */ 

int  CVICALLBACK PB_Cancel_Quit(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PB_Quit_App(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
