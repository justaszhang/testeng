#include "test.h"
#include "rte_tool.h"
#include "rte_uir.h"
//used in "printer_strobe" & "print_out"
#define PRN_LOOPCOUNT		0x10000



int printer_strobe(int port_address)
{

int i,j;
unsigned char port_value;

	port_value = inp(port_address+1);
	i = 0;
	while(~port_value & 0x80)
		{
		port_value = inp(port_address+1);
		i++;
		if(i == PRN_LOOPCOUNT)
		return (1);
		}
	port_address += 2;
	port_value = inp(port_address);
	port_value |= 0x01;
	outp(port_address, port_value);
	port_value &= 0xfe;
	outp(port_address, port_value);
	return(0);
}


int print_out (char print_str[])
{
int port_address,i,j;
unsigned char port_value;
int printstring_length;
	if(!globals->iprintmode) 
		return(0);				//if printing is disabled during  printout

	printstring_length = strlen(print_str);
//check for cut-string, there are binary data including zero !!
	if(print_str[printstring_length-2] == 0x1b) 
		printstring_length++; // if cut-string, send additional zero
	
	if(!strncmp(globals->sequ_select->printerport,"LPT",3))		//if LPT select, check for online (LPT1:0x378,LPT2:0x278)
		{
		port_address = 0x378;			//LPT1
		if(globals->sequ_select->printerport[3] == '2') 
			port_address = 0x278;		//LPT2
		if(globals->sequ_select->printerport[3] == '3') 
			port_address = 0x3bc;		//LPT3
		port_value = inp(port_address+1);
		i = 0;
		while(~port_value & 0x80)
			{
			port_value = inp(port_address+1);
			outp(port_address+1,(port_value &0xf7)); //printer select, clear bit3
			i++;
			if(i >= PRN_LOOPCOUNT)
				{
				if(ConfirmPopup ("\n Nummer ist Falsch !!","Drucken deaktivieren ? (Ja--> druck aus)"))
       				{//continue printing select
					if(!(inp (port_address+1) & 0x80)) //check online bit
						{									  
						MessagePopup ("Drucker Fehler","Drucker nicht vorhanden, Druckermode ist aus");
						globals->iprintmode = 0;	//set printmode off
						set_menu_printmode(globals->iprintmode);
						return(0);
						}
					}
				}
			}
		j = strlen(print_str);
// temp:
		if(print_str[j-2] == 0x1b) j++; // if cut-string, send additional zero
		for(i=0;i<j;i++)
			{
			outp (port_address, print_str[i]);
			if(printer_strobe (port_address))
//errormessage:
				return(0);
			}
		}
/*		
	if(!strncmp(globals->sequ_select->printerport,"proTest",7))		//if proTest select ?
		{
		protest_write_lpt (0x05, print_str, printstring_length);
		return(0);
		}
*/		
	return(0);
}


#ifdef DEBUG_TIME_MEAS
/* FUNCTION ***************************************************/            
int write_time_per_test()
{
	double seconds;
	char cInfo[256];
	FILE *fp;

	// first test
	if ( globals->sequ_select->act_index < 1 )
	{	
		
		fp=fopen("time_meas.csv", "w");    // write and clear
		
		sprintf(cInfo,"TestName;Sekunden\n");
		fputs(cInfo,fp);
		
		gl_timeBeginTest=gl_timeBegin;     // für den ersten Test

	}	
	else
	{	
		fp=fopen("time_meas.csv", "a");	   // write and append
	}	

	
	time(&gl_timeEndTest);
	seconds = difftime( gl_timeEndTest, gl_timeBeginTest);
	
	sprintf(cInfo,"%s;%d\n", globals->sequ_select->structpointer[globals->sequ_select->act_index]->username,(unsigned long)seconds);
	fputs(cInfo,fp);
	//MessagePopup("Time", cInfo);
	
	// last test
	if ( (globals->sequ_select->act_index+1) == globals->sequ_select->executeable_tests )
	{
		seconds = difftime( gl_timeEndTest, gl_timeBegin);
		sprintf(cInfo,"\nEnde;%d\n",(unsigned long)seconds);
		fputs(cInfo,fp);
		//MessagePopup("Time", cInfo);
	}
	
	fclose(fp);
	
	gl_timeBeginTest=gl_timeEndTest;     // neu für den nächsten Test
	
	return(0);

}
#endif

/* FUNCTION ***************************************************/            
// moved from tool.c to rte_tool.c @GC 30.08.2011
void logging_io_error(int error_code)
{
FILE 	*fa_w;
char 	printstring[256];
char 	temp_str[512];
char 	*ptr_char;
int 	act_index;

	sprintf(printstring,"%-17s->FAIL",globals->sequ_select->structpointer[globals->sequ_select->act_index]->username);
	sprintf(temp_str,IO_ERROR_GLOBAL,error_code);

	fa_w = fopen(globals->sequ_select->log_file,"a");
	if(fa_w)
		{	
		fprintf(fa_w,"%s%s",temp_str,LOG_SEPERATOR_GLOBAL);
		
		strcpy(temp_str,LOG_SEPERATOR_GLOBAL);
		act_index = globals->sequ_select->act_index;
		act_index += 2;
		while(act_index < globals->sequ_select->executeable_tests)
			{
			act_index++;
			strcat(temp_str, LOG_SEPERATOR_GLOBAL);
			}
		ptr_char = TimeStr();
		fprintf(fa_w,"%s%s%s",temp_str,ptr_char,LOG_SEPERATOR_GLOBAL);
		fclose(fa_w);
		}//end if logging
	
	return;
}


/* FUNCTION ***************************************************/            
double myRound(double value , int points)
{
	
	
	char cTemp[80];
	char cFormat[10];
	
	
	// Format String erzeugen
	sprintf(cFormat,"%%.%df",points);       // "%.2f"
	
	//MessagePopup("cFormat", cFormat );  
	
	// umwandeln in Text 
	sprintf(cTemp,cFormat,value);			// "%.2f"

	//MessagePopup("cTemp", cTemp );  

	value = atof(cTemp);
	
	return(value);	
}	


/* FUNCTION ***************************************************/            
int setPassAndLog(char *logStr)
/*                                                                          
 SPECIFICATION:                                                             
 		  call from executed test
          return == 0 --> result okay
          serve result-display
          serve data-logging
                                                                            
***************************************************************/            
{
FILE *datafile;
char printstring[256],logresult_str[256];

	sprintf(printstring,"%-17s",globals->sequ_select->structpointer[globals->sequ_select->act_index]->username);
	strcat(printstring,"-> set PASS");
	print_resultdisplay(printstring);
	print_resultdisplay(logStr);   
//log-result:		
	if(globals->idatalogmode)   //logging to global file for all results
		{
		datafile = fopen(globals->sequ_select->log_file,"a");
		if(datafile)
			{			
			globals->sequ_select->last_logged_index++;
			fprintf(datafile,"%s%s",logStr,LOG_SEPERATOR_GLOBAL);
			fclose(datafile);
//for logging data:
			globals->sequ_select->last_logged_index = globals->sequ_select->act_index;
			}//end if logging
            
		}//end if logmode
	return(0);
}	



