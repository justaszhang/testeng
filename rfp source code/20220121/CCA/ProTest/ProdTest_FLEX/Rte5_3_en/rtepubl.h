#ifndef RTE_PUBL_H
#define RTE_PUBL_H

#define		MAXSYSTEM_OFFSETS   20
#define 	HF_OFFSET			1
#define 	VMU_OFFSET			2
#define 	DCS_OFFSET			3


#define 	MAXSTRINGLEN		120		// default windows

#define 	MAXPATHNAMELEN		260		// default windows
#define		MAXFUNCTIONNAMEL	30		// 
#define 	MAXPARAINT			10		// 
#define		MAXPARADOUBLE		10		// 
#define		MAXPARASTRING		4		// 
#define		MAXUNITLEN			80


#define		COL_NEUTRAL	0xc0c0c0
#define		COL_GREEN	0x00ff00
#define		COL_RED		0xff0000
#define		COL_YELLOW	0xffff00
//#define     COL_BLUE    0x0000ff
#define     COL_BLUE    0x4080ff


#define		PASS		0
#ifndef		FAILED
#define		FAILED		1
#endif
#define MAXANZAHLFUNCTION 	100

#define	LEFT	1
#define RIGHT	2

#define 	PARA_INTEGER		1
#define 	PARA_DOUBLE			2
#define 	PARA_STRING			3
#define 	PARA_STRING_LONG 	4
#define 	PARA_HEX 			5
#define 	MAXTOKEN			50




typedef int	TESTFUNCTION; 


struct struct_file
	{
	char lines[360][MAXSTRINGLEN];
	int line_count;
	};

struct test_flow
	{
	int	runuut_left;
	int running_left;
	int	ready_left;
	int ioerror_left;
	int	start_left;
	int	runuut_right;
	int running_right;
	int ready_right;
	int ioerror_right;
	int	start_right;
	int position;
	int	hoodclosed_left;
	int	hoodclosed_right;
	int	error_code;		// for local handling io-error message
	int	dummy1;
	char *test_name;	// for local handling io-error
	};


struct data_backup
	{
	char command_line[MAXPATHNAMELEN+1];				
	char command_line_with_arg_left[2*MAXPATHNAMELEN+1];
	char command_line_with_arg_right[2*MAXPATHNAMELEN+1];
	char init_left;
	char init_right;
	char left_equal_right;
	int	 device_count_left;
	int	 device_count_right;
	int	 trigger_count;
	};


struct rte_globals				// global variables of test environment
	{
	struct seqence_parameters	*sequ_select;
	struct seqence_parameters	*left;
	struct seqence_parameters	*right;
	char loginname[20];
	char label_command_line[MAXPATHNAMELEN];
	char label_printer_name[MAXPATHNAMELEN];
	int iloginlevel;				// !=0 ->developerlevel
	int imainpanel;  
	int ieditpanel;
	int iloginpanel;
	int iabort;
	int debug_mode;
	int ipanelsize;
	int iclrdisplay;
	int iprintresult;
	int iprintmode;		  //
	int iservesystem;
	int showerrorrate;
	int iedittest;
	int ixposition;
	int iyposition;
	int ireference;
	int irepairmode;      // repairmode @GC 04.09.2011
	int ifullpanel;
	int idatalogmode;	  //
	int	isequ_selectmode;
	int	choice_sequ;
	int dll_mode;
	int executing;
	int	module_portB;     // rs232 port for protest communication ic2 & messurenment 
	struct test_flow *testflow;
	char rte_path[MAXPATHNAMELEN+1];
	struct data_backup *data_backup;
	char gpib_mode[32];
//free for later use: 
	int	module_portA;     // rs232 port for DUT communication
	int	dummy2;
	double dummy3;
	double dummy4;
	char system_id[MAXPATHNAMELEN+1];
	char fixtureServiceFile[MAXPATHNAMELEN+1];
	int irfEquipmentType;
	};
	

struct seqence_parameters				// variables/constants from left or right sequence
	{
	struct struct_test *structpointer[MAXANZAHLFUNCTION];
	int (*fktpointer[MAXANZAHLFUNCTION])(void *ptr);	//pointer to function
	char sequ_pfad[MAXPATHNAMELEN];	
	char product_pfad[MAXPATHNAMELEN];	
	char sachnumber[80];	
	char dll_pfad[MAXPATHNAMELEN];	
	char fixtureID[30];			
	char printerport[MAXPATHNAMELEN];		
	char log_file[MAXPATHNAMELEN];
	char startup_mod[MAXPATHNAMELEN];			
	char startup_fct[30];			
	char cleanup_mod[MAXPATHNAMELEN];			
	char cleanup_fct[30];			
	char flow_mod[MAXPATHNAMELEN];			
	char ioerr_fct[30];			
	char duterr_mod[MAXPATHNAMELEN];			
	char duterr_fct[30];			
	char ioerr_mod[MAXPATHNAMELEN];			
	char flow_fct[30];			
	int (*duterr)(void *ptr);
	int (*ioerr)(void *ptr);
	int (*flow)(void *ptr);
	int (*startup)(void *ptr);
	int (*cleanup)(int mode);
	char user_dir[MAXPATHNAMELEN];
	int	maxerror;
	int	maxerror_shadow;				// for re-login 
	int executeable_tests;				// executable tests in sequence
	int productnumber_handle;				//
	int sequdisplay_handle;  			//
	int sequencdisp_handle;
	int numtests_handle;
	int sequname_handle;
	int	sernum_handle;
	int	errorrate_handle;
	char sernumber[80];
	int status_handle;
	int	hint_handle;
	int	runuut_handle;
	int	singlepass_handle;
	int idevicefail;
	int idevicecount;
	int	act_index;
	int	last_logged_index;
	int act_loopcount;
	int jmp_absolut;
	int ierror;
	int	last_result;
	int io_error;
	};


struct struct_test		   //parameter from single test
	{
	char *modulename;
	char *errormessage;
	char *string[MAXPARASTRING];
	double system_offsets[MAXSYSTEM_OFFSETS];
	char fkt_name[MAXFUNCTIONNAMEL];
	char username[MAXFUNCTIONNAMEL];
	int	loopcount;
	int precond;
	int runmode;
	char unit[MAXUNITLEN];
	int prec;	
	int	switch_sequ;
	double lowerlimit;
	double upperlimit;
	double resultoffset;
	int intpara[MAXPARAINT];
	double doublepara[MAXPARADOUBLE];
	int resultstatus;
	};

int adapter_counter_clear(struct rte_globals *globals, int intervalnr);
int adapter_counter_update(struct rte_globals *globals, int count);
int adapter_counter_read(struct rte_globals *globals, int intervalnr, int *serviceCount, int *devCount, int *serviceDays, char *lastServiceDate, char *nextServiceDate, char *serviceText);



//never remove this from here !! 
struct test_flow *testflow;
struct rte_globals *globals;				// global variables of test environment
char 	token[MAXTOKEN][MAXSTRINGLEN];
char 	printkopf[260];		// test report header

#endif
