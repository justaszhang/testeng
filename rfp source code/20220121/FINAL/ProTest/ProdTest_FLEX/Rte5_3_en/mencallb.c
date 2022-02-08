#ifndef rtemodule

#include <utility.h>
#include "inifile.h"
#include <ansi_c.h>
#include <userint.h>
#include <cvirte.h>		/* Needed if linking in external compiler; harmless otherwise */
#include "rte_uir.h"
#include "rtepubl.h"
#include "rte.h"
#include "Adapter_Para.h"
#define rtemodule

#endif

//execute menu-functions:
void menue_add_functions (int menuBarHandle,int menuItemID, void *callbackData, int panelHandle )
{
int i;
	
	for(i=0;i<MAXMENUEFCT;i++)
		{
		if(menue_function.menu_id[i] == menuItemID)
			{
			(menue_function.menue_fctptr[i])();
			break;
			}
		if(menue_functionL.menu_id[i] == menuItemID)
			{
			(menue_functionL.menue_fctptr[i])();
			break;
			}
		if(menue_functionR.menu_id[i] == menuItemID)
			{
			(menue_functionR.menue_fctptr[i])();
			break;
			}
		}
	return;
}



void CVICALLBACK menu1 (int menuBar, int menuItem, void *callbackData,
		int panel)
{
int   itemp,i;
int   devCount,serviceCount;
int   serviceDays;
int   intervalNr;
char  *p;
short porta,portb;

char  pfad[MAXPATHNAMELEN];
char  temp_str[80];
char  ucBuffer[0x100];
char  cInfo[512];
char  lastServiceDate[20];
char  nextServiceDate[20];
char  serviceText[256];
		

	switch (menuItem) 
		{
		case MENU1_FILE_LOADSEQUENCE_SEQUL:
			global.sequ_select = &left_sequ;
			portb = 0;
			porta = 1;
			goto load_sequence_select;
		case MENU1_FILE_LOADSEQUENCE_SEQUR:
			global.sequ_select = &right_sequ;
			porta = 0;
			portb = 1;
load_sequence_select:
//			for(itemp=0;itemp<global.sequ_select->executeable_tests;itemp++)
//				free(global.sequ_select->structpointer[itemp]);
			PromptPopup ("Load new Sequence","Please Enter Adaptercode",temp_str, 30);
			i = atoi(temp_str);
			if(porta) porta = i;
			if(portb) portb = i;
			run_adaptercode(porta, portb);
			break;
		case MENU1_FILE_EDITSEQUENCE_SEQUL:
			edit_sequence(1);
			break;
		case MENU1_FILE_EDITSEQUENCE_SEQUR:
			edit_sequence(2);
			break;
		case MENU1_FILE_EDITSEQUENCE_NEWSEQU:
			edit_sequence(0);
			break;
			
		case MENU1_FILE_LOGIN:
			InstallPopup(global.iloginpanel);
			global.ireference = 0;
			global.irepairmode = 0;      // @GC 04.09.2011
			global.debug_mode = 0;       // @GC 04.09.2011
			set_menu1_attr(); 
			break;
			
//3 printmodes:			
		case MENU1_PRINT_PRINTON:
			GetMenuBarAttribute (menuBar, MENU1_PRINT_PRINTON,ATTR_CHECKED, &itemp);
			if(itemp)
				{
				SetMenuBarAttribute (menuBar, MENU1_PRINT_PRINTON,ATTR_CHECKED, 0);
				global.iprintmode = 0;
				}
			else
				{
				SetMenuBarAttribute (menuBar, MENU1_PRINT_PRINTON,ATTR_CHECKED, 1);
				GetMenuBarAttribute (menuBar, MENU1_PRINT_PRINTERROR,ATTR_CHECKED, &itemp);
				if(itemp) global.iprintmode = 1; 	//print error
				else global.iprintmode = 2;		//print all		
				}
			break;
		case MENU1_PRINT_PRINTERROR:
			GetMenuBarAttribute (menuBar, MENU1_PRINT_PRINTERROR,ATTR_CHECKED, &itemp);
			if(itemp) 	{SetMenuBarAttribute (menuBar, MENU1_PRINT_PRINTALL,ATTR_CHECKED, 1);
						SetMenuBarAttribute (menuBar, MENU1_PRINT_PRINTERROR,ATTR_CHECKED, 0);}
			else 		{SetMenuBarAttribute (menuBar, MENU1_PRINT_PRINTALL,ATTR_CHECKED, 0);
						SetMenuBarAttribute (menuBar, MENU1_PRINT_PRINTERROR,ATTR_CHECKED, 1);}
			if(global.iprintmode)
				{
				if(itemp) global.iprintmode = 2; //printall
				else global.iprintmode = 1;
				}
			break;
		case MENU1_PRINT_PRINTALL:
			GetMenuBarAttribute (menuBar, MENU1_PRINT_PRINTALL,ATTR_CHECKED, &itemp);
			if(itemp) 	{SetMenuBarAttribute (menuBar, MENU1_PRINT_PRINTALL,ATTR_CHECKED, 0);
						SetMenuBarAttribute (menuBar, MENU1_PRINT_PRINTERROR,ATTR_CHECKED, 1);}
			else 		{SetMenuBarAttribute (menuBar, MENU1_PRINT_PRINTALL,ATTR_CHECKED, 1);
						SetMenuBarAttribute (menuBar, MENU1_PRINT_PRINTERROR,ATTR_CHECKED, 0);}
			if(global.iprintmode)
				{
				if(itemp) global.iprintmode = 1; //printerror
				else global.iprintmode = 2;
				}
			break;
			
//3 display clear-modi: 0 == nie löschen, 1-every testloop, 2- every test
		case MENU1_SETS_RDISP_CLREVERYTEST:
			GetMenuBarAttribute (menuBar, MENU1_SETS_RDISP_CLREVERYTEST,ATTR_CHECKED, &itemp);
			if(itemp)
				{
				SetMenuBarAttribute (menuBar, MENU1_SETS_RDISP_CLREVERYTEST,ATTR_CHECKED, 0);
				global.iclrdisplay = 0; //never clear
				}
			else
				{
				SetMenuBarAttribute (menuBar, MENU1_SETS_RDISP_CLREVERYTEST,ATTR_CHECKED, 1);
				SetMenuBarAttribute (menuBar, MENU1_SETS_RDISP_CLREVERYLOOP,ATTR_CHECKED, 0);
				global.iclrdisplay = 2;
				}
			break;
		case MENU1_SETS_RDISP_CLREVERYLOOP:
			GetMenuBarAttribute (menuBar, MENU1_SETS_RDISP_CLREVERYLOOP,ATTR_CHECKED, &itemp);
			if(itemp)
				{
				SetMenuBarAttribute (menuBar, MENU1_SETS_RDISP_CLREVERYLOOP,ATTR_CHECKED, 0);
				global.iclrdisplay = 0; //never clear
				}
			else
				{
				SetMenuBarAttribute (menuBar, MENU1_SETS_RDISP_CLREVERYLOOP,ATTR_CHECKED, 1);
				SetMenuBarAttribute (menuBar, MENU1_SETS_RDISP_CLREVERYTEST,ATTR_CHECKED, 0);
				global.iclrdisplay = 1; 
				}
			break;
		// print to display-mode: all or only error
		case MENU1_SETS_RDISP_ONLYERROR:
			GetMenuBarAttribute (menuBar, MENU1_SETS_RDISP_ONLYERROR,ATTR_CHECKED, &itemp);
			if(itemp)
				{
				SetMenuBarAttribute (menuBar, MENU1_SETS_RDISP_ONLYERROR,ATTR_CHECKED, 0);
				global.iprintresult = 1;
				}
			else
				{
				SetMenuBarAttribute (menuBar, MENU1_SETS_RDISP_ONLYERROR,ATTR_CHECKED, 1);
				global.iprintresult = 0;
				}
			break;

//log-result-mode:		
		case MENU1_SETS_DATALOG_DATALOGON:
			GetMenuBarAttribute (menuBar, MENU1_SETS_DATALOG_DATALOGON,ATTR_CHECKED, &itemp);
			if(itemp)
				{
				SetMenuBarAttribute (menuBar, MENU1_SETS_DATALOG_DATALOGON,ATTR_CHECKED, 0);
				global.idatalogmode = 0;
				}
			else
				{
				SetMenuBarAttribute (menuBar, MENU1_SETS_DATALOG_DATALOGON,ATTR_CHECKED, 1);
				global.idatalogmode = 1;
				}
			break;

//System Events - set Performance			
		case MENU1_SETS_SYSEVENT_SRVEVERYTEST:
			GetMenuBarAttribute (menuBar, MENU1_SETS_SYSEVENT_SRVEVERYTEST,ATTR_CHECKED, &itemp);
			if(!itemp)
				{					  //process events after every test
				SetMenuBarAttribute (menuBar, MENU1_SETS_SYSEVENT_SRVEVERYTEST,ATTR_CHECKED, 1);
				SetMenuBarAttribute (menuBar, MENU1_SETS_SYSEVENT_SRVEVERYLOOP,ATTR_CHECKED, 0);
				global.iservesystem = 1;
				}
			break;				
		case MENU1_SETS_SYSEVENT_SRVEVERYLOOP:
			GetMenuBarAttribute (menuBar, MENU1_SETS_SYSEVENT_SRVEVERYLOOP,ATTR_CHECKED, &itemp);
			if(!itemp)
				{					 //process events after every loop
				SetMenuBarAttribute (menuBar, MENU1_SETS_SYSEVENT_SRVEVERYLOOP,ATTR_CHECKED, 1);
				SetMenuBarAttribute (menuBar, MENU1_SETS_SYSEVENT_SRVEVERYTEST,ATTR_CHECKED, 0);
				global.iservesystem = 0;
				}
			break;				
// Errorrate-Display
		case MENU1_ERRORRATE_SHOWER:
			GetMenuBarAttribute (menuBar,MENU1_ERRORRATE_SHOWER ,ATTR_CHECKED, &itemp);
			if(itemp)
				{					 
				SetMenuBarAttribute (menuBar, MENU1_ERRORRATE_SHOWER,ATTR_CHECKED, 0);
	 			SetCtrlAttribute (global.imainpanel, left_sequ.errorrate_handle, ATTR_VISIBLE,0);
	 			SetCtrlAttribute (global.imainpanel, right_sequ.errorrate_handle, ATTR_VISIBLE,0);
				global.showerrorrate = 0;
				}
			else
				{					 
				SetMenuBarAttribute (menuBar, MENU1_ERRORRATE_SHOWER,ATTR_CHECKED, 1);
	 			SetCtrlAttribute (global.imainpanel, left_sequ.errorrate_handle, ATTR_VISIBLE,1);
	 			SetCtrlAttribute (global.imainpanel, right_sequ.errorrate_handle, ATTR_VISIBLE,1);
				global.showerrorrate = 1;
				if(global.sequ_select->idevicecount)	//cancel division by zero
					{
					itemp = global.sequ_select->idevicefail*100/global.sequ_select->idevicecount;
					SetCtrlVal (global.imainpanel, global.sequ_select->errorrate_handle, itemp);
					}
				}
			break;
		case MENU1_ERRORRATE_RESETER:
			SetCtrlVal (global.imainpanel, global.sequ_select->errorrate_handle, 0);
			global.sequ_select->idevicefail = 0;
			global.sequ_select->idevicecount = 0;
			break;
		case MENU1_SPECIAL_REFERENCE:
			GetMenuBarAttribute (menuBar,MENU1_SPECIAL_REFERENCE , ATTR_CHECKED, &itemp);
			if(itemp)
				{
				global.ireference = 0;					 
				SetCtrlVal (global.imainpanel, PANEL1_ONLYMEASUREMSG, "");	
				}
			else
				{
				global.ireference = 1;					 
				SetCtrlVal (global.imainpanel, PANEL1_ONLYMEASUREMSG, "!! system runs in reference mode !!");	
				}
			SetMenuBarAttribute (menuBar, MENU1_SPECIAL_REFERENCE,ATTR_CHECKED, global.ireference);
			break;
       case MENU1_SPECIAL_REPAIR:
			GetMenuBarAttribute (menuBar,MENU1_SPECIAL_REPAIR , ATTR_CHECKED, &itemp);
			if(itemp)
				{
				global.irepairmode = 0;					 //@GC 04.09.2011
				SetCtrlVal (global.imainpanel, PANEL1_ONLYMEASUREMSG, "");	
				//SetPanelAttribute (global.imainpanel, ATTR_BACKCOLOR, VAL_LT_GRAY);
				}
			else
				{
				global.irepairmode = 1; 				 //@GC 04.09.2011
				SetCtrlVal (global.imainpanel, PANEL1_ONLYMEASUREMSG, "!! system works in repair mode !!");	
				//SetPanelAttribute (global.imainpanel, ATTR_BACKCOLOR, VAL_MAGENTA);
				}
			SetMenuBarAttribute (menuBar, MENU1_SPECIAL_REPAIR,ATTR_CHECKED, global.irepairmode);
			break;
/*
       case MENU1_SPECIAL_REPAIR:
			GetMenuBarAttribute (menuBar,MENU1_SPECIAL_REPAIR , ATTR_CHECKED, &itemp);
			if(itemp)
				{
				global.iloginlevel &= 0xFB;					 
				SetCtrlVal (global.imainpanel, PANEL1_ONLYMEASUREMSG, "");	
				SetMenuBarAttribute (menuBar, MENU1_SPECIAL_REPAIR,ATTR_CHECKED, 0);
				SetPanelAttribute (global.imainpanel, ATTR_BACKCOLOR, VAL_LT_GRAY);
				}
			else
				{
				global.iloginlevel |= 0x04;					 
				SetCtrlVal (global.imainpanel, PANEL1_ONLYMEASUREMSG, "!! system works in repair logging level !!");	
				SetMenuBarAttribute (menuBar, MENU1_SPECIAL_REPAIR,ATTR_CHECKED, 1);
				SetPanelAttribute (global.imainpanel, ATTR_BACKCOLOR, VAL_MAGENTA);
				}
			break;
*/     
        //@GC 04.09.2011
       case MENU1_SPECIAL_DEBUG:
			GetMenuBarAttribute (menuBar,MENU1_SPECIAL_DEBUG , ATTR_CHECKED, &itemp);
			if(itemp)
				{
				global.debug_mode = 0;
				//SetCtrlVal (global.imainpanel, PANEL1_ONLYMEASUREMSG, "");	
				}
			else
				{
				global.debug_mode = 1;
				//SetCtrlVal (global.imainpanel, PANEL1_ONLYMEASUREMSG, "!! system works in debug mode !!");
				}
				
			SetMenuBarAttribute (menuBar, MENU1_SPECIAL_DEBUG,ATTR_CHECKED, global.debug_mode);
			break;
       
		case MENU1_SPECIAL_MAXERROR:
			if(global.iloginlevel)
				{
		    	PromptPopup ("Change sequence defaults ", "Please enter MAX-Error count [1..Tests]", temp_str ,3);
		    	i = atoi(temp_str);
		    	if(i<1)
					MessagePopup ("Number Error !!","maxerror out of range, value is not changed !!");
		    	else
    				{
    				if(global.left)
    					global.left->maxerror = i; 
    				if(global.right)
    					global.right->maxerror = i;
    				}
				break;
				}
		}
		
		
	// @GC 05.09.2011
	if (  global.ireference == 0 && global.irepairmode == 0 && global.debug_mode == 0 )					 
    {
		// switsch back to normal color
		SetPanelAttribute (global.imainpanel, ATTR_BACKCOLOR, VAL_LT_GRAY);
	}	
	else
	{
		SetPanelAttribute (global.imainpanel, ATTR_BACKCOLOR, VAL_GRAY);   //dark gray
	}
	
	
}