/* FUNCTION ***************************************************/            
int check_value(double resultvalue)
/*                                                                          
 SPECIFICATION:                                                             
 		  call from every executed test
          return != 0 --> result over/under limit
          return == 0 --> result okay
          serve result-display
          serve data-logging
                                                                            
***************************************************************/            
{
FILE *datafile;
char tempstring[256], temp_str[512];
char printstring[256],logresult_str[256];
int error,i,temp;
int points;
double lowerlimit,upperlimit;
char *ptemp;
char timestamp_ready = 0;

	
	resultvalue += globals->sequ_select->structpointer[globals->sequ_select->act_index]->resultoffset;
	upperlimit = globals->sequ_select->structpointer[globals->sequ_select->act_index]->upperlimit;
	lowerlimit = globals->sequ_select->structpointer[globals->sequ_select->act_index]->lowerlimit;
	points = globals->sequ_select->structpointer[globals->sequ_select->act_index]->prec;

	sprintf(printstring,"%-17s",globals->sequ_select->structpointer[globals->sequ_select->act_index]->username);

	resultvalue = myRound(resultvalue,points);		  //new @GC 23.03.2012

	
	if((lowerlimit <= resultvalue) && (resultvalue <= upperlimit))
	{
		strcat(printstring,"->PASS");
		error = 0;
	}
	else 
	{
		strcat(printstring,"->FAIL");
		error = 1;
	}

	
	
	switch ( points )
		{
		case 1:
			sprintf(tempstring," %.1f < %.1f < %.1f",lowerlimit,resultvalue,upperlimit);
			sprintf(logresult_str,"%.1f",resultvalue);
			break;
		case 2:
			sprintf(tempstring," %.2f < %.2f < %.2f",lowerlimit,resultvalue,upperlimit);
			sprintf(logresult_str,"%.2f",resultvalue);
			break;
		case 3:
			sprintf(tempstring," %.3f < %.3f < %.3f",lowerlimit,resultvalue,upperlimit);
			sprintf(logresult_str,"%.3f",resultvalue);
			break;
		case 4:			
			sprintf(tempstring," %.4f < %.4f < %.4f",lowerlimit,resultvalue,upperlimit);
			sprintf(logresult_str,"%.4f",resultvalue);
			break;
		case 5:			
			sprintf(tempstring," %.5f < %.5f < %.5f",lowerlimit,resultvalue,upperlimit);
			sprintf(logresult_str,"%.5f",resultvalue);
			break;
		
		
		default:
			sprintf(tempstring," %.0f < %.0f < %.0f",lowerlimit,resultvalue,upperlimit);
			sprintf(logresult_str,"%.0f",resultvalue);
			
			//sprintf(tempstring," %d < %d < %d",(int)lowerlimit,(int)resultvalue,(int)upperlimit);
			//sprintf(logresult_str,"%d",(int)resultvalue);
			break;
		}
	
	
	

	
	// copy for QPDS full String    new @GC 30.08.2011
	strcpy(temp_str,tempstring);
	strcat(temp_str," [");	
	strcat(temp_str, globals->sequ_select->structpointer[globals->sequ_select->act_index]->unit);	
	strcat(temp_str, "]");	
//#define MAX_RESULT_LINE_LENGTH  28
#define MAX_RESULT_LINE_LENGTH  31	
	for(i=strlen(tempstring);i<MAX_RESULT_LINE_LENGTH;i++)
		tempstring[i] = 0x20; //fill spaces
	sprintf(&tempstring[MAX_RESULT_LINE_LENGTH],"[%s]",globals->sequ_select->structpointer[globals->sequ_select->act_index]->unit);	
	if(globals->sequ_select->act_loopcount && error)			//looped test not display or print if error & if loop!=0
		return (error);
#ifdef USE_QPDS	  
	sprintf(structQPDS.strFehlerInfo1,"%s",printstring);
	sprintf(structQPDS.strFehlerInfo2,"%s",temp_str);

#endif

		
//---------- loop over, serve display , printer, data - logging		
// print to result-display: (only if this shown)	
	if((globals->iprintresult == 1) || error)	
		{
		print_resultdisplay(printstring);
		print_resultdisplay(tempstring);
		if(error && globals->sequ_select->structpointer[globals->sequ_select->act_index]->errormessage)
			print_resultdisplay(globals->sequ_select->structpointer[globals->sequ_select->act_index]->errormessage);
		}
//print-report:		
	if(globals->iprintmode)
		{
		if((globals->iprintmode == 2) || ((globals->iprintmode == 1) && error))
			{
			if(printkopf[0] != 0) 
				{ 
				sprintf (temp_str,"%s\n",printkopf); 
				print_out(temp_str);
				printkopf[0] = 0;  //only one time
				}  
// printer needs a new line before close for printing last line !!!
			sprintf(temp_str,"%s\n",printstring);
			print_out(temp_str);
			sprintf(temp_str,"%s\n",tempstring);
			print_out(temp_str);
			if(globals->sequ_select->structpointer[globals->sequ_select->act_index]->errormessage)
				{
				sprintf(temp_str,"\n%s\n",globals->sequ_select->structpointer[globals->sequ_select->act_index]->errormessage);
				print_out(temp_str);
				}
			if(error)
				{
				sprintf(temp_str,"\n\n\n\n\n\n");
				print_out(temp_str);
				sprintf(temp_str,"%c%c%c",PRINTER_CUT_STRING); //cutting option
				print_out(temp_str);
				}
//linefeed next last test:
			if(globals->iprintmode == 2)
				{
				if(globals->sequ_select->executeable_tests == ((globals->sequ_select->act_index)+2))
					{
					sprintf(temp_str,"\n\n\n\n");
					print_out(temp_str);
					}
				}
			}//end if error
		}//end if printmode
//log-result:		
	if(globals->idatalogmode)   //logging to global file for all results
		{
		datafile = fopen(globals->sequ_select->log_file,"a");
		if(datafile)
			{			
			globals->sequ_select->last_logged_index++;
			while(globals->sequ_select->last_logged_index < globals->sequ_select->act_index)
				{
				fprintf(datafile,"(X)%s",LOG_SEPERATOR_GLOBAL);
				globals->sequ_select->last_logged_index++;
				}
			if(!error)
				fprintf(datafile,"%s%s",logresult_str,LOG_SEPERATOR_GLOBAL);
			else
				{
				fprintf(datafile,"%s(FAIL)%s",logresult_str,LOG_SEPERATOR_GLOBAL);
				if((globals->sequ_select->ierror + 1) == globals->sequ_select->maxerror) 
					{
//					strcpy(temp_str,LOG_SEPERATOR_GLOBAL);
					temp_str[0] = 0;
					i = globals->sequ_select->act_index;
					i++;
					while(i < globals->sequ_select->executeable_tests)
						{
						i++;
						strcat(temp_str, LOG_SEPERATOR_GLOBAL);
						}
					ptemp = TimeStr();
					fprintf(datafile,"%s%s%s",temp_str,ptemp,LOG_SEPERATOR_GLOBAL);
#ifdef USE_QPDS					
					sprintf(structQPDS.strEndZeit,"%c%c%c%c%c%c%c%c%c%c%c%c"	,cDateLabel[8],cDateLabel[9],cDateLabel[0],cDateLabel[1],cDateLabel[3],cDateLabel[4]
																			,ptemp[0],ptemp[1],ptemp[3],ptemp[4],ptemp[6],ptemp[7]);
#endif
					timestamp_ready = 1;
					}
				}
// if(last test in sequence) append time-string (for both: fail or okay)				
			if(!timestamp_ready  && ((globals->sequ_select->act_index+1) == globals->sequ_select->executeable_tests))
				{
				ptemp = TimeStr();
				fprintf(datafile,"%s%s",ptemp,LOG_SEPERATOR_GLOBAL);
#ifdef USE_QPDS				
				sprintf(structQPDS.strEndZeit,"%c%c%c%c%c%c%c%c%c%c%c%c"	,cDateLabel[8],cDateLabel[9],cDateLabel[0],cDateLabel[1],cDateLabel[3],cDateLabel[4]
																			,ptemp[0],ptemp[1],ptemp[3],ptemp[4],ptemp[6],ptemp[7]);
				WriteQPDS(0); //pass
#endif				
				}
			fclose(datafile);
//for logging data:
			globals->sequ_select->last_logged_index = globals->sequ_select->act_index;
			}//end if logging
		else
			{
			sprintf(temp_str,"schreiben in die Loggdatei nicht möglich!! [%s]",globals->sequ_select->log_file);
			MessagePopup ("proTest - Nachricht",temp_str);
			//MessagePopup ("proTest - Nachricht","schreiben in die Loggdatei nicht möglich!!");
			globals->idatalogmode = 0;
			}
            
        if(error)
		{
            WriteQPDS(1);  //fail
		}
            
            
		}//end if logmode
		
	
	
#ifdef DEBUG_TIME_MEAS
    write_time_per_test();
#endif	

	
	return(error);
}	




