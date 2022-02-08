
#include "test.h"



/**************************************************************/            
/**** Menue Function ******************************************/            
/**************************************************************/            
/**************************************************************/            

int MiniSysClose(void)
{

	MiniSys_close ();  
	return (0);
}


int MiniSysOpen(void)
{

	MiniSys_open (2);    
	return (0);
}


int dut_set_diversity1(void)
{
	if(ipRfp_rsx_SetDiversity(0x01)) 
		MessagePopup("error", "setDiversity1");

	return (0);
}

int dut_set_diversity2(void)
{
	if(ipRfp_rsx_SetDiversity(0x02)) 
		MessagePopup("error", "setDiversity2");

	return (0);
}


int MiniSysSetOC8(void)
{
	MiniSys_set_OC(8, 1);
	return 0;
}

int MiniSysReSetOC8(void) //OC7    
{
	MiniSys_set_OC(8, 0);
	return 0;
}

int MiniSysSetOC6(void)
{
	MiniSys_set_OC(6, 1);
	return 0;
}

int MiniSysReSetOC6(void) //OC7    
{
	MiniSys_set_OC(6, 0);
	return 0;
}


int MiniSysSetOC5(void)
{
	MiniSys_set_OC(5, 1);
	return 0;
}

int MiniSysReSetOC5(void) //OC5    
{
	MiniSys_set_OC(5, 0);
	return 0;
}




/* FUNCTION ***************************************************/            
int GetBarCode(char* strMAC, char* strSerial)
// insert Barcode - call: GetBarCode(gMAC_Scanned, gSerialNr_Scanned);
{
//#define GETBARCODE_ONLYSERIAL
		iCheckOkay = 0;
		DisplayPanel (BarCode_Label);		// setzt merker zurück & displays panel auf zugehöriger seite
		SetActivePanel (BarCode_Label);

		SetCtrlVal (BarCode_Label, BARCODE_InputBarCode, "");
#ifdef GETBARCODE_ONLYSERIAL		
		SetCtrlAttribute (BarCode_Label,BARCODE_InputBarCode, ATTR_DIMMED, 1);
		SetActiveCtrl (BarCode_Label, BARCODE_InputSerial);  
#endif		
		SetCtrlVal (BarCode_Label, BARCODE_InputSerial, "");
		//SetActiveCtrl (BarCode_Label, InputBarCode);
		while(iCheckOkay != 0x752) //is set from okay-button
				ProcessSystemEvents();
#ifndef GETBARCODE_ONLYSERIAL				
		GetCtrlVal (BarCode_Label, BARCODE_InputBarCode, strMAC);
#endif		
		GetCtrlVal (BarCode_Label, BARCODE_InputSerial, strSerial);
		HidePanel (BarCode_Label);
		
	return (0);
}


/**************************************************************/            
/**** System Function *****************************************/            
/**************************************************************/            
/**************************************************************/            


int dut_error (struct test_flow *test_flow)
{  //it calls from rte if dut-error > maxerror

	iBarCodeRight = 0;	   // undefine Mac & serial
	disconnect(0);
	set_statuswindow("\n ERROR",COL_RED,RIGHT);
	set_hintwindow("Please open hood !! ",COL_NEUTRAL,RIGHT);
	testflow->running_right = 0;
	testflow->ready_right = 1;

	LogShortResult(1); 
	return (0); //rte breaks the test-sequence
}
 
 void disconnect(int mode)
{
	iBarCodeRight = 0;	   // undefine Mac & serial
   	MiniSys_set_OC(5, 0);   // switch power off via opto-switch
	(void)ipRfp_libssh_Close();
	(void)ipRfp_rsx_Close();
// 20180905, not realy need, but RFP47 have sometimes trouble with OC-control
   MiniSys_write ("RST");
	
	return;
}

