void Get_Start_Time(int panel);
int print_TRACS_results(int panel);
int Get_Common_Serial_Numbers(int panel);
int read_card_serial(int panel, int com_port, char *serial_num, char *psc_rel_num);

int Open_Files_For_Write(int panel);
int Close_Data_Files(int panel);
int Open_Files_For_Write(int panel);
void Write_Log_File(char *msg, int panel);
void Write_Audio_Log_File(char *msg, int panel);
int Check_MAN_Info(int panel);
;
int Build_Menu(int panel);
int Config_Boot_Line(int panel); 
void save_panel(int in_panel);
int Run_FCT(int panel, char *test_number); 
int Run_FCT_2(int panel, char *test_number);

int card_delay;

//DECT
int DECT_612d_CCA_Test_Sequence(int panel, char *test_number); 
int DECT_622d_CCA_Test_Sequence(int panel, char *test_number);  
int DECT_632d_CCA_Test_Sequence(int panel, char *test_number);  
int DECT_CCA_Test_Sequence(int panel, char *test_number); 
int DECT_612d_Final_Test_Sequence(int panel, char *test_number); 
int DECT_622d_Final_Test_Sequence(int panel, char *test_number); 
int DECT_632d_Final_Test_Sequence(int panel, char *test_number); 
int DECT_Enhanced_Test_Sequence(int panel, char *test_number);
int DECT_Enhanced_User_Test_Sequence(int panel, char *test_number);
int DECT_Enhanced_Version_Test_Sequence(int panel, char *test_number);
int DECT_Enhanced_Test_Sequence_2(int panel, char *test_number);
int DECT_Enhanced_User_Test_Sequence_2(int panel, char *test_number);
int DECT_Enhanced_Version_Test_Sequence_2(int panel, char *test_number);
int DECT_Clean_Shutdown_Sequence(int panel);
int DECT_Clean_Shutdown_USB_Sequence(int panel);
int DECT_Final_Test_Sequence(int panel, char *test_number);  
int DECT_612d_Final_Serial_Number_Sequence(int panel, char *test_number);  
int DECT_622d_Final_Serial_Number_Sequence(int panel, char *test_number);  
int DECT_632d_Final_Serial_Number_Sequence(int panel, char *test_number);  


////////////////////////
int QUICK_READ_SERIAL (char *search_string, char *return_string, int COM_Port, int timeout);
int QUICK_SEND_SERIAL (char *send_string, int COM_Port);
int QUICK_READ_SERIAL_2 (char *search_string, char *return_string, int COM_Port, int timeout);
int QUICK_SEND_SERIAL_2 (char *send_string, int COM_Port);
int SEND_DATA_BYTES (int panel,  int com_port, int delay_time, char *send_string, char *response, char *return_string);