int check_value_str(char *resultvalue)
/*                                                                          
 SPECIFICATION:                                                             
 		  call from every executed test
          return != 0 --> result over/under limit
          return == 0 --> result okay
          serve result-display
          serve data-logging
                                                                            
***************************************************************/            
{
FILE *datafile;
char tempstring[256], temp_str[512];
char printstring[256],logresult_str[256];
int error,i,temp;
int    points;
double lowerlimit,upperlimit;
char *ptemp;
char timestamp_ready = 0;
char strlowerlimit[40],strupperlimit[40];  

	
	upperlimit = globals->sequ_select->structpointer[globals->sequ_select->act_index]->upperlimit;
	lowerlimit = globals->sequ_select->structpointer[globals->sequ_select->act_index]->lowerlimit;
	points = globals->sequ_select->structpointer[globals->sequ_select->act_index]->prec;
	
	sprintf(printstring,"%-17s",globals->sequ_select->structpointer[globals->sequ_select->act_index]->username);
	
	
	if(! points == 99)
	{	
		strcat(printstring,"->PASS");
		error = 0;
	}	
	else
	{	
		strcat(printstring,"->PASS");
		error = 0;
	}	
		
		
	sprintf(tempstring," 0 < %s < 0",resultvalue);
	sprintf(logresult_str,"%s",resultvalue);

	for(i=strlen(tempstring);i<MAX_RESULT_LINE_LENGTH;i++)
		tempstring[i] = ' '; //fill spaces
	sprintf(&tempstring[MAX_RESULT_LINE_LENGTH],"[%s]",globals->sequ_select->structpointer[globals->sequ_select->act_index]->unit);	
	
	if(globals->sequ_select->act_loopcount && error)			//looped test not display or print if error & if loop!=0
		return (error);

	
	// copy for QPDS full String    new @GC 30.08.2011
	sprintf(temp_str," %s [%s]",resultvalue,globals->sequ_select->structpointer[globals->sequ_select->act_index]->unit);
#ifdef USE_QPDS	
	sprintf(structQPDS.strFehlerInfo1,"%s",printstring);        // new @GC 20.07.2011  
	sprintf(structQPDS.strFehlerInfo2,"%s",temp_str);           // new @GC 20.07.2011  
#endif
#ifdef FUNKTEL	
	if(error)
		{
	   	write_file_to_funktel_QIS(resultvalue); 
		funktel_logdata(strLOGDatenFunktelPath,resultvalue, cPruefstatus);
		}
#endif			
		
//---------- loop over, serve display , printer, data - logging		
// print to result-display: (only if this shown)	
	if((globals->iprintresult == 1) || error)	
		{
		print_resultdisplay(printstring);
		print_resultdisplay(tempstring);
		if(error && globals->sequ_select->structpointer[globals->sequ_select->act_index]->errormessage)
			print_resultdisplay(globals->sequ_select->structpointer[globals->sequ_select->act_index]->errormessage);
		}
//print-report:		
	if(globals->iprintmode)
		{
		if((globals->iprintmode == 2) || ((globals->iprintmode == 1) && error))
			{
			if(printkopf[0] != 0) 
				{ 
				sprintf (temp_str,"%s\n",printkopf); 
				print_out(temp_str);
				printkopf[0] = 0;  //only one time
				}  
// printer needs a new line before close for printing last line !!!
			sprintf(temp_str,"%s\n",printstring);
			print_out(temp_str);
			sprintf(temp_str,"%s\n",tempstring);
			print_out(temp_str);
			if(globals->sequ_select->structpointer[globals->sequ_select->act_index]->errormessage)
				{
				sprintf(temp_str,"\n%s\n",globals->sequ_select->structpointer[globals->sequ_select->act_index]->errormessage);
				print_out(temp_str);
				}
			if(error)
				{
				sprintf(temp_str,"\n\n\n\n\n\n");
				print_out(temp_str);
				sprintf(temp_str,"%c%c%c",PRINTER_CUT_STRING); //cutting option
				print_out(temp_str);
#ifdef FUNKTEL	

			   	print_baco();
#endif			
				}
//linefeed next last test:
			if(globals->iprintmode == 2)
				{
				if(globals->sequ_select->executeable_tests == ((globals->sequ_select->act_index)+2))
					{
					sprintf(temp_str,"\n\n\n\n");
					print_out(temp_str);
					}
				}
			}//end if error
		}//end if printmode
//log-result:		
	if(globals->idatalogmode)   //logging to global file for all results
		{
		datafile = fopen(globals->sequ_select->log_file,"a");
		if(datafile)
			{			
			globals->sequ_select->last_logged_index++;
			while(globals->sequ_select->last_logged_index < globals->sequ_select->act_index)
				{
				fprintf(datafile,"(X)%s",LOG_SEPERATOR_GLOBAL);
				globals->sequ_select->last_logged_index++;
				}
			if(!error)
				fprintf(datafile,"%s%s",logresult_str,LOG_SEPERATOR_GLOBAL);
			else
				{
				fprintf(datafile,"%s(FAIL)%s",logresult_str,LOG_SEPERATOR_GLOBAL);
				if((globals->sequ_select->ierror + 1) == globals->sequ_select->maxerror) 
					{
//					strcpy(temp_str,LOG_SEPERATOR_GLOBAL);
					temp_str[0] = 0;
					i = globals->sequ_select->act_index;
					i++;
					while(i < globals->sequ_select->executeable_tests)
						{
						i++;
						strcat(temp_str, LOG_SEPERATOR_GLOBAL);
						}
					ptemp = TimeStr();
					fprintf(datafile,"%s%s%s",temp_str,ptemp,LOG_SEPERATOR_GLOBAL);
#ifdef USE_QPDS					
					sprintf(structQPDS.strEndZeit,"%c%c%c%c%c%c%c%c%c%c%c%c"	,cDateLabel[8],cDateLabel[9],cDateLabel[0],cDateLabel[1],cDateLabel[3],cDateLabel[4]
																			    ,ptemp[0],ptemp[1],ptemp[3],ptemp[4],ptemp[6],ptemp[7]);
#endif
					timestamp_ready = 1;
					}
				}
// if(last test in sequence) append time-string (for both: fail or okay)				
			if(!timestamp_ready  && ((globals->sequ_select->act_index+1) == globals->sequ_select->executeable_tests))
				{
				ptemp = TimeStr();
				fprintf(datafile,"%s%s",ptemp,LOG_SEPERATOR_GLOBAL);
#ifdef USE_QPDS				
				sprintf(structQPDS.strEndZeit,"%c%c%c%c%c%c%c%c%c%c%c%c"	,cDateLabel[8],cDateLabel[9],cDateLabel[0],cDateLabel[1],cDateLabel[3],cDateLabel[4]
																			,ptemp[0],ptemp[1],ptemp[3],ptemp[4],ptemp[6],ptemp[7]);

				WriteQPDS(0);  //fail     // new @GC 20.07.2011
#endif				
				}
			fclose(datafile);
//for logging data:
			globals->sequ_select->last_logged_index = globals->sequ_select->act_index;
			}//end if logging
		else
			{
			sprintf(temp_str,"schreiben in die Loggdatei nicht möglich!! [%s]",globals->sequ_select->log_file);
			MessagePopup ("proTest - Nachricht",temp_str);
			//MessagePopup ("proTest - Nachricht","schreiben in die Loggdatei nicht möglich!!");
			globals->idatalogmode = 0;
			}
#ifdef USE_QPDS	            
        if(error)
		{
            WriteQPDS(1);  //pass       // new @GC 20.07.2011
		}
#endif            
		}//end if logmode
		

#ifdef DEBUG_TIME_MEAS
    write_time_per_test();
#endif	
	
	return(error);
}	


