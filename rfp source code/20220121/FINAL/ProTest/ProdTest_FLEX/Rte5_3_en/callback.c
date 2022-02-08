#include "rte.h"


int CVICALLBACK timer_callb (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
// Precondition: user must set the right sequence
int temp;



	if (event == EVENT_TIMER_TICK)
		{
		
		ProcessSystemEvents();
		if(global.sequ_select == NULL) return(0);
		if(!global.sequ_select->flow) return(0);
		temp = (global.sequ_select->flow)(&test_flow);
		if(temp)
			{
			global.sequ_select->act_index = 0;
			testexecutive(SINGLEPASS);
			}
		}
	return 0;
}


//------------------------------------------------------

int CVICALLBACK sequ_selcallb (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
char temp_str[MAXSTRINGLEN];
char string[MAXSTRINGLEN];
int i,j,k;

	if ((event == EVENT_COMMIT)||(event == EVENT_LEFT_CLICK))			 // 3 for HOT
		{
		if(control == SEQU_SEL_OKAY)
			{
			GetCtrlVal (panel, SEQU_SEL_PRODUCTNUMBER, temp_str);
			i = strlen(temp_str);
			if(!i) return(1);
			
			GetNumListItems (panel, SEQU_SEL_SEQU_SELWIND, &j);
			while(j)
				{
				j--;
				GetLabelFromIndex (panel, SEQU_SEL_SEQU_SELWIND, j, string);
				i = strlen(string);
				for(k=0;k<i;k++)
					{
					if(string[k] == ';')
						{string[k]=0;break;}
					}
				k = strlen(string);
				i = strlen(temp_str);
				if(k != i) i = 0;			//if not equal, no match !!
				if(i)
					{
					if(!strncmp(temp_str,string,i))
						{
						SetCtrlIndex (panel, SEQU_SEL_SEQU_SELWIND, j);
						j = 0xff;
						break;
						}
					}
				}
			if(j == 0xff)
				{
				GetCtrlIndex (panel, SEQU_SEL_SEQU_SELWIND, &global.choice_sequ);
				global.choice_sequ++;											   //0 is an valid index
				}
			if(j != 0xff)
				{
				SetCtrlVal (panel,SEQU_SEL_STATUS , "\n Incorrect Number !!");
				SetCtrlAttribute (panel,SEQU_SEL_STATUS,ATTR_TEXT_BGCOLOR,COL_RED);
				}
			}
		if(control == SEQU_SEL_QUIT)
			global.choice_sequ = 0xffff;										   // set nothing to load (>1000)
		}
	return 0;
}

int CVICALLBACK sequ_selectwindow (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	if (event == EVENT_RIGHT_DOUBLE_CLICK) 
		{
		if(global.isequ_selectmode)	//select sequence with mouse not in dll-mode
			{
			GetCtrlIndex (panel, SEQU_SEL_SEQU_SELWIND, &global.choice_sequ);
			global.choice_sequ++;											   //0 is an valid index
			}
		}
	return 0;
}

//----------------------------------------------------------------------------------------------------------

int CVICALLBACK panel1 (int panel, int event, void *callbackData,
		int eventData1, int eventData2)
{
int i;

	if(event == EVENT_CLOSE)
		{
		if(global.sequ_select == NULL)
			{
//close com for modules, but hold the port number in memory !
			if(global.module_portB)
				CloseCom (global.module_portB);
			QuitUserInterface(0);
			return(0);
			}
		if(global.sequ_select->cleanup != NULL)
			(global.sequ_select->cleanup)(1);
		for(i=0;i<global.sequ_select->executeable_tests;i++)
			free(global.sequ_select->structpointer[i]);
		QuitUserInterface(0);
		exit(0);
		}
	return 0;
}


