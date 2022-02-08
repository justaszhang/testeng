#include <rs232.h>
#include <userint.h>
#include <formatio.h>
#include <utility.h>
#include <ansi_c.h>
#include "FCT Functions.h"
#include "Common Card FCT.h"
#include "Front Panel1.h"


//int VT100_Filter = 1;
int panel;
int VT100_Flag = 0;
int lm_sensors_flag = 0;


/*****************************************/
/*
/*
/*****************************************/
void Get_Start_Time(int panel)
{
 double dummy;
 char msg[256],buffer[512], *Date, *Time; 
 int buff_size;

	Date = DateStr ();
	Time = TimeStr ();
	sprintf(Start_Date_Time, "%s,%s ", Date, Time); // store start date and time for TRACS
 
	start_of_test_time = Timer (); 


 	sprintf(msg, "timer start value is: %f\n", start_of_test_time);


return;
}


/*****************************************/
/*
/*
/*****************************************/
int print_TRACS_results(panel)
{

char msg[256]; 
char fault_message [10*MAXLINE+256];
char fault_buffer [10*MAXLINE];
char *Date; 
char *Time; 
char fail_state[128]; 
char Stop_Date_Time[256]; 
char *sptr;
int card;
int i;
int count; 
int buff_size;
int H_TRACS;
int size; 
double test_time = 0;  

	Date = DateStr ();
	Time = TimeStr ();
	sprintf(Stop_Date_Time, "%s,%s ", Date, Time);
	
	
	end_of_test_time = Timer();

	test_time =  end_of_test_time - start_of_test_time;
	sprintf(test_time_2,"%1f Seconds", test_time);

	
	
	if(Test_Type == 0){
	SetDir("MITEL LOGS");
	if(GetFileInfo ("612d_CCA.TXT", &size)){ 
				H_TRACS = OpenFile ("612d_CCA.TXT", VAL_READ_WRITE, VAL_APPEND,VAL_ASCII);
			


				fault_buffer[0] = '\x0'; 

				if(Terminated_Early)
				{
					sprintf(fail_state, "UUT_CANCEL");
				}
	
				else if(CARD_STATE == FAIL)
				{
					sprintf(fail_state, "UUT_FAIL");
				}
				else
				{
					sprintf(fail_state, "UUT_PASS");
				}
 	
				sprintf(fault_message,"%s,%s,%s,%s,%s,%s,%s,%s,%s\n",Part_Number,Scan_Serial,Scan_Final_Fixture_Serial,fail_state,
				Start_Date_Time,Stop_Date_Time,test_time_2,GUI_Revision,data_string); 

				buff_size = strlen(fault_message);		
				WriteFile (H_TRACS, fault_message, buff_size);

	  	

		CloseFile(H_TRACS);
		SetDir(".."); 
		}
		else{
			H_TRACS = OpenFile ("612d_CCA.TXT", VAL_READ_WRITE, VAL_APPEND,VAL_ASCII);
			//create title block
			sprintf(fault_message,"Part Number,Serial Number,Fixture Serial,Pass_Fail,Start Date,Start Time,Finish Date,Finish Time,Test Time,GUI_Revision,Test Mode, Error Counter, P1V8 Adjustment,"
					"P1V8 Voltage, Ref Clk, Ref Clk Adjustment, Ref Clk Freq, Branding Set, Branding Check, HW Release, Write IPEI, Write SN, Reset Test Flow, File System, Open CMD60, CMD60 Loopback," 
					"SW Version, Park Search, PLI_1, Park_1, Test Mode, Service State, Connect CMD60, NTP, POS EU Deviation, NEG EU Deviation, Freq Offeset, Freq Drift, RSSI 45dBm, RSSI 75 dBm,"
					"RSSI K Factor, RSSI Offset, Receiver Sensitivity, Close CMD60, EEPROM LED, EEPROM Date, EEPROM PASS, Make Backup, Power Off\n");
			buff_size = strlen(fault_message);		
			WriteFile (H_TRACS, fault_message, buff_size);
	
					// format the fault string

				fault_buffer[0] = '\x0'; 

				if(Terminated_Early)
				{
					sprintf(fail_state, "UUT_CANCEL");
				}
	
				else if(CARD_STATE == FAIL)
				{
					sprintf(fail_state, "UUT_FAIL");
				}
				else
				{
					sprintf(fail_state, "UUT_PASS");
				}
				//sprintf(fault_message,"/* %s\t%s\t%s\t%s\t%s\t%s\t */\n", Part_Number, Scan_Serial, fail_state,
				//Start_Date_Time,Stop_Date_Time,data_string);  	
				sprintf(fault_message,"%s,%s,%s,%s,%s,%s,%s,%s,%s\n",Part_Number,Scan_Serial,Scan_Final_Fixture_Serial,fail_state,
				Start_Date_Time,Stop_Date_Time,test_time_2,GUI_Revision,data_string); 
			   // serial psc PASS/FAIL start date and time, stop date and time, failures
				buff_size = strlen(fault_message);		
				WriteFile (H_TRACS, fault_message, buff_size);

	  	

		CloseFile(H_TRACS);
		SetDir(".."); 
	
		}
}
	
	if(Test_Type == 1){
	SetDir("MITEL LOGS");
	if(GetFileInfo ("622d_CCA.TXT", &size)){ 
				H_TRACS = OpenFile ("622d_CCA.TXT", VAL_READ_WRITE, VAL_APPEND,VAL_ASCII);
			


				fault_buffer[0] = '\x0'; 

				if(Terminated_Early)
				{
					sprintf(fail_state, "UUT_CANCEL");
				}
	
				else if(CARD_STATE == FAIL)
				{
					sprintf(fail_state, "UUT_FAIL");
				}
				else
				{
					sprintf(fail_state, "UUT_PASS");
				}
 	
				sprintf(fault_message,"%s,%s,%s,%s,%s,%s,%s,%s,%s\n",Part_Number,Scan_Serial,Scan_Final_Fixture_Serial,fail_state,
				Start_Date_Time,Stop_Date_Time,test_time_2,GUI_Revision,data_string); 

				buff_size = strlen(fault_message);		
				WriteFile (H_TRACS, fault_message, buff_size);

	  	

		CloseFile(H_TRACS);
		SetDir(".."); 
		}
		else{
			H_TRACS = OpenFile ("622d_CCA.TXT", VAL_READ_WRITE, VAL_APPEND,VAL_ASCII);
			//create title block
			sprintf(fault_message,"Part Number,Serial Number,Fixture Serial,Pass_Fail,Start Date,Start Time,Finish Date,Finish Time,Test Time,GUI_Revision,Test Mode, Error Counter, P1V8 Adjustment,"
					"P1V8 Voltage, Ref Clk, Ref Clk Adjustment, Ref Clk Freq, Branding Set, Branding Check, HW Release, Write IPEI, Write SN, Reset Test Flow, File System, SD Card, Open CMD60, CMD60 Loopback," 
					"SW Version, Park Search, PLI_1, Park_1, Test Mode, Service State, Connect CMD60, NTP, POS EU Deviation, NEG EU Deviation, Freq Offeset, Freq Drift, RSSI 45dBm, RSSI 75 dBm,"
					"RSSI K Factor, RSSI Offset, Receiver Sensitivity, Close CMD60, Backup, Read MAC, Test Mode, Bluetooth Test Mode, Bluetooth Test Mode, Open MT8852B, Sync MT8852B, MT8852B Loopback Mode," 
					"MT8852B Scripts, BT Output Power, BT Mod Index, BT Frequency Tolerance, BT Frequency Drift, BT Slot Sensitivity, BT Sync Clock, BT Get Trim, BT Write Trim, Close MT8852B,"
					"Write MAC, EEPROM LED, EEPROM HOTKEY, EEPROM Vibrator, EEPROM Date, EEPROM PASS, Make Backup, Power Off\n");
			buff_size = strlen(fault_message);		
			WriteFile (H_TRACS, fault_message, buff_size);
	
					// format the fault string

				fault_buffer[0] = '\x0'; 

				if(Terminated_Early)
				{
					sprintf(fail_state, "UUT_CANCEL");
				}
	
				else if(CARD_STATE == FAIL)
				{
					sprintf(fail_state, "UUT_FAIL");
				}
				else
				{
					sprintf(fail_state, "UUT_PASS");
				}
				//sprintf(fault_message,"/* %s\t%s\t%s\t%s\t%s\t%s\t */\n", Part_Number, Scan_Serial, fail_state,
				//Start_Date_Time,Stop_Date_Time,data_string);  	
				sprintf(fault_message,"%s,%s,%s,%s,%s,%s,%s,%s,%s\n",Part_Number,Scan_Serial,Scan_Final_Fixture_Serial,fail_state,
				Start_Date_Time,Stop_Date_Time,test_time_2,GUI_Revision,data_string); 
			   // serial psc PASS/FAIL start date and time, stop date and time, failures
				buff_size = strlen(fault_message);		
				WriteFile (H_TRACS, fault_message, buff_size);

	  	

		CloseFile(H_TRACS);
		SetDir(".."); 
	
		}
}


if(Test_Type == 2){
	SetDir("MITEL LOGS");
	if(GetFileInfo ("632d_CCA.TXT", &size)){ 
				H_TRACS = OpenFile ("632d_CCA.TXT", VAL_READ_WRITE, VAL_APPEND,VAL_ASCII);
			


				fault_buffer[0] = '\x0'; 

				if(Terminated_Early)
				{
					sprintf(fail_state, "UUT_CANCEL");
				}
	
				else if(CARD_STATE == FAIL)
				{
					sprintf(fail_state, "UUT_FAIL");
				}
				else
				{
					sprintf(fail_state, "UUT_PASS");
				}
 	
				sprintf(fault_message,"%s,%s,%s,%s,%s,%s,%s,%s,%s\n",Part_Number,Scan_Serial,Scan_Final_Fixture_Serial,fail_state,
				Start_Date_Time,Stop_Date_Time,test_time_2,GUI_Revision,data_string); 

				buff_size = strlen(fault_message);		
				WriteFile (H_TRACS, fault_message, buff_size);

	  	

		CloseFile(H_TRACS);
		SetDir(".."); 
		}
		else{
			H_TRACS = OpenFile ("632d_CCA.TXT", VAL_READ_WRITE, VAL_APPEND,VAL_ASCII);
			//create title block
			sprintf(fault_message,"Part Number,Serial Number,Fixture Serial,Pass_Fail,Start Date,Start Time,Finish Date,Finish Time,Test Time,GUI_Revision,Test Mode, Error Counter, P1V8 Adjustment,"
					"P1V8 Voltage, Ref Clk, Ref Clk Adjustment, Ref Clk Freq, Branding Set, Branding Check, HW Release, Man Down,Write IPEI, Write SN, Reset Test Flow, File System, SD Card, Open CMD60, CMD60 Loopback," 
					"SW Version, Park Search, PLI_1, Park_1, Test Mode, Service State, Connect CMD60, NTP, POS EU Deviation, NEG EU Deviation, Freq Offeset, Freq Drift, RSSI 45dBm, RSSI 75 dBm,"
					"RSSI K Factor, RSSI Offset, Receiver Sensitivity, Close CMD60, Backup, Read MAC, Test Mode, Bluetooth Test Mode, Bluetooth Test Mode, Open MT8852B, Sync MT8852B, MT8852B Loopback Mode," 
					"MT8852B Scripts, BT Output Power, BT Mod Index, BT Frequency Tolerance, BT Frequency Drift, BT Slot Sensitivity, BT Sync Clock, BT Get Trim, BT Write Trim, Close MT8852B,"
					"Write MAC, EEPROM LED, EEPROM SOS, EEPROM HOTKEY, EEPROM Vibrator, EEPROM Loudness, EEPROM Date, EEPROM PASS, Make Backup, Power Off\n");
			buff_size = strlen(fault_message);		
			WriteFile (H_TRACS, fault_message, buff_size);
	
					// format the fault string

				fault_buffer[0] = '\x0'; 

				if(Terminated_Early)
				{
					sprintf(fail_state, "UUT_CANCEL");
				}
	
				else if(CARD_STATE == FAIL)
				{
					sprintf(fail_state, "UUT_FAIL");
				}
				else
				{
					sprintf(fail_state, "UUT_PASS");
				}
				//sprintf(fault_message,"/* %s\t%s\t%s\t%s\t%s\t%s\t */\n", Part_Number, Scan_Serial, fail_state,
				//Start_Date_Time,Stop_Date_Time,data_string);  	
				sprintf(fault_message,"%s,%s,%s,%s,%s,%s,%s,%s,%s\n",Part_Number,Scan_Serial,Scan_Final_Fixture_Serial,fail_state,
				Start_Date_Time,Stop_Date_Time,test_time_2,GUI_Revision,data_string); 
			   // serial psc PASS/FAIL start date and time, stop date and time, failures
				buff_size = strlen(fault_message);		
				WriteFile (H_TRACS, fault_message, buff_size);

	  	

		CloseFile(H_TRACS);
		SetDir(".."); 
	
		}
}


if(Test_Type == 3){
	SetDir("MITEL LOGS");
	if(GetFileInfo ("612d_FINAL.TXT", &size)){ 
				H_TRACS = OpenFile ("612d_FINAL.TXT", VAL_READ_WRITE, VAL_APPEND,VAL_ASCII);
			


				fault_buffer[0] = '\x0'; 

				if(Terminated_Early)
				{
					sprintf(fail_state, "UUT_CANCEL");
				}
	
				else if(CARD_STATE == FAIL)
				{
					sprintf(fail_state, "UUT_FAIL");
				}
				else
				{
					sprintf(fail_state, "UUT_PASS");
				}
 	
				sprintf(fault_message,"%s,%s,%s,%s,%s,%s,%s,%s,%s\n",Part_Number,Scan_Serial,Scan_Final_Fixture_Serial,fail_state,
				Start_Date_Time,Stop_Date_Time,test_time_2,GUI_Revision,data_string); 

				buff_size = strlen(fault_message);		
				WriteFile (H_TRACS, fault_message, buff_size);

	  	

		CloseFile(H_TRACS);
		SetDir(".."); 
		}
		else{
			H_TRACS = OpenFile ("612d_FINAL.TXT", VAL_READ_WRITE, VAL_APPEND,VAL_ASCII);
			//create title block
			sprintf(fault_message,"Part Number,Serial Number,Fixture Serial,Pass_Fail,Start Date,Start Time,Finish Date,Finish Time,Test Time,GUI_Revision,Test Mode, Error Counter, Reset Test Flow, READ SN, SN Check, READ IPEI, IPEI Check, KEYB LED, KEY12, LCD POS,"
					"LCD RED, LCD GREEN, LCD BLUE, LED RED, LED GREEN, LED ORANGE, Audio Headset, Audio MIC, Audio Speaker, Audio EP, Open CMD60, CMD60 Loopback,"
					"SW Version, Park Search, PLI_1, Park_1, Test Mode, Service State, Connect CMD60, NTP, Close CMD60, Write Date, Write Pass, Make Backup, Master Reset, Power Off\n");
			buff_size = strlen(fault_message);		
			WriteFile (H_TRACS, fault_message, buff_size);
	
					// format the fault string

				fault_buffer[0] = '\x0'; 

				if(Terminated_Early)
				{
					sprintf(fail_state, "UUT_CANCEL");
				}
	
				else if(CARD_STATE == FAIL)
				{
					sprintf(fail_state, "UUT_FAIL");
				}
				else
				{
					sprintf(fail_state, "UUT_PASS");
				}
				//sprintf(fault_message,"/* %s\t%s\t%s\t%s\t%s\t%s\t */\n", Part_Number, Scan_Serial, fail_state,
				//Start_Date_Time,Stop_Date_Time,data_string);  	
				sprintf(fault_message,"%s,%s,%s,%s,%s,%s,%s,%s,%s\n",Part_Number,Scan_Serial,Scan_Final_Fixture_Serial,fail_state,
				Start_Date_Time,Stop_Date_Time,test_time_2,GUI_Revision,data_string); 
			   // serial psc PASS/FAIL start date and time, stop date and time, failures
				buff_size = strlen(fault_message);		
				WriteFile (H_TRACS, fault_message, buff_size);

	  	

		CloseFile(H_TRACS);
		SetDir(".."); 
	
		}
}


if(Test_Type == 4){
	SetDir("MITEL LOGS");
	if(GetFileInfo ("622d_FINAL.TXT", &size)){ 
				H_TRACS = OpenFile ("622d_FINAL.TXT", VAL_READ_WRITE, VAL_APPEND,VAL_ASCII);
			


				fault_buffer[0] = '\x0'; 

				if(Terminated_Early)
				{
					sprintf(fail_state, "UUT_CANCEL");
				}
	
				else if(CARD_STATE == FAIL)
				{
					sprintf(fail_state, "UUT_FAIL");
				}
				else
				{
					sprintf(fail_state, "UUT_PASS");
				}
 	
				sprintf(fault_message,"%s,%s,%s,%s,%s,%s,%s,%s,%s\n",Part_Number,Scan_Serial,Scan_Final_Fixture_Serial,fail_state,
				Start_Date_Time,Stop_Date_Time,test_time_2,GUI_Revision,data_string); 

				buff_size = strlen(fault_message);		
				WriteFile (H_TRACS, fault_message, buff_size);

	  	

		CloseFile(H_TRACS);
		SetDir(".."); 
		}
		else{
			H_TRACS = OpenFile ("622d_FINAL.TXT", VAL_READ_WRITE, VAL_APPEND,VAL_ASCII);
			//create title block
			sprintf(fault_message,"Part Number,Serial Number,Fixture Serial,Pass_Fail,Start Date,Start Time,Finish Date,Finish Time,Test Time,GUI_Revision,Test Mode, Error Counter, Reset Test Flow, READ SN, SN Check, READ IPEI, IPEI Check, Read MAC, MAC Check, KEYB LED, KEY22, LCD POS,"
					"LCD RED, LCD GREEN, LCD BLUE, LED RED, LED GREEN, LED ORANGE, SD CARD, Audio Headset, Audio MIC, Audio Speaker, Audio EP, Audio Trembler, Open CMD60, CMD60 Loopback,"
					"SW Version, Park Search, PLI_1, Park_1, Test Mode, Service State, Connect CMD60, NTP, Close CMD60, Read MAC, Test Mode, BT Test Mode, BT Test Mode, Open MT8852B, Sync MT8852B,"
					"MT8852B Loopback, Run Scripts MT8852B, Power MT8852B, Close MT8852B, Disable BT, Write Date, Write Pass, Make Backup, Master Reset, Power Off\n");
			buff_size = strlen(fault_message);		
			WriteFile (H_TRACS, fault_message, buff_size);
	
					// format the fault string

				fault_buffer[0] = '\x0'; 

				if(Terminated_Early)
				{
					sprintf(fail_state, "UUT_CANCEL");
				}
	
				else if(CARD_STATE == FAIL)
				{
					sprintf(fail_state, "UUT_FAIL");
				}
				else
				{
					sprintf(fail_state, "UUT_PASS");
				}
				//sprintf(fault_message,"/* %s\t%s\t%s\t%s\t%s\t%s\t */\n", Part_Number, Scan_Serial, fail_state,
				//Start_Date_Time,Stop_Date_Time,data_string);  	
				sprintf(fault_message,"%s,%s,%s,%s,%s,%s,%s,%s,%s\n",Part_Number,Scan_Serial,Scan_Final_Fixture_Serial,fail_state,
				Start_Date_Time,Stop_Date_Time,test_time_2,GUI_Revision,data_string); 
			   // serial psc PASS/FAIL start date and time, stop date and time, failures
				buff_size = strlen(fault_message);		
				WriteFile (H_TRACS, fault_message, buff_size);

	  	

		CloseFile(H_TRACS);
		SetDir(".."); 
	
		}
}

if(Test_Type == 5){
	SetDir("MITEL LOGS");
	if(GetFileInfo ("632d_FINAL.TXT", &size)){ 
				H_TRACS = OpenFile ("632d_FINAL.TXT", VAL_READ_WRITE, VAL_APPEND,VAL_ASCII);
			


				fault_buffer[0] = '\x0'; 

				if(Terminated_Early)
				{
					sprintf(fail_state, "UUT_CANCEL");
				}
	
				else if(CARD_STATE == FAIL)
				{
					sprintf(fail_state, "UUT_FAIL");
				}
				else
				{
					sprintf(fail_state, "UUT_PASS");
				}
 	
				sprintf(fault_message,"%s,%s,%s,%s,%s,%s,%s,%s,%s\n",Part_Number,Scan_Serial,Scan_Final_Fixture_Serial,fail_state,
				Start_Date_Time,Stop_Date_Time,test_time_2,GUI_Revision,data_string); 

				buff_size = strlen(fault_message);		
				WriteFile (H_TRACS, fault_message, buff_size);

	  	

		CloseFile(H_TRACS);
		SetDir(".."); 
		}
		else{
			H_TRACS = OpenFile ("632d_FINAL.TXT", VAL_READ_WRITE, VAL_APPEND,VAL_ASCII);
			//create title block
			sprintf(fault_message,"Part Number,Serial Number,Fixture Serial,Pass_Fail,Start Date,Start Time,Finish Date,Finish Time,Test Time,GUI_Revision,Test Mode, Error Counter, Reset Test Flow, READ SN, SN Check, READ IPEI, IPEI Check, READ MAC, MAC Check,KEYB LED, KEY33, LCD POS,"
					"LCD RED, LCD GREEN, LCD BLUE, LED RED, LED GREEN, LED ORANGE, SD CARD, Audio Headset, Audio MIC, Audio Speaker, Audio EP, Audio Trembler, Open CMD60, CMD60 Loopback,"
					"SW Version, Park Search, PLI_1, Park_1, Test Mode, Service State, Connect CMD60, NTP, Close CMD60, Read MAC, Test Mode, BT Test Mode, BT Test Mode, Open MT8852B, Sync MT8852B,"
					"MT8852B Loopback, Run Scripts MT8852B, Power MT8852B, Close MT8852B, Disable BT,Write Date, Write Pass, Make Backup, Master Reset, Power Off\n");
			buff_size = strlen(fault_message);		
			WriteFile (H_TRACS, fault_message, buff_size);
	
					// format the fault string

				fault_buffer[0] = '\x0'; 

				if(Terminated_Early)
				{
					sprintf(fail_state, "UUT_CANCEL");
				}
	
				else if(CARD_STATE == FAIL)
				{
					sprintf(fail_state, "UUT_FAIL");
				}
				else
				{
					sprintf(fail_state, "UUT_PASS");
				}
				//sprintf(fault_message,"/* %s\t%s\t%s\t%s\t%s\t%s\t */\n", Part_Number, Scan_Serial, fail_state,
				//Start_Date_Time,Stop_Date_Time,data_string);  	
				sprintf(fault_message,"%s,%s,%s,%s,%s,%s,%s,%s,%s\n",Part_Number,Scan_Serial,Scan_Final_Fixture_Serial,fail_state,
				Start_Date_Time,Stop_Date_Time,test_time_2,GUI_Revision,data_string); 
			   // serial psc PASS/FAIL start date and time, stop date and time, failures
				buff_size = strlen(fault_message);		
				WriteFile (H_TRACS, fault_message, buff_size);

	  	

		CloseFile(H_TRACS);
		SetDir(".."); 
	
		}
}
	

return 1;

}