/**************************************************************************************************************
** check_value_TXT_cmp()
**
** neue Funktion GC 29.03.2012
**
** diese Funktion 
** aendert die Einheit "unit" nach "TXT"
** ruft den Text vergleich check_value_str_cmp() auf
** aendert die Einheit "unit" zurueck zum Original
** und liefert den Resultvalue zurueck
**
**
**************************************************************************************************************/
int  check_value_TXT_cmp(char *resultvalue, char *strExpected)
{
	
	int  resultstatus;
	char cTemp[80];
	
	//strcpy(cTemp,act_structptr->unit);
	//strcpy(act_structptr->unit,"TXT");
	strcpy(cTemp,globals->sequ_select->structpointer[globals->sequ_select->act_index]->unit);
	strcpy(globals->sequ_select->structpointer[globals->sequ_select->act_index]->unit,"TXT");
	
	resultstatus = check_value_str_cmp(resultvalue,strExpected);
	
	//strcpy(act_structptr->unit,cTemp);
	strcpy(globals->sequ_select->structpointer[globals->sequ_select->act_index]->unit,cTemp);
	
	return(resultstatus);
}	


/**************************************************************************************************************
** check_value_str_cmp()
**
** neue Funktion GC 31.03.2011
** String Vergleich mit Fehlererkennung fuer Project RFP35 eingefuehrt
**
** SPECIFICATION:                                                             
** 		  call from every executed test
**          return != 0 --> result over/under limit
**          return == 0 --> result okay
**          serve result-display
**          serve data-logging
**
** Dies ist eine Kopie von check_value_str()
** ist noch nicht komplett geaendert, nur ein Vergleich wurde eingefuehrt.
** Diese Funktion ist immer noch besser als das Original,
** welches noch nicht einmal einen Vergleich macht.
**
** Dir Funktion "check_value_str()" ist überhaupt kein Check, sondern nur eine Ausgabe.
**
** hier erstmal eine vorab Loesung mit Stringvergleich.
**
**************************************************************************************************************/
int check_value_str_cmp(char *resultvalue, char *strExpected)
{
FILE *datafile;
char tempstring[256], temp_str[512];
char printstring[256],logresult_str[256];
int error,i,temp;
double lowerlimit,upperlimit;
char *ptemp;
char timestamp_ready = 0;
//char strlowerlimit[40],strupperlimit[40];  

	
	upperlimit = globals->sequ_select->structpointer[globals->sequ_select->act_index]->upperlimit;
	lowerlimit = globals->sequ_select->structpointer[globals->sequ_select->act_index]->lowerlimit;
	sprintf(printstring,"%-17s",globals->sequ_select->structpointer[globals->sequ_select->act_index]->username);

	if( strcmp(resultvalue,strExpected) == 0 )
	{
		strcat(printstring,"->PASS");
		error = 0;
	}
	else 
	{
		strcat(printstring,"->FAIL");
		error = 1;
	}

		
	sprintf(tempstring," 0 < %s < 0",resultvalue);
	sprintf(logresult_str,"%s",resultvalue);
			
	
	for(i=strlen(tempstring);i<MAX_RESULT_LINE_LENGTH;i++)
		tempstring[i] = 0x20; //fill spaces
	sprintf(&tempstring[MAX_RESULT_LINE_LENGTH],"[%s]",globals->sequ_select->structpointer[globals->sequ_select->act_index]->unit);	
	if(globals->sequ_select->act_loopcount && error)			//looped test not display or print if error & if loop!=0
		return (error);

	
	// copy for QPDS full String    new @GC 30.08.2011
	sprintf(temp_str," %s [%s]",resultvalue,globals->sequ_select->structpointer[globals->sequ_select->act_index]->unit);
#ifdef USE_QPDS	
	sprintf(structQPDS.strFehlerInfo1,"%s",printstring);        // new @GC 20.07.2011  
	sprintf(structQPDS.strFehlerInfo2,"%s",temp_str);           // new @GC 20.07.2011  
#endif
	
#ifdef FUNKTEL	
	if(error)
		{
	   	write_file_to_funktel_QIS(resultvalue); 
		funktel_logdata(strLOGDatenFunktelPath,resultvalue, cPruefstatus);
		}
#endif			
		
//---------- loop over, serve display , printer, data - logging		
// print to result-display: (only if this shown)	
	if((globals->iprintresult == 1) || error)	
		{
		print_resultdisplay(printstring);
		print_resultdisplay(tempstring);
		if(error && globals->sequ_select->structpointer[globals->sequ_select->act_index]->errormessage)
			print_resultdisplay(globals->sequ_select->structpointer[globals->sequ_select->act_index]->errormessage);
		}
//print-report:		
	if(globals->iprintmode)
		{
		if((globals->iprintmode == 2) || ((globals->iprintmode == 1) && error))
			{
			if(printkopf[0] != 0) 
				{ 
				sprintf (temp_str,"%s\n",printkopf); 
				print_out(temp_str);
				printkopf[0] = 0;  //only one time
				}  
// printer needs a new line before close for printing last line !!!
			sprintf(temp_str,"%s\n",printstring);
			print_out(temp_str);
			sprintf(temp_str,"%s\n",tempstring);
			print_out(temp_str);
			if(globals->sequ_select->structpointer[globals->sequ_select->act_index]->errormessage)
				{
				sprintf(temp_str,"\n%s\n",globals->sequ_select->structpointer[globals->sequ_select->act_index]->errormessage);
				print_out(temp_str);
				}
			if(error)
				{
				sprintf(temp_str,"\n\n\n\n\n\n");
				print_out(temp_str);
				sprintf(temp_str,"%c%c%c",PRINTER_CUT_STRING); //cutting option
				print_out(temp_str);
#ifdef FUNKTEL	

			   	print_baco();
#endif			
				}
//linefeed next last test:
			if(globals->iprintmode == 2)
				{
				if(globals->sequ_select->executeable_tests == ((globals->sequ_select->act_index)+2))
					{
					sprintf(temp_str,"\n\n\n\n");
					print_out(temp_str);
					}
				}
			}//end if error
		}//end if printmode
//log-result:		
	if(globals->idatalogmode)   //logging to global file for all results
		{
		datafile = fopen(globals->sequ_select->log_file,"a");
		if(datafile)
			{			
			globals->sequ_select->last_logged_index++;
			while(globals->sequ_select->last_logged_index < globals->sequ_select->act_index)
				{
				fprintf(datafile,"(X)%s",LOG_SEPERATOR_GLOBAL);
				globals->sequ_select->last_logged_index++;
				}
			if(!error)
				fprintf(datafile,"%s%s",logresult_str,LOG_SEPERATOR_GLOBAL);
			else
				{
				fprintf(datafile,"%s(FAIL)%s",logresult_str,LOG_SEPERATOR_GLOBAL);
				if((globals->sequ_select->ierror + 1) == globals->sequ_select->maxerror) 
					{
//					strcpy(temp_str,LOG_SEPERATOR_GLOBAL);
					temp_str[0] = 0;
					i = globals->sequ_select->act_index;
					i++;
					while(i < globals->sequ_select->executeable_tests)
						{
						i++;
						strcat(temp_str, LOG_SEPERATOR_GLOBAL);
						}
					ptemp = TimeStr();
					fprintf(datafile,"%s%s%s",temp_str,ptemp,LOG_SEPERATOR_GLOBAL);
#ifdef USE_QPDS					
					sprintf(structQPDS.strEndZeit,"%c%c%c%c%c%c%c%c%c%c%c%c"	,cDateLabel[8],cDateLabel[9],cDateLabel[0],cDateLabel[1],cDateLabel[3],cDateLabel[4]
																			    ,ptemp[0],ptemp[1],ptemp[3],ptemp[4],ptemp[6],ptemp[7]);
#endif					
					timestamp_ready = 1;
					}
				}
// if(last test in sequence) append time-string (for both: fail or okay)				
			if(!timestamp_ready  && ((globals->sequ_select->act_index+1) == globals->sequ_select->executeable_tests))
				{
				ptemp = TimeStr();
				fprintf(datafile,"%s%s",ptemp,LOG_SEPERATOR_GLOBAL);
#ifdef USE_QPDS				
				sprintf(structQPDS.strEndZeit,"%c%c%c%c%c%c%c%c%c%c%c%c"	,cDateLabel[8],cDateLabel[9],cDateLabel[0],cDateLabel[1],cDateLabel[3],cDateLabel[4]
																			,ptemp[0],ptemp[1],ptemp[3],ptemp[4],ptemp[6],ptemp[7]);

				WriteQPDS(0);  //pass    // new @GC 20.07.2011
#endif				
				}
			fclose(datafile);
//for logging data:
			globals->sequ_select->last_logged_index = globals->sequ_select->act_index;
			}//end if logging
		else
			{
			sprintf(temp_str,"schreiben in die Loggdatei nicht möglich!! [%s]",globals->sequ_select->log_file);
			MessagePopup ("proTest - Nachricht",temp_str);
			//MessagePopup ("proTest - Nachricht","schreiben in die Loggdatei nicht möglich!!");
			globals->idatalogmode = 0;
			}

        if(error)
		{
            WriteQPDS(1);  //fail     // new @GC 20.07.2011
		}
            
		}//end if logmode
		
#ifdef DEBUG_TIME_MEAS
    write_time_per_test();
#endif	
		
	return(error);
}	


