#include "test.h"
#include <rte_uir.h>



//#define RFP35_DEBUG_MESSAGE_POPUP   // MessagePopup bei SystemCall Fehlermeldungen
#undef RFP35_DEBUG_MESSAGE_POPUP   // MessagePopup bei SystemCall Fehlermeldungen



/*
#define POSITION_OFFSET "login-level/login-name/Adapter-ID/side/Date/Start-Time"
#define POSITION_OFFSET1 "/////"	
#define POSITION_OFFSET_UNIT "////dd-mm-yyyy/hh:mm:ss"
*/
#define POSITION_OFFSET "system_ID/meas_mode/login-level/login-name/Adapter-ID/side/Date/Start-Time"
#define POSITION_OFFSET1 "///////"	
#define POSITION_OFFSET_UNIT "//////dd-mm-yyyy/hh:mm:ss"
//Start [@JM030308]
static int	iPrinterModeDb	= 0;
//Ende [@JM030308]


//Start [@JM220408]
static unsigned char	gl_ucProdData[32];
static unsigned char	gl_ucDeviatData[2];
static unsigned char	gl_ucRssiData[4];
static unsigned char	gl_ucBarcodeData[10];
static unsigned char	gl_ucClockData[1];
static unsigned char	gl_ucBandgapData[1];
static char				gl_cEepromDataPath[300] = ""; 
//Ende [@JM220408]

struct struct_file_LongLines //new [@JM090304]
	{
	char lines[360][5*MAXSTRINGLEN];
	int line_count;
	};

char file_get_group_LongLines(struct struct_file_LongLines *file_group,FILE *fa_r,char group[]); //new [@JM090304]
char file_getpara_LongLines (struct struct_file_LongLines *file_group,char tag[],void *parameter,int type); //new [@JM090304]

  
int write_sequ2logfile(struct rte_globals *rte_globals)
{
#define MAX_TABLE_LINE_LENGTH	2024
FILE *fa_w,*fa_r;
int j,i=0;
char log_file[MAXPATHNAMELEN];
char sequence_header_str[MAX_TABLE_LINE_LENGTH];
char new_header_flag;
char tempstr[64];
char line1[MAX_TABLE_LINE_LENGTH];
char line2[MAX_TABLE_LINE_LENGTH];
char line3[MAX_TABLE_LINE_LENGTH];
char line4[MAX_TABLE_LINE_LENGTH];
char line5[MAX_TABLE_LINE_LENGTH];
char line6[MAX_TABLE_LINE_LENGTH];
char line1_old[MAX_TABLE_LINE_LENGTH]={0};
char line2_old[MAX_TABLE_LINE_LENGTH]={0};
char line3_old[MAX_TABLE_LINE_LENGTH]={0};
char line4_old[MAX_TABLE_LINE_LENGTH]={0};
char line5_old[MAX_TABLE_LINE_LENGTH]={0};
char line6_old[MAX_TABLE_LINE_LENGTH]={0};


	//set_sprache(DEUTSCH);     	 
	 line1_old[0] = 0;
//get mat-number & remove brackets
	strcpy(log_file,&globals->sequ_select->sachnumber[1]);
	i = strlen(log_file);
	i--;
	log_file[i] = 0;
	for(j=0;j<i;j++)
		{
		if(log_file[j] == '.')
			{
			i--;
			memcpy(&log_file[j],&log_file[j+1],(i-j));
			log_file[i] = 0;
			break;
			}
		}
	
//-------------		
	i = strlen(globals->sequ_select->log_file);
	if(i>5)
		{
		if(!strcmp(&globals->sequ_select->log_file[i-4],".log"))
			i = 0;
		}

	if(i)
		{
		strcat(log_file,".log");
		strcat(globals->sequ_select->log_file,log_file);
		}
	fa_w = fopen(globals->sequ_select->log_file,"a");
	if(!fa_w)
		{
		print_resultdisplay("Writing to Log-File not possible !!");
		return(-1);
		}
	fclose(fa_w);		
	sprintf(line1,"/***Table - Header for sequence: %s",globals->sequ_select->sequ_pfad);
	sprintf(line2,"%s",POSITION_OFFSET);
	sprintf(line3,"(lower limit)%s",POSITION_OFFSET1);
	sprintf(line4,"(upper limit)%s",POSITION_OFFSET1);
	sprintf(line5,"(result offset)%s",POSITION_OFFSET1);
	sprintf(line6,"(unit)%s",POSITION_OFFSET_UNIT);
	i = 0;
	while(globals->sequ_select->fktpointer[i])
		{
//name:	
            
		//sprintf(tempstr,"/%s",globals->sequ_select->structpointer[i]->fkt_name);   // new @GC14.06.2011
		sprintf(tempstr,"/%s",globals->sequ_select->structpointer[i]->username);     // changed from fkt_name to user_name
		strcat(line2,tempstr);
		switch (globals->sequ_select->structpointer[i]->prec)
			{
			case 1:
				sprintf(tempstr,"/%.1f",globals->sequ_select->structpointer[i]->lowerlimit);
				strcat(line3,tempstr);
				sprintf(tempstr,"/%.1f",globals->sequ_select->structpointer[i]->upperlimit);
				strcat(line4,tempstr);
				sprintf(tempstr,"/%.1f",globals->sequ_select->structpointer[i]->resultoffset);
				strcat(line5,tempstr);
				break;
			case 2:
				sprintf(tempstr,"/%.2f",globals->sequ_select->structpointer[i]->lowerlimit);
				strcat(line3,tempstr);
				sprintf(tempstr,"/%.2f",globals->sequ_select->structpointer[i]->upperlimit);
				strcat(line4,tempstr);
				sprintf(tempstr,"/%.2f",globals->sequ_select->structpointer[i]->resultoffset);
				strcat(line5,tempstr);
				break;
			case 3:
				sprintf(tempstr,"/%.3f",globals->sequ_select->structpointer[i]->lowerlimit);
				strcat(line3,tempstr);
				sprintf(tempstr,"/%.3f",globals->sequ_select->structpointer[i]->upperlimit);
				strcat(line4,tempstr);
				sprintf(tempstr,"/%.3f",globals->sequ_select->structpointer[i]->resultoffset);
				strcat(line5,tempstr);
				break;
			case 4:
				sprintf(tempstr,"/%.4f",globals->sequ_select->structpointer[i]->lowerlimit);
				strcat(line3,tempstr);
				sprintf(tempstr,"/%.4f",globals->sequ_select->structpointer[i]->upperlimit);
				strcat(line4,tempstr);
				sprintf(tempstr,"/%.4f",globals->sequ_select->structpointer[i]->resultoffset);
				strcat(line5,tempstr);
				break;
			case 5:
				sprintf(tempstr,"/%.5f",globals->sequ_select->structpointer[i]->lowerlimit);
				strcat(line3,tempstr);
				sprintf(tempstr,"/%.5f",globals->sequ_select->structpointer[i]->upperlimit);
				strcat(line4,tempstr);
				sprintf(tempstr,"/%.5f",globals->sequ_select->structpointer[i]->resultoffset);
				strcat(line5,tempstr);
				break;
			default:
				sprintf(tempstr,"/%d",(int)globals->sequ_select->structpointer[i]->lowerlimit);
				strcat(line3,tempstr);
				sprintf(tempstr,"/%d",(int)globals->sequ_select->structpointer[i]->upperlimit);
				strcat(line4,tempstr);
				sprintf(tempstr,"/%d",(int)globals->sequ_select->structpointer[i]->resultoffset);
				strcat(line5,tempstr);
				break;
			}
//unit
		sprintf(tempstr,"/%s",globals->sequ_select->structpointer[i]->unit);
		strcat(line6,tempstr);
		i++;
		}
	strcat(line2,"/End-Time/");
	strcat(line3,"//");
	strcat(line4,"//");
	strcat(line5,"//");
	strcat(line6,"/hh:mm:ss/");

// get old & least table header
	fa_r = fopen(globals->sequ_select->log_file,"r");
	if(fa_r)
		{
		while(!feof(fa_r))
			{
			fgets(sequence_header_str,MAX_TABLE_LINE_LENGTH-1,fa_r);
			j = strlen(sequence_header_str);
			if(!j) break;
			sequence_header_str[j-1] = 0;
			if(!strncmp(sequence_header_str,line1,10))
				strcpy(line1_old,sequence_header_str);
			if(!strncmp(sequence_header_str,line2,10))
				strcpy(line2_old,sequence_header_str);
			if(!strncmp(sequence_header_str,line3,10))
				strcpy(line3_old,sequence_header_str);
			if(!strncmp(sequence_header_str,line4,10))
				strcpy(line4_old,sequence_header_str);
			if(!strncmp(sequence_header_str,line5,10))
				strcpy(line5_old,sequence_header_str);
			if(!strncmp(sequence_header_str,line6,10))
				strcpy(line6_old,sequence_header_str);
			}
		fclose(fa_r);
		}
//compare last table header, if changes, append to log-file
	new_header_flag = 0;
	if(strcmp(line1,line1_old)) new_header_flag++; 
	if(strcmp(line2,line2_old)) new_header_flag++; 
	if(strcmp(line3,line3_old)) new_header_flag++; 
	if(strcmp(line4,line4_old)) new_header_flag++; 
	if(strcmp(line5,line5_old)) new_header_flag++; 
	if(strcmp(line6,line6_old)) new_header_flag++; 
	if(new_header_flag)
		{
		fa_w = fopen(globals->sequ_select->log_file,"a");
		if(!fa_w)
			{
			MessagePopup("Startup Error ","Can't open log-file, check Disk-space !!");
			return(1);
			}
		fprintf(fa_w,"\n%s",line1);
		fprintf(fa_w,"\n%s",line2);
		fprintf(fa_w,"\n%s",line3);
		fprintf(fa_w,"\n%s",line4);
		fprintf(fa_w,"\n%s",line5);
		fprintf(fa_w,"\n%s",line6);
		fprintf(fa_w,"\n/***end");
		fclose(fa_w);			
		}
	return(0);
}

