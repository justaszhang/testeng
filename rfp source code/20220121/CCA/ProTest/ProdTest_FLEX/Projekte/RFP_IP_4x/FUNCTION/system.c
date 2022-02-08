
#include "test.h"
#include "MiniSystem.h"
#include "rtx2012base.h"
#include "HP53131A.h"

int start_up (struct rte_globals *global_sets);

int SpecialRunEndlesCounter = 0;

extern int Print2StandardIO;     //for debugging DUT rs232... 
/**************************************************************/            
/**** Menue Function ******************************************/            
/**************************************************************/            
/**************************************************************/            

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


/* FUNCTION ***************************************************/            
int dut_OpenLibssh(void)
{
int  rc;
char cIpAddr[256];
char cInfo[256];

	sprintf(cIpAddr, "");
	PromptPopup("ipadr", "Eingabe ipAddr\nz.b. 192.168.99.254", cIpAddr, 20);
	rc = ipRfp_libssh_Open( cIpAddr );
	sprintf(cInfo, "%s RetCode=%d", cIpAddr, rc);
	MessagePopup("StatusLibssh", cInfo);
	(void)ipRfp_libssh_Close();
	return(0);
	
} 



/* FUNCTION ***************************************************/            
int dut_OpenCOM1 (void)
{
	
	(void)ipRfp_rs232_Open(1,115200, 1000);
	
	return(0);
}

/* FUNCTION ***************************************************/            
int dut_CloseCOM1 (void)
{

	// disable COM, you can start teraterm.....	
	(void)ipRfp_rs232_Close();

	
	return(0);
}

/* FUNCTION ***************************************************/            
int dut_PowerOn (void)
{
/*
int polan_flag=0;	
	
	if( ConfirmPopup("Power Over LAN", "PoLAN = Yes    PoPSU = No") )
	{	
		polan_flag=1;
	}	
*/	
	MiniSys_set_OC(4, 1);   // switch power on via opto-switch

	return(0);
	
}



/* FUNCTION ***************************************************/            
int dut_PowerOff(void)
{
	MiniSys_set_OC(4, 0);   // switch power off via opto-switch
	return(0);
	
}


/**************************************************************/            
/**** System Function *****************************************/            
/**************************************************************/            
/**************************************************************/            


/* FUNCTION ***************************************************/            
int dut_error (struct test_flow *test_flow)
{  //it calls from rte if dut-error > maxerror

	(void)ipRfp_libssh_Close();
// close socket connection
	(void)ipRfp_rsx_Close();         
// power off
// open finger
	Sleep(250);      // 250ms
// open moving platte (unlook)
	if(testflow->running_left)
	{
		iBarCodeLeft = 0;
		set_statuswindow("\n ERROR",COL_RED,LEFT);
		set_hintwindow("Please open hood !! ",COL_NEUTRAL,LEFT);
		testflow->running_left = 0;
		testflow->ready_left = 1;
	}
	else
	{
		iBarCodeRight = 0;
		set_statuswindow("\n ERROR",COL_RED,RIGHT);
		set_hintwindow("Please open hood !! ",COL_NEUTRAL,RIGHT);
		testflow->running_right = 0;
		testflow->ready_right = 1;
	}

	LogShortResult(1);
	return (0); //rte breaks the test-sequence
}
 

