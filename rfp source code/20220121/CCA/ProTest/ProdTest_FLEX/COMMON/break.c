#include <cvirte.h>		
#include <userint.h>
#include "test.h"  
#include "break.h"

static int panelHandle;

int CVICALLBACK break_LED (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
		{
		case EVENT_COMMIT:

			break;
		}
	return 0;
}

int CVICALLBACK break_ok (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
		{
		case EVENT_COMMIT:
			iCheckOkay = 0x752 ;
			break;
		}
	return 0;
}