//---------------------------------------------------------------------------------------------------
int parse_line(char line_ptr[])
{
int token_ind,i,j,index;

	for(i=0;i<MAXTOKEN;i++)
		token[i][0] = 0;
	token_ind = 0;
	index = 0;
	i = strlen(line_ptr);
	for(j=0;j<i;j++)
		{
		token[token_ind][index] = line_ptr[j];  
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





char file_get_group(struct struct_file *file_group,FILE *fa_r,char group[])
{
int i,j;
char group_flag;
char line[MAXSTRINGLEN];

//struct struct_sequ_group  *psequ_group;
//	psequ_group = (struct struct_sequ_group*) sequ_group;
	

	for(i=0;i<50;i++)
		file_group->lines[i][0]=0;
	i = 0;		
	
	group_flag = 0;
	while(fgets(line,MAXSTRINGLEN,fa_r))
		{
		if(group_flag)
			{
			if(!strncmp(line,"[end group]",11)) break;
			j = strlen(line);
			for(;j>0;j--)
				{
				if(line[j] < 0x20) line[j] = 0;
				}
			strcpy(&file_group->lines[i][0],line);
			i++;
			}
		if(!strncmp(line,group,strlen(group)))
			group_flag = 1;
		}
	file_group->line_count = i;			
	return(group_flag);
}

char file_getpara (struct struct_file *file_group,char tag[],void *parameter,int type)
{
int 	i,j,k,flag;
int 	*int_ptr;
int 	*hex_ptr;
double 	*doub_ptr;
char	*char_ptr;

 
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
		case PARA_STRING_LONG:			//name with space
			char_ptr = parameter;
			*char_ptr = 0;
			break;
		}

	flag = 1;		// set default to error
	for(i=0;i<file_group->line_count;i++)
		{
		if(!strncmp(&file_group->lines[i][0],tag,strlen(tag)))
			{
			flag = 0; // set to succes, tagged name was found
			switch(type)
				{
				case PARA_INTEGER:
					parse_line(&file_group->lines[i][0]);
					 int_ptr = parameter;
					*int_ptr = atoi(&token[1][0]);
					break;
				case PARA_HEX:
					parse_line(&file_group->lines[i][0]);
					hex_ptr = parameter;
					sscanf(&token[1][0],"%x",hex_ptr);
					break;
				case PARA_DOUBLE:
					parse_line(&file_group->lines[i][0]);
					 doub_ptr = parameter;
					*doub_ptr = atof(&token[1][0]);
					break;
				case PARA_STRING:
					parse_line(&file_group->lines[i][0]);
					char_ptr = parameter;
					strcpy(char_ptr,&token[1][0]);
					file_group->lines[i][0] = 0;	//line served, clear line
					break;
				case PARA_STRING_LONG:			//name with space
					k = 0;
					j = strlen(&file_group->lines[i][0]);
					while((k<j)&&(file_group->lines[i][k] != '=')) k++;
					k++;
					while(file_group->lines[i][k] == ' ') k++;
					char_ptr = parameter;
					strcpy(char_ptr,&file_group->lines[i][k]);
					file_group->lines[i][0] = 0;	//line served, clear line
					break;
				}
			break; 
			}
		}
	return(flag);
}


int read_eeprom_file(char pfad[],char *buffer)
{
char temp_str[MAXPATHNAMELEN],line[160];
int adr,adr_last,byte,i,j=0,error = 0,str_pos;
FILE *fa_r;
	
	for(i=0;i<I2C_MAXBUFFER;i++) 
		buffer[i]=0;
//	adr = -1;
//load eeprom-file:
	fa_r = fopen(pfad,"rb"); 
	if(fa_r) 
		{
		fgets(line,MAXSTRINGLEN,fa_r);
		sscanf(line,"%04x",&adr);
		adr -= 1;
		do
			{
			adr_last = adr;
			if(sscanf(line,"%04x",&adr))
  				{
   				if((adr < I2C_MAXBUFFER) && ((adr_last+1) == adr)) 
   					{
   					i = 0;
   					while((line[i] != ',') && (line[i] != ';')) i++;
   					i++;
   					if(line[i] == '"') i++;
   					sscanf(&line[i],"%02X",&byte);
   					buffer[adr] = byte;
   					}
   				}
   			else 
   				{
       			sprintf(temp_str,"bad address %s in file %s!",line,pfad);
       			MessagePopup("I2C-Datenimport",temp_str); 
		  		error = -1;
		  		break;
       			}  // ende else 
   			}while(fgets(line,MAXSTRINGLEN,fa_r));
   // ende sscanf 
  		fclose(fa_r);
 		}
	else
		{
  		sprintf(temp_str,"Can't open File: %s !!",pfad);
  		MessagePopup("Read in  EEPROM-table",temp_str);
  		error = -2;
  		}
	return(error);  
} 

int read_eeprom_special(char pfad[], struct eeprom_special *buffer)
// address zero not allowed !!
{
char temp_str[MAXPATHNAMELEN],line[160];
int adr,byte,i,j,error = 0,str_pos,addr_count;
FILE *fa_r;
	
	for(i=0;i<EEPROM_SPECIAL_RANGE;i++) 
		buffer->addresse[i] = 0; 	
//load eeprom-file:
	fa_r = fopen(pfad,"rb"); 
	if(fa_r) 
		{
		addr_count = 0;
		while(fgets(line,MAXSTRINGLEN,fa_r))
			{
			if(sscanf(line,"%X",&adr))
  				{
   				if(!adr || (adr > EEPROM_SPECIAL_RANGE)) 
    				{
    	   			sprintf(temp_str,"address %x in file %s not supported! (range: 0 < address < 0x%x) ",adr,pfad,EEPROM_SPECIAL_RANGE);
	       			MessagePopup("Error: EEPROM - load Data table",temp_str); 
					}
   				else
   					{
   					buffer->addresse[addr_count] = adr;
   					i = 0;
   					while((line[i] != ',') && (line[i] != ';')) i++;
   					i++;
   					if(line[i] == '"') i++;
   					sscanf(&line[i],"%02X",&byte);
   					buffer->value[addr_count] = (unsigned char) byte;
					addr_count++;
   					}
   				}
   			else 
   				{
       			sprintf(temp_str,"bad address %s in file %s!",line,pfad);
       			MessagePopup("Error during EEPROM-Data import",temp_str); 
       			}  // ende else false address 
   			}   // ende sscanf 
  		fclose(fa_r);
  		error = 0;
 		}
	else
		{
  		sprintf(temp_str,"Can't open File: %s !!",pfad);
  		MessagePopup("Error during EEPROM-Data import",temp_str);
  		error = -1;
  		}
	return(error);  

}


//*********************************************************************************************//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void generate_xxxi(struct part_ident_parameter *ident_struct)
{
long int psn_crypt, xxxi; 

	if(!ident_struct->type) return;

//generating 5 byte cryption number:
// generate 20 bit PSN:
	psn_crypt = 0;// N = 0;
	if(ident_struct->type == 1) //PP
		{
		if(ident_struct->serial_number & 0x80000) psn_crypt |= 0x20000; 
		if(ident_struct->serial_number & 0x40000) psn_crypt |= 0x80000; 
		if(ident_struct->serial_number & 0x20000) psn_crypt |= 0x40000; 
		}
	if(ident_struct->serial_number & 0x10000) psn_crypt |= 0x01000; 
	if(ident_struct->serial_number & 0x08000) psn_crypt |= 0x04000; 
	if(ident_struct->serial_number & 0x04000) psn_crypt |= 0x00040; 
	if(ident_struct->serial_number & 0x02000) psn_crypt |= 0x00200; 
	if(ident_struct->serial_number & 0x01000) psn_crypt |= 0x08000; 
	if(ident_struct->serial_number & 0x00800) psn_crypt |= 0x00008; 
	if(ident_struct->serial_number & 0x00400) psn_crypt |= 0x02000; 
	if(ident_struct->serial_number & 0x00200) psn_crypt |= 0x00020; 
	if(ident_struct->serial_number & 0x00100) psn_crypt |= 0x00002; 
	if(ident_struct->serial_number & 0x00080) psn_crypt |= 0x10000; 
	if(ident_struct->serial_number & 0x00040) psn_crypt |= 0x00800; 
	if(ident_struct->serial_number & 0x00020) psn_crypt |= 0x00400; 
	if(ident_struct->serial_number & 0x00010) psn_crypt |= 0x00100; 
	if(ident_struct->serial_number & 0x00008) psn_crypt |= 0x00001; 
	if(ident_struct->serial_number & 0x00004) psn_crypt |= 0x00080; 
	if(ident_struct->serial_number & 0x00002) psn_crypt |= 0x00004; 
	if(ident_struct->serial_number & 0x00001) psn_crypt |= 0x00010; 
//generate 20 bit: N + emc		

	if(ident_struct->type == 1) //PP
		{
		xxxi = (psn_crypt >> 16) & 0x0f;
		xxxi |= ident_struct->EMC<<4;
		ident_struct->IPUI[4] = psn_crypt & 0xff;
		ident_struct->IPUI[3] = (psn_crypt >> 8) & 0xff;
		ident_struct->IPUI[2] = xxxi & 0xff;
		ident_struct->IPUI[1] = (xxxi >> 8) & 0xff;
		ident_struct->IPUI[0] = (xxxi >> 16) & 0xff;
		return;
		}

//Änderrung 19.10.2001 Kalich
	if(ident_struct->type == 2) //FP
		{
		ident_struct->RFPI[4] = (psn_crypt<<3) & 0xff;
		ident_struct->RFPI[3] = (psn_crypt >> 5) & 0xff;
		ident_struct->RFPI[2] = ((psn_crypt >> 13) & 0x0f)|((ident_struct->EMC & 0x0f) <<4);
		ident_struct->RFPI[1] = (ident_struct->EMC >> 4) & 0xff;
		ident_struct->RFPI[0] = (ident_struct->EMC >> 12) & 0x0f;
		return;
		}

/*
	xxxi = (psn_crypt >> 16) & 0x0f;
	xxxi |= ident_struct->EMC<<4;
	if(ident_struct->type == 1) //PP
		{
		ident_struct->IPUI[4] = psn_crypt & 0xff;
		ident_struct->IPUI[3] = (psn_crypt >> 8) & 0xff;
		ident_struct->IPUI[2] = xxxi & 0xff;
		ident_struct->IPUI[1] = (xxxi >> 8) & 0xff;
		ident_struct->IPUI[0] = (xxxi >> 16) & 0xff;
		return;
		}
	if(ident_struct->type == 2) //FP
		{
		psn_crypt <<= 3;	//implement RPN, set 3 lsb to 0
		ident_struct->RFPI[4] = psn_crypt & 0xff;
		ident_struct->RFPI[3] = (psn_crypt >> 8) & 0xff;
		ident_struct->RFPI[2] = xxxi & 0xff;
		ident_struct->RFPI[1] = (xxxi >> 8) & 0xff;
		ident_struct->RFPI[0] = (xxxi >> 16) & 0xff;
		return;
		}
*/	
	return;
}

#ifndef RFP35_PROJECT	// [@GC16052011]
//
// diese Funktion ist nur fuer die alten RFP32 Modelle
//

int get_part_identity(struct part_ident_parameter *ident_struct)
//
//try to get a number from local path, if not possible,
//try to get a new number pool from server, write new entrys for local pool
//
//if 
//	acces possible, overwrite the local file with new numbers
//else
//	return(!0); //testfunction must quit with run-time-error
//
{
#define WARNING_NUMBER 5000
  
char 	serial_global_path[MAXPATHNAMELEN];
FILE 	*fa_rw, *fa_rw_global;
int		last_number, part_type;
int 	i,give_numbers,
		PP_actual_global,
		PP_last_global,
		FP_actual_global,
		FP_last_global,
		dummy_actual_global,
		dummy_last_global;
struct 	struct_file  file_group;



	ident_struct->serial_number = 0;
	fa_rw = fopen(ident_struct->serial_local_path,"r+");
	if(!fa_rw)
		{
		MessagePopup ("Serial Number Error", "Can't get a new serial number");
		return(-1);									// no server acces
		}
	file_get_group(&file_group,fa_rw,"[serial_settings]");
//get parameters
	file_getpara(&file_group,"EMC", &ident_struct->EMC, PARA_HEX);
	file_getpara(&file_group,"TYPE", &i, PARA_INTEGER);
	if(ident_struct->type != i)
		{
		fclose(fa_rw);
		MessagePopup ("Serial Number Error","Selected part-type not the same as in Number-Datafile !");
		return(-2);									// no server acces
		}
	if((ident_struct->type) && (!ident_struct->EMC))   // check for zero - EMC
		{
		fclose(fa_rw);
		MessagePopup ("Serial Number Error", "Selected part-type has not a valid EMC !");
		return(-3);									// 
		}
	
	file_getpara(&file_group,"actual_number", &ident_struct->serial_number, PARA_INTEGER);
	file_getpara(&file_group,"last_number", &last_number, PARA_INTEGER);
   	file_getpara(&file_group,"serial_global_path", serial_global_path, PARA_STRING);
	if(ident_struct->serial_number >= last_number)
		{
		fa_rw_global = fopen(serial_global_path,"r+");
		if(!fa_rw_global)
			{
			MessagePopup ("Serial Number Error", "Can't acces global serial number");
			fclose(fa_rw);
			return(-4);									// no server acces
			}
		file_get_group(&file_group,fa_rw_global,"[global_serial_settings]");
		file_getpara(&file_group,"give_numbers", &give_numbers, PARA_INTEGER);
		file_getpara(&file_group,"EMC", &ident_struct->EMC, PARA_HEX); //new EMC overwrite old !!
		file_getpara(&file_group,"PP_actual_number", &PP_actual_global, PARA_INTEGER);
		file_getpara(&file_group,"PP_last_number", &PP_last_global, PARA_INTEGER);
		file_getpara(&file_group,"FP_actual_number", &FP_actual_global, PARA_INTEGER);
		file_getpara(&file_group,"FP_last_number", &FP_last_global, PARA_INTEGER);
		file_getpara(&file_group,"dummy_actual_number", &dummy_actual_global, PARA_INTEGER);
		file_getpara(&file_group,"dummy_last_number", &dummy_last_global, PARA_INTEGER);
		switch(ident_struct->type)
			{
			case 0: 
				if((dummy_actual_global+give_numbers) <= dummy_last_global)
					{
					ident_struct->serial_number = dummy_actual_global;
					dummy_actual_global += give_numbers;
					last_number = dummy_actual_global;
					if((dummy_last_global - dummy_actual_global) < WARNING_NUMBER)
						MessagePopup ("Serial Number for dummy", "Please inform your supervisor, numbers going empty !!");
					}
				break;
			case 1: 
				if((PP_actual_global+give_numbers) <= PP_last_global)
					{
					ident_struct->serial_number = PP_actual_global;
					PP_actual_global += give_numbers;
					last_number = PP_actual_global;
					if((PP_last_global - PP_actual_global) < WARNING_NUMBER)
						MessagePopup ("Serial Number: PP - EMC", "Please inform your supervisor, PP-EMC is running out !!");
					}
				break;
			case 2: 
				if((FP_actual_global+give_numbers) <= FP_last_global)
					{
					ident_struct->serial_number = FP_actual_global;
					FP_actual_global += give_numbers;
					last_number = FP_actual_global;
					if((FP_last_global - FP_actual_global) < WARNING_NUMBER)
						MessagePopup ("Serial Number: FP - EMC", "Please inform your supervisor, FP-EMC is running out !!");
					}
				break;
			}
		if(ident_struct->serial_number >= last_number)
			{
			fclose(fa_rw_global);
			fclose(fa_rw);
			MessagePopup ("Serial Number Error", "global serial number empty !!");
			return(-5);
			}
		rewind (fa_rw_global);//set file-pointer to position 0
		fflush (fa_rw_global);
		fprintf(fa_rw_global,"[global_serial_settings]\n");
		fprintf(fa_rw_global,"give_numbers = %d\n", give_numbers);
		fprintf(fa_rw_global,"EMC = %04x\n", ident_struct->EMC);
		fprintf(fa_rw_global,"PP_actual_number = %d\n",PP_actual_global);
		fprintf(fa_rw_global,"PP_last_number = %d\n", PP_last_global);
		fprintf(fa_rw_global,"FP_actual_number = %d\n",FP_actual_global);
		fprintf(fa_rw_global,"FP_last_number = %d\n", FP_last_global);
		fprintf(fa_rw_global,"dummy_actual_number = %d\n",dummy_actual_global);
		fprintf(fa_rw_global,"dummy_last_number = %d\n", dummy_last_global);
		fprintf(fa_rw_global,"[end group]\n");
		fclose(fa_rw_global);
		}
	ident_struct->serial_number++;
	rewind (fa_rw);					//set file-pointer to position 0
	fflush (fa_rw);
	fprintf(fa_rw,"[serial_settings]\n");
	fprintf(fa_rw,"EMC = %04x\n",ident_struct->EMC);
	fprintf(fa_rw,"TYPE = %d",ident_struct->type);
	fprintf(fa_rw," (0-unknown part, 1-PP, 2-FP)\n");
	fprintf(fa_rw,"actual_number = %d\n", ident_struct->serial_number);
	fprintf(fa_rw,"last_number = %d\n", last_number);
	fprintf(fa_rw,"serial_global_path = %s\n", serial_global_path);
	fprintf(fa_rw,"[end group]\n");
	fclose(fa_rw);
	if(ident_struct->serial_number <= 0)
		{
		MessagePopup ("Serial Number Error", " serial_number not valid (number <= 0) !!");
		return(-6);
		}
#ifdef LABEL_SPEZIAL
	ident_struct->serial_number = 	  iSerFP_LABEL;
#endif
	generate_xxxi(ident_struct);
	return(0);
}

#endif //RFP35_PROJECT	// [@GC16052011]




//Start [@GC10122012]
/**************************************************************************************************************************************
**  printLabelOnDataBase()
**
**	  Input:   structQPDS.strExID
**	  Input:   structQPDS.strBgName
**	  Input:   structQPDS.strCustSn
**	  Input:   structQPDS.strMacAddr
**	  Input:   structQPDS.strParkNr
**    Input:   repairMode
**
**
**  Label Print, direkt aus der Datenbank
**  alle Informationen zum Label stehen in der DatenBank
**  es werden nur noch die Variablen Werte (wie Seriennummer,mac,park usw) uebergeben
**
**
**  Das Kommando
**  rsh pdbux040 -ldbhp ./print_label -s9508101336 -l AATH-87-00001AUS-A -p test -k "CD 300012098D" -P 31100461401045 -M 003042182A77
**
** Rechner: pdbux040.pdb.flextronics.com 
** Installiert: ~/bin/print_label 
** User: dbhp 
** usage: print_label -s{exid} -l{le-tnr} -p{prplace} [-z{cnt}] [-k{kdsnr}] [-M{MAC}] [-I{MAC}] [-P{PARK}] [-r] [-R{rev}] [-v]
**         -s{exid}    - Modul für das gedruckt werden soll
**         -l{le_tnr}  - Liefereinheit für das gedruckt werden soll
**         -p{prplace} - Name unter dem der/die Drucker definiert sind
**         -k{kdsnr}   - vollstaendig Kundenseriennummer (optional)
**         -z{cnt}     - zählender Anteil der Kundenseriennummer (optional)
**         -M{MAC}     - MAC Adressen (optional auch mehrfach)
**         -I{MAC}     - intern ausgelesene MAC Adressen (zur Weiterleitung an QPDS)
**         -P{PARK}    - ausgelesene PARK Nummer octal (Produktspezifisch erforderlich)
** 	-r          - Explizit Repair Label auswählen (Default: Prüfe auf REBUS Auftrag)
**         -R{rev}     - Version (optional zum Umschalten auf verschiedene Label)
** 	-v          - verbose: Fehlertexte ausgeben
** 
** 
** 
**
**
**
*/

int printLabelOnDataBase(void)
{
	FILE	*fp; 
	int     retCode;
	int     outCode;
	int     i;
	int     j;
	char    *p;
	
	char	cCmd[512];
	
	char    cRepOption[80];
	char    cExIDArgv[80];
	char    cBgNameArgv[80];
	char    cMacArgv[80];
	char    cParkArgv[80];
	char    cCustSnArgv[80];
	char    cPrinterArgv[128];

	
	char	fileNameErr[256];
	char	fileNameOut[256];
	
	char 	strMac[80];
	
	char	cTemp[2048];
	char	cInfo[2048+256];


	char    *pFail = "Fehler beim Label drucken!!\n\n     Bitte H.Wiesing / H.Rüther informieren!!";

#ifdef USE_QPDS
	// structQPDS.strMacAddr = "00:30:42:12:90:00"
	// strMac                = "003042129000"
	for( i=0, p=strMac; structQPDS.strMacAddr[i] != '\0' ; i++	)
	{
		if ( structQPDS.strMacAddr[i] >= '0' && structQPDS.strMacAddr[i] <= '9' ||
		     structQPDS.strMacAddr[i] >= 'a' && structQPDS.strMacAddr[i] <= 'f' ||
		     structQPDS.strMacAddr[i] >= 'A' && structQPDS.strMacAddr[i] <= 'F'  )
		{	
			*p = structQPDS.strMacAddr[i]; 
			p++;
		}
	}
	*p='\0';    

	// -M "003042182A77"
	if ( strlen(strMac) > 0 ) sprintf(cMacArgv," -M \"%s\"",strMac);
	else				   	  strcpy(cMacArgv,"");
	
	// -P "31100461401045"
	if ( strlen(structQPDS.strParkNr) > 0 ) sprintf(cParkArgv," -P \"%s\"",structQPDS.strParkNr);
	else				   	  				strcpy(cParkArgv,"");
	
	
	// -k "CD 300012098D"
	if ( strlen(structQPDS.strCustSn) > 0 ) sprintf(cCustSnArgv," -k \"%s\"",structQPDS.strCustSn);
	else				   	  				strcpy(cCustSnArgv,"");

	// -s "9508101336"
	sprintf(cExIDArgv," -s \"%s\"",structQPDS.strExID);

	// -l "AATH-87-00001AUS-A"
	sprintf(cBgNameArgv," -l \"%s\"",structQPDS.strBgName);

	
	// Produktion Label ohne Option -r
	// Repair     Label mit  Option -r
	if ( isInRepairMode() )	strcpy(cRepOption," -r");
	else				   	strcpy(cRepOption,"");
	

	
	// Printer "uwm088\\ZebraS4M"
	// Printer 10.219.24.88
	// ist in  G:\ProTest\ProdTest_FLEX\Rte5_3_en\RTE_SETTINGS.TXT
	// -p "test"
	sprintf(cPrinterArgv," -p \"%s\"",globals->label_printer_name);


	
    sprintf(fileNameOut,"c:\\temp\\%s_out.prn", structQPDS.strExID);
    sprintf(fileNameErr,"c:\\temp\\%s_err.prn", structQPDS.strExID);
    remove(fileNameOut);  
    remove(fileNameErr);  
	

	
   	//
	// fuer Pipe Umlenkung wird die cmd.exe benoetigt, damit stdin,stdout usw vorhanden ist
	// /s = Sonderbehandlung von doppelten ""
	// es duerfen keine leeren Argv uebergeben werden
	//
		
	// rsh pdbux040 -ldbhp ./bin/print_label -s"9508101336" -l "AATH-87-00001AUS-A" -p "test" -k "CD 300012098D" -P "31100461401045" -M "003042182A77"
   	sprintf(cCmd,"cmd.exe /s /c \"\"rsh\" pdbux040 -ldbhp ./bin/print_label%s%s%s%s%s%s%s\" 2>%s >%s",
		cRepOption,                // -r 						
		cPrinterArgv,   		   // Printer standort laut F.Wiesing Datenbank Eintrag
		cExIDArgv,                 // -s "9508101336"
		cBgNameArgv,	           // -l "AATH-87-00001AUS-A"
		cCustSnArgv,          	   // -k "CD 300012098D"
	    cParkArgv,          	   // -P "31100461401045"
	    cMacArgv, 				   // -M "003042182A77"
		fileNameErr,        // c:\\temp\\9508101336_err.prn
		fileNameOut);       // c:\\temp\\9508101336_out.prn

	
	
	if ( isInDebugMode() )
		MessagePopup("printLabelOnDataBase",cCmd);

	
	//LE_SHOWNORMAL
	//LE_HIDE
	if ( (retCode=SystemCall (cCmd, LE_HIDE)) )
	{
		//return(retCode);     // error
	}		


	
	i=0;
	j=0;
	cTemp[0]='\0';
    // datei einlesen
    // "c:\\temp\\%s_out.prn"
    if ( (fp=fopen(fileNameOut,"r")) != NULL )
    {	
		if ( (i=fread(cTemp,1,sizeof(cTemp)-1,fp)) > 0 )
		{
			cTemp[i]='\0';
		}	
    	fclose(fp);
    }
    

    // datei einlesen
    // "c:\\temp\\%s_err.prn"
    if ( (fp=fopen(fileNameErr,"r")) != NULL )
    {	
		if ( (j=fread(cTemp+i,1,sizeof(cTemp)-i-1,fp)) > 0 )
		{
			cTemp[j]='\0';
		}	
    	fclose(fp);
    }	


	// CR LF entfernen
	//for ( p=cTemp ; *p != '\0' ; p++ )
	//if ( *p == '\n' || *p == '\r' ) *p='\0';

	outCode=4;
	if ( strstr(cTemp,"Connection refused") != NULL )
	{
		outCode=3;
	}	
	else if ( strstr(cTemp,"ERROR") != NULL )
	{
		outCode=2;
	}	
	else if ( strstr(cTemp,"OK") != NULL )
	{
		outCode=0;   // OK
	}	
	
	
	
	
	if ( retCode != 0 )
	{
		sprintf(cInfo,"Can't execute %s\n%s\n%s",cCmd,cTemp,pFail); 
		MessagePopup("printLabelOnDataBase", cInfo);
		return (retCode);
	}

	if ( outCode != 0 )
	{
		sprintf(cInfo,"%s\n%s",cTemp,pFail); 
		MessagePopup("printLabelOnDataBase", cInfo);
		return (outCode);
	}

	
    remove(fileNameErr);  
    remove(fileNameOut);  

#endif	
    // all ok
	return(0);
	
}
//Ende [@GC10122012]



//Start [@GC16052011]
/**************************************************************************************************************************
** getPartNumberFromDataBase()
**	  Input:   structQPDS.strExID
**    Output:  char *partNumber
**
**	"cmd.exe /c rsh pdbux040 -l dbhp bin/getpart %s >c:\\temp\\%s.bgn", structQPDS.strExID,structQPDS.strExID
**
**  GC 16.05.2011 
**  GC 25.02.2013 auf pdbux040 umgestellt
*/
int getPartNumberFromDataBase(char *partNumber)
{
	FILE	*fp; 
	int     handle;
	int     retCode;
	char	strPart[80];
	char	cCmd[255];
	char	fileName[255];
	char	tempstr[255];
	char	*p;

#ifdef USE_QPDS	
	strcpy(partNumber,"none");
	
	sprintf(cCmd,"cmd.exe /c rsh pdbux040 -l dbhp bin/getpart %s >c:\\temp\\%s.bgn", structQPDS.strExID,structQPDS.strExID);
	
	//LE_SHOWNORMAL
	//LE_HIDE
	if ( (retCode=SystemCall (cCmd, LE_HIDE)) )
	{
		return(retCode);     // error
	}		
	

	// datei einlesen
	sprintf(fileName,"c:\\temp\\%s.bgn", structQPDS.strExID);
	if ( (fp=fopen(fileName,"r")) == NULL )
	{	
#ifdef RFP35_DEBUG_MESSAGE_POPUP	
 		sprintf(tempstr,"can't open file: %s",fileName);
		MessagePopup("Error",tempstr);
#endif		
		return(-1);    // error
	}
	fgets(strPart,80,fp);
	fclose(fp);
	remove(fileName);  

	// CR LF entfernen
	for ( p=strPart ; *p != '\0' ; p++ )
		if ( *p == '\n' || *p == '\r' ) *p='\0';

	// partNumber e.g. AATH-001298-03
	strcpy(partNumber,strPart);
#endif	
	return(0);
	
}
//Ende [@GC16052011]

//Start [@GC01092011]
/**************************************************************************************************************************
** getDeliveryUnitNumbersFromDataBase()
**	  Input:   structQPDS.strExID
**    Output:  char *deliveryUnitNumbers           List blank separated  "AATH-68637016 AATH-001280-00"
**
**	"cmd.exe /c rsh pdbux040 -l dbhp bin/bgr2le -e %s -fl >c:\\temp\\%s.dun", structQPDS.strExID,structQPDS.strExID
**
**
**usage: ./bgr2le [-e{ExId}|[-m{part}] [-o{output}] [-f{format}]
**        -e{ExId}   - serial number ExId
**        -m{part}   - part number
**        -o{output} - output file name (default: stdout)
**        -f{format} - output file format (default: original)
**           n       - output fields in multiple lines
**           k       - comma as filed seperator
**           p       - part number (ExId)
**           l       - LE part number
**           b       - Name of LE
**           e       - ExId
**
**  GC 01.09.2011 
**  GC 25.02.2013  auf pdbux040 umgestellt 
**
**
*/
int getDeliveryUnitNumbersFromDataBase(char *deliveryUnitNumbers,int len)
{
	FILE	*fp; 
	int     retCode;
	//char	strPart[80];
	char	cCmd[255];
	char	fileName[255];
	char	tempstr[255];
	char	*p;
	int     i;

#ifdef USE_QPDS	
	strcpy(deliveryUnitNumbers,"none");
	
	
	sprintf(cCmd,"cmd.exe /c rsh pdbux040 -l dbhp bin/bgr2le -e %s -fl >c:\\temp\\%s.dun", structQPDS.strExID,structQPDS.strExID);
	
	
	//LE_SHOWNORMAL
	//LE_HIDE
	if ( (retCode=SystemCall (cCmd, LE_HIDE)) )
	{
		return(retCode);     // error
	}		
	

	// datei einlesen
	sprintf(fileName,"c:\\temp\\%s.dun", structQPDS.strExID);
	if ( (fp=fopen(fileName,"r")) == NULL )
	{	
#ifdef RFP35_DEBUG_MESSAGE_POPUP	
 		sprintf(tempstr,"can't open file: %s",fileName);
		MessagePopup("Error",tempstr);
#endif		
		return(-1);    // error
	}
	
	if ( (i=fread(deliveryUnitNumbers,1,len-1,fp)) > 0 )
	{
		deliveryUnitNumbers[i]='\0';
	}	
	fclose(fp);
	remove(fileName);  

	// CR LF gegen ' ' ersetzen
	for ( p=deliveryUnitNumbers ; *p != '\0' ; p++ )
		if ( *p == '\n' || *p == '\r' ) *p=' ';
		
		
#endif	
	return(0);
	
}
//Ende [@GC01092011]




//Start [@GC16052011]
/**************************************************************************************************************************
** getShortDeTeWeSerialNumberFromDataBase()
**	  Input:   structQPDS.strExID
**    Output:  char *serialNumber
**
**
**  Ein Kommando (erzeugen und kopieren)
**	"cmd.exe /c rsh pdbux040 -l dbhp bin/get_aat_snr -e%s -fns >c:\\temp\\%s.ser", structQPDS.strExID,structQPDS.strExID
**
**  GC 16.05.2011 
**  GC 25.02.2013  auf pdbux040 umgestellt ohne MAC
*/
int getShortDeTeWeSerialNumberFromDataBase(char *serialNumber)
{
	FILE	*fp; 
	int     retCode;
	char	strSerNo[80];
	char	strMac[80];
	char	cCmd[255];
	char	fileName[255];
	char	tempstr[255];
	char	*p;

#ifdef USE_QPDS	
	strcpy(serialNumber,"0");
	
	//1. Zuordnung MAC <-> FlexID

	//2. Zuordnung Seriennummer <-> FlexID; setzen Dateiname(in ueun47) und -format:
	// -unterschiedliche Zeilen für Datensätze
	// -Seriennummer in Kurzform
	// -erste MAC-Adresse
	
	sprintf(cCmd,"cmd.exe /c rsh pdbux040 -l dbhp bin/get_aat_snr -e%s -fns >c:\\temp\\%s.sser", structQPDS.strExID,structQPDS.strExID);
	
	
	//LE_SHOWNORMAL
	//LE_HIDE
	if ( (retCode=SystemCall (cCmd, LE_HIDE)) )
	{
		return(retCode);     // error
	}		
	

	// datei einlesen
	sprintf(fileName,"c:\\temp\\%s.sser", structQPDS.strExID);
	if ( (fp=fopen(fileName,"r")) == NULL )
	{	
#ifdef RFP35_DEBUG_MESSAGE_POPUP	
 		sprintf(tempstr,"can't open file: %s",fileName);
		MessagePopup("Error",tempstr);
#endif		
		return(-1);    // error
	}
	fgets(strSerNo,80,fp);
	//fgets(strMac,80,fp);
	fclose(fp);

	// CR LF entfernen
	for ( p=strSerNo ; *p != '\0' ; p++ )
		if ( *p == '\n' || *p == '\r' ) *p='\0';

	// D2 3000149312
	//     00014931
	if ( strlen(strSerNo) != 8 )
	{
#ifdef RFP35_DEBUG_MESSAGE_POPUP	
		sprintf(tempstr, "Error: Wrong ShortDeTeWeSerialNumber: [%s]", strSerNo);
		MessagePopup("getShortDeTeWeSerialNumberFromDataBase", tempstr);
#endif			
		return (-1);
	}			
	remove(fileName);  

	// short serial Number
	strcpy(serialNumber,strSerNo);
#endif	
	return(0);
	
}
//Ende [@GC16052011]



//Start [@GC16052011]
/**************************************************************************************************************************
**  getLongDeTeWeSerialNumberFromDataBase()
**	  Input:   structQPDS.strExID
**    Output:  char *serialNumber
**
**
**  Ein Kommando (erzeugen und kopieren)
**  cmd.exe /c rsh pdbux040 -l dbhp bin/get_aat_snr -e%s -fnl >c:\\temp\\%s.lser", structQPDS.strExID,structQPDS.strExID
**
**  GC 16.05.2011 
**  GC 25.02.2013  auf pdbux040 umgestellt, keine MAC abfrage 
*/
int getLongDeTeWeSerialNumberFromDataBase(char *serialNumber)
/*
get a number from data base
*/
{
	FILE	*fp; 
	int     retCode;
	char	strSerNo[80];
	char	strMac[80];
	char	cCmd[255];
	char	fileName[255];
	char	tempstr[255];
	char	*p;

#ifdef USE_QPDS	
	strcpy(serialNumber,"0");
	sprintf(cCmd,"cmd.exe /c rsh pdbux040 -l dbhp bin/get_aat_snr -e%s -fnl >c:\\temp\\%s.lser", structQPDS.strExID,structQPDS.strExID);

	//LE_SHOWNORMAL
	//LE_HIDE
	if ( (retCode=SystemCall (cCmd, LE_HIDE)) )
	{
		return(retCode);     // error
	}		
	

	// datei einlesen
	sprintf(fileName,"c:\\temp\\%s.lser", structQPDS.strExID);
	if ( (fp=fopen(fileName,"r")) == NULL )
	{	
#ifdef RFP35_DEBUG_MESSAGE_POPUP	
 		sprintf(tempstr,"can't open file: %s",fileName);
		MessagePopup("Error",tempstr);
#endif		
		return(-1);    // error
	}
	fgets(strSerNo,80,fp);
	//fgets(strMac,80,fp);
	fclose(fp);

	// CR LF entfernen
	for ( p=strSerNo ; *p != '\0' ; p++ )
		if ( *p == '\n' || *p == '\r' ) *p='\0';

	// D2 3000149312
	if ( strlen(strSerNo) != 13 )
	{
#ifdef RFP35_DEBUG_MESSAGE_POPUP	
		sprintf(tempstr, "Error: Wrong LongDeTeWeSerialNumber: [%s]", strSerNo);
		MessagePopup("getLongDeTeWeSerialNumberFromDataBase", tempstr);
#endif			
		return (-1);
	}			
	remove(fileName);  


	// long serial Number
	strcpy(serialNumber,strSerNo);
#endif	
	return(0);
	
}	
//Ende [@GC16052011]


//Start [@GC16052011]
/***********************************************************************************************************************
** getPartInfoFromDataBase()
**	  Input:   structQPDS.strExID
**    Output:  char *macAddr
**    Output:  char *partnummer
**    Output:  char *custSnr
**
**	"cmd.exe /c rsh pdbux040 -l dbhp bin/get_info -e %s -fnspm >c:\\temp\\%s.inn", structQPDS.strExID,structQPDS.strExID
**
**  Das Kommando wird nur einmalig pro Testdurchlauf ausgeführt, die Variablen werden Zwischengespeichert,
**  und können mit der gleichen Funktion erneut geholt werden.
**
**  CustSnr Zeile1 B6 300000023C
**	Mac     Zeile2 003042126AAC
**	PartNr  Zeile3 AATH-001298-83      // vor dem Final test ist das die Nummer der FlachBaugruppe, nach dem Final Test ist das die Nummer vom Gerät
**                                                                      // je nach dem Welcher Test als letztes ausgeführt wurde hat diese Nummer eine Andere Bedeutung
**
**  GC 09.06.2011 
**  GC 25.02.2013  auf pdbux040 umgestellt 
*/

int getPartInfoFromDataBase(char *serialNumber, char *partNumber, char *macAddr)
{
#ifdef USE_QPDS
	FILE	*fp; 
	int     retCode;

	char	cCmd[255];
	char	fileName[255];
	char	tempstr[255];
	char	*p;
	
//dies struct nur intern zu erreichen	
static struct structPartInfo
	{	
	char strExId[80]; 
	char strPartNumber[80]; 
	char strCustSnr[80]; 
	char strMacAddr[80]; 
	} partInfo = { "","","","" };
	
	
	// alles auf blank
	if ( serialNumber != NULL ) strcpy(serialNumber,"----------");
	if ( partNumber != NULL )   strcpy(partNumber,  "----------");
	if ( macAddr != NULL )      strcpy(macAddr,     "--------");

	
	
	if ( strcmp(structQPDS.strExID,partInfo.strExId) != 0 )
	{
		// exid passt nicht zur zwischen gespeicherten also den DB Zugriff machen
		
		partInfo.strCustSnr[0]='\0';
		partInfo.strPartNumber[0]='\0';
		partInfo.strMacAddr[0]='\0';
		partInfo.strExId[0]='\0';
		
		
		sprintf(cCmd,"cmd.exe /c rsh pdbux040 -l dbhp bin/get_info -e %s -fnspm >c:\\temp\\%s.inn", structQPDS.strExID,structQPDS.strExID);
		
		
		//LE_SHOWNORMAL
		//LE_HIDE
		if ( (retCode=SystemCall (cCmd, LE_HIDE)) )
		{
			return(retCode);     // error
		}		
		

		// datei einlesen
		sprintf(fileName,"c:\\temp\\%s.inn", structQPDS.strExID);
		if ( (fp=fopen(fileName,"r")) == NULL )
		{	
#ifdef RFP35_DEBUG_MESSAGE_POPUP	
	 		sprintf(tempstr,"can't open file: %s",fileName);
			MessagePopup("Error",tempstr);
#endif			
			return(-1);    // error
		}
		fgets(partInfo.strCustSnr,80,fp);
		fgets(partInfo.strPartNumber,80,fp);
		fgets(partInfo.strMacAddr,80,fp);
		fclose(fp);

		// CR LF entfernen
		for ( p=partInfo.strCustSnr ; *p != '\0' ; p++ )
			if ( *p == '\n' || *p == '\r' ) *p='\0';
		for ( p=partInfo.strPartNumber ; *p != '\0' ; p++ )
			if ( *p == '\n' || *p == '\r' ) *p='\0';
		for ( p=partInfo.strMacAddr ; *p != '\0' ; p++ )
			if ( *p == '\n' || *p == '\r' ) *p='\0';

		// zugriff hat nicht geklappt oder Nummer noch nicht vergeben	
		if ( strlen(partInfo.strCustSnr) < 2 )
		{
			
#ifdef RFP35_DEBUG_MESSAGE_POPUP	
			sprintf(tempstr, "Wrong CustomerSerialnumber: [%s] [%s] [%s] [%s]", structQPDS.strExID, partInfo.strCustSnr, partInfo.strPartNumber, partInfo.strMacAddr);
			MessagePopup("Error", tempstr);
#endif			
			return (-1);
		}
		remove(fileName);  
			

		// Exid passt jetzt zum Temporären Zwischenspeicher
		strcpy(partInfo.strExId,structQPDS.strExID);
	}

	// jetzt passt es
	if ( strcmp(structQPDS.strExID,partInfo.strExId) == 0 )
	{
		if ( serialNumber != NULL ) strcpy(serialNumber,partInfo.strCustSnr);
		if ( partNumber != NULL )   strcpy(partNumber,partInfo.strPartNumber);
		if ( macAddr != NULL )      strcpy(macAddr,partInfo.strMacAddr);
	}	
#endif	
	return(0);
	
}
//Ende [@GC16052011]


//Start [@GC16052011]
/**************************************************************************************************************************************
**  getMacAddrFromDataBase()
**	  Input:   structQPDS.strExID
**	  Input:   structQPDS.strBgName
**    Output:  char *strMacAdress
**
**  MAC Addr generieren, und in den Pointer strMacAdress ablegen
**  ist der Pointer strMacAdress NULL wird nur generiert
**
**  Die macAddr wird ausserdem noch zwischen gespeichert, so das mehrfach zugriffe nicht zu einem erneuten DB Zugriff führt
**
**
**  Das gleiche in einem Kommando
**  ein Kommando (erzeugen und kopieren)
**  auf der pdbux040 wird die PartNummer nicht mehr als Kriterium benötigt,
**	es wird nur noch das Model DTW benötigt
**  "cmd /c rsh pdbux040 -ldbhp bin/get_aat_mac -mDTW -s%s -fH >c:\\temp\\%s.mac",structQPDS.strExID,structQPDS.strExID );
**
**  GC 16.05.2011 
**  GC 25.02.2013  auf pdbux040 umgestellt 
**
**  Diese Funktion ist nur für den Funktionstest
**
*/
int getMacAddrFromDataBase(char *strMacAddress)
{
#ifdef USE_QPDS
	FILE	*fp; 
	int     retCode;
	char	cCmd[255];
	char	fileName[255];
	char	tempstr[255];
	char	*p;

	
/*	usage: get_aat_mac -s<Serien-Nr.> -m<Modell> [-n<nb>] [-f<Format>] [ -o<file>] [-R] [-i]
**		-s<Serien-Nr.> - Seriennummer des Modules
**		-m<Modell>     - Produktgruppe (PBX957, PBX958, DTW oder A470_CPU2)
**		-n<nb>         - Anzahl MAC-Adressen (Default 1)
**		-f<format>     - Ausgabeformat
**		  h - <sernr> <HHHHHHHHHHHHHH>
**		       Seriennummer + MAC als HEX (Default)
**		  H - <HHHHHHHHHHHHHH>
**		       MAC als HEX
**		  d - <sernr> <d> <d> <d> <d> <d> <d> <d>
**		       Seriennummer + MAC dezimal mit Blanks
**		  D - <d> <d> <d> <d> <d> <d> <d>
**		       MAC dezimal mit Blanks
**		  B - spezielles Format (Bilbao)
**		      mac_addr_pref       =   XXXXXX
**		      fe_mac_addr_start   =   000000
**		      fe_mac_addr_end     =   000000
**		      mac_addr_start      =   XXXXXX
**		      mac_addr_end        =   XXXXXX
**		-o<file>       - Zielfilename default: ./stdout
**		-R    Reparatur neue MAC adresse vergeben
**		-i    Info mode: nur eine schon zugewiesene MAC Adresse ausgeben (explizit gesetzt bei info_aat_mac)
*/


	
//diese struct ist nur intern zu erreichen	
static struct 
{	
	char exid[80]; 
	char macAddr[80]; 
} m = { "","" };
	
	
    if ( strMacAddress != NULL ) strcpy(strMacAddress,"000000000000");
        
	
   	//
	// fuer Pipe Umlenkung wird die cmd.exe benoetigt, damit stdin,stdout usw vorhanden ist
	//


	// noch nicht aktuell also vom server holen
	if ( strcmp( structQPDS.strExID ,m.exid) != 0 )
	{
	
	
	    // generate and create local file %s.mac
		// -mDTW = Model
    	sprintf(cCmd,"cmd /c rsh pdbux040 -ldbhp bin/get_aat_mac -mDTW -s%s -fH >c:\\temp\\%s.mac",structQPDS.strExID,structQPDS.strExID );
		


		//LE_SHOWNORMAL
		//LE_HIDE
		if ( (retCode=SystemCall (cCmd, LE_HIDE)) )
		{
			return(retCode);     // error
		}		

        // datei einlesen
        sprintf(fileName,"c:\\temp\\%s.mac", structQPDS.strExID);
        if ( (fp=fopen(fileName,"r")) == NULL )
        {	
#ifdef RFP35_DEBUG_MESSAGE_POPUP	
           	sprintf(tempstr,"can't open file: %s",fileName);
        	MessagePopup("Error",tempstr);
#endif        	
            return(-1);    // error
        }
       	fgets(m.macAddr, 40, fp); 
        fclose(fp);

		// CR LF entfernen
		for ( p=m.macAddr ; *p != '\0' ; p++ )
			if ( *p == '\n' || *p == '\r' ) *p='\0';

		// 	Blanks entfernen
		for ( p=m.macAddr ; *p != '\0' ; p++ )
			if ( *p == ' ' || *p == '\t' ) *p='\0';

		if ( strlen(m.macAddr) != 12 )
		{	
#ifdef RFP35_DEBUG_MESSAGE_POPUP	
			sprintf(tempstr, "Error: Wrong macAddr: [%s]", m.macAddr);
			MessagePopup("getMacAddrFromDataBase", tempstr);
#endif			
			return (-1);
		}			
        remove(fileName);  
		
		strcpy(m.exid,structQPDS.strExID);
    }

	if ( strMacAddress != NULL && strcmp( structQPDS.strExID ,m.exid) == 0 )
	{

	    strcpy(strMacAddress,m.macAddr);
	}	
	
    // all ok
#endif    
	return(0);
	
}	
//Ende [@GC16052011]

//Start [@GC22092012]
/**************************************************************************************************************************************
**  getParkNumberFromDataBase()
**	  Input:   structQPDS.strExID
**    Output:  char *strParkNumber
**
**  PARK Number generieren, und in den Pointer strParkNumber ablegen
**  ist der Pointer strParkNumber NULL wird nur generiert.
**  Ret: 10 Stellen HEX Wert  (5 Byte Werte)
**
**  Die PARK-Number wird ausserdem noch zwischen gespeichert, so das mehrfach Zugriffe nicht zu einem erneuten DB Zugriff führen
**
** rsh pdbux040 -ldbhp bin/get_aat_park  -fH -s 9508032859
** FFFFFFFFF0
**
** usage: get_aat_park -s<Serien-Nr.> [-n<nb>] [-f<Format>] [ -o<file>] [-R] [-i]
**                 -s<Serien-Nr.> - Seriennummer des Modules
**                 -n<nb>         - Anzahl PARK (default 1)
**                 -f<format>     - Ausgabeformat
**                   h - <sernr> <HHHHHHHHHH>
**                        Serienummer + PARK-Nummer als HEX (Default)
**                   H - <HHHHHHHHHH>
**                        PARK-Nummer als HEX
**                   d - <sernr> <d> <d> <d> <d> <d> <d> <d>
**                        Seriennummer + PARK-Nummer dezimal mit Blanks
**                   D - <d> <d> <d> <d> <d> <d> <d>
**                        PARK-Nummer dezimal mit Blanks
**                 -o<file>       - Zielfilename default: ./stdout
**                 -R    Reparatur: neue PARK Nummer vergeben
**                 -i    Info mode: nur eine schon zugewiesene PARK Adresse ausgeben
**
**  Das Kommando
**  "cmd /c rsh pdbux040 -ldbhp bin/get_aat_park -fH -s%s > c:\\temp\\%s.prk",structQPDS.strExID,structQPDS.strExID );
**
**  Diese Funktion ist nur für den Funktionstest (mit Generierung der PARK-Nummer)
**
*/
int getParkNumberFromDataBase(char *strParkNumber)
{
#ifdef USE_QPDS
	FILE	*fp; 
	int     retCode;
	char	cCmd[255];
	char	fileName[255];
	char	tempstr[255];
	char	*p;

	
//diese struct ist nur intern zu erreichen	
static struct 
{	
	char exid[80]; 
	char parkNumber[80]; 
} park = { "","" };
	
	
    if ( strParkNumber != NULL ) strcpy(strParkNumber,"0000000000");
        
	
   	//
	// fuer Pipe Umlenkung wird die cmd.exe benoetigt, damit stdin,stdout usw vorhanden ist
	//

	// m.exid ist noch nicht aktuell also vom Server holen
	if ( strcmp( structQPDS.strExID ,park.exid) != 0 )
	{
	
	
	    // generate and create local file %s.prk
    	sprintf(cCmd,"cmd /c rsh pdbux040 -ldbhp bin/get_aat_park -fH -s%s >c:\\temp\\%s.prk",structQPDS.strExID,structQPDS.strExID );

   		//MessagePopup("getParkNumberFromDataBase",cCmd);

		//LE_SHOWNORMAL
		//LE_HIDE
		if ( (retCode=SystemCall (cCmd, LE_HIDE)) )
		{
			return(retCode);     // error
		}		

        // datei einlesen
        sprintf(fileName,"c:\\temp\\%s.prk", structQPDS.strExID);
        if ( (fp=fopen(fileName,"r")) == NULL )
        {	
#ifdef RFP35_DEBUG_MESSAGE_POPUP	
           	sprintf(tempstr,"can't open file: %s",fileName);
        	MessagePopup("Error",tempstr);
#endif        	
            return(-1);    // error
        }
       	fgets(park.parkNumber, 40, fp); 
        fclose(fp);

		// CR LF entfernen
		for ( p=park.parkNumber ; *p != '\0' ; p++ )
			if ( *p == '\n' || *p == '\r' ) *p='\0';

		if ( strlen(park.parkNumber) != 10 ) 
		{	
#ifdef RFP35_DEBUG_MESSAGE_POPUP	
			sprintf(tempstr, "Error: Wrong parkNumber: %s", park.parkNumber);
			MessagePopup("getParkNumberFromDataBase", tempstr);
#endif			
			return (-1);
		}			
        remove(fileName);  
		
		strcpy(park.exid,structQPDS.strExID);
    }

	if ( strParkNumber != NULL && strcmp( structQPDS.strExID ,park.exid) == 0 )
	{

	    strcpy(strParkNumber,park.parkNumber);
	}	
	
    // all ok
#endif    
	return(0);
	
}
/**************************************************************************************************************************************
**  getParkNumberInfoFromDataBase()
**	  Input:   structQPDS.strExID
**    Output:  char *strParkNumber
**
**  PARK Number vom Server lesen,(nicht generieren) und in den Pointer strParkNumber ablegen
**  Ret: 10 Stellen HEX Wert  (5 Byte Werte)
**
**  Die PARK-Number wird ausserdem noch zwischen gespeichert, so das mehrfach Zugriffe nicht zu einem erneuten DB Zugriff führen
**
** rsh pdbux040 -ldbhp bin/get_aat_park -i -fH -s 9508032859
** FFFFFFFFF0
**
** usage: get_aat_park -s<Serien-Nr.> [-n<nb>] [-f<Format>] [ -o<file>] [-R] [-i]
**                 -s<Serien-Nr.> - Seriennummer des Modules
**                 -n<nb>         - Anzahl PARK (default 1)
**                 -f<format>     - Ausgabeformat
**                   h - <sernr> <HHHHHHHHHH>
**                        Serienummer + PARK-Nummer als HEX (Default)
**                   H - <HHHHHHHHHH>
**                        PARK-Nummer als HEX
**                   d - <sernr> <d> <d> <d> <d> <d> <d> <d>
**                        Seriennummer + PARK-Nummer dezimal mit Blanks
**                   D - <d> <d> <d> <d> <d> <d> <d>
**                        PARK-Nummer dezimal mit Blanks
**                 -o<file>       - Zielfilename default: ./stdout
**                 -R    Reparatur: neue PARK Nummer vergeben
**                 -i    Info mode: nur eine schon zugewiesene PARK Adresse ausgeben
**
**  Das Kommando
**  "cmd /c rsh pdbux040 -ldbhp bin/get_aat_park -i -fH -s%s > c:\\temp\\%s.prk",structQPDS.strExID,structQPDS.strExID );
**
**  Diese Funktion ist nur für den Finaltest (ohne Generierung der PARK-Nummer)
**
*/
int getParkNumberInfoFromDataBase(char *strParkNumber)
{
#ifdef USE_QPDS
	FILE	*fp; 
	int     retCode;
	char	cCmd[255];
	char	fileName[255];
	char	tempstr[255];
	char	*p;

	
//diese struct ist nur intern zu erreichen	
static struct 
{	
	char exid[80]; 
	char parkNumber[80]; 
} park = { "","" };
	
	
    if ( strParkNumber != NULL ) strcpy(strParkNumber,"0000000000");
        
	
   	//
	// fuer Pipe Umlenkung wird die cmd.exe benoetigt, damit stdin,stdout usw vorhanden ist
	//

	// m.exid ist noch nicht aktuell also vom Server holen
	if ( strcmp( structQPDS.strExID ,park.exid) != 0 )
	{
	
	
	    // generate and create local file %s.prk
    	sprintf(cCmd,"cmd /c rsh pdbux040 -ldbhp bin/get_aat_park -i -fH -s%s >c:\\temp\\%s.prk",structQPDS.strExID,structQPDS.strExID );

   		//MessagePopup("getParkNumberInfoFromDataBase",cCmd);

		//LE_SHOWNORMAL
		//LE_HIDE
		if ( (retCode=SystemCall (cCmd, LE_HIDE)) )
		{
			return(retCode);     // error
		}		

        // datei einlesen
        sprintf(fileName,"c:\\temp\\%s.prk", structQPDS.strExID);
        if ( (fp=fopen(fileName,"r")) == NULL )
        {	
#ifdef RFP35_DEBUG_MESSAGE_POPUP	
           	sprintf(tempstr,"can't open file: %s",fileName);
        	MessagePopup("Error",tempstr);
#endif        	
            return(-1);    // error
        }
       	fgets(park.parkNumber, 40, fp); 
        fclose(fp);

		// CR LF entfernen
		for ( p=park.parkNumber ; *p != '\0' ; p++ )
			if ( *p == '\n' || *p == '\r' ) *p='\0';

		if ( strlen(park.parkNumber) != 10 ) 
		{	
#ifdef RFP35_DEBUG_MESSAGE_POPUP	
			sprintf(tempstr, "Error: Wrong parkNumber: %s", park.parkNumber);
			MessagePopup("getParkNumberInfoFromDataBase", tempstr);
#endif			
			return (-1);
		}			
        remove(fileName);  
		
		strcpy(park.exid,structQPDS.strExID);
    }

	if ( strParkNumber != NULL && strcmp( structQPDS.strExID ,park.exid) == 0 )
	{

	    strcpy(strParkNumber,park.parkNumber);
	}	
	
    // all ok
#endif    
	return(0);
	
}	


//Ende [@GC22092012]















//Start [@JM120907]
int get_part_identity_db(struct part_ident_parameter *ident_struct)
/*
get a number from data base

if 
	acces possible, 
else
	return(!0); //testfunction must quit with run-time-error
*/
{
	char	cSerNoStr[20] = {0};

	//reset global serial number
	ident_struct->serial_number = 0;
	
	if ( getShortDeTeWeSerialNumberFromDataBase(cSerNoStr) )
	{	
		return(-1);
	}
	
	ident_struct->serial_number = atoi(cSerNoStr);

	

#ifdef LABEL_SPEZIAL
	ident_struct->serial_number = 	  iSerFP_LABEL;
#endif
	generate_xxxi(ident_struct);
	return(0);
}
//End   [@JM120907]


#ifndef RFP35_PROJECT	
//
// diese Funktion ist nur fuer die alten RFP32 Modelle
//

#ifdef PRINT_LABEL
int print_label_global(int flag)
{
//print label:
/*----> outline: single line (without tabs)
    Nr1             Nr2             Nr3             Nr4             Nr5                 Nr6     Nr7         Nr8         Nr9         Nr10            Nr11                Nr12             Nr13    Nr14                Nr15           Nr16
	401961799999,	12345.678,		12345.678,		12345.678,		12345.678,			03/99,	654321,		123456,		123456,		123456,		  	360000123456789		00407 0173096 0  X5      00:00:00:00:00:00   X5 300123456A  xx
	user-barcode	Set-sachnumber	fp-sachnumber	pp-sachnumber	charger-sachnumber	date	FP-serial	PP-serial1	PP-serial2	Charger-serial	Park				Label-IPEI       date    mac                 DeteweSerial   AvajaSerial
	(EAN)			(Sachnbr.)		(Sachnbr.)      (Sachnbr.)      (Sachnbr.)      
*/								

FILE 						*fa_w;
char						tempstr[160];
char						label_str[260];
long int			number; 
int							first_number,year;
char 		*Date;
char 		*id;
char 		park_number[16+1], //+1 for ","
			ser_sachn_pp[20],ser_sachn_fp[20],
			temp_str[30];
int			i,j,k,e_number,p_number,chk_sum,add_number;
unsigned char chk_digit; 
char		str_din_date[3] = {0};

char		str_din_year[] = {'A','B','C','D','E','F','H','J','K','L','M','N','P','R','S','T','U','V','W','X'};
BYTE 		temp_value;
unsigned char 	RFPI_global_sicher[5]; 
//char 	SerialNrAvaja[20] = {0};    
unsigned char detewe_Sern[20] = {0};
int help_add_int;
int help_int;



	Date = DateStr ();

	Date[2] = '/';
	Date[3] = Date[8];
	Date[4] = Date[9];
	Date[5] = 0;

	//labelNr1		 
	if(globals->left->executeable_tests)
		strcpy(label_str,user_barcode);
	else
		strcpy(label_str,user_barcode_pp);
     //label                        Nr2            Nr3           Nr4           Nr5    
	sprintf(tempstr,",%s,%s,%s,%s,",set_sachnumber,fp_sachnumber,pp_sachnumber,charger_sachnumber);
	strcat(label_str,tempstr);	
	
	ser_sachn_fp[0] = fp_sachnumber[6];
	ser_sachn_fp[1] = fp_sachnumber[7];
	ser_sachn_fp[2] = 0;

	ser_sachn_pp[0] = pp_sachnumber[6];
	ser_sachn_pp[1] = pp_sachnumber[7];
	ser_sachn_pp[2] = 0;
	
#ifdef BEETEL_ANORMALIE
	add_number = emc_counter_fp * 140000;
	serial_FP = serial_FP + add_number;
#endif

     //label                        Nr6  Nr7
	sprintf(tempstr,"%s,%s%d%0.6d,",Date,ser_sachn_fp,emc_counter_fp,serial_FP);
	strcat(label_str,tempstr);
     //label                     Nr8
	sprintf(tempstr,"%s%d%0.6d,",ser_sachn_pp,emc_counter_pp,serial_PP1);
	strcat(label_str,tempstr);
     //label                     Nr9
	sprintf(tempstr,"%s%d%0.6d,",ser_sachn_pp,emc_counter_pp,serial_PP2);
	strcat(label_str,tempstr);
	if(flag)   
		{
// get serial number for charger
		if(get_part_identity(part_ident_parameter)) return (0x0800);
		serial_charger = part_ident_parameter->serial_number;
        //label                   Nr10
		sprintf(tempstr,"%0.6d,", serial_charger);
		strcat(label_str,tempstr);
		}
	else
		strcat(label_str,","); // no parameter

//****create the park-number from RFPI:**********************
// first 2 digits: length of used RFPI (5bytes, most significant nibble ever 0) = 36  | example: RFPI = 001971088e
// set RFPI to octal value      													  | octal:   003134204216 = 011.001.011.100.010.000.100.010.001.110
// msb left, count numbers of used bits	--> fixed :36 (complete RFPI, first nibble ever 0)
// fill number of bits up to 15 with 0's from right (msb stay's left)				  |							011.001.011.100.010.000.100.010.001.110
// calculate lsd: add all 1's multiplied by position								  |
// divide by 11, module multiplied by 11--> last digit, overwrite last digit with chksum(index15) 																	  |
/*
//sample: RFPI 0019700dbd
	RFPI_global[0] = 0x00;
	RFPI_global[1] = 0x19;
	RFPI_global[2] = 0x70;
	RFPI_global[3] = 0x0d;
	RFPI_global[4] = 0xbd;
*/	
			RFPI_global_sicher[0] = RFPI_global[0];
			RFPI_global_sicher[1] = RFPI_global[1];
			RFPI_global_sicher[2] = RFPI_global[2];
			RFPI_global_sicher[3] = RFPI_global[3];
			RFPI_global_sicher[4] = RFPI_global[4];

	park_number[0] = '3';	
	park_number[1] = '6';	
//convert to octal (39bit !!):	
	RFPI_global[0] &= 0x7f; //msb = 0
	for(i=14;i>=2;i--)
	{
		park_number[i] = (RFPI_global[4] & 0x07) + 0x30;
		RFPI_global[4] >>= 3;
		temp_value = RFPI_global[3] & 0x07;
		RFPI_global[4] |= (temp_value << 5);
		RFPI_global[3] >>= 3;
		temp_value = RFPI_global[2] & 0x07;
		RFPI_global[3] |= (temp_value << 5);
		RFPI_global[2] >>= 3;
		temp_value = RFPI_global[1] & 0x07;
		RFPI_global[2] |= (temp_value << 5);
		RFPI_global[1] >>= 3;
		temp_value = RFPI_global[0] & 0x07;
		RFPI_global[1] |= (temp_value << 5);
		RFPI_global[0] >>= 3;
	}

	//create checksum for last digit:
	number = 0;
	for(i=0;i<15;i++)
		number += (park_number[i] - 0x30) * (i+1);
	while(number >= 0)
		number -= 11;
	number += 11;
	park_number[14] = number + 0x30; 
	if(number == 10)
		park_number[14] = '*';

	park_number[15] = ',';
	park_number[16] = 0;
    //label          Nr11
	strcat(label_str,park_number);

	RFPI_global[0] = RFPI_global_sicher[0];
	RFPI_global[1] = RFPI_global_sicher[1];
	RFPI_global[2] = RFPI_global_sicher[2];
	RFPI_global[3] = RFPI_global_sicher[3];
	RFPI_global[4] = RFPI_global_sicher[4];

	// process IPEI-Label-string:
	//-------------------------------------------------------
	// process EMC:
	e_number = (IPEI_global[2]&0xf0)>>4;
	e_number += IPEI_global[1] * 0x10;
	e_number += IPEI_global[0] * 0x1000;
	//process PSN:	
	p_number = (IPEI_global[2]&0x0f)*0x10000;
	p_number += IPEI_global[3] * 0x100;
	p_number += IPEI_global[4];
	for(i=0;i<16;i++) temp_str[i]=0;
	sprintf(temp_str,"%05d",e_number);
	sprintf(&temp_str[5],"%07d",p_number);
	chk_sum = 0;
	
	for(i=0;i<12;i++)
		chk_sum += (temp_str[i] - 0x30) * (i+1);
	chk_sum %= 11;
	chk_digit = (unsigned char) (chk_sum);
	if(chk_digit == 10)
		chk_digit = '*';
	else
		chk_digit += 0x30;
    //label          Nr12
	sprintf(temp_str,"%05d %07d %c,",e_number,p_number,chk_digit);		
	strcat(label_str,temp_str);
//***********************************
	Date = DateStr ();


	if(Date[0] == '1')
		{
		if(Date[1] == '0')
			str_din_date[1] = 'O';	//Oktober		
		if(Date[1] == '1')
			str_din_date[1] = 'N';	//November		
		if(Date[1] == '2')
			str_din_date[1] = 'D'; //Dezember
		}
	
	else
		str_din_date[1] = Date[1];			
		
	year = (int)atoi(&Date[6]);
	year = year - 1990;		 //realy year
	while(year >= 20)
		year -= 20;
	str_din_date[0] = str_din_year[year];	
    //label          Nr13
	sprintf(temp_str,"%s,",str_din_date);		
	strcat(label_str,temp_str);
	
	
	
	
	
	
#ifdef MAC_ADRESS	
	sprintf(temp_str,"%c%c:%c%c:%c%c:%c%c:%c%c:%c%c,",	mac_adress_parameter->strMac_adress[0]
													,	mac_adress_parameter->strMac_adress[1]
													,	mac_adress_parameter->strMac_adress[2]
													,	mac_adress_parameter->strMac_adress[3]
													,	mac_adress_parameter->strMac_adress[4]
													,	mac_adress_parameter->strMac_adress[5]
													,	mac_adress_parameter->strMac_adress[6]
													,	mac_adress_parameter->strMac_adress[7]
													,	mac_adress_parameter->strMac_adress[8]
													,	mac_adress_parameter->strMac_adress[9]
													,	mac_adress_parameter->strMac_adress[10]
													,	mac_adress_parameter->strMac_adress[11]);
    //label          Nr14
	strcat(label_str,temp_str);  
#endif	 


	generateDeTeWeSerialNumber(detewe_Sern);

    //label          Nr15
	strcat(label_str,detewe_Sern); 
	
 //////////////////SerialNrAvaja///////////////////////////////
	
//	get_avaja_ser(SerialNrAvaja);
	
	sprintf(temp_str,",%s ",SerialNrAvaja);
    //label          Nr16
	strcat(label_str,temp_str);
//////////////////////////////////////////////////////



	fa_w = fopen(label_datei,"w");
	if(fa_w)	
		{
		fprintf(fa_w,label_str);
		fclose(fa_w);
		}
	else return(0x0801);
		
#ifdef USE_BARTENDER
//Start [@JM030308]
//execute only for no db-based Label
if ( get_printermode_db() == 0 )       //same as  !get_printermode_db() 
//Ende [@JM030308]
	LaunchExecutable(globals->label_command_line);
#endif	// USE_BARTENDER
	return(0);
}		
#endif 	//PRINT_LABEL

#endif //ifndef RFP35_PROJECT	


int emc_counter(char *rfpi_ipei_dut, char *emc_list)
{
char temp_emc[10];
char temp_emc_list[MAXPATHNAMELEN]; 
char temp_rfpi_ipei[10]; 
int iStrlaenge,equal;
char *tempstr;
int   rfpi_ipei_help, emc_help;
int emc_counter;
char *error;	
	


	strcpy(temp_emc_list,emc_list);
	emc_help		=  (int)(rfpi_ipei_dut[0]  *  0x1000);
	emc_help		+= (int)(rfpi_ipei_dut[1]  *  0x10);
	emc_help		+= (int)((rfpi_ipei_dut[2] & 0xf0 ) >> 4);
	
	iStrlaenge = strlen(temp_emc_list);
	emc_counter = 0;
	equal = 0;
	while(iStrlaenge)
		{
		tempstr =strchr(temp_emc_list,44);
		if(tempstr)
			{
			*tempstr = 0;
			strcpy(temp_emc,temp_emc_list);
			tempstr++;
			strcpy(temp_emc_list,tempstr);
			rfpi_ipei_help 	=(int)strtol(temp_emc,&error,16);
			emc_counter++;
			if(rfpi_ipei_help == emc_help)
				{
				equal = 1;
				break;
				}
			}
		else
			{
			break;
			}
		iStrlaenge--;
		}	
// EMC_COUNTER with zerro started
	if(emc_counter)
	 	emc_counter--;

	if(!equal)
		{
		MessagePopup ("EMC-Error","EMC is not correct!!");
		emc_counter = -10;
		}
	 	
return(emc_counter);
}

void write_reference_date(void)
{
char cActualDate[11];
char cInitOffset[5] = {0};
char *ptemp,*error;

//MM-DD-YYYY
 	ptemp = DateStr ();
	cActualDate[0] =	  	0x01;	  //Subaddrese EEP
	cActualDate[1] =	  	0x00;	  //Subaddrese EEP  
 	cActualDate[2] = 	 ptemp[6];
 	cActualDate[3] = 	 ptemp[7];
 	cActualDate[4] = 	 ptemp[8];
 	cActualDate[5] = 	 ptemp[9];
 	cActualDate[6] = 	 ptemp[0];
 	cActualDate[7] = 	 ptemp[1];
 	cActualDate[8] = 	 ptemp[3];
 	cActualDate[9] = 	 ptemp[4];
	cActualDate[10] = 	 '\0'	 ;
//	protest_write_i2c (0x10, 0, 0xa6,cActualDate, 10, 0);

}
void check_reference(void)
{
char cRefereceDate[10];
char cActualDate[10];
double dRefereceDate;
double dActualDate;
char cInitOffset[5] = {0};
char *ptemp,*error;
double help;

	cInitOffset[0] =	  	0x01;	  //Subaddrese EEP
	cInitOffset[1] =	  	0x00;	  //Subaddrese EEP  

//	protest_write_i2c (0x10, 0, 0xa6,cInitOffset, 2, 0);
//	protest_read_i2c (0x10, 0, 0xa6,cRefereceDate , 8);
	cRefereceDate[8] = 	 '\0'	 ;

//MM-DD-YYYY
 	ptemp = DateStr ();
 	cActualDate[0] = 	 ptemp[6];
 	cActualDate[1] = 	 ptemp[7];
 	cActualDate[2] = 	 ptemp[8];
 	cActualDate[3] = 	 ptemp[9];
 	cActualDate[4] = 	 ptemp[0];
 	cActualDate[5] = 	 ptemp[1];
 	cActualDate[6] = 	 ptemp[3];
 	cActualDate[7] = 	 ptemp[4];
	cActualDate[8] = 	 '\0'	 ;
	dActualDate = strtod(cActualDate,&error);
	dRefereceDate = strtod(cRefereceDate,&error);
/*
	if(dActualDate != dRefereceDate)		// check of 24 houers
		{
		SetCtrlVal (Global_imainpanel, PANEL1_ONLYMEASUREMSG, structSprache.Bitte_Reference_pruefen);	
		SetPanelAttribute (Global_imainpanel, ATTR_BACKCOLOR, VAL_DK_RED);
		ProcessDrawEvents();
		ProcessSystemEvents();
		}
*/
}


int check_double_connect(unsigned char* pruefstatus,int mode)
{
// mode = 0 -> aufruf aus Funktionsprüfung
// mode = 1 -> aufruf aus Funktionsprüfung im Referencetest
// mode = 2 -> aufruf aus Finalprüfung
// mode = 3 -> aufruf aus Finalprüfung im Referencetest

//Bitkey for byte
//PASS FKT   	00010000  	0x10
//PASS FINAL   	00110000	0x30
//REFERENCE		11001100	0x66
//CONNECT COUNT 00000011	0x00-0x03

 if( (*pruefstatus == 0x01) || (*pruefstatus == 0x02) || (*pruefstatus == 0x03) || (*pruefstatus == 0x10) || 
 	 (*pruefstatus == 0x11) || (*pruefstatus == 0x12) || (*pruefstatus == 0x13) || (*pruefstatus == 0x30) ||
 	 (*pruefstatus == 0x66))
 	 {
	switch(mode)
		{
		case 0:	if((*pruefstatus == 0x10))
					{
					*pruefstatus = 0x1;
					}
				if((*pruefstatus & 0x66) == 0x66 ) 
					{
					MessagePopup (" DUT is REFERENCE ", "Referenceboard could not run in normal Test !!!");
					return(1);
					}
				if((globals->iloginlevel == 4) || (globals->iloginlevel == 5) || (globals->iloginlevel == 1))
					{
					*pruefstatus = 2;
					return(0);
					}
				if(*pruefstatus < Conect_counter)
					{
					*pruefstatus = *pruefstatus+1;
					return(0);
					}
				else
					{
					MessagePopup (" doubletest ", "it is not allowed to test this DUT once more"); 
					return(1);
					}

				break;
				
		case 1:	if((*pruefstatus & 0x66) == 0x66 ) 
					{
					return(0);
					}
					
				break;
				
		case 2:	if((*pruefstatus == 0x30))
					{
					*pruefstatus = 0x10;
					return(0);
					}
				if((*pruefstatus & 0x66) == 0x66 ) 
					{
					MessagePopup (" DUT is REFERENCE ", "Referenceboard could not run in normal Test !!!");
					return(1);
					}
				if((*pruefstatus & 0x10) != 0x10)
					{
					MessagePopup ("DUT-Error", "DUT not passed in FunctionalTest");
					return(1);
					}
				if((globals->iloginlevel == 4) || (globals->iloginlevel == 5) || (globals->iloginlevel == 1))
					{
					*pruefstatus = (Conect_counter+0x10);
					return(0);
					}
				if((*pruefstatus  < (Conect_counter+0x10)))
					{
					*pruefstatus++;
					return(0);
					}
				else
					{
					MessagePopup (" doubletest ", "it is not allowed to test this DUT once more"); 
					return(1);
					}
					
				break;
				
		case 3:	if((*pruefstatus & 0x66) == 0x66 ) 
					{
					return(0);
					}
					
				break;

		default : MessagePopup ("\n ERROR","Invalid call to the\n check_double_connect\n funktion ");
				  return(0);
		
				break;
		}
	}
else
	{
	*pruefstatus = 0x01;
	}
	
return(0);		

}





void cmd60_settings_for_connect(void)
{
int proTestID;
#ifdef  CMD60_SETTINGS_DECT

	proTestID = (globals->system_id[7]-0x30 )*100;
	proTestID += (globals->system_id[8]-0x30)*10;
	proTestID += (globals->system_id[9] -0x30);
	switch (proTestID)
		{
		case proTest001:  
						 	iDummyslot			=	0;
 							iDummycarrier		=	9;
						 	iTrafficslot		=	10;  // alt 9
 							iTrafficcarrier		=	6;
							RFPI_hex[0] 		= 0x00;
							RFPI_hex[1] 		= 0x34;
							RFPI_hex[2] 		= 0x56;
							RFPI_hex[3] 		= 0x78;
							RFPI_hex[4] 		= 0x91;
							break;
		case proTest002:  
						 	iDummyslot			=	2; //alt 1
 							iDummycarrier		=	8;
						 	iTrafficslot		=	10;
							iTrafficcarrier		=	5;  
							RFPI_hex[0] 		= 0x00;
							RFPI_hex[1] 		= 0x35;
							RFPI_hex[2] 		= 0x57;
							RFPI_hex[3] 		= 0x79;
							RFPI_hex[4] 		= 0x92;
							break;
		case proTest003:  
						 	iDummyslot			=	2;
 							iDummycarrier		=	8;
						 	iTrafficslot		=	10; // alt 11
 							iTrafficcarrier		=	4;
							RFPI_hex[0] 		= 0x00;
							RFPI_hex[1] 		= 0x36;
							RFPI_hex[2] 		= 0x58;
							RFPI_hex[3] 		= 0x80;
							RFPI_hex[4] 		= 0x93;
							break;
		case proTest004:  
						 	// alt iDummyslot			=	3;
						 	iDummyslot			=	4;  // alt 3
 							iDummycarrier		=	6;
						 	iTrafficslot		=	0;
 							iTrafficcarrier		=	4;
							RFPI_hex[0] 		= 0x00;
							RFPI_hex[1] 		= 0x37;
							RFPI_hex[2] 		= 0x59;
							RFPI_hex[3] 		= 0x81;
							RFPI_hex[4] 		= 0x94;
							break;
		case proTest005:  
						 	iDummyslot			=	4;
 							iDummycarrier		=	5;
						 	iTrafficslot		=	2; // alt1  
 							iTrafficcarrier		=	2;
							RFPI_hex[0] 		= 0x00;
							RFPI_hex[1] 		= 0x38;
							RFPI_hex[2] 		= 0x60;
							RFPI_hex[3] 		= 0x82;
							RFPI_hex[4] 		= 0x95;
							break;
		case proTest006:  
	           			 	iDummyslot			=	6;   // alt 5
 							iDummycarrier		=	4;
						 	iTrafficslot		=	2;
 							iTrafficcarrier		=	1;
							RFPI_hex[0] 		= 0x00;
							RFPI_hex[1] 		= 0x39;
							RFPI_hex[2] 		= 0x61;
							RFPI_hex[3] 		= 0x83;
							RFPI_hex[4] 		= 0x96;
							break;
		case proTest007:
						 	iDummyslot			=	6;
 							iDummycarrier		=	3;
						 	iTrafficslot		=	4;  // alt 3
 							iTrafficcarrier		=	0;
							RFPI_hex[0] 		= 0x00;
							RFPI_hex[1] 		= 0x40;
							RFPI_hex[2] 		= 0x62;
							RFPI_hex[3] 		= 0x84;
							RFPI_hex[4] 		= 0x97;
							break;
		case proTest008:
						 	iDummyslot			=	8;  // alt 7
 							iDummycarrier		=	2;
						 	iTrafficslot		=	4;
 							iTrafficcarrier		=	9;
							RFPI_hex[0] 		= 0x00;
							RFPI_hex[1] 		= 0x41;
							RFPI_hex[2] 		= 0x63;
							RFPI_hex[3] 		= 0x85;
							RFPI_hex[4] 		= 0x98;
							break;
		case proTest009:
						 	iDummyslot			=	8;
 							iDummycarrier		=	1;
						 	iTrafficslot		=	6;  // alt 7
 							iTrafficcarrier		=	8;
							RFPI_hex[0] 		= 0x00;
							RFPI_hex[1] 		= 0x42;
							RFPI_hex[2] 		= 0x64;
							RFPI_hex[3] 		= 0x86;
							RFPI_hex[4] 		= 0x99;
							break;
		case proTest010:
						 	iDummyslot			=	10; // alt 9
 							iDummycarrier		=	0;
						 	iTrafficslot		=	6;
 							iTrafficcarrier		=	7;
							RFPI_hex[0] 		= 0x00;
							RFPI_hex[1] 		= 0x43;
							RFPI_hex[2] 		= 0x65;
							RFPI_hex[3] 		= 0x87;
							RFPI_hex[4] 		= 0x03;
							break;
		case proTest011:
						 	iDummyslot			=	10;
 							iDummycarrier		=	9;
						 	iTrafficslot		=	8; // alt 7
 							iTrafficcarrier		=	6;
							RFPI_hex[0] 		= 0x00;
							RFPI_hex[1] 		= 0x44;
							RFPI_hex[2] 		= 0x66;
							RFPI_hex[3] 		= 0x88;
							RFPI_hex[4] 		= 0x04;
							break;
		case proTest012:
						 	iDummyslot			=	10;  // alt 11
 							iDummycarrier		=	8;
						 	iTrafficslot		=	8;
 							iTrafficcarrier		=	5;
							RFPI_hex[0] 		= 0x00;
							RFPI_hex[1] 		= 0x45;
							RFPI_hex[2] 		= 0x67;
							RFPI_hex[3] 		= 0x89;
							RFPI_hex[4] 		= 0x05;
							break;
		case proTest013:
						 	iDummyslot			=	2;
 							iDummycarrier		=	4;
						 	iTrafficslot		=	6;
 							iTrafficcarrier		=	4;
							RFPI_hex[0] 		= 0x00;
							RFPI_hex[1] 		= 0x46;
							RFPI_hex[2] 		= 0x68;
							RFPI_hex[3] 		= 0x90;
							RFPI_hex[4] 		= 0x06;
							break;
		case proTest014:
						 	iDummyslot			=	4;   // alt 3
 							iDummycarrier		=	5;
						 	iTrafficslot		=	8;   // alt 7 
 							iTrafficcarrier		=	5;
							RFPI_hex[0] 		= 0x00;
							RFPI_hex[1] 		= 0x47;
							RFPI_hex[2] 		= 0x69;
							RFPI_hex[3] 		= 0x91;
							RFPI_hex[4] 		= 0x07;
							break;
		case proTest015:
						 	iDummyslot			=	4;
 							iDummycarrier		=	6;
						 	iTrafficslot		=	8;
 							iTrafficcarrier		=	6;
							RFPI_hex[0] 		= 0x00;
							RFPI_hex[1] 		= 0x48;
							RFPI_hex[2] 		= 0x70;
							RFPI_hex[3] 		= 0x92;
							RFPI_hex[4] 		= 0x08;
							break;
		case proTest016:
						 	iDummyslot			=	6;  // alt 5
 							iDummycarrier		=	7;
						 	iTrafficslot		=	10;  // alt 9 
 							iTrafficcarrier		=	7;
							RFPI_hex[0] 		= 0x00;
							RFPI_hex[1] 		= 0x49;
							RFPI_hex[2] 		= 0x71;
							RFPI_hex[3] 		= 0x93;
							RFPI_hex[4] 		= 0x09;
							break;
		case proTest017:
						 	iDummyslot			=	6;
 							iDummycarrier		=	8;
						 	iTrafficslot		=	10;
 							iTrafficcarrier		=	8;
							RFPI_hex[0] 		= 0x00;
							RFPI_hex[1] 		= 0x50;
							RFPI_hex[2] 		= 0x72;
							RFPI_hex[3] 		= 0x94;
							RFPI_hex[4] 		= 0x10;
							break;
		case proTest018:
						 	iDummyslot			=	8;  // alt 7
 							iDummycarrier		=	9;
						 	iTrafficslot		=	10;  // alt 11
 							iTrafficcarrier		=	9;
							RFPI_hex[0] 		= 0x00;
							RFPI_hex[1] 		= 0x51;
							RFPI_hex[2] 		= 0x73;
							RFPI_hex[3] 		= 0x95;
							RFPI_hex[4] 		= 0x11;
							break;
		case proTest019:
						 	iDummyslot			=	10;
 							iDummycarrier		=	8;
						 	iTrafficslot		=	6;
 							iTrafficcarrier		=	4;
							RFPI_hex[0] 		= 0x00;
							RFPI_hex[1] 		= 0x52;
							RFPI_hex[2] 		= 0x74;
							RFPI_hex[3] 		= 0x96;
							RFPI_hex[4] 		= 0x12;
							break;
		case proTest020:
						 	iDummyslot			=	10;  // alt 9
 							iDummycarrier		=	1;
						 	iTrafficslot		=	2;   // alt 1
 							iTrafficcarrier		=	3;
							RFPI_hex[0] 		= 0x00;
							RFPI_hex[1] 		= 0x53;
							RFPI_hex[2] 		= 0x75;
							RFPI_hex[3] 		= 0x97;
							RFPI_hex[4] 		= 0x13;
							break;
		case proTest021:
						 	iDummyslot			=	10;
 							iDummycarrier		=	2;
						 	iTrafficslot		=	2;
 							iTrafficcarrier		=	4;
							RFPI_hex[0] 		= 0x00;
							RFPI_hex[1] 		= 0x54;
							RFPI_hex[2] 		= 0x76;
							RFPI_hex[3] 		= 0x98;
							RFPI_hex[4] 		= 0x14;
							break;
		case proTest022:
						 	iDummyslot			=	10;   // alt 11
 							iDummycarrier		=	3;
						 	iTrafficslot		=	4;    // 3
 							iTrafficcarrier		=	5;
							RFPI_hex[0] 		= 0x00;
							RFPI_hex[1] 		= 0x55;
							RFPI_hex[2] 		= 0x77;
							RFPI_hex[3] 		= 0x99;
							RFPI_hex[4] 		= 0x15;
							break;
		case proTest023:
						 	iDummyslot			=	0;
 							iDummycarrier		=	4;
						 	iTrafficslot		=	4;
 							iTrafficcarrier		=	6;
							RFPI_hex[0] 		= 0x00;
							RFPI_hex[1] 		= 0x56;
							RFPI_hex[2] 		= 0x01;
							RFPI_hex[3] 		= 0x02;
							RFPI_hex[4] 		= 0x16;
							break;
		case proTest024:
						 	iDummyslot			=	2;   // alt 1
 							iDummycarrier		=	5;
						 	iTrafficslot		=	6;   // alt 5
 							iTrafficcarrier		=	7;
							RFPI_hex[0] 		= 0x00;
							RFPI_hex[1] 		= 0x57;
							RFPI_hex[2] 		= 0x02;
							RFPI_hex[3] 		= 0x03;
							RFPI_hex[4] 		= 0x17;
							break;
		case proTest025:
						 	iDummyslot			=	6;
 							iDummycarrier		=	2;
						 	iTrafficslot		=	2;
 							iTrafficcarrier		=	8;
							RFPI_hex[0] 		= 0x00;
							RFPI_hex[1] 		= 0x58;
							RFPI_hex[2] 		= 0x03;
							RFPI_hex[3] 		= 0x04;
							RFPI_hex[4] 		= 0x18;
							break;
		case proTest026:
						 	iDummyslot			=	8;   // alt 7
 							iDummycarrier		=	3;
						 	iTrafficslot		=	4;   // alt 3
 							iTrafficcarrier		=	9;
							RFPI_hex[0] 		= 0x00;
							RFPI_hex[1] 		= 0x59;
							RFPI_hex[2] 		= 0x04;
							RFPI_hex[3] 		= 0x05;
							RFPI_hex[4] 		= 0x19;
							break;
		case proTest027:
						 	iDummyslot			=	8;
 							iDummycarrier		=	4;
						 	iTrafficslot		=	4;
 							iTrafficcarrier		=	0;
							RFPI_hex[0] 		= 0x00;
							RFPI_hex[1] 		= 0x60;
							RFPI_hex[2] 		= 0x05;
							RFPI_hex[3] 		= 0x06;
							RFPI_hex[4] 		= 0x20;
							break;
		case proTest028:
						 	iDummyslot			=	10;    // alt 9
 							iDummycarrier		=	5;
						 	iTrafficslot		=	6;    // alt 5
 							iTrafficcarrier		=	1;
							RFPI_hex[0] 		= 0x00;
							RFPI_hex[1] 		= 0x61;
							RFPI_hex[2] 		= 0x06;
							RFPI_hex[3] 		= 0x07;
							RFPI_hex[4] 		= 0x21;
							break;
		case proTest029:
						 	iDummyslot			=	10;
 							iDummycarrier		=	6;
						 	iTrafficslot		=	6;
 							iTrafficcarrier		=	2;
							RFPI_hex[0] 		= 0x00;
							RFPI_hex[1] 		= 0x62;
							RFPI_hex[2] 		= 0x07;
							RFPI_hex[3] 		= 0x08;
							RFPI_hex[4] 		= 0x22;
							break;
		case proTest030:
						 	iDummyslot			=	10;  // alt 11
 							iDummycarrier		=	7;
						 	iTrafficslot		=	6;   // alt 7
 							iTrafficcarrier		=	3;
							RFPI_hex[0] 		= 0x00;
							RFPI_hex[1] 		= 0x63;
							RFPI_hex[2] 		= 0x08;
							RFPI_hex[3] 		= 0x09;
							RFPI_hex[4] 		= 0x23;
							break;
		case proTest103:
						 	iDummyslot			=	8;  // alt 11
 							iDummycarrier		=	2;
						 	iTrafficslot		=	6;   // alt 7
 							iTrafficcarrier		=	4;
							RFPI_hex[0] 		= 0x00;
							RFPI_hex[1] 		= 0x63;
							RFPI_hex[2] 		= 0x22;
							RFPI_hex[3] 		= 0x09;
							RFPI_hex[4] 		= 0x23;
							break;
		
		case proTest105:
						 	iDummyslot			=	8;  // alt 11
 							iDummycarrier		=	4;
						 	iTrafficslot		=	6;   // alt 7
 							iTrafficcarrier		=	2;
							RFPI_hex[0] 		= 0x00;
							RFPI_hex[1] 		= 0x63;
							RFPI_hex[2] 		= 0x08;
							RFPI_hex[3] 		= 0x09;
							RFPI_hex[4] 		= 0x23;
							break;
		case proTest106:
						 	iDummyslot			=	0;  // alt 11
 							iDummycarrier		=	6;
						 	iTrafficslot		=	4;   // alt 7
 							iTrafficcarrier		=	2;
							RFPI_hex[0] 		= 0x21;
							RFPI_hex[1] 		= 0x83;
							RFPI_hex[2] 		= 0x08;
							RFPI_hex[3] 		= 0x07;
							RFPI_hex[4] 		= 0x53;
							break;
	

		default:
						 	iDummyslot			=	2;
 							iDummycarrier		=	6;
						 	iTrafficslot		=	8;
 							iTrafficcarrier		=	4;
							RFPI_hex[0] 		= 0x00;
							RFPI_hex[1] 		= 0x22;
							RFPI_hex[2] 		= 0x33;
							RFPI_hex[3] 		= 0x44;
							RFPI_hex[4] 		= 0x55;
							break;
		}


#endif

}





 
double hex_str_to_double(unsigned char *buffer, int length)
{
int i,j;
double hexdouble;
char *error;
double tempdouble;
unsigned char helpstr[2];
	
	
	if(!(strlen(buffer)>= length))
		return(-1);
	
	hexdouble = 0;
	for(i = length,j = 0; j < length; i--,j++)
		{
		helpstr[0] = buffer[i];
		helpstr[1] = '\0';		
		tempdouble = strtoul(helpstr,&error,16);
		if(tempdouble != 0)
			{
			tempdouble *= pow(16,j);
			hexdouble += tempdouble;
			}
		}

return(hexdouble);
}


int double_to_hex_str(double wert, int length, unsigned char *hexstr)
{
int i,j,n,k;
double hexdouble;
char *error;
int tempint;
unsigned char helpstr[2];
unsigned char help_hexstr[20] = {0};
	
	
	i=length;
//	help_hexstr[0] = NULL;
	while(wert)
		{
		tempint =fmod(wert,16);
		wert = (wert - tempint)/16;
		sprintf(helpstr,"%x",tempint);
		strcat(help_hexstr,helpstr);
		}
	j = strlen(help_hexstr);
	for(j, n = 0; j<=i;j++,n++)
		hexstr[n] = '0';
	j = strlen(help_hexstr);
	for(n = i-j+1,k=j-1 ; n <=i;n++,k--) 
		hexstr[n] = help_hexstr[k];    
	
	
return(0);
}




#ifndef RFP35_PROJECT	// [@GC16052011]
//
// diese Funktion ist nur fuer die alten RFP32 Modelle
//

int generateDeTeWeSerialNumber(char* strDeTeWeSerialNumber)
{
char 			*Date; 
char			str_din_date[3] = {0};
char			str_din_year[] = {'A','B','C','D','E','F','H','J','K','L','M','N','P','R','S','T','U','V','W','X'};
int				year;
char			temp_str[40];
unsigned char 	detewe_Sern[40] = {0}; 
int 			help_add_int;
int 			help_int;
char			tempstr[80];

//Start [@JM010408]
	if ( isInRepairMode() )
	{
		// get customer serial number from data base
		strcpy(strDeTeWeSerialNumber, structQPDS.strCustSn);
		
		//work done, bye ...
		return(0);	
	}
//Ende [@JM010408]

	Date = DateStr ();


	if(Date[0] == '1')
		{
		if(Date[1] == '0')
			str_din_date[1] = 'O';	//Oktober		
		if(Date[1] == '1')
			str_din_date[1] = 'N';	//November		
		if(Date[1] == '2')
			str_din_date[1] = 'D'; //Dezember
		}
	
	else
		str_din_date[1] = Date[1];			
		
	year = (int)atoi(&Date[6]);
	year = year - 1990;		 //realy year
	while(year >= 20)
		year -= 20;
	str_din_date[0] = str_din_year[year];			
	
	

	sprintf(temp_str,"%06d",serial_FP);
	
	detewe_Sern[0] = str_din_date[0];				
	detewe_Sern[1] = str_din_date[1];
	detewe_Sern[2] = 0x20;
	detewe_Sern[3] = cHerstellerCode;
	detewe_Sern[4] = 0x30; 
	detewe_Sern[5] = 0x30;
	detewe_Sern[6] = temp_str[0]; 
	detewe_Sern[7] = temp_str[1];
	detewe_Sern[8] = temp_str[2]; 
	detewe_Sern[9] = temp_str[3]; 
	detewe_Sern[10] = temp_str[4];
	detewe_Sern[11] = temp_str[5];
	
	help_add_int	 = 		        (   detewe_Sern[0]*1
									+ 	detewe_Sern[1]*2 
									+   detewe_Sern[3]*3
									+   detewe_Sern[4]*4 
									+   detewe_Sern[5]*5 
									+   detewe_Sern[6]*6 
									+   detewe_Sern[7]*7 
									+   detewe_Sern[8]*8 
									+   detewe_Sern[9]*9 
									+   detewe_Sern[10]*10 
									+   detewe_Sern[11]*11);
								
	help_int = help_add_int/16;	
	help_int *= 16;
	help_int = help_int - help_add_int;
	if(help_int<0)
		help_int *=-1;


	sprintf(tempstr,"%c%c %c00%c%c%c%c%c%c%X",  str_din_date[0],
										    	str_din_date[1],
												cHerstellerCode,
												temp_str[0],
												temp_str[1],
												temp_str[2],
												temp_str[3],
												temp_str[4],
												temp_str[5],
												(help_int&0xf));
												

	sprintf(strDeTeWeSerialNumber,tempstr);
	

	
	
return(0);	
}

#endif //RFP35_PROJECT	// [@GC16052011]


int WriteQPDS(int iPrufstatus)
{
#ifdef USE_QPDS
FILE *DateiHandel; 
char temp_str[MAXPATHNAMELEN];

	//MessagePopup ("QPDS", "Write");


	sprintf(temp_str,"%s%s%s",structQPDS.strPathQpds,structQPDS.strStartZeit,structQPDS.strHostName);
	DateiHandel = fopen(temp_str,"a");
	if(DateiHandel)
		{
		switch (iPrufstatus)
			{
			case 1:				//FAIL
				{
				
				fprintf(DateiHandel,"{@BATCH|%s|%s|%s||KDSNR:%s|%s||%s|%s|||\n",	structQPDS.strBgName,
																			structQPDS.strBgName,
																			structQPDS.strAdapterNr,
																			structQPDS.strCustSn,
																			structQPDS.strPruefstufe,
																			structQPDS.strWerkfachgruppe,
																			structQPDS.strHostName);
																			
				fprintf(DateiHandel,"{@BTEST|%s|01|%s|||||n|n|%s||1|\n",		structQPDS.strExID,
																			structQPDS.strStartZeit,
																			structQPDS.strEndZeit);
																			
				fprintf(DateiHandel,"{@RPT|Faild}\n");
				fprintf(DateiHandel,"{@RPT|%s}\n",structQPDS.strFehlerInfo1);
				fprintf(DateiHandel,"{@RPT|%s}\n",structQPDS.strFehlerInfo2);

				fprintf(DateiHandel,"}\n");
				fprintf(DateiHandel,"}\n");
										

				break;
				}
			case 0:				//PASS
				{
				fprintf(DateiHandel,"{@BATCH|%s|%s|%s||KDSNR:%s|%s||%s|%s|||\n",	structQPDS.strBgName,
																			structQPDS.strBgName,
																			structQPDS.strAdapterNr,
																			structQPDS.strCustSn,
																			structQPDS.strPruefstufe,
																			structQPDS.strWerkfachgruppe,
																			structQPDS.strHostName);
																			
				fprintf(DateiHandel,"{@BTEST|%s|00|%s|||||n|n|%s||1|\n",		structQPDS.strExID,
																			structQPDS.strStartZeit,
																			structQPDS.strEndZeit);
																			
				fprintf(DateiHandel,"{@RPT|Passed}\n");
				fprintf(DateiHandel,"}\n");
				fprintf(DateiHandel,"}\n");
										
				break;
				}
			}
		

		 fclose(DateiHandel);
		}
	else
		{
		MessagePopup ("QPDS", "Datei für QPDS System kann nicht erstellt werden!!!!");
		}
#endif		
return(0);
}




/**************************************************************************************************************************************
**  set_printermode_db()
**
** Start    [@JM03032008]
** Modified [@GC09122012]
*/
void set_printermode_db(char *cLabelName)
{
	char *p;
	char cTmpStr[255]; 

	//copy to temp-string
	strcpy(cTmpStr,cLabelName);
	
	// convert to upper char
	for ( p=cTmpStr ; *p != '\0' ; p++ ) *p = toupper(*p);
	
	//set flag to default=0 -> no db-based Label
	iPrinterModeDb = 0;			// the oldest version    for RFP22 and RFP32
								// with C:\Programme\BarTender\bartend.exe
	

	//compare extension with expected extension ".ini" (= ini-based Label)
	if ( strcmp( cTmpStr+strlen(cTmpStr)-strlen(".INI") , ".INI" ) == 0 )
	{	
		iPrinterModeDb = 1;     // the next generation with ini Files   for RFP22, RFP32 and RFP35 
								// with c:\test\lab_prt\lab_prt.exe
	}	
	
	//compare with "true" value        (= db-based Label)
	else if ( strcmp( cTmpStr , "TRUE" ) == 0 )
	{	
		iPrinterModeDb = 2;     // the newest generation with DataBased Label   for RFP35
								// with "rsh pdbux040 -ldbhp ./print_label
	}	
	
	
	return;
}


int get_printermode_db(void)
{
	//return flag:	flase 0 -> no db-based Label
	//				true  1 -> ini-based Label
	//				true  2 -> database-based Label    @GC 09.12.2012
	return (iPrinterModeDb);
}





#ifndef RFP35_PROJECT	// [@GC16052011]
//
// diese Funktion ist nur fuer die alten RFP32 Modelle
//

int get_avaja_ser(char *SerialNrAvaja)
{
char *pDate; 
char *ptemp;
int week;
FILE *f_file;
char read_buffer[1000] = {0};
char *pString1;
char *pString2;
int iSerialNr;
int iWeek;
char tempSer[10];
char week_temp[5];
struct 	struct_file  file_group; 
char AvayaProductNumber_temp[20];
char tempstr[250];
//***********************************
//Serialnumber for Avaya
//MM-DD-YYYY
#ifdef AVAYA_SER
//Start [@JM010408]
	if ( isInRepairMode() )
	{
		// get customer serial number from data container
		if(iStartAvaya == 1)
			strcpy(SerialNrAvaja, gl_cAvayaSerNum);
		else
			strcpy(SerialNrAvaja, "0");
		
		//work done, bye ...
		return(0);	
	}
//Ende [@JM010408]

	if(iStartAvaya == 1)
		{

		pDate = DateStr ();  
		SerialNrAvaja[0]   	= pDate[8];   //JAHR
		SerialNrAvaja[1]   	= pDate[9];   //JAHR 
		SerialNrAvaja[2]   	=  0x44;	  //D
		SerialNrAvaja[3]   	=  0x57;	  //W
		week = atoi((pDate+3));
		week += (atof((pDate+0))-1) * 30.5;
		week /= 7;
//Avaya_ser.txt																						   
		
		f_file = fopen(strAvayaPath,"r+");
		if(f_file == 0)
			{
			sprintf(tempstr,"Dateifehler : %s",strAvayaPath);
			
			MessagePopup ("Pfad: ", strAvayaPath);     // JJ 25.03.10
			
			MessagePopup ("Avaya Seriennummer",tempstr);
			return(-1); 
			}
		
		sprintf(AvayaProductNumber_temp,"[%s]", AvayaProductNumber);

		file_get_group(&file_group,f_file,AvayaProductNumber_temp);
		
   		file_getpara(&file_group,"SERIENNUMER", &iSerialNr, PARA_INTEGER); 
   		file_getpara(&file_group,"KW", &iWeek, PARA_INTEGER); 

   		if(iWeek != week)
   			 iSerialNr =1;
   		else
   			 iSerialNr++;
   			 
   			 
 		rewind (f_file);//set file-pointer to position 0
		fflush (f_file);
		fprintf(f_file,"[%s]\n",AvayaProductNumber); 
		fprintf(f_file,"SERIENNUMER: = %d\n",iSerialNr);
		fprintf(f_file,"KW: = %d\n", week);
		fprintf(f_file,"[end_group]\n"); 
		fclose(f_file);
		
		sprintf(tempSer,"%05d", iSerialNr);

		sprintf(week_temp,"%02d",week);
		SerialNrAvaja[4]   	=  week_temp[0]; 
		SerialNrAvaja[5]   	=  week_temp[1]; 
		SerialNrAvaja[6]   	=  iHerstellerkennung; 
		SerialNrAvaja[7]   	=  tempSer[0]; 
		SerialNrAvaja[8]   	=  tempSer[1]; 
		SerialNrAvaja[9]   	=  tempSer[2]; 
		SerialNrAvaja[10]   =  tempSer[3]; 
		SerialNrAvaja[11]   =  tempSer[4]; 
		}
	else
		{
		SerialNrAvaja[0]  = 0x30;
		SerialNrAvaja[1]  = 0; 
		}
	

#else
		SerialNrAvaja[0]  = 0x30;
		SerialNrAvaja[1]  = 0; 
#endif

return(0);
}

#endif //RFP35_PROJECT	// [@GC16052011]





#ifndef RFP35_PROJECT
//
// diese Funktion ist nur fuer die alten RFP32 Modelle
//

//int read_label_dat(struct struct_LabelDat *strLabelDat)
int read_label_dat(void)
{

	FILE 		*fa_w;
	char		cTmpStr[160];
	char		cLabelStr[260];
	char		*pcLabelData;
	long int	iIndex;
	int			iLen;
	int			i, j;

#ifdef PRINT_LABEL
	//open Label.dat and read first line (Label-Information)
//Start [@JM010408]
	//fa_w = fopen("LabelDb.txt","r");
	fa_w = fopen(label_datei,"r");
//End [@JM010408]
	if(fa_w)	
		{
			fgets(cLabelStr, 160, fa_w);
			fclose(fa_w);
		}
	else return(1);
	
	//separate single information
	iIndex = 0;
	iLen = strlen(cLabelStr);
	j = 0;
	for (i=0; i<iLen; i++)
	{
		//too much parameter
		if (iIndex >= MAX_LABEL_DAT_INDEX)
			return(1);
		
		//new data found
		if(cLabelStr[i] == ',')
		{
			strLabelDat.cLabelDat[iIndex++][j] = 0;
			j = 0;
			continue;
		}
		//copy single information to result array + increment index
		strLabelDat.cLabelDat[iIndex][j++] = cLabelStr[i];
	}
		
#endif	
	
		
	return(0);
}

#endif //ifndef RFP35_PROJECT


int DateSelect(int iSelect, char *cReturn)
{
	int		iKW=0;
	int		iDay=0;
	int		iYear=0;
	int		iMonth=0;
	int		iCwYear=0;
	
	int		iSeparator;
	char	cDate[11];
	char	cDay[3];
	char	cMonth[3];
	char	cFullYear[5];
	char	cHalfYear[3];
	char	cCwFullYear[5];
	char	cCwHalfYear[3];
	char	cSeparator[3];
	char	*pcDate;
	
	pcDate = &cDate[0];
	pcDate = DateStr ();
	//pcDate MM-DD-YYYY
	//index  0123456789
	strcpy(cFullYear, &pcDate[6]);	
	pcDate[5] = '\0';
	strcpy(cDay, &pcDate[3]);	
	pcDate[2] = '\0';
	strcpy(cMonth, pcDate);	
	
	//YearFull YYYY
	//index    0123
	strcpy(cHalfYear, &cFullYear[2]);
	
	//demask separator
	iSeparator = iSelect & 0xff00;
	
	//
	switch (iSeparator)
	{
		case 0x0100:
			strcpy(cSeparator, "-");
			break;
		case 0x0200:
			strcpy(cSeparator, "/");
			break;
		case 0x0300:
			strcpy(cSeparator, " ");
			break;
		case 0x0400:
			strcpy(cSeparator, "");
			break;
		case 0x0500:
			strcpy(cSeparator, "\\");
			break;
		case 0x0600:
			strcpy(cSeparator, ":");
			break;
		case 0x0700:
			strcpy(cSeparator, ".");
			break;
		default:
			iSelect = 0xffff;
	}
	

	//get all CW infos
	GetSystemDate (&iMonth, &iDay, &iYear);
	iKW=GetCalWeek(iDay, iMonth, iYear, &iCwYear);
	sprintf(cCwFullYear, "%d", iCwYear);
	strcpy(cCwHalfYear, &cCwFullYear[2]);

	//demask separator
	iSelect &= 0x00ff;
	switch (iSelect)
	{
		case 0x0001: //DD<>MM<>YYYY
			sprintf(cReturn, "%s%s%s%s%s", cDay, cSeparator, cMonth, cSeparator, cFullYear);
			break;
		case 0x0002: //DD<>MM<>YY
			sprintf(cReturn, "%s%s%s%s%s", cDay, cSeparator, cMonth, cSeparator, cHalfYear);
			break;
		case 0x0003: //MM<>DD<>YYYY
			sprintf(cReturn, "%s%s%s%s%s", cMonth, cSeparator, cDay, cSeparator, cFullYear);
			break;
		case 0x0004: //MM<>DD<>YY
			sprintf(cReturn, "%s%s%s%s%s", cMonth, cSeparator, cDay, cSeparator, cHalfYear);
			break;
		case 0x0005: //YYYY<>MM<>DD
			sprintf(cReturn, "%s%s%s%s%s", cFullYear, cSeparator, cMonth, cSeparator, cDay);
			break;
		case 0x0006: //YY<>MM<>DD
			sprintf(cReturn, "%s%s%s%s%s", cHalfYear, cSeparator, cMonth, cSeparator, cDay);
			break;
		case 0x0007: //YYYY<>DD<>MM
			sprintf(cReturn, "%s%s%s%s%s", cFullYear, cSeparator, cDay, cSeparator, cMonth);
			break;
		case 0x0008: //YY<>DD<>MM
			sprintf(cReturn, "%s%s%s%s%s", cHalfYear, cSeparator, cDay, cSeparator, cMonth);
			break;
		case 0x0009: //MM<>YYYY
			sprintf(cReturn, "%s%s%s", cMonth, cSeparator, cCwFullYear);
			break;
		case 0x000a: //MM<>YY
			sprintf(cReturn, "%s%s%s", cMonth, cSeparator, cCwHalfYear);
			break;
		case 0x000b: //KWxx<>YYYY
			sprintf(cReturn, "KW%d%s%s", iKW, cSeparator, cCwFullYear);
			break;
		case 0x000c: //KWxx<>YY
			sprintf(cReturn, "KW%d%s%s", iKW, cSeparator, cCwHalfYear);
			break;
		case 0x000d: //CWxx<>YYYY
			sprintf(cReturn, "CW%d%s%s", iKW, cSeparator, cCwFullYear);
			break;
		case 0x000e: //CWxx<>YY
			sprintf(cReturn, "CW%d%s%s", iKW, cSeparator, cCwHalfYear);
			break;
		default:
			sprintf(cReturn, DateStr());
			break;
	}
	
	
	return (0);
}

int GetCalWeek(int iDay, int iMonth, int iYear, int *iKwYear)
{
	int		i;
	int		iKW;
	int		iDayInYear=0;
	int 	iDayIndex=0;
	int 	iSchaltjahre=1;
	
	enum eDays { Mo=1, Di, Mi, Do, Fr, Sa, So};
	
	//count days in iYear
	for (i=1; i<iMonth; i++)
	{
		if (	(i == 1) ||
				(i == 3) ||
				(i == 5) ||
				(i == 7) ||
				(i == 8) ||
				(i == 10)
			)
		{
			iDayInYear += 31;
		}
		else if	(i == 2)
		{
			iDayInYear += 28;
			//Schaltjahr
			if( (iYear%4 == 0 && iYear%100 != 0) || iYear%400 == 0)
				iDayInYear++;
		}
		else
		{
			iDayInYear += 30;
		}
		
	}
	
	//add days of current month
	iDayInYear += iDay;
	
	//calculate expected Day based on Mon., 01.01.1900
	//in seven years monday again (without Schaltjahr)
	iDayIndex += (iYear-1900)%7;
	
	//add Schaltjahre but not this (it's in //count days in iYear)
	iDayIndex += (iYear-1-1900)/4;
	
	//Day at 01. Jan iYear
	iDayIndex %= 7;
	iDayIndex++; //index=0 -> Sunday, index7
	
	//First CW if 01.Jan was Mo-Do, for 01.Jan was Fr-So ->01.jan in last CW  last year
	if (iDayIndex < Fr)	iDayInYear += iDayIndex-1;
	else				iDayInYear -= So-iDayIndex+1;
	
	//01.Jan was Fr-So ->01.jan is in last CW  last year
	if (iDayInYear<1)
	{
		iKW = GetCalWeek(31, 12, iYear-1, iKwYear);
	}
	else
	{
		//Calculate CW
		iKW = 1 + (iDayInYear-1)/7;
		*iKwYear = iYear;
	
		//check if kw1 next year
		if ((iKW == 53) && (iDay>28))
		{
			iDayIndex += iDayInYear-1;
			iDayIndex %= 7;
			if (iDayIndex<4)
			{
				iKW = 1;
				*iKwYear = iYear+1;
			}
		}
	}

	return(iKW);
}
//Ende [@JM030308]



/*
**    systemCall(....)
**
**    same as system() runs only Hide or Normal
**
*/

int SystemCall(char *cCmd,int hide)
{

	int handle;
	int ret;
	int timeOut;
	char cInfo[512];
	

	//LE_SHOWNORMAL
	//LE_HIDE
	if ( (ret=LaunchExecutableEx (cCmd, hide ,&handle)) )
	{
		RetireExecutableHandle (handle);
#ifdef RFP35_DEBUG_MESSAGE_POPUP	
		sprintf(cInfo,"can't execute: %s",cCmd);
		MessagePopup("Error",cInfo);
#endif		
		return(ret);     // error
	}		

	//warten bis exe zurück kommt
	//timeOut 45 Sec
	timeOut = 45 * (1000/250); 
	while( (ExecutableHasTerminated(handle) == 0)  && (timeOut > 0) )
	{
#ifdef _WINDOWS_
		Sleep(250);      // 250ms   the better way
#else		
		Delay(0.25);     // 250ms
#endif
		timeOut--;
	}		
	
	// noch nicht beendet
	if( ExecutableHasTerminated(handle) == 0 ) 
	{
		// nur die cmd.exe kann gekillt werden, die xxx.exe laeuft weiter
		//fprintf(stderr,"kill\n");
		TerminateExecutable(handle);
		// handle freigeben
		RetireExecutableHandle (handle);
#ifdef RFP35_DEBUG_MESSAGE_POPUP	
		sprintf(cInfo,"can't execute: %s: timeout",cCmd);
		MessagePopup("Error",cInfo);
#endif		
		return(-1);    // error
	}
	RetireExecutableHandle (handle);   // handle freigeben


	return(0);   //ok
	
}	






int DisableTestfunktionIfValid(struct struct_test *act_structptr)
{
	char	cErrMsg[254];
	char	cDisabledDevices[512];
	
	FILE	*fa_r;

	struct struct_file_LongLines	file_group;
	
	//no repairmode -> always enabled
	if ( !isInRepairMode() )	return (0);

	//check validation of tag + reset [@JM090423]
	cDisabledDevices[0] = '\0';
	if (strlen(act_structptr->string[3]) == 0 || act_structptr->string[3] == NULL)
	{
		return(0);
	}
	
//changed [@JM090304] start
	// open sequence-file
	fa_r = fopen(globals->sequ_select->sequ_pfad,"r");
	if(!fa_r)
	{
		sprintf(cErrMsg, "Fehler Dateiöffnen in %s bei DisableTestfunktionIfValid()!\nTEST ABBRECHEN!" , globals->sequ_select->structpointer[globals->sequ_select->act_index]->username);
		MessagePopup(globals->sequ_select->sequ_pfad, cErrMsg);
		return(0);
	}
	
	//return 0: group not found -> test enabled
	if ( !file_get_group_LongLines(&file_group, fa_r, "[Disabled]") )
	{
		fclose(fa_r);
		return(0);
	}

	//return 1: entry not found -> test enabled
   	if ( file_getpara_LongLines(&file_group, act_structptr->string[3], cDisabledDevices, PARA_STRING) )
	{
		fclose(fa_r);
		return(0);
	}
	
	// close sequence-file
	fclose(fa_r);
#ifdef USE_QPDS	
	//name of device is not in disable list -> test enabled
	if (strstr(cDisabledDevices, structQPDS.strBgName) == NULL) return (0); 
#endif
	//if (strstr(act_structptr->string[3], structQPDS.strBgName) == NULL) return (0);
//changed [@JM090304] end

	// set unit to "disabled" for report
	sprintf(globals->sequ_select->structpointer[globals->sequ_select->act_index]->unit, "disabled");

	//set all to zero for report
	globals->sequ_select->structpointer[globals->sequ_select->act_index]->resultoffset = 0;
	globals->sequ_select->structpointer[globals->sequ_select->act_index]->prec = 0;
	globals->sequ_select->structpointer[globals->sequ_select->act_index]->upperlimit = 0;
	globals->sequ_select->structpointer[globals->sequ_select->act_index]->lowerlimit = 0;
	
	//write result
	act_structptr->resultstatus = check_value(0);
	
	//tell calling function test is disabled and result is written soon
	return(1);
}







#ifndef RFP35_PROJECT	  //@GC 05.09.2011  
//
// diese Funktion ist nur fuer die alten RFP32 Modelle
//


int GetRepairInfoFromDb(void)
/*
get some infos from data base

if 
	acces possible, 
else
	return(!0); //testfunction must quit with run-time-error
*/
{
#define		REPAIR_INFO_FILE "RepairInfo.txt"	
#define		REPAIR_INFO_BAT  "GetInfoByFlexId.bat"	
	int		iReturn = 0;

	char	tempstr[255] = {0};
	char	cUpper;
	
	FILE	*fa_r = NULL;


	
	//delete info file
	remove(REPAIR_INFO_FILE);
	
	//reset global parameter
	structQPDS.strBgName[0] = '\0';
	structQPDS.strCustSn[0] = '\0';
	gl_cMacAddress[0] = '\0';
	gl_cAvayaSerNum[0] = '\0';
	
	//2. info <-> FlexID; set format:
	// -n: different lines
	// -s: customer serial number
	// -p: flex delivery unit
	// -m: first MAC-address
	// -a?: avaya serial number
	//sprintf(tempstr,"rsh ueun47 -l dbhp bin/get_info -e %s -fnspm > %s", structQPDS.strExID, REPAIR_INFO_FILE);
	sprintf(tempstr,"%s %s %s",REPAIR_INFO_BAT, structQPDS.strExID, REPAIR_INFO_FILE);
	system(tempstr);

	fa_r = fopen(REPAIR_INFO_FILE,"r");
	if(fa_r)
	{
		fgets(structQPDS.strCustSn, 20, fa_r);
		if (structQPDS.strCustSn[strlen(structQPDS.strCustSn)-1] == '\n') structQPDS.strCustSn[strlen(structQPDS.strCustSn)-1] = '\0';
		//change lower- to upper-case character [@JM160708]
		cUpper = toupper(structQPDS.strCustSn[strlen(structQPDS.strCustSn)-1]);
		structQPDS.strCustSn[strlen(structQPDS.strCustSn)-1] = cUpper;
		fgets(structQPDS.strBgName, 20, fa_r);
		if (structQPDS.strBgName[strlen(structQPDS.strBgName)-1] == '\n') structQPDS.strBgName[strlen(structQPDS.strBgName)-1] = '\0';
		fgets(gl_cMacAddress, 20, fa_r); 
		if (gl_cMacAddress[strlen(gl_cMacAddress)-1] == '\n') gl_cMacAddress[strlen(gl_cMacAddress)-1] = '\0';
		//disable avaya [@JM200508]
		//fgets(gl_cAvayaSerNum, 20, fa_r); 
		//if (gl_cAvayaSerNum[strlen(gl_cAvayaSerNum)-1] == '\n') gl_cAvayaSerNum[strlen(gl_cAvayaSerNum)-1] = '\0';
		fclose(fa_r);
		remove(REPAIR_INFO_FILE);
		
		if (strlen(structQPDS.strCustSn) < 2)
		{
			sprintf(tempstr, "Wrong customer serial number: %s", structQPDS.strCustSn);
			MessagePopup("", tempstr);
			return (-1);
		}
	}
	else
	{
		sprintf(tempstr, "Unable to open %s", REPAIR_INFO_FILE);
		MessagePopup("", tempstr);
		return (-1);
	}

	return (0);
}
//Ende [@JM010408]

#endif  //RFP35_PROJECT    //@GC 05.09.2011






#ifndef RFP35_PROJECT	//[@GC05052011]
//
// diese Funktion ist nur fuer die alten RFP32 Modelle
//

//Start [@JM220408]
int BackupEepromDataToFile(void)
{
	int		iReturn = 0;
	int		i;

	char	cPath[255] = {0};
	char	cTmpStr[255] = {0};
	
	FILE	*fa_w = NULL;

	sprintf(cPath, "%s\\%s.backup", BACKUP_EEPROM_DATA_PATH, structQPDS.strExID);
	fa_w = fopen(cPath,"w");
	if(fa_w)
	{
		fputs("[EEPROM_DATA]\n", fa_w);
		for(i=0; i<32; i++)
		{
			sprintf(cTmpStr, "%d = %d\n", i, gl_ucProdData[i]);
			fputs(cTmpStr, fa_w);
		}
		for(i=0; i<2; i++)
		{
			sprintf(cTmpStr, "%d = %d\n", i+EEPROM_MODULATION_DEVIATION0, gl_ucDeviatData[i]);
			fputs(cTmpStr, fa_w);
		}
		for(i=0; i<4; i++)
		{
			sprintf(cTmpStr, "%d = %d\n", i+EEPROM_RSSI_COMP_OFFSET0, gl_ucRssiData[i]);
			fputs(cTmpStr, fa_w);
		}
		for(i=0; i<10; i++)
		{
			sprintf(cTmpStr, "%d = %d\n", i+0x03f0, gl_ucBarcodeData[i]);
			fputs(cTmpStr, fa_w);
		}
		fputs("[end group]\n\n", fa_w);

		fputs("[FUNCTION_DATA]\n", fa_w);
		
		sprintf(cTmpStr, "ClockData = %d\n", gl_ucClockData[0]);
		fputs(cTmpStr, fa_w);

		sprintf(cTmpStr, "BandgapData = %d\n", gl_ucBandgapData[0]);
		fputs(cTmpStr, fa_w);
		
		fputs("[end group]\n\n", fa_w);

		fclose(fa_w);
	}
	else
	{
		sprintf(cTmpStr, "Unable to open %s", cPath);
		MessagePopup("", cTmpStr);
		return (-1);
	}
	
	return (iReturn);
}


#endif //ifndef RFP35_PROJECT



int get_ProdData(unsigned char *ucData)
{
	if ( memcpy(ucData, gl_ucProdData, 32) == NULL) return(-1);

	return (0);
}

int set_ProdData(unsigned char *ucData)
{
	if ( memcpy(gl_ucProdData, ucData, 32) == NULL) return(-1);
	
	return (0);
}

int get_DeviatData(unsigned char *ucData)
{
	if ( memcpy(ucData, gl_ucDeviatData, 2) == NULL) return(-1);
	
	return (0);
}

int set_DeviatData(unsigned char *ucData)
{
	if ( memcpy(gl_ucDeviatData, ucData, 2) == NULL) return(-1);
	
	return (0);
}

int get_RssiData(unsigned char *ucData)
{
	if ( memcpy(ucData, gl_ucRssiData, 4) == NULL) return(-1);
	
	return (0);
}

int set_RssiData(unsigned char *ucData)
{
	if ( memcpy(gl_ucRssiData, ucData, 4) == NULL) return(-1);
	
	return (0);
}

int get_BarcodeData(unsigned char *ucData)
{
	if ( memcpy(ucData, gl_ucBarcodeData, 10) == NULL) return(-1);
	
	return (0);
}

int set_BarcodeData(unsigned char *ucData)
{
	if ( memcpy(gl_ucBarcodeData, ucData, 10) == NULL) return(-1);
	
	return (0);
}

int get_ClockData(unsigned char *ucData)
{
	if ( memcpy(ucData, gl_ucClockData, 1) == NULL) return(-1);
	
	return (0);
}

int set_ClockData(unsigned char *ucData)
{
	if ( memcpy(gl_ucClockData, ucData, 1) == NULL) return(-1);
	
	return (0);
}

int get_BandgapData(unsigned char *ucData)
{
	if ( memcpy(ucData, gl_ucBandgapData, 1) == NULL) return(-1);
	
	return (0);
}

int set_BandgapData(unsigned char *ucData)
{
	if ( memcpy(gl_ucBandgapData, ucData, 1) == NULL) return(-1);
	
	return (0);
}

void get_EepromDataPath(char *cPath)
{
	sprintf(cPath, gl_cEepromDataPath);
}

void set_EepromDataPath(char *cPath)
{
	sprintf(gl_cEepromDataPath, cPath);
}
//Ende [@JM220408]



char file_get_group_LongLines(struct struct_file_LongLines *file_group,FILE *fa_r,char group[]) //new [@JM090304]
{
int i,j;
char group_flag;
char line[5*MAXSTRINGLEN];

//struct struct_sequ_group  *psequ_group;
//	psequ_group = (struct struct_sequ_group*) sequ_group;
	

	for(i=0;i<50;i++)
		file_group->lines[i][0]=0;
	i = 0;		
	
	group_flag = 0;
	while(fgets(line,5*MAXSTRINGLEN,fa_r))
		{
		if(group_flag)
			{
			if(!strncmp(line,"[end group]",11)) break;
			j = strlen(line);
			for(;j>0;j--)
				{
				if(line[j] < 0x20) line[j] = 0;
				}
			strcpy(&file_group->lines[i][0],line);
			i++;
			}
		if(!strncmp(line,group,strlen(group)))
			group_flag = 1;
		}
	file_group->line_count = i;			
	return(group_flag);
}

char file_getpara_LongLines (struct struct_file_LongLines *file_group,char tag[],void *parameter,int type) //new [@JM090304]
{
int 	i,j,k,flag;
int 	*int_ptr;
int 	*hex_ptr;
double 	*doub_ptr;
char	*char_ptr;

 
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
		case PARA_STRING_LONG:			//name with space
			char_ptr = parameter;
			*char_ptr = 0;
			break;
		}

	flag = 1;		// set default to error
	for(i=0;i<file_group->line_count;i++)
		{
		if(!strncmp(&file_group->lines[i][0],tag,strlen(tag)))
			{
			flag = 0; // set to succes, tagged name was found
			switch(type)
				{
				case PARA_INTEGER:
					parse_line(&file_group->lines[i][0]);
					 int_ptr = parameter;
					*int_ptr = atoi(&token[1][0]);
					break;
				case PARA_HEX:
					parse_line(&file_group->lines[i][0]);
					hex_ptr = parameter;
					sscanf(&token[1][0],"%x",hex_ptr);
					break;
				case PARA_DOUBLE:
					parse_line(&file_group->lines[i][0]);
					 doub_ptr = parameter;
					*doub_ptr = atof(&token[1][0]);
					break;
				case PARA_STRING:
					parse_line(&file_group->lines[i][0]);
					char_ptr = parameter;
					strcpy(char_ptr,&token[1][0]);
					file_group->lines[i][0] = 0;	//line served, clear line
					break;
				case PARA_STRING_LONG:			//name with space
					k = 0;
					j = strlen(&file_group->lines[i][0]);
					while((k<j)&&(file_group->lines[i][k] != '=')) k++;
					k++;
					while(file_group->lines[i][k] == ' ') k++;
					char_ptr = parameter;
					strcpy(char_ptr,&file_group->lines[i][k]);
					file_group->lines[i][0] = 0;	//line served, clear line
					break;
				}
			break; 
			}
		}
	return(flag);
}