/* FUNCTION ***************************************************/            
// set structpointer to temporary struct with parameters, do "check_value"
// & set back structpointer
int check_other_value(char name[],double lowerlimit,double resultvalue,double upperlimit,int prec,char unit[],char err_messg[])
{
struct struct_test *temp_ptr;
struct struct_test  test_struct;
int check_answer;

	test_struct.lowerlimit = lowerlimit;
	test_struct.upperlimit = upperlimit;
	test_struct.resultoffset = 0;
	strcpy(test_struct.username,name);
	strcpy(test_struct.unit,unit);
	test_struct.errormessage = malloc(strlen(err_messg)+1);
	strcpy(test_struct.errormessage,err_messg);
	test_struct.prec = prec;
	temp_ptr = globals->sequ_select->structpointer[globals->sequ_select->act_index];
	globals->sequ_select->structpointer[globals->sequ_select->act_index] = &test_struct;
	check_answer = check_value(resultvalue);
	free(test_struct.errormessage);
	globals->sequ_select->structpointer[globals->sequ_select->act_index] = temp_ptr;
	return(check_answer);
}


void set_serial_number(int number)
{
char temp_str[80];

	sprintf(globals->sequ_select->sernumber,"%d",number);
	
	SetCtrlVal (globals->imainpanel, globals->sequ_select->sernum_handle, globals->sequ_select->sernumber );
	
	sprintf(temp_str,"Serial Number:  %s\n", globals->sequ_select->sernumber);
	
	strcat(printkopf,"\n");
	strcat(printkopf,temp_str);
	print_resultdisplay(temp_str);
	sprintf(temp_str,"---------------------------");
	print_resultdisplay(temp_str);
	strcat(printkopf,temp_str);
	return;
}