int CVICALLBACK panel1_callb (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
int index,temp;



	switch (control) 
		{
		case PANEL1_RUNLOOPL:				 // run endles
			if(event == EVENT_LEFT_DOUBLE_CLICK)
				{
				test_flow.runuut_left = 1;
				}
			break;
		case PANEL1_RUNSINGLEPL:				//single pass
			if(event == EVENT_LEFT_DOUBLE_CLICK)
				test_flow.start_left = 1;
			break;
		case PANEL1_RUNABORTL:			 //break run if run else clear sequence-status
			if(event == EVENT_COMMIT)
				{
				test_flow.ioerror_left = 0; //master break for runtime error (i/o) error
				if(global.iabort)
					{
					global.sequ_select = &left_sequ;
					fill_sequdisplay(1);
					}
				global.iabort = 1;
				test_flow.runuut_left = 0;
				test_flow.runuut_right = 0;
				}
			break;
		case PANEL1_RUNLOOPR:				 // run endles
			if(event == EVENT_LEFT_DOUBLE_CLICK)
				{
				test_flow.runuut_right = 1;
				}
			break;
		case PANEL1_RUNSINGLEPR:				//single pass
			if(event == EVENT_LEFT_DOUBLE_CLICK)
				test_flow.start_right = 1;
			break;
		case PANEL1_RUNABORTR:			 //break run if run else clear sequence-status
			if(event == EVENT_COMMIT)
				{
				test_flow.ioerror_right = 0; //master break for runtime error (i/o) error
				if(global.iabort)
					{
					global.sequ_select = &right_sequ;
					fill_sequdisplay(1);
					}
				global.iabort = 1;
				test_flow.runuut_right = 0;
				test_flow.runuut_left = 0;
				}
			break;
		case PANEL1_CLRRESULTD:
			if(event == EVENT_COMMIT)
				ResetTextBox (panel, PANEL1_RESULTDISPLAY, "");
			break;
		case PANEL1_CURRENTLOGIN:
		    if (event == EVENT_LEFT_DOUBLE_CLICK) 	
				InstallPopup(global.iloginpanel);
			break;
		}
	return 0;
}




int CVICALLBACK login_callb (int panel, int control, int event,void *callbackData, int eventData1, int eventData2)
{
char temp[20];
int itemp;


	
	if (event == EVENT_COMMIT)			  //taste okay | (LoginLevel (Hot-Mode) == Enter)
		{
		
		
		GetCtrlVal (panel, LOGIN_USERNAME, global.loginname);
		GetCtrlVal (panel, LOGIN_LOGINLEVEL, temp);
		if(strcmp(temp,DEVELOPERACCOUNT)) global.iloginlevel = 0;
		else	global.iloginlevel = 1;
        
        
		HidePanel (panel);  					 //needed for re-login
//		RemovePopup(global.iloginpanel);
		strcat(temp," / ");
		strcat(temp,global.loginname);
		SetCtrlVal (global.imainpanel, PANEL1_CURRENTLOGIN, temp);
   		itemp = GetPanelMenuBar (global.imainpanel);
	    if (global.iloginlevel)
	    	{
			//SetMenuBarAttribute (itemp, MENU1_SETS, ATTR_DIMMED, 0);					// @GC 17.02.2012
			SetMenuBarAttribute (itemp, MENU1_SETS_RDISP, ATTR_DIMMED, 0);           	// @GC 17.02.2012
			SetMenuBarAttribute (itemp, MENU1_SETS_SYSEVENT, ATTR_DIMMED, 0);			// @GC 17.02.2012
			SetMenuBarAttribute (itemp, MENU1_SETS_DATALOG, ATTR_DIMMED, 0);			// @GC 17.02.2012
			
			SetMenuBarAttribute (itemp, MENU1_FILE_EDITSEQUENCE, ATTR_DIMMED, 1);   //deactivated
			SetMenuBarAttribute (itemp, MENU1_LEFTSPEC, ATTR_DIMMED, 0);
			SetMenuBarAttribute (itemp, MENU1_RIGHTSPEC, ATTR_DIMMED, 0);
			SetMenuBarAttribute (itemp, MENU1_SPECIAL_DEBUG, ATTR_DIMMED, 0);       //activate @GC 14.09.2011 
			SetMenuBarAttribute (itemp, MENU1_SPECIAL_MAXERROR, ATTR_DIMMED, 0);
			
			SetCtrlAttribute (global.imainpanel,PANEL1_RUNSINGLEPR , ATTR_VISIBLE,1);
			SetCtrlAttribute (global.imainpanel,PANEL1_RUNSINGLEPL , ATTR_VISIBLE,1);
			SetCtrlAttribute (global.imainpanel,PANEL1_RUNLOOPR , ATTR_VISIBLE,1);
			SetCtrlAttribute (global.imainpanel,PANEL1_RUNLOOPL , ATTR_VISIBLE,1);
			}
		else
			{
			//SetMenuBarAttribute (itemp, MENU1_SETS, ATTR_DIMMED, 1);					// @GC 17.02.2012
			SetMenuBarAttribute (itemp, MENU1_SETS_RDISP, ATTR_DIMMED, 1);          	// @GC 17.02.2012
			SetMenuBarAttribute (itemp, MENU1_SETS_SYSEVENT, ATTR_DIMMED, 1);			// @GC 17.02.2012
			SetMenuBarAttribute (itemp, MENU1_SETS_DATALOG, ATTR_DIMMED, 1);			// @GC 17.02.2012

			SetMenuBarAttribute (itemp, MENU1_FILE_EDITSEQUENCE, ATTR_DIMMED, 1);
			SetMenuBarAttribute (itemp, MENU1_LEFTSPEC, ATTR_DIMMED, 1);
			SetMenuBarAttribute (itemp, MENU1_RIGHTSPEC, ATTR_DIMMED, 1);
			SetMenuBarAttribute (itemp, MENU1_SPECIAL_DEBUG, ATTR_DIMMED, 1);       //deactivate @GC 14.09.2011 
			SetMenuBarAttribute (itemp, MENU1_SPECIAL_MAXERROR, ATTR_DIMMED, 1);
			if(global.left)
				global.left->maxerror = global.left->maxerror_shadow;
			if(global.right)
				global.right->maxerror = global.right->maxerror_shadow;
			SetCtrlAttribute (global.imainpanel,PANEL1_RUNSINGLEPR , ATTR_VISIBLE,0);
			SetCtrlAttribute (global.imainpanel,PANEL1_RUNSINGLEPL , ATTR_VISIBLE,0);
			}
			
		}
	return 0;
}




