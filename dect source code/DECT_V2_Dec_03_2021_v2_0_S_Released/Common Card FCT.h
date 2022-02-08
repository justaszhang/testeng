

#ifndef cardtest

#define DEFAULT_READLEN 1024

#define MAXLINE 10*4096   
#define UCT_GPIB 03 
#define MT8852B_GPIB 27
#define CMD60_GPIB 01
#define AGILENT_SIM_GPIB 5



int COM_port, COM_port_2;   
int Stop_Test_Exe;
int Test_In_Process;
int Monitor_Channel;
int Number_Of_Cards;
int PSU_Cards_Ilimit;
int Log_File_Handle;
int Audio_File_Handle;
int Test_File_Handle;
int COM_cfg_File_Handle;
int tester_cfg_File_Handle; 
int rfoffset_cfg_File_Handle; 
int btoffset_cfg_File_Handle;  
int mem_cfg_File_Handle; 
int DUPLEX_RUN;
int DUP_LNK_TST;
int	DPLX_Tested;
int CELLBUS_RUN;
int Stopped;
int Terminated_Early;
int Re_Transmit;
int Time_Displayed;
int Card_Index;
int Stop_In_Process;
int Test_Type;
int Printer_Port_On;
int debug;
int network_enable;
int VT100_Filter;
int apc_present_in;
int kernel_silent;
char temp_string[36];  
int lid_position;
int test_fixture;
int debug;
int debug_File_Handle; 
int results_File_Handle; 
int FAIL_Continue;

int panelHandle, QuitHandle,Scan_Serial_Handle, Scan_Test_Handle, StoppingHandle, StopHandle, DTS_Handle, Scan_MAC_Handle ;
char  serial_num[20];
char log_file[256];
char log_file_2[256]; 
char Scan_Serial[64];
char Scan_MAC[20];
char Station_ID[128];

char rf_EquipType[128];
int irfEquipmentType;

char mandownsensor[128];
int imandownsensor;



char Part_Number[128];
char Network_Path[256];
char Scan_Test[20];
char Prompt[10];
int Card_Type_Number;
int Card_Numbers[100];
int FCT_DELAY[100];
int BOOT_DELAY[100];
char COM_Index[2];
char COM_Index_2[2]; 
char android_time_2[2];
char log_file_debug[256]; 
char log_file_results[256];
char No_FAIL[4];
char test_time_2[64];
char Product_Prefix[4];


int outlog;
int outfile;
int CARD_STATE;
int Serial_Num_Read;
int MAC_Address_Read;
int Test_Num_Read; 
int Upgrade_DTS;
int Upgrade_Select;
int COM_port_cfg;
int COM_port_cfg_2; 
int android_time;
int long_form_final;

enum STATUS{PASS,FAIL,INCOMP};
char Start_Date_Time[256];
double start_of_test_time;
typedef char pscs[18];
typedef char prompts[10];

char card_prompts[100][10], card_pscs[100][18];

char Statusbuf[256];
 
void Get_Start_Time(int panel);
void Display_OK_MSG(char *msg); 
void Display_YES_MSG(char *msg);
void Flush_COM_1 (void);
void Set_DAQ_IO(void); 
 
 
int MSG_FLAG_1;
int MSG_YES_FLAG;
int MSG_YES_PASS;

void Toggle_LabJack_Pin(void);

//COM
int Open_COM_PORT_1(int panel);
int Open_COM_PORT_2(int panel); 


//DAC
int EIO0_On(void);
int EIO0_Off(void);

int EIO1_On(void);
int EIO1_Off(void);

int EIO2_On(void);
int EIO2_Off(void);

int EIO3_On(void);
int EIO3_Off(void);

int EIO4_On(void);
int EIO4_Off(void);

int EIO5_On(void);
int EIO5_Off(void);

int EIO6_On(void);
int EIO6_Off(void);

int EIO7_On(void);
int EIO7_Off(void);

int EIALL_Off(void);

//Barrel Jack Power
int FIO0_On(void);
int FIO0_Off(void);


//DC Power Supply
int Aquarius_DC_On(void); 
int Aquarius_DC_Off(void); 

//DAQ
int Setup_DAQ(int panel); 

//logging
char data_string[2048]; 
char GUI_Revision[10];
char global_log_path[256];