void set_char_serial_number(char *number)
{
char temp_str[80];
    
	sprintf(globals->sequ_select->sernumber,"%s",number);
	
	SetCtrlVal (globals->imainpanel, globals->sequ_select->sernum_handle, globals->sequ_select->sernumber );
	
	sprintf(temp_str,"Serial Number:  %s\n", globals->sequ_select->sernumber);
	
	strcat(printkopf,"\n");
	strcat(printkopf,temp_str);
	print_resultdisplay(temp_str);
	sprintf(temp_str,"---------------------------");
	print_resultdisplay(temp_str);
	strcat(printkopf,temp_str);
	return;
}



int backup_logdata_files(struct rte_globals *globals)
{
int i;


	//@GC 18.02.2012
	if(globals->data_backup->left_equal_right)			//if both sides equal only left side
		i = globals->data_backup->device_count_left;
	else
		i = globals->data_backup->device_count_left + globals->data_backup->device_count_right;
		
	if(i > 0)
	{
		// protest_adapterTag_counter_update (i);
	 	adapter_counter_update(globals, i);   //@GC 20.02.2012
	}
	
	if(!globals->data_backup->trigger_count) return(0);   //do this, only if initialized
	//@GC 18.02.2012
	//if(globals->debug_mode) return(0);                  // debug-logfiles
	if(globals->data_backup->init_left && globals->data_backup->device_count_left)
	{
		system (globals->data_backup->command_line_with_arg_left);
		if(globals->data_backup->left_equal_right)		  // if both sides equal then only left side
			globals->data_backup->device_count_right = 0; // function ready !
	}
	if(globals->data_backup->init_right && globals->data_backup->device_count_right)
		system (globals->data_backup->command_line_with_arg_right);
	return (0);
}




int serve_logdata_handling(struct rte_globals *globals)
// generation of sequence-header, if neccessary
// make backup to global path (look in rte_settings.txt) , if neccessary
{
int i;
//2.03.2001: check for table-header generation and initialize commandline for external backup-exe:
// exe-path & first argument is built in rte.exe
	if(globals->sequ_select == globals->left)
		{
		if(!globals->data_backup->init_left)
			{
			write_sequ2logfile(globals);
			strcpy(globals->data_backup->command_line_with_arg_left,globals->data_backup->command_line);
			strcat(globals->data_backup->command_line_with_arg_left," ");
//globals->sequ_select->log_file is every time rebuilt from write_sequ2logfile			
			strcat(globals->data_backup->command_line_with_arg_left,globals->sequ_select->log_file);
			globals->data_backup->device_count_left = 0;
			globals->data_backup->init_left = 1;			
			return(0);
			}
		}

	if(globals->sequ_select == globals->right)
		{
		if(!globals->data_backup->init_right)
			{
			write_sequ2logfile(globals);
			strcpy(globals->data_backup->command_line_with_arg_right,globals->data_backup->command_line);
			strcat(globals->data_backup->command_line_with_arg_right," ");
//globals->sequ_select->log_file is every time rebuilt from write_sequ2logfile			
			strcat(globals->data_backup->command_line_with_arg_right,globals->sequ_select->log_file);
			globals->data_backup->device_count_right = 0;
			globals->data_backup->init_right = 1;			
// check, if both sides the same
			if(!strcmp(globals->left->sachnumber,globals->right->sachnumber))
				globals->data_backup->left_equal_right = 1;			
			return(0);
			}
		}

//2.03.2001: check for data-backup of log-file:
	if(globals->data_backup->trigger_count)   //do this, only if initialized
		{
		if((globals->sequ_select == globals->left) || globals->data_backup->left_equal_right) //if both sides equal, use only left
			{
			globals->data_backup->device_count_left++;
			if(globals->data_backup->trigger_count <= globals->data_backup->device_count_left)
				{
				system (globals->data_backup->command_line_with_arg_left);
			 	adapter_counter_update(globals, globals->data_backup->device_count_left );   //@GC 20.02.2012
				// @GC 18.02.2012
				//protest_adapterTag_counter_update(globals->data_backup->device_count_left);
				globals->data_backup->device_count_left = 0;
				write_sequ2logfile(globals);
				return(0);
				}
			}
		if(globals->sequ_select == globals->right)
			{
			globals->data_backup->device_count_right++;
			if(globals->data_backup->trigger_count <= globals->data_backup->device_count_right)
				{
				system (globals->data_backup->command_line_with_arg_right);
			 	adapter_counter_update(globals, globals->data_backup->device_count_right );   //@GC 20.02.2012
				// @GC 18.02.2012
				//protest_adapterTag_counter_update(globals->data_backup->device_count_right);
				globals->data_backup->device_count_right = 0;
				write_sequ2logfile(globals);
				}
			}
		}
	return (0);
}




/* FUNCTION ***************************************************/            
void set_testheader(int mode)
/*                                                                          
 SPECIFICATION:                                                             
 		  call from (first) test, set up print-header for printer,
 		  						  set up device header for data-logging
***************************************************************/            
{
int  i,j;
char temp[40];
char temp_str[40];
char *ptemp = &temp[0];
char strbarcode[20];

FILE *datafile = NULL;  


    // @GC 20.03.2014
	globals->sequ_select->sernumber[0] = '\0'; 	//use for setting additional slash in log-file, if zero
	SetCtrlVal (globals->imainpanel, globals->sequ_select->sernum_handle, globals->sequ_select->sernumber );


//set testheader for datafile: -> if (datalogmode && actual loopcount == entree-loopcount)
	if(!(globals->idatalogmode && (globals->sequ_select->act_loopcount == globals->sequ_select->structpointer[globals->sequ_select->act_index]->loopcount)))
		return;	
		
// new: 02.02.2001
// generating the sequence-header, make backup to global path
	serve_logdata_handling(globals);
//------------------------------------------------------------------	

	
	if(globals->idatalogmode)
		datafile = fopen(globals->sequ_select->log_file,"a");
		
	sprintf(printkopf,"\n -- Test Report --\n");
	if(datafile)
		{
        //@GC 04.09.2011
		sprintf(temp_str,"\n%s%s%d%d%s",globals->system_id,LOG_SEPERATOR_GLOBAL,globals->ireference,globals->irepairmode,LOG_SEPERATOR_GLOBAL);
		fprintf(datafile,temp_str);
		sprintf(temp_str,"%d%s%s%s",globals->iloginlevel,LOG_SEPERATOR_GLOBAL,globals->loginname,LOG_SEPERATOR_GLOBAL);
		fprintf(datafile,temp_str);
		sprintf(temp_str,"%s%s",globals->sequ_select->fixtureID,LOG_SEPERATOR_GLOBAL);		//new 18.01.2001, ro
		fprintf(datafile,temp_str);
		if(globals->sequ_select == globals->left) strcpy(temp,"left"); 
		if(globals->sequ_select == globals->right) strcpy(temp,"right"); 
		sprintf(temp_str,"%s%s",temp,LOG_SEPERATOR_GLOBAL);		//new 18.01.2001, ro
		fprintf(datafile,temp_str);
		}
	strcat(printkopf,"DATE:           ");
	ptemp = DateStr ();
// change format 	MM-TT-JJJJ to TT-MM-JJJJ
///					0123456789
	strcpy(cDateLabel,ptemp);
	strcpy(temp,ptemp);
	temp[0] = cDateLabel[3];
	temp[1] = cDateLabel[4];
	temp[3] = cDateLabel[0];
	temp[4] = cDateLabel[1];
	sprintf(temp_str,"%s%s",temp,LOG_SEPERATOR_GLOBAL);
	if(datafile)
		fprintf(datafile,temp_str);
	strcat(printkopf,temp);
	strcat(printkopf,"\nTIME:           ");
	//HH:MM:SS
	ptemp = TimeStr();
	strcpy(cTimeLabel,ptemp);
//	temp[5] = 0;
	sprintf(temp_str,"%s%s",cTimeLabel,LOG_SEPERATOR_GLOBAL);
	if(datafile)
		fprintf(datafile,temp_str);
	strcat(printkopf,cTimeLabel);
//	strcat(printkopf,"\nProduct:     ");
	strcat(printkopf,"\n");
	strcat(printkopf,globals->sequ_select->sachnumber);				//adapterkennung
//string to result-display:
	print_resultdisplay(printkopf);
	if(datafile)
		fclose(datafile);
#ifdef USE_QPDS
	sprintf(structQPDS.strStartZeit,"%c%c%c%c%c%c%c%c%c%c%c%c"	,cDateLabel[8],cDateLabel[9],cDateLabel[0],cDateLabel[1],cDateLabel[3],cDateLabel[4]
																,cTimeLabel[0],cTimeLabel[1],cTimeLabel[3],cTimeLabel[4],cTimeLabel[6],cTimeLabel[7]);
	

#endif

#ifdef DEBUG_TIME_MEAS
	time(&gl_timeBegin);
#endif
	
	
	
	return;
}