/* FUNCTION ***************************************************/            
int start_up (struct rte_globals *global_sets)
/*                                                                          
 SPECIFICATION: get all handles for system-resources
***************************************************************/            
{
char	cErrMsg[1024];
char temp_str[MAXPATHNAMELEN];
char current_dir[MAXPATHNAMELEN];
int panel,i,j,str_pos, byte, local_error = 0;
char eeprom_to0x1f[MAXSTRINGLEN];
struct struct_file  file_group;
FILE *fa_r;

// power on in startup-function (system.c), the switch is every time powered on
//LAN connection ist connected, only the power is switched
	if(MiniSys_open (2))
	{
		sprintf(temp_str,"Mini Sys open error");
		print_resultdisplay( temp_str );
		local_error++;
	}
	if(MiniSys_set_OC(4, 1))   // switch power on via opto-switch
		return(0x1001); //1..8:0|1    
	Sleep(2000); // needs much mor time		

	iBarCodeLeft = 0;
	iBarCodeRight = 0;
	Print2StandardIO = 1;
	globals = global_sets;	//set pointer for acces to all variables from rte.exe
	testflow = globals->testflow; //set pointer for acces to testflow-flags
//für die Fenstersteuerung check der referenz BG
	Global_imainpanel = globals->imainpanel;
	

// open COM-Communication
	if(ipRfp_rs232_Open(globals->module_portA,115200, 1000))        // COM1
	{
		local_error++;
		print_resultdisplay(" RS232-PORT not open !!");
	}
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
// get eeprom-file, software release, emc
		file_get_group(&file_group,fa_r,globals->sequ_select->sachnumber);
   		file_getpara(&file_group,"flex_uir", strFlexUir, PARA_STRING);
		BarCode_Label = LoadPanelEx(Global_imainpanel, strFlexUir,BARCODE, __CVIUserHInst);
		
		file_getpara(&file_group,"ERP_PartNo", DutParameter.strERP_PartNo, PARA_STRING);
//		file_getpara(&file_group,"tftp_daemon", strTftpDaemon, PARA_STRING);
//		file_getpara(&file_group,"openOCDScriptPath", strOpenOCDScriptPath, PARA_STRING);
//		file_getpara(&file_group,"manuSoftwarePath", strManuSoftwarePath, PARA_STRING);
//		file_getpara(&file_group,"HW_type", strHwType, PARA_STRING);
//		file_getpara(&file_group,"HW_type", DutParameter.strHwType, PARA_STRING);
//		file_getpara(&file_group,"HW_branding", strHwBranding, PARA_STRING);
//		file_getpara(&file_group,"HW_branding", DutParameter.strHwBranding, PARA_STRING);
		file_getpara(&file_group,"SpecialRunEndlesCounter", &SpecialRunEndlesCounter, PARA_INTEGER);
		file_getpara(&file_group,"StdIOforDebug", &Print2StandardIO, PARA_INTEGER);
		
	
		if(globals->sequ_select == globals->left)
			{
			print_resultdisplay(" startup left sequence ...");
	   		file_getpara(&file_group,"sw-release", softw_release_left, PARA_STRING);
	   		file_getpara(&file_group,"SoftwareVersion", strSoftwareVersion_left, PARA_STRING);
			/*
			part_ident_parameter_left.type = 0; // type = 0  ohne EMC
			*/	
	   		}
	   	else
	   		{
			print_resultdisplay(" startup right sequence ...");
			file_getpara(&file_group,"DEBUG-MINISYS",&gDebugMiniSys, PARA_INTEGER);
			file_getpara(&file_group,"DEBUG-WLAN_EMBEDDED",&gDebugWlanEmbedded, PARA_INTEGER);
	   		file_getpara(&file_group,"sw-release", softw_release_right, PARA_STRING);
	   		file_getpara(&file_group,"SoftwareVersion", strSoftwareVersion_right, PARA_STRING);
			/*
			part_ident_parameter_right.type = 0; // type = 0  ohne EMC
			*/
	   		}
			

		file_getpara(&file_group,"Conect_counter", &Conect_counter, PARA_INTEGER);
	   	fclose(fa_r);
		}
	else
		{
		MessagePopup ("Load Product Data Error, can't open File:", temp_str);
		QuitUserInterface(1);
		}

//----------------------------------------------------------------
#ifdef START_TFTP_FROM_HERE
	if ( strlen( strTftpDaemon ) != 0 )
	{	
//	
/*
c:\DHCP_Server\dhcpsrv.exe -start

-runapp 	runs the DHCP Server in application mode (regardless of service installs)
-remove 	removes the service
-install 	installs the DHCP Server as a service
-start 	starts the service
-stop 	stops the service
-control 	opens the above dialog box to control the service
-help 	shows a help message dialog box
-ini filename 	sets the pathname of the ini file to be used by the DHCP Server (default is dhcpsrv.ini). This works only in application mode
-configfirewall 	Configures firewall exceptions for dhcpsrv.exe
-removefirewall 	Removes firewall exceptions for dhcpsrv.exe
*/
		
		MessagePopup ("Tftpd", strTftpDaemon);
		if(system("c:\\DHCP_Server\\dhcpsrv.exe -start") !=0 )
//		if ( system(strTftpDaemon) != 0 ) 
		{
			MessagePopup ("Error" , "can't start the dhcp/tftp Daemon");
			QuitUserInterface(1);
		}
	}
	// Ende start tftpd
			
