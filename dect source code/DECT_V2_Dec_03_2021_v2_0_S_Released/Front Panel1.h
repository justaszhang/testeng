/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/* Copyright (c) National Instruments 2021. All Rights Reserved.          */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  PANEL                            1
#define  PANEL_Status_Window              2       /* control type: textBox, callback function: (none) */
#define  PANEL_Serial_Window_1            3       /* control type: textBox, callback function: (none) */
#define  PANEL_FIXTURE_ID                 4       /* control type: string, callback function: (none) */
#define  PANEL_STATION_ID                 5       /* control type: string, callback function: (none) */
#define  PANEL_Log_File_String            6       /* control type: string, callback function: (none) */
#define  PANEL_Mitel_Serial_Number        7       /* control type: string, callback function: (none) */
#define  PANEL_MAC_Address                8       /* control type: string, callback function: (none) */
#define  PANEL_IPEI_Number                9       /* control type: string, callback function: (none) */
#define  PANEL_CM_Serial_Number           10      /* control type: string, callback function: (none) */
#define  PANEL_RING                       11      /* control type: ring, callback function: Set_Test_Type */
#define  PANEL_RING_2                     12      /* control type: ring, callback function: (none) */
#define  PANEL_COMMANDBUTTON              13      /* control type: command, callback function: Start */
#define  PANEL_STATUS                     14      /* control type: command, callback function: (none) */
#define  PANEL_PB_STOP                    15      /* control type: command, callback function: Stop */
#define  PANEL_COMMANDBUTTON_2            16      /* control type: command, callback function: Quit */
#define  PANEL_LED                        17      /* control type: LED, callback function: (none) */
#define  PANEL_PICTURE                    18      /* control type: picture, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK Quit(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Set_Test_Type(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Start(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Stop(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