//DECT
void print_hex_memory(int panel, void *mem, int size);
int Power_ON_DECT(void);
int Power_OFF_DECT(void);
int Open_COM_Port_Relays(void);  
int Close_COM_Port_Relays(void); 
int Serial_Port_Scan (int panel);
int DECT_Send_String(int panel, unsigned char *string_to_send, unsigned char *expected_string, char *read_string, int check_length, int verbose, int delay); 
int DECT_Send_String_USB(int panel,unsigned char *string_to_send, unsigned char *expected_string, char *read_string, int check_length, int verbose, int delay);
int DECT_Send_String_USB_V2(int panel,unsigned char *string_to_send, unsigned char *expected_string, char *read_string, int check_length, int verbose, int delay); 


int DECT_Read_String(int panel, int minlen,char *read_string,int * readlen, int verbose, int timeout) ;
//int DECT_Parse_SN(int panel);

//DECT_TESTS
int DECT_Enter_Test_Mode(int panel);
int DECT_Write_HW_Release_Test (int panel);
int DECT_File_System_Test(int panel);
int DECT_Reset_EEPROM_Test_Flow_Test (int panel);
int DECT_SD_Card_Test(int panel);
int DECT_Make_Backup_Test(int panel);
int DECT_Power_Off_Test(int panel);
int DECT_Write_IPEI_Test(int panel);
int DECT_Write_Serial_Number_Test(int panel);
int DECT_Branding_Check_Test(int panel);
int DECT_Branding_Set_Test(int panel);
int DECT_P1V8_Voltage_Adjustment_Test(int panel);
int DECT_P3V_Voltage_Measure_Test(int panel);   
int DECT_Activate_Ref_Clk_Test(int panel);
int DECT_Set_Clk_Test(int panel);
int DECT_Get_SW_Version_Test_612 (int panel);
int DECT_Get_SW_Version_Test_622 (int panel);   
int DECT_Get_SW_Version_Test_632 (int panel);   
int DECT_Get_HW_Version_Test (int panel,int iType);

int DECT_Park_Search_Criteria_Test (int panel);
int DECT_EEPROM_PLI_1_Test (int panel);
int DECT_EEPROM_Park_1_Test (int panel);
int DECT_Get_Service_State_Test (int panel);
int DECT_Measure_NTP_Power_Test (int panel);
int DECT_Measure_EU_Deviation_Test (int panel);
int DECT_Measure_Frequency_Offset_Test (int panel);
int DECT_Measure_Frequency_Drift_Test (int panel);
int DECT_Measure_RSSI_45dBm_Test (int panel);
int DECT_Measure_RSSI_75dBm_Test (int panel);
int DECT_Program_RSSI_K_Factor_Test (int panel); 
int DECT_Program_RSSI_Offset_Test (int panel);
int DECT_Measure_Receiver_Sensitivity_Test (int panel);
int DECT_Power_Template_Match_Test(int panel);
int DECT_Read_Device_MAC_Address_Test (int panel);
int DECT_Read_Error_Counter_Test(int panel);
int DECT_Master_Reset_Test(int panel);
int DECT_Measure_NTP_Power_Radiated_Test(int panel);
int DECT_Set_RF_Board_Serial_Number(int panel,int control); 
int DECT_EEPROM_WRITE_PASS_Test(int panel);
int DECT_EEPROM_WRITE_FAIL_Test(int panel);
int DECT_EEPROM_LED_Test(int panel);
int DECT_EEPROM_SOS_Test(int panel);
int DECT_EEPROM_HOTKEY_Test(int panel); 
int DECT_Set_RF_Final_Serial_Number(int panel,int control);
int DECT_EEPROM_WRITE_DATE_Test(int panel);
int DECT_Verify_MAC_Address_Test (int panel);


//DECT_TESTS_USB
int DECT_Write_IPEI_USB_Test(int panel);
int DECT_Enter_Test_Mode_USB(int panel);
int DECT_Enter_Bluetooth_Test_Mode_USB(int panel); 
int DECT_Get_SW_Version_USB_Test (int panel);
int DECT_Bluetooth_Sync_Clock_USB_Test(int panel);
int DECT_Bluetooth_Get_Analog_Trim_USB_Test(int panel);
int DECT_Bluetooth_Write_Analog_Trim_USB_Test(int panel);