/*
print_logfile(char path[])
{
// outline: partnumber(sachnumber), serialnumber, date , time, 
FILE *fa_w;
char 	temp_str[MAXPATHNAMELEN];
char	tempstr[20];
int i,str_pos;

	strcpy(temp_str,path);
	strcat(temp_str,"\\");
	i = strlen(logstring);
	str_pos = 0;
	while(logstring[str_pos] != LOGSEPERATOR)
		{
		tempstr[str_pos] = logstring[str_pos];
		str_pos++;
		if(i == str_pos) break;
		}
	tempstr[str_pos] = 0;
	strcat(tempstr,".txt");
	strcat(temp_str,tempstr);
	fa_w = fopen(temp_str,"w");
	if(fa_w)
		{
		fprintf(fa_w,logstring);
		fclose(fa_w);
		}
	else
		MessagePopup ("LOGFILE open error", temp_str);
	return(0);
}

*/


//----------------------------------------------------------
/* FUNCTION ***************************************************/            
void print_resultdisplay(char buffer[])
/*                                                                          
 SPECIFICATION: print string to resultdisplay                                                            
***************************************************************/            
{
	if(globals && globals->imainpanel)
		InsertTextBoxLine (globals->imainpanel, PANEL1_RESULTDISPLAY, 0,buffer);
	ProcessDrawEvents();
	return;
}		

/* FUNCTION ***************************************************/            
void set_statuswindow(char *buffer,long background, int select)
/*                                                                          
 SPECIFICATION: print string to resultdisplay                                                            
				
***************************************************************/            
{
	if(select == LEFT)	
		{
		SetCtrlVal (globals->imainpanel, globals->left->status_handle, buffer);
		SetCtrlAttribute (globals->imainpanel, globals->left->status_handle,ATTR_TEXT_BGCOLOR, background);
		}
	if(select == RIGHT)	
		{
		SetCtrlVal (globals->imainpanel, globals->right->status_handle, buffer);
		SetCtrlAttribute (globals->imainpanel, globals->right->status_handle,ATTR_TEXT_BGCOLOR, background);
		}
	return;
}		

/* FUNCTION ***************************************************/            
void set_hintwindow(char *buffer,long background, int select)
/*                                                                          
 SPECIFICATION: print string to resultdisplay                                                            
				
***************************************************************/            
{
	if(select == LEFT)	
		{
		SetCtrlVal (globals->imainpanel, globals->left->hint_handle, buffer);
		SetCtrlAttribute (globals->imainpanel, globals->left->hint_handle,ATTR_TEXT_BGCOLOR, background);
		}
	if(select == RIGHT)	
		{
		SetCtrlVal (globals->imainpanel, globals->right->hint_handle, buffer);
		SetCtrlAttribute (globals->imainpanel, globals->right->hint_handle,ATTR_TEXT_BGCOLOR, background);
		}
	return;
}		

/* FUNCTION ***************************************************/            
void goto_test(char test_username[])
/*                                                                          
 SPECIFICATION: search label from test_username,if succes: set actual index to test
***************************************************************/            
{
int index;
char tempstring[50];

	for(index = 0;index<globals->sequ_select->executeable_tests;index++)	//search name for & back,!
		{
		if(!(strcmp(globals->sequ_select->structpointer[index]->username,test_username)))
			{
			globals->sequ_select->jmp_absolut = index+1;			  //new index, zero-index for jmp possible 
			break;
			}
		}
	if(index == globals->sequ_select->executeable_tests)		//no match
		{
		sprintf(tempstring,"Test: %s nicht gefunden, Test wird unterbrochen !",test_username);
		MessagePopup ("Funktion: Testprozedur:  Zielposition nicht gefunden !", tempstring);
		globals->iabort = 1;
		}
	ProcessSystemEvents ();		//if goto pre-test or same test: there is an endless loop ! serve abort-buttom
	return;
}

/* FUNCTION ***************************************************/            
void set_menu_printmode(int on_off)
/*                                                                          
 SPECIFICATION: switch to other sequence
***************************************************************/            
{

//	
	SetMenuBarAttribute (GetPanelMenuBar (globals->imainpanel), MENU1_PRINT_PRINTON,ATTR_CHECKED, on_off);
	return;
}

/* FUNCTION ***************************************************/            
void set_sequence(int select)
/*                                                                          
 SPECIFICATION: switch to other sequence
***************************************************************/            
{

	if(select == LEFT)
		globals->sequ_select = globals->left;
	if(select == RIGHT)
		globals->sequ_select = globals->right;
	return;
}

int get_testresult(int select)
{
	if(select == LEFT)	
		return (globals->left->ierror);
	if(select == RIGHT)	
		return (globals->right->ierror);
	return(0);
}


#ifndef RFP35_PROJECT
void set_RepairMode(int iRepairMode)
{
	globals->irepairmode=iRepairMode;     // @GC 05.09.2011
	
}
#endif


// new function for RFP35

/* FUNCTION ***************************************************/            
int	isInProductionMode(void)
{
	
	// Menu->specials->reference
	if( globals->ireference != 0 ) 
		return(0);     // only measure
		
	return(1);  //normal Production Mode
}

/* FUNCTION ***************************************************/            
int	isInReferenceMode(void)
{
	
	// Menu->specials->reference
	if( globals->ireference == 0 ) 
		return(0);     //normal Production Mode
		
	return(1);  //Reference Mode
}