void set_menu1_attr(void)
{
int menu;


	menu = GetPanelMenuBar (global.imainpanel);

	SetMenuBarAttribute (menu,MENU1_ERRORRATE_SHOWER ,ATTR_CHECKED,global.showerrorrate);
	if(global.sequ_select != NULL) SetCtrlAttribute (global.imainpanel, global.sequ_select->errorrate_handle, ATTR_VISIBLE,global.showerrorrate);
	SetMenuBarAttribute (menu, MENU1_SETS_DATALOG_DATALOGON, ATTR_CHECKED,global.idatalogmode);
	SetMenuBarAttribute (menu, MENU1_PRINT_PRINTON,ATTR_CHECKED, global.iprintmode);
	
		
	switch (global.iprintmode)
		{
		case 1:
			SetMenuBarAttribute (menu, MENU1_PRINT_PRINTERROR,ATTR_CHECKED, 1);
			SetMenuBarAttribute (menu, MENU1_PRINT_PRINTALL,ATTR_CHECKED, 0);
			break;
		case 2:
			SetMenuBarAttribute (menu, MENU1_PRINT_PRINTERROR,ATTR_CHECKED, 0);
			SetMenuBarAttribute (menu, MENU1_PRINT_PRINTALL,ATTR_CHECKED, 1);
			break;
		}		
	if(global.iclrdisplay == 1)		
		{
		SetMenuBarAttribute (menu, MENU1_SETS_RDISP_CLREVERYLOOP,ATTR_CHECKED, 1);
		SetMenuBarAttribute (menu, MENU1_SETS_RDISP_CLREVERYTEST,ATTR_CHECKED, 0);
		}		
	if(global.iclrdisplay == 2)	
		{
		SetMenuBarAttribute (menu, MENU1_SETS_RDISP_CLREVERYLOOP,ATTR_CHECKED, 0);
		SetMenuBarAttribute (menu, MENU1_SETS_RDISP_CLREVERYTEST,ATTR_CHECKED, 1);
		}		
	SetMenuBarAttribute (menu, MENU1_SETS_RDISP_ONLYERROR,ATTR_CHECKED, !global.iprintresult);
				
	if(global.iservesystem)
		{
		SetMenuBarAttribute (menu, MENU1_SETS_SYSEVENT_SRVEVERYTEST,ATTR_CHECKED, 1);
		SetMenuBarAttribute (menu, MENU1_SETS_SYSEVENT_SRVEVERYLOOP,ATTR_CHECKED, 0);
		}
	else
		{
		SetMenuBarAttribute (menu, MENU1_SETS_SYSEVENT_SRVEVERYTEST,ATTR_CHECKED, 0);
		SetMenuBarAttribute (menu, MENU1_SETS_SYSEVENT_SRVEVERYLOOP,ATTR_CHECKED, 1);
		}
		
		
	if(!global.iloginlevel)		
		{
		// not all menubaritems visible !
		//SetMenuBarAttribute (menu, MENU1_SETS, ATTR_DIMMED, 1);
		
		SetMenuBarAttribute (menu, MENU1_SETS_RDISP, ATTR_DIMMED, 1);
		SetMenuBarAttribute (menu, MENU1_SETS_SYSEVENT, ATTR_DIMMED, 1);
		SetMenuBarAttribute (menu, MENU1_SETS_DATALOG, ATTR_DIMMED, 1);
		
		SetMenuBarAttribute (menu, MENU1_FILE_EDITSEQUENCE, ATTR_DIMMED, 1);
		SetMenuBarAttribute (menu, MENU1_LEFTSPEC, ATTR_DIMMED, 1);
		SetMenuBarAttribute (menu, MENU1_RIGHTSPEC, ATTR_DIMMED, 1);
		SetMenuBarAttribute (menu, MENU1_SPECIAL_DEBUG, ATTR_DIMMED, 1);       //deactivate @GC 14.09.2011 
		SetMenuBarAttribute (menu, MENU1_SPECIAL_MAXERROR, ATTR_DIMMED, 1);
		}

    
// new @GC 04.09.2011 
	SetMenuBarAttribute (menu, MENU1_SPECIAL_DEBUG,ATTR_CHECKED, global.debug_mode);
	SetMenuBarAttribute (menu, MENU1_SPECIAL_REFERENCE,ATTR_CHECKED, global.ireference);
	SetMenuBarAttribute (menu, MENU1_SPECIAL_REPAIR,ATTR_CHECKED, global.irepairmode);
    
// new @GC 04.09.2011 
   	if(global.irepairmode && global.ireference )
		SetCtrlVal (global.imainpanel, PANEL1_ONLYMEASUREMSG, "!! system works in reference/repair mode !!");	
   	else if(global.irepairmode)
		SetCtrlVal (global.imainpanel, PANEL1_ONLYMEASUREMSG, "!! system works in repair mode !!");	
	else if(global.ireference)
		SetCtrlVal (global.imainpanel, PANEL1_ONLYMEASUREMSG, "!! system works in reference mode !!");	
	else
		SetCtrlVal (global.imainpanel, PANEL1_ONLYMEASUREMSG, "");	




// new @GC 05.09.2011
	if (  global.ireference == 0 && global.irepairmode == 0 && global.debug_mode == 0 )					 
    {
		// switsch back to normal color
		SetPanelAttribute (global.imainpanel, ATTR_BACKCOLOR, VAL_LT_GRAY);
	}	
	else
	{
		SetPanelAttribute (global.imainpanel, ATTR_BACKCOLOR, VAL_GRAY);   //dark gray
	}

	return;
}		