int DECT_Bluetooth_Read_ClockAdjust_USB_Test(int panel) ;
int DECT_Power_Off_USB_Test(int panel);
int DECT_Make_Backup_USB_Test(int panel);
int DECT_Write_MAC_Address_USB_Test(int panel);
int DECT_EEPROM_LED_USB_Test(int panel);
int DECT_EEPROM_SOS_USB_Test(int panel);
int DECT_EEPROM_HOTKEY_USB_Test(int panel);
int DECT_EEPROM_VIBRATOR_USB_Test(int panel);
int DECT_EEPROM_LOUDNESS_USB_Test(int panel);
int DECT_EEPROM_WRITE_DATE_USB_Test(int panel);
int DECT_EEPROM_WRITE_PASS_USB_Test(int panel);
int DECT_EEPROM_WRITE_FAIL_USB_Test(int panel);


//DECT Misc.
int DECT_Connect_USB_Cable(int panel);
int DECT_Disconnect_USB_Cable(int panel); 
int WriteToTerminal(int panel, char *buffer);
int Switch_DECT_USB_COM_OFF(void); 
int Switch_DECT_USB_COM_ON(void); 

//DECT Variables
char Scan_Data[4][32];
char Serial_Global[32];
char IPEI_Global[32];
char MAC_Global[32];
char MAC_Global_2[32]; 
char Product_Serial_Global[32];
char Bluetooth_Analog_Trim[1];
int Error_Counter;

//DECT Frequency Counter
int Init_Counter (int panel);
int TRIGGER_HP53131A_TO_MEASURE(void);
int Frequency_Measurement(int panel);

//DECT Bluetooth Tester
int Init_MT8852B (int panel);
int Open_MT8852B (int panel);
int Close_MT8852B (int panel); 
int Check_Sync_MT8852B (int panel);
int Set_MT8852B_Loopback_Mode (int panel);
int Set_MT8852B_Loopback_Mode_Radiated (int panel);
int Measure_Output_Power_MT8852B (int panel);
int Measure_Modulation_Index_MT8852B (int panel); 
int Measure_Carrier_Frequency_Tolerance_MT8852B (int panel);
int Measure_Carrier_Frequency_Drift_MT8852B (int panel);
int Measure_Sensitive_Single_Slot_Packets_MT8852B (int panel);
int Check_BTADDR_MT8852B (int panel);
int Check_EUTADDR_MT8852B (int panel); 
int Check_BTADDR_MT8852B_2 (int panel);
int Run_Scripts_MT8852B (int panel);


//DECT CMD60 
int Init_CMD60 (int panel);
int Open_CMD60 (int panel);
int Set_CMD60_in_Loopback_Mode_EU_Setting(int panel);
int Set_CMD60_in_Loopback_Mode_Radiated_Setting(int panel);   
int Close_CMD60 (int panel);
int Setup_Connect_CMD60 (int panel);
int RSSI_45dBm;
int RSSI_75dBm;


//NEW DECT TESTS
//audio
int DECT_Enter_Enhanced_Test_Mode(int panel);
int DECT_Audio_Speaker_Test(int panel);
int DECT_Audio_MIC_Test(int panel);
int DECT_Audio_EP_Test(int panel);
int DECT_Audio_Trembler_Test(int panel);

//user
int DECT_Audio_Headset_Test(int panel, int iRepeat);
int DECT_USB_Test(int panel);
int DECT_LCD_RED_Test(int panel);
int DECT_LCD_GREEN_Test(int panel);
int DECT_LCD_BLUE_Test(int panel);
int DECT_LCD_POS_Test(int panel);
int DECT_LED_RED_Test(int panel);
int DECT_LED_GREEN_Test(int panel);
int DECT_LED_ORANGE_Test(int panel);
int DECT_LED_KEYB_Test(int panel);
int DECT_SDCARD_Test(int panel);
int DECT_KEY12_Test(int panel); 
int DECT_KEY22_Test(int panel);
int DECT_KEY32_Test(int panel);