/* FUNCTION ***************************************************/            
int	isInDebugMode(void)
{
	// Menu->special->repair
    
// @GC04.09.2011	
//	if( (globals->iloginlevel & 0x04) != 0 )        // Bit 2 gesetzt dann repairmode in trayMenu gesetzt
//		return(1);  // repair
//
//	if( globals->irepairmode != 0 )                 // @GC04.09.2011 neues Flag 
//		return(1);  // repair
		
	if( globals->debug_mode != 0 )                 // wird beim start von rte.exe gesetzt Argv3 muss "debug" sein 
		return(1);  // debug
		
	return(0);
}

/* FUNCTION ***************************************************/            
int	isInRepairMode(void)
{
	
	// Menu->specials->repair
	if( globals->irepairmode != 0 ) 
		return(1);     // repair Mode
		
	return(0);  //normal Mode
}












int  print_baco(void)
{
#ifdef	FUNKTEL		  // Daten für QIS 
/////////////////////new barcode 27.05.2003 kalich/////////////////////////		
char strbarcode[25];
char strbarcode_help[20];
char sigen;
char tempstring[140], temp_str[260];

char *ptemp = &tempstring[0];
int port_address ,i,j;
int port_value ;

	
	
	
	if(!globals->iprintmode) 
		return(0);				//if printing is disabled during  printout
	if(!strncmp(globals->sequ_select->printerport,"LPT",3))		//if LPT select, check for online (LPT1:0x378,LPT2:0x278)
		{
		port_address = 0x378;			//LPT1
		if(globals->sequ_select->printerport[3] == '2') 
			port_address = 0x278;		//LPT2
		if(globals->sequ_select->printerport[3] == '3') 
			port_address = 0x3bc;		//LPT3
		port_value = inp(port_address+1);
		i = 0;
		while(~port_value & 0x80)
			{
			port_value = inp(port_address+1);
			outp(port_address+1,(port_value &0xf7)); //printer select, clear bit3
			i++;
			if(i >= PRN_LOOPCOUNT)
				{
				if(ConfirmPopup ("Drucker Fehler","Druckerausgabe Fehler: Drucker ist aus oder ohne Papier"))
       				{//continue printing select
					if(!(inp (port_address+1) & 0x80)) //check online bit
						{									  
						MessagePopup ("Drucker Fehler","Drucker nicht vorhanden, Druckermode ist aus");
						globals->iprintmode = 0;	//set printmode off
						set_menu_printmode(globals->iprintmode);
						return(0);
						}
					}
				}
			}
		}
	
	
	
	i = 0;
	strbarcode_help[i] = 0x1b;
	i++;
	strbarcode_help[i] = 0x74;
	i++;
	strbarcode_help[i] = 0x0;
	sprintf(temp_str,"%s%c",strbarcode_help,0x0);
	for(j = 0; j <= i; j++)
		{
		outp (port_address, strbarcode_help[j]);
		if(printer_strobe (port_address))
				break;
		}

	//print_out(temp_str);	

	
	
	i = 0;
	strbarcode_help[i] = 0x1b;
	i++;
	strbarcode_help[i] = 0x52;
	i++;
	strbarcode_help[i] = 0x2;
	i++;
	strbarcode_help[i] = 0x0;
	sprintf(temp_str,"%s",strbarcode_help);
	for(j = 0; j <= i; j++)
		{
		outp (port_address, strbarcode_help[j]);
		if(printer_strobe (port_address))
				break;
		}
	//print_out(temp_str);	

////BREITE/////////
	i = 0;
	strbarcode_help[i] = 0x1d;
	i++;
	strbarcode_help[i] = 0x77;
	i++;
	strbarcode_help[i] = 0x3;
	i++;
////HÖHE/////////
	strbarcode_help[i] = 0x1d;
	i++;
	strbarcode_help[i] = 0x68;
	i++;
	strbarcode_help[i] = 0x20;
	i++;
////Text unter barcode/////////
	strbarcode_help[i] = 0x1d;
	i++;
	strbarcode_help[i] = 0x48;
	i++;
	strbarcode_help[i] = 0x2;
	i++;
////Text Font/////////
	strbarcode_help[i] = 0x1d;
	i++;
	strbarcode_help[i] = 0x66;
	i++;
	strbarcode_help[i] = 0x00;
	sprintf(temp_str,"%s%c",strbarcode_help,0x0);
	for(j = 0; j <= i; j++)
		{
		outp (port_address, strbarcode_help[j]);
		if(printer_strobe (port_address))
				break;
		}
	//print_out(temp_str);	


	i = 0;	
	strbarcode[i] = 0x1d;
	i++;
	strbarcode[i] = 0x6b;
	i++;
	strbarcode[i] = 0x5;
	i++;
	
	//cDateLabel = DateStr ();
	strbarcode[i] = cDateLabel[8]; //Jahr
	i++;
	strbarcode[i] = cDateLabel[9]; //Jahr
	i++;
	strbarcode[i] = cDateLabel[0]; //Monat
	i++;
	strbarcode[i] = cDateLabel[1]; //Monat
	i++;
	strbarcode[i] = cDateLabel[3]; //Tag
	i++;
	strbarcode[i] = cDateLabel[4]; //Tag
	i++;
	//cTimeLabel = TimeStr();
	//HH:MM:SS
	strbarcode[i] = cTimeLabel[0]; //Stunde
	i++;
	strbarcode[i] = cTimeLabel[1]; //Stunde
	i++;
	strbarcode[i] = cTimeLabel[3]; //Minute
	i++;
	strbarcode[i] = cTimeLabel[4]; //Minute
	i++;
	strbarcode[i] = cTimeLabel[6]; //Sekunden
	i++;
	strbarcode[i] = cTimeLabel[7]; //Sekunden
	i++;
	
	strbarcode[i] = strKennung[0]; 
	i++;
	strbarcode[i] = strKennung[1];
	i++;
	strbarcode[i] = strKennung[2];
	i++;
	strbarcode[i] = strKennung[3];
	i++;
	strbarcode[i] = strKennung[4];
	i++;
	strbarcode[i] = strKennung[5];			
	i++;		
	strbarcode[i] = 0x00;
	i++;		
	strbarcode[i] = 0x0a;
		
	sprintf(temp_str,"%s%c",strbarcode,0x0);
	for(j = 0; j <= i; j++)
		{
		outp (port_address, strbarcode[j]);
		if(printer_strobe (port_address))
				break;
		}
	//print_out(temp_str);	
	sprintf(temp_str,"\n\n\n\n\n\n");
	i=6;
	for(j = 0; j <= i; j++)
		{
		outp (port_address, temp_str[j]);
		if(printer_strobe (port_address))
				break;
		}
	//print_out(temp_str);	
	
	sprintf(temp_str,"%c%c",0x1b,0x6d);
	i=1;
	for(j = 0; j <= i; j++)
		{
		outp (port_address, temp_str[j]);
		if(printer_strobe (port_address))
				break;
		}
	//print_out(temp_str);	
#endif	
//////////////////////////////////////////////////////////////////////////////////////////////////////////////7
return(0);
}
