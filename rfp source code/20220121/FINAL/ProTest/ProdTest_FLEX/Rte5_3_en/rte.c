#include "rte.h"


unsigned char ResultStr[0x100];     

char *get_line(FILE *fa_r,char line[])
{
int i=0,j;

	if(!feof(fa_r))
		{					
		fgets(line,MAXSTRINGLEN,fa_r);
		j = strlen(line);
		if(j)
			{
			for(i=0;i<j;i++)
				if(line[i] != 0x20) break;	   //first spaces raus
			return(&line[i]);
			}
		return(&line[0]);
		}
	return NULL;
}

int parse(char line_ptr[])
{
int token_ind,i,j,index;

	for(i=0;i<MAXTOKEN;i++)
		token[i][0] = 0;
	token_ind = 0;
	index = 0;
	i = strlen(line_ptr);
	for(j=0;j<i;j++)
		{
		token[token_ind][index] = line_ptr[j];  //copy line_ptr to actual token until
		if(line_ptr[j] == '=')
			{
			token[token_ind][index] = 0;
			token_ind++;
			index = 0;
//remove first spaces from token:
			j++;
			while(line_ptr[j] == ' ') j++;
			j--;
			}
		else	
			index++;
		}
	token[token_ind][index] = 0;
	return token_ind;
}

//*************** my tools end ****************************************




char get_file_group(struct struct_sequ_group *sequ_group,FILE *fa_r,char group[])
{
int i,j;
char group_flag;

//struct struct_sequ_group  *psequ_group;
//	psequ_group = (struct struct_sequ_group*) sequ_group;
	
	for(i=0;i<320;i++)
		sequ_group->lines[i][0]=0;
	i = 0;		
	
	group_flag = 0;
	while(get_line(fa_r,line)!= NULL)
		{
		if(group_flag)
			{
			if(!strncmp(line,"[end group]",11)) break;
			j = strlen(line);
			for(;j>0;j--)
				{
				if(line[j] < 0x20) line[j] = 0;
				}
			strcpy(&sequ_group->lines[i][0],line);
			i++;
			}
		if(!strncmp(line,group,strlen(group)))
			group_flag = 1;
		}
	sequ_group->line_count = i;			
	return(group_flag);
}

//Funktion in arbeit
char get_buffer_group(struct struct_sequ_group *sequ_group,char *fileBuffer,char group[])
{
int i,j,iZeilen_Anzahl,n,l;
char group_flag;
char *helpster;
//************************************************
char	*orgBuffer;
//struct struct_sequ_group  *psequ_group;
//	psequ_group = (struct struct_sequ_group*) sequ_group;
	l = strlen(fileBuffer);
	orgBuffer =  fileBuffer;
	iZeilen_Anzahl = 0;
	while(helpster =  strchr(fileBuffer,10))
		{	
		iZeilen_Anzahl++;
		fileBuffer = (helpster +1);
		}
	sequ_group->lines_ptr = (char**)malloc(iZeilen_Anzahl * sizeof(char**));
	fileBuffer = orgBuffer;
	n=0;
	group_flag = 0;
	while(helpster =  strchr(fileBuffer,10))
		{	
		*helpster = NULL;
		j = strlen(fileBuffer);
		if(j)
			{
			for(i=0;i<j;i++)
				if(fileBuffer[i] != 0x20) break;	   //first spaces raus
			}
		if(group_flag)
			{
			if(!strncmp(fileBuffer,"[end group]",11)) break;
/*
			j = strlen(line);
			for(;j>0;j--)
				{
				if(fileBuffer[j] < 0x20) fileBuffer[j] = 0;
				}
*/
			sequ_group->lines_ptr[n] = fileBuffer;
			fileBuffer = (helpster +1);
			n++;
			}
		else
			{
			fileBuffer = (helpster +1);
			}
		if(!strncmp(fileBuffer,group,strlen(group)))
			{
			fileBuffer = (helpster +1);
			group_flag = 1;
			}
		}

	sequ_group->line_count = n;			
	return(group_flag);
}




char Sequ_Getpara (struct struct_sequ_group *sequ_group,char tag[],void *parameter,int type)
{
int 	i,j,k,flag,parameter_number;
int 	*int_ptr;
int 	*hex_ptr;
double 	*doub_ptr;
char	*char_ptr;
unsigned char module_addr;

	switch(type)	//set default all zero
		{
		case PARA_INTEGER:
			 int_ptr = parameter;
			*int_ptr = 0;
			break;
		case PARA_HEX:
			 hex_ptr = parameter;
			*hex_ptr = 0;
			break;
		case PARA_DOUBLE:
			 doub_ptr = parameter;
			*doub_ptr = 0;
			break;
		case PARA_STRING:
			char_ptr = parameter;
			*char_ptr = 0;
			break;
		}

	flag = 0;
	for(i=0;i<sequ_group->line_count;i++)
		{
		if(!strncmp(&sequ_group->lines[i][0],tag,strlen(tag)))
			{
			parse(&sequ_group->lines[i][0]);
			
#define WITHOUT_ADAPTER_TAG_PARAMETER			 //@GC 17.02.2012 wird nicht gebraucht
#ifndef WITHOUT_ADAPTER_TAG_PARAMETER			
//check for adapter-parameter: AdapterTag[p001]
			if(!strncmp(&token[1][0],"AdapterTag",10))
				{
				j = 10;
/*	old: modul[0x10,1]			
				while(token[1][j] != 'x') j++;
				j++;
				sscanf(&token[1][j],"%x",&j);
				module_addr = (unsigned char)j;
				j = 5;
				
				while(token[1][j] != ',') j++;
				j++;
*/				
//new: [p001]
				while(token[1][j] != 'p') j++;
				j++;
				parameter_number = atoi(&token[1][j]);
				parameter_number--;
				j = 3;
				module_addr = ADAPTER_MODULE_ADDR;
				while(j)
					{
					if(!module_read_para(module_addr, ADAPTER_TAG_I2C_ADDR, parameter_number, &token[1][0],0))
						break;
					Delay(0.1);
					j--;
					}
				if(!j)
					{
					MessagePopup ("Run - Adaptercode Error","Can't read parameter from module !!");
					QuitUserInterface(0);
////					token[1][0] = 0;					
					}
				}
#endif //WITHOUT_ADAPTER_TAG_PARAMETER			

//-----replace token ready ------------------------------------------
			flag = 1;
			switch(type)
				{
				case PARA_INTEGER:
					 int_ptr = parameter;
					*int_ptr = atoi(&token[1][0]);
					break;
				case PARA_HEX:
					hex_ptr = parameter;
					sscanf(&token[2][0],"%x",hex_ptr);
					break;
				case PARA_DOUBLE:
					 doub_ptr = parameter;
					*doub_ptr = atof(&token[1][0]);
					break;
				case PARA_STRING:
					char_ptr = parameter;
					j = 0;
					while(token[1][j] == 0x20) j++;
					strcpy(char_ptr,&token[1][j]);
					sequ_group->lines[i][0] = 0;	//line served, clear line
					break;
				}
			break; 
			}
		}
	return(flag);
}

//damit da niemand einfach rumfummelt !!
#include "callback.c"

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int MiniSysRte_write (char write_char[])
{
int bytesWritten, count, localError=0;
char rxBuffer[0x200];

//	FlushOutQ (MiniSysPort); 
	count = strlen(write_char);
	FlushInQ (global.module_portB);	
    bytesWritten = ComWrt (global.module_portB, write_char, count);
//    bytesWritten += ComWrtByte (MiniSysPort,0x0a);
    bytesWritten += ComWrtByte (global.module_portB,0x0d);
// check mirrored data:	
	count = ComRd (global.module_portB,rxBuffer,bytesWritten);
    if(count != bytesWritten) localError++;
    return(localError);
    
}

int MiniSysRte_read (char read[])
{
int bytesRead;
    Delay(0.05); // USB needs time for answer ;(
    bytesRead = GetInQLen (global.module_portB);
//    ComRdTerm (global.module_portB, read, 10, '\r');
    ComRd (global.module_portB, read, bytesRead);
    read[bytesRead] = 0; //terminate string
	return(bytesRead);
}


int MiniSysRte_get_SID (char* systemID)
{
int i;
char *p;
//   MiniSys_write ("SID?");
   MiniSysRte_write ("EEPR:1");
   if(!MiniSysRte_read (ResultStr))
   		return(-1);
// remove trailing control-char
	i = strlen(ResultStr);
	while(i && ((ResultStr[i] < 0x20) || ResultStr[i] == 0x3a))  
	{	
		ResultStr[i] = 0;
		i--;
	}
// remove leading control-char	
	p = ResultStr;
	while(*p < 0x20 || *p == 0x3a) p++;
	strcpy(systemID, p);
	return(0);
}