/* FUNCTION ***************************************************/            
int start_up (struct rte_globals *global_sets)
/*                                                                          
 SPECIFICATION: get all handles for system-resources
***************************************************************/            
{
char temp_str[MAXPATHNAMELEN];
char current_dir[MAXPATHNAMELEN];
int panel,i,j,str_pos, byte, local_error = 0;
struct struct_file  file_group;
FILE *fa_r;

	globals = global_sets;	//set pointer for acces to all variables from rte.exe
	testflow = globals->testflow; //set pointer for acces to testflow-flags
//für die Fenstersteuerung check der referenz BG
	Global_imainpanel = globals->imainpanel;
	iBarCodeRight = 0;  
	BarCode_Label = LoadPanelEx(Global_imainpanel, "G:\\Protest\\ProdTest_FLEX\\COMMON\\flex.uir",BARCODE, __CVIUserHInst);
	PanelBreak = LoadPanelEx(Global_imainpanel, "G:\\Protest\\ProdTest_FLEX\\COMMON\\break.uir",PANELBREAK, __CVIUserHInst);
	
	// The WSAStartup function initiates use of the Winsock DLL by a process. 	
	if ( ipRfp_startWinsock() )
	{
		local_error++;
		print_resultdisplay(" winSocket not open !!");
	}
	strcpy(temp_str,globals->sequ_select->product_pfad);
	fa_r = fopen(temp_str,"r");
	if(fa_r)
	{
		file_get_group(&file_group,fa_r,globals->sequ_select->sachnumber);
		if(globals->sequ_select == globals->left)
		{
			print_resultdisplay(" startup left sequence ...");
	   		file_getpara(&file_group,"sw-release", softw_release_left, PARA_STRING);
	   		file_getpara(&file_group,"SoftwareVersion", strSoftwareVersion_left, PARA_STRING);
		}
		else
		{
			print_resultdisplay(" startup right sequence ...");
			
			file_getpara(&file_group,"DEBUG-MINISYS",&gDebugMiniSys, PARA_INTEGER);
			file_getpara(&file_group,"DEBUG-WLAN_EMBEDDED",&gDebugWlanEmbedded, PARA_INTEGER);
	   		file_getpara(&file_group,"sw-release", softw_release_right, PARA_STRING);
	   		file_getpara(&file_group,"SoftwareVersion", strSoftwareVersion_right, PARA_STRING);
		}
	   	fclose(fa_r);
	}
	else
	{
		MessagePopup ("Load Product Data Error, can't open File:", temp_str);
		QuitUserInterface(1);
	}
	if(MiniSys_open (2))    
	{
		sprintf(temp_str,"Mini Sys open error");
		print_resultdisplay( temp_str );
		local_error++;
	}
	i =MiniSys_getLightSensor();    // first time as dummy for setup internal Sensor
/*
//test for possible HW-reset from system:
	ComSetEscape (2, 3);		// set RTS   default state = low
	ComSetEscape (2, 4);		// clr RTS
	ComSetEscape (2, 5);	   // set DTR	  default state = low
	ComSetEscape (2, 6);		// clr DTR
*/	
	if(WLanEmbedded_open (4,5,6))		// open modules and setup mode (AT+CWMODE=1, Client mode)
	{
		sprintf(temp_str,"embedded WLan setup error");
		print_resultdisplay( temp_str );
		local_error++;
	}
	// print Fixture: 
	//globals->left->fixtureID
	//globals->right->fixtureID
	//(testflow->running_left ? globals->left->fixtureID : globals->right->fixtureID)
	//globals->sequ_select->fixtureID
	sprintf(temp_str," Fixture: %s",globals->sequ_select->fixtureID);
	print_resultdisplay( temp_str );
	if(CMD60_Initialize (GPIB_CMD60, VI_ON, VI_ON, &gpib_handle.cmd60))
	{
char tempstr[256];
		local_error++;
		sprintf(tempstr," CMD60 Error: dev \"%s\" not respond !!", GPIB_CMD60);
		print_resultdisplay(tempstr);
	}
	else
	{
// new get Version of CMD60 and,and,and
char driverRev[80];
char instrRev[80];
char dectFirmVers[80];
char tempstr[256];
double version=0;
int n[2];
//Software Version V 4.23 
//DECT Firmware E3.45M       (PP32Z prolonged preamble) 
		if ( ! CMD60_RevisionQuery(gpib_handle.cmd60, driverRev, instrRev) )
		{
			// "V 4.23 22.3.05"
			// aus dem String "V 4.23" einen double Wert machen
			sscanf( instrRev, "V %d.%d",&n[0],&n[1]);
			version=(n[0]*100 + n[1]) / 100.0;			
		}	
			// "E3.45M 23.10.2001"
		if ( ! CMD60_addon_GetFirmwareVersion (gpib_handle.cmd60,dectFirmVers) )
		{	
			// das " entfernen und nur bis zum ersten Blank
			sscanf( dectFirmVers, "\"%s ",dectFirmVers);
			//MessagePopup ("CMD60 FirmwareVersion", dectFirmVers);
		}
		sprintf(tempstr," CMD60 present V %.2f DECT %s",version,dectFirmVers);
		print_resultdisplay( tempstr );
		if ( version < 4.23 )
		{
			print_resultdisplay(" CMD60 Error: wrong Version!!");
			local_error++;
		}	
	} // CMD60 end
	if(!local_error)
		print_resultdisplay(" ----  System okay -----");
	else
		print_resultdisplay(" !! SYSTEM CAN'T WORK PROPERLY !!");
	
	return(0);
}

/* FUNCTION ***************************************************/            
int clean_up (int mode)
/*                                                                          
 SPECIFICATION: get all handles for system-resources
***************************************************************/            
{
//ipRfp_rsx_Close();
	WLanEmbedded_close();
	MiniSys_close ();  
	CMD60_Reset (gpib_handle.cmd60);
	(void)ipRfp_cleanupWinsock();
	print_resultdisplay("---- System down -----");
	return(0);
}


#undef RFP35_ONLY_LEFT
#define RFP35_ONLY_RIGHT
/* FUNCTION ***************************************************/            