/*****************************************/
/*
/*
/*****************************************/
int Open_Files_For_Write(int panel)
{


int size, i;
char msg[128];
char *timeData = "         ";
char log_path[256];
char projectDir[MAX_PATHNAME_LEN];
char UUT_Type[32];
char serial_num_time[128];
char *date;
char tftp_BT_MAC_path[MAX_PATHNAME_LEN];  

		 	
            CopyString(timeData,0, TimeStr(), 0, 2);
    		strcat(timeData,"-");
    		CopyString(timeData,3, TimeStr(), 3, 2); 
    		strcat(timeData,"-");
    		CopyString(timeData,6, TimeStr(), 6, 2);  
			
			memset(log_file_debug,'\0', sizeof(log_file_debug));   
			
	
			//DECT CCA Test
			if(Test_Type == 0){
				strcpy(log_file, Product_Serial_Global);
		    	strcat(log_file,"_");
				strcat(log_file,"DECT_612d_CCA"); 
				strcat(log_file,"_"); 
		    	strcat(log_file,DateStr()); 
		    	strcat(log_file,"_"); 
		    	strcat(log_file,timeData);
				strcat(log_file_debug,log_file);
				strcat(log_file_debug,"_debug.log");
				strcat(log_file,".log");
				
			
				//SetDir("CARD LOGS");
				sprintf(UUT_Type,"DECT_612d_CCA");
				GetProjectDir (projectDir);
				sprintf(log_path,"%s\\CARD LOGS\\%s",projectDir,UUT_Type); 
				MakeDir(log_path);
				date = DateStr();
				sprintf(serial_num_time,"%s_%s_%s",Product_Serial_Global,timeData,date); 
				//sprintf(log_path,"%s\\CARD LOGS\\%s\\%s",projectDir,UUT_Type,Scan_Serial); 
				sprintf(log_path,"%s\\CARD LOGS\\%s\\%s",projectDir,UUT_Type,serial_num_time);
				MakeDir(log_path);
				SetDir(log_path);
				
				
				
				if(GetFileInfo (log_file, &size))
				{   // file exists so append
					if((Log_File_Handle = OpenFile (log_file, VAL_READ_WRITE, VAL_APPEND,VAL_ASCII))<=0)
					{
						sprintf(msg, "Unable to open output file %s\n",log_file); 
						SetCtrlVal(panel,PANEL_Status_Window, msg);
						Write_Log_File (msg,panel);
					
						return 0;
					}
				
				}
				else 
				{   // open new file
					Log_File_Handle = OpenFile (log_file, VAL_WRITE_ONLY, VAL_OPEN_AS_IS, VAL_ASCII);

				}
				
				
				if(debug == 1){
					debug_File_Handle = OpenFile (log_file_debug, VAL_READ_WRITE, VAL_OPEN_AS_IS, VAL_ASCII);	
				}
				
				SetCtrlVal(panel,PANEL_Log_File_String,log_file);
				
				sprintf(global_log_path,"%s",log_path); 
				
				
		    	SetDir (projectDir);
			
			}
			//622d
			else if(Test_Type == 1){
				strcpy(log_file, Product_Serial_Global);
		    	strcat(log_file,"_");
				strcat(log_file,"DECT_622d_CCA"); 
				strcat(log_file,"_"); 
		    	strcat(log_file,DateStr()); 
		    	strcat(log_file,"_"); 
		    	strcat(log_file,timeData);
				strcat(log_file_results,log_file);
				strcat(log_file_results,"_results.log");
				strcat(log_file_debug,log_file);
				strcat(log_file_debug,"_debug.log");
				strcat(log_file,".log");
				
			
				//SetDir("CARD LOGS");
				sprintf(UUT_Type,"DECT_622d_CCA");
				GetProjectDir (projectDir);
				sprintf(log_path,"%s\\CARD LOGS\\%s",projectDir,UUT_Type); 
				MakeDir(log_path);
				date = DateStr();
				sprintf(serial_num_time,"%s_%s_%s",Product_Serial_Global,timeData,date); 
				//sprintf(log_path,"%s\\CARD LOGS\\%s\\%s",projectDir,UUT_Type,Scan_Serial); 
				sprintf(log_path,"%s\\CARD LOGS\\%s\\%s",projectDir,UUT_Type,serial_num_time);
				MakeDir(log_path);
				SetDir(log_path);
				
				
				
				if(GetFileInfo (log_file, &size))
				{   // file exists so append
					if((Log_File_Handle = OpenFile (log_file, VAL_READ_WRITE, VAL_APPEND,VAL_ASCII))<=0)
					{
						sprintf(msg, "Unable to open output file %s\n",log_file); 
						SetCtrlVal(panel,PANEL_Status_Window, msg);
						Write_Log_File (msg,panel);
					
						return 0;
					}
				
				}
				else 
				{   // open new file
					Log_File_Handle = OpenFile (log_file, VAL_WRITE_ONLY, VAL_OPEN_AS_IS, VAL_ASCII);

				}
				
				
				if(debug == 1){
					debug_File_Handle = OpenFile (log_file_debug, VAL_READ_WRITE, VAL_OPEN_AS_IS, VAL_ASCII);	
				}
				
				
				results_File_Handle = OpenFile (log_file_debug, VAL_READ_WRITE, VAL_OPEN_AS_IS, VAL_ASCII); 
				
				
				SetCtrlVal(panel,PANEL_Log_File_String,log_file);
				
				sprintf(global_log_path,"%s",log_path); 
				
				
		    	SetDir (projectDir);
			
			}
			//632d 
			if(Test_Type == 2){
				strcpy(log_file, Product_Serial_Global);
		    	strcat(log_file,"_");
				strcat(log_file,"DECT_632d_CCA"); 
				strcat(log_file,"_"); 
		    	strcat(log_file,DateStr()); 
		    	strcat(log_file,"_"); 
		    	strcat(log_file,timeData);
				strcat(log_file_debug,log_file);
				strcat(log_file_debug,"_debug.log");
				strcat(log_file,".log");
				
			
				//SetDir("CARD LOGS");
				sprintf(UUT_Type,"DECT_632d_CCA");
				GetProjectDir (projectDir);
				sprintf(log_path,"%s\\CARD LOGS\\%s",projectDir,UUT_Type); 
				MakeDir(log_path);
				date = DateStr();
				sprintf(serial_num_time,"%s_%s_%s",Product_Serial_Global,timeData,date); 
				//sprintf(log_path,"%s\\CARD LOGS\\%s\\%s",projectDir,UUT_Type,Scan_Serial); 
				sprintf(log_path,"%s\\CARD LOGS\\%s\\%s",projectDir,UUT_Type,serial_num_time);
				MakeDir(log_path);
				SetDir(log_path);
				
				
				
				if(GetFileInfo (log_file, &size))
				{   // file exists so append
					if((Log_File_Handle = OpenFile (log_file, VAL_READ_WRITE, VAL_APPEND,VAL_ASCII))<=0)
					{
						sprintf(msg, "Unable to open output file %s\n",log_file); 
						SetCtrlVal(panel,PANEL_Status_Window, msg);
						Write_Log_File (msg,panel);
					
						return 0;
					}
				
				}
				else 
				{   // open new file
					Log_File_Handle = OpenFile (log_file, VAL_WRITE_ONLY, VAL_OPEN_AS_IS, VAL_ASCII);

				}
				
				
				if(debug == 1){
					debug_File_Handle = OpenFile (log_file_debug, VAL_READ_WRITE, VAL_OPEN_AS_IS, VAL_ASCII);	
				}
				
				SetCtrlVal(panel,PANEL_Log_File_String,log_file);
				
				sprintf(global_log_path,"%s",log_path); 
				
				
		    	SetDir (projectDir);
			
			}
			//DECT 612d FINAL Test
			else if(Test_Type == 3){
				strcpy(log_file, Product_Serial_Global);
		    	strcat(log_file,"_");
				strcat(log_file,"DECT_612d_FINAL"); 
				strcat(log_file,"_"); 
		    	strcat(log_file,DateStr()); 
		    	strcat(log_file,"_"); 
		    	strcat(log_file,timeData);
				strcat(log_file_debug,log_file);
				strcat(log_file_debug,"_debug.log");
				strcat(log_file,".log");
			
			
				//SetDir("CARD LOGS");
				sprintf(UUT_Type,"DECT_612d_FINAL");
				GetProjectDir (projectDir);
				sprintf(log_path,"%s\\CARD LOGS\\%s",projectDir,UUT_Type); 
				MakeDir(log_path);
				date = DateStr();
				sprintf(serial_num_time,"%s_%s_%s",Product_Serial_Global,timeData,date); 
				//sprintf(log_path,"%s\\CARD LOGS\\%s\\%s",projectDir,UUT_Type,Scan_Serial); 
				sprintf(log_path,"%s\\CARD LOGS\\%s\\%s",projectDir,UUT_Type,serial_num_time);
				MakeDir(log_path);
				SetDir(log_path);
				
				
				
				if(GetFileInfo (log_file, &size))
				{   // file exists so append
					if((Log_File_Handle = OpenFile (log_file, VAL_READ_WRITE, VAL_APPEND,VAL_ASCII))<=0)
					{
						sprintf(msg, "Unable to open output file %s\n",log_file); 
						SetCtrlVal(panel,PANEL_Status_Window, msg);
						Write_Log_File (msg,panel);
					
						return 0;
					}
				
				}
				else 
				{   // open new file
					Log_File_Handle = OpenFile (log_file, VAL_WRITE_ONLY, VAL_OPEN_AS_IS, VAL_ASCII);

				}
				
				
				if(debug == 1){
					debug_File_Handle = OpenFile (log_file_debug, VAL_READ_WRITE, VAL_OPEN_AS_IS, VAL_ASCII);	
				}
				
				
				SetCtrlVal(panel,PANEL_Log_File_String,log_file);
				
				sprintf(global_log_path,"%s",log_path); 
				
		    	SetDir (projectDir);
			
			}
			//DECT 622d FINAL Test
			else if(Test_Type == 4){
				strcpy(log_file, Product_Serial_Global);
		    	strcat(log_file,"_");
				strcat(log_file,"DECT_622d_FINAL"); 
				strcat(log_file,"_"); 
		    	strcat(log_file,DateStr()); 
		    	strcat(log_file,"_"); 
		    	strcat(log_file,timeData);
				strcat(log_file_debug,log_file);
				strcat(log_file_debug,"_debug.log");
				strcat(log_file,".log");
			
			
				//SetDir("CARD LOGS");
				sprintf(UUT_Type,"DECT_622d_FINAL");
				GetProjectDir (projectDir);
				sprintf(log_path,"%s\\CARD LOGS\\%s",projectDir,UUT_Type); 
				MakeDir(log_path);
				date = DateStr();
				sprintf(serial_num_time,"%s_%s_%s",Product_Serial_Global,timeData,date); 
				//sprintf(log_path,"%s\\CARD LOGS\\%s\\%s",projectDir,UUT_Type,Scan_Serial); 
				sprintf(log_path,"%s\\CARD LOGS\\%s\\%s",projectDir,UUT_Type,serial_num_time);
				MakeDir(log_path);
				SetDir(log_path);
				
				
				
				if(GetFileInfo (log_file, &size))
				{   // file exists so append
					if((Log_File_Handle = OpenFile (log_file, VAL_READ_WRITE, VAL_APPEND,VAL_ASCII))<=0)
					{
						sprintf(msg, "Unable to open output file %s\n",log_file); 
						SetCtrlVal(panel,PANEL_Status_Window, msg);
						Write_Log_File (msg,panel);
					
						return 0;
					}
				
				}
				else 
				{   // open new file
					Log_File_Handle = OpenFile (log_file, VAL_WRITE_ONLY, VAL_OPEN_AS_IS, VAL_ASCII);

				}
				
				
				if(debug == 1){
					debug_File_Handle = OpenFile (log_file_debug, VAL_READ_WRITE, VAL_OPEN_AS_IS, VAL_ASCII);	
				}
				
				
				SetCtrlVal(panel,PANEL_Log_File_String,log_file);
				
				sprintf(global_log_path,"%s",log_path); 
				
		    	SetDir (projectDir);
			
			}
			//DECT 632d FINAL Test
			else if(Test_Type == 5){
				strcpy(log_file, Product_Serial_Global);
		    	strcat(log_file,"_");
				strcat(log_file,"DECT_632d_FINAL"); 
				strcat(log_file,"_"); 
		    	strcat(log_file,DateStr()); 
		    	strcat(log_file,"_"); 
		    	strcat(log_file,timeData);
				strcat(log_file_debug,log_file);
				strcat(log_file_debug,"_debug.log");
				strcat(log_file,".log");
			
			
				//SetDir("CARD LOGS");
				sprintf(UUT_Type,"DECT_632d_FINAL");
				GetProjectDir (projectDir);
				sprintf(log_path,"%s\\CARD LOGS\\%s",projectDir,UUT_Type); 
				MakeDir(log_path);
				date = DateStr();
				sprintf(serial_num_time,"%s_%s_%s",Product_Serial_Global,timeData,date); 
				//sprintf(log_path,"%s\\CARD LOGS\\%s\\%s",projectDir,UUT_Type,Scan_Serial); 
				sprintf(log_path,"%s\\CARD LOGS\\%s\\%s",projectDir,UUT_Type,serial_num_time);
				MakeDir(log_path);
				SetDir(log_path);
				
				
				
				if(GetFileInfo (log_file, &size))
				{   // file exists so append
					if((Log_File_Handle = OpenFile (log_file, VAL_READ_WRITE, VAL_APPEND,VAL_ASCII))<=0)
					{
						sprintf(msg, "Unable to open output file %s\n",log_file); 
						SetCtrlVal(panel,PANEL_Status_Window, msg);
						Write_Log_File (msg,panel);
					
						return 0;
					}
				
				}
				else 
				{   // open new file
					Log_File_Handle = OpenFile (log_file, VAL_WRITE_ONLY, VAL_OPEN_AS_IS, VAL_ASCII);

				}
				
				
				if(debug == 1){
					debug_File_Handle = OpenFile (log_file_debug, VAL_READ_WRITE, VAL_OPEN_AS_IS, VAL_ASCII);	
				}
				
				
				SetCtrlVal(panel,PANEL_Log_File_String,log_file);
				
				sprintf(global_log_path,"%s",log_path); 
				
		    	SetDir (projectDir);
			
			}
///////////////////////////////////////////////////////////////////////////////			
			
			//DECT 612d FINAL Test
			else if(Test_Type == 6){
				strcpy(log_file, Product_Serial_Global);
		    	strcat(log_file,"_");
				strcat(log_file,"DECT_612d_SN"); 
				strcat(log_file,"_"); 
		    	strcat(log_file,DateStr()); 
		    	strcat(log_file,"_"); 
		    	strcat(log_file,timeData);
				strcat(log_file_debug,log_file);
				strcat(log_file_debug,"_debug.log");
				strcat(log_file,".log");
			
			
				//SetDir("CARD LOGS");
				sprintf(UUT_Type,"DECT_612d_SN_FIX");
				GetProjectDir (projectDir);
				sprintf(log_path,"%s\\CARD LOGS\\%s",projectDir,UUT_Type); 
				MakeDir(log_path);
				date = DateStr();
				sprintf(serial_num_time,"%s_%s_%s",Product_Serial_Global,timeData,date); 
				//sprintf(log_path,"%s\\CARD LOGS\\%s\\%s",projectDir,UUT_Type,Scan_Serial); 
				sprintf(log_path,"%s\\CARD LOGS\\%s\\%s",projectDir,UUT_Type,serial_num_time);
				MakeDir(log_path);
				SetDir(log_path);
				
				
				
				if(GetFileInfo (log_file, &size))
				{   // file exists so append
					if((Log_File_Handle = OpenFile (log_file, VAL_READ_WRITE, VAL_APPEND,VAL_ASCII))<=0)
					{
						sprintf(msg, "Unable to open output file %s\n",log_file); 
						SetCtrlVal(panel,PANEL_Status_Window, msg);
						Write_Log_File (msg,panel);
					
						return 0;
					}
				
				}
				else 
				{   // open new file
					Log_File_Handle = OpenFile (log_file, VAL_WRITE_ONLY, VAL_OPEN_AS_IS, VAL_ASCII);

				}
				
				
				if(debug == 1){
					debug_File_Handle = OpenFile (log_file_debug, VAL_READ_WRITE, VAL_OPEN_AS_IS, VAL_ASCII);	
				}
				
				
				SetCtrlVal(panel,PANEL_Log_File_String,log_file);
				
				sprintf(global_log_path,"%s",log_path); 
				
		    	SetDir (projectDir);
			
			}
			//DECT 622d FINAL Test
			else if(Test_Type == 7){
				strcpy(log_file, Product_Serial_Global);
		    	strcat(log_file,"_");
				strcat(log_file,"DECT_622d_SN_FIX"); 
				strcat(log_file,"_"); 
		    	strcat(log_file,DateStr()); 
		    	strcat(log_file,"_"); 
		    	strcat(log_file,timeData);
				strcat(log_file_debug,log_file);
				strcat(log_file_debug,"_debug.log");
				strcat(log_file,".log");
			
			
				//SetDir("CARD LOGS");
				sprintf(UUT_Type,"DECT_622d_SN_FIX");
				GetProjectDir (projectDir);
				sprintf(log_path,"%s\\CARD LOGS\\%s",projectDir,UUT_Type); 
				MakeDir(log_path);
				date = DateStr();
				sprintf(serial_num_time,"%s_%s_%s",Product_Serial_Global,timeData,date); 
				//sprintf(log_path,"%s\\CARD LOGS\\%s\\%s",projectDir,UUT_Type,Scan_Serial); 
				sprintf(log_path,"%s\\CARD LOGS\\%s\\%s",projectDir,UUT_Type,serial_num_time);
				MakeDir(log_path);
				SetDir(log_path);
				
				
				
				if(GetFileInfo (log_file, &size))
				{   // file exists so append
					if((Log_File_Handle = OpenFile (log_file, VAL_READ_WRITE, VAL_APPEND,VAL_ASCII))<=0)
					{
						sprintf(msg, "Unable to open output file %s\n",log_file); 
						SetCtrlVal(panel,PANEL_Status_Window, msg);
						Write_Log_File (msg,panel);
					
						return 0;
					}
				
				}
				else 
				{   // open new file
					Log_File_Handle = OpenFile (log_file, VAL_WRITE_ONLY, VAL_OPEN_AS_IS, VAL_ASCII);

				}
				
				
				if(debug == 1){
					debug_File_Handle = OpenFile (log_file_debug, VAL_READ_WRITE, VAL_OPEN_AS_IS, VAL_ASCII);	
				}
				
				
				SetCtrlVal(panel,PANEL_Log_File_String,log_file);
				
				sprintf(global_log_path,"%s",log_path); 
				
		    	SetDir (projectDir);
			
			}
			//DECT 632d FINAL Test
			else if(Test_Type == 8){
				strcpy(log_file, Product_Serial_Global);
		    	strcat(log_file,"_");
				strcat(log_file,"DECT_632d_SN_FIX"); 
				strcat(log_file,"_"); 
		    	strcat(log_file,DateStr()); 
		    	strcat(log_file,"_"); 
		    	strcat(log_file,timeData);
				strcat(log_file_debug,log_file);
				strcat(log_file_debug,"_debug.log");
				strcat(log_file,".log");
			
			
				//SetDir("CARD LOGS");
				sprintf(UUT_Type,"DECT_632d_SN_FIX");
				GetProjectDir (projectDir);
				sprintf(log_path,"%s\\CARD LOGS\\%s",projectDir,UUT_Type); 
				MakeDir(log_path);
				date = DateStr();
				sprintf(serial_num_time,"%s_%s_%s",Product_Serial_Global,timeData,date); 
				//sprintf(log_path,"%s\\CARD LOGS\\%s\\%s",projectDir,UUT_Type,Scan_Serial); 
				sprintf(log_path,"%s\\CARD LOGS\\%s\\%s",projectDir,UUT_Type,serial_num_time);
				MakeDir(log_path);
				SetDir(log_path);
				
				
				
				if(GetFileInfo (log_file, &size))
				{   // file exists so append
					if((Log_File_Handle = OpenFile (log_file, VAL_READ_WRITE, VAL_APPEND,VAL_ASCII))<=0)
					{
						sprintf(msg, "Unable to open output file %s\n",log_file); 
						SetCtrlVal(panel,PANEL_Status_Window, msg);
						Write_Log_File (msg,panel);
					
						return 0;
					}
				
				}
				else 
				{   // open new file
					Log_File_Handle = OpenFile (log_file, VAL_WRITE_ONLY, VAL_OPEN_AS_IS, VAL_ASCII);

				}
				
				
				if(debug == 1){
					debug_File_Handle = OpenFile (log_file_debug, VAL_READ_WRITE, VAL_OPEN_AS_IS, VAL_ASCII);	
				}
				
				
				SetCtrlVal(panel,PANEL_Log_File_String,log_file);
				
				sprintf(global_log_path,"%s",log_path); 
				
		    	SetDir (projectDir);
			
			}	

return 1;

}