#endif //START_TFTP_FROM_HERE
	
	if(0 == globals->irfEquipmentType)	   //cmd60
	{
#define CMD60_INIT
#ifdef CMD60_INIT


		if(CMD60_Initialize (GPIB_CMD60, VI_ON, VI_ON, &gpib_handle.cmd60))		 //GPIB::5
		{
			char tempstr[256];
			local_error++;

			sprintf(tempstr," CMD60 Error: dev \"%s\" not respond !!", GPIB_CMD60);
			print_resultdisplay(tempstr);
		}
		else
		{
			// new get Version of CMD60 and,and,and
			// [@GC25052011]
			char driverRev[80];
			char instrRev[80];
			char dectFirmVers[80];
			char hwOption[256];
			char tempstr[256];
			double version=0;
			int n[2];
			char *p;

			//
			//Software Version V 4.23
			//DECT Firmware E3.45M       (PP32Z prolonged preamble)
			//

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
			if ( CMD60_addon_SetPacketTypePP32Z (gpib_handle.cmd60) )
			{
				//PP32Z prolonged preamble
				print_resultdisplay( " CMD60 Error: PP32Z not supported!!");
				local_error++;

			}
			else
			{
				//PP32Z prolonged preamble
				print_resultdisplay( " CMD60 PP32Z supported");
			}
#ifdef CMD60_CHECK_OPTIONS
			if ( ! CMD60_addon_GetOption (gpib_handle.cmd60,hwOption) )
			{
				//B1,,,,B3,,B4,B41,,,B44,,,,B6,B61,B62,,,,,,,,,,,,,,,,,,,,,,,,,,,,,CPU3,8MB
				if ( (p=strstr(hwOption , ",CPU")) != NULL ) *p='\0';

				// LATAM Option ??
				if ( (p=strstr(hwOption , ",K61,")) == NULL )
				{
					print_resultdisplay( " CMD60 Error: K61 LATAM not supported!!");
					local_error++;
				}
				else
				{
					print_resultdisplay( " CMD60 K61 LATAM supported");
				}

				//MessagePopup ("CMD60 SystemOption", hwOption);
				//sprintf(tempstr," CMD60 HW-Option %s",hwOption);
				//print_resultdisplay( tempstr );
			}
#endif
			// new end
//#define GPIB_CMD60CNT  "GPIB::6"
#ifdef GPIB_CMD60CNT
			if(CMD60_Initialize (GPIB_CMD60CNT, VI_ON, VI_ON, &gpib_handle.cmd60Cnt))
			{
				char tempstr[256];
				local_error++;

				sprintf(tempstr," CMD60 Error: dev \"%s\" not respond !!", GPIB_CMD60CNT);
				print_resultdisplay(tempstr);
			}
#else
			gpib_handle.cmd60Cnt = gpib_handle.cmd60;
#endif
		} // CMD60 end

#endif	  //CMD60_INIT

	}

	else	   //rtx2012
	{
		char msg[256] = {'\0'};
		if(!rtx2012_Initialize (msg))
		{
			char tempstr[256];
			local_error++;

			sprintf(tempstr," RTX Error: %s !!", msg);
			print_resultdisplay(tempstr);
		}
		





	

	}
	

	if(0 != globals->irfEquipmentType)	   //not cmd60
	{
		//HP53131A init		  combind with RTX2012
		if(!Init_Counter())
		{
			char tempstr[256];
			local_error++;

			sprintf(tempstr," HP53131 Error !!");
			print_resultdisplay(tempstr);
		}
	}






	
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

	//MessagePopup("clean","clean_up");
	backup_logdata_files(globals);

	// power off
	// open EN_GiGABIT_LAN Relay

	//disconnect RS232
	if(0 == globals->irfEquipmentType)	   //cmd60
	{
		CMD60_Reset (gpib_handle.cmd60);
	}
	else
	{
		rtx2012_Reset();
	}

	print_resultdisplay("---- System down -----");
	(void)ipRfp_rs232_Close();
	(void)ipRfp_cleanupWinsock();
	return(0);
}




/* FUNCTION ***************************************************/            
/* FUNCTION ***************************************************/            
int flow_control (struct test_flow *test_flow)
{
unsigned char port;
int i,j,k,h;

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

	i = MiniSys_get_IN();
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
			GetBarCode(strBarcode_right, strSerial,1);		//valid MAC and serial
			 h=strlen(strMAC) ;
//			if (gSerialNr_Scanned[0]==0x33)
			k=strlen(strSerial);
		if (k==15&&h==17) //lenght of Serial number strimg
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
	
	if((port & HOODRIGHT)&&!test_flow->running_right) 
		{
		if(!iBarCodeRight)
			{
			if(!test_flow->running_left)
				{
				GetBarCode(strBarcode_right, strSerial,1);		//valid MAC and serial
				iBarCodeRight = 1;
				}
			else
				set_hintwindow("waiting for left ",COL_NEUTRAL,RIGHT);

			}
		else
			{
			test_flow->hoodclosed_right= 0;  //it's open
			set_hintwindow("Please close hood !! ",COL_NEUTRAL,RIGHT);
			set_statuswindow(strBarcode_right,COL_BLUE,RIGHT);
			}
		}
	else
		test_flow->hoodclosed_right= 1;

//get hood-state ready-----------------------------------------------	
//Reset FlexId + SerNo only if BOTH hoods are open [@JM090305]
	if ( test_flow->hoodclosed_left == 0 && test_flow->hoodclosed_right == 0)
	{
#ifdef USE_QPDS	
		structQPDS.strExID[0] = 0;
		structQPDS.strCustSn[0] = 0;
		structQPDS.strMacAddr[0] = 0;
		structQPDS.strParkNr[0] = 0;
		//--- after running test: serve result-window & wait for open hood:
#endif		
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
				set_statuswindow("\nOKAY",COL_GREEN,RIGHT);
				}
			test_flow->ready_right = 2;
			}