int MiniSysRte_get_AdapterID (char* adapterID)  //Achtung:hexadecimal. high-byte für porta (links), lowbyte für portb (rechts)
{
int i;

   MiniSysRte_write ("EEPR:2");
   if(!MiniSysRte_read (ResultStr))
   		return(-1);
	sscanf(ResultStr,"%x", &i);
	return(i);
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++



#include "mencallb.c"
//+++++++++++++++++++++++++++++++++++++


void run_adaptercode(short porta, short portb)
{
struct struct_sequ_group adapter_group;
char temp_str[MAXPATHNAMELEN+1];
char adapter_str[MAXPATHNAMELEN+1];
char serviceText[256];
char lastServiceDate[20];
char nextServiceDate[20];
char selected_line[60];
char cInfo[512];
unsigned char ucBuffer[0x100+1];
char *p;
int i,l,j,k,r,panel,str_pos;
FILE *fa_r;
int iZeilen;
int anzahlbyte;
long int datei_start,datei_end;
char *pDateibuffer;
int  daynow,daylast;
int  serviceCount,devCount,serviceDays,intervalNr;


	fa_r = fopen(rte_settings_path,"r");	//
    if(!fa_r) 	
    	{
		strcat(rte_settings_path," - not found !");
		MessagePopup ("System-Settings Load Error", rte_settings_path);
    	return;
    	}
	panel = LoadPanel (0, uir_path, SEQU_SEL);
//new for Sprache++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	SetCtrlAttribute(panel, SEQU_SEL_OKAY, ATTR_LABEL_TEXT, "__load selected sequence");
	SetCtrlAttribute(panel, SEQU_SEL_PRODUCTNUMBER, ATTR_LABEL_TEXT, "Please Enter Number:");
	SetCtrlAttribute(panel, SEQU_SEL_QUIT, ATTR_LABEL_TEXT,"__Quit / no sequence");
//get globel settings 
	get_file_group(&adapter_group,fa_r,"[global]");	
//panelsize
	Sequ_Getpara (&adapter_group,"panelsize",&global.ipanelsize,PARA_INTEGER);
//x-position
	Sequ_Getpara (&adapter_group,"xpanelposition",&global.ixposition,PARA_INTEGER);
//y-position
	Sequ_Getpara (&adapter_group,"ypanelposition",&global.iyposition,PARA_INTEGER);
// login-mode
	Sequ_Getpara (&adapter_group,"login_mode",&i,PARA_INTEGER);
	if(global.debug_mode)
		{
		if(global.iloginpanel)	
			HidePanel (global.iloginpanel);  					 //needed for re-login
//			RemovePopup(global.iloginpanel);
		}
	else
		{
		if(!i)
			login_callb (global.iloginpanel, 0, EVENT_COMMIT, NULL, 0, 0);
		}
		
//get modul port & read out parameters:
	Sequ_Getpara (&adapter_group,"module_portA",&global.module_portA,PARA_INTEGER);

//get modul port & read out parameters:
	Sequ_Getpara (&adapter_group,"module_portB",&global.module_portB,PARA_INTEGER);
//	if(OpenComConfig (global.module_portB, "", 921600, 0, 8, 1, 512, 512))
	if(OpenComConfig (global.module_portB, "",921600, 0, 8, 1, 512, 512)) //
	{
		//MessagePopup ("Run - Adaptercode Error","Can't read parameter from module !!");
		sprintf(temp_str,"Can't open MiniSys COM%d port!!",global.module_portB);
		MessagePopup ("Run - Adaptercode Error",temp_str);
		global.module_portB = 0;
		if(!global.debug_mode)
//		if(global.dll_mode)
		{
			QuitUserInterface(0);
			return;	   //only for rte.prj or rte.exe !!
		}
	}
	else
	{
		SetComTime (global.module_portB, 0.5);
		
Delay(1); // power up after reset 		
//------------------------------------------------------------------------------------------------------------
		
//set default:
		strcpy(global.system_id,"unknown");
		strcpy(global.left->fixtureID,"unknown");
		strcpy(global.right->fixtureID,"unknown");

		memset(ucBuffer,0x00,0x100);
//get system ID:		
		if(MiniSysRte_get_SID (ucBuffer))  		
		{
			MessagePopup ("Run - Adaptercode Error","can't read System-Tag !!");
			CloseCom (global.module_portB);
			QuitUserInterface(0);
			return;	   //only for rte.prj or rte.exe !!
		}
		ucBuffer[0x2c] = 0;		//max 30 char !
		strcpy(global.system_id,ucBuffer);

//get Adapter ID: //eigentlich Blödsinn, da die system-ID eineindeutig ist, aber aus kompatibilitätsgründen...		
		i = MiniSysRte_get_AdapterID (ucBuffer);  		
		if(i<0)
		{
			MessagePopup ("Run - Adaptercode Error","can't read System-Tag !!");
			CloseCom (global.module_portB);
			QuitUserInterface(0);
			return;	   //only for rte.prj or rte.exe !!
		}
		portb = i & 0xff;

	CloseCom (global.module_portB);
		
//------------------------------------------------------------------------------------------------------------		
		
//fixture ID:		
		strcpy(global.right->fixtureID,ucBuffer);
		
//------------------------------------------------------------------------------------------------------------		
	}	
//------------------------------------------------------------------------------------------------------------		
		

// module-read ready
//get gpib_mode:
	Sequ_Getpara (&adapter_group,"gpib_mode",&global.gpib_mode,PARA_STRING);
//log-mode
	Sequ_Getpara (&adapter_group,"datalogmode",&global.idatalogmode,PARA_INTEGER);
//print-mode
	Sequ_Getpara (&adapter_group,"printmode",&global.iprintmode,PARA_INTEGER);
//resultdisplay-mode:
	Sequ_Getpara (&adapter_group,"resultdisplaymode",&global.iprintresult,PARA_INTEGER);
	Sequ_Getpara (&adapter_group,"clearresultdisplay",&global.iclrdisplay,PARA_INTEGER);
//sequence select mode:
	Sequ_Getpara (&adapter_group,"sequence_selectmode",&global.isequ_selectmode,PARA_INTEGER);
	if(!global.dll_mode || global.debug_mode)	//select sequence with mouse not in dll-mode
		global.isequ_selectmode = 1;
//systemeventmode:
	Sequ_Getpara (&adapter_group,"systemeventmode",&global.iservesystem,PARA_INTEGER);
//errorrate:
	Sequ_Getpara (&adapter_group,"errorrateshow",&global.showerrorrate,PARA_INTEGER);
// get label-program-commandlines
   	Sequ_Getpara(&adapter_group,"label_command_line", global.label_command_line, PARA_STRING); 
   	Sequ_Getpara(&adapter_group,"label_printer_name", global.label_printer_name, PARA_STRING); 
// get logdata-backup parameters
   	Sequ_Getpara(&adapter_group,"logdata_exe",data_backup.command_line , PARA_STRING); 
	Sequ_Getpara (&adapter_group,"logdata_trigger",&data_backup.trigger_count,PARA_INTEGER);
   	Sequ_Getpara(&adapter_group,"logdata_destination", temp_str , PARA_STRING); 
//append first argument:
   	strcat(data_backup.command_line," ");
   	strcat(data_backup.command_line,temp_str);

// get adaptercode-path
   	Sequ_Getpara(&adapter_group,"adaptercodeFile", adapter_str, PARA_STRING); 
	//strcpy(adapter_str,temp_str);
	
	fclose(fa_r);
// open adapter.txt-file:
	fa_r = fopen(adapter_str,"r");	//first char: left or right
    if(!fa_r) 	
    	{
		strcat(adapter_str," - not found !");
		MessagePopup ("Run - Adaptercode Error", adapter_str);
    	return;
    	}
    else
    	{
 //////////////////////////////////////////////////////////////////////////////////////////////////
 //////////////////////////////////////////////////////////////////////////////////////////////////
		fseek(fa_r,0,SEEK_SET);
		datei_start = ftell(fa_r);
		fseek(fa_r,0,SEEK_END);
		datei_end = ftell(fa_r);
		fseek(fa_r,0,SEEK_SET);
		anzahlbyte = datei_end - datei_start;
		pDateibuffer = (char*)malloc(anzahlbyte * sizeof(char));
  		fread(pDateibuffer,sizeof(char),anzahlbyte,fa_r);
  		fseek(fa_r,0,SEEK_SET);
		fclose(fa_r);
		pDateibuffer[anzahlbyte-1] = 0;
  //////////////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////
    	}
#ifdef USE_LEFT_SIDE    	
//-----------------------------------------------------------
	if(!porta)
    	{
    	PromptPopup ("Run - Adaptercode Error", "TSU-PORTA not wired --> please enter number !", temp_str ,3);
    	porta = atoi(temp_str);
		}
	if(porta)
		{
//		get_file_group(&adapter_group,fa_r,"[left]");	
 //////////////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////
 		get_buffer_group(&adapter_group,pDateibuffer,"[left]");	
  //////////////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////
 		global.choice_sequ = 0;
		for(i=0;i<adapter_group.line_count;i++)
			{
			if(porta == atoi(adapter_group.lines_ptr[i]))		  // porta-number == line-> first char
				{
				k = 0;
				j = strlen(adapter_group.lines_ptr[i]);
				while (adapter_group.lines_ptr[i][k] != '=')
					{k++; if(k==j) break;}
				InsertListItem (panel, SEQU_SEL_SEQU_SELWIND, -1, &adapter_group.lines_ptr[i][k+2], global.choice_sequ);
				global.choice_sequ++;
				}
			}
		if(global.choice_sequ)	
			{
			SetPanelAttribute (panel, ATTR_TITLE," Please choice sequence for left Adapter / left side !!");
			InstallPopup (panel);
			SetActiveCtrl (panel, SEQU_SEL_PRODUCTNUMBER);
			global.choice_sequ = 0;
			SetCtrlVal (panel,SEQU_SEL_STATUS ,"\nPlease Enter Number");
			SetCtrlAttribute (panel,SEQU_SEL_STATUS,ATTR_TEXT_BGCOLOR,COL_BLUE);

			while(!global.choice_sequ) //is set from okay-button
				ProcessSystemEvents();
// get adaptercode-path
			if(global.choice_sequ != QUIT_ID)
				{
				
				global.choice_sequ--;
				GetLabelFromIndex (panel, SEQU_SEL_SEQU_SELWIND, global.choice_sequ, temp_str);
					
				SetCtrlVal (panel,SEQU_SEL_STATUS , "\n .. loading sequence ..");
				SetCtrlAttribute (panel,SEQU_SEL_STATUS,ATTR_TEXT_BGCOLOR,COL_YELLOW);
				ProcessDrawEvents();
				get_sequence(temp_str,'l',1); //last 0: do not startup-function
				}
			RemovePopup (0);
			set_menu1_attr();  
			DiscardPanel (panel);
			}
		}
free(pDateibuffer);
	fa_r = fopen(adapter_str,"r");	//first char: left or right
    if(!fa_r) 	
    	{
		strcat(adapter_str," - not found !");
		MessagePopup ("Run - Adaptercode Error", adapter_str);
    	return;
    	}
    else
    	{
 //////////////////////////////////////////////////////////////////////////////////////////////////
 //////////////////////////////////////////////////////////////////////////////////////////////////
		fseek(fa_r,0,SEEK_SET);
		datei_start = ftell(fa_r);
		fseek(fa_r,0,SEEK_END);
		datei_end = ftell(fa_r);
		fseek(fa_r,0,SEEK_SET);
		anzahlbyte = datei_end - datei_start;
		pDateibuffer = (char*)malloc(anzahlbyte * sizeof(char));
  		fread(pDateibuffer,sizeof(char),anzahlbyte,fa_r);
  		fseek(fa_r,0,SEEK_SET);
		fclose(fa_r);
		pDateibuffer[anzahlbyte-1] = 0;   //////////////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////
    	}
#endif //#ifdef USE_LEFT_SIDE

//second sequence		
	if(!portb)
    	{
	   	PromptPopup ("Run - Adaptercode Error","TSU-PORTB not wired --> please enter number !", temp_str ,3);
	   	portb = atoi(temp_str);
    	}
	if(portb)
		{
//		get_file_group(&adapter_group,fa_r,"[right]");	
 //////////////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////
 		get_buffer_group(&adapter_group,pDateibuffer,"[right]");	
  //////////////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////
		global.choice_sequ = 0;
		panel = LoadPanel (0, uir_path, SEQU_SEL);
//new for Sprache++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		SetCtrlAttribute(panel, SEQU_SEL_OKAY, ATTR_LABEL_TEXT, "__load selected sequence");
		SetCtrlAttribute(panel, SEQU_SEL_PRODUCTNUMBER, ATTR_LABEL_TEXT, "Please Enter Number:");
		SetCtrlAttribute(panel, SEQU_SEL_QUIT, ATTR_LABEL_TEXT,"__Quit / no sequence");

		for(i=0;i<adapter_group.line_count;i++)
			{
			if(portb == atoi(adapter_group.lines_ptr[i]))
				{
				k = 0;
				j = strlen(adapter_group.lines_ptr[i]);
				while (adapter_group.lines_ptr[i][k] != '=')
					{k++; if(k==j) break;}
				InsertListItem (panel, SEQU_SEL_SEQU_SELWIND, -1, &adapter_group.lines_ptr[i][k+2], global.choice_sequ);
				global.choice_sequ++;
				}
			}
		if(global.choice_sequ)	
			{
			SetCtrlVal (panel,SEQU_SEL_STATUS , "Please Enter Number:");
			SetCtrlAttribute (panel,SEQU_SEL_STATUS,ATTR_TEXT_BGCOLOR,COL_BLUE);
			SetPanelAttribute (panel, ATTR_TITLE," Please choice sequence for right Adapter / right side !!");
			InstallPopup (panel);
			SetActiveCtrl (panel, SEQU_SEL_PRODUCTNUMBER);			
			global.choice_sequ = 0;
			while(!global.choice_sequ) //is set from okay-button
				ProcessSystemEvents();

// get adaptercode-path
			if(global.choice_sequ != QUIT_ID)
				{

				global.choice_sequ--;
				GetLabelFromIndex (panel, SEQU_SEL_SEQU_SELWIND, global.choice_sequ, temp_str);
				
				SetCtrlVal (panel,SEQU_SEL_STATUS ,"\n .. loading sequence ..");
				SetCtrlAttribute (panel,SEQU_SEL_STATUS,ATTR_TEXT_BGCOLOR,COL_YELLOW);
				ProcessDrawEvents();
				get_sequence(temp_str,'r',1); //last 0: do not startup-function
				}
			RemovePopup (0);
			set_menu1_attr();  
			DiscardPanel (panel);
			}
		}
	free(pDateibuffer);
	return;
}


int main (int argc, char *argv[])
{
int itemp,i;
short porta,portb;	


	if(argc>1)
		{
		strcpy(rte_settings_path,argv[1]);
		strcpy(uir_path,rte_settings_path);
		i = strlen(uir_path);
		while(i)
			{
			i--;
			if(uir_path[i] == '\\')
				break;
			}
		strcpy(&uir_path[i+1],"rte_uir.uir");
		}
	else
		{
		strcpy(rte_settings_path,"rte_settings.txt");
		strcpy(uir_path,"rte_uir.uir");
		}
	if (InitCVIRTE (0, argv, 0) == 0)	// Needed if linking in external compiler; harmless otherwise 
		return -1;	// out of memory 
	if ((global.imainpanel = LoadPanel (0, uir_path, PANEL1)) < 0)
		return -1;
	SuspendTimerCallbacks ();
	if ((global.iloginpanel = LoadPanel (0, uir_path, LOGIN)) < 0)
		return -1;
	if ((global.iedittest = LoadPanel (0, uir_path, EDITTEST)) < 0)
		return -1;

	if(SetCtrlVal (global.iloginpanel, LOGIN_TESTVERSION, TESTVERSION)) exit(0);	
//	GetPanelAttribute (global.imainpanel, ATTR_WIDTH, &global.ipanelsize);
//if running rte.exe --> load functions from dll, else: cvi-debug-mode


	global.dll_mode = 0;
	itemp = strlen(argv[0]);
	for(i=0;i<itemp;i++)
		argv[0][i] = tolower (argv[0][i]);
	if(!strcmp(argv[0],"rte.exe"))
		global.dll_mode = 1;
	if(!strcmp(argv[0],"rte.prj"))
		global.dll_mode = 1;				 //must load dll-file with all subfunctions from object-modules
		
    i=0;

        
	if((argc>2) && (!(strcmp(argv[2],DEVELOPERACCOUNT))) )
		{
		global.iloginlevel = 1;
		SetCtrlVal (global.imainpanel, PANEL1_CURRENTLOGIN, argv[2]);
		}
	else
		InstallPopup(global.iloginpanel);


        
//if debug-mode set flag debug:	
	if((argc > 3) && (!(strcmp(argv[3],"debug")))) global.debug_mode = 1;
		else global.debug_mode = 0;

	global.ieditpanel = 0;		//merker
	global.ireference = 0;    // flag for Reference test
	global.irepairmode = 0;     //@GC 04.09.2011
	global.testflow = &test_flow;
	global.data_backup = &data_backup;
	GetDir (global.rte_path);

	data_backup.command_line[0] = 0;
	data_backup.trigger_count = 0;
	data_backup.device_count_left = 0;
	data_backup.device_count_right = 0;
	data_backup.init_left = 0;
	data_backup.init_right = 0;
	data_backup.left_equal_right = 0;
//-----------------------------------------------------------------------------
	right_sequ.sequ_pfad[0] = 0;
	left_sequ.sequ_pfad[0] = 0;
	
//set all global for left seuence:
	global.left = &left_sequ;
    global.left->executeable_tests = 0;
	global.left->errorrate_handle = PANEL1_ERRORRATEL;
	global.left->numtests_handle = PANEL1_NUMTESTSL;
	global.left->sequencdisp_handle = PANEL1_SEQUENCEDISPLAYL;
	global.left->sernum_handle =  PANEL1_SERNUML;
	global.left->productnumber_handle = PANEL1_PRODUCTNUMBERL;
	global.left->sequname_handle = PANEL1_SEQNAMEL;
	global.left->status_handle =  PANEL1_CURRENTSTATUSL;
	global.left->hint_handle =  PANEL1_HINTLEFT;
	global.left->runuut_handle = PANEL1_RUNLOOPL;
	global.left->singlepass_handle = PANEL1_RUNSINGLEPL;
	global.left->idevicefail = 0;
	global.left->idevicecount = 0;
	menue_functionL.function_count = 0;   //prepare menue-functions
//set all global for left seuence:
	global.right = &right_sequ;
    global.right->executeable_tests = 0;
	global.right->errorrate_handle = PANEL1_ERRORRATER;
	global.right->numtests_handle = PANEL1_NUMTESTSR;
	global.right->sequencdisp_handle = PANEL1_SEQUENCEDISPLAYR;
	global.right->productnumber_handle = PANEL1_PRODUCTNUMBERR;
	global.right->sernum_handle =  PANEL1_SERNUMR;
	global.right->sequname_handle = PANEL1_SEQNAMER;
	global.right->status_handle =  PANEL1_CURRENTSTATUSR;
	global.right->hint_handle =  PANEL1_HINTRIGHT;
	global.right->runuut_handle = PANEL1_RUNLOOPR;
	global.right->singlepass_handle = PANEL1_RUNSINGLEPR;
	global.right->idevicefail = 0;
	global.right->idevicecount = 0;
	menue_functionR.function_count = 0;   //prepare menue-functions
//--------------------------------	
	test_flow.runuut_left = 0;
	test_flow.running_left = 0;
	test_flow.runuut_right = 0;
	test_flow.running_right = 0;
	test_flow.ready_left = 0;
	test_flow.ready_right = 0;
	
	global.left =	&left_sequ;
	global.right =	&right_sequ;
	global.executing = 0;
	global.module_portA = 0;
	global.module_portB = 0;
//------------------------------------------------------------------------------
// run adaptercode: if commandline, use parameters
	porta = 0;
	portb = 0;
	if(argc>4)
		{
		if(!strncmp(argv[4],"porta:",6))		
			porta = atoi(&argv[4][6]);
		if(!strncmp(argv[4],"portb:",6))		
			portb = atoi(&argv[4][6]);
		}
	if((argc>5) && (!(strncmp(argv[5],"portb:",6))))
		{
		if(!strncmp(argv[5],"porta:",6))		
			porta = atoi(&argv[5][6]);
		if(!strncmp(argv[5],"portb:",6))		
			portb = atoi(&argv[5][6]);
		}
	run_adaptercode(porta,portb);

//-----------------------------------------------------------------------------		

//	SetPanelAttribute (global.imainpanel, ATTR_WIDTH, global.ipanelsize);
	DisplayPanel (global.imainpanel);  //

//	SetSleepPolicy (VAL_SLEEP_SOME);
	ResumeTimerCallbacks ();
	RunUserInterface ();
	return 0;
}

    		
/* FUNCTION ***************************************************/            
int get_sequence(char *pfad,char window_pos,int do_startup)                                               
/*                                                                          
 SPECIFICATION:                                                             
		  get sequence from produkt-file
          load sequence with all parameters, first global switches
          									then parameters for every test                                       
***************************************************************/            
{                                                                           
char module[MAXPATHNAMELEN],file_path[MAXPATHNAMELEN],product_path[MAXPATHNAMELEN],fktname[80];
char sequ_position[80],testvalue[80];
char function[80],temp_str[MAXSTRINGLEN];
char line[MAXSTRINGLEN];
char sachnumber[80];
int status,itemp;		
int i,j,k;
int index;
struct struct_sequ_group sequ_group;

	if(pfad == NULL)
		return(1);
// path pointed to sachnummer & product-file, search selected "Sachnummer" & get sequence-path
//isolate sachnumber:
	sachnumber[0] = '[';
    i = strlen(pfad);
    for(j=0;j<i;j++)
    	{
    	sachnumber[j+1] = pfad[j];
    	if(sachnumber[j+1] == ';')
    		{
    		j++;
    		sachnumber[j] = ']';
    		sachnumber[j+1] = 0;
    		break;
    		}
    	if(j>40)
    		{
		   	sprintf(file_path,"Material-Number: %s to long !!", sachnumber);
			MessagePopup ("Product-line Load Error", file_path);
    		return(1);
    		}
    	}
    	
//get product-path:
	strcpy(product_path,&pfad[j]);
	fa_r = fopen(product_path,"r");	
    if(!fa_r) 	
    	{
	   	sprintf(file_path,"File: %s not found !!", product_path);
		MessagePopup ("Product-line Load Error", file_path);
    	return(1);
    	}
    
// find sequence path:
	if(!get_file_group(&sequ_group,fa_r,sachnumber))
		{
	   	sprintf(file_path,"Number: %s not found in file: ", sachnumber);
		strcat(file_path,product_path);
		MessagePopup ("Product-line Load Error", file_path);
		return(1);
		}
	Sequ_Getpara(&sequ_group,"sequence", &file_path,PARA_STRING); //get sequence-file
				
//	ResetTextBox (global.imainpanel, PANEL1_RESULTDISPLAY, "");
	fa_r = fopen(file_path,"r");	
    if(!fa_r) 	
    	{
		strcat(file_path," - not found !");
		MessagePopup ("Sequence Load Error", file_path);
    	return(1);
    	}
	i = 1;
   	if(window_pos =='l')
		{
		global.sequ_select = &left_sequ;
    	global.sequ_select->executeable_tests = 0;
    	i = 0;
    	}
   	if(window_pos =='r')
		{
		global.sequ_select = &right_sequ;
    	global.sequ_select->executeable_tests = 0;
    	i = 0;
    	}
    if(i) 	
    	{
		strcat(file_path," - no panel position !");
		MessagePopup ("Sequence Load Error", file_path);
    	return(1);
    	}
//set sachnumber:
	strcpy(global.sequ_select->sachnumber,sachnumber);
	strcpy(temp_str,&sachnumber[1]);
	i = strlen(temp_str);
	temp_str[i-1] = 0;
	SetCtrlVal (global.imainpanel, global.sequ_select->productnumber_handle, temp_str);
	
//set product-path:
	strcpy(global.sequ_select->product_pfad,product_path);
//set sequence-path:
	strcpy(global.sequ_select->sequ_pfad,file_path);
//read first group from sequence in buffer:	
	get_file_group(&sequ_group,fa_r,"[global]");	
// get working Directory:
   	Sequ_Getpara(&sequ_group,"workdirectory", &global.sequ_select->user_dir,PARA_STRING);
	SetDir (global.sequ_select->user_dir);    
// get dll Directory:
   	Sequ_Getpara(&sequ_group,"dll_library", &global.sequ_select->dll_pfad,PARA_STRING);
	
	
//	log-file 
    Sequ_Getpara(&sequ_group,"logfile_path", &global.sequ_select->log_file,PARA_STRING);
    i = strlen(global.sequ_select->log_file);
    if(i)
    	{
	    i--;
    	if(global.sequ_select->log_file[i] != '\\')
    		{
    		i++;
	    	global.sequ_select->log_file[i] = '\\';
    		}
    	}
    else
    	{
    	MessagePopup("Logging path not right","default directory: c:\\test_log");
    	strcpy(global.sequ_select->log_file,"c:\\test_log\\");
    	}
//printport:		
   	Sequ_Getpara(&sequ_group,"printport", &global.sequ_select->printerport,PARA_STRING);
//	start-up function 
   	Sequ_Getpara(&sequ_group,"startupmodul", &global.sequ_select->startup_mod,PARA_STRING);
   	Sequ_Getpara(&sequ_group,"startupfunction", &global.sequ_select->startup_fct,PARA_STRING);
	if((global.sequ_select->startup_fct[0] == 0) || (global.sequ_select->startup_mod[0] == 0))
		global.sequ_select->startup = NULL;
	else global.sequ_select->startup = get_fctaddress(pfad,global.sequ_select->startup_mod,global.sequ_select->startup_fct);
//	clean-up function 
	Sequ_Getpara(&sequ_group,"cleanupmodul", &global.sequ_select->cleanup_mod,PARA_STRING);
	Sequ_Getpara(&sequ_group,"cleanupfunction", &global.sequ_select->cleanup_fct,PARA_STRING);
	if((global.sequ_select->cleanup_fct[0] == 0) || (global.sequ_select->cleanup_mod[0] == 0))
		global.sequ_select->cleanup = NULL;
	else global.sequ_select->cleanup = get_fctaddress(pfad,global.sequ_select->cleanup_mod,global.sequ_select->cleanup_fct);
//	flow function 
   	Sequ_Getpara(&sequ_group,"flowmodul", &global.sequ_select->flow_mod,PARA_STRING);
   	Sequ_Getpara(&sequ_group,"flowfunction", &global.sequ_select->flow_fct,PARA_STRING);
	if((global.sequ_select->flow_fct[0] == 0) || (global.sequ_select->flow_mod[0] == 0))
		MessagePopup ("Run-time error in flow control!! ", "Error: no function for handling present !!");
	else global.sequ_select->flow = get_fctaddress(pfad,global.sequ_select->flow_mod,global.sequ_select->flow_fct);
	if (global.sequ_select->flow == NULL)
		MessagePopup ("Run-time error in flow control!! ", "Error: no function for handling present !!");
//	dut error function 
   	Sequ_Getpara(&sequ_group,"duterrmodul", &global.sequ_select->duterr_mod,PARA_STRING);
   	Sequ_Getpara(&sequ_group,"duterrfunction", &global.sequ_select->duterr_fct,PARA_STRING);
	if((global.sequ_select->duterr_fct[0] == 0) || (global.sequ_select->duterr_mod[0] == 0))
		{
		MessagePopup ("Run-time error in DUT control!! ", "Error: no function for  DUT-Error handling present !!");
		global.sequ_select->duterr = NULL;
		}
	else global.sequ_select->duterr = get_fctaddress(pfad,global.sequ_select->duterr_mod,global.sequ_select->duterr_fct);
//	device i/o-error function 
   	Sequ_Getpara(&sequ_group,"ioerrmodul", &global.sequ_select->ioerr_mod,PARA_STRING);
   	Sequ_Getpara(&sequ_group,"ioerrfunction", &global.sequ_select->ioerr_fct,PARA_STRING);
	if((global.sequ_select->ioerr_fct[0] == 0) || (global.sequ_select->ioerr_mod[0] == 0))
		global.sequ_select->ioerr = NULL;
	else global.sequ_select->ioerr = get_fctaddress(pfad,global.sequ_select->ioerr_mod,global.sequ_select->ioerr_fct);

// reference - testcondition for reference test   new: @ GC 20.02.2012
	//global.ireference=0;
	Sequ_Getpara (&sequ_group,"reference",&global.ireference,PARA_INTEGER);
//	if(global.ireference) global.ireference = 1;
	
// maxerror - breakcondition for test
	Sequ_Getpara (&sequ_group,"maxerror",&global.sequ_select->maxerror,PARA_INTEGER);
	if(!global.sequ_select->maxerror) global.sequ_select->maxerror = 1;
	global.sequ_select->maxerror_shadow = global.sequ_select->maxerror;
// start with run endles ? (only for dll-mode)
	if(global.dll_mode == 1)				
		{
	   	if(window_pos =='l')
			Sequ_Getpara (&sequ_group,"run_endles",&test_flow.runuut_left,PARA_INTEGER);
	   	if(window_pos =='r')
			Sequ_Getpara (&sequ_group,"run_endles",&test_flow.runuut_right,PARA_INTEGER);
		}
//----------------------------------------------------------------------
//	additional menue-functions:

	if(menue_function.function_count)
		{
		menue_function.function_count--;
		for(;menue_function.function_count>=0;menue_function.function_count--)
			DiscardMenuItem (GetPanelMenuBar (global.imainpanel),menue_function.menu_id[menue_function.function_count]);
		}
	menue_function.function_count = 0;   //prepare menue-functions
	//for(;menue_function.function_count<5;menue_function.function_count++)
	for(;menue_function.function_count<MAXMENUEFCT;menue_function.function_count++) //@GC29.05.2011 from 5 to MAXMENUEFCT
		{
		Sequ_Getpara(&sequ_group,"menue_special", &menue_function.function_path[menue_function.function_count],PARA_STRING);
		if(!menue_function.function_path[menue_function.function_count][0]) 
			{
			menue_function.menue_fctptr[menue_function.function_count] = NULL;
			break;
			}
	   	Sequ_Getpara(&sequ_group,"menue_fktname", &menue_function.functionname[menue_function.function_count],PARA_STRING);
		if(menue_function.functionname[menue_function.function_count][0])
			menue_function.menue_fctptr[menue_function.function_count] = get_fctaddress(pfad,&menue_function.function_path[menue_function.function_count][0],&menue_function.functionname[menue_function.function_count][0]);
		}
	if(menue_function.function_count)
		{
		//for(itemp=0;itemp <= menue_function.function_count;itemp++)
		for(itemp=0;itemp < menue_function.function_count;itemp++)       //@GC29.05.2011 from <= to <
			{
			if(menue_function.menue_fctptr[itemp])
				menue_function.menu_id[itemp] = NewMenuItem (GetPanelMenuBar (global.imainpanel), MENU1_SPECIAL,&menue_function.functionname[itemp][0] , -1, 0,menue_add_functions, 0);
			}
		}
//	additional menue-functions left: 
   	if(window_pos =='l')
   		{
   		menue_functionL.function_count--;
		for(;menue_functionL.function_count>=0;menue_functionL.function_count--)
			DiscardMenuItem (GetPanelMenuBar (global.imainpanel),menue_functionL.menu_id[menue_functionL.function_count]);
		menue_functionL.function_count = 0;   //prepare menue-functions
		//for(;menue_functionL.function_count<5;menue_functionL.function_count++)
		for(;menue_functionL.function_count<MAXMENUEFCT;menue_functionL.function_count++) //@GC29.05.2011 from 5 to MAXMENUEFCT
			{
   			Sequ_Getpara(&sequ_group,"menue_module", &menue_functionL.function_path[menue_functionL.function_count],PARA_STRING);
			if(!menue_functionL.function_path[menue_functionL.function_count][0]) 
				{
				menue_functionL.menue_fctptr[menue_functionL.function_count] = NULL;
				break;
				}
	   		Sequ_Getpara(&sequ_group,"menue_fktname", &menue_functionL.functionname[menue_functionL.function_count],PARA_STRING);
			if(menue_functionL.functionname[menue_functionL.function_count][0])			
				menue_functionL.menue_fctptr[menue_functionL.function_count] = get_fctaddress(pfad,&menue_functionL.function_path[menue_functionL.function_count][0],&menue_functionL.functionname[menue_functionL.function_count][0]);
			}
		if(menue_functionL.function_count)
			{
			//for(itemp=0;itemp <= menue_functionL.function_count;itemp++)
			for(itemp=0;itemp < menue_functionL.function_count;itemp++)       //@GC29.05.2011 from <= to <
				{
				if(menue_functionL.menue_fctptr[itemp])
					menue_functionL.menu_id[itemp] = NewMenuItem (GetPanelMenuBar (global.imainpanel), MENU1_LEFTSPEC,&menue_functionL.functionname[itemp][0] , -1, 0,menue_add_functions, 0);
				}
			}
		}
//	additional menue-functions right: 
   	if(window_pos =='r')
   		{
   		menue_functionR.function_count--;
		for(;menue_functionR.function_count>=0;menue_functionR.function_count--)
			DiscardMenuItem (GetPanelMenuBar (global.imainpanel),menue_functionR.menu_id[menue_functionR.function_count]);
		menue_functionR.function_count = 0;   //prepare menue-functions
		//for(;menue_functionR.function_count<5;menue_functionR.function_count++)
		for(;menue_functionR.function_count<MAXMENUEFCT;menue_functionR.function_count++) //@GC29.05.2011 from 5 to MAXMENUEFCT
			{
   			Sequ_Getpara(&sequ_group,"menue_module", &menue_functionR.function_path[menue_functionR.function_count],PARA_STRING);
			if(!menue_functionR.function_path[menue_functionR.function_count][0])
				{
				menue_functionR.menue_fctptr[menue_functionR.function_count] = NULL;
				break;
				}
		   	Sequ_Getpara(&sequ_group,"menue_fktname", &menue_functionR.functionname[menue_functionR.function_count],PARA_STRING);
			
 			if(menue_functionR.functionname[menue_functionR.function_count][0])		   		
				menue_functionR.menue_fctptr[menue_functionR.function_count] = get_fctaddress(pfad,&menue_functionR.function_path[menue_functionR.function_count][0],&menue_functionR.functionname[menue_functionR.function_count][0]);
			}
		

		if(menue_functionR.function_count)
			{
			//for(itemp=0;itemp <= menue_functionR.function_count;itemp++)
			for(itemp=0;itemp < menue_functionR.function_count;itemp++)       //@GC29.05.2011 from <= to <
				{
					
				if(menue_functionR.menue_fctptr[itemp])
					menue_functionR.menu_id[itemp] = NewMenuItem (GetPanelMenuBar (global.imainpanel), MENU1_RIGHTSPEC,&menue_functionR.functionname[itemp][0] , -1, 0,menue_add_functions, 0);
				}
			}
		}
//--------------------------------------------------------------------------------------------------

	index = 0;
    for(i=1;i<MAXANZAHLFUNCTION;i++)
    	{
//read next group from sequence in buffer:	
		get_file_group(&sequ_group,fa_r,"[TEST]");	
    	if(!(Sequ_Getpara(&sequ_group,"modul", module,PARA_STRING)))
    		goto readnexttest;			  //read next (if one or more remark)
    	if(!(Sequ_Getpara(&sequ_group,"fktname", fktname,PARA_STRING)))
			goto readnexttest;
    	strcpy(file_path,module);
		global.sequ_select->fktpointer[index] = get_fctaddress(file_path,module,fktname);
		if(global.sequ_select->fktpointer[index] == NULL)
			return(1);
//function is load, get all parameters:
		global.sequ_select->structpointer[index] = malloc(sizeof(struct struct_test));
//copy all system-offsets
		for(j=0;j<MAXSYSTEM_OFFSETS;j++)
			global.sequ_select->structpointer[index]->system_offsets[j] = system_offsets[j];
		strcpy(global.sequ_select->structpointer[index]->fkt_name,fktname);
 		global.sequ_select->structpointer[index]->modulename =malloc((strlen(file_path)+1));
		strcpy(global.sequ_select->structpointer[index]->modulename,file_path);
		Sequ_Getpara (&sequ_group,"username",&global.sequ_select->structpointer[index]->username[0],PARA_STRING);
		Sequ_Getpara (&sequ_group,"switch_sequ",&global.sequ_select->structpointer[index]->switch_sequ,PARA_INTEGER);
    	Sequ_Getpara(&sequ_group,"unit", &global.sequ_select->structpointer[index]->unit[0],PARA_STRING);
		Sequ_Getpara (&sequ_group,"runmode",&global.sequ_select->structpointer[index]->runmode,PARA_INTEGER);
		Sequ_Getpara (&sequ_group,"loopcount",&global.sequ_select->structpointer[index]->loopcount,PARA_INTEGER);
		Sequ_Getpara (&sequ_group,"precond",&global.sequ_select->structpointer[index]->precond,PARA_INTEGER);
		Sequ_Getpara (&sequ_group,"points",&global.sequ_select->structpointer[index]->prec,PARA_INTEGER);
		Sequ_Getpara (&sequ_group,"errormessage",temp_str,PARA_STRING);
		if(temp_str[0])
			{
			global.sequ_select->structpointer[index]->errormessage = malloc((strlen(temp_str)+1));
			strcpy(global.sequ_select->structpointer[index]->errormessage,temp_str);
			}
		else global.sequ_select->structpointer[index]->errormessage = NULL;
		for(j=0;j<MAXPARASTRING;j++)
			{
	    	sprintf(testvalue,"string%d",j+1);
			Sequ_Getpara (&sequ_group,testvalue,temp_str,PARA_STRING);
			if(strlen(temp_str))
				{
				global.sequ_select->structpointer[index]->string[j] = malloc(strlen(temp_str)+1);
				strcpy(global.sequ_select->structpointer[index]->string[j],temp_str);
				}
			else
				{
				global.sequ_select->structpointer[index]->string[j] = malloc(1);
				global.sequ_select->structpointer[index]->string[j][0]  = 0;
				}
			}
		if(!Sequ_Getpara (&sequ_group,"lowerlimit",&global.sequ_select->structpointer[index]->lowerlimit,PARA_DOUBLE))
			global.sequ_select->structpointer[index]->lowerlimit= 0;
		if(!Sequ_Getpara (&sequ_group,"upperlimit",&global.sequ_select->structpointer[index]->upperlimit,PARA_DOUBLE))
			global.sequ_select->structpointer[index]->upperlimit= 0;
		if(!Sequ_Getpara (&sequ_group,"resultoffset",&global.sequ_select->structpointer[index]->resultoffset,PARA_DOUBLE))
			global.sequ_select->structpointer[index]->resultoffset= 0;
		for(j=0;j<MAXPARAINT;j++)
			{
	    	sprintf(testvalue,"int%d",j+1);
			if(!Sequ_Getpara (&sequ_group,testvalue,&global.sequ_select->structpointer[index]->intpara[j],PARA_INTEGER))
				global.sequ_select->structpointer[index]->intpara[j]= 0;
			}//end read integer
		for(j=0;j<MAXPARADOUBLE;j++)
			{
	    	sprintf(testvalue,"double%d",j+1);
			if(!Sequ_Getpara (&sequ_group,testvalue,&global.sequ_select->structpointer[index]->doublepara[j],PARA_DOUBLE))
				global.sequ_select->structpointer[index]->doublepara[j] = 0;					
			}//end read double
		global.sequ_select->executeable_tests++;				
		index++;	
readnexttest:
		j = 0;	// dummy command for compiler
		}//end read sequ-file
    fclose(fa_r);
//set all panel & menu1-attributes
	SetPanelPos (global.imainpanel,global.iyposition, global.ixposition);
	set_menu1_attr();  // 
    fill_sequdisplay(0);
	global.sequ_select->idevicefail = 0;
	global.sequ_select->idevicecount = 0;
	global.sequ_select->ierror = 0;
	global.sequ_select->act_index = 0;
	global.sequ_select->last_result = 0;
	global.sequ_select->io_error = 0;
	SetCtrlVal (global.imainpanel, global.sequ_select->errorrate_handle, 0);
	i = 0;
	if((global.sequ_select->startup != NULL) && do_startup)
		i = (global.sequ_select->startup)(&global);
	if(i)
		{
		MessagePopup ("Start sequence error !!!!! ","Error: startup function failed, sequence can't work correct !!");
		}
	SetActiveCtrl (global.imainpanel, global.sequ_select->sequencdisp_handle);
	return(0);
}


/* FUNCTION ***************************************************/            
void fill_sequdisplay(int mode)
//          print to sequence display
// PRECONDITIONS:   	if(mode<0xff) --> replace; else insert new line
// 					if(mode>0xff) --> replace one item
//**************************************************************            
{
int index;
char tempstring[40];

		index = 0;		//default all 
		if(mode > 0xff)	//one item update 
			index |= (mode & 0xff); //replace only one line
		if(!mode)
			{
			SetCtrlVal (global.imainpanel, global.sequ_select->numtests_handle, global.sequ_select->executeable_tests);
			//if load new sequence, delete old	
			ClearListCtrl (global.imainpanel, global.sequ_select->sequencdisp_handle);
			SetCtrlVal (global.imainpanel, global.sequ_select->sequname_handle, global.sequ_select->sequ_pfad);
			}
 
		while(index<global.sequ_select->executeable_tests)
			{	
			sprintf(tempstring,"%-21s",global.sequ_select->structpointer[index]->username);
			switch(global.sequ_select->structpointer[index]->runmode)
				{
				case 1:			//skip
					strcat(tempstring,"S:skip");
					global.sequ_select->structpointer[index]->resultstatus = 2;
					break;
				case 2:			//pass
					strcat(tempstring,"S:pass");
					global.sequ_select->structpointer[index]->resultstatus = 0;
					break;
				case 3:			//fail
					strcat(tempstring,"S:fail");
					global.sequ_select->structpointer[index]->resultstatus = 1;
					break;
				default:
					global.sequ_select->structpointer[index]->runmode = 0;
					break;
				}
//serve sequence-listbox:  
			if(mode)
				{
    			ReplaceListItem (global.imainpanel, global.sequ_select->sequencdisp_handle,index,tempstring, index);
				CheckListItem (global.imainpanel, global.sequ_select->sequencdisp_handle, index, 0);
				}
			else
			
				InsertListItem (global.imainpanel, global.sequ_select->sequencdisp_handle, -1, tempstring,1);
			index++;
			if(mode>0xff)
				index = global.sequ_select->executeable_tests;					 
			}
	return;
}

/* FUNCTION ***************************************************/            
int testexecutive (int mode) 
/*                                                                          
 SPECIFICATION:                                                             
          run single or all tests
          serve sequence display
***************************************************************/            
{
int itemp;
char tempstring[50];

	global.executing = 1;
	global.sequ_select->jmp_absolut = 0; //index for jmps, default: no jmp
	global.iabort = 0;
//	if(mode != SINGLETEST)
		{
		if(global.sequ_select == &left_sequ )
			{
			test_flow.running_left = 1;
			test_flow.ioerror_left = 0;
			}
		else
			{
			test_flow.running_right = 1;
			test_flow.ioerror_right = 0;
			}
		}
	global.sequ_select->last_result = 0;					//last test = succes, needed for precondition
	global.sequ_select->ierror = 0;
	if(mode == SINGLEPASS)		// reset complete sequence status
		{
		for(itemp=0;itemp < global.sequ_select->executeable_tests; itemp++)  //clear old sequence-status
			{
			if(!(global.sequ_select->structpointer[itemp]->runmode))
	    		{
	    		ReplaceListItem (global.imainpanel, global.sequ_select->sequencdisp_handle,itemp,global.sequ_select->structpointer[itemp]->username, itemp);
				CheckListItem (global.imainpanel, global.sequ_select->sequencdisp_handle, itemp, 0);
				}
			}
		if(global.iclrdisplay == 1)
			ResetTextBox (global.imainpanel, PANEL1_RESULTDISPLAY, "");
		}
//loop for SINGLEPASS, SINGLETEST break these loop:		
	for(;global.sequ_select->act_index < global.sequ_select->executeable_tests;global.sequ_select->act_index++) 
		{
		if(global.sequ_select->jmp_absolut)
			{
			global.sequ_select->act_index = global.sequ_select->jmp_absolut-1;
			global.sequ_select->jmp_absolut = 0; //index for jmps, default: no jmp
			}
		if(global.iclrdisplay == 2)
			ResetTextBox (global.imainpanel, PANEL1_RESULTDISPLAY, "");
		if(global.iservesystem)
        	{
        	SetCtrlIndex (global.imainpanel, global.sequ_select->sequencdisp_handle, global.sequ_select->act_index);	 //tag active test
        	ProcessDrawEvents();
        	}
//if any runmode(skip,fail,pass) --> do not test		        
        if(global.sequ_select->structpointer[global.sequ_select->act_index]->runmode) global.sequ_select->act_loopcount = 0;
        else {global.sequ_select->act_loopcount = (global.sequ_select->structpointer[global.sequ_select->act_index]->loopcount)+1;}
//preconditions?:
		if(global.sequ_select->act_loopcount && global.sequ_select->structpointer[global.sequ_select->act_index]->precond)
			{
			itemp = 0;
			//if((global.sequ_select->last_result==0) && (global.sequ_select->structpointer[global.sequ_select->act_index]->precond == 2)) itemp++;	//if lasttest succes && precond == fail
			//if((global.sequ_select->last_result==1) && (global.sequ_select->structpointer[global.sequ_select->act_index]->precond == 1)) itemp++;	//if lasttest fail && precond == pass
			
			// new Precondition @GC 15.03.2014
			// 0=normal 1=onlyIfLastTestPassed 2=onlyIfLastTestFail 3=onlyIfRepairmode 4=onlyIfNoneRepairmode
			if((global.sequ_select->structpointer[global.sequ_select->act_index]->precond == 1) && (global.sequ_select->last_result==1)) itemp++;	//if precond==1 and LastTest failed then ThisTest Skiped
			if((global.sequ_select->structpointer[global.sequ_select->act_index]->precond == 2) && (global.sequ_select->last_result==0)) itemp++;	//if precond==2 and LastTest passed then ThisTest Skiped
			if((global.sequ_select->structpointer[global.sequ_select->act_index]->precond == 3) && (global.irepairmode==0)) itemp++;            	//if precond==3 and NoneRepairmode then ThisTest Skiped
			if((global.sequ_select->structpointer[global.sequ_select->act_index]->precond == 4) && (global.irepairmode!=0)) itemp++;            	//if precond==4 and Repairmode then ThisTest Skiped
			if(itemp)
				{
				global.sequ_select->structpointer[global.sequ_select->act_index]->resultstatus = 2;  //skip
				global.sequ_select->act_loopcount = 0;
				sprintf(tempstring,"%-21s skip",global.sequ_select->structpointer[global.sequ_select->act_index]->username);
		    	ReplaceListItem (global.imainpanel, global.sequ_select->sequencdisp_handle,global.sequ_select->act_index, tempstring, global.sequ_select->act_index);
				}
			}					
// execute pointed test n times:
		while(global.sequ_select->act_loopcount)	//loopcount
			{
			global.sequ_select->act_loopcount--;
//do function:			
// these syntax:   global.sequ_select->structpointer[global.sequ_select->act_index] is a address,because structpointer is a pointer !!
			global.sequ_select->io_error = (global.sequ_select->fktpointer[global.sequ_select->act_index])(global.sequ_select->structpointer[global.sequ_select->act_index]);	//if return(functioncall)--> device-IO-error
			if(global.sequ_select->io_error || global.sequ_select->structpointer[global.sequ_select->act_index]->resultstatus)	//if(resultstatus)--> DUT-error 
				{							// test failed
				if(!global.sequ_select->act_loopcount) 	//if (loop != 0), goto while
					{		//loop over, test with error
					if(global.sequ_select->io_error)
						{
						if(global.sequ_select->ioerr != NULL)
							{
							test_flow.error_code = global.sequ_select->io_error;
							test_flow.test_name = global.sequ_select->structpointer[global.sequ_select->act_index]->username;
							if(!(global.sequ_select->ioerr)(&test_flow))
								{
								global.executing = 0;
								return(0);	//escape without modify dut-error-rate!!
								}
							}
						else
							{
							sprintf(tempstring,"Test: %s, Errorcode: 0x%x",global.sequ_select->structpointer[global.sequ_select->act_index]->username,global.sequ_select->io_error);
							MessagePopup ("Runt-time error, no function for handling present !!,", tempstring);
							}
						}
					global.sequ_select->ierror++;
					sprintf(tempstring,"%-21s fail",global.sequ_select->structpointer[global.sequ_select->act_index]->username);
					if(global.sequ_select->jmp_absolut)
						{
						sprintf(&tempstring[26]," go:%+d",(global.sequ_select->jmp_absolut-global.sequ_select->act_index-1));
				    	ReplaceListItem (global.imainpanel, global.sequ_select->sequencdisp_handle,global.sequ_select->act_index, tempstring, global.sequ_select->act_index);
						}
					else
			    		ReplaceListItem (global.imainpanel, global.sequ_select->sequencdisp_handle,global.sequ_select->act_index, tempstring, global.sequ_select->act_index);
					}
				}//end if function failed
			else				//test succes
				{
				CheckListItem (global.imainpanel, global.sequ_select->sequencdisp_handle, global.sequ_select->act_index, 1);
				if(global.sequ_select->jmp_absolut)
					{
					sprintf(tempstring,"%-21s pass",global.sequ_select->structpointer[global.sequ_select->act_index]->username);
					sprintf(&tempstring[26]," go:%+d",(global.sequ_select->jmp_absolut-global.sequ_select->act_index-1));
			    	ReplaceListItem (global.imainpanel, global.sequ_select->sequencdisp_handle,global.sequ_select->act_index, tempstring, global.sequ_select->act_index);
					}
				global.sequ_select->act_loopcount = 0;	// function ready, break: while(iloop)
				}
			}
		if(global.sequ_select->structpointer[global.sequ_select->act_index]->resultstatus != 2)	//2==skip
			global.sequ_select->last_result = global.sequ_select->structpointer[global.sequ_select->act_index]->resultstatus;

		if(mode == SINGLETEST)
			{
			test_flow.running_left = 0;
			test_flow.running_right = 0;
			break;
			}
    	if(global.sequ_select->ierror >= global.sequ_select->maxerror)
    		{
			if(global.sequ_select->duterr) // if function present..
				{
				global.sequ_select->idevicefail++;		// one more error device
				(global.sequ_select->duterr)(&test_flow);
				global.executing = 0; //flag !!
				break;	
				}
			}
		if(global.iabort)
			{
			test_flow.runuut_left = 0;
			test_flow.runuut_right = 0;
			break;
			}
        if(global.iservesystem) 
        	ProcessSystemEvents ();//performance
		(global.sequ_select->flow)(&test_flow); //do flow-function
		}//end for(index < executable tests)
	if(mode != SINGLETEST)   //only for normal operation
		{//do this, if DUT-Error but not maxerror !!   (global.executing used as flag)
		if(global.sequ_select->ierror && global.executing)
			{
			global.sequ_select->idevicefail++;		// one more error device
			if(global.sequ_select->duterr)  // if function present..
				(global.sequ_select->duterr)(&test_flow);
			}
		}
	if(global.sequ_select == &left_sequ)
		test_flow.ready_left = 1;
	else
		test_flow.ready_right = 1;
	global.sequ_select->act_index	= 0;
	global.sequ_select->idevicecount++;			// one more device tested
	itemp = global.sequ_select->idevicefail*100/global.sequ_select->idevicecount;
	if(global.showerrorrate)
		SetCtrlVal (global.imainpanel, global.sequ_select->errorrate_handle, itemp);
	global.executing = 0;
	ProcessSystemEvents ();
	return (0);
}	



/* FUNCTION ***************************************************/            
void edit_sequence(int mode)
/*                                                                          
 SPECIFICATION: edit/generate sequence file
 				mode = 0, --> new/other
 				mode = 1, --> left sequence
 				mode = 2, --> right sequence
***************************************************************/            
{
int i,nmodul,mod_index;
char *temp[MAXMODULES];
char pfad[MAXPATHNAMELEN];
char temp_char;
	
	for(i=0;i<MAXMODULES;i++)
		temp[i] = NULL;

	if(!mode)	   //use  the same variables
		{
		for(i=0;i<global.sequ_select->executeable_tests;i++)
			{
			free(global.sequ_select->structpointer[i]->modulename);
			free(global.sequ_select->structpointer[i]);
			}
		if(FileSelectPopup ("", "*.squ", "", "", VAL_OK_BUTTON, 0, 0, 1, 1,pfad) != 1)
			return;
		}
	if(mode == 1)	//left
		{
		global.sequ_select = &left_sequ;
		temp_char = 'l';
		}
	if(mode == 2)	//left
		{
		global.sequ_select = &right_sequ;
		temp_char = 'r';
		}
		
	GetCtrlVal (global.imainpanel, global.sequ_select->sequname_handle, pfad);  //current sequence
	if ((global.ieditpanel = LoadPanel (0, uir_path, EDITSEQU)) < 0)
		return ;

	SetCtrlVal (global.ieditpanel, EDITSEQU_SEQNAME, global.sequ_select->sequ_pfad);
	get_sequence(pfad,temp_char,0); //complete new initialize
	fill_sequ(0,0);
//get all module names:	
	i = 1;
	if(global.sequ_select->structpointer[0] != NULL) 
		{
		temp[0] = malloc(strlen(global.sequ_select->structpointer[0]->modulename)+1);
		strcpy(temp[0],global.sequ_select->structpointer[0]->modulename);
		i = global.sequ_select->executeable_tests;
		}
	nmodul = 1;
	for(;i<global.sequ_select->executeable_tests;i++)
		{
		mod_index = 0;
		while(strcmp(temp[mod_index],global.sequ_select->structpointer[i]->modulename))
			{
			mod_index++;
			if(mod_index == nmodul) 
				{
				temp[nmodul] = malloc(strlen(global.sequ_select->structpointer[i]->modulename)+1);
				strcpy(temp[nmodul],global.sequ_select->structpointer[i]->modulename);
				nmodul++;
				break;
				}
			}
		if(nmodul == MAXMODULES-1)
			break;
		}	
	for(i=0;i<nmodul;i++)
		{
		if(temp[i] != NULL)
			{
			fill_proto(temp[i]);
			InsertListItem(global.ieditpanel, EDITSEQU_MODULES, -1,temp[i],-1);
			}
		}

	InstallPopup(global.ieditpanel);
	return;
}



/* FUNCTION ***************************************************/            
void fill_proto(char pfad[])
/*                                                                          
 SPECIFICATION:                                                             
          print to edit display, insert/delete functions
          						                                       
                                                                            
 PRECONDITIONS:   	
 SIDE_EFFECTS:                                                              
 RETURN_VALUES:                                                             
 EXCEPTIONS:                                                                
***************************************************************/            
{
FILE *fa;	
char temp[41],line[MAXPATHNAMELEN],zeichen;
int i,count,j,linelen;
	
		strcpy(line,pfad);
		i = strlen(line);
		strcpy(&line[i],".c");
		if(!(fa = fopen(line,"r")))
			{
			strcpy(&line[i],".h");
			if(!(fa = fopen(line,"r")))
				{
				MessagePopup ("Sequence Edit Error - File not found (*.c, *.h)!", line);
				return;
				}
			}
		
		
		strcpy(line,pfad);
		strcat(line,":");
		linelen = strlen(line);	
		i = 0;
		count = 0;
		while(!feof(fa))
			{
			if((zeichen = fgetc(fa)) != ' ')
				{
				temp[i] = zeichen;
				i++;
				}
				
			if(i && ((temp[i-1] == '\n') || (i>40)))
				{
				if(!(strncmp(temp,"TESTFUNCTION",12)))
					{
					for(i=12;i<40;i++)
						if(temp[i] == '(')
							break;
					temp[i] = 0;
					strcpy(&line[linelen],&temp[12]);
					InsertListItem (global.ieditpanel, EDITSEQU_PROTOTYPES, -1,line, -1);
					}
				i = 0;
				}
			}
		fclose(fa);
		
	return;
}


/* FUNCTION ***************************************************/            
void fill_sequ(int index,int mode)
/*                                                                          
 SPECIFICATION:                                                             
          print to sequence display from edit-panel
          						                                       
                                                                            
 PRECONDITIONS:   	if(mode==0) --> insert all
 					if(mode==1) --> delete one line in position(index)
 					if(mode==2) --> insert/replace line in position(index)
***************************************************************/            
{
char tempstring[40];

	SetCtrlVal (global.ieditpanel, EDITSEQU_NUMTESTS,global.sequ_select->executeable_tests);

//if load new sequence, delete old	
	if(!mode)
		ClearListCtrl (global.ieditpanel, EDITSEQU_SEQUENCEDISPLAY);
 
	while(index<global.sequ_select->executeable_tests)
		{	
		sprintf(tempstring,"%-21s",global.sequ_select->structpointer[index]->username);
		switch(global.sequ_select->structpointer[index]->runmode)
			{
			case 1:			//skip
				strcat(tempstring,"S:skip");
				break;
			case 2:			//pass
				strcat(tempstring,"S:pass");
				break;
			case 3:			//fail
				strcat(tempstring,"S:fail");
				break;
			}
//serve sequence-listbox:  
		switch (mode)
			{
			case 2:
	   			ReplaceListItem (global.ieditpanel, EDITSEQU_SEQUENCEDISPLAY,index,tempstring, index);
				index = global.sequ_select->executeable_tests;					 
				break;
			case 1:
				InsertListItem (global.ieditpanel, EDITSEQU_SEQUENCEDISPLAY, index, tempstring,index);
				index = global.sequ_select->executeable_tests;					 
				break;
			case 0:
				InsertListItem (global.ieditpanel, EDITSEQU_SEQUENCEDISPLAY, -1, tempstring,-1);
				break;
			}
		index++;
		}
	return;
}


	
/*subfunction to get_sequence*/
/*
if there a function witch calls another subfunction that position is not known, the function GetExternal... return error
*/
void *get_fctaddress(char *pfad, char *module,char *fktname)
{
int module_id,status,go_fct_retry=0;
char message[MAXSTRINGLEN],temp_str[MAXPATHNAMELEN];
void *fct_addresse;


		strcpy(temp_str,module); //use every time absolute path !!
//--- debug - mode?
    	if(global.debug_mode)
    		strcat (temp_str, ".c");	  //only load, if c-modules are in cvi-project, else error
		else
    		strcat (temp_str, ".obj");
try_again:
		if(global.dll_mode)
			{
			strcpy(temp_str,global.sequ_select->dll_pfad);
			if((module_id = LoadExternalModuleEx (temp_str,__CVIUserHInst)) < 0)   // <0==error, an allready loaded module returns the same id
				{
				MessagePopup ("Sequence Load Error: Modul not found !",temp_str);
				return(NULL);
				}
			}
		else
			{
			if((module_id = LoadExternalModule (temp_str)) < 0)   // <0==error, an allready loaded module returns the same id
				{
				MessagePopup ("Sequence Load Error: Modul not found !",temp_str);
				return(NULL);
				}
			}
//module is load, get function:
		fct_addresse = GetExternalModuleAddr (module_id,fktname,&status);
		if(fct_addresse == NULL)					   //function not found
			{
			if(status == -5)						   // CVI-Error ?? load another known Module & get fct-addr.
				{
				module_id = LoadExternalModule (DUMMY_LOAD);
				fct_addresse = GetExternalModuleAddr (module_id,DUMMY_FUNCTION,&status);
				go_fct_retry++;
				if(go_fct_retry<3)
					goto try_again;
				}
				
			sprintf(message,"Funktion not found:  %s , module ID: %d, status:%d  !",fktname,module_id,status);
			MessagePopup ("Sequence Load Error", message);
			return(NULL);
			}
	return(fct_addresse);    	
}