/*****************************************/
/*
/*
/*****************************************/
int Close_Data_Files(int panel)
{

int i, err, COLOR, LED;
char msg[512], fail_state[256];

 	CloseFile(Log_File_Handle);
	CloseFile(Audio_File_Handle);
	
return 1;
}

/*****************************************/
/*
/*
/*****************************************/
void Write_Log_File(char *msg, int panel)
{
int buff_size;
int status = 0;
char buffer[MAXLINE+512], *Date, *Time, *temp_char;
char temp_path[256]={'\0'};
int temp_int = 0;
char projectDir[MAX_PATHNAME_LEN];

			SetCtrlAttribute (panel, PANEL_LED, ATTR_ON_COLOR, VAL_DK_GREEN);
			SetCtrlAttribute (panel, PANEL_LED, ATTR_OFF_COLOR, VAL_BLACK);
			Date = DateStr ();
			Time = TimeStr ();
			temp_char = "BMS";
			SetDir(global_log_path); 
			//SetDir("CARD LOGS");
			temp_int = strcmp( Station_ID, temp_char );
			if(temp_int == 0)
				sprintf(buffer, "%s,%s,%s,%s,%s,%s", Station_ID, Part_Number, Product_Serial_Global, Date, Time, msg); 
			else
				sprintf(buffer, "%s,%s,%s,%s,%s", Station_ID,Product_Serial_Global,Date,Time,msg); 
			buff_size = strlen(buffer);		
			WriteFile (Log_File_Handle, buffer, buff_size);
			if(network_enable == 1)
			{	
				strcat(temp_path,Network_Path);
				strcat(temp_path,"\\"); 
				strcat(temp_path, log_file);
				status = CopyFile (log_file, temp_path);
				if (status == 0)
					SetCtrlAttribute (panel, PANEL_LED, ATTR_CTRL_VAL, 1);
				else
					SetCtrlAttribute (panel, PANEL_LED, ATTR_CTRL_VAL, 0);
			}
			//SetDir(".."); 
			GetProjectDir (projectDir); 
			SetDir (projectDir);
}


/*****************************************/
/*
/*
/*****************************************/
void Write_Audio_Log_File(char *msg, int panel)
{
int buff_size;
int status = 0;
char buffer[MAXLINE+512], *Date, *Time, *temp_char;
char temp_path[256]={'\0'};
int temp_int = 0;

			SetCtrlAttribute (panel, PANEL_LED, ATTR_ON_COLOR, VAL_DK_GREEN);
			SetCtrlAttribute (panel, PANEL_LED, ATTR_OFF_COLOR, VAL_BLACK);
			Date = DateStr ();
			Time = TimeStr ();
			temp_char = "BMS";
			//SetDir(".."); 
			SetDir("AUDIO LOGS");
			temp_int = strcmp( Station_ID, temp_char );
			if(temp_int == 0)
				sprintf(buffer, "%s,%s,%s,%s,%s,%s", Station_ID, Part_Number, Product_Serial_Global, Date, Time, msg); 
			else
				sprintf(buffer, "%s,%s,%s,%s,%s", Station_ID,Product_Serial_Global,Date,Time,msg); 
			buff_size = strlen(buffer);		
			WriteFile (Audio_File_Handle, buffer, buff_size);
			if(network_enable == 1)
			{	
				strcat(temp_path,Network_Path);
				strcat(temp_path,"\\"); 
				strcat(temp_path, log_file);
				status = CopyFile (log_file, temp_path);
				if (status == 0)
					SetCtrlAttribute (panel, PANEL_LED, ATTR_CTRL_VAL, 1);
				else
					SetCtrlAttribute (panel, PANEL_LED, ATTR_CTRL_VAL, 0);
			}
			SetDir(".."); 
}


/*****************************************/
/*
/*
/*****************************************/
int Build_Menu(int panel)
{
  int File_Handle, count, index, value, err;

  
  
  			err = InsertListItem (panel, PANEL_RING, 0, "DECT 612d CCA Test", 0);
			err = InsertListItem (panel, PANEL_RING, 1, "DECT 622d CCA Test", 1); 
			err = InsertListItem (panel, PANEL_RING, 2, "DECT 632d CCA Test", 2); 
			err = InsertListItem (panel, PANEL_RING, 3, "DECT 612d Final Test", 3);  
			err = InsertListItem (panel, PANEL_RING, 4, "DECT 622d Final Test", 4); 
			err = InsertListItem (panel, PANEL_RING, 5, "DECT 632d Final Test", 5); 
			
			//err = InsertListItem (panel, PANEL_RING, 6, "DECT 612d Final S/N Fix", 6);  
			//err = InsertListItem (panel, PANEL_RING, 7, "DECT 622d Final S/N Fix", 7);
			//err = InsertListItem (panel, PANEL_RING, 8, "DECT 632d Final S/N Fix", 8);
			
			//////////////////////
			/*err = InsertListItem (panel, PANEL_RING, 6, "DECT Enhanced Tests USB", 6);  
			err = InsertListItem (panel, PANEL_RING, 7, "DECT Enhanced User Tests USB", 7);
			err = InsertListItem (panel, PANEL_RING, 8, "DECT Enhanced Version Tests USB", 8);
			err = InsertListItem (panel, PANEL_RING, 9, "DECT Enhanced Tests UART", 9);  
			err = InsertListItem (panel, PANEL_RING, 10, "DECT Enhanced User Tests UART", 10);
			err = InsertListItem (panel, PANEL_RING, 11, "DECT Enhanced Version Tests UART", 11);
			*/						
			err = SetCtrlIndex (panel, PANEL_RING, 0);


			
			err = InsertListItem (panel, PANEL_RING_2, 0, "8", 8);
			err = InsertListItem (panel, PANEL_RING_2, 1, "9", 9);
			err = InsertListItem (panel, PANEL_RING_2, 2, "10", 10);
			err = InsertListItem (panel, PANEL_RING_2, 3, "11", 11);
			err = InsertListItem (panel, PANEL_RING_2, 4, "12", 12);
			err = InsertListItem (panel, PANEL_RING_2, 5, "13", 13);
			err = InsertListItem (panel, PANEL_RING_2, 6, "14", 14); 
			err = InsertListItem (panel, PANEL_RING_2, 7, "15", 15); 
			err = SetCtrlIndex (panel, PANEL_RING_2, 4);
			

return 1;
}




void save_panel(int in_panel)
{
	
panel =	in_panel;


}	




