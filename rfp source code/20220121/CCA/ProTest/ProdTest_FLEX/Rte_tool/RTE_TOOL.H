#ifndef RTE_TOOL_H
#define RTE_TOOL_H

#define EEPROM_SPECIAL_RANGE	0x4000
#define IO_ERROR				"io_error: 0x%x"				

#define LOG_SEPERATOR		"/"
#define LOGSEPERATOR		'/'
#define PRINTER_CUT_STRING	0x1b,0x50,0x0
#define FINAL 		1
#define FUNKTION 	2

int iFkt_Final;
char cTimeLabel[20];
char cDateLabel[20];



#undef DEBUG_TIME_MEAS                  // mit Zeitmessung per Test

#ifdef DEBUG_TIME_MEAS
time_t gl_timeBegin;
time_t gl_timeBeginTest;
time_t gl_timeEndTest;
#endif


int print_baco(void);
int print_out (char print_str[]);
int printer_strobe(int port_address);
void set_statuswindow(char *buffer,long background,int select);
void set_hintwindow(char *buffer,long background, int select);
void print_resultdisplay(char buffer[]);
void set_serial_number(int number);
void set_char_serial_number(char *number);
void set_testheader(int mode); 	//
void set_sequence(int select); 		// switch to other sequence
void goto_test(char test_username[]);
void logging_io_error(int error_code);   // moved from tool.h to rte_tool.h  @GC 30.08.2011
int check_value(double resultvalue);
int check_value_str(char *resultvalue);
int check_value_str_cmp(char *resultvalue, char *strExpected);  // GC 31.03.2011
int check_value_TXT_cmp(char *resultvalue, char *strExpected);  // GC 29.03.2012
int check_other_value(char name[],double lowerlimit,double resultvalue,double upperlimit,int prec,char unit[],char err_messg[]);
int setPassAndLog(char *logStr);
#ifndef RFP35_PROJECT
void set_RepairMode(int iRepairMode);
#endif
int	isInProductionMode(void);	 //new @GC27.05.2011
int	isInReferenceMode(void);	 //new @GC20.02.2012
int	isInDebugMode(void);		 //new @GC27.05.2011
int isInRepairMode(void);		 //new @GC05.09.2011

void set_menu_printmode(int on_off);
int get_testresult(int select);
int print_logfile(char *path);
int backup_logdata_files(struct rte_globals *globals);

#endif //RTE_TOOL_H

