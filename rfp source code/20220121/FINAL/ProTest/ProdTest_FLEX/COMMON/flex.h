/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/* Copyright (c) National Instruments 2018. All Rights Reserved.          */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  BARCODE                         1
#define  BARCODE_InputBarCode            2       /* callback function: InputBarCode */
#define  BARCODE_OKAY                    3       /* callback function: get_okay */
#define  BARCODE_InputSerial             4       /* callback function: InputSerial */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */ 

int  CVICALLBACK get_okay(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK InputBarCode(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK InputSerial(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