//special run mode:
		if(SpecialRunEndlesCounter)
		{
			SpecialRunEndlesCounter--;
			test_flow->hoodclosed_right = 0;
		}
		if(!test_flow->hoodclosed_right)
			test_flow->ready_right = 0;
		}

	
	if(globals && !globals->executing)
		{
//------------------------------------------------
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
//if test uut --> set start		(runuut = run_endles)
#define NORMAL_MODE

#ifdef	NORMAL_MODE

		if((test_flow->runuut_right) && (!test_flow->hoodclosed_right)) // && (!test_flow->start_left))
			{
			test_flow->start_right = 1;
			}
#endif

#ifndef NORMAL_MODE
		if((test_flow->runuut_right) && !toggle && !test_flow->start_left)
			{
			test_flow->start_right = 1;
			toggle = 1;
			}
#endif 
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
	

//	sprintf(log_fn,"%s%s_%s_%s%s","c:\\short_log\\",DutParameter.strCustSn,DateStr(),time_help,".txt");   
	sprintf(log_fn,"%s%s_%s_%s%s","c:\\short_log\\",strSerial,DateStr(),time_help,".log");   
	
	fa_w = fopen(log_fn,"a");

    if(fa_w)
    {
        fprintf(fa_w,"FCT_RFP,");
        fprintf(fa_w,"%s,",strSerial);
        fprintf(fa_w,"%s,%s,",DateStr(),TimeStr());
        fprintf(fa_w,"%s\n",resultStr);
        fclose(fa_w);
    }
     return;
}



/* FUNCTION ***************************************************/            
 void disconnect(int mode)
{
	return;
}


/* FUNCTION ***************************************************/            
int io_error (struct test_flow *test_flow)
{
char temp_str[100];
unsigned char port = 0;	
FILE *fa_w;

	(void)ipRfp_libssh_Close();
// close socket connection
	(void)ipRfp_rsx_Close();  
	CMD60_Reset (gpib_handle.cmd60);
	globals->executing = 0;
	if(test_flow->running_left)
		{
		iBarCodeLeft = 0;
		sprintf(temp_str,"-RUNTIME-ERROR-\nTest:%s, \nErrorcode: 0x%x ",test_flow->test_name,test_flow->error_code);
		set_statuswindow(temp_str,COL_NEUTRAL,LEFT);
		test_flow->ioerror_left = 1;
		//wait for hood-open:	
		set_hintwindow("Please open hood !! ",COL_NEUTRAL,LEFT);
		while(!(port & HOODLEFT))
			{
			port = MiniSys_get_IN();
			ProcessSystemEvents();
			Sleep(100);      // 100ms
			}
		test_flow->running_left = 0;
		}
	else
		{
		iBarCodeRight = 0;
		sprintf(temp_str,"RUNTIME-ERROR-\nTest:%s, \nErrorcode: 0x%x ",test_flow->test_name,test_flow->error_code);
		set_statuswindow(temp_str,COL_NEUTRAL,RIGHT);
		test_flow->ioerror_right = 1;
		//wait for hood-open:	
		set_hintwindow("Please open hood !! ",COL_NEUTRAL,RIGHT);
		while(!(port & HOODRIGHT))
			{
			port = MiniSys_get_IN();
			ProcessSystemEvents();
			Sleep(100);      // 100ms
			}
		test_flow->running_right = 0;
		}
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

/* FUNCTION ***************************************************/            
int GetBarCode(char* strBarCode, char* strSerial,int site)
{
		iCheckOkay = 0;
		DisplayPanel (BarCode_Label);		// setzt merker zurück & displays panel auf zugehöriger seite
		SetActivePanel (BarCode_Label);

		SetCtrlVal (BarCode_Label, BARCODE_InputBarCode, "");
		SetCtrlVal (BarCode_Label, BARCODE_InputSerial, "");
		//SetActiveCtrl (BarCode_Label, InputBarCode);
		while(iCheckOkay != 0x752) //is set from okay-button
				ProcessSystemEvents();
				
		GetCtrlVal (BarCode_Label, BARCODE_InputBarCode, strMAC);
		GetCtrlVal (BarCode_Label, BARCODE_InputSerial, strSerial);
		HidePanel (BarCode_Label);
		
return (0);
}
