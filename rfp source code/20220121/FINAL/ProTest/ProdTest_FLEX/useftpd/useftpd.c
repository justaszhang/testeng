//==============================================================================
//
// Title:       useftpd.c
// Purpose:     
//
// Created on:  11.12.2011 by G.Cordes
// Copyright:   . All Rights Reserved.
//
// das Programm ftpdmin.exe wird im hintergrund gestartet und gestopt
//
//==============================================================================

//==============================================================================
// Include files
#include <windows.h>
#include <winsock.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <userint.h>
#include <utility.h>
#include <ansi_c.h>


//----------------------------------------------------------------
#if(_CVI_ < 710)   // kleiner CVI 7.1

// cvintwrk.dll von cvi2009 kopiert

#ifndef _SIZE_T_DEFINED
#define _SIZE_T_DEFINED
typedef unsigned int size_t;
#endif

#ifndef _SSIZE_T_DEFINED
#define _SSIZE_T_DEFINED
typedef int ssize_t;
#endif


#include "cvintwrk.h"

#else

// cvintwrk.dll ist ab CVI7.1 vorhanden

#include <cvintwrk.h>       // cvintwrk.h erst ab CVI7.1

#endif
//----------------------------------------------------------------





//==============================================================================
// Global variables
int useftpd_debug = 0;		// 0=no output, 1=error output   on stderr or on MessagePupup



//==============================================================================
// Static global variables

static int ftpd_handle=0;

static char *ftpdeamon[]  = {"ftpdmin.exe", "ftpdmin\\ftpdmin.exe", NULL};
	

//==============================================================================
// Constants




//==============================================================================
// Local functions

int isFileExists(char *filename)
{
	FILE *fp;
	
	// check if exists
	if ( (fp=fopen(filename,"r")) == NULL )
	{	
		return(0);    // no
	}	
	fclose(fp);
	return(1);   // yes
}	




//==============================================================================
// Global functions


/*==============================================================================
**
**  startFtpDeamon(char *ipAddr, char *rootPath)   
**
**  return:
**	=  0  == ok
**  <> 0  == fail
**
*/

int startFtpDeamon(char *ipAddr,char *rootPath)
{
	
	//int ret;
	int  timeOut;
	int  i;
	char *p;

	char tempstr[512];
	char strCmdExe[256];
	
	char WindowsDir[128];
	char SystemDir[128];


	
	
	for (i=0 ; ftpdeamon[i] != NULL ; i++ )
	{
		sprintf(strCmdExe,"%s",ftpdeamon[i]);
		if ( isFileExists(strCmdExe) )
		{	
			break;  //found
		}	
			
		if ( (p=getenv ("ProgramFiles")) != NULL )            // "C:\Programme"  oder "C:\Program Files"
		{
			sprintf(strCmdExe,"%s\\%s",p,ftpdeamon[i]);
			if ( isFileExists(strCmdExe) )                  // "C:\Program Files\ftpdmin\ftpdmin.exe"
			{
				break;	  //found
			}	
		}	
	}	
	
	if ( ftpdeamon[i] == NULL )
	{
		if ( useftpd_debug != 0 )
		{	
	 		sprintf(tempstr,"can't find file: %s",strCmdExe);
#ifdef _CONSOLE
			fprintf(stderr,"%s\n",tempstr);
#else
			MessagePopup("Error",tempstr);
#endif		
		}	
		return(1);    // error
	}	
	
					


	// "ftpdmin.exe -ha 192.168.99.2 c:\temp"
	sprintf(tempstr,"\"%s\" -ha %s \"%s\"", strCmdExe, ipAddr, rootPath);

	//LE_SHOWNORMAL
	//LE_HIDE
	if ( LaunchExecutableEx (tempstr, LE_HIDE ,&ftpd_handle ) )
	{
		RetireExecutableHandle (ftpd_handle);
		if ( useftpd_debug != 0 )
		{	
			sprintf(tempstr,"can't execute: %s",strCmdExe);
#ifdef _CONSOLE
			fprintf(stderr,"%s\n",tempstr);
#else
			MessagePopup("Error",tempstr);
#endif		
		}	
		return(1);     // error
	}		
	

	

	// warten bis .exe gestartet ist 
	// timeOut = 3Sec
	timeOut = 3000 / 500; 
	do
	{
		Sleep(500);      // 500ms   (1/2) Sec          
		timeOut--;
	}	
	while( (ExecutableHasTerminated(ftpd_handle) != 0)  && (timeOut > 0) );

	
	// .exe noch nicht gestartet
	// oder vorzeitig beendet
	if( ExecutableHasTerminated(ftpd_handle) != 0 ) 
	{
		// handle freigeben
		RetireExecutableHandle (ftpd_handle);
		if ( useftpd_debug != 0 )
		{	
	 		sprintf(tempstr,"can't start %s",strCmdExe);
#ifdef _CONSOLE
			fprintf(stderr,"%s\n",tempstr);
#else
			MessagePopup("Error",tempstr);
#endif		
		}	
		return(2);    // error

	}
	
	
	return(0); // ok

}	