int flow_control (struct test_flow *test_flow)
{
unsigned char port;
int i,j;
//get hood-state:
// 0xff kein Bit gesetzt
// 0xfd Bit1 , right closed 
// 0xfe Bit0 , left closed

//	ProcessDrawEvents (); 
//	ProcessSystemEvents();
	j = gDebugMiniSys;
	gDebugMiniSys = 0;
	i = MiniSys_get_IN();
	gDebugMiniSys = j;	
	port = i;

	if(port & HOODRIGHT)		
	{
		test_flow->hoodclosed_right= 0;  //it's open
		set_hintwindow("Please close hood !! ",COL_NEUTRAL,RIGHT);
		set_statuswindow("\n",COL_BLUE,RIGHT);
	}
	else
		test_flow->hoodclosed_right= 1;
	

	if((port & HOODRIGHT)&&!test_flow->running_right)    //hood open & not running
	{
		while(!iBarCodeRight)
			{
			GetBarCode(gMAC_Scanned, gSerialNr_Scanned);

			if (gSerialNr_Scanned[0]==0x33)
				{
				iBarCodeRight = 1;
				break;
				}
			else
				{
				iBarCodeRight = 0;
				}
			}
	}	

/*
	if((port & HOODRIGHT)&&!test_flow->running_right)    //hood open & not running
	{
		while (!(serial_ok == 1))
			{
			if(!iBarCodeRight)
				{
				iBarCodeRight = 1;
				GetBarCode(gMAC_Scanned, gSerialNr_Scanned);

				if (gSerialNr_Scanned[0]==0x33)
					{
					serial_ok=1;
					}
				else
					{
					serial_ok=0;
					iBarCodeRight = 0;
					}
				}
			}
		}	
*/
	if(test_flow->ready_right)
	{
		test_flow->running_right = 0;
		if(test_flow->ready_right == 1)
		{
			set_hintwindow("Please open hood",COL_NEUTRAL,RIGHT);
			if(!get_testresult(RIGHT))
				{
				LogShortResult(0); 
				set_statuswindow("\n OKAY",COL_GREEN,RIGHT);
				}
			test_flow->ready_right = 2;
		}
		if(!test_flow->hoodclosed_right)
			test_flow->ready_right = 0;
	}
	
	
	if(globals && !globals->executing)
	{
		if((test_flow->start_right)&&(test_flow->hoodclosed_right))
		{
			if(globals->right->executeable_tests)
			{
				test_flow->ready_right = 0;
				test_flow->running_right = 1;
				test_flow->start_right = 0;
				set_hintwindow(" ",COL_NEUTRAL,RIGHT);
				set_sequence(RIGHT);
				set_statuswindow("\nTest run..",COL_YELLOW,RIGHT);
				ProcessDrawEvents ();
				return(1);
			}
		}
	}
	
//if test uut --> set start				(runuut = run_endles)
	if((test_flow->runuut_right) && (!test_flow->hoodclosed_right))// && (!test_flow->start_left))
	{
		test_flow->start_right = 1;
	}
	return(0);
}

void LogShortResult(int mode)
// is called from functions: ; flow_control 
{
 FILE *fa_w;
char resultStr[0x10];
char log_fn[0x200];
char *time_help;
int x;

    if(mode)
        strcpy(resultStr,"FAIL");
    else
        strcpy(resultStr,"PASS");

	time_help=TimeStr();
	for (x=0;x<9;x++)
		{
		if(time_help[x]==0x3A)
			time_help[x]=0x2D;
		}
	

	sprintf(log_fn,"%s%s_%s_%s%s","c:\\short_log\\",gSerialNr_Scanned,DateStr(),time_help,".log");   
	
	fa_w = fopen(log_fn,"a");

    if(fa_w)
    {
        fprintf(fa_w,"FINAL_RFP,");
        fprintf(fa_w,"%s,",gSerialNr_Scanned);
        fprintf(fa_w,"%s,%s,",DateStr(),TimeStr());
        fprintf(fa_w,"%s\n",resultStr);
        fclose(fa_w);
    }
     return;
}


int io_error (struct test_flow *test_flow)
{
char temp_str[100];
unsigned char port = 0;	
FILE *fa_w;

	iBarCodeRight = 0;	   // undefine Mac & serial
	disconnect(0);
	CMD60_Reset (gpib_handle.cmd60);
	globals->executing = 0;
	sprintf(temp_str,"RUNTIME-ERROR-\nTest:%s, \nErrorcode: 0x%x ",test_flow->test_name,test_flow->error_code);
	set_statuswindow(temp_str,COL_NEUTRAL,RIGHT);
	test_flow->ioerror_right = 1;
//wait for hood-open:	
	set_hintwindow("Please open hood",COL_NEUTRAL,RIGHT);
	while(!(port & HOODRIGHT))
	{
		port = MiniSys_get_IN();
		ProcessSystemEvents();
		Delay(0.1);
	}
	test_flow->running_right = 0;
	if(globals->idatalogmode)
	{
		logging_io_error(test_flow->error_code);
	}
	if(globals->iprintmode)
	{
		print_out(temp_str);		//error-code
		sprintf(temp_str,"\n\n\n\n\n\n");
		print_out(temp_str);
		sprintf(temp_str,"%c%c%c",PRINTER_CUT_STRING); //cutting option
		print_out(temp_str);
	}

	LogShortResult(1); 
	return (0); //rte breaks the test-sequence
}