//*************************************************************
//* 
//*
//*************************************************************
int DECT_612d_CCA_Test_Sequence(int panel, char *test_number)
{
	char  buffer[MAXLINE+1] = {'\0'},  msg_buffer[MAXLINE+128];  
	double delay_time = 600;
	int status = 0;
	int timeout = 0;
	int i = 0;
	char msg[256] = {'\0'};

	
	sprintf(buffer,"Starting DECT CCA Tests\n\n");  
	SetCtrlVal (panel, PANEL_Serial_Window_1, buffer);


	if(Power_ON_DECT () != 0){
		sprintf(test_number, "DECT_Power"); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1) 
		return 0; 
	
	
	if(Open_COM_Port_Relays () != 0){
		sprintf(test_number, "COM_PORT_Relays"); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1) 
		return 0; 
	
	
	Delay(15);
	
	//Start Tests
	if(DECT_Enter_Test_Mode (panel) != 1){
		sprintf(test_number, "Enter_Test_Mode"); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_Read_Error_Counter_Test (panel) != 1){
		sprintf(test_number, "Read_Error_Counter_Test"); 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
/*	if(Error_Counter != 0){
		if(DECT_Master_Reset_Test (panel) != 1){
			sprintf(test_number, "Master_Reset_Test"); 
			return 0;
		} 
		
		
		if(DECT_Power_Off_Test (panel) != 1){
			sprintf(test_number, "Power_Off_Test"); 
			return 0;
		}
	
		ProcessSystemEvents ();
		if (Stop_Test_Exe == 1) 
			return 0; 
	
	
		if(Close_COM_Port_Relays () != 0){
			sprintf(test_number, "COM_PORT_Relays"); 
			return 0;
		}
	
		return 0;
		
	}*/
	
	
	if(DECT_P1V8_Voltage_Adjustment_Test (panel) != 1){
		sprintf(test_number, "P1V8_Adjust");
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_Activate_Ref_Clk_Test (panel) != 1){
		sprintf(test_number, "Activate_Ref_Clk");
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_Set_Clk_Test (panel) != 1){
		sprintf(test_number, "Set_Ref_Clk");
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	

	if(DECT_Branding_Set_Test (panel) != 1){
		sprintf(test_number, "Branding_Set_Test");
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_Branding_Check_Test (panel) != 1){
		sprintf(test_number, "Branding_Check_Test");
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	

	if(DECT_Write_HW_Release_Test (panel) != 1){
		sprintf(test_number, "HW_Release_Test");
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_Write_IPEI_Test (panel) != 1){
		sprintf(test_number, "IPEI_Mode"); 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	} 
	
	
	if(DECT_Write_Serial_Number_Test (panel) != 1){
		sprintf(test_number, "Serial_Number");
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	} 
	
	if(DECT_Reset_EEPROM_Test_Flow_Test (panel) != 1){
		sprintf(test_number, "Test_Flow_Test");
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_File_System_Test (panel) != 1){
		sprintf(test_number, "File_System_Test");
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	} 
	
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	} 
	
	if(Open_CMD60 (panel) != 1){
		sprintf(test_number, "Open_CMD60");
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	} 
	
	
	if(Set_CMD60_in_Loopback_Mode_EU_Setting (panel) != 1){
		sprintf(test_number, "CMD60_Loopback");
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	} 
	

	if(DECT_Get_SW_Version_Test (panel) != 1){
		sprintf(test_number, "SW_Version_Test");
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_Park_Search_Criteria_Test (panel) != 1){
		sprintf(test_number, "Park_Search_Test");
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_EEPROM_PLI_1_Test  (panel) != 1){
		sprintf(test_number, "PLI_1_Test");
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_EEPROM_Park_1_Test  (panel) != 1){
		sprintf(test_number, "Park_1_Test");
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_Enter_Test_Mode (panel) != 1){
		sprintf(test_number, "Enter_Test_Mode");
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_Get_Service_State_Test(panel) != 1){
		sprintf(test_number, "Get_Service_State");
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	if(Setup_Connect_CMD60(panel) != 1){
		sprintf(test_number, "Setup_Connect_CMD60");
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	 
	if(DECT_Measure_NTP_Power_Test(panel) != 1){
		sprintf(test_number, "NTP_Power");
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_Measure_EU_Deviation_Test(panel) != 1){
		sprintf(test_number, "EU_Deviation");
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_Measure_Frequency_Offset_Test(panel) != 1){
		sprintf(test_number, "Freq_Offset");
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_Measure_Frequency_Drift_Test(panel) != 1){
		sprintf(test_number, "Freq_Drift");
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_Measure_RSSI_45dBm_Test(panel) != 1){
		sprintf(test_number, "RSSI_45dBm");
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_Measure_RSSI_75dBm_Test(panel) != 1){
		sprintf(test_number, "RSSI_75dBm");
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_Program_RSSI_K_Factor_Test(panel) != 1){
		sprintf(test_number, "RSSI_KFactor");
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_Program_RSSI_Offset_Test(panel) != 1){
		sprintf(test_number, "RSSI_Offset");
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_Measure_Receiver_Sensitivity_Test(panel) != 1){
		sprintf(test_number, "Receiver Sensitivity");
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	/*if(DECT_Power_Template_Match_Test(panel) != 1){
		sprintf(test_number, "Template Match"); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1) 
		return 0;
	*/
	
	
	if(Close_CMD60(panel) != 1){
		sprintf(test_number, "Close_CMD60");
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	
	if(DECT_EEPROM_LED_Test (panel) != 1){	   		//612d, 622d, 632d
		sprintf(test_number, "LED_Test");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	} 
	
	
	if(DECT_EEPROM_WRITE_DATE_Test (panel) != 1){
		sprintf(test_number, "Write_Date_Test");
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_EEPROM_WRITE_PASS_Test (panel) != 1){
		sprintf(test_number, "Write_PASS_Test");
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_Make_Backup_Test (panel) != 1){
		sprintf(test_number, "Make_Backup_Test");
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	} 
	

	if(DECT_Power_Off_Test (panel) != 1){
		sprintf(test_number, "Power_Off_Test"); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		return 0;
	} 
	
	
	if(Close_COM_Port_Relays () != 0){
		sprintf(test_number, "COM_PORT_Relays"); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		return 0;
	}
	

	CloseCom(COM_port);
	
	if(Power_OFF_DECT () != 0){
		sprintf(test_number, "DECT_Power"); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1) 
		return 0; 
	
	
  return 1;

}


//*************************************************************
//* 
//*
//*************************************************************
int DECT_622d_CCA_Test_Sequence(int panel, char *test_number)
{
	char  buffer[MAXLINE+1] = {'\0'},  msg_buffer[MAXLINE+128];  
	double delay_time = 600;
	int status = 0;
	int timeout = 0;
	int i = 0;
	char msg[256] = {'\0'};

	
	sprintf(buffer,"Starting DECT CCA Tests\n\n");  
	SetCtrlVal (panel, PANEL_Serial_Window_1, buffer);


	if(Power_ON_DECT () != 0){
		sprintf(test_number, "DECT_Power"); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1) 
		return 0; 
	
	
	if(Open_COM_Port_Relays () != 0){
		sprintf(test_number, "COM_PORT_Relays"); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1) 
		return 0; 
	
	
	Delay(15);
	//Delay(30);
	
	//Start Tests
	if(DECT_Enter_Test_Mode (panel) != 1){
		sprintf(test_number, "Enter_Test_Mode"); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_Read_Error_Counter_Test (panel) != 1){
		sprintf(test_number, "Read_Error_Counter_Test"); 
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	/*if(Error_Counter != 0){
		if(DECT_Master_Reset_Test (panel) != 1){
			sprintf(test_number, "Master_Reset_Test"); 
			return 0;
		} 
		
		
		if(DECT_Power_Off_Test (panel) != 1){
			sprintf(test_number, "Power_Off_Test"); 
			return 0;
		}
	
		ProcessSystemEvents ();
		if (Stop_Test_Exe == 1) 
			return 0; 
	
	
		if(Close_COM_Port_Relays () != 0){
			sprintf(test_number, "COM_PORT_Relays"); 
			return 0;
		}
	
		return 0;
		
	}*/
	
	if(DECT_P1V8_Voltage_Adjustment_Test (panel) != 1){
		sprintf(test_number, "P1V8_Adjust"); 
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	if(DECT_Activate_Ref_Clk_Test (panel) != 1){
		sprintf(test_number, "Activate_Ref_Clk");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	if(DECT_Set_Clk_Test (panel) != 1){
		sprintf(test_number, "Set_Ref_Clk");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	

	if(DECT_Branding_Set_Test (panel) != 1){
		sprintf(test_number, "Branding_Set_Test"); 
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_Branding_Check_Test (panel) != 1){
		sprintf(test_number, "Branding_Check_Test");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	

	if(DECT_Write_HW_Release_Test (panel) != 1){
		sprintf(test_number, "HW_Release_Test");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_Write_IPEI_Test (panel) != 1){
		sprintf(test_number, "IPEI_Mode"); 
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_Write_Serial_Number_Test (panel) != 1){
		sprintf(test_number, "Serial_Number");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	if(DECT_Reset_EEPROM_Test_Flow_Test (panel) != 1){
		sprintf(test_number, "Test_Flow_Test");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_File_System_Test (panel) != 1){
		sprintf(test_number, "File_System_Test");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_SD_Card_Test (panel) != 1){
		sprintf(test_number, "SD_CARD_Test");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	if(Open_CMD60 (panel) != 1){
		sprintf(test_number, "Open_CMD60");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	} 
	
	
	if(Set_CMD60_in_Loopback_Mode_EU_Setting (panel) != 1){
		sprintf(test_number, "CMD60_Loopback");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	

	if(DECT_Get_SW_Version_Test (panel) != 1){
		sprintf(test_number, "SW_Version_Test");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_Park_Search_Criteria_Test (panel) != 1){
		sprintf(test_number, "Park_Search_Test");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_EEPROM_PLI_1_Test  (panel) != 1){
		sprintf(test_number, "PLI_1_Test");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_EEPROM_Park_1_Test  (panel) != 1){
		sprintf(test_number, "Park_1_Test");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_Enter_Test_Mode (panel) != 1){
		sprintf(test_number, "Enter_Test_Mode");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_Get_Service_State_Test(panel) != 1){
		sprintf(test_number, "Get_Service_State");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	if(Setup_Connect_CMD60(panel) != 1){
		sprintf(test_number, "Setup_Connect_CMD60");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	 
	if(DECT_Measure_NTP_Power_Test(panel) != 1){
		sprintf(test_number, "NTP_Power");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_Measure_EU_Deviation_Test(panel) != 1){
		sprintf(test_number, "EU_Deviation");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_Measure_Frequency_Offset_Test(panel) != 1){
		sprintf(test_number, "Freq_Offset");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_Measure_Frequency_Drift_Test(panel) != 1){
		sprintf(test_number, "Freq_Drift");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_Measure_RSSI_45dBm_Test(panel) != 1){
		sprintf(test_number, "RSSI_45dBm");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_Measure_RSSI_75dBm_Test(panel) != 1){
		sprintf(test_number, "RSSI_75dBm");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_Program_RSSI_K_Factor_Test(panel) != 1){
		sprintf(test_number, "RSSI_KFactor");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_Program_RSSI_Offset_Test(panel) != 1){
		sprintf(test_number, "RSSI_Offset");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_Measure_Receiver_Sensitivity_Test(panel) != 1){
		sprintf(test_number, "Receiver Sensitivity");
		DECT_Clean_Shutdown_Sequence(panel);  
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	} 
	
	
	/*if(DECT_Power_Template_Match_Test(panel) != 1){
		sprintf(test_number, "Template Match"); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1) 
		return 0;
	*/
	
	
	if(Close_CMD60(panel) != 1){
		sprintf(test_number, "Close_CMD60");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_Make_Backup_Test (panel) != 1){
		sprintf(test_number, "Make_Backup_Test");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_Read_Device_MAC_Address_Test(panel) != 1){
		sprintf(test_number, "Read_Device_MAC_Address"); 
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	
	//USB Tests
	DECT_Connect_USB_Cable(panel);
	
	
	if(DECT_Enter_Test_Mode_USB (panel) != 1){
		sprintf(test_number, "Enter_Test_Mode_USB");
		CloseCom(COM_port_2);
		//sprintf(msg,"Disconnect USB Cable.");
		//Display_OK_MSG(msg);
		Open_COM_PORT_1(panel); 
		Open_COM_Port_Relays ();
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	}

	
	/*if(DECT_Get_SW_Version_USB_Test (panel) != 1){
		sprintf(test_number, "SW_Version_USB"); 
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	}*/
	
			   
	if(DECT_Enter_Bluetooth_Test_Mode_USB (panel) != 1){
		sprintf(test_number, "Blutooth_Test_Mode");
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	}

	
	if(DECT_Enter_Bluetooth_Test_Mode_USB (panel) != 1){
		sprintf(test_number, "Blutooth_Test_Mode");
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	}
	
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	}
	
	
	if(Open_MT8852B (panel) != 1){
		sprintf(test_number, "Open_MT8852B ");
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	}

	
	if(Check_Sync_MT8852B (panel) != 1){
		sprintf(test_number, "Check_Sync_MT8852B ");
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	} 
	
	
	Delay(5);
	
	
	if(Set_MT8852B_Loopback_Mode (panel) != 1){
		sprintf(test_number, "Set_MT8852B_Loopback_Mode");
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	} 
	
	
	Delay(5);  
	
	if(Run_Scripts_MT8852B (panel) != 1){
		sprintf(test_number, "Run_Scripts_MT8852B");
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	} 
	

	if(Measure_Output_Power_MT8852B (panel) != 1){
		sprintf(test_number, "Output_Power_MT8852B");
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	}  

	
	if(Measure_Modulation_Index_MT8852B (panel) != 1){
		sprintf(test_number, "Modulation_Index_MT8852B"); 
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	} 
	
	
	if(Measure_Carrier_Frequency_Tolerance_MT8852B (panel) != 1){
		sprintf(test_number, "Carrier_Frequency_Tolerance_MT8852B");
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	} 
	
	
	if(Measure_Carrier_Frequency_Drift_MT8852B (panel) != 1){
		sprintf(test_number, "Carrier_Frequency_Drift_MT8852B");
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	}  
	
	
	if(Measure_Sensitive_Single_Slot_Packets_MT8852B (panel) != 1){
		sprintf(test_number, "Single_Slot_Packets_MT8852B");
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_Bluetooth_Sync_Clock_USB_Test(panel) != 1){
		sprintf(test_number, "Bluetooth_Sync_Clock ");
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	}
	
		
	if(DECT_Bluetooth_Get_Analog_Trim_USB_Test(panel) != 1){
		sprintf(test_number, "Bluetooth_Get_Trim ");
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_Bluetooth_Write_Analog_Trim_USB_Test(panel) != 1){
		sprintf(test_number, "Bluetooth_Write_Trim ");
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	}
	
	
	if(Close_MT8852B (panel) != 1){
		sprintf(test_number, "Close_MT8852B ");
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_Write_MAC_Address_USB_Test (panel) != 1){
		sprintf(test_number, "Write_MAC ");
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	}
	
	if(DECT_EEPROM_LED_USB_Test (panel) != 1){	   		//612d,622d,632d
		sprintf(test_number, "LED_USB_Test");
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_EEPROM_HOTKEY_USB_Test (panel) != 1){  		//622d,632d 
		sprintf(test_number, "HOTKEY_USB_Test");
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_EEPROM_VIBRATOR_USB_Test (panel) != 1){		//622d,632d 
		sprintf(test_number, "VIBRATOR_USB_Test");
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	}

	
	if(DECT_EEPROM_WRITE_DATE_USB_Test (panel) != 1){
		sprintf(test_number, "Write_Date_USB_Test");
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_EEPROM_WRITE_PASS_USB_Test (panel) != 1){
		sprintf(test_number, "Write_PASS_USB_Test");
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_Make_Backup_USB_Test (panel) != 1){
		sprintf(test_number, "Make_Backup_USB_Test");
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_Power_Off_USB_Test (panel) != 1){
		sprintf(test_number, "Power_Off_USB_Test");
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1) 
		return 0; 
	

	DECT_Disconnect_USB_Cable(panel); 
	
	
	if(Power_OFF_DECT () != 0){
		sprintf(test_number, "DECT_Power"); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1) 
		return 0; 
	
	
  return 1;

}


//*************************************************************
//* 
//*
//*************************************************************
int DECT_632d_CCA_Test_Sequence(int panel, char *test_number)
{
	char  buffer[MAXLINE+1] = {'\0'},  msg_buffer[MAXLINE+128];  
	double delay_time = 600;
	int status = 0;
	int timeout = 0;
	int i = 0;
	char msg[256] = {'\0'};

	
	sprintf(buffer,"Starting DECT CCA Tests\n\n");  
	SetCtrlVal (panel, PANEL_Serial_Window_1, buffer);
	

	if(Power_ON_DECT () != 0){
		sprintf(test_number, "DECT_Power"); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1) 
		return 0; 
	
	
	if(Open_COM_Port_Relays () != 0){
		sprintf(test_number, "COM_PORT_Relays"); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1) 
		return 0; 
	
	
	Delay(15);
	
	//Start Tests
	if(DECT_Enter_Test_Mode (panel) != 1){
		sprintf(test_number, "Enter_Test_Mode"); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	if(DECT_Read_Error_Counter_Test (panel) != 1){
		sprintf(test_number, "Read_Error_Counter_Test"); 
		DECT_Clean_Shutdown_Sequence(panel);  
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	/*if(Error_Counter != 0){
		if(DECT_Master_Reset_Test (panel) != 1){
			sprintf(test_number, "Master_Reset_Test"); 
			return 0;
		} 
		
		
		if(DECT_Power_Off_Test (panel) != 1){
			sprintf(test_number, "Power_Off_Test"); 
			return 0;
		}
	
		ProcessSystemEvents ();
		if (Stop_Test_Exe == 1) 
			return 0; 
	
	
		if(Close_COM_Port_Relays () != 0){
			sprintf(test_number, "COM_PORT_Relays"); 
			return 0;
		}
	
		return 0;
		
	}*/
	
	if(DECT_P1V8_Voltage_Adjustment_Test (panel) != 1){
		sprintf(test_number, "P1V8_Adjust"); 
		DECT_Clean_Shutdown_Sequence(panel);  
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_Activate_Ref_Clk_Test (panel) != 1){
		sprintf(test_number, "Activate_Ref_Clk");
		DECT_Clean_Shutdown_Sequence(panel);  
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_Set_Clk_Test (panel) != 1){
		sprintf(test_number, "Set_Ref_Clk");
		DECT_Clean_Shutdown_Sequence(panel);  
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	

	if(DECT_Branding_Set_Test (panel) != 1){
		sprintf(test_number, "Branding_Set_Test");
		DECT_Clean_Shutdown_Sequence(panel);  
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_Branding_Check_Test (panel) != 1){
		sprintf(test_number, "Branding_Check_Test");
		DECT_Clean_Shutdown_Sequence(panel);  
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	

	if(DECT_Write_HW_Release_Test (panel) != 1){
		sprintf(test_number, "HW_Release_Test");
		DECT_Clean_Shutdown_Sequence(panel);  
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_MAN_DOWN_SENSOR_Test(panel) != 1){
		sprintf(test_number, "Man_Down_Test");
		DECT_Clean_Shutdown_Sequence(panel);  
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	if(DECT_Write_IPEI_Test (panel) != 1){
		sprintf(test_number, "IPEI_Mode");
		DECT_Clean_Shutdown_Sequence(panel);  
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_Write_Serial_Number_Test (panel) != 1){
		sprintf(test_number, "Serial_Number");
		DECT_Clean_Shutdown_Sequence(panel);  
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	if(DECT_Reset_EEPROM_Test_Flow_Test (panel) != 1){
		sprintf(test_number, "Test_Flow_Test");
		DECT_Clean_Shutdown_Sequence(panel);  
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_File_System_Test (panel) != 1){
		sprintf(test_number, "File_System_Test");
		DECT_Clean_Shutdown_Sequence(panel);  
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_SD_Card_Test (panel) != 1){
		sprintf(test_number, "SD_CARD_Test");
		DECT_Clean_Shutdown_Sequence(panel);  
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	if(Open_CMD60 (panel) != 1){
		sprintf(test_number, "Open_CMD60");
		DECT_Clean_Shutdown_Sequence(panel);  
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	if(Set_CMD60_in_Loopback_Mode_EU_Setting (panel) != 1){
		sprintf(test_number, "CMD60_Loopback");
		DECT_Clean_Shutdown_Sequence(panel);  
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	} 
	

	if(DECT_Get_SW_Version_Test (panel) != 1){
		sprintf(test_number, "SW_Version_Test");
		DECT_Clean_Shutdown_Sequence(panel);  
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_Park_Search_Criteria_Test (panel) != 1){
		sprintf(test_number, "Park_Search_Test");
		DECT_Clean_Shutdown_Sequence(panel);  
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_EEPROM_PLI_1_Test  (panel) != 1){
		sprintf(test_number, "PLI_1_Test");
		DECT_Clean_Shutdown_Sequence(panel);  
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	};
	
	
	if(DECT_EEPROM_Park_1_Test  (panel) != 1){
		sprintf(test_number, "Park_1_Test");
		DECT_Clean_Shutdown_Sequence(panel);  
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_Enter_Test_Mode (panel) != 1){
		sprintf(test_number, "Enter_Test_Mode");
		DECT_Clean_Shutdown_Sequence(panel);  
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_Get_Service_State_Test(panel) != 1){
		sprintf(test_number, "Get_Service_State");
		DECT_Clean_Shutdown_Sequence(panel);  
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	if(Setup_Connect_CMD60(panel) != 1){
		sprintf(test_number, "Setup_Connect_CMD60");
		DECT_Clean_Shutdown_Sequence(panel);  
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	 
	if(DECT_Measure_NTP_Power_Test(panel) != 1){
		sprintf(test_number, "NTP_Power"); 
		DECT_Clean_Shutdown_Sequence(panel);  
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_Measure_EU_Deviation_Test(panel) != 1){
		sprintf(test_number, "EU_Deviation");
		DECT_Clean_Shutdown_Sequence(panel);  
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_Measure_Frequency_Offset_Test(panel) != 1){
		sprintf(test_number, "Freq_Offset");
		DECT_Clean_Shutdown_Sequence(panel);  
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_Measure_Frequency_Drift_Test(panel) != 1){
		sprintf(test_number, "Freq_Drift");
		DECT_Clean_Shutdown_Sequence(panel);  
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_Measure_RSSI_45dBm_Test(panel) != 1){
		sprintf(test_number, "RSSI_45dBm");
		DECT_Clean_Shutdown_Sequence(panel);  
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_Measure_RSSI_75dBm_Test(panel) != 1){
		sprintf(test_number, "RSSI_75dBm");
		DECT_Clean_Shutdown_Sequence(panel);  
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_Program_RSSI_K_Factor_Test(panel) != 1){
		sprintf(test_number, "RSSI_KFactor");
		DECT_Clean_Shutdown_Sequence(panel);  
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_Program_RSSI_Offset_Test(panel) != 1){
		sprintf(test_number, "RSSI_Offset");
		DECT_Clean_Shutdown_Sequence(panel);  
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_Measure_Receiver_Sensitivity_Test(panel) != 1){
		sprintf(test_number, "Receiver Sensitivity");
		DECT_Clean_Shutdown_Sequence(panel);  
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	/*if(DECT_Power_Template_Match_Test(panel) != 1){
		sprintf(test_number, "Template Match"); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1) 
		return 0;
	*/
	
	
	if(Close_CMD60(panel) != 1){
		sprintf(test_number, "Close_CMD60");
		DECT_Clean_Shutdown_Sequence(panel);  
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_Make_Backup_Test (panel) != 1){
		sprintf(test_number, "Make_Backup_Test");
		DECT_Clean_Shutdown_Sequence(panel);  
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	

	if(DECT_Read_Device_MAC_Address_Test(panel) != 1){
		sprintf(test_number, "Read_Device_MAC_Address");
		DECT_Clean_Shutdown_Sequence(panel);  
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	
	//USB Tests
	DECT_Connect_USB_Cable(panel);
	
	
	if(DECT_Enter_Test_Mode_USB (panel) != 1){
		sprintf(test_number, "Enter_Test_Mode_USB");
		DECT_Clean_Shutdown_USB_Sequence(panel);  
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	}

	
	/*if(DECT_Get_SW_Version_USB_Test (panel) != 1){
		sprintf(test_number, "SW_Version_USB");
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	}*/
																											  
			   
	if(DECT_Enter_Bluetooth_Test_Mode_USB (panel) != 1){
		sprintf(test_number, "Blutooth_Test_Mode");
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_Enter_Bluetooth_Test_Mode_USB (panel) != 1){
		sprintf(test_number, "Blutooth_Test_Mode");
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	}
	
	
	if(Open_MT8852B (panel) != 1){
		sprintf(test_number, "Open_MT8852B ");
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	} 

	
	if(Check_Sync_MT8852B (panel) != 1){
		sprintf(test_number, "Check_Sync_MT8852B "); 
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	}
	
	
	Delay(5);  
	
	if(Set_MT8852B_Loopback_Mode (panel) != 1){
		sprintf(test_number, "Set_MT8852B_Loopback_Mode"); 
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	}
	
	
	Delay(5);  
	
	if(Run_Scripts_MT8852B (panel) != 1){
		sprintf(test_number, "Run_Scripts_MT8852B");
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	} 
	

	if(Measure_Output_Power_MT8852B (panel) != 1){
		sprintf(test_number, "Output_Power_MT8852B");
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	} 

	
	if(Measure_Modulation_Index_MT8852B (panel) != 1){
		sprintf(test_number, "Modulation_Index_MT8852B");
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	} 
	
	
	if(Measure_Carrier_Frequency_Tolerance_MT8852B (panel) != 1){
		sprintf(test_number, "Carrier_Frequency_Tolerance_MT8852B");
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	} 
	
	
	if(Measure_Carrier_Frequency_Drift_MT8852B (panel) != 1){
		sprintf(test_number, "Carrier_Frequency_Drift_MT8852B"); 
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	}  
	
	
	if(Measure_Sensitive_Single_Slot_Packets_MT8852B (panel) != 1){
		sprintf(test_number, "Single_Slot_Packets_MT8852B"); 
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_Bluetooth_Sync_Clock_USB_Test(panel) != 1){
		sprintf(test_number, "Bluetooth_Sync_Clock ");
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	} 
	
		
	if(DECT_Bluetooth_Get_Analog_Trim_USB_Test(panel) != 1){
		sprintf(test_number, "Bluetooth_Get_Trim ");
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_Bluetooth_Write_Analog_Trim_USB_Test(panel) != 1){
		sprintf(test_number, "Bluetooth_Write_Trim ");
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	}
	
	
	if(Close_MT8852B (panel) != 1){
		sprintf(test_number, "Close_MT8852B ");
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_Write_MAC_Address_USB_Test (panel) != 1){
		sprintf(test_number, "Write_MAC ");
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	}
	
	
	
	if(DECT_EEPROM_LED_USB_Test (panel) != 1){	   		//612d, 622d, 632d
		sprintf(test_number, "LED_USB_Test");
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	} 
	
	
	if(DECT_EEPROM_SOS_USB_Test (panel) != 1){	  		//612d, 622d, 632d 
		sprintf(test_number, "SOS_USB_Test");
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_EEPROM_HOTKEY_USB_Test (panel) != 1){   	//622d, 632d 
		sprintf(test_number, "HOTKEY_USB_Test");
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_EEPROM_VIBRATOR_USB_Test (panel) != 1){ 	//622d, 632d 
		sprintf(test_number, "VIBRATOR_USB_Test");
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	} 
	
	
	if(DECT_EEPROM_LOUDNESS_USB_Test (panel) != 1){ 	//632d 
		sprintf(test_number, "LOUDNESS_USB_Test");
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_EEPROM_WRITE_DATE_USB_Test (panel) != 1){
		sprintf(test_number, "Write_Date_USB_Test");
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	} 
	
	
	if(DECT_EEPROM_WRITE_PASS_USB_Test (panel) != 1){
		sprintf(test_number, "Write_PASS_USB_Test");
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	}; 
	
	
	if(DECT_Make_Backup_USB_Test (panel) != 1){
		sprintf(test_number, "Make_Backup_USB_Test");
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_Power_Off_USB_Test (panel) != 1){
		sprintf(test_number, "Power_Off_USB_Test");
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		return 0;
	}
	

	DECT_Disconnect_USB_Cable(panel); 

	
	if(Power_OFF_DECT () != 0){
		sprintf(test_number, "DECT_Power"); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1) 
		return 0; 
	
	
  return 1;

}



//*************************************************************
//* 
//*
//*************************************************************
int DECT_612d_Final_Test_Sequence(int panel, char *test_number)
{
	char  buffer[MAXLINE+1] = {'\0'},  msg_buffer[MAXLINE+128];  
	double delay_time = 600;
	int status = 0;
	int timeout = 0;
	int i = 0;
	char msg[256] = {'\0'}; 
	int counter = 0;
	
	sprintf(buffer,"Starting DECT FINAL Tests\n");  
	SetCtrlVal (panel, PANEL_Serial_Window_1, buffer);
	
	
	sprintf(msg,"Power on the Phone manually. Use the ESC key on the phone to enter Blue Screen.\n\n Press OK when Blue Screen is visible."); 
	Display_OK_MSG(msg); 
	
	
	if(DECT_Enter_Enhanced_Test_Mode (panel) != 1){
		sprintf(test_number, "Enter_Enhanced_Test_Mode"); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1) 
		return 0;
	
	
	if(DECT_Read_Error_Counter_Test (panel) != 1){
		sprintf(test_number, "Read_Error_Counter_Test"); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1) 
		return 0;
	
	
	
	if(DECT_Reset_EEPROM_Test_Flow_Test (panel) != 1){
		sprintf(test_number, "Test_Flow_Test");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	if(DECT_Vendor_SN_Test(panel) != 1){
		sprintf(test_number, "SN_CHECK");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	//IPEI Check
	if(DECT_VERS_IPEI_Test(panel) != 1){
		sprintf(test_number, "IPEI_CHECK");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	} 
	
	//User Tests
	////////////////////////
	if(DECT_LED_KEYB_Test(panel) != 1){
		sprintf(test_number, "LED_KEYB");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
		
	
	if(DECT_KEY12_Test(panel) != 1){
		sprintf(test_number, "KEY12");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	} 
	
	if(DECT_LCD_POS_Test(panel) != 1){
		sprintf(test_number, "LCD_POS");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	if(DECT_LCD_RED_Test(panel) != 1){
		sprintf(test_number, "LCD_RED");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	if(DECT_LCD_GREEN_Test(panel) != 1){
		sprintf(test_number, "LCD_GREEN");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	
	if(DECT_LCD_BLUE_Test(panel) != 1){
		sprintf(test_number, "LCD_BLUE");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}

	
	if(DECT_LED_RED_Test(panel) != 1){
		sprintf(test_number, "LED_RED");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	if(DECT_LED_GREEN_Test(panel) != 1){
		sprintf(test_number, "LED_GREEN");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
		
	if(DECT_LED_ORANGE_Test(panel) != 1){
		sprintf(test_number, "LED_ORANGE");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	if(DECT_Audio_Headset_Test(panel) != 1){
		sprintf(test_number, "Audio_Headset");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	} 
	
	//Audio Tests
	////////////////////////
	sprintf(msg,"Close Test Fixture Cover, then Press OK.");
	Display_OK_MSG(msg);
	
	
	while(DECT_Check_Final_Fixture_Status_Test(panel) != 1){
		sprintf(msg,"Close Test Fixture Cover, then Press OK.");
		Display_OK_MSG(msg);
		if(counter == 5){
			sprintf(test_number, "Fixture_Cover");
			DECT_Clean_Shutdown_Sequence(panel); 
			return 0;
		}
		counter = counter + 1; 
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel); 
	 	return 0;
	}
	
	
	if(DECT_Audio_MIC_Test (panel) != 1){
		sprintf(test_number, "Audio_MIC");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel); 
	 	return 0;
	}
	

	if(DECT_Audio_Speaker_Test (panel) != 1){
		sprintf(test_number, "Audio_SPK");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	if(DECT_Audio_EP_Test (panel) != 1){
		sprintf(test_number, "Audio_EP");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	} 		
	
	//CMD60 Tests
	////////////////////////
	if(Open_CMD60 (panel) != 1){
		sprintf(test_number, "Open_CMD60"); 
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1) 
		return 0; 
	
	
	if(Set_CMD60_in_Loopback_Mode_Radiated_Setting (panel) != 1){
		sprintf(test_number, "CMD60_Loopback");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1) 
		return 0; 
	

	if(DECT_Get_SW_Version_Test (panel) != 1){
		sprintf(test_number, "SW_Version_Test");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1) 
		return 0;
	
	
	if(DECT_Park_Search_Criteria_Test (panel) != 1){
		sprintf(test_number, "Park_Search_Test");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1) 
		return 0;
	
	
	if(DECT_EEPROM_PLI_1_Test  (panel) != 1){
		sprintf(test_number, "PLI_1_Test");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1) 
		return 0;
	
	
	if(DECT_EEPROM_Park_1_Test  (panel) != 1){
		sprintf(test_number, "Park_1_Test"); 
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1) 
		return 0;
	
	
	if(DECT_Enter_Test_Mode (panel) != 1){
		sprintf(test_number, "Enter_Test_Mode");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1) 
		return 0;
	
	
	if(DECT_Get_Service_State_Test(panel) != 1){
		sprintf(test_number, "Get_Service_State");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1) 
		return 0;
	
	
	if(Setup_Connect_CMD60(panel) != 1){
		sprintf(test_number, "Setup_Connect_CMD60");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1) 
		return 0;
	
	 
	Delay(5);

	if(DECT_Measure_NTP_Power_Radiated_Test(panel) != 1){
		sprintf(test_number, "NTP_RAD_Power");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1) 
		return 0;

	
	if(Close_CMD60(panel) != 1){
		sprintf(test_number, "Close_CMD60");
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}								  
	
	if(DECT_EEPROM_WRITE_FINAL_DATE_Test(panel) != 1){
		sprintf(test_number, "Write_Final_Date_Test");
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_EEPROM_WRITE_PASS_Final_Test (panel) != 1){
		sprintf(test_number, "Write_PASS_Test");
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_Make_Backup_Test (panel) != 1){
		sprintf(test_number, "Make_Backup_Test");
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	} 
	
	
	if(DECT_Master_Reset_Test (panel) != 1){
		sprintf(test_number, "Master_Reset_Test"); 
		return 0;
	} 
	
	
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);     
		return 0;
	}
	
	Delay(3); 

	if(DECT_Power_Off_Test (panel) != 1){
		sprintf(test_number, "Power_Off_Test"); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		return 0;
	}
	
	Delay(3); 

	CloseCom(COM_port);
	
	sprintf(msg,"Open Test Fixture Cover, then Press OK.");
	Display_OK_MSG(msg);
	
	
	return 1;

	
}



//*************************************************************
//* 
//*
//*************************************************************
int DECT_622d_Final_Test_Sequence(int panel, char *test_number)
{
	char  buffer[MAXLINE+1] = {'\0'},  msg_buffer[MAXLINE+128];  
	double delay_time = 600;
	int status = 0;
	int timeout = 0;
	int i = 0;
	char msg[256] = {'\0'}; 
	int counter = 0;
	
	sprintf(buffer,"Starting DECT FINAL Tests\n");  
	SetCtrlVal (panel, PANEL_Serial_Window_1, buffer);
	
	
	sprintf(msg,"Power on the Phone manually. Use the ESC key on the phone to enter Blue Screen.\n\n Press OK when Blue Screen is visible.");
	Display_OK_MSG(msg); 
	
	
	if(DECT_Enter_Enhanced_Test_Mode (panel) != 1){
		sprintf(test_number, "Enter_Enhanced_Test_Mode"); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1) 
		return 0;
	
	
	
	/*if(DECT_Serial_Number_String_Test (panel) != 1){
		sprintf(test_number, "Serial_Number_String"); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1) 
		return 0;*/
	
	
	/*if(DECT_Serial_Number_String_Test_2 (panel) != 1){
		sprintf(test_number, "Serial_Number_String"); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1) 
		return 0;*/
	
	
	if(DECT_Read_Error_Counter_Test (panel) != 1){
		sprintf(test_number, "Read_Error_Counter_Test"); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1) 
		return 0;
	
	
	if(DECT_Reset_EEPROM_Test_Flow_Test (panel) != 1){
		sprintf(test_number, "Test_Flow_Test");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	/*if(Error_Counter != 0){
		if(DECT_Master_Reset_Test (panel) != 1){
			sprintf(test_number, "Master_Reset_Test"); 
			return 0;
		} 
		
		
		if(DECT_Power_Off_Test (panel) != 1){
			sprintf(test_number, "Power_Off_Test"); 
			return 0;
		}
	
		ProcessSystemEvents ();
		if (Stop_Test_Exe == 1) 
			return 0; 
	
	
		if(Close_COM_Port_Relays () != 0){
			sprintf(test_number, "COM_PORT_Relays"); 
			return 0;
		}
	
		return 0;
		
	}*/
	
	
	
	//IPEI and Bluetooth and SNO Checks
	if(DECT_Vendor_SN_Test(panel) != 1){
		sprintf(test_number, "SN_CHECK");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	
	
	if(DECT_VERS_IPEI_Test(panel) != 1){
		sprintf(test_number, "IPEI_CHECK");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	
	if(DECT_Verify_MAC_Address_Test (panel) != 1){
		sprintf(test_number, "MAC_CHECK");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	//User Tests
	////////////////////////
	if(DECT_LED_KEYB_Test(panel) != 1){
		sprintf(test_number, "LED_KEYB");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
		
	
	if(DECT_KEY22_Test(panel) != 1){
		sprintf(test_number, "KEY22");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	} 
	
	if(DECT_LCD_POS_Test(panel) != 1){
		sprintf(test_number, "LCD_POS");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	if(DECT_LCD_RED_Test(panel) != 1){
		sprintf(test_number, "LCD_RED");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	if(DECT_LCD_GREEN_Test(panel) != 1){
		sprintf(test_number, "LCD_GREEN");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	
	if(DECT_LCD_BLUE_Test(panel) != 1){
		sprintf(test_number, "LCD_BLUE");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}

	
	if(DECT_LED_RED_Test(panel) != 1){
		sprintf(test_number, "LED_RED");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	if(DECT_LED_GREEN_Test(panel) != 1){
		sprintf(test_number, "LED_GREEN");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
		
	if(DECT_LED_ORANGE_Test(panel) != 1){
		sprintf(test_number, "LED_ORANGE");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	if(DECT_SDCARD_Test(panel) != 1){
		sprintf(test_number, "SDCard");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	if(DECT_Audio_Headset_Test(panel) != 1){
		sprintf(test_number, "Audio_Headset");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	} 
	
	//Audio Tests
	////////////////////////
	
	sprintf(msg,"Close Test Fixture Cover, then Press OK.");
	Display_OK_MSG(msg);
	
	while(DECT_Check_Final_Fixture_Status_Test(panel) != 1){
		sprintf(msg,"Close Test Fixture Cover, then Press OK.");
		Display_OK_MSG(msg);
		if(counter == 5){
			sprintf(test_number, "Fixture_Cover");
			DECT_Clean_Shutdown_Sequence(panel); 
			return 0;
		}
		counter = counter + 1; 
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel); 
	 	return 0;
	}
	
	
	if(DECT_Audio_MIC_Test (panel) != 1){
		sprintf(test_number, "Audio_MIC");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel); 
	 	return 0;
	}
	

	if(DECT_Audio_Speaker_Test (panel) != 1){
		sprintf(test_number, "Audio_SPK");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	if(DECT_Audio_EP_Test (panel) != 1){
		sprintf(test_number, "Audio_EP");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}

	
	if(DECT_Audio_Trembler_Test(panel) != 1){
		sprintf(test_number, "Audio_Trembler");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}			   
	
	//CMD60 Tests
	////////////////////////
	if(Open_CMD60 (panel) != 1){
		sprintf(test_number, "Open_CMD60");
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1) 
		return 0; 
	
	
	if(Set_CMD60_in_Loopback_Mode_Radiated_Setting (panel) != 1){
		sprintf(test_number, "CMD60_Loopback");
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1) 
		return 0; 
	

	if(DECT_Get_SW_Version_Test (panel) != 1){
		sprintf(test_number, "SW_Version_Test");
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1) 
		return 0;
	
	
	if(DECT_Park_Search_Criteria_Test (panel) != 1){
		sprintf(test_number, "Park_Search_Test");
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1) 
		return 0;
	
	
	if(DECT_EEPROM_PLI_1_Test  (panel) != 1){
		sprintf(test_number, "PLI_1_Test");
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1) 
		return 0;
	
	
	if(DECT_EEPROM_Park_1_Test  (panel) != 1){
		sprintf(test_number, "Park_1_Test");
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1) 
		return 0;
	
	
	if(DECT_Enter_Test_Mode (panel) != 1){
		sprintf(test_number, "Enter_Test_Mode");
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1) 
		return 0;
	
	
	if(DECT_Get_Service_State_Test(panel) != 1){
		sprintf(test_number, "Get_Service_State");
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1) 
		return 0;
	
	
	if(Setup_Connect_CMD60(panel) != 1){
		sprintf(test_number, "Setup_Connect_CMD60");
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1) 
		return 0;
	
	 
	Delay(5);

	if(DECT_Measure_NTP_Power_Radiated_Test(panel) != 1){
		sprintf(test_number, "NTP_RAD_Power");
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
		
	Delay(1);
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1) 
		return 0;


	
	if(Close_CMD60(panel) != 1){
		sprintf(test_number, "Close_CMD60");
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1) 
		return 0;  			 
	

	if(DECT_Read_Device_MAC_Address_Test(panel) != 1){
		sprintf(test_number, "Read_Device_MAC_Address");
		DECT_Clean_Shutdown_Sequence(panel);  
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	

	CloseCom(COM_port);
	
	sprintf(msg,"Open Test Fixture Cover, then Press OK.");
	Display_OK_MSG(msg);
	
	//USB and Bluetooth Tests
	DECT_Connect_USB_Cable(panel);
	
	sprintf(msg,"Close Test Fixture Cover, then Press OK.");
	Display_OK_MSG(msg);
	
		while(DECT_Check_Final_Fixture_Status_Test(panel) != 1){
		sprintf(msg,"Close Test Fixture Cover, then Press OK.");
		Display_OK_MSG(msg);
		if(counter == 5){
			sprintf(test_number, "Fixture_Cover");
			DECT_Clean_Shutdown_Sequence(panel); 
			return 0;
		}
		counter = counter + 1; 
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel); 
	 	return 0;
	}
	if(DECT_Enter_Test_Mode_USB (panel) != 1){
		sprintf(test_number, "Enter_Test_Mode_USB");
		DECT_Clean_Shutdown_USB_Sequence(panel);  
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	}

	
	/*if(DECT_Get_SW_Version_USB_Test (panel) != 1){
		sprintf(test_number, "SW_Version_USB");
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	}*/
	
			   
	if(DECT_Enter_Bluetooth_Test_Mode_USB (panel) != 1){
		sprintf(test_number, "Blutooth_Test_Mode");
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_Enter_Bluetooth_Test_Mode_USB (panel) != 1){
		sprintf(test_number, "Blutooth_Test_Mode");
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	}
	
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	}
	
	
	
	if(Open_MT8852B (panel) != 1){
		sprintf(test_number, "Open_MT8852B ");
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	} 

	
	if(Check_Sync_MT8852B (panel) != 1){
		sprintf(test_number, "Check_Sync_MT8852B "); 
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	}
	
	
	if(Set_MT8852B_Loopback_Mode_Radiated (panel) != 1){
		sprintf(test_number, "Set_MT8852B_Loopback_Mode_Radiated"); 
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	}
	
	
	if(Run_Scripts_MT8852B (panel) != 1){
		sprintf(test_number, "Run_Scripts_MT8852B");
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	} 
	

	if(Measure_Output_Power_MT8852B (panel) != 1){
		sprintf(test_number, "Output_Power_MT8852B");
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	} 

	
	if(Close_MT8852B (panel) != 1){
		sprintf(test_number, "Close_MT8852B ");
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	}

	
	if(DECT_Disable_Bluetooth_USB_Test (panel) != 1){
		sprintf(test_number, "Disable Bluetooth"); 
		return 0;
	} 
	
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	}

	
	sprintf(msg,"Open Test Fixture Cover, then Press OK.");
	Display_OK_MSG(msg);
	
	
	
	DECT_Disconnect_USB_Cable(panel); 
	
	
	if(Open_COM_PORT_1(panel) != 1){
		sprintf(test_number, "COM Switch");
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_EEPROM_WRITE_FINAL_DATE_Test(panel) != 1){
		sprintf(test_number, "Write_Final_Date_Test");
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_EEPROM_WRITE_PASS_Final_Test (panel) != 1){
		sprintf(test_number, "Write_PASS_Test");
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_Make_Backup_Test (panel) != 1){
		sprintf(test_number, "Make_Backup_Test");
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	} 
	
	
	if(DECT_Master_Reset_Test (panel) != 1){
		sprintf(test_number, "Master_Reset_Test"); 
		return 0;
	} 
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);     
		return 0;
	}
	
	Delay(3);

	if(DECT_Power_Off_Test (panel) != 1){
		sprintf(test_number, "Power_Off_Test"); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		return 0;
	}
	
	Delay(3); 
	

	CloseCom(COM_port);
	
	/*if(DECT_Power_Off_USB_Test (panel) != 1){
		sprintf(test_number, "Power_Off_USB_Test");
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		return 0;
	}*/
	
	
	//sprintf(msg,"Press the red power button until the Switch Off screen appears, then select YES.\n Wait for corner LED to stop flashing then press OK when power down is complete.");
	//Display_OK_MSG(msg); 
	
  return 1;

	
}


//*************************************************************
//* 
//*
//*************************************************************
int DECT_632d_Final_Test_Sequence(int panel, char *test_number)
{
	char  buffer[MAXLINE+1] = {'\0'},  msg_buffer[MAXLINE+128];  
	double delay_time = 600;
	int status = 0;
	int timeout = 0;
	int i = 0;
	char msg[256] = {'\0'}; 
	int counter = 0;

	
	sprintf(buffer,"Starting DECT FINAL Tests\n");  
	SetCtrlVal (panel, PANEL_Serial_Window_1, buffer);
	
	
	sprintf(msg,"Power on the Phone manually. Use the ESC key on the phone to enter Blue Screen.\n\n Press OK when Blue Screen is visible."); 
	Display_OK_MSG(msg); 
	
	
	if(DECT_Enter_Enhanced_Test_Mode (panel) != 1){
		sprintf(test_number, "Enter_Enhanced_Test_Mode"); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1) 
		return 0;
	
	
	if(DECT_Read_Error_Counter_Test (panel) != 1){
		sprintf(test_number, "Read_Error_Counter_Test"); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1) 
		return 0;
	
	
	
	if(DECT_Reset_EEPROM_Test_Flow_Test (panel) != 1){
		sprintf(test_number, "Test_Flow_Test");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
  
	//IPEI and Bluetooth and SN Checks
	if(DECT_Vendor_SN_Test(panel) != 1){
		sprintf(test_number, "SN_CHECK");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	if(DECT_VERS_IPEI_Test(panel) != 1){
		sprintf(test_number, "IPEI_CHECK");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	
	if(DECT_Verify_MAC_Address_Test (panel) != 1){
		sprintf(test_number, "MAC_CHECK");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	
	
	//User Tests
	////////////////////////
	if(DECT_LED_KEYB_Test(panel) != 1){
		sprintf(test_number, "LED_KEYB");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
		
	
	if(DECT_KEY32_Test(panel) != 1){
		sprintf(test_number, "KEY32");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	} 
	
	if(DECT_LCD_POS_Test(panel) != 1){
		sprintf(test_number, "LCD_POS");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	if(DECT_LCD_RED_Test(panel) != 1){
		sprintf(test_number, "LCD_RED");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	if(DECT_LCD_GREEN_Test(panel) != 1){
		sprintf(test_number, "LCD_GREEN");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	
	if(DECT_LCD_BLUE_Test(panel) != 1){
		sprintf(test_number, "LCD_BLUE");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}

	
	if(DECT_LED_RED_Test(panel) != 1){
		sprintf(test_number, "LED_RED");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	if(DECT_LED_GREEN_Test(panel) != 1){
		sprintf(test_number, "LED_GREEN");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
		
	if(DECT_LED_ORANGE_Test(panel) != 1){
		sprintf(test_number, "LED_ORANGE");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	if(DECT_SDCARD_Test(panel) != 1){
		sprintf(test_number, "SDCard");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	if(DECT_Audio_Headset_Test(panel) != 1){
		sprintf(test_number, "Audio_Headset");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	} 
	
	//Audio Tests
	////////////////////////
	
	sprintf(msg,"Close Test Fixture Cover, then Press OK.");
	Display_OK_MSG(msg);
	
	
	while(DECT_Check_Final_Fixture_Status_Test(panel) != 1){
		sprintf(msg,"Close Test Fixture Cover, then Press OK.");
		Display_OK_MSG(msg);
		if(counter == 5){
			sprintf(test_number, "Fixture_Cover");
			DECT_Clean_Shutdown_Sequence(panel); 
			return 0;
		}
		counter = counter + 1; 
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel); 
	 	return 0;
	}
	
	if(DECT_Audio_MIC_Test (panel) != 1){
		sprintf(test_number, "Audio_MIC");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel); 
	 	return 0;
	}
	

	if(DECT_Audio_Speaker_Test (panel) != 1){
		sprintf(test_number, "Audio_SPK");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	if(DECT_Audio_EP_Test (panel) != 1){
		sprintf(test_number, "Audio_EP");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}

	
	if(DECT_Audio_Trembler_Test(panel) != 1){
		sprintf(test_number, "Audio_Trembler");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	
	//CMD60 Tests
	////////////////////////
	if(Open_CMD60 (panel) != 1){
		sprintf(test_number, "Open_CMD60"); 
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1) 
		return 0; 
	
	
	if(Set_CMD60_in_Loopback_Mode_Radiated_Setting (panel) != 1){
		sprintf(test_number, "CMD60_Loopback");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1) 
		return 0; 
	

	if(DECT_Get_SW_Version_Test (panel) != 1){
		sprintf(test_number, "SW_Version_Test");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1) 
		return 0;
	
	
	if(DECT_Park_Search_Criteria_Test (panel) != 1){
		sprintf(test_number, "Park_Search_Test");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1) 
		return 0;
	
	
	if(DECT_EEPROM_PLI_1_Test  (panel) != 1){
		sprintf(test_number, "PLI_1_Test");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1) 
		return 0;
	
	
	if(DECT_EEPROM_Park_1_Test  (panel) != 1){
		sprintf(test_number, "Park_1_Test");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1) 
		return 0;
	
	
	if(DECT_Enter_Test_Mode (panel) != 1){
		sprintf(test_number, "Enter_Test_Mode");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1) 
		return 0;
	
	
	if(DECT_Get_Service_State_Test(panel) != 1){
		sprintf(test_number, "Get_Service_State");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1) 
		return 0;
	
	
	if(Setup_Connect_CMD60(panel) != 1){
		sprintf(test_number, "Setup_Connect_CMD60");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1) 
		return 0;
	
	 
	Delay(5);

	if(DECT_Measure_NTP_Power_Radiated_Test(panel) != 1){
		sprintf(test_number, "NTP_RAD_Power");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1) 
		return 0;

	
	if(Close_CMD60(panel) != 1){
		sprintf(test_number, "Close_CMD60");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1) 
		return 0;  
	

	if(DECT_Read_Device_MAC_Address_Test(panel) != 1){
		sprintf(test_number, "Read_Device_MAC_Address");
		DECT_Clean_Shutdown_Sequence(panel);  
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	CloseCom(COM_port);
	
	sprintf(msg,"Open Test Fixture Cover, then Press OK.");
	Display_OK_MSG(msg);
	
	//USB and Bluetooth Tests
	DECT_Connect_USB_Cable(panel);
	
	sprintf(msg,"Close Test Fixture Cover, then Press OK.");
	Display_OK_MSG(msg);
	
		while(DECT_Check_Final_Fixture_Status_Test(panel) != 1){
		sprintf(msg,"Close Test Fixture Cover, then Press OK.");
		Display_OK_MSG(msg);
		if(counter == 5){
			sprintf(test_number, "Fixture_Cover");
			DECT_Clean_Shutdown_Sequence(panel); 
			return 0;
		}
		counter = counter + 1; 
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel); 
	 	return 0;
	}
	if(DECT_Enter_Test_Mode_USB (panel) != 1){
		sprintf(test_number, "Enter_Test_Mode_USB");
		DECT_Clean_Shutdown_USB_Sequence(panel);  
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	}

	
	/*if(DECT_Get_SW_Version_USB_Test (panel) != 1){
		sprintf(test_number, "SW_Version_USB");
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	}*/
	
			   
	if(DECT_Enter_Bluetooth_Test_Mode_USB (panel) != 1){
		sprintf(test_number, "Blutooth_Test_Mode");
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_Enter_Bluetooth_Test_Mode_USB (panel) != 1){
		sprintf(test_number, "Blutooth_Test_Mode");
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	}
	
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	}
	
	if(Open_MT8852B (panel) != 1){
		sprintf(test_number, "Open_MT8852B ");
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	} 

	
	if(Check_Sync_MT8852B (panel) != 1){
		sprintf(test_number, "Check_Sync_MT8852B "); 
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	}
	
	
	if(Set_MT8852B_Loopback_Mode_Radiated (panel) != 1){
		sprintf(test_number, "Set_MT8852B_Loopback_Mode_Radiated"); 
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	}
	
	
	if(Run_Scripts_MT8852B (panel) != 1){
		sprintf(test_number, "Run_Scripts_MT8852B");
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	} 
	

	if(Measure_Output_Power_MT8852B (panel) != 1){
		sprintf(test_number, "Output_Power_MT8852B");
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	} 
	
	if(Close_MT8852B (panel) != 1){
		sprintf(test_number, "Close_MT8852B ");
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_Disable_Bluetooth_USB_Test (panel) != 1){
		sprintf(test_number, "Disable Bluetooth"); 
		return 0;
	} 
	
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel);
		return 0;
	}

	
	sprintf(msg,"Open Test Fixture Cover, then Press OK.");
	Display_OK_MSG(msg);
	
	DECT_Disconnect_USB_Cable(panel); 
	
	
	if(Open_COM_PORT_1(panel) != 1){
		sprintf(test_number, "COM Switch");
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_EEPROM_WRITE_FINAL_DATE_Test(panel) != 1){
		sprintf(test_number, "Write_Final_Date_Test");
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_EEPROM_WRITE_PASS_Final_Test (panel) != 1){
		sprintf(test_number, "Write_PASS_Test");
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	if(DECT_Make_Backup_Test (panel) != 1){
		sprintf(test_number, "Make_Backup_Test");
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	} 
	
	
	if(DECT_Master_Reset_Test (panel) != 1){
		sprintf(test_number, "Master_Reset_Test"); 
		return 0;
	} 
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);     
		return 0;
	}
	
	Delay(3); 

	if(DECT_Power_Off_Test (panel) != 1){
		sprintf(test_number, "Power_Off_Test"); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		return 0;
	}
	
	Delay(3); 
	

	CloseCom(COM_port);
	
	
	/*if(DECT_Power_Off_USB_Test (panel) != 1){
		sprintf(test_number, "Power_Off_USB_Test");
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		return 0;
	}*/
	
	
	//sprintf(msg,"Press the red power button until the Switch Off screen appears, then select YES.\n Wait for corner LED to stop flashing then press OK when power down is complete.");
	//Display_OK_MSG(msg); 
	
  return 1;
	
}

///////////////////////////////////////////////////////////////////////////////////////
//Vendor SN Fixes
///////////////////////////////////////////////////////////////////////////////////////


//*************************************************************
//* 
//*
//*************************************************************
int DECT_612d_Final_Serial_Number_Sequence(int panel, char *test_number)
{
	char  buffer[MAXLINE+1] = {'\0'},  msg_buffer[MAXLINE+128];  
	double delay_time = 600;
	int status = 0;
	int timeout = 0;
	int i = 0;
	char msg[256] = {'\0'}; 

	
	sprintf(buffer,"Starting DECT FINAL Serial Number Repair Tests\n");  
	SetCtrlVal (panel, PANEL_Serial_Window_1, buffer);
	
	
	sprintf(msg,"Power on the Phone manually. Use the ESC key on the phone to enter Blue Screen.\n\n Press OK when Blue Screen is visible."); 
	Display_OK_MSG(msg); 
	
	
	if(DECT_Enter_Enhanced_Test_Mode (panel) != 1){
		sprintf(test_number, "Enter_Enhanced_Test_Mode"); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1) 
		return 0;
	
	
	if(DECT_Read_Error_Counter_Test (panel) != 1){
		sprintf(test_number, "Read_Error_Counter_Test"); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1) 
		return 0;
	
	
	if(DECT_Write_Serial_Number_Final_Test (panel) != 1){
		sprintf(test_number, "Make_Backup_Test");
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	} 
	
	
	if(DECT_Make_Backup_Test (panel) != 1){
		sprintf(test_number, "Make_Backup_Test");
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	} 
	
	
	if(DECT_Master_Reset_Test (panel) != 1){
		sprintf(test_number, "Master_Reset_Test"); 
		return 0;
	} 
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);     
		return 0;
	}
	
	Delay(3); 

	if(DECT_Power_Off_Test (panel) != 1){
		sprintf(test_number, "Power_Off_Test"); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		return 0;
	}
	
	Delay(3); 
	

	CloseCom(COM_port);
	
	
  return 1;
	
}

//*************************************************************
//* 
//*
//*************************************************************
int DECT_622d_Final_Serial_Number_Sequence(int panel, char *test_number)
{
	char  buffer[MAXLINE+1] = {'\0'},  msg_buffer[MAXLINE+128];  
	double delay_time = 600;
	int status = 0;
	int timeout = 0;
	int i = 0;
	char msg[256] = {'\0'}; 

	
	sprintf(buffer,"Starting DECT FINAL Serial Number Repair Tests\n");  
	SetCtrlVal (panel, PANEL_Serial_Window_1, buffer);
	
	
	sprintf(msg,"Power on the Phone manually. Use the ESC key on the phone to enter Blue Screen.\n\n Press OK when Blue Screen is visible."); 
	Display_OK_MSG(msg); 
	
	
	if(DECT_Enter_Enhanced_Test_Mode (panel) != 1){
		sprintf(test_number, "Enter_Enhanced_Test_Mode"); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1) 
		return 0;
	
	
	if(DECT_Read_Error_Counter_Test (panel) != 1){
		sprintf(test_number, "Read_Error_Counter_Test"); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1) 
		return 0;
	
	
	if(DECT_Write_Serial_Number_Final_Test (panel) != 1){
		sprintf(test_number, "Make_Backup_Test");
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	} 
	
	
	if(DECT_Make_Backup_Test (panel) != 1){
		sprintf(test_number, "Make_Backup_Test");
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	} 
	
	
	if(DECT_Master_Reset_Test (panel) != 1){
		sprintf(test_number, "Master_Reset_Test"); 
		return 0;
	} 
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);     
		return 0;
	}
	
	Delay(3); 

	if(DECT_Power_Off_Test (panel) != 1){
		sprintf(test_number, "Power_Off_Test"); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		return 0;
	}
	
	Delay(3); 
	

	CloseCom(COM_port);
	
	
  return 1;
	
}



//*************************************************************
//* 
//*
//*************************************************************
int DECT_632d_Final_Serial_Number_Sequence(int panel, char *test_number)
{
	char  buffer[MAXLINE+1] = {'\0'},  msg_buffer[MAXLINE+128];  
	double delay_time = 600;
	int status = 0;
	int timeout = 0;
	int i = 0;
	char msg[256] = {'\0'}; 

	
	sprintf(buffer,"Starting DECT FINAL Serial Number Repair Tests\n");  
	SetCtrlVal (panel, PANEL_Serial_Window_1, buffer);
	
	
	sprintf(msg,"Power on the Phone manually. Use the ESC key on the phone to enter Blue Screen.\n\n Press OK when Blue Screen is visible."); 
	Display_OK_MSG(msg); 
	
	
	if(DECT_Enter_Enhanced_Test_Mode (panel) != 1){
		sprintf(test_number, "Enter_Enhanced_Test_Mode"); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1) 
		return 0;
	
	
	if(DECT_Read_Error_Counter_Test (panel) != 1){
		sprintf(test_number, "Read_Error_Counter_Test"); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1) 
		return 0;
	
	
	if(DECT_Write_Serial_Number_Final_Test (panel) != 1){
		sprintf(test_number, "Make_Backup_Test");
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	} 
	
	
	if(DECT_Make_Backup_Test (panel) != 1){
		sprintf(test_number, "Make_Backup_Test");
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	} 
	
	
	if(DECT_Master_Reset_Test (panel) != 1){
		sprintf(test_number, "Master_Reset_Test"); 
		return 0;
	} 
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);     
		return 0;
	}
	
	Delay(3); 

	if(DECT_Power_Off_Test (panel) != 1){
		sprintf(test_number, "Power_Off_Test"); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		return 0;
	}
	
	Delay(3); 
	

	CloseCom(COM_port);
	
	
  return 1;
	
}

/////////////////////////////////////////////////////////////////
//Enhanced Tests
/////////////////////////////////////////////////////////////////
//*************************************************************
//* 
//*
//*************************************************************
int DECT_Enhanced_Test_Sequence(int panel, char *test_number)
{
	char  buffer[MAXLINE+1] = {'\0'},  msg_buffer[MAXLINE+128];  
	double delay_time = 600;
	int status = 0;
	int timeout = 0;
	int i = 0;
	char msg[256] = {'\0'}; 
	
	
	sprintf(buffer,"Starting DECT ENHANCED Tests\n");  
	SetCtrlVal (panel, PANEL_Serial_Window_1, buffer);
	
	
	
	Delay(2);
	
	//Start Tests
	if(DECT_Enter_Enhanced_Test_Mode (panel) != 1){
		sprintf(test_number, "Enter_Enhanced_Test_Mode"); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1) 
		return 0;
	
	
	
	if(DECT_Read_Error_Counter_Test (panel) != 1){
		sprintf(test_number, "Read_Error_Counter_Test");
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	
		if(Error_Counter != 0){
		if(DECT_Master_Reset_Test (panel) != 1){
			sprintf(test_number, "Master_Reset_Test"); 
			return 0;
		} 
		
		
		if(DECT_Power_Off_Test (panel) != 1){
			sprintf(test_number, "Power_Off_Test"); 
			return 0;
		}
	
		ProcessSystemEvents ();
		if (Stop_Test_Exe == 1) 
			return 0; 
	
	
		if(Close_COM_Port_Relays () != 0){
			sprintf(test_number, "COM_PORT_Relays"); 
			return 0;
		}
	
		return 0;
		
	}
	
	
    if(DECT_Audio_MIC_Test (panel) != 1){
		sprintf(test_number, "Audio_MIC");
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel); 
	 	return 0;
	}
	

	if(DECT_Audio_Speaker_Test (panel) != 1){
		sprintf(test_number, "Audio_SPK");
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	if(DECT_Audio_EP_Test (panel) != 1){
		sprintf(test_number, "Audio_EP");
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}

	
	if(DECT_Audio_Trembler_Test(panel) != 1){
		sprintf(test_number, "Audio_Trembler");
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	
	/*if(DECT_Audio_Headset_Test(panel) != 1){
		sprintf(test_number, "Audio_Headset");
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}*/


	if(DECT_Make_Backup_Test (panel) != 1){
		sprintf(test_number, "Make_Backup_Test"); 
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0; 
	}

	
	if(DECT_Power_Off_Test (panel) != 1){
		sprintf(test_number, "Power_Off_Test");
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){
		return 0; 
	}


	CloseCom(COM_port);
	sprintf(msg,"Disconnect USB Cable.");
	Display_OK_MSG(msg); 
	

	
	
  return 1;

}


/////////////////////////////////////////////////////////////////
//Enhanced Tests
/////////////////////////////////////////////////////////////////
//*************************************************************
//* 
//*
//*************************************************************
int DECT_Enhanced_User_Test_Sequence(int panel, char *test_number)
{
	char  buffer[MAXLINE+1] = {'\0'},  msg_buffer[MAXLINE+128];  
	double delay_time = 600;
	int status = 0;
	int timeout = 0;
	int i = 0;
	char msg[256] = {'\0'}; 
	
	
	sprintf(buffer,"Starting DECT ENHANCED USER Tests\n");  
	SetCtrlVal (panel, PANEL_Serial_Window_1, buffer);
	
	
	/*if(DECT_Parse_SN (panel) != 1){
		sprintf(test_number, "Parse_SN"); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1) 
		return 0;
	*/
	
	
	Delay(2);
	
	//Start Tests
	if(DECT_Enter_Enhanced_Test_Mode (panel) != 1){
		sprintf(test_number, "Enter_Enhanced_Test_Mode"); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1) 
		return 0;
	
	
	
	if(DECT_Read_Error_Counter_Test (panel) != 1){
		sprintf(test_number, "Read_Error_Counter_Test");
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	
		if(Error_Counter != 0){
		if(DECT_Master_Reset_Test (panel) != 1){
			sprintf(test_number, "Master_Reset_Test"); 
			return 0;
		} 
		
		
		if(DECT_Power_Off_Test (panel) != 1){
			sprintf(test_number, "Power_Off_Test"); 
			return 0;
		}
	
		ProcessSystemEvents ();
		if (Stop_Test_Exe == 1) 
			return 0; 
	
	
		if(Close_COM_Port_Relays () != 0){
			sprintf(test_number, "COM_PORT_Relays"); 
			return 0;
		}
	
		return 0;
		
	}
	
/*	if(DECT_USB_Test (panel) != 1){
		sprintf(test_number, "USB_Test"); 
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0; 
	}
	
	
	if(DECT_Audio_Headset_Test(panel) != 1){
		sprintf(test_number, "Audio_Headset");
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
*/	
	
	if(DECT_LCD_POS_Test(panel) != 1){
		sprintf(test_number, "LCD_POS");
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	if(DECT_LCD_RED_Test(panel) != 1){
		sprintf(test_number, "LCD_RED");
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	if(DECT_LCD_GREEN_Test(panel) != 1){
		sprintf(test_number, "LCD_GREEN");
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	
	if(DECT_LCD_BLUE_Test(panel) != 1){
		sprintf(test_number, "LCD_BLUE");
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}

	
	if(DECT_LED_RED_Test(panel) != 1){
		sprintf(test_number, "LED_RED");
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	if(DECT_LED_GREEN_Test(panel) != 1){
		sprintf(test_number, "LED_GREEN");
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
		
	if(DECT_LED_ORANGE_Test(panel) != 1){
		sprintf(test_number, "LED_ORANGE");
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	
	if(DECT_LED_KEYB_Test(panel) != 1){
		sprintf(test_number, "LED_KEYB");
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
		
	
	if(DECT_KEY22_Test(panel) != 1){
		sprintf(test_number, "KEY22");
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	} 
	
	if(DECT_SDCARD_Test(panel) != 1){
		sprintf(test_number, "SDCard");
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	
	if(DECT_Make_Backup_Test (panel) != 1){
		sprintf(test_number, "Make_Backup_Test"); 
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0; 
	}

	
	if(DECT_Power_Off_Test (panel) != 1){
		sprintf(test_number, "Power_Off_Test");
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){
		return 0; 
	}


	CloseCom(COM_port);
	sprintf(msg,"Disconnect USB Cable.");
	Display_OK_MSG(msg); 
	

	
	
  return 1;

}




//*************************************************************
//* 
//*
//*************************************************************
int DECT_Enhanced_Version_Test_Sequence(int panel, char *test_number)
{
	char  buffer[MAXLINE+1] = {'\0'},  msg_buffer[MAXLINE+128];  
	double delay_time = 600;
	int status = 0;
	int timeout = 0;
	int i = 0;
	char msg[256] = {'\0'}; 
	
	
	sprintf(buffer,"Starting DECT ENHANCED USER Tests\n");  
	SetCtrlVal (panel, PANEL_Serial_Window_1, buffer);
	
	
	/*if(DECT_Parse_SN (panel) != 1){
		sprintf(test_number, "Parse_SN"); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1) 
		return 0;
	*/
	
	
	Delay(2);
	
	//Start Tests
	if(DECT_Enter_Enhanced_Test_Mode (panel) != 1){
		sprintf(test_number, "Enter_Enhanced_Test_Mode"); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1) 
		return 0;
	
	
	
	if(DECT_Read_Error_Counter_Test (panel) != 1){
		sprintf(test_number, "Read_Error_Counter_Test");
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	
		if(Error_Counter != 0){
		if(DECT_Master_Reset_Test (panel) != 1){
			sprintf(test_number, "Master_Reset_Test"); 
			return 0;
		} 
		
		
		if(DECT_Power_Off_Test (panel) != 1){
			sprintf(test_number, "Power_Off_Test"); 
			return 0;
		}
	
		ProcessSystemEvents ();
		if (Stop_Test_Exe == 1) 
			return 0; 
	
	
		if(Close_COM_Port_Relays () != 0){
			sprintf(test_number, "COM_PORT_Relays"); 
			return 0;
		}
	
		return 0;
		
	}
	
	/*if(DECT_Write_IPEI_USB_Test (panel) != 1){
		sprintf(test_number, "IPEI_USB_Test"); 
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0; 
	}*/
	

	if(DECT_VERS_SW_Test (panel) != 1){
		sprintf(test_number, "VERS_SW_Test"); 
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0; 
	}
	
	
	if(DECT_VERS_LOADER_Test (panel) != 1){
		sprintf(test_number, "VERS_LOADER_Test"); 
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0; 
	}
	
	if(DECT_VERS_HW_Test (panel) != 1){
		sprintf(test_number, "VERS_HW_Test"); 
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0; 
	}
	
	
	if(DECT_VERS_BRAND_Test(panel) != 1){
		sprintf(test_number, "VERS_BRAND_Test"); 
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0; 
	}
	
	if(DECT_VERS_ATS_Test(panel) != 1){
		sprintf(test_number, "VERS_ATS_Test"); 
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0; 
	}
	
	if(DECT_VERS_IP65_Test(panel) != 1){
		sprintf(test_number, "VERS_IP65_Test"); 
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0; 
	}
	
	if(DECT_VERS_SNO_Test(panel) != 1){
		sprintf(test_number, "VERS_SNO_Test"); 
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0; 
	}
	
	
	if(DECT_VERS_FINAL_Test(panel) != 1){
		sprintf(test_number, "VERS_FINAL_Test"); 
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0; 
	}
	
	
	if(DECT_VERS_IPEI_Test(panel) != 1){
		sprintf(test_number, "VERS_IPEI_Test"); 
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0; 
	}
	
	if(DECT_Make_Backup_Test (panel) != 1){
		sprintf(test_number, "Make_Backup_Test"); 
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0; 
	}

	
	if(DECT_Power_Off_Test (panel) != 1){
		sprintf(test_number, "Power_Off_Test");
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){
		return 0; 
	}


	CloseCom(COM_port);
	sprintf(msg,"Disconnect USB Cable.");
	Display_OK_MSG(msg); 
	

	
	
  return 1;

}



//*************************************************************
//* 
//*
//*************************************************************
int DECT_Enhanced_Test_Sequence_2(int panel, char *test_number)
{
	char  buffer[MAXLINE+1] = {'\0'},  msg_buffer[MAXLINE+128];  
	double delay_time = 600;
	int status = 0;
	int timeout = 0;
	int i = 0;
	char msg[256] = {'\0'}; 
	
	
	sprintf(buffer,"Starting DECT ENHANCED Tests\n");  
	SetCtrlVal (panel, PANEL_Serial_Window_1, buffer);
	
	
	/*if(DECT_Parse_SN (panel) != 1){
		sprintf(test_number, "Parse_SN"); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1) 
		return 0;
	*/
	
	sprintf(msg,"Power on Phone and Close Test Fixture Cover");
	Display_OK_MSG(msg); 
	
	
	Delay(2);
	
	//Start Tests
	if(DECT_Enter_Enhanced_Test_Mode (panel) != 1){
		sprintf(test_number, "Enter_Enhanced_Test_Mode"); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1) 
		return 0;
	
	
	
	if(DECT_Read_Error_Counter_Test (panel) != 1){
		sprintf(test_number, "Read_Error_Counter_Test");
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	
		if(Error_Counter != 0){
		if(DECT_Master_Reset_Test (panel) != 1){
			sprintf(test_number, "Master_Reset_Test"); 
			return 0;
		} 
		
		
		if(DECT_Power_Off_Test (panel) != 1){
			sprintf(test_number, "Power_Off_Test"); 
			return 0;
		}
	
		ProcessSystemEvents ();
		if (Stop_Test_Exe == 1) 
			return 0; 
	
	
		if(Close_COM_Port_Relays () != 0){
			sprintf(test_number, "COM_PORT_Relays"); 
			return 0;
		}
	
		return 0;
		
	}
	
	
    if(DECT_Audio_MIC_Test (panel) != 1){
		sprintf(test_number, "Audio_MIC");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel); 
	 	return 0;
	}
	

	if(DECT_Audio_Speaker_Test (panel) != 1){
		sprintf(test_number, "Audio_SPK");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	if(DECT_Audio_EP_Test (panel) != 1){
		sprintf(test_number, "Audio_EP");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}

	
	if(DECT_Audio_Trembler_Test(panel) != 1){
		sprintf(test_number, "Audio_Trembler");
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	
	/*if(DECT_Audio_Headset_Test(panel) != 1){
		sprintf(test_number, "Audio_Headset");
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}*/


	if(DECT_Make_Backup_Test (panel) != 1){
		sprintf(test_number, "Make_Backup_Test"); 
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0; 
	}

	
	if(DECT_Power_Off_Test (panel) != 1){
		sprintf(test_number, "Power_Off_Test");
		DECT_Clean_Shutdown_Sequence(panel);
		return 0;
	}
	
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){
		return 0; 
	}


	CloseCom(COM_port);
	sprintf(msg,"Open Test Fixture Cover");
	Display_OK_MSG(msg); 
	

	
	
  return 1;

}



//*************************************************************
//* 
//*
//*************************************************************
int DECT_Enhanced_User_Test_Sequence_2(int panel, char *test_number)
{
	char  buffer[MAXLINE+1] = {'\0'},  msg_buffer[MAXLINE+128];  
	double delay_time = 600;
	int status = 0;
	int timeout = 0;
	int i = 0;
	char msg[256] = {'\0'}; 
	
	
	sprintf(buffer,"Starting DECT ENHANCED USER Tests\n");  
	SetCtrlVal (panel, PANEL_Serial_Window_1, buffer);
	
	
	/*if(DECT_Parse_SN (panel) != 1){
		sprintf(test_number, "Parse_SN"); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1) 
		return 0;
	*/
	
	sprintf(msg,"Power on Phone and Close Test Fixture Cover");
	Display_OK_MSG(msg); 
	
	
	Delay(2);
	
	//Start Tests
	if(DECT_Enter_Enhanced_Test_Mode (panel) != 1){
		sprintf(test_number, "Enter_Enhanced_Test_Mode"); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1) 
		return 0;
	
	
	
	if(DECT_Read_Error_Counter_Test (panel) != 1){
		sprintf(test_number, "Read_Error_Counter_Test");
		DECT_Clean_Shutdown_Sequence(panel);  
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	
		if(Error_Counter != 0){
		if(DECT_Master_Reset_Test (panel) != 1){
			sprintf(test_number, "Master_Reset_Test"); 
			return 0;
		} 
		
		
		if(DECT_Power_Off_Test (panel) != 1){
			sprintf(test_number, "Power_Off_Test"); 
			return 0;
		}
	
		ProcessSystemEvents ();
		if (Stop_Test_Exe == 1) 
			return 0; 
	
	
		if(Close_COM_Port_Relays () != 0){
			sprintf(test_number, "COM_PORT_Relays"); 
			return 0;
		}
	
		return 0;
		
	}
	
	
	if(DECT_USB_Test (panel) != 1){
		sprintf(test_number, "USB_Test"); 
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){
		DECT_Clean_Shutdown_Sequence(panel);  
		return 0; 
	}
	
	
	if(DECT_Audio_Headset_Test(panel) != 1){
		sprintf(test_number, "Audio_Headset");
		DECT_Clean_Shutdown_Sequence(panel);  
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel); 
		return 0;
	}
	
	
	if(DECT_LCD_POS_Test(panel) != 1){
		sprintf(test_number, "LCD_POS");
		DECT_Clean_Shutdown_Sequence(panel);  
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);  
		return 0;
	}
	
	if(DECT_LCD_RED_Test(panel) != 1){
		sprintf(test_number, "LCD_RED");
		DECT_Clean_Shutdown_Sequence(panel);  
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);  
		return 0;
	}
	
	if(DECT_LCD_GREEN_Test(panel) != 1){
		sprintf(test_number, "LCD_GREEN");
		DECT_Clean_Shutdown_Sequence(panel);  
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);  
		return 0;
	}
	
	
	if(DECT_LCD_BLUE_Test(panel) != 1){
		sprintf(test_number, "LCD_BLUE");
		DECT_Clean_Shutdown_Sequence(panel);  
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);  
		return 0;
	}

	
	if(DECT_LED_RED_Test(panel) != 1){
		sprintf(test_number, "LED_RED");
		DECT_Clean_Shutdown_Sequence(panel);  
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);  
		return 0;
	}
	
	if(DECT_LED_GREEN_Test(panel) != 1){
		sprintf(test_number, "LED_GREEN");
		DECT_Clean_Shutdown_Sequence(panel);  
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);  
		return 0;
	}
		
	if(DECT_LED_ORANGE_Test(panel) != 1){
		sprintf(test_number, "LED_ORANGE");
		DECT_Clean_Shutdown_Sequence(panel);  
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);  
		return 0;
	}
	
	
	if(DECT_LED_KEYB_Test(panel) != 1){
		sprintf(test_number, "LED_KEYB");
		DECT_Clean_Shutdown_Sequence(panel);  
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);  
		return 0;
	}
		
	
	if(DECT_KEY22_Test(panel) != 1){
		sprintf(test_number, "KEY22");
		DECT_Clean_Shutdown_Sequence(panel);  
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);  
		return 0;
	} 
	
	if(DECT_SDCARD_Test(panel) != 1){
		sprintf(test_number, "SDCard");
		DECT_Clean_Shutdown_Sequence(panel);  
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_Sequence(panel);  
		return 0;
	}
	
	
	if(DECT_Make_Backup_Test (panel) != 1){
		sprintf(test_number, "Make_Backup_Test"); 
		DECT_Clean_Shutdown_Sequence(panel);  
		return 0;
	}
	
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){
		DECT_Clean_Shutdown_Sequence(panel);  
		return 0; 
	}

	
	if(DECT_Power_Off_Test (panel) != 1){
		sprintf(test_number, "Power_Off_Test");
		DECT_Clean_Shutdown_Sequence(panel);  
		return 0;
	}
	
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){
		return 0; 
	}


	CloseCom(COM_port);
	//sprintf(msg,"Disconnect USB Cable.");
	//Display_OK_MSG(msg); 
	

	
	
  return 1;

}


//*************************************************************
//* 
//*
//*************************************************************
int DECT_Enhanced_Version_Test_Sequence_2(int panel, char *test_number)
{
	char  buffer[MAXLINE+1] = {'\0'},  msg_buffer[MAXLINE+128];  
	double delay_time = 600;
	int status = 0;
	int timeout = 0;
	int i = 0;
	char msg[256] = {'\0'}; 
	
	
	sprintf(buffer,"Starting DECT ENHANCED USER Tests\n");  
	SetCtrlVal (panel, PANEL_Serial_Window_1, buffer);
	
	
	/*if(DECT_Parse_SN (panel) != 1){
		sprintf(test_number, "Parse_SN"); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1) 
		return 0;
	*/
	
	
	Delay(2);
	
	//Start Tests
	if(DECT_Enter_Enhanced_Test_Mode (panel) != 1){
		sprintf(test_number, "Enter_Enhanced_Test_Mode"); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1) 
		return 0;
	
	
	
	if(DECT_Read_Error_Counter_Test (panel) != 1){
		sprintf(test_number, "Read_Error_Counter_Test");
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){ 
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	
		if(Error_Counter != 0){
		if(DECT_Master_Reset_Test (panel) != 1){
			sprintf(test_number, "Master_Reset_Test"); 
			return 0;
		} 
		
		
		if(DECT_Power_Off_Test (panel) != 1){
			sprintf(test_number, "Power_Off_Test"); 
			return 0;
		}
	
		ProcessSystemEvents ();
		if (Stop_Test_Exe == 1) 
			return 0; 
	
	
		if(Close_COM_Port_Relays () != 0){
			sprintf(test_number, "COM_PORT_Relays"); 
			return 0;
		}
	
		return 0;
		
	}
	
	/*if(DECT_Write_IPEI_USB_Test (panel) != 1){
		sprintf(test_number, "IPEI_USB_Test"); 
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0; 
	}*/
	

	if(DECT_VERS_SW_Test (panel) != 1){
		sprintf(test_number, "VERS_SW_Test"); 
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0; 
	}
	
	
	if(DECT_VERS_LOADER_Test (panel) != 1){
		sprintf(test_number, "VERS_LOADER_Test"); 
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0; 
	}
	
	if(DECT_VERS_HW_Test (panel) != 1){
		sprintf(test_number, "VERS_HW_Test"); 
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0; 
	}
	
	
	if(DECT_VERS_BRAND_Test(panel) != 1){
		sprintf(test_number, "VERS_BRAND_Test"); 
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0; 
	}
	
	if(DECT_VERS_ATS_Test(panel) != 1){
		sprintf(test_number, "VERS_ATS_Test"); 
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0; 
	}
	
	if(DECT_VERS_IP65_Test(panel) != 1){
		sprintf(test_number, "VERS_IP65_Test"); 
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0; 
	}
	
	if(DECT_VERS_SNO_Test(panel) != 1){
		sprintf(test_number, "VERS_SNO_Test"); 
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0; 
	}
	
	
	if(DECT_VERS_FINAL_Test(panel) != 1){
		sprintf(test_number, "VERS_FINAL_Test"); 
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0; 
	}
	
	
	if(DECT_VERS_IPEI_Test(panel) != 1){
		sprintf(test_number, "VERS_IPEI_Test"); 
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0; 
	}
	
	if(DECT_Make_Backup_Test (panel) != 1){
		sprintf(test_number, "Make_Backup_Test"); 
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0; 
	}

	
	if(DECT_Power_Off_Test (panel) != 1){
		sprintf(test_number, "Power_Off_Test");
		DECT_Clean_Shutdown_USB_Sequence(panel); 
		return 0;
	}
	
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1){
		return 0; 
	}


	CloseCom(COM_port);
//	sprintf(msg,"Disconnect USB Cable.");
	Display_OK_MSG(msg); 
	

	
	
  return 1;

}


//*************************************************************
//* 
//*
//*************************************************************
int DECT_Clean_Shutdown_Sequence(int panel){

	if(DECT_EEPROM_WRITE_FAIL_Test (panel) != 1){
		return 0;
	}
	
	if(DECT_Power_Off_Test (panel) != 1){
		return 0;
	}
	
	if(Close_COM_Port_Relays () != 0){
		return 0;
	}

	CloseCom(COM_port);

	if(Power_OFF_DECT () != 0){
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1) 
		return 0; 
	
	return 1;

}




//*************************************************************
//* 
//*
//*************************************************************
int DECT_Clean_Shutdown_USB_Sequence(int panel){
	
	char msg[256] = {'\0'}; 
	
	if(DECT_EEPROM_WRITE_FAIL_USB_Test (panel) != 1){
		return 0;
	}
	

	if(DECT_Power_Off_USB_Test (panel) != 1){
		return 0;
	}
	

	CloseCom(COM_port_2);
	//sprintf(msg,"Disconnect USB Cable.");
	//Display_OK_MSG(msg); 
	
	DECT_Disconnect_USB_Cable(panel); 
	
	
	if(Power_OFF_DECT () != 0){
		return 0;
	}
	
	ProcessSystemEvents ();
	if (Stop_Test_Exe == 1) 
		return 0; 

	
 	return 1;

}




////////////////////////////////////////////////////////
///////////////////////////////////////////////////////
