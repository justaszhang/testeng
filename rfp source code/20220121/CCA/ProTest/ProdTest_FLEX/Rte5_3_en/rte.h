#ifndef RTE_H
#define RTE_H


#define QUIT_ID					0xffff

#define	TESTVERSION			"rte protech, release: 5.3 (06/2002)"
#define DEVELOPERACCOUNT	"Developer"
//#define SYSTEM_PFAD		"c:\\test\\rte\\"
//#define UIR_PFAD			"c:\\test\\rte\\rte_uir.uir"
//#define ADAPTERCODE_PFAD	"c:\\test\\adaptercode\\adapter.txt"
//#define RTE_SETTINGS_PFAD	"rte_settings.txt"

//there is a problem during load from external Module-->get fkt-address, do an dummy-load !
#define DUMMY_LOAD			"c:\\test\\pool\\pool.obj"
#define DUMMY_FUNCTION		"check_value"

//Declarationen bezüglich teststruktur:
#define MAXMODULES			10


#define SINGLETEST				1
#define SINGLEPASS				2
#define RUNUUT					3


#define		MAXMENUEFCT			20		  // from 10 to 20 @GC 23.08.2011

#define 	MAXSTRINGLEN		120



//#include "Ts.h"
#include <utility.h>
#include <ansi_c.h>
#include <userint.h>
#include <cvirte.h>		// Needed if linking in external compiler; harmless otherwise 
#include "rte_uir.h"
#include "rtepubl.h"
#include <rs232.h>



FILE 		*fa_r,
			*fa_w;

char 		token[MAXTOKEN][MAXSTRINGLEN],
			line[MAXSTRINGLEN];

char 		*line_ptr;
char		rte_settings_path[MAXPATHNAMELEN+1];
char		uir_path[MAXPATHNAMELEN+1];
double 		system_offsets[MAXSYSTEM_OFFSETS];
struct 		seqence_parameters left_sequ,right_sequ; 


struct test_flow 	test_flow;	//defined in rte_public
struct rte_globals	global;			// global variables of test environment
struct data_backup data_backup;


struct struct_sequ_group
	{
	char lines[600][MAXSTRINGLEN];
	char **lines_ptr;
	int line_count;
	};

struct menue_functions
	{
	int function_count;
	int menu_id[MAXMENUEFCT];
	int (*menue_fctptr[MAXMENUEFCT])(void);	//pointer to function
	char functionname[MAXMENUEFCT][40];			
	char function_path[MAXMENUEFCT][MAXPATHNAMELEN+1];			
	}menue_function, menue_functionL, menue_functionR;

//-------- Funktionen ---------------------
char *get_line(FILE *fa_r,char line[]);
int parse(char line_ptr[]);

void run_adaptercode(short porta, short portb);
void *get_fctaddress(char *pfad, char *module,char *fktname);
int get_sequence(char *spfad,char window_pos, int do_startup);
int testexecutive (int mode);
void set_menu1_attr(void);    
void fill_sequdisplay(int mode);
void fill_sequ(int index,int mode);
void edit_sequence(int mode);
void fill_proto(char pfad[]);
char get_file_group(struct struct_sequ_group *sequ_group,FILE *fa_r,char group[]);
char Sequ_Getpara (struct struct_sequ_group *sequ_group,char tag[],void *parameter,int type);
char get_buffer_group(struct struct_sequ_group *sequ_group,char *fileBuffer,char group[]);

#endif
