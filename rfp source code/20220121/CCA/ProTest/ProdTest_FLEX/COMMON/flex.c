#include <userint.h>
#include "test.h" 
#include "flex.h"

int CVICALLBACK get_okay (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
//	if ((event == EVENT_COMMIT)||(event == EVENT_LEFT_CLICK))			 // 3 for HOT 
	if (event == EVENT_LEFT_CLICK)			 // 3 for HOT 
		iCheckOkay = 0x752 ;
	return 0;
}


int CVICALLBACK InputBarCode (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
		{
		case EVENT_COMMIT:
			SetActiveCtrl (panel, BARCODE_InputSerial);
			break;
		}
	return 0;
}

int CVICALLBACK InputSerial (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
		{
		case EVENT_COMMIT:
			iCheckOkay = 0x752 ; // wird als button ok gewertet
			break;
		}
	return 0;
}
