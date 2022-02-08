/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/* Copyright (c) National Instruments 1999. All Rights Reserved.          */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  CHOICE                          1
#define  CHOICE_CHOICEWIND               2
#define  CHOICE_OKAY                     3       /* callback function: choice_callb */

#define  MULTIMETER                      2
#define  MULTIMETER_MULTI2               2
#define  MULTIMETER_MULTI1               3
#define  MULTIMETER_VCOOK                4       /* callback function: vcook */
#define  MULTIMETER_TEXTMSG              5
#define  MULTIMETER_TEXTMSG_2            6
#define  MULTIMETER_PICTURE_1            7
#define  MULTIMETER_PICTURE_2            8

#define  VCOADJUST                       3
#define  VCOADJUST_VCO                   2
#define  VCOADJUST_VCOOK                 3       /* callback function: vcook */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */ 

int  CVICALLBACK choice_callb(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK vcook(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