/* FUNCTION ***************************************************/            
int CVICALLBACK displseqcallb (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
/*                                                                          
 SPECIFICATION:                                                             
		run single test (mouse-position) if double-click 		
***************************************************************/            
{
char temp[200];
int index,status,i;

    if (!global.iloginlevel)
    	return(0);


//    if ((event == 3) && (!global.ieditpanel)) 	//if editwindow, no test !!
    if ((event == EVENT_LEFT_DOUBLE_CLICK) && (!global.ieditpanel)) 	//if editwindow, no test !!
    	{
		if((control == PANEL1_SEQUENCEDISPLAYL ) && (!test_flow.running_left) && (!test_flow.running_right))
			{
		    if ((eventData2 > 100) || (!test_flow.hoodclosed_left))	//mouse position
    			return(0);
			global.sequ_select = &left_sequ;
			GetCtrlIndex (global.imainpanel, global.sequ_select->sequencdisp_handle, &global.sequ_select->act_index);
			ProcessDrawEvents ();
			testexecutive (SINGLETEST);
			return (0);
			}
		if((control == PANEL1_SEQUENCEDISPLAYR ) && (!test_flow.running_left) && (!test_flow.running_right))
			{
		    if ((eventData2 > 800)  || (!test_flow.hoodclosed_right))	//mouse position
    			return(0);
			global.sequ_select = &right_sequ;
			GetCtrlIndex (global.imainpanel, global.sequ_select->sequencdisp_handle, &global.sequ_select->act_index);
			ProcessDrawEvents ();
			testexecutive (SINGLETEST);
			return (0);
			}
    	}
/*		edit single test (mouse-position) if right double-click 		
***************************************************************/            
    if (event == EVENT_RIGHT_DOUBLE_CLICK) 	
    	{
		if(control == PANEL1_SEQUENCEDISPLAYR)
			global.sequ_select = &right_sequ;
		if(control == PANEL1_SEQUENCEDISPLAYL)
			global.sequ_select = &left_sequ;
    	if(!global.ieditpanel)
			GetCtrlIndex (global.imainpanel, global.sequ_select->sequencdisp_handle, &index); //position == global.sequ_select->structpointer[test]
		else			
			GetCtrlIndex (global.ieditpanel, EDITSEQU_SEQUENCEDISPLAY, &index); //position == global.sequ_select->structpointer[test]
		SetCtrlVal (global.iedittest, EDITTEST_INDEX,index);				//used in edit-callback
		SetCtrlVal (global.iedittest, EDITTEST_RUNMODE,global.sequ_select->structpointer[index]->runmode);
		SetCtrlVal (global.iedittest, EDITTEST_PRECOND,global.sequ_select->structpointer[index]->precond);
		SetCtrlVal (global.iedittest, EDITTEST_LOOPCOUNT,global.sequ_select->structpointer[index]->loopcount);
		SetCtrlVal (global.iedittest, EDITTEST_PRECISION,global.sequ_select->structpointer[index]->prec);
		SetCtrlVal (global.iedittest, EDITTEST_MESUNIT,global.sequ_select->structpointer[index]->unit);
		SetCtrlVal (global.iedittest, EDITTEST_NAME,global.sequ_select->structpointer[index]->username);
		sprintf(temp,"%s:%s",global.sequ_select->structpointer[index]->modulename,global.sequ_select->structpointer[index]->fkt_name);
		SetCtrlVal (global.iedittest, EDITTEST_MODULE,temp);
		if(global.sequ_select->structpointer[index]->string[0] != NULL)
			SetCtrlVal (global.iedittest, EDITTEST_STRING1,global.sequ_select->structpointer[index]->string[0]);
		if(global.sequ_select->structpointer[index]->string[1] != NULL)
			SetCtrlVal (global.iedittest, EDITTEST_STRING2,global.sequ_select->structpointer[index]->string[1]);
		if(global.sequ_select->structpointer[index]->string[2] != NULL)
			SetCtrlVal (global.iedittest, EDITTEST_STRING3,global.sequ_select->structpointer[index]->string[2]);
		if(global.sequ_select->structpointer[index]->string[3] != NULL)
			SetCtrlVal (global.iedittest, EDITTEST_STRING4,global.sequ_select->structpointer[index]->string[3]);
		if(global.sequ_select->structpointer[index]->errormessage != NULL)
			SetCtrlVal (global.iedittest, EDITTEST_ERRORMESSAGE,global.sequ_select->structpointer[index]->errormessage);

		SetCtrlVal (global.iedittest, EDITTEST_LOWLIMIT,global.sequ_select->structpointer[index]->lowerlimit);
		SetCtrlVal (global.iedittest, EDITTEST_UPLIMIT,global.sequ_select->structpointer[index]->upperlimit);
		SetCtrlVal (global.iedittest, EDITTEST_RESOFFSET,global.sequ_select->structpointer[index]->resultoffset);

		for(i=0;i<MAXPARAINT;i++)
			{
			sprintf(temp,"%8d               (%3d)",global.sequ_select->structpointer[index]->intpara[i],i);
    		ReplaceListItem (global.iedittest, EDITTEST_LISTINT,i, temp, i);
    		}
		for(i=0;i<MAXPARADOUBLE;i++)
			{
			sprintf(temp,"%6.6f          (%3d)",global.sequ_select->structpointer[index]->doublepara[i],i);
    		ReplaceListItem (global.iedittest, EDITTEST_LISTDBL,i, temp, i);
    		}
    	InstallPopup (global.iedittest);
    	}
     	
	return 0;
}


//*************** Single test Editor-Callbacks **************************************


int CVICALLBACK edit_callb (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
int index,i;
char temp[40],temp_str[MAXSTRINGLEN];
int ivalue;
double dvalue;
			

	if((event == EVENT_COMMIT) && (control == EDITTEST_GETNEW))
		{
		GetCtrlVal (panel, EDITTEST_INDEX, &index);		//get pointer to actual teststructure

		GetCtrlVal (global.iedittest, EDITTEST_RUNMODE,&global.sequ_select->structpointer[index]->runmode);
		GetCtrlVal (global.iedittest, EDITTEST_PRECOND,&global.sequ_select->structpointer[index]->precond);
		GetCtrlVal (global.iedittest, EDITTEST_LOOPCOUNT,&global.sequ_select->structpointer[index]->loopcount);
		GetCtrlVal (global.iedittest, EDITTEST_PRECISION,&global.sequ_select->structpointer[index]->prec);
		GetCtrlVal (global.iedittest, EDITTEST_MESUNIT,(char *)global.sequ_select->structpointer[index]->unit);
		GetCtrlVal (global.iedittest, EDITTEST_NAME,(char *)global.sequ_select->structpointer[index]->username);
//		GetCtrlVal (global.iedittest, EDITTEST_MODULE,(char *)global.sequ_select->structpointer[index]->modulename);
/*
		for(i=0;i<4;i++)
			if(global.sequ_select->structpointer[index]->string[0] != NULL) free(global.sequ_select->structpointer[index]->string[i]);
		if(global.sequ_select->structpointer[index]->errormessage != NULL) free(global.sequ_select->structpointer[index]->errormessage);
*/		
		GetCtrlVal (global.iedittest, EDITTEST_LOWLIMIT,&global.sequ_select->structpointer[index]->lowerlimit);
		GetCtrlVal (global.iedittest, EDITTEST_UPLIMIT,&global.sequ_select->structpointer[index]->upperlimit);
		GetCtrlVal (global.iedittest, EDITTEST_RESOFFSET,&global.sequ_select->structpointer[index]->resultoffset);

		GetCtrlVal (global.iedittest, EDITTEST_STRING1,temp_str);
		if((i=strlen(temp_str))>2)
			{
			global.sequ_select->structpointer[index]->string[0] = malloc(i+1);
			strcpy(global.sequ_select->structpointer[index]->string[0],temp_str);
			}
		GetCtrlVal (global.iedittest, EDITTEST_STRING2,temp_str);
		if(i=strlen(temp_str))
			{					
			global.sequ_select->structpointer[index]->string[1] = malloc(i+1);
			strcpy(global.sequ_select->structpointer[index]->string[1],temp_str);
			}
		GetCtrlVal (global.iedittest, EDITTEST_STRING3,temp_str);
		if(i=strlen(temp_str))
			{
			global.sequ_select->structpointer[index]->string[2] = malloc(i+1);
			strcpy(global.sequ_select->structpointer[index]->string[2],temp_str);
			}
		GetCtrlVal (global.iedittest, EDITTEST_STRING4,temp_str);
		if(i=strlen(temp_str))
			{
			global.sequ_select->structpointer[index]->string[3] = malloc(i+1);
			strcpy(global.sequ_select->structpointer[index]->string[3],temp_str);
			}
		GetCtrlVal (global.iedittest, EDITTEST_ERRORMESSAGE,temp_str);
		if(i=strlen(temp_str))
			{
			global.sequ_select->structpointer[index]->errormessage = malloc(i+1);
			strcpy(global.sequ_select->structpointer[index]->errormessage,temp_str);
			}

		if(global.ieditpanel)
			fill_sequ(index,2);
		else
			fill_sequdisplay(0x100+index); //update testname 
			
		for(i=0;i<MAXPARAINT;i++)
			{
			GetLabelFromIndex (panel, EDITTEST_LISTINT, i, temp);
			ivalue = atoi(temp);
			global.sequ_select->structpointer[index]->intpara[i] = ivalue;			
			}
		for(i=0;i<MAXPARADOUBLE;i++)
			{
			GetLabelFromIndex (panel, EDITTEST_LISTDBL, i, temp);
			dvalue = atof(temp);
			global.sequ_select->structpointer[index]->doublepara[i] = dvalue;			
    		}
		RemovePopup(0);
		return (0);
		}
	if((event == EVENT_COMMIT) && (control == EDITTEST_CANCEL))
		{
		RemovePopup(0);
		return (0);
		}

    if (event == EVENT_LEFT_DOUBLE_CLICK) 	
    	if(control == EDITTEST_LISTINT)
    		{
			GetCtrlIndex (panel, EDITTEST_LISTINT, &index);
			PromptPopup ("Edit Single Test Parameters", "Enter new Integer", temp,8);
			ivalue = atoi(temp);
			sprintf(temp,"%8d               (%3d)",ivalue,index);
    		ReplaceListItem (panel, EDITTEST_LISTINT,index, temp, index);
    		}
   if (event == EVENT_LEFT_DOUBLE_CLICK) 	
    	if(control == EDITTEST_LISTDBL)
    		{
			GetCtrlIndex (panel, EDITTEST_LISTDBL, &index);
			PromptPopup ("Edit Single Test Parameters", "Enter new Double", temp,12);
			dvalue = atof(temp);
			sprintf(temp,"%6.6f          (%3d)",dvalue,index);
    		ReplaceListItem (panel, EDITTEST_LISTDBL,index, temp, index);
    		}
	return 0;
}


//+++++++++++++++++ edit panel callbacks +++++++++++++
int CVICALLBACK edit_sequ (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
int i = 0,j,temp,x,index;
char pfad[MAXPATHNAMELEN],tempstr[MAXPATHNAMELEN];
char temp_char,dummy_fctname[MAXFUNCTIONNAMEL];
FILE *fa;

	if(global.sequ_select == &left_sequ)
		temp_char = 'l';
	else temp_char = 'r';

	switch (control) 
		{
		case EDITSEQU_CANCEL:
			if(event == EVENT_COMMIT)
				{
/*
				for(i=0;i<global.sequ_select->executeable_tests;i++)
					{
//					free(global.sequ_select->structpointer[i]->modulename);
					free(global.sequ_select->structpointer[i]);
					}
*/					
				RemovePopup (0);
				DiscardPanel(global.ieditpanel);
				global.ieditpanel = 0;		//merker
				get_sequence(global.sequ_select->sequ_pfad,temp_char,0); //komplett neu initialisieren
				fill_sequdisplay(0);	
				}
			break;
		case EDITSEQU_SAVE:			//write sequence to file
			if(event == EVENT_COMMIT)
				{
				i = FileSelectPopup ("", "*.squ", ".squ", "", VAL_OK_BUTTON, 0, 0, 1, 1,pfad);
				if(i<=0) break;
				if(i == 1)
					 if(!(ConfirmPopup ("Save Sequence File allready exist, overwrite ?", pfad)))
					 	break;
				fa = fopen(pfad,"w");
				fprintf(fa,"[global]");
				fprintf(fa,"\nworkdirectory = %s",global.sequ_select->user_dir);
				fprintf(fa,"\ndll_library = %s",global.sequ_select->dll_pfad);
				fprintf(fa,"\nstartupmodul = %s",global.sequ_select->startup_mod);
				fprintf(fa,"\nstartupfunction = %s",global.sequ_select->startup_fct);
				fprintf(fa,"\ncleanupmodul = %s",global.sequ_select->cleanup_mod);
				fprintf(fa,"\ncleanupfunction = %s",global.sequ_select->cleanup_fct);
				fprintf(fa,"\nflowmodul = %s",global.sequ_select->flow_mod);
				fprintf(fa,"\nflowfunction = %s",global.sequ_select->flow_fct);
				fprintf(fa,"\nioerrmodul = %s",global.sequ_select->ioerr_mod);
				fprintf(fa,"\nioerrfunction = %s",global.sequ_select->ioerr_fct);
				fprintf(fa,"\nprintport = %s",global.sequ_select->printerport);
				fprintf(fa,"\nlogfile = %s",global.sequ_select->log_file);
				fprintf(fa,"\nmaxerror = %d",global.sequ_select->maxerror);
				
				fprintf(fa,"\nreference = %d",global.ireference);
//menue functions:
				if(global.sequ_select == &left_sequ)
					{
					for(i=0;i<menue_functionL.function_count;i++)
						{
						fprintf(fa,"\nmenue_module = %s",menue_functionL.function_path[i]);
						fprintf(fa,"\nmenue_fktname = %s",menue_functionL.functionname[i]);
						}
					}
				else
					{
					for(i=0;i<menue_functionR.function_count;i++)
						{
						fprintf(fa,"\nmenue_module = %s",menue_functionR.function_path[i]);
						fprintf(fa,"\nmenue_fktname = %s",menue_functionR.functionname[i]);
						}
					}
//menue functions ready					
				fprintf(fa,"\n[end group]"); 
				for(index=0;index<global.sequ_select->executeable_tests;index++)
					{
					fprintf(fa,"\n\n[TEST]");
					strcpy(tempstr,global.sequ_select->structpointer[index]->modulename);
					fprintf(fa,"\nmodule = %s",tempstr);
					fprintf(fa,"\nfktname = %s",global.sequ_select->structpointer[index]->fkt_name);
					fprintf(fa,"\nusername = %s",global.sequ_select->structpointer[index]->username);
					fprintf(fa,"\nloopcount = %d",global.sequ_select->structpointer[index]->loopcount);
					fprintf(fa,"\nprecond = %d",global.sequ_select->structpointer[index]->precond);
					fprintf(fa,"\nrunmode = %d",global.sequ_select->structpointer[index]->runmode);
					fprintf(fa,"\nunit = %s",global.sequ_select->structpointer[index]->unit);
					fprintf(fa,"\npoints = %d",global.sequ_select->structpointer[index]->prec);
					if(global.sequ_select->structpointer[index]->errormessage != NULL)
						fprintf(fa,"\nerrormessage = %s",global.sequ_select->structpointer[index]->errormessage);
					fprintf(fa,"\nlowerlimit = %f",global.sequ_select->structpointer[index]->lowerlimit);
					fprintf(fa,"\nupperlimit = %f",global.sequ_select->structpointer[index]->upperlimit);
					fprintf(fa,"\nresultoffset = %f",global.sequ_select->structpointer[index]->resultoffset);
					for(i=0;i<MAXPARASTRING;i++)
						if(global.sequ_select->structpointer[index]->string[i] != NULL)
							fprintf(fa,"\nstring%d = %s",i+1,global.sequ_select->structpointer[index]->string[i]);
					for(i=0;i<MAXPARAINT;i++)
						{
						if(global.sequ_select->structpointer[index]->intpara[i])			
							fprintf(fa,"\nint%d = %d",i+1,global.sequ_select->structpointer[index]->intpara[i]);			
						}
					for(i=0;i<MAXPARADOUBLE;i++)
						{
						if(global.sequ_select->structpointer[index]->doublepara[i])			
							fprintf(fa,"\ndouble%d = %f",i+1,global.sequ_select->structpointer[index]->doublepara[i]);		
    					}
					fprintf(fa,"\n[end group]"); 
    				}
				fclose(fa);
				
				for(i=0;i<global.sequ_select->executeable_tests;i++)
					{
					free(global.sequ_select->structpointer[i]->modulename);
					free(global.sequ_select->structpointer[i]);
					}
					
				RemovePopup (0);
				DiscardPanel(global.ieditpanel);
				global.ieditpanel = 0;		//merker
					
				get_sequence(global.sequ_select->sequ_pfad,temp_char,1); //komplett neu initialisieren
				fill_sequdisplay(0);	
				}
			break;
		case EDITSEQU_REMOVE:			
			if((event == EVENT_COMMIT) || (event == EVENT_LEFT_CLICK))
				{
				GetCtrlIndex (global.ieditpanel, EDITSEQU_SEQUENCEDISPLAY, &temp);
				DeleteListItem (global.ieditpanel, EDITSEQU_SEQUENCEDISPLAY, temp, 1);
				free(global.sequ_select->structpointer[temp]);
				global.sequ_select->executeable_tests--;
				for(;temp<global.sequ_select->executeable_tests;temp++)
					{global.sequ_select->structpointer[temp] = global.sequ_select->structpointer[temp+1];}
				SetCtrlVal (global.ieditpanel, EDITSEQU_NUMTESTS,global.sequ_select->executeable_tests);
				}
			break;
		case EDITSEQU_INSERT:		//insert above	
			if((event == EVENT_COMMIT) || (event == EVENT_LEFT_CLICK))
				{
				GetCtrlIndex (global.ieditpanel, EDITSEQU_SEQUENCEDISPLAY, &temp);
				temp++;
				goto insert_test;
				}
			break;
		case EDITSEQU_INSERTB:		//insert below	
			if((event == EVENT_COMMIT) || (event == EVENT_LEFT_CLICK))
				{
				GetCtrlIndex (global.ieditpanel, EDITSEQU_SEQUENCEDISPLAY, &temp);
				SetCtrlIndex (global.ieditpanel, EDITSEQU_SEQUENCEDISPLAY, (temp+1));
insert_test:				
				
				i=global.sequ_select->executeable_tests;
				global.sequ_select->executeable_tests++;
				global.sequ_select->structpointer[i] = malloc(sizeof(struct struct_test));
				for(;i>temp;i--)
						global.sequ_select->structpointer[i+1] = global.sequ_select->structpointer[i];
				
				i = temp; //position to insert	
				GetCtrlIndex (global.ieditpanel, EDITSEQU_PROTOTYPES, &temp);
				GetLabelFromIndex (global.ieditpanel,EDITSEQU_PROTOTYPES, temp, pfad);
				temp = strlen(pfad);
				for(x=0;x<temp;x++)
					if((x>4) && (pfad[x] == ':'))	//no trigger first : !
						break;
				pfad[x] = 0;						
				strcpy(dummy_fctname,&pfad[x+1]);
				global.sequ_select->structpointer[i]->modulename = malloc((strlen(pfad)+1));
				strcpy(global.sequ_select->structpointer[i]->modulename,pfad);
				strcpy(global.sequ_select->structpointer[i]->fkt_name,&pfad[x+1]);
				global.sequ_select->structpointer[i]->runmode = 0;
				global.sequ_select->structpointer[i]->precond = 0;
				global.sequ_select->structpointer[i]->loopcount = 0;
				global.sequ_select->structpointer[i]->prec = 0;
				strcpy(global.sequ_select->structpointer[i]->unit,"?");
				
				strcpy(global.sequ_select->structpointer[i]->username,dummy_fctname);
				for(x=0;x<MAXPARAINT;x++)
					global.sequ_select->structpointer[i]->intpara[x] = 0;
				for(x=0;x<MAXPARADOUBLE;x++)
					global.sequ_select->structpointer[i]->doublepara[x] = 0;
				for(x=0;x<MAXPARASTRING;x++)
					global.sequ_select->structpointer[i]->string[x] = NULL;
				global.sequ_select->structpointer[i]->errormessage = 0;
				global.sequ_select->structpointer[i]->upperlimit = 0;
				global.sequ_select->structpointer[i]->lowerlimit = 0;
				global.sequ_select->structpointer[i]->resultoffset = 0;

				SetCtrlVal (global.ieditpanel, EDITSEQU_NUMTESTS,global.sequ_select->executeable_tests);
				InsertListItem (global.ieditpanel, EDITSEQU_SEQUENCEDISPLAY, i,dummy_fctname,i);
				}		
			break;
		case EDITSEQU_REMHEAD:			
			if((event == EVENT_COMMIT) || (event == EVENT_LEFT_CLICK))
				{
				GetCtrlIndex (global.ieditpanel, EDITSEQU_MODULES, &temp);
				if(temp >= 0)
					{
					GetLabelFromIndex (global.ieditpanel,EDITSEQU_MODULES, temp, pfad);
					i = strlen(pfad);
					GetNumListItems (global.ieditpanel, EDITSEQU_PROTOTYPES, &x);
					x--;
					for(;x>=0;x--)
						{
						GetLabelFromIndex (global.ieditpanel,EDITSEQU_PROTOTYPES, x, tempstr);
						if(!(strncmp(pfad,tempstr,i)))
							{
							DeleteListItem (global.ieditpanel, EDITSEQU_PROTOTYPES, x,1);
							}
						}
					DeleteListItem (global.ieditpanel, EDITSEQU_MODULES, temp,1);
					}
				}
			break;
		case EDITSEQU_ADDHEAD:			
			if((event == EVENT_COMMIT) || (event == EVENT_LEFT_CLICK))
				{
				if(FileSelectPopup ("", "*.c", "*.h", "Load Source or Header with Function", VAL_LOAD_BUTTON, 0, 0, 1, 0, pfad))
					{
//					i = strlen(pfad);
//					for(x=0;x<i;x++)
//						pfad[x] = tolower (pfad[x]);
					GetNumListItems (global.ieditpanel, EDITSEQU_MODULES, &temp);
					x = --temp;
					for(;x>=0;x--)
						{
						GetLabelFromIndex (global.ieditpanel,EDITSEQU_MODULES, x, tempstr);
						if(strncmp(pfad,tempstr,i))
							temp--;
						}
					if(temp)
						{
						i = strlen(pfad);
						while(i)
							{
							if(pfad[i] == '.')
								{
								pfad[i] = 0;
								break;
								}
							i--;
							}
						InsertListItem(global.ieditpanel, EDITSEQU_MODULES, -1, pfad,-1);
						fill_proto(pfad);
						}
					}
				}
			break;
		}
		
	return 0;
}



int CVICALLBACK edit_sequglob (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
int handle;

	if((event == EVENT_COMMIT) || (event == EVENT_LEFT_DOUBLE_CLICK))
		{
		if ((handle = LoadPanel (0, uir_path, SEQU_HEAD)) < 0)
			return -1;

		SetCtrlVal (handle, SEQU_HEAD_SEQNAME, global.sequ_select->sequ_pfad);
		SetCtrlVal (handle, SEQU_HEAD_WORKDIR, global.sequ_select->user_dir);
		SetCtrlVal (handle, SEQU_HEAD_DLL_LIB, global.sequ_select->dll_pfad);
		SetCtrlVal (handle, SEQU_HEAD_STARTUP, global.sequ_select->startup_mod);
		SetCtrlVal (handle, SEQU_HEAD_STARTFCT, global.sequ_select->startup_fct);
		SetCtrlVal (handle, SEQU_HEAD_CLEANUP, global.sequ_select->cleanup_mod);
		SetCtrlVal (handle, SEQU_HEAD_CLEANFCT, global.sequ_select->cleanup_fct);
		SetCtrlVal (handle, SEQU_HEAD_FLOW, global.sequ_select->flow_mod);
		SetCtrlVal (handle, SEQU_HEAD_FLOWFCT, global.sequ_select->flow_fct);
		SetCtrlVal (handle, SEQU_HEAD_IOERR, global.sequ_select->ioerr_mod);
		SetCtrlVal (handle, SEQU_HEAD_IOERRFCT, global.sequ_select->ioerr_fct);
		SetCtrlVal (handle, SEQU_HEAD_FIX_ID, global.sequ_select->fixtureID);
		SetCtrlVal (handle, SEQU_HEAD_PRINTPORT, global.sequ_select->printerport);
		SetCtrlVal (handle, SEQU_HEAD_LOGFILE, global.sequ_select->log_file);
		SetCtrlVal (handle, SEQU_HEAD_MAXERROR, global.sequ_select->maxerror);
		InstallPopup(handle);
		}
	return 0;
}

//+++++++++++++++++ edit sequence header panel callbacks +++++++++++++
int CVICALLBACK edit_sequhead (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	if((event == EVENT_COMMIT) || (event == EVENT_LEFT_DOUBLE_CLICK))
		{
		if(control == SEQU_HEAD_GETNEW)
			{
			GetCtrlVal (panel, SEQU_HEAD_WORKDIR, global.sequ_select->user_dir);
			GetCtrlVal (panel, SEQU_HEAD_DLL_LIB, global.sequ_select->dll_pfad);
			GetCtrlVal (panel, SEQU_HEAD_STARTUP, global.sequ_select->startup_mod);
			GetCtrlVal (panel, SEQU_HEAD_STARTFCT, global.sequ_select->startup_fct);
			GetCtrlVal (panel, SEQU_HEAD_CLEANUP, global.sequ_select->cleanup_mod);
			GetCtrlVal (panel, SEQU_HEAD_CLEANFCT, global.sequ_select->cleanup_fct);
			GetCtrlVal (panel, SEQU_HEAD_FLOW, global.sequ_select->flow_mod);
			GetCtrlVal (panel, SEQU_HEAD_FLOWFCT, global.sequ_select->flow_fct);
			GetCtrlVal (panel, SEQU_HEAD_IOERR, global.sequ_select->ioerr_mod);
			GetCtrlVal (panel, SEQU_HEAD_IOERRFCT, global.sequ_select->ioerr_fct);
			GetCtrlVal (panel, SEQU_HEAD_FIX_ID, global.sequ_select->fixtureID);
			GetCtrlVal (panel, SEQU_HEAD_PRINTPORT, global.sequ_select->printerport);
			GetCtrlVal (panel, SEQU_HEAD_LOGFILE, global.sequ_select->log_file);
			GetCtrlVal (panel, SEQU_HEAD_MAXERROR, &global.sequ_select->maxerror);
			}
		DiscardPanel (panel);
//		RemovePopup(handle);
		}
	return 0;
}