/*==============================================================================
**
**  stopFtpDeamon(void)   
**
**  return:
**	=  0  == ok
**  <> 0  == fail
**
*/

int stopFtpDeamon(void)
{
	int timeOut;

	char tempstr[512];
	
	
	
	// noch nicht beendet
	if( ExecutableHasTerminated(ftpd_handle) == 0 ) 
	{
		TerminateExecutable(ftpd_handle);

		//warten bis exe terminated
		// timeOut = 3Sec
		timeOut = 3000 / 250; 
		while( (ExecutableHasTerminated(ftpd_handle) == 0)  && (timeOut > 0) )
		{
			Sleep(250);      // 250ms   (1/4) Sec
			timeOut--;
		}		
	}	
	
	// noch nicht beendet
	if( ExecutableHasTerminated(ftpd_handle) == 0 ) 
	{
		if ( useftpd_debug != 0 )
		{	
			sprintf(tempstr,"can't stop: %s","ftpdeamon");
#ifdef _CONSOLE
			fprintf(stderr,"%s\n",tempstr);
#else
			MessagePopup("Error",tempstr);
#endif		
		}	
		return(1);    // error

	}

		

	return(0); // ok
	

}	

/*==============================================================================
**
**  isFtpDeamonRunning(char *ipAddr)   
**
**  check if FTP Deamon is Running
**
**  return:
**	=  0  == not
**  =  1  == running
**
*/

int isFtpDeamonRunning(char *server)
{

	int error;

	int ftpclient_handle = -1;

	int passiveMode = 1;

	char *username = "none";
	char *password = "none";
	
	
	char    dir[256];
	
	char    tempstr[512];
	
    // Login to FTP site and set passive mode
    
	// int InetFTPLoginEx (const char *FTP_Server, const char *User_Name, const char *Password, int Timeout);
	
    if (  (ftpclient_handle = InetFTPLogin (server, username, password)) < 0 ) 
    {
		if ( useftpd_debug != 0 )
		{	
#ifdef _CONSOLE
	 		sprintf(tempstr,"can't connect to server: %s", server);
			fprintf(stderr,"%s\n",tempstr);
#endif		
		}
        return(0);
    }

		
	InetFTPSetPassiveMode (ftpclient_handle, passiveMode);
	
	
    // Get current remote directory and its contents
    if ( (error=InetFTPGetDir (ftpclient_handle, dir, sizeof(dir)-1 )) < 0 )
    {
		if ( useftpd_debug != 0 )
		{	
#ifdef _CONSOLE
	 		sprintf(tempstr,"can't get remote directory: %s\nERROR %d: %s", server,error,InetGetErrorMessage(error));
			fprintf(stderr,"%s\n",tempstr);
#endif		
		}
        return(0);
    }
		
	if (ftpclient_handle >= 0)
	{	
	    if ( (error = InetFTPClose (ftpclient_handle)) < 0)
		{	
			if ( useftpd_debug != 0 )
			{	
#ifdef _CONSOLE
	 			sprintf(tempstr,"can't close remote connection: %s\nERROR %d: %s", server,error,InetGetErrorMessage(error));
				fprintf(stderr,"%s\n",tempstr);
#endif		
			}
        	return(0);
		}
	}	
	
	
	return(1);	// ok  server is running
}	




/*==============================================================================
**
**  checkFtpDeamon(char *ipAddr, int timeOut)   
**
**  warte bis FTP Deamon am laeuft
**
**  return:
**	=  0  == ok
**  <> 0  == fail
**
*/
int checkFtpDeamon(char *server, int timeOut)   // timeOut in ms
{
	
	do		
	{
		// check if running
		if ( isFtpDeamonRunning(server) ) return(0); // ok  running
		
		Sleep(200);      // 200ms
		timeOut=timeOut-200;
	}
	while ( timeOut > 0 );   

	return(1); // fail server not running
	
}	