//version
int DECT_VERS_SW_Test(int panel); 
int DECT_VERS_LOADER_Test(int panel);
int DECT_VERS_HW_Test(int panel);
int DECT_VERS_BRAND_Test(int panel);
int DECT_VERS_ATS_Test(int panel);
int DECT_VERS_IP65_Test(int panel);
int DECT_VERS_SNO_Test(int panel);
int DECT_VERS_FINAL_Test(int panel);
int DECT_VERS_IPEI_Test(int panel);


//misc
int get_prefix_code(int panel);
char Scan_Final_Fixture_Serial[20];
double end_of_test_time; 
int DECT_EEPROM_WRITE_PASS_USB_Final_Test(int panel); 
int DECT_EEPROM_WRITE_PASS_Final_Test(int panel); 
int DECT_MAN_DOWN_SENSOR_Test(int panel);
int DECT_EEPROM_WRITE_FINAL_DATE_USB_Test(int panel);
int DECT_EEPROM_WRITE_FINAL_DATE_Test(int panel);

//misc BT
int Clear_BT_Tester(void);
int Check_Sync_BT_Tester(void);  
int DECT_Disable_Bluetooth_USB_Test(int panel);
int DECT_Master_Reset_USB_Test(int panel);

//misc
int DECT_Write_Serial_Number_Final_Test(int panel);
int DECT_Vendor_SN_Test(int panel);
int Check_Sync_MT8852B_2 (int panel); 
int Set_MT8852B_Loopback_Mode_2 (int panel);
int Set_MT8852B_Loopback_Mode_Radiated_2 (int panel); 
int DECT_Serial_Number_String_Test(int panel);
int DECT_Serial_Number_String_Test_2(int panel);
int DECT_Check_Final_Fixture_Status_Test(int panel);


//Agilent
int Init_Agilent(int panel);
int Set_Agilent_LeakCurr(int panel);
int Measure_Agilent_InsCurr(int panel) ;
int Measure_Agilent_LeakCurr(int panel) ;
int Set_Agilent_CharCurr(int panel);
int Measure_Agilent_CharCurr(int mode, int panel) ;
int Set_Agilent_Off(int panel)  ;
int Measure_Agilent_PwrOnCurr(int panel) ;
int DECT_SetChargeCurrentMode_0_USB(int panel) ;
int DECT_SetChargeCurrentMode_1_USB(int panel);
int DECT_DisableCharger_USB(int panel);
int DECT_GetChargerStatus_USB(int panel)  ;
int DECT_GetChargerStatus(int panel) ;
int Set_Agilent_NormalWorkVolt(int panel)    ;

int DECT_Check_Serial_Number_String_Test(int panel)  ;





// Audio
int DECT_Audio_TremblerTest_Test_Ex(int panel, int iRepeat) ;  
int DECT_Audio_EarpieceTest_Test_Ex(int panel, int iRepeat)  ;
int DECT_Audio_Speaker_Test_Ex(int panel, int iRepeat)  ; 

int DECT_Audio_TestReset_Test(int panel) ;
int DECT_Audio_TestPass_Test(int panel)  ;
int DECT_Trembler_TestReset_Test(int panel) ;
int DECT_Trembler_TestPass_Test(int panel)  ;


//RTX
int Set_RTX_in_Loopback_Mode_EU_Setting(int panel)  ;  
int Set_RTX_Mask_Setting(int panel) ;
int Open_RTX (int panel)   ;
int Setup_Connect_RTX (int panel)    ;
int Close_RTX (int panel) ;
int DECT_Measure_RTX_NTP_Power_Test (int panel);
int DECT_Measure_RTX_EU_Deviation_Test (int panel)     ;
int DECT_Measure_RTX_Frequency_Offset_Test (int panel)    ;
int DECT_Measure_RTX_Frequency_Drift_Test (int panel)  ;
int DECT_Measure_RTX_RSSI_45dBm_Test (int panel) ;
int DECT_Measure_RTX_RSSI_75dBm_Test (int panel) ;
int DECT_Measure_RTX_Receiver_Sensitivity_Test (int panel)     ;
int DECT_RTX_Power_Template_Match_Test(int panel)      ;
int Reset_RTX(int panel);


//CMD60 Power Mask
int Set_CMD60_Mask_Setting(int panel);

#define DTS 10
 
#define cardtest 
#endif

