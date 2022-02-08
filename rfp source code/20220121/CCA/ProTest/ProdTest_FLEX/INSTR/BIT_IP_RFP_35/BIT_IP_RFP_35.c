#include <windows.h>
#include <winsock.h>    
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <userint.h>
#include <utility.h>
#include <ansi_c.h>
#include <errno.h>
#include <string.h>
#include <rs232.h>
#include <tcpsupp.h>    


//----------------------------------------------------------------
#if(_CVI_ < 710)   // kleiner CVI 7.1

// cvintwrk.dll von cvi2009 kopiert

	    // in CVI 6.0 ist kein ping vorhanden
	    // mit cvi2009 cvintwrk.dll ist auch ping möglich

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

#define BIT_IP_RFP35_DEBUG_RSX 	    "c:\\temp\\rsx.log"		  	   // Debug erzeugt RSX Logs
#define BIT_IP_RFP35_DEBUG_RSX_TIME                                // Debug erzeugt Zeit Messung in RSX Logs
#define BIT_IP_RFP35_DEBUG_RS232    "c:\\temp\\rs232.log"		   // Debug erzeugt RS232 Logs
#define BIT_IP_RFP35_DEBUG_SSH      "c:\\temp\\ssh.log"			   // Debug erzeugt ssh Logs

#include "uselibssh.h"

#include "BIT_IP_RFP_35.h"   

#include "rfp35_eeprom_01.h"

#include "test.h"
unsigned char gResponse[400000];
int Print2StandardIO;

// Socketvariable, die nach erfolgreichem Verbindungsaufbau einen Wert ungleich
// INVALID_SOCKET hat, und bei jeder weiteren Socketoperation als Handle genutzt wird
SOCKET sock = INVALID_SOCKET;
// Flag, das anzeigt ob eine Verbindung zum IP-RFP besteht
BYTE bIsConnected = NOT_CONNECTED;


static char *sshUser = {"omm"};		      // omm    Omm
static char *sshPasswd = {"omm"};			  // omm    Omm

static char *rootUser = {"root"};
static char *rootPasswd = {"22222"};

static int rsxPort = 38477;

static char ipAddrForReopen[80] = {""};

static struct mail_para
	{
	unsigned char  task_ID;
	unsigned short primitiv;
	unsigned char  pID;			//programm-ID
	unsigned char  eeprom_write_blocksize;
	unsigned char  eeprom_read_blocksize;
	}mail_para;




void stringToUp(char *p)
{
	for ( ; *p != '\0' ; p++ )
	{
		*p=toupper(*p);	
	}
}	

void stringToLow(char *p)
{
	for ( ; *p != '\0' ; p++ )
	{
		*p=tolower(*p);	
	}
}	


//==============================================================================
/*
**   ipRfp_clearTempFiles()
**
**   loescht alle Temporary Files
*/

int ipRfp_clearTempFiles(void)
{

	
#ifdef BIT_IP_RFP35_DEBUG_RS232
		remove(BIT_IP_RFP35_DEBUG_RS232);
#endif

#ifdef BIT_IP_RFP35_DEBUG_SSH
		remove(BIT_IP_RFP35_DEBUG_SSH);
#endif

#ifdef BIT_IP_RFP35_DEBUG_RSX
		remove(BIT_IP_RFP35_DEBUG_RSX);
#endif
	
	return(0);  
}





//==============================================================================
/*
**	startet openocd.exe
**  Retcode <> 0:error
**  RetCode 0   :ok
**
**  c:\Programme\openocd\0.4.0\bin\openOcd.exe"
**  c:\Program Files\openocd\0.4.0\bin\openOcd.exe"
**
*/


int ipRfp_runOpenOCD(char *openocdScripts, char *respBuffer, int respSize)		   // openOCD Script path
{
	int handle;
	int ret;
	int timeOut;

	char tempstr[512];
	
	char response[4096];
	char *p;
	
	
	FILE *fp;
	char *strRespFile = "c:\\temp\\openocd_resp.log";
	char *strErrFile =  "c:\\temp\\openocd_err.log";

	char strCmdExe[256];


	if ( respBuffer != NULL && respSize > 0 ) *respBuffer = '\0';
	
		
	if ( (p=getenv ("ProgramFiles")) == NULL )            // "C:\Programme"  oder "C:\Program Files"
	{
	 	sprintf(tempstr,"can't not find environment: %s","ProgramFiles");
		MessagePopup("Error",tempstr);
		return(-1);    // error
	}
		
	// check if exists
	//C:\Program Files\OpenOCD\0.4.0\bin\openocd.exe
	sprintf(strCmdExe,"%s%s",p ,"\\OpenOCD\\0.4.0\\bin\\openocd.exe");
	if ( (fp=fopen(strCmdExe,"r")) == NULL )
	{	
	 	sprintf(tempstr,"can't find file: %s",strCmdExe);
		MessagePopup("Error",tempstr);
		return(-2);    // error
	}	
	fclose(fp);
		

	// response und error loeschen
	(void)remove(strRespFile);
	(void)remove(strErrFile);
	



/*	
**	Promt-Screen: Run openocd ( execute openocd.exe with t,he following parameter:
**  openocd.exe -s e:\openocd-scripts -f board\rfp43-aastra.cfg -c init -c rfp43_load_uboot -c exit
*/
	

	// Achtung: keine Leerzeichen im Script Path
	// cmd.exe /c openocd.exe -s e:\openocd-scripts -f board\rfp43-aastra.cfg -c init -c rfp43_load_uboot -c exit 2>err.txt >resp.txt
	// mit option /s auch Leerzeichen möglich
	// cmd.exe /s /c ""openocd.exe" -s "e:\openocd-scripts" -f "board\rfp43-aastra.cfg" -c init -c "rfp43_load_uboot" -c "exit"" 2>err.txt >resp.txt
	sprintf(tempstr,"cmd.exe /s /c \"\"%s\" -s \"%s\" -f board\\rfp43-aastra.cfg -c init -c rfp43_load_uboot -c exit\" 2>%s >%s", strCmdExe, openocdScripts, strErrFile,strRespFile);

	/*
	MessagePopup("wait",tempstr);
	*/
	
	
	//LE_SHOWNORMAL
	//LE_HIDE
	if ( LaunchExecutableEx (tempstr, LE_HIDE ,&handle ) )
	{
		RetireExecutableHandle (handle);
		sprintf(tempstr,"can't execute: %s",tempstr);
		MessagePopup("Error",tempstr);
		return(-3);     // error
	}		

	//warten bis exe zurück kommt
	//timeOut 30 Sec
	timeOut = 30 * 1000/250; 
	while( (ExecutableHasTerminated(handle) == 0)  && (timeOut > 0) )
	{
		//fprintf(stderr,"Warte\n");
		Sleep(250);      // 250ms   the better way
		timeOut--;
	}		
	
	// noch nicht beendet
	if( ExecutableHasTerminated(handle) == 0 ) 
	{
		// nur die cmd kann gekillt werden, die openocd.exe laeuft weiter
		//fprintf(stderr,"kill\n");
		TerminateExecutable(handle);
		// handle freigeben
		RetireExecutableHandle (handle);
		return(-4);    // error

	}
	RetireExecutableHandle (handle);   // handle freigeben
	

	// error lesen
	if ( (fp=fopen(strErrFile,"r")) == NULL )
	{	
 		sprintf(tempstr,"can't open file: %s",strErrFile);
		MessagePopup("Error",tempstr);
		return(-5);    // error
	}	
	ret=fread(&response[0],1,sizeof(response)-1,fp);
	response[ret]='\0';
	fclose(fp);
		

	// response lesen
	if ( (fp=fopen(strRespFile,"r")) == NULL )
	{	
		sprintf(tempstr,"can't open file: %s",strRespFile);
		MessagePopup("Error",tempstr);
		return(-6);
	}	
	ret = ret + fread(&response[ret],1,sizeof(response)-ret-1,fp);
	response[ret]='\0';
	fclose(fp);


	if ( respBuffer != NULL && respSize > 0 )
	{	
		strncpy(respBuffer,response,respSize);
		*(respBuffer+respSize-1)='\0';
	}	

	
	if ( (p=strstr(response,"Error:")) != NULL )
	{	
		return(-7);    //error
	}
	
	if ( strstr(response,"booter image OK") != NULL ) 
	{
		return(0);   //ok	
	}	
	
	
	return(-8);    //error

}	


//==============================================================================
/*
**   Test ob Device vorhanden ist und sich meldet
**   return(1) = vorhanden
*/

int ipRfp_SystemPing(char *ipAddr , int timeOut)		   // timeOut in ms
{

	int ret,available;
	int handle;

#ifdef _CVINTWRK_H	                             // cvintwrk.h vorhanden also auch InetPing()
	
	if ( timeOut < 1000) timeOut=1000;
	ret= InetPing (ipAddr, &available, timeOut);

	if ( ret == 0 && available != 0 )
		return(1);            // available 
	
#else								            // cvintwrk.h nicht vorhanden also auch kein ping
	
	// dies nur für CVI 6.0 dort ist kein ping vorhanden
	
	char tempstr[512];
	FILE *fp;
	char *strRespFile = "c:\\temp\\ping_resp.log";
	
	(void)remove(strRespFile);

	
	if ( timeOut < 500) timeOut=500;
	//ping kommando liegt c:\windows\system32\ping.exe
	sprintf(tempstr,"cmd /c ping.exe -n 1 -w %d %s >Nul && echo PING_OK >%s",timeOut, ipAddr,strRespFile);
	// system kommt immer mit 0 zurück also ok
	//fprintf(stderr,"%s\n",tempstr);
	//LE_SHOWNORMAL
	//LE_HIDE
	if ( LaunchExecutableEx (tempstr, LE_HIDE ,&handle ) )
	{
		RetireExecutableHandle (handle);
 		sprintf(tempstr,"can't execute: %s",tempstr);
		MessagePopup("Error",tempstr);
		return(0);     // error
	}		

	//warten bis exe zurück kommt
	timeOut = (timeOut/250) + (3000/250);    // 3Sek laenger
	while( (ExecutableHasTerminated(handle) == 0)  && (timeOut > 0) )
	{
		//fprintf(stderr,"Warte\n");
		Sleep(250);      // 250ms   the better way
		timeOut--;
	}		
	
	// noch nicht beendet
	if( ExecutableHasTerminated(handle) == 0 ) 
	{
		// nur die cmd kann gekillt werden, die plink.exe laeuft weiter
		//fprintf(stderr,"kill\n");
		TerminateExecutable(handle);
		// handle freigeben
		RetireExecutableHandle (handle);
		return(0);    // error

	}
	RetireExecutableHandle (handle);

	
	
	available=0;
	if ( (fp=fopen(strRespFile,"r")) != NULL )
	{	
		available=0;
		while ( fgets(tempstr,sizeof(tempstr),fp) )
		{
			// PING: Übertragung fehlgeschlagen. 
			
			if ( strstr(tempstr,"PING_OK") != NULL )
			{	
				available=1;   // vorhanden
			}
			
			
		}
		fclose(fp);
		remove (strRespFile);
	}
	if ( available != 0 )
		return(1);            // available 
		
	
#endif //_CVINTWRK_H	
	
	return(0);   // not available
}


//==============================================================================
/*
**   ipRfp_getAdapterIpAddr()
**
**   suche den Ethernet Adapter mit Namen xxx und gebe die IpAddr zurueck
**   return(0) = ok gefunden
*/

int ipRfp_getAdapterIpAddr(char *adapterName, char *adapterIpAddr)
{

	int handle;
	int timeOut;
	int available=0;
	int rc=1;         // fail or not found
	char *p;

	
	char tempstr[512];
	char sAdapter[256];
	FILE *fp;
	char *strRespFile = "c:\\temp\\ipconfig_resp.log";

	if ( adapterIpAddr != NULL ) strcpy(adapterIpAddr,"0.0.0.0");

	
	(void)remove(strRespFile);

	
	sprintf(sAdapter," %s:",adapterName);
	for (p=sAdapter ; *p != '\0' ; p++ ) *p=tolower(*p);	 
	
	
	
	//  ipconfig /all
	//  
	
	sprintf(tempstr,"cmd /c ipconfig.exe /all 2>Nul >%s",strRespFile);
	//fprintf(stderr,"%s\n",tempstr);
	//LE_SHOWNORMAL
	//LE_HIDE
	if ( LaunchExecutableEx (tempstr, LE_HIDE ,&handle ) )
	{
		RetireExecutableHandle (handle);
 		sprintf(tempstr,"can't execute: %s",tempstr);
		MessagePopup("Error",tempstr);
		return(rc);     // error
	}		

	//warten bis exe zurück kommt
	timeOut = ((10*1000)/250);    // 10Sek
	while( (ExecutableHasTerminated(handle) == 0)  && (timeOut > 0) )
	{
		//fprintf(stderr,"Warte\n");
		Sleep(250);      // 250ms   the better way
		timeOut--;
	}		
	
	// noch nicht beendet
	if( ExecutableHasTerminated(handle) == 0 ) 
	{
		// nur die cmd kann gekillt werden, die plink.exe laeuft weiter
		//fprintf(stderr,"kill\n");
		TerminateExecutable(handle);
		// handle freigeben
		RetireExecutableHandle (handle);
		return(rc);    // error

	}
	RetireExecutableHandle (handle);
	
	
	available=0;
	if ( (fp=fopen(strRespFile,"r")) != NULL )
	{	
		while ( fgets(tempstr,sizeof(tempstr),fp) )
		{
			for (p=tempstr ; *p != '\0' ; p++ ) *p=tolower(*p);	 
			
			for (p=tempstr ; *p != '\0' ; p++ ) if ( *p == '\r' || *p == '\n') *p='\0';	 
	
			if ( strlen(tempstr) == 0 ) continue;
			
		//Ethernetadapter LAN-Verbindung:
        //	Verbindungsspezifisches DNS-Suffix: WRVS4400Nv2
        //	Beschreibung. . . . . . . . . . . : Realtek RTL8168C(P)/8111C(P) PCI-E Gigabit Ethernet NIC
        //	Physikalische Adresse . . . . . . : 00-1F-D0-23-94-63
        //	DHCP aktiviert. . . . . . . . . . : Ja
        //	Autokonfiguration aktiviert . . . : Ja
        //	IP-Adresse. . . . . . . . . . . . : 10.100.1.1
        //	Subnetzmaske. . . . . . . . . . . : 255.255.255.0
        //	Standardgateway . . . . . . . . . : 10.100.1.128
        //	DHCP-Server . . . . . . . . . . . : 10.100.1.128
        //	DNS-Server. . . . . . . . . . . . : 10.101.1.128
        //	Lease erhalten. . . . . . . . . . : Dienstag, 17. Januar 2012 17:13:28
        //	Lease läuft ab. . . . . . . . . . : Mittwoch, 18. Januar 2012 17:13:28
		//
			if ( strncmp(tempstr,"\t",1) != 0 && strncmp(tempstr,"  ",2) != 0 ) 
			{	
				//Ethernetadapter LAN-Verbindung:  
				//Ethernet adapter
				//Server Adapter
				available=0;   // ende Adapter Bereich
				if ( strstr(tempstr,sAdapter) != NULL )        // " Testnetz:"
				{	
					//fprintf(stderr,tempstr);
					available=1;   // start Testnetz
				}
			}
			if ( available != 0 )
			{	
				if ( strstr(tempstr,"ip-adresse") != NULL || strstr(tempstr,"ip address") != NULL  )
				{	
					available=0;
					if ( (p=strstr(tempstr,":")) != NULL )
					{
						for (p++ ; *p == ' ' || *p == '\t' ; p++ );	 

						if ( adapterIpAddr != NULL ) strcpy(adapterIpAddr,p);
						rc=0;                    // found
						break;
					}	
				}	
			}
		}
		fclose(fp);
	}
	remove (strRespFile);
	return(rc);  
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////  RS232  /////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int ipRfp_rs232_mail_out (char *cmdBuffer,char *bufferAnswerA,char *bufferAnswerB)
{
    return( ipRfp_rs232_mail_out_timeout (cmdBuffer, bufferAnswerA, bufferAnswerB, 5 /*Sec*/ ) );
}

int ipRfp_rs232_mail_out_V2 (char *cmdBuffer,char *bufferAnswerA,char *bufferAnswerB, char *bufferResponse)
{
    return( ipRfp_rs232_mail_out_timeout_V2 (cmdBuffer, bufferAnswerA, bufferAnswerB,bufferResponse, 5 /*Sec*/ ) );
}


int ipRfp_rs232_mail_out_timeout (char *cmdBuffer,char *bufferAnswerA,char *bufferAnswerB,int timeOutSec)
{
int i;
int iTimeout;
char *pAnswer;	
int answerCount;

int write_count=strlen(cmdBuffer);
int read_count=strlen(bufferAnswerA);


	//  bufferAnswerA =  die Antwort auf die gewartet wird
	//  bufferAnswerB =  (Optional) die Zweite Antwort auf die gewartet wird

#ifdef BIT_IP_RFP35_DEBUG_RS232
	FILE *fp;
	fp=fopen(BIT_IP_RFP35_DEBUG_RS232, "a");
#endif
	
	if(write_count)
	{
		
		FlushInQ (rs232_RFP.port);			  //clear status
		if(read_count) FlushOutQ (rs232_RFP.port);
		i = ComWrt(rs232_RFP.port,cmdBuffer,write_count); 
		
#ifdef BIT_IP_RFP35_DEBUG_RS232
		fwrite(cmdBuffer,1,write_count,fp);
#endif				
		if(Print2StandardIO)
		{
			printf(cmdBuffer);
			printf("\n");
		}
			

		if(i != write_count)
			return(-1);
		
	}
	
	if(read_count)
	{
		gResponse[0] = '\0';  	
		answerCount = 0;
		
		//iTimeout = 50;
		iTimeout = (timeOutSec * 1000) / 100;    // von sec nach mSec geteilt durch 100ms
		if ( iTimeout < 10 ) iTimeout = 10;      // minimal 10 entspricht 1Sec
		
		
		while( iTimeout > 0 && answerCount < sizeof(gResponse) )             // 50*100ms = 5 Sec
		{
			Sleep(100);          // Delay(0.100);   // 100ms
			while ( GetInQLen (rs232_RFP.port) )
			{
				if ( ComRd(rs232_RFP.port,&(gResponse[answerCount]),1) != 0 )
				{
#ifdef BIT_IP_RFP35_DEBUG_RS232
					fputc(gResponse[answerCount],fp);
#endif				
			if(Print2StandardIO)
			{
				printf("%c",gResponse[answerCount]);
			}
					
					answerCount++;
					gResponse[answerCount] = '\0';  	
				}
				else
				{
				    break;
				}    
			}
			
			if ( (pAnswer = strstr(gResponse,bufferAnswerA)) != NULL)
				break;
			
			if ( bufferAnswerB != NULL )
			{	
				if ( (pAnswer = strstr(gResponse,bufferAnswerB)) != NULL)
					break;
			}	
			
			iTimeout--;
		}	
	
		if( pAnswer == NULL)
		{
#ifdef BIT_IP_RFP35_DEBUG_RS232
			fclose(fp);
#endif			
			return(-1);         //nichts gefunden
		}	
		
	}

#ifdef BIT_IP_RFP35_DEBUG_RS232
fclose(fp);
#endif
return(0);
}


int ipRfp_rs232_mail_out_timeout_V2 (char *cmdBuffer,char *bufferAnswerA,char *bufferAnswerB,char * bufferResponse,int timeOutSec)
{
int i;
int iTimeout;
char *pAnswer;	
int answerCount;

int write_count=strlen(cmdBuffer);
int read_count=strlen(bufferAnswerA);


	//  bufferAnswerA =  die Antwort auf die gewartet wird
	//  bufferAnswerB =  (Optional) die Zweite Antwort auf die gewartet wird

#ifdef BIT_IP_RFP35_DEBUG_RS232
	FILE *fp;
	fp=fopen(BIT_IP_RFP35_DEBUG_RS232, "a");
#endif
	
	if(write_count)
	{
		
		FlushInQ (rs232_RFP.port);			  //clear status
		if(read_count) FlushOutQ (rs232_RFP.port);
		i = ComWrt(rs232_RFP.port,cmdBuffer,write_count); 
		
#ifdef BIT_IP_RFP35_DEBUG_RS232
		fwrite(cmdBuffer,1,write_count,fp);
#endif				
		if(Print2StandardIO)
		{
			printf(cmdBuffer);
			printf("\n");
		}
			

		if(i != write_count)
			return(-1);
		
	}
	
	if(read_count)
	{
		gResponse[0] = '\0';  	
		answerCount = 0;
		
		//iTimeout = 50;
		iTimeout = (timeOutSec * 1000) / 100;    // von sec nach mSec geteilt durch 100ms
		if ( iTimeout < 10 ) iTimeout = 10;      // minimal 10 entspricht 1Sec
		
		
		while( iTimeout > 0 && answerCount < sizeof(gResponse) )             // 50*100ms = 5 Sec
		{
			Sleep(100);          // Delay(0.100);   // 100ms
			while ( GetInQLen (rs232_RFP.port) )
			{
				if ( ComRd(rs232_RFP.port,&(gResponse[answerCount]),1) != 0 )
				{
#ifdef BIT_IP_RFP35_DEBUG_RS232
					fputc(gResponse[answerCount],fp);
#endif				
			if(Print2StandardIO)
			{
				printf("%c",gResponse[answerCount]);
			}
					
					answerCount++;
					gResponse[answerCount] = '\0';  	
				}
				else
				{
				    break;
				}    
			}
			
			if ( (pAnswer = strstr(gResponse,bufferAnswerA)) != NULL)
				break;
			
			if ( bufferAnswerB != NULL )
			{	
				if ( (pAnswer = strstr(gResponse,bufferAnswerB)) != NULL)
					break;
			}	
			
			iTimeout--;
		}	
	
		if( pAnswer == NULL)
		{
#ifdef BIT_IP_RFP35_DEBUG_RS232
			fclose(fp);
#endif			
			return(-1);         //nichts gefunden
		}	
		
	}

#ifdef BIT_IP_RFP35_DEBUG_RS232
fclose(fp);
#endif


strcpy(bufferResponse,gResponse);
return(0);
}





int ipRfp_rs232_activate(void)
{

char mail_buffer[256];
char *p;


	sprintf(mail_buffer,"%c",0x0d);
	if(ipRfp_rs232_mail_out(mail_buffer, "# ",NULL)) return(-1);
	
	sprintf(mail_buffer,"echo UAUA%c",0x0d);
	if(ipRfp_rs232_mail_out(mail_buffer,"# ",NULL)) return(-2);
	
	// find next Line
	p = strpbrk(gResponse,"\r\n");
	if ( p != NULL )
	{	
		if ( strstr(p,"UAUA") != NULL )
			return(0);						
	}	
	return(-3);						
}


int ipRfp_rs232_sendShellCmd(char *strCmd)
{

char mail_buffer[256];
char *p;


	// send a simple sh command, without anny error 

	sprintf(mail_buffer,"cd /%c",0x0d);
	if(ipRfp_rs232_mail_out(mail_buffer,"# ",NULL)) return(-1);
	
	sprintf(mail_buffer,"%s%c",strCmd,0x0d);
	if(ipRfp_rs232_mail_out(mail_buffer,"# ",NULL)) return(-2);
	
	return(0);						
}


int ipRfp_rs232_sync(void)
{
char *p;
int  delay=0;
unsigned long   t;

char mail_buffer[256];
char cInfo[256];


	// das sync Kommando im RFP kommt ab und zu nicht zurueck
	// oder dauert länger als 5 Sec
	// wenn dies der Fall ist wird mehrmals ein CR ausgegeben
	// und auf die Response gewartet.
	


	//t=GetTickCount();       //time in ms

	sprintf(mail_buffer,"cd /%c",0x0d);
	if(ipRfp_rs232_mail_out(mail_buffer,"# ",NULL)) return(-1);
	
	// das sync brauch ab und an mehr als 8 Sec
	sprintf(mail_buffer,"sync;echo $?%c",0x0d);
	if(ipRfp_rs232_mail_out(mail_buffer,"# ",NULL)) 
	{
		// nur noch 10mal CR ausgeben, bis Antwort kommt
		delay=10;
		sprintf(mail_buffer,"%c",0x0d);  
		do
		{
			if ( delay == 0 )
			{
				//sprintf(cInfo,"sync delay=%dms", (GetTickCount()-t)); 
				//MessagePopup("ipRfp_rs232_sync",cInfo); 
				return(-2);
			}	
			delay--;
		}		
		while ( ipRfp_rs232_mail_out(mail_buffer,"# ",NULL) != 0 ); 
	}
	
	return(0);						
}



int ipRfp_rs232_getTemperature(int *internal, int *external)
{

char mail_buffer[256];
char *p;

/*: 
manu temp
temperature (degree Celsius): internal=39, external=38
[root@rfp4g_C3DC64]#
*/

	*internal = -10;
	*external = -10;
	sprintf(mail_buffer,"manu temp%c",0x0d);
	if(ipRfp_rs232_mail_out(mail_buffer,"# ",NULL)) return(-1);
	if (p = strstr(gResponse,"internal"))
		*internal = atoi(&p[9]);
	if (p = strstr(gResponse,"external"))
		*external = atoi(&p[9]);
		
	
	return(0);						
}


int ipRfp_rs232_flashErase(void)
{

char mail_buffer[256];
char *p;



	sprintf(mail_buffer,"cd /%c",0x0d);
	if(ipRfp_rs232_mail_out(mail_buffer,"# ",NULL)) return(-1);


	//if(ipRfp_rs232_mail_out(mail_buffer,"# ",NULL)) return(-1);


	// # manu flash_erase
	// Erasing 128 Kibyte @ 0 --  0 % complete.Erasing 128 Kibyte @ 20000 -- 25 % complete.Erasing 128 Kibyte @ 40000 -- 50 % complete.Erasing 128 Kibyte @ 60000 -- 75 % complete.Erasing 128 Kibyte @ 60000 -- 75 % complete.
	// Erasing 128 Kibyte @ 0 --  0 % complete.Erasing 128 Kibyte @ 0 --  0 % complete.
	// Erasing 128 Kibyte @ 0 --  0 % complete.Erasing 128 Kibyte @ 0 --  0 % complete.
	// Erasing 128 Kibyte @ 0 --  0 % complete.Erasing 128 Kibyte @ 20000 --  0 % complete
	// Erasing 128 KibRestarting system.6 % complete
	
	
	sprintf(mail_buffer,"manu flash_erase%c",0x0d);
	if(ipRfp_rs232_mail_out(mail_buffer,"Restarting system",NULL)) return(-2);
	
	// read Buffer empty
	(void)ipRfp_rs232_WaitForBootUp(2,NULL,NULL,NULL,NULL,NULL);       // 2 sec 
	
	
	return(0);						
}




int ipRfp_rs232_CheckOTParea(void)
{
char mail_buffer[256];
char *p;

	// logread
	// 
	// Jan  1 00:00:04 init: starting pid 961, tty '/dev/null': '/opt/rfpm/rfpm'
	// Jan  1 00:00:04 init: starting pid 962, tty '/dev/null': '/opt/mediasrv/mediasrv'
	// Jan  1 00:00:04 mediasrv: ***     00:00:04.035 os_core_unix.c  pjlib 1.8.10 for POSIX initialized
	// Jan  1 00:00:04 mediasrv: ****    00:00:04.039       server.c  Listening socket ready
	// Jan  1 00:00:04 init: starting pid 966, tty '/dev/null': '/opt/ip_rfp/ip_rfp'
	// Jan  1 00:00:04 rfp: 4294937818 !     MAIN: Starting iprfp ...
	// Jan  1 00:00:04 rfpm: 4294937818 !     MAIN: Starting rfpm ...
	// Jan  1 00:00:04 rfpm: 4294937822 !     TEMP: Board 35 C
	// Jan  1 00:00:04 udhcpc[991]: udhcpc (v1.13.4) started
	// Jan  1 00:00:04 udhcpc[991]: Sending discover...
	// Jan  1 00:00:04 init: starting pid 967, tty '/dev/ttyS0': '-/bin/sh'
	// Jan  1 00:00:05 rfp: 4294937919 !     OTPDB: Simulation file <otppages.bin> found - Flash OTP area will not be used !
	// Jan  1 00:00:06 kernel: eth0: link up, 1000 Mb/s, full duplex, flow control disabled
	// Jan  1 00:00:06 kernel: eth1: link up, 100 Mb/s, full duplex, flow control disabled
	// Jan  1 00:00:07 udhcpc[991]: Sending discover...
	// Jan  1 00:00:07 udhcpc[991]: bogus vend options, option fields too long (194 >= 6)
	// Jan  1 00:00:07 udhcpc[991]: bogus vendor encapsulated options, option fields too long (193 >= 6)
	// Jan  1 00:00:07 udhcpc[991]: Sending select for 192.168.99.250...
	// Jan  1 00:00:07 udhcpc[991]: bogus vend options, option fields too long (194 >= 6)
	// Jan  1 00:00:07 udhcpc[991]: bogus vendor encapsulated options, option fields too long (193 >= 6)


	// der String "Flash OTP area will not be used" wird kurz for linkup ausgegeben.
	// also den logread kurz nach linkstatus auffuehren
	// aber vor der ipAddr Vergabe, sonnst fuellt der dhcp den logreadbuffer
	


	// logread |grep -i otp
    //Pass (Example)
    //Example 1:(Date)kernel: NAND device: Setting OTP functions for MT29F1G08AAC
    //Example 2:(Date)kernel: NAND device: Setting OTP functions for MT29F1G08ABADAWP

    //Fail (Example)
    //(Date)kernel: NAND device: Setting OTP functions for MT29F1G08AAC
    //(Date)rfp: 4294937907 !     OTPDB: Simulation file <otppages.bin> found - Flash OTP area will not be used !
    //(Date)rfp: 4294937908 ****  OTPDB: Error -3 reading BMC_CALIBRATION_DATA in otpReadElement: Element not found
    //
    //Remark: The identification for malfunction of the OTP area is given by the string “Flash OTP area will not be used”


	sprintf(mail_buffer,"cd /%c",0x0d);
	if(ipRfp_rs232_mail_out(mail_buffer,"# ",NULL)) return(-1);
	
	sprintf(mail_buffer,"logread | grep -i otp%c",0x0d);
	if(ipRfp_rs232_mail_out(mail_buffer,"# ",NULL)) return(-2);

	if ( strstr(gResponse,"Flash OTP area will not be used") != NULL )
        return(1);					//der Text der Fehler bedeutet  
    
	return(0);  //ok						
}




int ipRfp_rs232_checkBootImage(char *errBuffer) 								// @GC 05.10.2011
{
unsigned char *p;
int  retry;
char mail_buffer[256];


	if ( errBuffer != NULL ) strcpy(errBuffer,"Boot Image Failed");


	// # df
	// Filesystem           1K-blocks      Used Available Use% Mounted on
	// tmpfs                    16384        44     16340   0% /tmp
	// udev                      1024        48       976   5% /dev
	// /dev/usbStick          3908100     19800   3888300   1% /media/usbStick
	// ubi3:flash               57832     16064     38828  29% /flash
	// ubi4:var                 56884       112     53880   0% /var
	// varlock                   1024         0      1024   0% /var/lock
	// varrun                   32768        60     32708   0% /var/run
	// shmfs                    62336         0     62336   0% /dev/shm
	
	sprintf(mail_buffer,"cd /%c",0x0d);
	if(ipRfp_rs232_mail_out(mail_buffer,"# ",NULL)) return(-1);
	
	// von 10 auf 30 Sec. hoch gesetzt @GC 18.03.2014
	for ( retry=15;  retry > 0 ; retry-- )
	{
		Sleep(2000);           // wait 2s       

		// ab FW SIP-DECT 6.0 SP2-BA11   
		// benoetigt der RFP bis zu 12Sek. bis der Prompt zurueck kommt.
		// speziell bei dem "df" Kommando wenn der USB Stick gemountet wird
		// 18.01.2016 @ GC
		
		sprintf(mail_buffer,"df%c",0x0d);
		// if(ipRfp_rs232_mail_out(mail_buffer,"# ",NULL)) return(-2);
		if(ipRfp_rs232_mail_out_timeout(mail_buffer,"# ",NULL, 15 )) return(-2);

		// ubi3:flash vorhanden ?
		if ( (p=strstr(gResponse,"ubi3:flash")) == NULL )
		{
			continue;       // /flash not found
		}
	
		// /dev/usbStick vorhanden ?
		if ( (p=strstr(gResponse,"/dev/usbStick")) == NULL )
		{
			continue;       // usbStick not found
		}
		// alles vorhanden
		break;
		
	}
	if ( retry == 0 )
	{	
		if ( errBuffer != NULL ) strcpy(errBuffer,"Filesystem missing");
		return(1);   // Filesystem?? missing
	}	

	
	

	// # ls -lR /flash 
	// /flash:
	// drwxr-xr-x    2 root     root          520 Aug 19 05:13 boot
	//
	// /flash/boot:
	// lrwxrwxrwx    1 root     root           10 Aug 19 05:13 bootimage -> bootimage0
	// -rw-r--r--    1 root     root      8449680 Jan  1  2000 bootimage0
	// -rw-r--r--    1 root     root          340 Jan  1  2000 bootimage0.hdr
	// -rw-r--r--    1 root     root      8449680 Jan  1  2000 bootimage1
	// -rw-r--r--    1 root     root          340 Jan  1  2000 bootimage1.hdr
	
	
	// von 30 auf 60 Sec. hoch gesetzt @GC 18.03.2014
	// damit sind dann bootimage0 und der link auf bootimage0 vorhanden.
	// Das bootimage1 ist immer noch nicht vorhanden, die Abfrage auf bootimage1 rausgenommen
	// @GC 18.03.2014
	for ( retry=30;  retry > 0 ; retry-- )
	{
		Sleep(2000);           // wait 2s       
		
		if ( ipRfp_rs232_sync() ) return(-3);
		
		
		sprintf(mail_buffer,"manu bit%c",0x0d);
		if(ipRfp_rs232_mail_out(mail_buffer,"# ",NULL)) return(-4);

		
		sprintf(mail_buffer,"ls -lR /flash%c",0x0d);
		if(ipRfp_rs232_mail_out(mail_buffer,"# ",NULL)) return(-5);

		// ist /flash/boot vorhanden ?
		if ( (p=strstr(gResponse,"/flash/boot:")) == NULL )
		{
			continue;       // /flash/boot not found 
		}

		// ist bootimage als link vorhanden ?
		if ( (p=strstr(gResponse,"bootimage ->")) == NULL )
		{
			continue;       // bootimage not found   
		}
	
		// ist bootimage0 vorhanden ?
		if ( (p=strstr(gResponse,"bootimage0")) == NULL )
		{
			continue;       // bootimage0 not found       
		}

    	// die Abfrage auf bootimage1 rausgenommen,
    	// wird anscheinend erst beim naechsten Boot kopiert
    	// @GC 18.03.2014
		// ist bootimage1 vorhanden ?
//		if ( (p=strstr(gResponse,"bootimage1")) == NULL )
//		{
//			continue;       // bootimage1 not found       
//		}

		// alles vorhanden
		break;

	}
	if ( retry == 0 )
	{	
		if ( errBuffer != NULL ) strcpy(errBuffer,"Boot Image missing");
		return(1);   // bootimage?? missing
	}	
	
	
	
	// # ls -l
	// -rwxr-xr-x    1 root     root      8450020 Jul 19 10:32 iprfp43.dnld
	// drwxr-xr-x    5 root     root         4096 Aug 24 10:50 openocd-scripts
	// drwxr-xr-x    3 root     root         4096 Aug 24 10:51 rfp43
	// -rwxr-xr-x    1 root     root      8449680 Jul 19 10:32 uImage
	// drwxr-xr-x    2 root     root         4096 Aug 24 10:51 windows
	
	sprintf(mail_buffer,"ls -l /media/usbStick%c",0x0d);
	if(ipRfp_rs232_mail_out(mail_buffer,"# ",NULL)) return(-6);
	

	// # diff /media/usbStick/uImage /flash/boot/bootimage
	sprintf(mail_buffer,"diff /media/usbStick/uImage /flash/boot/bootimage;echo RET=[$?]%c",0x0d);
	// if(ipRfp_rs232_mail_out(mail_buffer,"# ",NULL)) return(-7);
	if(ipRfp_rs232_mail_out_timeout(mail_buffer,"# ",NULL, 15 )) return(-7);
	
	// ist RET=[0]
	if ( (p=strstr(gResponse,"RET=[0]")) == NULL )
	{
		if ( errBuffer != NULL ) strcpy(errBuffer,"Boot Image failed");
		return(2);       // diff failed
	}
	

	if ( errBuffer != NULL ) strcpy(errBuffer,"");
	return(0);   // ok						
}



int ipRfp_rs232_CheckUBIerror(char *errBuffer)
{
char *pResp, *p,*pa;
char mail_buffer[256];
char cDev[80];
char cTemp[80];



	// es wird ueberprueft ob die UBI Filesysteme die auf dem Flash liegen
	// auch ok sind und richtig angebunden (attached) sind
	// dies kann bei 
	// mtd0   uboot
	// mtd1   uboot-env0
	// mtd2   uboot-env1
	// mtd3   ubifs-flash	   Filesystem /flash
	// mtd4   ubifs-var		   Filesystem /var
	// der fall sein.


	// Der output der RS232 beim Booten kann ueberwacht werden
    // oder mit den Kommandos
	// logread | grep "UBI error:"
	// cat /tmp/boot.log | grep "UBI error:"
	// abgesucht werden
	//
	// das Kommando "cat /tmp/boot.log" ist die beste Alternative
    // da dies kein Ring Buffer ist sondern nur der Boot abgefangen wird.
	// cat /tmp/boot.log | egrep "UBI: attaching|UBI error:"



	// hier der logread Abschnitt ohne Fehler
	// Oct 29 06:11:14 kernel: UBI: attaching mtd3 to ubi3
	// Oct 29 06:11:14 kernel: UBI: physical eraseblock size:   131072 bytes (128 KiB)
	// Oct 29 06:11:14 kernel: UBI: logical eraseblock size:    129024 bytes
	// Oct 29 06:11:14 kernel: UBI: smallest flash I/O unit:    2048
	// Oct 29 06:11:14 kernel: UBI: sub-page size:              512
	// Oct 29 06:11:14 kernel: UBI: VID header offset:          512 (aligned 512)
	// Oct 29 06:11:14 kernel: UBI: data offset:                2048
	// Oct 29 06:11:14 kernel: UBI: max. sequence number:       187
	// Oct 29 06:11:14 kernel: UBI: attached mtd3 to ubi3
	// Oct 29 06:11:14 kernel: UBI: MTD device name:            "ubifs-flash"
	// Oct 29 06:11:14 kernel: UBI: MTD device size:            64 MiB
	// Oct 29 06:11:14 kernel: UBI: number of good PEBs:        512
	// Oct 29 06:11:14 kernel: UBI: number of bad PEBs:         0
	// Oct 29 06:11:14 kernel: UBI: number of corrupted PEBs:   0
	// Oct 29 06:11:14 kernel: UBI: max. allowed volumes:       128
	// Oct 29 06:11:14 kernel: UBI: wear-leveling threshold:    4096
	// Oct 29 06:11:14 kernel: UBI: number of internal volumes: 1
	// Oct 29 06:11:14 kernel: UBI: number of user volumes:     1
	// Oct 29 06:11:14 kernel: UBI: available PEBs:             0
	// Oct 29 06:11:14 kernel: UBI: total number of reserved PEBs: 512
	// Oct 29 06:11:14 kernel: UBI: number of PEBs reserved for bad PEB handling: 5
	// Oct 29 06:11:14 kernel: UBI: max/mean erase counter: 2/1
	// Oct 29 06:11:14 kernel: UBI: image sequence number:  -1855566463
	// Oct 29 06:11:14 kernel: UBI: background thread "ubi_bgt3d" started, PID 770
	// Oct 29 06:11:14 kernel: UBIFS: recovery needed
	// Oct 29 06:11:14 kernel: UBIFS: recovery completed
	// Oct 29 06:11:14 kernel: UBIFS: mounted UBI device 3, volume 0, name "flash"
	// Oct 29 06:11:14 kernel: UBIFS: file system size:   63737856 bytes (62244 KiB, 60 MiB, 494 LEBs)
	// Oct 29 06:11:14 kernel: UBIFS: journal size:       3225600 bytes (3150 KiB, 3 MiB, 25 LEBs)
	// Oct 29 06:11:14 kernel: UBIFS: media format:       w4/r0 (latest is w4/r0)
	// Oct 29 06:11:14 kernel: UBIFS: default compressor: lzo
	// Oct 29 06:11:14 kernel: UBIFS: reserved for root:  3010495 bytes (2939 KiB)
	// Oct 29 06:11:14 kernel: UBI: attaching mtd4 to ubi4
	// Oct 29 06:11:14 kernel: UBI: physical eraseblock size:   131072 bytes (128 KiB)
	// Oct 29 06:11:14 kernel: UBI: logical eraseblock size:    129024 bytes
	// Oct 29 06:11:14 kernel: UBI: smallest flash I/O unit:    2048
	// Oct 29 06:11:14 kernel: UBI: sub-page size:              512
	// Oct 29 06:11:14 kernel: UBI: VID header offset:          512 (aligned 512)
	// Oct 29 06:11:14 kernel: UBI: data offset:                2048
	// Oct 29 06:11:14 kernel: UBI: max. sequence number:       55
	// Oct 29 06:11:14 kernel: UBI: attached mtd4 to ubi4
	// Oct 29 06:11:14 kernel: UBI: MTD device name:            "ubifs-var"
	// Oct 29 06:11:14 kernel: UBI: MTD device size:            63 MiB
	// Oct 29 06:11:14 kernel: UBI: number of good PEBs:        504
	// Oct 29 06:11:14 kernel: UBI: number of bad PEBs:         0
	// Oct 29 06:11:14 kernel: UBI: number of corrupted PEBs:   0
	// Oct 29 06:11:14 kernel: UBI: max. allowed volumes:       128
	// Oct 29 06:11:14 kernel: UBI: wear-leveling threshold:    4096
	// Oct 29 06:11:14 kernel: UBI: number of internal volumes: 1
	// Oct 29 06:11:14 kernel: UBI: number of user volumes:     1
	// Oct 29 06:11:14 kernel: UBI: available PEBs:             0
	// Oct 29 06:11:14 kernel: UBI: total number of reserved PEBs: 504
	// Oct 29 06:11:14 kernel: UBI: number of PEBs reserved for bad PEB handling: 5
	// Oct 29 06:11:14 kernel: UBI: max/mean erase counter: 2/1
	// Oct 29 06:11:14 kernel: UBI: image sequence number:  540116897
	// Oct 29 06:11:14 kernel: UBI: background thread "ubi_bgt4d" started, PID 786
	// Oct 29 06:11:14 kernel: UBIFS: recovery needed
	// Oct 29 06:11:14 kernel: UBIFS: recovery completed
	// Oct 29 06:11:14 kernel: UBIFS: mounted UBI device 4, volume 0, name "var"
	// Oct 29 06:11:14 kernel: UBIFS: file system size:   62705664 bytes (61236 KiB, 59 MiB, 486 LEBs)
	// Oct 29 06:11:14 kernel: UBIFS: journal size:       3096576 bytes (3024 KiB, 2 MiB, 24 LEBs)
	// Oct 29 06:11:14 kernel: UBIFS: media format:       w4/r0 (latest is w4/r0)
	// Oct 29 06:11:14 kernel: UBIFS: default compressor: lzo
	// Oct 29 06:11:14 kernel: UBIFS: reserved for root:  2961743 bytes (2892 KiB)
	// Oct 29 06:11:14 kernel: Orion Watchdog Timer: Initial timeout 21 sec, nowayout


	// hier einige Fehler Beispiele

		// UBI: attaching mtd3 to ubi3
		// UBI: physical eraseblock size:   131072 bytes (128 KiB)
		// UBI: logical eraseblock size:    129024 bytes
		// UBI: smallest flash I/O unit:    2048
		// UBI: sub-page size:              512
		// UBI: VID header offset:          512 (aligned 512)
		// UBI: data offset:                2048
		// UBI: max. sequence number:       0
//fehler   UBI error: ubi_read_volume_table: the layout volume was not found
		// UBI: attaching mtd3 to ubi3
		// UBI: physical eraseblock size:   131072 bytes (128 KiB)
		// UBI: logical eraseblock size:    129024 bytes
		// UBI: smallest flash I/O unit:    2048
		// UBI: sub-page size:              512
		// UBI: VID header offset:          512 (aligned 512)
		// UBI: data offset:                2048
		// UBI: empty MTD device detected
		// UBI: max. sequence number:       0
		// UBI: create volume table (copy #1)
		// UBI: create volume table (copy #2)
		// UBI: attached mtd3 to ubi3
		// UBI: MTD device name:            "ubifs-flash"
		// UBI: MTD device size:            64 MiB
		// UBI: number of good PEBs:        512
		// UBI: number of bad PEBs:         0
		// UBI: number of corrupted PEBs:   0
		// UBI: max. allowed volumes:       128
		// UBI: wear-leveling threshold:    4096
		// UBI: number of internal volumes: 1
		// UBI: number of user volumes:     0
		// UBI: available PEBs:             503
		// UBI: total number of reserved PEBs: 9
		// UBI: number of PEBs reserved for bad PEB handling: 5
		// UBI: max/mean erase counter: 0/0
		// UBI: image sequence number:  1440669299
		// UBI: background thread "ubi_bgt3d" started, PID 809
		// UBIFS: default file-system created
		// UBIFS: mounted UBI device 3, volume 0, name "flash"


		// UBI: attaching mtd1 to ubi0
		// UBI: physical eraseblock size:   131072 bytes (128 KiB)
		// UBI: logical eraseblock size:    129024 bytes
		// UBI: smallest flash I/O unit:    2048
		// UBI: sub-page size:              512
		// UBI: VID header offset:          512 (aligned 512)
		// UBI: data offset:                2048
//fehler   UBI error: ubi_read_volume_table: the layout volume was not found
//fehler   UBI error: ubi_init: cannot attach mtd1
//fehler   UBI error: ubi_init: UBI error: cannot initialize UBI, error -22
		// UBI init error 22
//normal   UBIFS error (pid 0): ubifs_get_sb: cannot open "ubi:flash", error -19
		// Error reading superblock on volume 'ubi:flash'!
		// UBIFS not mounted, use ubifs mount to mount volume first!
		// UBIFS not mounted, use ubifs mount to mount volume first!
		// UBIFS not mounted, use ubifs mount to mount volume first!



	// cat /tmp/boot.log | egrep "UBI: attach|UBI error:"
	// UBI: attaching mtd3 to ubi3
	// UBI error: ubi_read_volume_table: the layout volume was not found
	// UBI: attaching mtd4 to ubi4
	// UBI: attached mtd4 to ubi4


	if ( errBuffer != NULL ) strcpy(errBuffer,"");



	sprintf(mail_buffer,"cd /%c",0x0d);
	if(ipRfp_rs232_mail_out(mail_buffer,"# ",NULL)) return(-1);
	
	sprintf(mail_buffer,"cat /tmp/boot.log | egrep \"UBI: attach|UBI error:\"%c",0x0d);
	if(ipRfp_rs232_mail_out(mail_buffer,"# ",NULL)) return(-2);


	// erste Zeile ueberspringen
	pResp=gResponse;
	while ( *pResp != '\n' && *pResp != '\r'  && *pResp != '\0' )  pResp++;

	
	strcpy(cDev,"");
		
	
	// UBI: attaching mtd3 to ubi3
    // UBI error: ubi_read_volume_table: the layout volume was not found
	// UBI: attaching mtd3 to ubi3
	// UBI: attached mtd3 to ubi3
	
	// UBI error: ist nur Fehler wenn nachvolgend das Device nicht doch noch attached wurde

	while ( (p=strstr( pResp,"UBI error: ")) != NULL )
	{
        
		// zurueck suchen bis    "attaching mtd3 to ubi3"
		pa=p; 
		while ( pa > pResp && strncmp(pa,"attaching", sizeof("attaching")-1 ) != 0  ) pa--;
		sscanf( pa ,"attaching %s",cDev); 	  // mtd3
		  
		
		// wird wirklich nicht attached ??
		// "UBI: attached mtd3 to"
		sprintf(cTemp,"UBI: attached %s to",cDev);
		if ( strstr( p, cTemp) == NULL )
		{	
			if ( errBuffer != NULL ) sprintf(errBuffer,"UBI-Error attaching %s",cDev);
			return(1);  // fail
		}
		// next "UBI error: "
		pResp=p+1;

	}	

	return(0);   // ok
}






int ipRfp_rs232_setIpAddress(char *ipStr)
{

char mail_buffer[256];
int i,retry;
int n1,n2,n3,n4;
char *p;
char read_ipStr[80];
char *interface=NULL;

	strcpy(read_ipStr,"0.0.0.0");
	
	//# ifconfig
	//br0       Link encap:Ethernet  HWaddr 00:30:42:12:69:9C
	//          inet addr:192.168.99.254  Bcast:192.168.99.255  Mask:255.255.255.0
	//          UP BROADCAST RUNNING MULTICAST  MTU:1500  Metric:1
	//          RX packets:25 errors:0 dropped:0 overruns:0 frame:0
	//          TX packets:0 errors:0 dropped:0 overruns:0 carrier:0
	//          collisions:0 txqueuelen:0
	//          RX bytes:2118 (2.0 KiB)  TX bytes:0 (0.0 B)
	//
	//eth0      Link encap:Ethernet  HWaddr 00:30:42:12:69:9C
	//          UP BROADCAST RUNNING MULTICAST  MTU:1500  Metric:1
	//          RX packets:25 errors:0 dropped:0 overruns:0 frame:0
	//          TX packets:10 errors:0 dropped:0 overruns:0 carrier:0
	//          collisions:0 txqueuelen:1000
	//          RX bytes:2568 (2.5 KiB)  TX bytes:460 (460.0 B)
	//          Interrupt:11
	//
	//eth1      Link encap:Ethernet  HWaddr 02:30:42:4B:49:52
	//          UP BROADCAST RUNNING MULTICAST  MTU:1500  Metric:1
	//          RX packets:442 errors:0 dropped:0 overruns:0 frame:0
	//          TX packets:442 errors:0 dropped:0 overruns:0 carrier:0
	//          collisions:0 txqueuelen:1000
	//          RX bytes:28288 (27.6 KiB)  TX bytes:135903 (132.7 KiB)
	//          Interrupt:15
	//
	//lo        Link encap:Local Loopback
	//          inet addr:127.0.0.1  Mask:255.0.0.0
	//          UP LOOPBACK RUNNING  MTU:16436  Metric:1
	//          RX packets:631 errors:0 dropped:0 overruns:0 frame:0
	//          TX packets:631 errors:0 dropped:0 overruns:0 carrier:0
	//          collisions:0 txqueuelen:0
	//          RX bytes:31572 (30.8 KiB)  TX bytes:31572 (30.8 KiB)
	//
	//#
	
	sprintf(mail_buffer,"cd /%c",0x0d);
	if(ipRfp_rs232_mail_out(mail_buffer,"# ",NULL)) return(-1);
	
	sprintf(mail_buffer,"ifconfig%c",0x0d);
	if(ipRfp_rs232_mail_out(mail_buffer,"# ",NULL)) return(-2);

	// ist interface br0 vorhanden
	if ( strstr(gResponse,"br0       Link encap") != NULL )
	{
		interface="br0";
	}
		// ist interface eth0 vorhanden
	else if ( strstr(gResponse,"eth0      Link encap") != NULL )
	{
		interface="eth0";
	}
	else
	{
		return(-3);
	}

// write ipAddr	
	sprintf(mail_buffer,"cd /%c",0x0d);
	if(ipRfp_rs232_mail_out(mail_buffer,"# ",NULL)) return(-4);
	
	
	// ipStr must be in "192.168.99.254"
	// ifconfig br0 192.168.99.254
	sprintf(mail_buffer,"ifconfig %s %s%c",interface, ipStr, 0x0d);
	if(ipRfp_rs232_mail_out(mail_buffer,"# ",NULL)) return(-5);
	
	
	
// read ipAddr	
	
	sprintf(mail_buffer,"ifconfig %s%c",interface, 0x0d);
	if(ipRfp_rs232_mail_out(mail_buffer,"# ",NULL)) return(-6);
	
	p = strstr(gResponse,"inet addr:");
	if ( p != NULL )
	{
		if ( sscanf( p ,"inet addr:%d.%d.%d.%d",&n1,&n2,&n3,&n4) == 4 )
		{	
			// 0.0.0.0 bis 255.255.255.255
			sprintf(read_ipStr,"%d.%d.%d.%d",(n1&0xff),(n2&0xff),(n3&0xff),(n4&0xff));
		}	
	}

	
	if ( strcmp(ipStr,read_ipStr) != 0 )
	{
		return(-7);						
	}
	

	return(0);   // ok						
}

int ipRfp_rs232_getIpAddress(char *ipStr)
{

char mail_buffer[256];
int i,retry;
int n1,n2,n3,n4;
char *p,*pStart,*pEnd;



	strcpy(ipStr,"0.0.0.0");

	// # hostname -i
	// 192.168.99.254 
	// # 

	//  # hostname -i
	// hostname: ?: Unknown host
	// # 

	//ifconfig
	//br0       Link encap:Ethernet  HWaddr 00:30:42:12:90:00
	//          inet addr:192.168.99.254  Bcast:192.168.99.255  Mask:255.255.255.0
	//          UP BROADCAST RUNNING MULTICAST  MTU:1500  Metric:1
	//          RX packets:41 errors:0 dropped:0 overruns:0 frame:0
	//          TX packets:15 errors:0 dropped:0 overruns:0 carrier:0
	//          collisions:0 txqueuelen:0
	//          RX bytes:5859 (5.7 KiB)  TX bytes:7263 (7.0 KiB)
	//
	//eth0      Link encap:Ethernet  HWaddr 00:30:42:12:90:00
	//          UP BROADCAST RUNNING MULTICAST  MTU:1500  Metric:1
	//          RX packets:41 errors:0 dropped:0 overruns:0 frame:0
	//          TX packets:17 errors:0 dropped:0 overruns:0 carrier:0
	//          collisions:0 txqueuelen:1000
	//          RX bytes:6597 (6.4 KiB)  TX bytes:6811 (6.6 KiB)
	//          Interrupt:11
	//
	//eth1      Link encap:Ethernet  HWaddr 02:30:42:4B:49:52
	//          UP BROADCAST RUNNING MULTICAST  MTU:1500  Metric:1
	//          RX packets:201 errors:0 dropped:1 overruns:0 frame:0
	//          TX packets:200 errors:0 dropped:0 overruns:0 carrier:0
	//          collisions:0 txqueuelen:1000
	//          RX bytes:12864 (12.5 KiB)  TX bytes:131547 (128.4 KiB)
	//          Interrupt:15
	//
	//lo        Link encap:Local Loopback
	//          inet addr:127.0.0.1  Mask:255.0.0.0
	//          UP LOOPBACK RUNNING  MTU:16436  Metric:1
	//          RX packets:135 errors:0 dropped:0 overruns:0 frame:0
	//          TX packets:135 errors:0 dropped:0 overruns:0 carrier:0
	//          collisions:0 txqueuelen:0
	//          RX bytes:6772 (6.6 KiB)  TX bytes:6772 (6.6 KiB)
		  
	//  Wenn der RFP mit WLan Karte Bestückt ist, wird ein Netzwerk "br0" (Bridge) erzeugt, dann steht dort die HWaddr und IP Addr.
	//  ist keine WLAN Karte bestückt oder nicht erkannt worden wird nur ein Netzwerk "eth0" erzeugt , dann steht dort die HWaddr und IP Addr.
	//  
	//  kommando einzeln absetzen "ifconfig br0" oder "ifconfig eth0" funktioniert nicht, da die ssh mit Fehler zuruek kommt.
	//  also nur "ifconfig" absetzten und den Text filtern.
	//
	//  eth1 ist die Interne Netzwerkschnitstelle zum BMC
	

	for ( retry=0 ; retry < 30 ; retry++ ) 
	{
	
		sprintf(mail_buffer,"cd /%c",0x0d);
		if(ipRfp_rs232_mail_out(mail_buffer,"#",NULL)) return(-1);
	
		sprintf(mail_buffer,"ifconfig%c",0x0d);
		if(ipRfp_rs232_mail_out(mail_buffer,"#",NULL)) return(-2);

	
		if ( (pStart=strstr(gResponse,"br0")) != NULL )			// Device "br0" vorhanden ?
		{
		    if ( (pEnd=strstr(pStart,"eth0")) != NULL )			// nur bis Device "eth0" suchen
			{	
				*pEnd='\0';                                     // ende setzen
			}	
				
		}		
		else if ( (pStart=strstr(gResponse,"eth0")) != NULL )	// Device "eth0" vorhanden ?
		{
	    	if ( (pEnd=strstr(pStart,"eth1")) != NULL )			// nur bis Device "eth1 suchen
			{	
				*pEnd='\0';                                     // ende setzen
			}	
				
		}
		else
		{	
			pStart=gResponse;
		}	

		p = strstr(pStart,"inet addr:");
		if ( p != NULL )
		{
			if ( sscanf( p ,"inet addr:%d.%d.%d.%d",&n1,&n2,&n3,&n4) == 4 )
			{	
				// 0.0.0.0 bis 255.255.255.255
				sprintf(ipStr,"%d.%d.%d.%d",(n1&0xff),(n2&0xff),(n3&0xff),(n4&0xff));
  				return(0);						
			}	
		}
		
		// netz noch nicht im ifconfig eingetragen
		Sleep(500);   //500ms
	}	

	return(0);						
}





int ipRfp_rs232_getLinkStatus(char *ethDev,  char *linkStatus)				   // ethDev = eth0, eth1
{

char mail_buffer[256];

int  i;
char *p;
char tempstr[80];

	strcpy(linkStatus,"link down");
	sprintf(mail_buffer,"cd /%c",0x0d);
	if(ipRfp_rs232_mail_out(mail_buffer,"# ",NULL)) return(-1);
	
	// ethtool "eth0"
//	sprintf(mail_buffer,"/sbin/ethtool \"%s\"%c", ethDev, 0x0d);
	sprintf(mail_buffer,"ethtool eth0\n");
	if(ipRfp_rs232_mail_out(mail_buffer,"# ",NULL)) return(-2);

	
	

	p = strstr( gResponse,"Link detected: yes");
	if ( p != NULL )
		strcpy(linkStatus,"link up, ");
	else
		strcpy(linkStatus,"link down, ");

	//        Speed: 1000Mb/s
	p = strstr( gResponse,"Speed: ");
	if ( p != NULL )
    {
        if ( sscanf( p ,"Speed: %s",tempstr) == 1 )
        {
            strcat(linkStatus,tempstr);         // 1000Mb/s
            strcat(linkStatus,", ");
        }    
    }        
        
	p = strstr( gResponse,"Duplex: ");
	if ( p != NULL )
    {
        if ( sscanf( p ,"Duplex: %s",tempstr) == 1 )
        {
            stringToLow(tempstr);
            strcat(linkStatus,tempstr);         //full
            strcat(linkStatus," duplex");
        }   
    }        
    
    
    for ( p=linkStatus; *p != '\0' ; p++ )    // es darf kein / im string sein  
        if ( *p == '/' ) *p='\'';             // also gegen ' ersetzen
    
    
	return(0);
	
}



int ipRfp_rs232_getUsbDev(char *usbdevStr)
{
int  i,n;
int  retry;
char *p;
char mail_buffer[256];
char cTemp[256];


	strcpy(usbdevStr,"UNPLUGGED");
	
	
	
	// # ls -l /dev/usb*
	// lrwxrwxrwx    1 root     root            4 Jul 10 10:20 /dev/usbStick -> sda1
	// crw-rw----    1 root     root     189,   0 Jul  7 03:57 /dev/usbdev1.1
	// crw-rw----    1 root     root     189,   2 Jul 10 10:20 /dev/usbdev1.3
	// # 
	

	// # df
	// Filesystem           1K-blocks      Used Available Use% Mounted on
	// tmpfs                    16384        40     16344   0% /tmp
	// udev                      1024        48       976   5% /dev
	// /dev/usbStick           983760     21728    962032   2% /media/usbStick
	// ubi3:flash               57832     16428     38464  30% /flash
	// ubi4:var                 56884       228     53764   0% /var
	// varlock                   1024         0      1024   0% /var/lock
	// varrun                   32768        28     32740   0% /var/run
	// shmfs                    62184         0     62184   0% /dev/shm
	// #
	
	sprintf(mail_buffer,"cd /%c",0x0d);
	if(ipRfp_rs232_mail_out(mail_buffer,"# ",NULL)) return(-1);
	

	// der mount der Filesystem kann bis zu 5 Sec dauern
	
	for ( p=NULL, retry=12;  retry > 0 ; retry-- )
	{

		// ab FW SIP-DECT 6.0 SP2-BA11   
		// benoetigt der RFP bis zu 12Sek. bis der Prompt zurueck kommt.
		// speziell bei dem "df" Kommando wenn der USB Stick gemountet wird
		// 18.01.2016 @ GC
	
		sprintf(mail_buffer,"df%c",0x0d);
		// if(ipRfp_rs232_mail_out(mail_buffer,"# ",NULL)) return(-2);
		if(ipRfp_rs232_mail_out_timeout(mail_buffer,"# ",NULL,15)) return(-2);

		// /dev/usbStick vorhanden ?
		if ( (p=strstr(gResponse,"/dev/usb")) != NULL )
		{
			break;       // usbStick found
		}
		
		Sleep(1000);           // wait 1s       
		
	}

	if ( p != NULL )
	{
		if ( sscanf( p ,"/dev/%s %d",usbdevStr,&n) != 2 )   // "usbStick"    // es darf kein "/" zurück gegeben werden
		{
	  		strcpy(usbdevStr,"UNPLUGGED");
		}			
	}



	// @GC neu 12.03.2014 
	// wenn PARK.xml auf usbstick vorhanden ist dann
	// kommt als Zusatz ein "+" hinter dem usbStick
	// zur Erkennung in den LogFiles ob PARK.xml auf dem Stick vorhanden ist 
	
	// der RFP kopiert sich die "/media/usbStick/PARK.xml" nach "/var/opt/omm/PARK.xml"
	// und auch wieder zurueck, damit ist sie dann auf dem Stick
	
	
	// usb stick vorhanden
	if ( strcmp(usbdevStr,"UNPLUGGED") != 0 )  
	{	
		sprintf(mail_buffer,"ls -l /var/opt/omm%c",0x0d);
		if(ipRfp_rs232_mail_out(mail_buffer,"# ",NULL)) return(-2);

		sprintf(mail_buffer,"ls -l /media/usbStick%c",0x0d);
		if(ipRfp_rs232_mail_out(mail_buffer,"# ",NULL)) return(-2);
	
		if ( (p = strstr(gResponse,"PARK.xml")) != NULL )
		{
			strcat(usbdevStr,"+");   // "PARK.xml" vorhanden
		}

		
	}



	
	//
	// neu 27.09.2012 
	// ab FW "SIP-DECT-Lite_3_1RC3" hat das manu Kommando auch eine Erkennung des USB-Stick
	// wird vorerst nur bei RFP SL35 eingesetzt, 
	// kann also nicht immer abgefragt werden.
	//
	// der Output ist:
	// USB STORAGE                             PLUGGED|UNPLUGGED
	//	
	//	# fmanu bit
	//USB STORAGE                              PLUGGED
	//BMC CALIBRATION DATA                     OK
	//BMC STATUS                               RUNNING
	//TEMPERATURE SENSOR                       36 C
	//BOOTIMAGE DOWNLOAD                       IMAGE (file:///media/usbStick/iprfp3G_Addons.dnld) is already local
	//BOOTIMAGE 0                              UPTODATE
	//BOOTDEVICE                               bootimage 0 (via link)
	//RTC (Real Time Clock)                    OK (BOOTTIME: Thu Sep 27 06:58:06 UTC 2012)
	//WLAN MODULE                              NOT FOUND
	//SNTP                                     OK (192.168.99.2)
	//DECT LICENSE INFO                        PARK 26C01873A3, Regulatory Domain US
	//#

    // muss auf jeden Fall über "fmanu" gemacht werden, denn das "manu" Kommando ist ohne USB-Stick nicht vorhanden.	
	sprintf(mail_buffer,"fmanu bit%c",0x0d);
	if(ipRfp_rs232_mail_out(mail_buffer,"# ",NULL)) return(-3);

	// der String "USB STORAGE" im Output gefunden also ist das manu Kommando neu
	// und kann ausgwertet werden
	if ( (p=strstr(gResponse,"USB STORAGE")) != NULL )
	{
		p = p+strlen("USB STORAGE");
		while ( *p == ' ' || *p == '\t') p++;      // alle Blanks ueberspringen
		for ( i=0 ; *p != '\0' && *p != '\n' && *p != '\r' ; p++,i++)	   //bis Zeilenende kopieren
		{	
			cTemp[i] = *p;
		}	
		cTemp[i]='\0';
		if ( strcmp(cTemp,"PLUGGED") != 0 )  
		{
			// doch nicht gefunden	
			strcpy(usbdevStr,cTemp);        // UNPLUGGED
		}	
	}
	
	
	return(0);
	
}


/*
**   den BMC STATUS lesen
*/
int ipRfp_rs232_getBMCStatus(char *bmcStatus, char *bmcTestmode, char *bmcCalibStatus)
{

char *p,*pa;
int  n;
char mail_buffer[256];

	if ( bmcStatus != NULL ) strcpy(bmcStatus,"xxxxx");
	if ( bmcTestmode != NULL ) strcpy(bmcTestmode,"");
	if ( bmcCalibStatus != NULL ) strcpy(bmcCalibStatus,"xx");


	// # manu bit
	// BMC CALIBRATION DATA                     OK
	// BMC STATUS                               RUNNING
	// TEMPERATURE SENSOR                       43 C
	// # 


	// der BMC STATUS            geht von LOADING nach RUNNING
	// der BMC CALIBRATION DATA  Status "OK" wird nach dem finalisieren gesetzt
	//                           wird aber erst nach einem Reboot aktualisiert
	
	// "if type manu 2>/dev/null >/dev/null; then manu bit; else fmanu bit; fi" 
	// "if type manu 2>/dev/null >/dev/null; then { manu bit; } else  { fmanu bit; } fi" 
	
	sprintf(mail_buffer,"cd /%c",0x0d);
	if(ipRfp_rs232_mail_out(mail_buffer,"# ",NULL)) return(-1);
	
	sprintf(mail_buffer,"manu bit%c",0x0d);
	if(ipRfp_rs232_mail_out(mail_buffer,"# ",NULL)) return(-2);
	
	if ( bmcStatus != NULL )
    {
        p = strstr(gResponse,"BMC STATUS");
        if ( p != NULL )
        {	
            //sscanf(p,"BMC STATUS %s",bmcStatus);
			p=p+strlen("BMC STATUS");
			while ( *p == ' ' || *p == '\t') p++;      // alle Blanks ueberspringen
			
			for ( pa=bmcStatus ; *p != '\0' && *p != '\n' && *p != '\r' ; pa++, p++)	   //bis Zeilenende kopieren
			{	
				*pa = *p;
			}	
			*pa='\0';       // ende str
			
			if ( (p=strstr(bmcStatus," (TESTMODE)")) != NULL  ) 
			{	
				*p='\0';    // nur bis TESTMODE
				if (  bmcTestmode != NULL )
				{
					strcpy(bmcTestmode,p+1);       // Testmode 
				}	
			}	

			
				
        }
    }    
        
	if ( bmcCalibStatus != NULL )
    {
        p = strstr(gResponse,"BMC CALIBRATION DATA");
        if ( p != NULL )
        {	
            //sscanf(p,"BMC CALIBRATION DATA %s",bmcCalibStatus);
			p=p+strlen("BMC CALIBRATION DATA");
			while ( *p == ' ' || *p == '\t') p++;      // alle Blanks ueberspringen
			
			for ( pa=bmcCalibStatus ; *p != '\0' && *p != '\n' && *p != '\r' ; pa++, p++)	   //bis Zeilenende kopieren
			{	
				*pa = *p;
			}	
			*pa='\0';       // ende str
			
        }
    }    
        
    return(0);						
}



int ipRfp_rs232_writePARK(char *strParkHexNumber, char *strDECTrDomain, char *strWLANrDomain)
{
/*
** Generation of PARK.xml file
**
** @GC 22.09.2012
**
*/

//int i;
//int n;
char *p;
char mail_buffer[256];
//char strValue[256];
//char expValue[256];
//char *searchStr;

	

	// Generation of PARK.xml file 
	// Testinterface: (6.2 Serial interface)
	// Generates a PARK-xml file for RFP SL35 SI35 SX35 IP
	// All data must be typed as below example
	//
	//	PARK in HEX notation 	XXXXXXXXXX
	//	DECT regulatory domain  US  | EU
	//  WLAN regulatory domain  US  | DE

	// manu genpark "XXXXXXXXXX" "US" "US"
	// manu genpark "XXXXXXXXXX" "EU" "DE"

   	// # manu genpark 1F101873A2 US US
	// park = 1F101873A2, DectRegDomain=2, WlanRegDomain=111
	// successfully created /var/opt/omm/PARK.xml
	//
	//#
	
	sprintf(mail_buffer,"cd /%c",0x0d);
	if(ipRfp_rs232_mail_out(mail_buffer,"# ",NULL)) return(-1);

	
	// manu genpark "1F101873A2" "US" "US"
	sprintf(mail_buffer,"manu genpark \"%s\" \"%s\" \"%s\";echo Ret=[$?]%c",strParkHexNumber,strDECTrDomain,strWLANrDomain,0x0d);
	if(ipRfp_rs232_mail_out(mail_buffer,"# ",NULL)) return(-2);


	// FATAL ERROR dann Fehler
	if ( (p = strstr(gResponse,"FATAL ERROR:")) != NULL )
	{
		return(-3);
	}

	// successfully created  abfragen
	if ( (p = strstr(gResponse,"successfully created")) == NULL ) return(-4);

	
	// Ret=[0] abfragen
	if ( strstr( gResponse,"Ret=[0]") == NULL ) return(-5);

	
	// Damit die PARK auch im Flash-Filesystem gespeichert wird
	sprintf(mail_buffer,"sync%c",0x0d);
	if(ipRfp_rs232_mail_out(mail_buffer,"# ",NULL)) return(-6);
	
	
	
	// all ok
	return(0);

}


int ipRfp_rs232_readPARK(char *strParkHexNumber)
{

/*
** read PARK Numbers mit Kommando "manu bit"
**
** Return Strings
** parkNumber: XXXXXXXXXX  
**
** @GC 12.10.2012
**
**
*/


int i,n;
char *p;
char *searchStr;
char mail_buffer[256];
char cTemp[256];
	

	strcpy(strParkHexNumber,"");

	
	sprintf(mail_buffer,"cd /%c",0x0d);
	if(ipRfp_rs232_mail_out(mail_buffer,"# ",NULL)) return(-1);
	

	// 	# fmanu bit
	// USB STORAGE                              UNPLUGGED
	// BMC CALIBRATION DATA                     OK
	// BMC STATUS                               RUNNING
	// TEMPERATURE SENSOR                       36 C
	// BOOTDEVICE                               bootimage 0 (via link)
	// RTC (Real Time Clock)                    OK (BOOTTIME: Thu Sep 27 08:37:43 UTC 2012)
	// WLAN MODULE                              NOT FOUND
	// SNTP                                     OK (192.168.99.2)
	// DECT LICENSE INFO                        PARK 26C01873A3, Regulatory Domain US
	// #
	
	// # fmanu bit
	// USB STORAGE                              UNPLUGGED
	// BMC CALIBRATION DATA                     OK
	// BMC STATUS                               RUNNING
	// TEMPERATURE SENSOR                       31 C
	// DECT LICENSE INFO                        ERROR -2
	// BOOTDEVICE                               bootimage 0 (via link)
	// RTC (Real Time Clock)                    OK (BOOTTIME: Thu Sep 27 06:29:11 UTC 2012)
	// WLAN MODULE                              NOT FOUND
	// SNTP                                     OK (192.168.99.2)		
	//
	// Achtung!
	// bei dem "manu bit" oder "fmanu bit" Kommando dauert es 
	// ca 20-30Sec bis die geaenderten Wert angezeigt wird. 
	//
	// nach einem Boot wird dieser String auch erst nach 30 Sec angezeigt
	// DECT LICENSE INFO                        ERROR -2
	// 
	//
	// also geht write und read nicht gleichzeitig
	//
	// In der Ausgabe fehlt eine von beiden Regulatory Domain (ist so gewollt)
	// also kann nur die PARK ueberprueft werden
	//
	//
	
	
	sprintf(mail_buffer,"fmanu bit%c",0x0d);
	if(ipRfp_rs232_mail_out(mail_buffer,"# ",NULL)) return(-2);
	
	
	p=strstr(gResponse,"DECT LICENSE INFO");
	if ( p != NULL)
	{	
		p=p+strlen("DECT LICENSE INFO");
		while ( *p == ' ' || *p == '\t') p++;      // alle Blanks ueberspringen
		for ( i=0; *p != '\n' && *p != '\r' && *p != '\0' && *p != ',' ; i++,p++ ) cTemp[i]=*p;	// kopieren bis ',' oder blank oder ende zeile
		cTemp[i] = '\0';

		// PARK 26C01873A3, Regulatory Domain US
		if ( sscanf( cTemp ,"PARK %s",strParkHexNumber) != 1 )
		{
			strcpy(strParkHexNumber,cTemp);			// "ERROR -2"
		}	
		
	}
	

	
	// all ok
	return(0);

	
}


	
	
	
	



int ipRfp_rs232_readPARKxml(char *strParkHexNumber, char *strParkOctNumber, char *strDECTrDomain, char *strWLANrDomain)
{

/*
** read PARK Numbers mit Kommando "cat /var/opt/omm/PARK.xml"
**
** Return Strings
** parkHexNumber: XXXXXXXXXX  	    1F101873A2
** parkOctNumber: oooooooooooooo	31100303472103
** DECTregulationDomain:            US EU
** WLANregulationDomain:            US DE
**
** @GC 22.09.2012
** @GC 18.03.2014  vorher Auswertung ob PARK.xml vorhanden
**
**
*/

int i;
int n;
char *p;
char *searchStr;
char mail_buffer[256];
char cTemp[256];
	

	if ( strParkHexNumber != NULL ) strcpy(strParkHexNumber,"");
	if ( strParkOctNumber != NULL ) strcpy(strParkOctNumber,"");
	if ( strDECTrDomain != NULL ) strcpy(strDECTrDomain,"");
	if ( strWLANrDomain != NULL ) strcpy(strWLANrDomain,"");
	
	
	sprintf(mail_buffer,"cd /%c",0x0d);
	if(ipRfp_rs232_mail_out(mail_buffer,"# ",NULL)) return(-1);
	
		
	// Example output of the cat command:
	// # cat /var/opt/omm/PARK.xml
	// <SRFPLicenseImport>
	// <!--The PARK for the SRFP (31100303472103)-->
	// <HW PARK="1F-10-18-73-A2" />
	// <!--SRFP license + description-->
	// <LICENSE desc="SRFP DECT reg. domain : US - WLAN reg. domain : US" key="7X3TS-RDLZF-78NDE-PWXTE-F66HG" />
	// <!--Checksum-->
	// <MD5>848134C48667F694EBFEC1C091CD2422</MD5>
	// </SRFPLicenseImport>
	// #	
	

    // testen ob irgend eine PARK.xml vorhanden ist
	sprintf(mail_buffer,"ls -l /var/opt/omm%c",0x0d);
	if(ipRfp_rs232_mail_out(mail_buffer,"# ",NULL)) return(-2);
	if ( (p = strstr(gResponse,"PARK.xml")) != NULL )
	{
		//vorhanden, also mit dummy fuellen
		if ( strParkHexNumber != NULL ) strcpy(strParkHexNumber,"PARK.xml");
		//if ( strParkHexNumber != NULL ) strcpy(strParkHexNumber,"XXXXXXXXXX");
		//if ( strParkOctNumber != NULL ) strcpy(strParkOctNumber,"oooooooooooooo");
	}



    // jetzt die PARK.xml auswerten
	sprintf(mail_buffer,"cat /var/opt/omm/PARK.xml%c",0x0d);
	if(ipRfp_rs232_mail_out(mail_buffer,"# ",NULL)) return(-2);

	
	//------------------------------------------------------
	// <!--The PARK for the SRFP (31100303472103)-->
	searchStr="The PARK for the SRFP (";
	if ( (p = strstr(gResponse,searchStr)) != NULL )
	{
		for ( p=p+strlen(searchStr) ; *p == ' ' || *p == '\t' ; p++ );			// anfang suchen
		for ( i=0; *p != '\n' && *p != '\r' && *p != '\0' && *p != '\t' && *p != ' ' && *p != ')' ; p++ )	// kopieren bis ')' oder blank oder ende zeile
		{	
			cTemp[i]=*p;
			i++;
		}
		cTemp[i]='\0';
		if ( strParkOctNumber != NULL ) strcpy(strParkOctNumber,cTemp);   
	}
	
	//------------------------------------------------------
	// <HW PARK="1F-10-18-73-A2" />
	searchStr="<HW PARK=\"";
	if ( (p = strstr(gResponse,searchStr)) != NULL )
	{
		for ( p=p+strlen(searchStr) ; *p == ' ' || *p == '\t' ; p++ );			// anfang suchen
		for ( i=0; *p != '\n' && *p != '\r' && *p != '\0' && *p != '\t' && *p != ' ' && *p != '"'; p++ )	// kopieren bis '"' oder blank oder ende zeile
		{	
			if ( *p != '-' && *p != ':' )              //aus "1F-10-18-73-A2" wird "1F101873A2"
			{	
				cTemp[i]=*p;
				i++;
			}	
		}
		cTemp[i]='\0';
		if ( strParkHexNumber != NULL ) strcpy(strParkHexNumber,cTemp);   
	}
	

	//------------------------------------------------------
	// <LICENSE desc="SRFP DECT reg. domain : US - WLAN reg. domain : US" key="7X3TS-RDLZF-78NDE-PWXTE-F66HG" />
	searchStr="DECT reg. domain :";
	if ( (p = strstr(gResponse,searchStr)) != NULL )
	{
		for ( p=p+strlen(searchStr) ; *p == ' ' || *p == '\t' ; p++ );			// anfang suchen
		for ( i=0; *p != '\n' && *p != '\r' && *p != '\0' && *p != '\t' && *p != ' ' && *p != '"' ; p++ )	// kopieren bis '"' oder blank oder ende zeile
		{	
			cTemp[i]=*p;
			i++;
		}
		cTemp[i]='\0';
		if ( strDECTrDomain != NULL ) strcpy(strDECTrDomain,cTemp);   
	}
	
	//------------------------------------------------------
	// <LICENSE desc="SRFP DECT reg. domain : US - WLAN reg. domain : US" key="7X3TS-RDLZF-78NDE-PWXTE-F66HG" />
	searchStr="WLAN reg. domain :";
	if ( (p = strstr(gResponse,searchStr)) != NULL )
	{
		for ( p=p+strlen(searchStr) ; *p == ' ' || *p == '\t' ; p++ );			// anfang suchen
		for ( i=0; *p != '\n' && *p != '\r' && *p != '\0' && *p != '\t' && *p != ' ' && *p != '"' ; p++ )	// kopieren bis '"' oder blank oder ende zeile
		{	
			cTemp[i]=*p;
			i++;
		}
		cTemp[i]='\0';
		if ( strWLANrDomain != NULL ) strcpy(strWLANrDomain,cTemp);   
	}

	
	// for debug
	//if ( strParkHexNumber != NULL && strParkOctNumber != NULL && strDECTrDomain != NULL && strWLANrDomain != NULL )
	//{	
	//	sprintf(cTemp,"ParkHexNumber=[%s]\nParkOctNumber=[%s]\nDECTregulationDomain=[%s]\nWLANregulationDomain=[%s]",strParkHexNumber, strParkOctNumber, strDECTrDomain, strWLANrDomain);
	//    MessagePopup("ipRfp_rs232_readPARKxml", cTemp );
	//}
	//else if ( strParkHexNumber != NULL && strDECTrDomain != NULL && strWLANrDomain != NULL )
	//{	
	//	sprintf(cTemp,"ParkHexNumber=[%s]\nParkOctNumber=[----]\nDECTregulationDomain=[%s]\nWLANregulationDomain=[%s]",strParkHexNumber, strDECTrDomain, strWLANrDomain);
	//    MessagePopup("ipRfp_rs232_readPARKxml", cTemp );
	//}

	
	
	
	// all ok
	return(0);

}





int ipRfp_rs232_writeConfig(char *mac, char *hwType, char *branding, char *sernr, char *exid)
{

int i;
int n;
char *p;
char mail_buffer[256];
char strValue[256];
char expValue[256];
char *searchStr;

	
	//
	//  Write Mac, hwType, Branding, SerNr, Exid
	//  in OTP Flash Memory
	//
	//  Type=(RFP35|RFP36|RFP43)
	//  Branding=(ffsip|none)
	//
	//manu conf "00:30:42:12:90:00" "RFP35" "ffsip" "R9 100000714C" "9500000001"
	//#
	
	sprintf(mail_buffer,"cd /%c",0x0d);
	if(ipRfp_rs232_mail_out(mail_buffer,"# ",NULL)) return(-1);

	
	//manu conf "00:30:42:12:90:00" "RFP35" "ffsip" "R9 100000714C" "9500000001"
	sprintf(mail_buffer,"manu conf \"%s\" \"%s\" \"%s\" \"%s\" \"%s\"%c",mac,hwType,branding,sernr,exid,0x0d);
	if(ipRfp_rs232_mail_out(mail_buffer,"# ",NULL)) return(-2);


	if ( (p = strstr(gResponse,"FATAL ERROR:")) != NULL )
	{
		return(-3);
	}


	
	// all ok
	return(0);

	
}

int ipRfp_rs232_readConfig(char *mac, char *hwType, char *branding, char *sernr, char *exid)
{

/*
** Return Strings
**
** mac:        00:30:42:12:69:9f  --> 00:30:42:12:69:9F   wird nach UpCase gewandelt, da UpCase geschrieben wird
** hwType:     RFP 43             --> RFP43      das Blank wird entfernt, da auch ohne Blank geschrieben wird  nur UpCase zulässig
** hwBranding: FFSIP              --> ffsip      wird nach LowCase gewandelt, da auch nur mit LowCase geschrieben wird
**             FFSIP(SINGLE CELL) --> ffsip_sc   aus FFSIP(SINGLE CELL) wird ffsip_sc  
** string 1:   R9 100000714C            so wie es ist
** string 2:   9500848378			    so wie es ist
*/


int i;
int n;
char *p;
char mail_buffer[256];
char *searchStr;
	
	mac[0]='\0';
	hwType[0]='\0';
	branding[0]='\0';
	sernr[0]='\0';
	exid[0]='\0';
	
	sprintf(mail_buffer,"cd /%c",0x0d);
	if(ipRfp_rs232_mail_out(mail_buffer,"# ",NULL)) return(-1);
	
	
	//manu conf
	sprintf(mail_buffer,"manu conf%c",0x0d);
	if(ipRfp_rs232_mail_out(mail_buffer,"# ",NULL)) return(-4);

	//# manu conf
	//current mac:       00:30:42:12:69:92
	//current hw:        RFP 43
	//current branding:  FFSIP
	//current string 1:  R9 100000714C
	//current string 2:  9500848378
	//# 
	
	
	//# manu conf
	//current mac:       00:30:42:17:99:0d
	//current hw:        RFP 35
	//current branding:  FFSIP(SINGLE CELL)
	//current string 1:  C7 3000089941
	//current string 2:  9508037561
	
	
	//----------------------------------
	//current mac:       00:30:42:12:69:92
	//current mac:       00:30:42:12:69:9f
	searchStr= "current mac:";
	
	if ( (p = strstr(gResponse,searchStr)) != NULL )
	{
		for ( p=p+strlen(searchStr) ; *p == ' ' ; p++ );			// anfang suchen
		for ( i=0; *p != '\n' && *p != '\r' && *p != '\0'; p++ )	// kopieren bis ende zeile
		{	
	    	mac[i]=toupper(*p);						// mac wird mit UpCchar geschrieben, in ifconfig steht auch UpChar
			i++;									// hier wird beim lesen nur LowChar ausgegeben
	    	mac[i]='\0';							// also nach Up wandeln
		}	
	}	

	
	//----------------------------------
	//current hw:        RFP 43
	searchStr="current hw:";
	
	if ( (p = strstr(gResponse,searchStr)) != NULL )
	{
		for ( p=p+strlen(searchStr) ; *p == ' ' ; p++ );			// anfang suchen
		for ( i=0; *p != '\n' && *p != '\r' && *p != '\0'; p++ )	// kopieren bis ende zeile
		{	
			if ( *p != ' ' )                        //aus "RFP 43" wird "RFP43"
			{	
				hwType[i]=*p;
				i++;
				hwType[i]='\0';
			}	
		}	
	}
	//----------------------------------
	//current branding:  FFSIP
	searchStr="current branding:";
	
	if ( (p = strstr(gResponse,searchStr)) != NULL )
	{
		for ( p=p+strlen(searchStr) ; *p == ' ' ; p++ );			// anfang suchen
		for ( i=0; *p != '\n' && *p != '\r' && *p != '\0'; p++ )	// kopieren bis ende zeile
		{	
			branding[i]=tolower(*p);                 // ffsip,none wird in LowerChar geschrieben  zurück wandeln, FFSIP=>ffsip NONE->none
			i++;									 // nur LowerChar zulässig
			branding[i]='\0';
		}
		
		
		// @GC 22.10.2012
		// aus FFSIP(SINGLE CELL)  wird ffsip_sc
		if ( strcmp(branding,"ffsip(single cell)") == 0 )
		{	
			strcpy(branding,"ffsip_sc");
		}	
		
	}	
	
	//----------------------------------
	//current string 1:  R9 100000714C
	searchStr="current string 1:";
	
	if ( (p = strstr(gResponse,searchStr)) != NULL )
	{
		for ( p=p+strlen(searchStr) ; *p == ' ' ; p++ );				// anfang suchen
		for ( i=0; *p != '\n' && *p != '\r' && *p != '\0'; p++)			// kopieren bis ende zeile
		{	
	    	sernr[i]=*p;
			i++;
	    	sernr[i]='\0';
		}	
	}	

	//----------------------------------
	//current string 2:  9500848378
	searchStr="current string 2:";
	
	if ( (p = strstr(gResponse,searchStr)) != NULL )
	{
		for ( p=p+strlen(searchStr) ; *p == ' ' ; p++ );				// anfang suchen
		for ( i=0; *p != '\n' && *p != '\r' && *p != '\0'; p++)			// kopieren bis ende zeile
		{	
	    	exid[i]=*p;
			i++;
	    	exid[i]='\0';
		}	
	}	
	
	
	// all ok
	return(0);

	
}

int ipRfp_rs232_enableRSX(void)
{

char mail_buffer[256];
int i;
int n;
char *p;
/*
   Als Alternative das Consol Kommando verwenden
   In der Systemshell bitte eingeben :
  # conclient -p 10318 -x "rsx on"
//   (Hinweis unserer SWer : Fehlermeldungen beim Aufruf bitte ignorieren)
    
*/


	sprintf(mail_buffer,"cd /%c",0x0d);
	if(ipRfp_rs232_mail_out(mail_buffer,"# ",NULL)) return(-1);
	
	sprintf(mail_buffer,"conclient -p 10318 -x \"rsx on\"%c",0x0d);
	if(ipRfp_rs232_mail_out(mail_buffer,"# ",NULL)) return(-2);
	
	// port 38477 open for communication with RSX
	if ( strstr(gResponse,"/var/opt/console/10318") == NULL )
		return(-4);         // fail
	
	
	return(0);

	
}



int ipRfp_rs232_enableSSH(void)
{

char mail_buffer[256];
int i;
int n;
char *p;

	//  # manu ssh enable
	// Dauerhafte Aktivierung, also über Reset hinweg, erfolgt durch 
	// manu ssh enable
	// Deaktivierung erfolgt durch
	// manu ssh disable
	// Oder
	// (f)manu factoryreset
	// test:
	//# killall dropbear
	//# manu ssh disable
	//# sleep 1
	//# manu ssh enable
	//# ps |grep dropbear
  	//899 root      1352 S    /usr/sbin/dropbear -b /var/run/ip_rfp/banner.txt -F -
	
	
	sprintf(mail_buffer,"cd /%c",0x0d);
	if(ipRfp_rs232_mail_out(mail_buffer,"# ",NULL)) return(-1);
	

	sprintf(mail_buffer,"manu ssh enable%c",0x0d);
	if(ipRfp_rs232_mail_out(mail_buffer,"# ",NULL)) return(-2);

	// sicherstellen das auch alles im Flash Filesystem ist
	// denn SSH enable muss ueber das Reset hinaus aktiviert werden, auch für den Final-Test
	sprintf(mail_buffer,"sync%c",0x0d);
	if(ipRfp_rs232_mail_out(mail_buffer,"# ",NULL)) return(-3);


	// check if ssh Prozess is running
	for ( p=NULL, n=10; n > 0 ; n-- )
	{	
		Sleep(1000);     // 1 sec
		sprintf(mail_buffer,"ps | grep dropbear%c",0x0d);
		if(ipRfp_rs232_mail_out(mail_buffer,"# ",NULL)) return(-4);
	
		// /dropbear ist am laufen
		if ( (p=strstr(gResponse,"/usr/sbin/dropbear")) != NULL ) 
		{
			break;  // ja
		}
	}		
	if ( p == NULL ) return(-5);   		// /ssh Prozess ist nicht am laufen
	return(0);

	
}

int ipRfp_rs232_sendMACandSerial(char* mac, char* serial)
{

char mail_buffer[256] = {0};
char mail_response[1024] = {0};
char cHW[10] = {0};
char *p, *searchStr;
int i;




	/*sprintf(mail_buffer,"manu conf -mac %s\n", mac);
	if(ipRfp_rs232_mail_out(mail_buffer,"# ",NULL)) return(-1);

	sprintf(mail_buffer,"manu conf -sn %s\n", serial);
	if(ipRfp_rs232_mail_out(mail_buffer,"# ",NULL)) return(-1);  */
	
	sprintf(mail_buffer,"manu conf -mac %s\n", mac);  
	if(ipRfp_rs232_mail_out_V2(mail_buffer,"# ",NULL,mail_response)) return(-1);
	if(strstr (mail_response, "unknown") != NULL)   return(-1); 
	if(strstr (mail_response, "mismatch") != NULL)   return(-1); 
	
	if(strstr (mail_response, "not") != NULL)   return(-1); 
	
	if(strstr (mail_response, "Error") != NULL)   return(-1);  

	sprintf(mail_buffer,"manu conf -sn %s\n", serial);
	if(ipRfp_rs232_mail_out_V2(mail_buffer,"# ",NULL,mail_response)) return(-1);
	
	
	if(strstr (mail_response, "unknown") != NULL)   return(-1);
	
	if(strstr (mail_response, "mismatch") != NULL)   return(-1); 
	
	if(strstr (mail_response, "not") != NULL)   return(-1); 
	
	if(strstr (mail_response, "Error") != NULL)   return(-1);   
	
	
	
	sprintf(mail_buffer,"manu conf\n");   
	if(ipRfp_rs232_mail_out_V2(mail_buffer,"# ",NULL,mail_response)) return(-1);
	
	
	/////////////////////////////////
	//get HW type:
	searchStr= "hw      :  ";
	
	if ( (p = strstr(mail_response,searchStr)) != NULL )
	{
		for ( p=p+strlen(searchStr) ; *p == ' ' ; p++ );			
		for ( i=0; *p != '\n' && *p != '\r' && *p != '\0'; p++ )	
		{	
	    	cHW[i]=toupper(*p);					 
			i++;								
	    	cHW[i]='\0';						
		}	
	}	
	
	if(strncmp(cHW,"RFP44",5) == 0)
	{
		if(strncmp(serial, "3BY",3) != 0)
			return(-1); 	
	}
	else if(strncmp(cHW,"RFP45",5) == 0)
	{
		if((strncmp(serial, "3BX",3) != 0) && (strncmp(serial, "3HG",3) != 0) && (strncmp(serial, "3E7",3) != 0)&& (strncmp(serial, "3L9",3) != 0))
			return(-1); 
	}
	else if(strncmp(cHW,"RFP47",5) == 0)
	{
		if((strncmp(serial, "3BZ",3) != 0)  &&  (strncmp(serial, "3C1",3) != 0))
			return(-1);
	}
	else if(strncmp(cHW,"RFP48",5) == 0)
	{
		if(strncmp(serial, "3C2",3) != 0)
			return(-1);
	}
	else
	{
		return(-1); 
		
	}
	/////////////////////////////////
	
	
	//readback check
	
	
	
	
	//sprintf(mail_buffer,"manu conf\n");   
	//if(ipRfp_rs232_mail_out_V2(mail_buffer,"# ",NULL,mail_response)) return(-1);
	
	
	StringUpperCase (mail_response);
	
	if(strstr (mail_response, mac) == NULL)   return(-1);
	
	if(strstr (mail_response, serial) == NULL)   return(-1); 
	
/*	sprintf(mail_buffer,"ifconfig\n");   
	if(ipRfp_rs232_mail_out_V2(mail_buffer,"# ",NULL,mail_response)) return(-1);
	
	
	StringUpperCase (mail_response);
	if(strstr (mail_response, mac) == NULL)   return(-1);   */

	return(0);
}



int ipRfp_rs232_sendBootFlashImage()
{

char mail_buffer[256] = {0};
char mail_response[1024] = {0};

	
	sprintf(mail_buffer,"cat /proc/cmdline\n");  
	if(ipRfp_rs232_mail_out_V2(mail_buffer,"# ",NULL,mail_response)) return(-1);
	if(strstr (mail_response, "RFP4G_UBootVer=0.5") == NULL)   return(-1); 


	return(0);
}


int ipRfp_rs232_sendHalt(void)
{

char mail_buffer[256];
int i;
int n;
char *p;

	// Wenn die Spannung abgezogen wird, kann es sein, dass noch nicht alle Prozesse abgeschlossen sind.
	// Dies wird mit dem zusätzlichen Kommando "halt" erreicht. Wir nehmen dieses als letzen Befehl (vor dem Abziehen der Spannungsversorgung im Funktionstest)
    // Die Systemmeldungen über die Serielle Schnittstelle sehen so aus:
	//# halt
	//The system is going down NOW!
	//Sending SIGTERM to all processes
	//Requesting system halt
	//System halted.


	//Wenn der Befehl "System halted." kommt, ist das System runtergefahren (und führt einen Watchdog-Reset nach kurzer Zeit durch)
	//Wichtig: Die Funktionalität ist hier nur für die Serielle Schnittstelle gegeben. (die Ethernet-Prozesse werden auch mit abgeschaltet)
	sprintf(mail_buffer,"cd /%c",0x0d);
	if(ipRfp_rs232_mail_out(mail_buffer,"# ",NULL)) return(-1);
	sprintf(mail_buffer,"halt%c",0x0d);
	if(ipRfp_rs232_mail_out(mail_buffer,"The system is going down NOW!",NULL)) return(-2);
	// wait for "System halted." 
	if(ipRfp_rs232_WaitForBootUp(10 /*sec*/, "System halted", NULL, NULL, NULL,NULL )!=1) return(-3);
	return(0);
}


int ipRfp_rs232_getFinalizeState(char *respBuffer)
{

char mail_buffer[256];
int i;
int n;
char *p;

//# minirsx
//minirsx v.1.0.0  100701
//Connected to 127.0.0.1
//
//MainMenu
//  1 <G>et Version
//  2 Adjust <V>oltage
//  3 Adjust <C>lock
//  4 <E>nter TestMode
//  5 Get Compile <T>ime
//  6 Ee<p>rom
//  7 E<x>it
//
//cmd :6
//
//Select Eeprom command
//  0 <F>inalize
//  1 <G>et Finalization Status
//  2 <C>ancel command
//
//Selection ? :g
//
//   > EEPROM NOT finalized
//
//cmd ('?' for help) :x
//
//   > end
//  #

	
	// EEPROM ALREADY finalized
	// EEPROM NOT finalized
	strcpy(respBuffer,"xxxxxxxx");
	sprintf(mail_buffer,"cd /%c",0x0d);
	if(ipRfp_rs232_mail_out(mail_buffer,"# ",NULL)) return(-1);
	sprintf(mail_buffer,"minirsx%c",0x0d);
	if(ipRfp_rs232_mail_out(mail_buffer,"cmd :",NULL)) return(-2);
	sprintf(mail_buffer,"p");
	if(ipRfp_rs232_mail_out(mail_buffer,"Selection ? :",NULL)) return(-3);
	sprintf(mail_buffer,"g");
	if(ipRfp_rs232_mail_out(mail_buffer," :",NULL)) return(-4);
	p = strstr(gResponse,"  > ");
	if ( p != NULL )
	{
		for ( i=0, p=p+strlen("  > "); *p != '\n' && *p != '\r' && *p != '\0'; p++)			// kopieren bis ende zeile
		{	
    		respBuffer[i]=*p;
			i++;
    		respBuffer[i]='\0';
		}
	}	
	sprintf(mail_buffer,"x");
	if(ipRfp_rs232_mail_out(mail_buffer,"# ",NULL)) return(-5);
	return(0);
}

int ipRfp_rs232_setFinalizeState(void)
{

char mail_buffer[256];
int i;
int n;
char *p;


//# minirsx
//minirsx v.1.0.0  100701
//Connected to 127.0.0.1
//
//MainMenu
//  1 <G>et Version
//  2 Adjust <V>oltage
//  3 Adjust <C>lock
//  4 <E>nter TestMode
//  5 Get Compile <T>ime
//  6 Ee<p>rom
//  7 E<x>it
//
//cmd :p
//
//Select Eeprom command
//  0 <F>inalize
//  1 <G>et Finalization Status
//  2 <C>ancel command
//
//Selection ? :f
//
//  > Eeprom finalized
//
//cmd ('?' for help) :x
//
//   > end
//#
	sprintf(mail_buffer,"cd /%c",0x0d);
	if(ipRfp_rs232_mail_out(mail_buffer,"# ",NULL)) return(-1);
	sprintf(mail_buffer,"minirsx%c",0x0d);
	if(ipRfp_rs232_mail_out(mail_buffer,"cmd :",NULL)) return(-2);
	sprintf(mail_buffer,"p");
	if(ipRfp_rs232_mail_out(mail_buffer,"Selection ? :",NULL)) return(-3);
	sprintf(mail_buffer,"f");
	if(ipRfp_rs232_mail_out(mail_buffer," :",NULL)) return(-4);
	sprintf(mail_buffer,"x");
	if(ipRfp_rs232_mail_out(mail_buffer,"# ",NULL)) return(-5);
	return(0);
}


int ipRfp_rs232_Open (int ComPort,int BaudRate, int TimeOut /*milliSec*/)
{
int i;
double temp;
char   cInfo[256];

//@JM04,07.11
//clear logfile
#ifdef BIT_IP_RFP35_DEBUG_RS232
	FILE *fp;
	fp=fopen(BIT_IP_RFP35_DEBUG_RS232, "w");
	fclose(fp);	    
#endif
	if ( OpenComConfig ( ComPort, "" /*devName*/, BaudRate, 0 /*Parity*/, 8 /*dataBits*/, 1/*stopBits*/, 1024/*inQueueSize*/, 1024/*outQueueSize*/) )
	{
		sprintf(cInfo,"Can not open COM%d !!",rs232_RFP.port);
		MessagePopup ("ipRfp_OpenRS232",cInfo);
		return(-1);
	}
	rs232_RFP.port = ComPort;
	rs232_RFP.baudrate = BaudRate;
	temp = (double)TimeOut;
	temp /= 1000;
	SetComTime (rs232_RFP.port, temp   /*timeout_sec*/ );
	SetCTSMode (rs232_RFP.port, LWRS_HWHANDSHAKE_OFF);			//@GC 04.11.2011
	SetXMode (rs232_RFP.port, 0);   // off						//@GC 04.11.2011
	FlushInQ (rs232_RFP.port);		//@JM 04.07.11
	FlushOutQ (rs232_RFP.port);		//@JM 04.07.11
	return(0);
}	


int ipRfp_rs232_WaitForBootUp(int timeOut, char* bootstringA,char* bootstringB,char* bootstringC,char* bootstringD,char* bootstringE)   //timeOut in Sec
{

// sucht nach Teilstrings in der Bootausgabe auf der Rs232
// es kann nach alternativen bootstringA bis bootstringE gesucht werden
// welcher dann gefunden wurde wird als nummer zurueck gegeben
// 	bootstringA == 1
// 	bootstringB == 2
// 	bootstringC == 3
// 	bootstringD == 4
// 	bootstringE == 5
	
	
//SoC:   Kirkwood 88F6281_A1
//==== booting from UBI ====
//## Booting kernel	
//Please press Enter to activate	

int     found_nr;	
char	*chrptr;
int 	timeCount;
int 	count;
char 	charstr[20];

#ifdef BIT_IP_RFP35_DEBUG_RS232
FILE *fp;
fp=fopen(BIT_IP_RFP35_DEBUG_RS232, "a");
#endif

	timeCount=timeOut * 1000 / 100;               // aus Sekunde einen Counter machen

	count=0;	    
	found_nr=0;
 	FlushInQ (rs232_RFP.port);	
	while ( timeCount > 0 && count < sizeof(gResponse))
	{
		Sleep(100);          // 100ms
		while ( GetInQLen (rs232_RFP.port) )
		{
			if ( ComRd(rs232_RFP.port,&(gResponse[count]),1) == 1 )		// one char 500ms
			{
			
    			if ( gResponse[count] == '\0' ) gResponse[count]='x';
#ifdef BIT_IP_RFP35_DEBUG_RS232
				fputc(gResponse[count],fp);
#endif			
				if(Print2StandardIO)
					printf("%c",gResponse[count]);

				count++;                                            // 1 Zeichen 
				if ( timeCount < (1*1000/100) )
					timeCount=(1*1000/100);                         // timeCount neu starten da Zeichen gekommen, danach immer 1 Sek warten 
	    		gResponse[count]='\0';                                  // '\0' anhaengen   
				
	    		if (      bootstringA != NULL && (chrptr = strstr(gResponse,bootstringA)) != NULL )           // sucht den bootsring
					found_nr=1;      // gefunden
	    		else if ( bootstringB != NULL && (chrptr = strstr(gResponse,bootstringB)) != NULL )
					found_nr=2;      // gefunden
	    		else if ( bootstringC != NULL && (chrptr = strstr(gResponse,bootstringC)) != NULL )
					found_nr=3;      // gefunden
	    		else if ( bootstringD != NULL && (chrptr = strstr(gResponse,bootstringD)) != NULL )
					found_nr=4;      // gefunden
	    		else if ( bootstringE != NULL && (chrptr = strstr(gResponse,bootstringE)) != NULL )
					found_nr=5;      // gefunden
			}
			else
			{
			    //break;
			}  
			if ( found_nr > 0 )
				break;
    	}
		if ( found_nr > 0 )
			break;
		
   		timeCount--;
	}    
    
#ifdef BIT_IP_RFP35_DEBUG_RS232
	fclose(fp);	    
#endif	
	 
return(found_nr);       // 0= nicht gefunden   1=A-gefunden 2=B-gefunden usw
}



int ipRfp_rs232_set_timeout (int TimeOut)
{
double temp;

	temp = (double)TimeOut;
	temp /= 1000;
	return(SetComTime (rs232_RFP.port, temp));
}

void ipRfp_rs232_Close (void)
{
	CloseCom (rs232_RFP.port);
	rs232_RFP.port=0;
	rs232_RFP.baudrate=0;
	return;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////  RSX  ///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef BIT_IP_RFP35_DEBUG_RSX
//--------------------------------------------------------------------------
static char *ipRfp_rsx_GetMessageCMD(int rsxcmd)
{
	struct ipRfp_rsxCmd {
		int  cmd;
		char *pText;
	};


	static struct ipRfp_rsxCmd  ipRfp_rsxMessage[] = 
	{
		{ 0xf004, "CMD_ENTER_TESTMODE" },
		{ 0xf000, "RSP_INITIALIZATION_FINISHED" },
		{ 0xf005, "CMD_EXIT_TESTMODE" },
		{ 0xf008, "CMD_GET_VERSION" },
		{ 0xf009, "RSP_GET_VERSION" },
		{ 0xf11c, "CMD_READ_EEP_DEVIATION" },
		{ 0xf11d, "RSP_READ_EEP_DEVIATION" },
		{ 0xf11e, "CMD_WRITE_EEP_DEVIATION" },
		{ 0xf11f, "RSP_WRITE_EEP_DEVIATION" },
		{ 0xf113, "CMD_SET_DEVIATION" },
		{ 0xfe03, "RSP_SET_DEVIATION" },
		{ 0xf10d, "CMD_SET_DIVERSITY" },
		{ 0xfe01, "RSP_SET_DIVERSITY" },
		{ 0xf104, "CMD_SETUP_DUMMY_BEARER" },
		{ 0xfe00, "RSP_SETUP_DUMMY_BEARER" },
		{ 0xf10b, "CMD_GET_RSSI" },
		{ 0xf10c, "RSP_GET_RSSI" },
		{ 0xf042, "CMD_WRITE_EEPROM" },
		{ 0xf043, "RSP_WRITE_EEPROM" },
		{ 0xf040, "CMD_READ_EEPROM" },
		{ 0xf041, "RSP_READ_EEPROM" },
		{ 0xf107, "CMD_SET_B_FIELD" },
		{ 0xfe04, "RSP_SET_B_FIELD" },
		{ 0xf156, "CMD_SET_CLOCK" },
		{ 0xfe05, "RSP_SET_CLOCK" },
		{ 0xf15d, "CMD_READ_EEP_CLOCK" },
		{ 0xf15e, "RSP_READ_EEP_CLOCK" },
		{ 0xf15f, "CMD_WRITE_EEP_CLOCK" },
		{ 0xf160, "RSP_WRITE_EEP_CLOCK" },
		{ 0xf14b, "CMD_SET_BANDGAP" },
		{ 0xfe06, "RSP_SET_BANDGAP" },
		{ 0xf167, "CMD_READ_EEP_BANDGAP" },
		{ 0xf168, "RSP_READ_EEP_BANDGAP" },
		{ 0xf169, "CMD_WRITE_EEP_BANDGAP" },
		{ 0xf16a, "RSP_WRITE_EEP_BANDGAP" },
		{ 0xf044, "CMD_FINALIZE_EEPROM" },
		{ 0xf045, "RSP_FINALIZE_EEPROM" },
		{ 0xf132, "CMD_SET_FREQUENCY_BAND" },
		{ 0xf133, "RSP_SET_FREQUENCY_BAND" },
		{ 0xf130, "CMD_SET_POWER_LEVEL" },
		{ 0xf131, "RSP_SET_POWER_LEVEL" },
		{ 0xf00e, "RSP_PRINTF" },	                //FreeRunning Text
		{ 0x0000, "UNKNOWN" },
	};	

	int i;

	for (i=0 ; ipRfp_rsxMessage[i].cmd != 0  ; i++)
	{
		if ( rsxcmd == ipRfp_rsxMessage[i].cmd )
		{
			return(ipRfp_rsxMessage[i].pText);
		}	
	}
	
	return(ipRfp_rsxMessage[i].pText);

}
#endif //BIT_IP_RFP35_DEBUG_RSX



// Mail Out Funktion über IP SOCKET
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int ipRfp_rsx_mail_out (int write_count,unsigned char *buffer,int read_count)
{
int i,len;
int local_error=0;

#if defined(BIT_IP_RFP35_DEBUG_RSX) && defined(BIT_IP_RFP35_DEBUG_RSX_TIME) 
	unsigned long t;
#endif

#ifdef BIT_IP_RFP35_DEBUG_RSX
int  iTemp;
char cTemp[80];
char cInfo[ (sizeof(ip_struct.rx_buffer) * 5) + 256 ];  //genug Platz um den ganzen rx_buffer in hex "0x00 " darzustellen

FILE *fp;
fp=fopen(BIT_IP_RFP35_DEBUG_RSX, "a");
#endif


#if defined(BIT_IP_RFP35_DEBUG_RSX) && defined(BIT_IP_RFP35_DEBUG_RSX_TIME) 
		t=GetTickCount();
#endif			


		ip_struct.tx_buffer[0] = write_count+7;				//size
		ip_struct.tx_buffer[1] = 0x00;                      //reserved
		ip_struct.tx_buffer[2] = ip_struct.tx_buffer[0]-2;  //length
		ip_struct.tx_buffer[3] = 0x00;					    //reserved
		ip_struct.tx_buffer[4] = 0x00;                      //reserved
		ip_struct.tx_buffer[5] = mail_para.primitiv & 0xff; //low-byte
		ip_struct.tx_buffer[6] = mail_para.primitiv >> 8; 	//high-byte
		if(write_count)
			memcpy (&ip_struct.tx_buffer[7], buffer, write_count);

		
		
//chksum:
		ip_struct.tx_count = write_count+7;
		ip_struct.tx_buffer[ip_struct.tx_count] = 0x00;
		for(i=3;i<ip_struct.tx_count;i++)
			ip_struct.tx_buffer[ip_struct.tx_count] ^= ip_struct.tx_buffer[i];
		ip_struct.tx_count++;
//chksum ready
		
#ifdef BIT_IP_RFP35_DEBUG_RSX
		//sprintf(cInfo,"COMD 0x%04x ", (ip_struct.tx_buffer[5]|ip_struct.tx_buffer[6]<<8) );
		sprintf(cInfo,"COMD %s[0x%04x] ", ipRfp_rsx_GetMessageCMD(ip_struct.tx_buffer[5]|ip_struct.tx_buffer[6]<<8),(ip_struct.tx_buffer[5]|ip_struct.tx_buffer[6]<<8) );
		for (iTemp=0; iTemp<ip_struct.tx_count; iTemp++)
		{
			sprintf(cTemp,"0x%02x ",ip_struct.tx_buffer[iTemp]);
			strcat(cInfo,cTemp);
		}
		fprintf(fp,"\n%s\n",cInfo);
		fclose(fp);	    
#endif
		
		ipRfp_rsx_SendDataToRfp(ip_struct.tx_count, ip_struct.tx_buffer);
		
#if defined(BIT_IP_RFP35_DEBUG_RSX) && defined(BIT_IP_RFP35_DEBUG_RSX_TIME) 
		fp=fopen(BIT_IP_RFP35_DEBUG_RSX, "a");
		fprintf(fp,"SendDataToRfp(%d)=%dms\n",ip_struct.tx_count,GetTickCount()-t); 
		fclose(fp);
		t=GetTickCount();
#endif			
		
		
//if required, read answer
		if(read_count != 0)
		{
			// @GC 17.08.2011
			len = ipRfp_rsx_ReceiveDataFromIpRfp( sizeof(ip_struct.rx_buffer), ip_struct.rx_buffer);
				
#ifdef BIT_IP_RFP35_DEBUG_RSX
			fp=fopen(BIT_IP_RFP35_DEBUG_RSX, "a");

			if ( len > 3 )
				//sprintf(cInfo,"RESP 0x%04x ", (ip_struct.rx_buffer[3]|ip_struct.rx_buffer[4]<<8) );
				sprintf(cInfo,"RESP %s[0x%04x] ", ipRfp_rsx_GetMessageCMD(ip_struct.rx_buffer[3]|ip_struct.rx_buffer[4]<<8),(ip_struct.rx_buffer[3]|ip_struct.rx_buffer[4]<<8) );
			else
				sprintf(cInfo,"RESP ");
			
			for (iTemp=0; iTemp<len; iTemp++)
			{
				sprintf(cTemp,"0x%02x ",ip_struct.rx_buffer[iTemp]);
				strcat(cInfo,cTemp);
			}
			fprintf(fp,"%s\n",cInfo);
			fflush(fp);
					
			if ( len > 3 )
			{
				sprintf(cInfo,"            RESP_ASCII ");
				for (iTemp=0; iTemp<len; iTemp++)
				{
					cTemp[0]='.';
					cTemp[1]='\0';	
					if ( ip_struct.rx_buffer[iTemp] >= ' ' && ip_struct.rx_buffer[iTemp] <= '~' )
						cTemp[0]=ip_struct.rx_buffer[iTemp];
					
					strcat(cInfo,cTemp);
				}
				fprintf(fp,"%s\n",cInfo);
			}
			if ( len > 3 )
			{
				sprintf(cInfo,"            RESP_DECODE ");
				for (iTemp=0; iTemp<len;  )
				{
					sprintf(cTemp,"len=0x%02x %s[0x%04x] ",ip_struct.rx_buffer[0+iTemp], ipRfp_rsx_GetMessageCMD(ip_struct.rx_buffer[3+iTemp]|ip_struct.rx_buffer[4+iTemp]<<8), (ip_struct.rx_buffer[3+iTemp]|ip_struct.rx_buffer[4+iTemp]<<8) );
					strcat(cInfo,cTemp);
					iTemp+= 1 + ip_struct.rx_buffer[0+iTemp];
					
				}
				fprintf(fp,"%s\n",cInfo);
			}
			fclose(fp);	    
#endif

			
#if defined(BIT_IP_RFP35_DEBUG_RSX) && defined(BIT_IP_RFP35_DEBUG_RSX_TIME) 
			fp=fopen(BIT_IP_RFP35_DEBUG_RSX, "a");
			fprintf(fp,"ReceiveDataFromIpRfp(%d)=%dms\n", len ,GetTickCount()-t); 
			fclose(fp);
#endif			
			

			if( len < read_count ) local_error++;   // nur noch auf kleiner kleiner abfragen
			
			
		} //if(read_count != 0)	

	
	return(local_error);
}


//--------------------------------------------------------------------------

/*
**********************************************************************************
** EnterTestmode
**
*/
int ipRfp_rsx_EnterTestmode()
{
char mail_buffer[6];  
int  addlen=0;

	mail_para.task_ID  = TB_HANDLE_TASK;
	mail_para.primitiv = TEST_ENTER_TESTMODE;
	mail_buffer[0] = mail_para.pID;

//send out & recieve initialisation finished:	
	//if(ipRfp_rsx_mail_out (1,mail_buffer,9))  //read 9bytes back
	if(ipRfp_rsx_mail_out (1,mail_buffer,9))  //read 9bytes back
		return(-1);
	
//check aditional message   // [@GC07052012]
	while( (ip_struct.rx_buffer[3+addlen]!=0x00)||(ip_struct.rx_buffer[4+addlen]!=0xf0) )
	{   // additional message
	    if ( ip_struct.rx_buffer[0+addlen]==0 ) break;
	    addlen+=1+(unsigned char)ip_struct.rx_buffer[0+addlen];   // len from additional message
	}
	
//check confirmation
	if((ip_struct.rx_buffer[3+addlen]!= 0x00) ||(ip_struct.rx_buffer[4+addlen]!= 0xf0))
		return(-2);  

	if( ip_struct.rx_buffer[0+addlen] != 8 )  //check resp length
		return(-3);  

	return(0);						
}



/*
**********************************************************************************
** ExitTestmode
**
*/
int ipRfp_rsx_ExitTestmode()
{
char mail_buffer[6];   

	mail_para.task_ID = TB_HANDLE_TASK;
	mail_para.primitiv = TEST_EXIT_TESTMODE;
	mail_buffer[0] = mail_para.pID;
 //send out & recieve initialisation finished:	
	if(ipRfp_rsx_mail_out (1,mail_buffer,0))  //read 3bytes back
		return(-1);
	Sleep(1000);     // Delay(1.0);	  // 1Sec  reboot
	return(0);						
}



/*
**********************************************************************************
** GetSoftwareVersion_Retry
**
*/
char *ipRfp_rsx_GetSoftwareVersion_Retry(char *buffer, int retry)
{
	char *p;
	
	while( (p=ipRfp_rsx_GetSoftwareVersion(buffer)) == NULL )
	{
		if(retry <= 0)
		{
			return(p);      //error
		}
		retry--;
		Sleep(200);		// 200ms
	}
	return(p);          // ok
}

/*
**********************************************************************************
** GetSoftwareVersion
**
*/
char *ipRfp_rsx_GetSoftwareVersion(char *buffer)
{
char mail_buffer[6]; 	
int  addlen=0;

	mail_para.task_ID  = TB_HANDLE_TASK;
	mail_para.primitiv = TEST_GET_VERSION_REQ;
	mail_buffer[0] = mail_para.pID;
	
//send out & recieve answer:	
	//if(ipRfp_rsx_mail_out (1,&mail_para.pID,9))  //read 9bytes back
	if(ipRfp_rsx_mail_out (1,mail_buffer,9))  //read 9bytes back
		return(NULL);

//check for additional message [@GC26052011]
	while( (ip_struct.rx_buffer[3+addlen]!=0x09)||(ip_struct.rx_buffer[4+addlen]!=0xf0) )
	{   // additional message
    	if ( ip_struct.rx_buffer[0+addlen]==0 ) break;
    	addlen+=1+(unsigned char)ip_struct.rx_buffer[0+addlen];   // len from additional message
	}
        

//check confirmation
	if((ip_struct.rx_buffer[3+addlen]!= 0x09) ||(ip_struct.rx_buffer[4+addlen]!= 0xf0))
		return(NULL);  
        
	if( ip_struct.rx_buffer[0+addlen] != 8 )  //check resp length
		return(NULL);  

	memcpy (buffer, &ip_struct.rx_buffer[5+addlen], 3);
	return(buffer);
}




/*
**********************************************************************************
** ReadEeprom
**
*/
char *ipRfp_rsx_ReadEeprom(int address, char *buffer, int count)
{
int 		  sub_index,sub_address, sub_count;
int 		  addlen=0;
unsigned char mail_buffer[6];
//#ifdef BIT_IP_RFP35_DEBUG_RSX
//	FILE *fp;
//#endif


	if(!count)
	{	
		return(NULL);
	}	

	
	memset(buffer, 0x00, count);     // set all to Zero
	
	
	sub_address = address;
	sub_index = 0;
	do
	{
		if(count < mail_para.eeprom_read_blocksize )                                //max read-buffer length
			sub_count = count;
		else
			sub_count = mail_para.eeprom_read_blocksize; 
			
		mail_para.task_ID = TB_HANDLE_TASK;
		mail_para.primitiv = 0xF041;        // new, 0xF040
		mail_buffer[0] = mail_para.pID;
		mail_buffer[1] = sub_address & 0xff;	//low-byte
		mail_buffer[2] = sub_address >> 8;		//high-byte
		mail_buffer[3] = sub_count;
		
//send out & recieve answer:	
		if(ipRfp_rsx_mail_out (4,mail_buffer,(9+sub_count)))  //read 9+count bytes back
		{
			return(NULL);
		}	
		
//@GC 20.02.2012
//check for additional message
		addlen=0;								 // @GC 06.07.2012
		while( (ip_struct.rx_buffer[3+addlen]!=0x41)||(ip_struct.rx_buffer[4+addlen]!=0xf0) )
		{   // additional message
	    	if ( ip_struct.rx_buffer[0+addlen]==0 ) break;
	    	addlen+=1+(unsigned char)ip_struct.rx_buffer[0+addlen];   // len from additional message
		}

//@GC 20.02.2012
//check response message		
		if((ip_struct.rx_buffer[3+addlen]!=0x41)||(ip_struct.rx_buffer[4+addlen]!=0xf0))
		{
			return(NULL);
		}	
			
//@GC 20.02.2012
//check resp length
		if( ip_struct.rx_buffer[0+addlen] != (8+sub_count) )
		{
			return(NULL);  
		}			
			
//copy response bytes
		memcpy (&buffer[sub_index], &ip_struct.rx_buffer[8+addlen], sub_count);
		sub_index += sub_count;
		sub_address += sub_count;	
		count -= sub_count;
	}
	while(count>0);
	
	return(buffer);
}






/*
**********************************************************************************
*/
char *ipRfp_rsx_hexstr2bytestr(char *buffer)
{
//buffer: string with format: 0x..,0x..
int i,j,k;
char temp;	

	i = strlen(buffer);
	k = 0;
	for(j=0;j<i;j++)
		{
		if((buffer[j] == '0')&&(buffer[j+1] == 'x'))
			{
			j++;
			j++;
			buffer[k] = buffer[j] - 0x30;
			if(buffer[k] > 9)buffer[k] -= 0x27;
			if(!(buffer[j+1] == ','))
				{
				buffer[k] *= 16;
				temp = buffer[j+1] - 0x30;
				if(temp > 9) temp -= 0x27;
				buffer[k] += temp;
				}
			k++;
			}
		}
	buffer[k] = 0;
	return(buffer);
}


/*
**********************************************************************************
** WriteEeprom
**
*/
int ipRfp_rsx_WriteEeprom(int address, char *buffer_ori, int count, int convert)
{
int sub_index,sub_address, sub_count,i;
char mail_buffer[MAX_MAIL_LENGTH];
char read_buffer[MAX_MAIL_LENGTH];
int  addlen=0;
char convert_buffer[260];
char *buffer;
int counter;

	counter = 0;
	
	if(convert)			//only useful for function-panel
	{
		strcpy(convert_buffer,buffer_ori);
		ipRfp_rsx_hexstr2bytestr(convert_buffer);
		buffer = convert_buffer;
	}
	else
	{
		buffer = buffer_ori;
	}	
		
		
		
	sub_address = address;
	sub_index = 0;
	do
	{
		if(count < mail_para.eeprom_write_blocksize)  //max write-buffer length
			sub_count = count;
		else
			sub_count = mail_para.eeprom_write_blocksize; 
			
		mail_para.task_ID = TB_HANDLE_TASK;
		mail_para.primitiv = TEST_WRITE_EEPROM_REQ;        // 0xF042
		mail_buffer[0] = mail_para.pID;
		mail_buffer[1] = sub_address & 0xff;	//low-byte
		mail_buffer[2] = sub_address >> 8;		//high-byte
		mail_buffer[3] = sub_count;
		memcpy(&mail_buffer[4],&buffer[sub_index],sub_count);
		

//send out & recieve answer:	
		if(ipRfp_rsx_mail_out ((4+sub_count),mail_buffer,8))  //read 8 bytes back
			return(-1);
			
//@GC 20.02.2012
//check aditional message
		addlen=0;								 // @GC 06.07.2012
		while( (ip_struct.rx_buffer[3+addlen]!=0x43)||(ip_struct.rx_buffer[4+addlen]!=0xf0) )
		{   // additional message
    		if ( ip_struct.rx_buffer[0+addlen]==0 ) break;
    		addlen+=1+(unsigned char)ip_struct.rx_buffer[0+addlen];   // len from additional message
		}
		
//@GC 20.02.2012
//check response message
		if((ip_struct.rx_buffer[3+addlen]!= 0x43) ||(ip_struct.rx_buffer[4+addlen]!= 0xf0))
			return(-2);
	
//check resp length
		if( ip_struct.rx_buffer[0+addlen] != 7 )
			return(-3);
			
//check answer  (eeprom addr)  
		if((ip_struct.rx_buffer[5+addlen]!= (sub_address&0xff)) ||(ip_struct.rx_buffer[6+addlen]!= (sub_address>>8)))
			return(-4);
			
		
#ifdef READ_BACK		
// check written bytes
// read  bytes back
		if(!(ipRfp_rsx_ReadEeprom(sub_address, read_buffer, sub_count)) ) 
			return(-5);
			
		for(i=0 ; i < sub_count ; i++)
		{
			if( buffer[sub_index+i] != read_buffer[i] )
				return(-6);
		}
		
		sub_index += sub_count;
		sub_address += sub_count;	
		count -= sub_count;

		if ( count > 0 )
			Sleep(200); //Delay(0.2);
#endif			
	count = 0;	
	}
	while(count>0);
	
	
	
	return(0);
}




/*
**********************************************************************************
** GetRSSI
** 
*/
int ipRfp_rsx_GetRSSI(unsigned char *value)
{
unsigned char mail_buffer[6]; 	

	mail_para.task_ID = TB_HANDLE_TASK;
	mail_para.primitiv = TEST_BMC_GET_RSSI_REQ;
	mail_buffer[0] = mail_para.pID;

//send out, no answer:	
	if(ipRfp_rsx_mail_out (1,mail_buffer,9))   //read 9bytes back
		return(-1);
//check confirmation
	if((ip_struct.rx_buffer[3]!= 0x0c) ||(ip_struct.rx_buffer[4]!= 0xf1))
		return(-2);
	
//check resp length
	if( ip_struct.rx_buffer[0] != 8 )
		return(-3);  

	
	*value = ip_struct.rx_buffer[5];
	return(0);
}


/*
**********************************************************************************
** SetupDummyBearer
** 
*/
int ipRfp_rsx_SetupDummyBearer(unsigned char dummychannel, unsigned char dummyslot)
{
unsigned char mail_buffer[6]; 
int  addlen=0; 

	mail_para.task_ID = TB_HANDLE_TASK;
	mail_para.primitiv = TEST_SETUP_DUMMY_BEARER;
	mail_buffer[0] = dummychannel;
	mail_buffer[1] = dummyslot;
//send out, no answer:	
	if(ipRfp_rsx_mail_out (2,mail_buffer,6))   //read 6bytes back
		return(-1);
	
//check aditional message   // [@GC07052012]
	while( (ip_struct.rx_buffer[3+addlen]!=0x00)||(ip_struct.rx_buffer[4+addlen]!=0xfe) )
	{   // additional message
	    if ( ip_struct.rx_buffer[0+addlen]==0 ) break;
	    addlen+=1+(unsigned char)ip_struct.rx_buffer[0+addlen];   // len from additional message
	}
	
	
	if((ip_struct.rx_buffer[3+addlen]!= 0x00) ||(ip_struct.rx_buffer[4+addlen]!= 0xfe))
		return(-2);
	
//check resp length
	if( ip_struct.rx_buffer[0+addlen] != 5 )
		return(-3);  

	return(0);
}


/*
**********************************************************************************
** SetDiversity
** 1 byte antenna to activate
**		0x01: antenna 0
**		0x02: antenna 1
** 
*/
int ipRfp_rsx_SetDiversity(unsigned char value)
{
unsigned char mail_buffer[6]; 	
int  addlen=0;

	//if(value > 3) return(-1);
	if(value < 0x01 || value > 0x02) return(-1);
	
	mail_para.task_ID = TB_HANDLE_TASK;
	mail_para.primitiv = TEST_BMC_SET_DIVERSITY;
	mail_buffer[0] = value;
//send out, no answer:	
	if(ipRfp_rsx_mail_out (1,mail_buffer,6))  //read 6bytes back
		return(-1);
		
//check aditional message   // [@GC26052011]
	while( (ip_struct.rx_buffer[3+addlen]!=0x01)||(ip_struct.rx_buffer[4+addlen]!=0xfe) )
	{   // additional message
	    if ( ip_struct.rx_buffer[0+addlen]==0 ) break;
	    addlen+=1+(unsigned char)ip_struct.rx_buffer[0+addlen];   // len from additional message
	}
		
		
	if((ip_struct.rx_buffer[3+addlen]!= 0x01) ||(ip_struct.rx_buffer[4+addlen]!= 0xfe))  //@GC26.05.2011
		return(-2);
	
//check resp length
	if( ip_struct.rx_buffer[0+addlen] != 5 )
		return(-3);  

	
	return(0);
}




/*
**********************************************************************************
** SetRxChannel
** 
*/
//not in RFP35   [@GC26.05.2011]
//int ipRfp_rsx_SetRxChannel(unsigned char channel,unsigned char radio)
//{
//unsigned char mail_buffer[6]; 
//
//	mail_para.task_ID = TB_HANDLE_TASK;
//	mail_para.primitiv = TEST_BMC_SET_RX_CHANNEL;
//	mail_buffer[0] =  channel;
//	mail_buffer[1] =  radio; 
////send out, no answer:	
//	if(ipRfp_rsx_mail_out (2,mail_buffer,6))  
//		return(-1);
////check confirmation
//	if((ip_struct.rx_buffer[3]!= 0x02) ||(ip_struct.rx_buffer[4]!= 0xfe))
//		return(-2);
//	return(0);
//}




/*
**********************************************************************************
** SetDeviation
** 
*/
int ipRfp_rsx_SetDeviation(unsigned char value)
{
unsigned char mail_buffer[6]; 	
int  addlen=0;

int  fail=0;
int  len;
char *ptr;
char cTemp[256];


	mail_para.task_ID = TB_HANDLE_TASK;
	mail_para.primitiv = TEST_BMC_SET_DEVIATION;
	mail_buffer[0] = value;

//send out, no answer:	
	//if(ipRfp_rsx_mail_out (1,&value,6))  
	if(ipRfp_rsx_mail_out (1,mail_buffer,6))   //read 6bytes back  
		return(-1);
		
//check aditional message   // [@GC26052011]
	while( (ip_struct.rx_buffer[3+addlen]!=0x03)||(ip_struct.rx_buffer[4+addlen]!=0xfe) )
	{   // additional message
	
		if((ip_struct.rx_buffer[3+addlen] == 0x0e) && (ip_struct.rx_buffer[4+addlen]== 0xf0))
		{
			//check 0xF00E  free running Text Response.
			//S....Mread deviation: no connection active - using last active loopback connection.......&..
			//*....$set deviation: no connection active..
			len=(unsigned char)ip_struct.rx_buffer[5+addlen];   // len of String
			ptr=&ip_struct.rx_buffer[6+addlen];   // start of String
			memcpy(cTemp,ptr,len); 
			cTemp[len]='\0';
			// if String "no connection active" found then fail
			if ( strstr(cTemp,"no connection active" ) != NULL ) fail++;   
		}	

	    if ( ip_struct.rx_buffer[0+addlen]==0 ) break;
	    addlen+=1+(unsigned char)ip_struct.rx_buffer[0+addlen];   // len from additional message
	}
		
	if ( fail > 0 )
		return(fail);
		
	if((ip_struct.rx_buffer[3+addlen]!= 0x03) ||(ip_struct.rx_buffer[4+addlen]!= 0xfe))
		return(-2);
	
//check resp length
	if( ip_struct.rx_buffer[0+addlen] != 5 )
		return(-3);  
	
	return(fail);	     // ok or "no connection"

}


/*
**********************************************************************************
** SetFrequencyBand
** new only for rfp35
*/
int ipRfp_rsx_SetFrequencyBand(unsigned char value)
{
unsigned char mail_buffer[6]; 	
int addlen=0;
int i;

	// Changes the operating parameters of the RF-Part to the specified frequency band. There
	// are 3 frequency bands defined:
	// FreqBand 1: EU - 10 channels (ch. 9: 1881,792 MHz... ch. 0: 1897,344 MHz)
	// FreqBand 2: 10      channels (ch. 9: 1912,896 MHz... ch. 0: 1928,448 MHz)
	// FreqBand 3: FCC - 5 channels (ch. 4: 1921,536 MHz... ch. 0: 1928,448 MHz)
	// Default in testmode after reset is FreqBand 1.

	mail_para.task_ID = TB_HANDLE_TASK;
	mail_para.primitiv = 0xf132;		  //TEST_BMC_SET_FREQUENCY_BAND_REQ;
	mail_buffer[0] = value;
	
//send out, no answer:	
	//if(ipRfp_rsx_mail_out (1,&value,7))  
	if(ipRfp_rsx_mail_out (1,mail_buffer,7))   //read 7bytes back  
		return(-1);

	//15
	//7d22
	//29
	//f00e
	//5
	//fe01
	//70
	//f00e
	//15
	//7d22
	//29
	//f00e
	//5
	//fe01
	//70
	//f00e
	//6
	//f133


//check aditional message   // [@GC18082011]
	while( (ip_struct.rx_buffer[3+addlen]!=0x33)||(ip_struct.rx_buffer[4+addlen]!=0xf1) )
	{   // additional message
	    if ( ip_struct.rx_buffer[0+addlen]==0 ) break;
	    addlen+=1+(unsigned char)ip_struct.rx_buffer[0+addlen];   // len from additional message
	}

	/*
	while( ((ip_struct.rx_buffer[3+addlen]==0x0e)&&(ip_struct.rx_buffer[4+addlen]==0xf0)) ||
	       ((ip_struct.rx_buffer[3+addlen]==0x22)&&(ip_struct.rx_buffer[4+addlen]==0x7d)) ||
	       ((ip_struct.rx_buffer[3+addlen]==0x01)&&(ip_struct.rx_buffer[4+addlen]==0xfe))  )
	{   // additional message
	    i = 1 + (unsigned char)ip_struct.rx_buffer[0+addlen];   // len from additional message
	    addlen+= i;       
	}
	*/	
		
		
	if((ip_struct.rx_buffer[3+addlen]!= 0x33) ||(ip_struct.rx_buffer[4+addlen]!= 0xf1))
		return(-2);
	
	//check resp length
	if( ip_struct.rx_buffer[0+addlen] != 6 )
		return(-3);  

		
	return(0);
}


/*
**********************************************************************************
** SetTransmitPower
** new only for rfp35
*/
int ipRfp_rsx_SetTransmitPower(unsigned char value)
{
unsigned char mail_buffer[6]; 
int addlen=0;    
	

	// There are 2 transmit power levels the RFP can use:
	// 0 : 20 dBm
	// 1 : 24 dBm (default in testmode after reset)


	mail_para.task_ID = TB_HANDLE_TASK;
	mail_para.primitiv = 0xf130;		  //TEST_BMC_SET_PWER_LEVEL_REQ;
	mail_buffer[0] = value;
	
//send out, no answer:	
	if(ipRfp_rsx_mail_out (1,mail_buffer,7))   //read 7bytes back  
		return(-1);
	
	
//check aditional message   // [@GC03052012]
	while( (ip_struct.rx_buffer[3+addlen]!=0x31)||(ip_struct.rx_buffer[4+addlen]!=0xf1) )
	{   // additional message
	    if ( ip_struct.rx_buffer[0+addlen]==0 ) break;
	    addlen+=1+(unsigned char)ip_struct.rx_buffer[0+addlen];   // len from additional message
	}
	
	
	if((ip_struct.rx_buffer[3+addlen]!= 0x31) ||(ip_struct.rx_buffer[4+addlen]!= 0xf1))
		return(-2);
	
//check resp length
	if( ip_struct.rx_buffer[0+addlen] != 6 )
		return(-3);  

	return(0);
}


/*
**********************************************************************************
** ReadEepromDeviation
** 			
** geändert, der Parameter Radio ist nun ein pointer, zum zurück lesen
** 
*/
int ipRfp_rsx_ReadEepromDeviation(unsigned char *value, unsigned char *radio)
{
unsigned char mail_buffer[6];
int addlen=0;

int  fail=0;
int  len;
char *ptr;
char cTemp[256];

	mail_para.task_ID = TB_HANDLE_TASK;
	mail_para.primitiv = TEST_BMC_READ_EEPROM_DEVIATION_REQ;
	mail_buffer[0] = mail_para.pID;
	mail_buffer[1] = 0;                   
//send out, no answer:	
	//if(ipRfp_rsx_mail_out (1,&mail_para.pID,8))  
	if(ipRfp_rsx_mail_out (2,mail_buffer,8))   //read 8bytes back  
		return(-1);
		
//check aditional message   // [@GC26052011]
	while( (ip_struct.rx_buffer[3+addlen]!=0x1d)||(ip_struct.rx_buffer[4+addlen]!=0xf1) )
	{   // additional message
		if((ip_struct.rx_buffer[3+addlen] == 0x0e) && (ip_struct.rx_buffer[4+addlen]== 0xf0))
		{
			//check 0xF00E  free running Text Response.
			//S....Mread deviation: no connection active - using last active loopback connection.......&..
			//*....$set deviation: no connection active..
			len=(unsigned char)ip_struct.rx_buffer[5+addlen];   // len of String
			ptr=&ip_struct.rx_buffer[6+addlen];   // start of String
			memcpy(cTemp,ptr,len); 
			cTemp[len]='\0';
			// if String "no connection active" found then fail
			if ( strstr(cTemp,"no connection active" ) != NULL ) fail++;   
		}	
		
	    if ( ip_struct.rx_buffer[0+addlen]==0 ) break;
	    addlen+=1+(unsigned char)ip_struct.rx_buffer[0+addlen];   // len from additional message
	}
		
//check confirmation primitive
	if((ip_struct.rx_buffer[3+addlen]!= 0x1d) ||(ip_struct.rx_buffer[4+addlen]!= 0xf1))
		return(-2);

//check resp length
	if( ip_struct.rx_buffer[0+addlen] != 7 )
		return(-3);  

	
// assigne locked radio
// check it outside	
	*radio=ip_struct.rx_buffer[6+addlen];
	
//assigne eeprom deviation value		
	*value = ip_struct.rx_buffer[5+addlen];
	
	return(fail);	     // ok or "no connection"

}

/*
**********************************************************************************
** WriteEepromDeviation
** 
*/
int ipRfp_rsx_WriteEepromDeviation(unsigned char value, int radio)
{
unsigned char mail_buffer[6]; 
int checkvalue;
int addlen=0;

int  fail=0;
int  len;
char *ptr;
char cTemp[256];

	checkvalue = (int)value;
	if(checkvalue > 63) return(-1);
	
	mail_para.task_ID = TB_HANDLE_TASK;
	mail_para.primitiv = TEST_BMC_WRITE_EEPROM_DEVIATION_REQ;
	//mail_buffer[0] = 0;
	mail_buffer[0] = mail_para.pID;	//@JM19.04.11
	mail_buffer[1] = 0;                   
	mail_buffer[2] = value;
//send out, :	
	//if(ipRfp_rsx_mail_out (3,mail_buffer,7))  
	if(ipRfp_rsx_mail_out (3,mail_buffer,7))   //read 7bytes back  
		return(-2);
		
//check aditional message   // [@GC26052011]
	//len=0x07 resp=0xf043
	//len=0x06 resp=0xf11f
	
	while( (ip_struct.rx_buffer[3+addlen]!=0x1f)||(ip_struct.rx_buffer[4+addlen]!=0xf1) )
	{   // additional message
		if((ip_struct.rx_buffer[3+addlen] == 0x0e) && (ip_struct.rx_buffer[4+addlen]== 0xf0))
		{
			//check 0xF00E  free running Text Response.
			//S....Mread deviation: no connection active - using last active loopback connection.......&..
			//*....$set deviation: no connection active..
			len=(unsigned char)ip_struct.rx_buffer[5+addlen];    // len of String
			ptr=&ip_struct.rx_buffer[6+addlen];   // start of String
			memcpy(cTemp,ptr,len); 
			cTemp[len]='\0';
			// if String "no connection active" found then fail
			if ( strstr(cTemp,"no connection active" ) != NULL ) fail++;   
		}	
	
	    if ( ip_struct.rx_buffer[0+addlen]==0 ) break;
	    addlen+=1+(unsigned char)ip_struct.rx_buffer[0+addlen];   // len from additional message
	}
		
		
//check confirmation
	if((ip_struct.rx_buffer[3+addlen]!= 0x1f) ||(ip_struct.rx_buffer[4+addlen]!= 0xf1))
		return(-2);
	
//check resp length
	if( ip_struct.rx_buffer[0+addlen] != 6 )
		return(-3);  

//check radio
	if(ip_struct.rx_buffer[5+addlen]!= radio)
		return(-4);
	
	return(fail);	     // ok or "no connection"
}


/*
**********************************************************************************
** SetB_fieldContent
** 
*/
int ipRfp_rsx_SetB_fieldContent(unsigned char value1, unsigned char value2)
{
unsigned char mail_buffer[6];
int addlen=0;

	mail_para.task_ID = TB_HANDLE_TASK;
	mail_para.primitiv = TEST_BMC_SET_B_FIELD;
	mail_buffer[0] = value1;
	mail_buffer[1] = value2;
	
//send out, no answer:	
	if(ipRfp_rsx_mail_out (2,mail_buffer,6))   //read 6bytes back  
		return(-1);
	
	
	while((ip_struct.rx_buffer[3+addlen] == 0x0e) && (ip_struct.rx_buffer[4+addlen]== 0xf0))
	{   // additional message
	    addlen=addlen + 1 + (unsigned char)ip_struct.rx_buffer[0+addlen];   // len from additional message
	}
	
	
//check confirmation
	if((ip_struct.rx_buffer[3+addlen]!= 0x04) ||(ip_struct.rx_buffer[4+addlen]!= 0xfe))
		return(-2);
	
//check resp length
	if( ip_struct.rx_buffer[0+addlen] != 5 )
		return(-3);  

	
	return(0);
}

/*
**********************************************************************************
** SetClock
** 
*/
int ipRfp_rsx_SetClock(unsigned char value)
{
unsigned char mail_buffer[6];
int addlen=0;

	mail_para.task_ID = TB_HANDLE_TASK;
	mail_para.primitiv = TEST_SET_CLOCK;
	mail_buffer[0] = value;
	
//send out, no answer:	
	if(ipRfp_rsx_mail_out (1,mail_buffer,6))   //read 6bytes back  
		return(-1);
	
//check aditional message   // [@GC13042012]
	while((ip_struct.rx_buffer[3+addlen] == 0x0e) && (ip_struct.rx_buffer[4+addlen]== 0xf0))
	{   // additional message
	    addlen=addlen + 1 + (unsigned char)ip_struct.rx_buffer[0+addlen];   // len from additional message
	}
		
		
//check confirmation
	if((ip_struct.rx_buffer[3+addlen]!= 0x05) ||(ip_struct.rx_buffer[4+addlen]!= 0xfe))
		return(-2);

//check resp length
	if( ip_struct.rx_buffer[0+addlen] != 5 )
		return(-3);  
	
	
	return(0);
}





/*
**********************************************************************************
** ReadEepromClockFrequency
** 
*/
int ipRfp_rsx_ReadEepromClockFrequency(unsigned char *value)
{
unsigned char mail_buffer[6];
int addlen=0;

	
	mail_para.task_ID = TB_HANDLE_TASK;
	mail_para.primitiv = 0xf15d;			//TEST_BMC_READ_EEPROM_CLOCK_REQ;
	mail_buffer[0] = mail_para.pID;
	
//send out, no answer:	
	if(ipRfp_rsx_mail_out (1,mail_buffer,7))   //read 7bytes back    
		return(-1);
		
//check aditional message   // [@GC13042012]
	while((ip_struct.rx_buffer[3+addlen] == 0x0e) && (ip_struct.rx_buffer[4+addlen]== 0xf0))
	{   // additional message
	    addlen=addlen + 1 + (unsigned char)ip_struct.rx_buffer[0+addlen];   // len from additional message
	}
		
//check confirmation
	if((ip_struct.rx_buffer[3+addlen]!= 0x5e) ||(ip_struct.rx_buffer[4+addlen]!= 0xf1))
		return(-2);
	
//check resp length
	if( ip_struct.rx_buffer[0+addlen] != 6 )
		return(-3);  

	
	*value = ip_struct.rx_buffer[5+addlen];
	return(0);
}

/*
**********************************************************************************
** WriteEepromClockFrequency
** 
*/
int ipRfp_rsx_WriteEepromClockFrequency(unsigned char value)
{
unsigned char mail_buffer[6];
int addlen=0;

	mail_para.task_ID = TB_HANDLE_TASK;
	mail_para.primitiv = 0xf15f;			//TEST_BMC_WRITE_EEPROM_CLOCK_REQ;
	mail_buffer[0] = mail_para.pID;
	mail_buffer[1] = value;
	
//send out, :	
	if(ipRfp_rsx_mail_out (2,mail_buffer,6))   //read 6bytes back      
		return(-1);
		
//check aditional message   // [@GC13042012]
	while((ip_struct.rx_buffer[3+addlen] == 0x0e) && (ip_struct.rx_buffer[4+addlen]== 0xf0))
	{   // additional message
	    addlen=addlen + 1 + (unsigned char)ip_struct.rx_buffer[0+addlen];   // len from additional message
	}
		
//check confirmation
	if((ip_struct.rx_buffer[3+addlen]!= 0x60) ||(ip_struct.rx_buffer[4+addlen]!= 0xf1))
		return(-2);
	
//check resp length
	if( ip_struct.rx_buffer[0+addlen] != 6 )
		return(-3);  
	
	
	return(0);
}


/*
**********************************************************************************
** SetBandgap
** 
*/
int ipRfp_rsx_SetBandgap(unsigned char value)
{
unsigned char mail_buffer[6];
int addlen=0;
	
	if(value > 15) return(-1);
	mail_para.task_ID = TB_HANDLE_TASK;
	mail_para.primitiv = 0xf14b;
	mail_buffer[0] = value;
	
//send out, no answer:	
	if(ipRfp_rsx_mail_out (1,mail_buffer,6))   //read 6bytes back      
		return(-1);

//check aditional message   // [@GC13042012]
	while((ip_struct.rx_buffer[3+addlen] == 0x0e) && (ip_struct.rx_buffer[4+addlen]== 0xf0))
	{   // additional message
	    addlen=addlen + 1 + (unsigned char)ip_struct.rx_buffer[0+addlen];   // len from additional message
	}
		
//check confirmation
	if((ip_struct.rx_buffer[3+addlen]!= 0x06) ||(ip_struct.rx_buffer[4+addlen]!= 0xfe))
		return(-2);
	
//check resp length
	if( ip_struct.rx_buffer[0+addlen] != 5 )
		return(-3);  
	
	return(0);
}



/*
**********************************************************************************
** ReadEepromBandgap
** 
*/
int ipRfp_rsx_ReadEepromBandgap(unsigned char *value)
{
unsigned char mail_buffer[6];
int addlen=0;

	mail_para.task_ID = TB_HANDLE_TASK;
	mail_para.primitiv = 0xf167;                //TEST_BMC_READ_EEPROM_VCC1_REQ;
	mail_buffer[0] = mail_para.pID;

//send out, no answer:	
	if(ipRfp_rsx_mail_out (1,mail_buffer,7))   //read 7bytes back        
		return(-1);

//check aditional message   // [@GC13042012]
	while((ip_struct.rx_buffer[3+addlen] == 0x0e) && (ip_struct.rx_buffer[4+addlen]== 0xf0))
	{   // additional message
	    addlen=addlen + 1 + (unsigned char)ip_struct.rx_buffer[0+addlen];   // len from additional message
	}
		
//check confirmation
	if((ip_struct.rx_buffer[3+addlen]!= 0x68) ||(ip_struct.rx_buffer[4+addlen]!= 0xf1))
		return(-2);

//check resp length
	if( ip_struct.rx_buffer[0+addlen] != 6 )
		return(-3);  
	
	*value = ip_struct.rx_buffer[5+addlen];
	return(0);
}

/*
**********************************************************************************
** WriteEepromBandgap
** 
*/
int ipRfp_rsx_WriteEepromBandgap(unsigned char value)
{
unsigned char mail_buffer[6];
int addlen=0;

	mail_para.task_ID = TB_HANDLE_TASK;
	mail_para.primitiv = 0xf169;                //TEST_BMC_WRITE_EEPROM_VCC1_REQ;
	mail_buffer[0] = mail_para.pID;
	mail_buffer[1] = value;
//send out, :	
	if(ipRfp_rsx_mail_out (2,mail_buffer,6))   //read 6bytes back        
		return(-1);
		
//check aditional message   // [@GC13042012]
	while((ip_struct.rx_buffer[3+addlen] == 0x0e) && (ip_struct.rx_buffer[4+addlen]== 0xf0))
	{   // additional message
	    addlen=addlen + 1 + (unsigned char)ip_struct.rx_buffer[0+addlen];   // len from additional message
	}
		
//check confirmation
	if((ip_struct.rx_buffer[3+addlen]!= 0x6a) ||(ip_struct.rx_buffer[4+addlen]!= 0xf1))
		return(-2);

//check resp length
	if( ip_struct.rx_buffer[0+addlen] != 5 )
		return(-3);  
	
	
	return(0);
}




/*
**********************************************************************************
** FinalizeEeprom
** 
*/
int ipRfp_rsx_FinalizeEeprom(void)
{
unsigned char mail_buffer[6];
int addlen=0;


	mail_para.task_ID = TB_HANDLE_TASK;
	mail_para.primitiv = 0xF044;                    //TEST_FINALIZE_EEPROM_REQ;
	mail_buffer[0] = mail_para.pID;

	
//send out & recieve answer:	
	//if(ipRfp_rsx_mail_out (1,&mail_para.pID,6))  //read 6bytes back
	if(ipRfp_rsx_mail_out (1,mail_buffer,6))  //read 6bytes back
		return(-1);


//check aditional message   // [@GC26052011]
	
	// das können mehrere sein
	while((ip_struct.rx_buffer[3+addlen] == 0x0e) && (ip_struct.rx_buffer[4+addlen]== 0xf0))
	{   // additional message
	    addlen=addlen + 1 + (unsigned char)ip_struct.rx_buffer[0+addlen];   // len from additional message
	}
	
//check confirmation
	if((ip_struct.rx_buffer[3+addlen]!= 0x45) || (ip_struct.rx_buffer[4+addlen]!= 0xf0))
		return(-2);

//check resp length
	if( ip_struct.rx_buffer[0+addlen] != 5 )
		return(-3);  
	
	return(0);
}


/*
**********************************************************************************
** showEepromValues
** 
*/
int ipRfp_rsx_showEepromValues(unsigned char *eeprom_value_adjust,unsigned char *eeprom_value_reboot)
{

char cInfo[4096];

/*
EEPROM_BANDGAP                   (0x0025)
EEPROM_RFPI                      (0x0040)

EEPROM_MODULATION_DEVIATION0     (0x0045)
EEPROM_MODULATION_DEVIATION1     (0x0046)
EEPROM_RSSI_COMP_OFFSET0         (0x0047)
EEPROM_RSSI_COMP_OFFSET1         (0x0048)
EEPROM_RSSI_COMP_K_FACTOR0       (0x0049)
EEPROM_RSSI_COMP_K_FACTOR1       (0x004A)
EEPROM_DEFAULT_FREQUENCY         (0x004F)
*/

    sprintf(cInfo,"Values       adjust/reboot\nValue1V8 0x%02x/0x%02x\nValueClock 0x%02x/0x%02x\nRSSI_COMP_K_FACTOR1 0x%02x/0x%02x\nRSSI_COMP_K_FACTOR0 0x%02x/0x%02x\nRSSI_COMP_OFFSET1 0x%02x/0x%02x\nRSSI_COMP_OFFSET0 0x%02x/0x%02x\nMODULATION_DEVIATION1 0x%02x/0x%02x\nMODULATION_DEVIATION0 0x%02x/0x%02x",
				eeprom_value_adjust[EEPROM_BANDGAP],eeprom_value_reboot[EEPROM_BANDGAP],
				eeprom_value_adjust[EEPROM_DEFAULT_FREQUENCY],eeprom_value_reboot[EEPROM_DEFAULT_FREQUENCY],
				eeprom_value_adjust[EEPROM_RSSI_COMP_K_FACTOR1],eeprom_value_reboot[EEPROM_RSSI_COMP_K_FACTOR1],
				eeprom_value_adjust[EEPROM_RSSI_COMP_K_FACTOR0],eeprom_value_reboot[EEPROM_RSSI_COMP_K_FACTOR0],
				eeprom_value_adjust[EEPROM_RSSI_COMP_OFFSET1],eeprom_value_reboot[EEPROM_RSSI_COMP_OFFSET1],
				eeprom_value_adjust[EEPROM_RSSI_COMP_OFFSET0],eeprom_value_reboot[EEPROM_RSSI_COMP_OFFSET0],
				eeprom_value_adjust[EEPROM_MODULATION_DEVIATION1],eeprom_value_reboot[EEPROM_MODULATION_DEVIATION1],
				eeprom_value_adjust[EEPROM_MODULATION_DEVIATION0],eeprom_value_reboot[EEPROM_MODULATION_DEVIATION0]);
	
	//MessagePopup("ipRfp_showEepromValues",cInfo);
    
    strcat(cInfo,"\n\nPassed = Yes    Fail = No");
    if( !ConfirmPopup( "ipRfp_showEepromValues" , cInfo) )
    {
        return(-1);       // fail
    }
        


	
return(0);	
}	

/*
**********************************************************************************
** getEepromValues
** 
*/
int ipRfp_rsx_getEepromValues(unsigned char *eeprom_value)
{

unsigned char value;

/*
EEPROM_BANDGAP                   (0x0025)
EEPROM_RFPI                      (0x0040)

EEPROM_MODULATION_DEVIATION0     (0x0045)
EEPROM_MODULATION_DEVIATION1     (0x0046)
EEPROM_RSSI_COMP_OFFSET0         (0x0047)
EEPROM_RSSI_COMP_OFFSET1         (0x0048)
EEPROM_RSSI_COMP_K_FACTOR0       (0x0049)
EEPROM_RSSI_COMP_K_FACTOR1       (0x004A)
EEPROM_DEFAULT_FREQUENCY         (0x004F)
*/

	// read all EEPROM Values
	if(!ipRfp_rsx_ReadEeprom(0x20, &eeprom_value[0x20], 0x30))return(-1);		//read offset 0x20 to 0x4f

    
/*    
    //1v8
	if(ipRfp_ReadEepromBandgap (&value)) return(-1); 	                    // 0x25
    eeprom_value[EEPROM_BANDGAP]=value;
    
    //freq
  	if(ipRfp_ReadEepromClockFrequency (&value)) return(-2); 				// 0x4F
    eeprom_value[EEPROM_DEFAULT_FREQUENCY]=value;
	
	//if(!ipRfp_ReadEeprom(EEPROM_RFPI, ReadRfpi, 5))return(-3);				// 0x40
	

		
	// eeprom rssi Values	
	if(!ipRfp_ReadEeprom(EEPROM_RSSI_COMP_K_FACTOR1, &value, 1))return(-4);		//RF-Part1 0x4A
    eeprom_value[EEPROM_RSSI_COMP_K_FACTOR1]=value;
	if(!ipRfp_ReadEeprom(EEPROM_RSSI_COMP_K_FACTOR0, &value, 1))return(-5);        //RF-Part2 0x49
    eeprom_value[EEPROM_RSSI_COMP_K_FACTOR0]=value;

	if(!ipRfp_ReadEeprom(EEPROM_RSSI_COMP_OFFSET1, &value, 1))return(-6);	        //RF-Part1 0x48
    eeprom_value[EEPROM_RSSI_COMP_OFFSET1]=value;
	if(!ipRfp_ReadEeprom(EEPROM_RSSI_COMP_OFFSET0, &value, 1))return(-7);			//RF-Part2 0x47
    eeprom_value[EEPROM_RSSI_COMP_OFFSET0]=value;

	//eeprom deviation values
	if(!ipRfp_ReadEeprom(EEPROM_MODULATION_DEVIATION1 , &value, 1))return(-8);	        //RF-Part_ 0x46
    eeprom_value[EEPROM_MODULATION_DEVIATION1]=value;
	if(!ipRfp_ReadEeprom(EEPROM_MODULATION_DEVIATION0 , &value, 1))return(-9);	        //RF-Part_ 0x45
    eeprom_value[EEPROM_MODULATION_DEVIATION0]=value;
    
*/
	
return(0);	
}	


/*********************************************************************************/
/*********************************************************************************/
/*********************************************************************************/



/*
**********************************************************************************
** OpenConnection 
**
*/
int ipRfp_rsx_Open (char *ipAddr, int port)
{
	struct sockaddr_in addr;               // Socket Address Struktur fuer den connect-Aufruf

    mail_para.eeprom_write_blocksize = 0x20;	//default: RFP
    mail_para.eeprom_read_blocksize  = 0x20;	//default: RFP
	mail_para.pID = 240;						//default: program ID
	//mail_para.pID = 0x82;

#ifdef BIT_IP_RFP35_DEBUG_RSX
{
	FILE *fp;
	fp=fopen(BIT_IP_RFP35_DEBUG_RSX, "w");
	fclose(fp);
}	
#endif
   
	// IP-Socket mit TCP-Protokoll erstellen
	if ((sock = socket(AF_INET, SOCK_STREAM, 0) ) == INVALID_SOCKET)
	{
    	return(1);
   	}


	addr.sin_family           = AF_INET;               // Adressfamilie IP
	addr.sin_port             = htons (port);         // Port in Network-Byte-Order umwandeln
	addr.sin_addr.S_un.S_addr = inet_addr (ipAddr);    // IP-Adresse umwandeln

	// Verbindung zum IP-RFP herstellen
	if ( connect(sock, (struct sockaddr*) &addr, sizeof (struct sockaddr_in)) == SOCKET_ERROR)
	{
    	//fprintf (stderr, "Error: Unable to connect to %s", ipAddr);
	   	closesocket(sock);
		sock = INVALID_SOCKET;
      	return(2);
   	}
   
    bIsConnected = CONNECTED;

   	return(0);   //ok
}

/*******************************************************************************************************
 * Function:   CloseConnection
 *             Ein bestehender Socket wird abgebaut. Alle genutzten Resourcen werden
 *             freigegeben
 *
 * Input:
 *    None
 *
 * Output:
 *    Die globale Variable sock wird auf den Wert INVALID_SOCKET gesetzt, und die globale
 *    Variable bIsConnected wird auf den Wert NOT_CONNECTED gesetzt
 *
 * Result:
 *    None
 */

int ipRfp_rsx_Close (void)
{
	int rc;	
	
	rc = shutdown(sock,2);              // 0=Receive 1=send 2=both
	
   	rc = rc | closesocket(sock);

   	sock = INVALID_SOCKET;

   	bIsConnected = NOT_CONNECTED;
   
	return(rc);	

}
	






/*******************************************************************************************************
 * Function:   SendDataToRfp
 *             Es wird ein Datenblock an den IP-RFP gesendet
 *
 * Input:
 *    iLength: Laenge des zu sendenden Datenblocks
 *    pBuffer: Zeiger auf den zu sendenden Datenblock
 *
 * Output:
 *    None
 *
 * Result:
 *    iLength, wenn der Datenblock gesendet werden konnte
 *    0, wenn der Datenblock nicht gesendet werden konnte, oder der Parameter iLength <= 0 ist
 */
int ipRfp_rsx_SendDataToRfp (int iLength, unsigned char *pBuffer)
{
   if (iLength <= 0)
   {
      //fprintf (stderr, "SendDataToRfp called with Parameter iLength <= 0\n");
      return 0;
   }

   if (bIsConnected == CONNECTED)
   {
      if (send (sock, pBuffer, iLength, NO_FLAGS_SET) == SOCKET_ERROR) 
      {
         bIsConnected = NOT_CONNECTED;
         //fprintf (stderr, "%s: send \n", strerror (errno));
         return 0;
      }
	  return iLength;
   }
   
   return 0;
   
}






// @GC 17.08.2011
/*******************************************************************************************************
 * Function:   ReceiveDataFromIpRfp
 *             Es wird ein Datenblock vom IP-RFP gelesen
 *
 * Input:
 *    length:  Maximale Laenge die abgelegt werden kann
 *    pBuffer: Zeiger auf einen Speicherbereich des Aufrufers, der ausreichend gross ist
 *             um den kompletten Datenblock aufzunehmen
 *
 * Output:
 *    Nach einem erfolgreichen Aufruf enthaelt der Puffer, auf den pBuffer zeigt, die
 *    gelesenen Daten
 *
 * Result:
 *    Anzahl der gelesenen Bytes, wenn der Aufruf erfolgreich war
 *    0, wenn der Datenblock nicht gelesen werden konnte
 *
 * New @GC 17.08.2011
 * Der socket wird jetzt immer komplett gelesen, 
 * auf das erste Byte wird 3 Sec gewartet,
 * auf alle nachfolgenden Bytes wird nur noch 400ms gewartet,
 * wenn 400ms keine Daten mehr kommen ist der Socket leer
 *
 */
int ipRfp_rsx_ReceiveDataFromIpRfp (int iMaxLength, unsigned char *pBuffer)
{
	int  i;
	int  len = 0;
	int  retCode;
   
	int  maxCount = 100;       // maximal 100 mal lesen, timeout wenn zu viele zeichen/zeilen kommen
		 					   // dies ist nur ein sicherheits Counter
	
	struct timeval FAR timeout;  // timeout per select wenn keine Zeichen vorhanden
	
	fd_set fdRead;               // struct fd_set   



#if defined(BIT_IP_RFP35_DEBUG_RSX) && defined(BIT_IP_RFP35_DEBUG_RSX_TIME) 
	unsigned long t;
	FILE *fp;
#endif
	
	
	// In Echt
	// müssen wir das erste mal ca 300 ms Warten bis der erste anteil im Buffer ist
	// alle weiteren Teile sind dann nach je 200ms im Buffer
	//
	
    // 3 Sec warten, denn es muss auf jeden fall mindestens 1 Byte kommen.	
	// die 3 Sec werden im fehlerfall gewartet wenn keine Response kommt
	// aber auch nur wenn wirklich keine Daten kommen.
	timeout.tv_sec = 3;        // 3 sec warten
   	timeout.tv_usec = 0;
   
  
	
	memset(pBuffer,0,iMaxLength);     // den ganzen Buffer loeschen
	
	

	if (bIsConnected == CONNECTED)
   	{
		
#if defined(BIT_IP_RFP35_DEBUG_RSX) && defined(BIT_IP_RFP35_DEBUG_RSX_TIME) 
		fp=fopen(BIT_IP_RFP35_DEBUG_RSX, "a");
#endif
		
      	// Read-Set loeschen
		FD_ZERO(&fdRead);

      	// Socket sock zum Read-Set hinzufuegen
      	FD_SET(sock, &fdRead);

      	// Mit dem select-Aufruf wird ermittelt, ob der Socket empfangsbereit ist
		do 
		{  

#if defined(BIT_IP_RFP35_DEBUG_RSX) && defined(BIT_IP_RFP35_DEBUG_RSX_TIME) 
			t=GetTickCount();
#endif			

	        // Mit dem select-Aufruf wird ermittelt, ob der Socket empfangsbereit ist
			// -1 = error  0=keine daten >0 = daten
          	if ( (retCode = select(sock+1, &fdRead, NULL, NULL, &timeout)) == SOCKET_ERROR )
      	  	{
       			bIsConnected = NOT_CONNECTED;
             	//fprintf (stderr, "%s: select \n", strerror (errno));
				len=0;
	        	break;
    	  	}
			
#if defined(BIT_IP_RFP35_DEBUG_RSX) && defined(BIT_IP_RFP35_DEBUG_RSX_TIME) 
			fprintf(fp,"select(%d)=%dms ",retCode,GetTickCount()-t); 
			fflush(fp);
#endif			

        	// wenn Socket sock im Read-Set enthalten ist, kann gelesen werden
   			if ( retCode > 0 && FD_ISSET(sock, &fdRead) ) 
		    {
#if defined(BIT_IP_RFP35_DEBUG_RSX) && defined(BIT_IP_RFP35_DEBUG_RSX_TIME) 
				t=GetTickCount();
#endif			
				// read Byteanzahl im buffer
         		if ((i = recv (sock, (pBuffer+len) , iMaxLength-len , NO_FLAGS_SET)) <= 0) 
         		{
            		bIsConnected = NOT_CONNECTED;
            		//fprintf (stderr, "%s: recv \n", strerror (errno));
					len=0;
            		break;
				}
				len = len + i;

#if defined(BIT_IP_RFP35_DEBUG_RSX) && defined(BIT_IP_RFP35_DEBUG_RSX_TIME) 
				fprintf(fp,"recv(%d)=%dms ",i,GetTickCount()-t); 
				fflush(fp);
#endif			
					
			}
			maxCount--;
			
	        // wenn bereits Zeichen gekommen sind, dann timeout runter setzen
			timeout.tv_sec = 0;           //0   sec
   			timeout.tv_usec = 400*1000;   //400 millisec
			
		} 
		while ( retCode > 0 && maxCount > 0 && len < iMaxLength );
		
#if defined(BIT_IP_RFP35_DEBUG_RSX) && defined(BIT_IP_RFP35_DEBUG_RSX_TIME) 
		fprintf(fp,"len=%d\n",len); 
		fclose(fp);
#endif			

		
		
	}  // if == CONNECTED
	

	return len;
	
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// WinSocket       ////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
**********************************************************************************
** start WinSocket
**
** WinSocket
** wird fuer rsx Kommandos und libssh Kommandos benoetigt
**
** The WSAStartup function initiates use of the Winsock DLL by a process.
**
**  An application can call WSAStartup more than once if it needs to obtain the WSADATA structure information
**  more than once. On each such call, the application can specify any version number supported by the Winsock DLL.
**
**	An application must call the WSACleanup function for every successful time the WSAStartup function is called.
**  This means, for example, that if an application calls WSAStartup three times, it must call WSACleanup three times.
**  The first two calls to WSACleanup do nothing except decrement an internal counter; the final WSACleanup call
**  for the task does all necessary resource deallocation for the task.
**
*/
int ipRfp_startWinsock(void)
{
	WSADATA            wsaData;            // wird durch den WSAStartup-Aufruf mit Informationen gefuellt
	WORD               wVersionRequested;
	int                rc;
   
	
	wVersionRequested = MAKEWORD (2, 2);

	// Winsock-DLL laden
	if ((rc = WSAStartup (wVersionRequested, &wsaData)) != 0)
	{
	   	(void)WSACleanup();
		return(1);
	}

	// Sicherstellen, dass die geladene WinSock-DLL die Version 2.2 unterstuetzt
	if (LOBYTE (wsaData.wVersion) != 2 || HIBYTE (wsaData.wVersion) != 2 )
   	{
	   	(void)WSACleanup();
    	return(2); 
	}

	return(0);
}	


/*
**********************************************************************************
** cleanup WinSocket
**
**   The WSACleanup function terminates use of the Winsock 2 DLL (Ws2_32.dll).
**   There must be a call to WSACleanup for each successful call to WSAStartup.
**   Only the final WSACleanup function call performs the actual cleanup.
**   The preceding calls simply decrement an internal reference count in the WS2_32.DLL.
**
*/
int ipRfp_cleanupWinsock(void)
{
	int  i;

	for ( i=20 ; i > 0 ; i-- )
	{	
    	// DLL-Resourcen freigeben
   		if ( WSACleanup() != 0 )         // bei -1 keine Resource mehr belegt.
		{
			return(0);	 // all close
		}
	}
		
	
	return(1);	 // timeout
}	
		






//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// SSH  per libssh ////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*==============================================================================
**
** int ipRfp_libssh_WriteAndRead(char *wBuffer, char *rBuffer, int maxread, char *wildString, double timeoutSec )
**
**  return:
**	=  0  == ok
**  <> 0  == fail
**
*/
int ipRfp_libssh_WriteAndRead(char *wBuffer, char *rBuffer, int maxread, char *wildString, double timeoutSec )
{
	int  rc;
  	int  nwrite;
  	int  nread;
	char cErr[512];	

	nwrite=strlen(wBuffer);
	nread=0;
	
    if (nwrite > 0)
	{
		rc=writeSSH(wBuffer, nwrite, cErr );
		
#ifdef BIT_IP_RFP35_DEBUG_SSH
		{
		FILE *fp;
		fp=fopen(BIT_IP_RFP35_DEBUG_SSH, "a");
		fwrite(wBuffer,1,nwrite,fp);
		if ( rc != 0 )
			fprintf(fp,"%s\n",cErr);
		
		fclose(fp);
		}
#endif				
		if ( rc != 0 )
		{
#ifdef _CONSOLE	
			fprintf(stderr,"%s\n",cErr);
#endif
			return(1);
		}			
		
    }

	
	

    if (maxread > 0 && timeoutSec > 0 )
	{
		
	 	rc=readSSH(rBuffer, maxread, &nread, wildString, timeoutSec , cErr );

#ifdef BIT_IP_RFP35_DEBUG_SSH
		{
		FILE *fp;
		fp=fopen(BIT_IP_RFP35_DEBUG_SSH, "a");
		if ( nread > 0 )
			fwrite(rBuffer,1,nread,fp);
		if ( rc != 0 )
			fprintf(fp,"%s\n",cErr);
		fclose(fp);
		}
#endif				
		
		if ( rc != 0 )
		{
#ifdef _CONSOLE	
			fprintf(stderr,"%s\n",cErr);
#endif
			return(1);
		}			

		
	}
	return(0);		// ok
}	
	

/*==============================================================================
**
** int ipRfp_libssh_Close(void)
**
**  return:
**	=  0  == ok
**  <> 0  == fail
**
*/
int ipRfp_libssh_Close(void)
{
	
#ifdef BIT_IP_RFP35_DEBUG_SSH
	{
	FILE *fp;
	fp=fopen(BIT_IP_RFP35_DEBUG_SSH, "a");
	fprintf(fp,"closeSSH()\n");
	fclose(fp);
	}
#endif				
	
	strcpy(ipAddrForReopen,"");			  // clear ip for Reopen function
	
	return( closeSSH() );
}	


/*==============================================================================
**
** int ipRfp_libssh_Reopen(void)
**
**  return:
**	=  0  == ok
**  <> 0  == fail
**
*/
int ipRfp_libssh_Reopen(void)
{
	int rc;
	int retry;
	char ip[80];

	
	// Das SSH Reopen wird benötigt, da der RFP den SSH Service schließt und dann neu startet.
	//
	// Bei allen RFP die bereits die richtige MAC Adr haben (nicht die default mac) laufen nicht mehr im Test mode,
	// dort wird die SSH disabled.
	//
	// Um die SSH einzuschalten gibt es das "manu ssh enable" Kommando, leider hat dieses Kommando den
	// Nebeneffekt das der alte SSH Prozess gelöscht wird und ein neuer gestartet wird.
	// Dieses löschen und neu starten kommt etwa 10 bis 30 Sek nach dem absetzten des Kommandos.
	// Dies Stört dann alle nachfolgenden Testsequenzen z.b. Check_LED, Check_Heater, LinkStatus,BMC Status usw.
	//
	// In all diese Funktionen habe ich im Fehlerfall das Reopen der SSH eingebaut.
	// @ GC 15.05.2014
	


	
	strcpy(ip,ipAddrForReopen);		// get saved ip
	
	(void)ipRfp_libssh_Close();     // close ssh
	
	
	// 10x Retry 1 Sekunden (max 10Sekunden)
	retry=10;
	do
	{	
		Sleep(1000); // 1 Sec
		rc=ipRfp_libssh_Open(ip);
		retry--;
		
	}
	while ( rc != 0 && retry > 0 );

	
	return( rc );
}	


/*==============================================================================
**
** int ipRfp_libssh_Open(char *ipaddr)
**
**  return:
**	=  0  == ok
**  <> 0  == fail
**
*/
int ipRfp_libssh_Open(char *ipaddr)
{
	int  rc;
  	int  nread;
	char cErr[512];	
	
	char cmd[256];

	
#ifdef BIT_IP_RFP35_DEBUG_SSH
	{
	FILE *fp;
	//fp=fopen(BIT_IP_RFP35_DEBUG_SSH, "w");
	fp=fopen(BIT_IP_RFP35_DEBUG_SSH, "a");
	fprintf(fp,"\nopenSSH(%s)\n",ipaddr);
	fclose(fp);
	}
#endif				
	
	
	rc = openSSH(ipaddr,(char*)sshUser,(char*)sshPasswd,cErr);
#ifdef _CONSOLE	
	fprintf(stderr,"\nopenSSH Ret=%d\n",rc);
#endif		

#ifdef BIT_IP_RFP35_DEBUG_SSH
	{
	FILE *fp;
	fp=fopen(BIT_IP_RFP35_DEBUG_SSH, "a");
	fprintf(fp,"\nopenSSH Ret=%d\n",rc);
	fclose(fp);
	}
#endif				
	
	
	
	if ( rc != 0 ) 
	{
#ifdef BIT_IP_RFP35_DEBUG_SSH
		{
		FILE *fp;
		fp=fopen(BIT_IP_RFP35_DEBUG_SSH, "a");
		fprintf(fp, cErr );
		fclose(fp);
		}
#endif		
			
#ifdef _CONSOLE	
		fprintf(stderr,"%s\n",cErr);
#endif
		return(rc);
	}	
		
		
	strcpy(cmd,"");   // kein write
//	rc = ipRfp_libssh_WriteAndRead( cmd, gResponse,sizeof(gResponse), "omm@?*.?*.?*.*? > |Omm@?*.?*.?*.*? > |# ", 2.5 );
	rc = ipRfp_libssh_WriteAndRead( cmd, gResponse,sizeof(gResponse), "omm@|rfp4g*", 2.5 );
#ifdef _CONSOLE	
	fprintf(stderr,"\nWriteAndRead Ret=%d\n",rc);
#endif		
	if ( rc != 0 )
	{
		ipRfp_libssh_Close();
		return(rc);
	}		
		
	strcpy(cmd,"\r");
//	rc = ipRfp_libssh_WriteAndRead( cmd, gResponse, sizeof(gResponse), "omm@?*.?*.?*.*? > |Omm@?*.?*.?*.*? > |# ", 2.5 );
	rc = ipRfp_libssh_WriteAndRead( cmd, gResponse, sizeof(gResponse), "omm@|rfp4g*", 2.5 );
#ifdef _CONSOLE	
	fprintf(stderr,"\nWriteAndRead Ret=%d\n",rc);
#endif		
	if ( rc != 0 )
	{
		ipRfp_libssh_Close();
		return(rc);
	}		

		
	strcpy(ipAddrForReopen,ipaddr);			// save ip for Reopen function

		
	return(0);		// ok
}	
	


/*==============================================================================
**
** int ipRfp_libssh_changeToRootUser()
**
**  return:
**	=  0  == ok
**  <> 0  == fail
**
*/
int ipRfp_libssh_changeToRootUser(void)
{
	int  rc;
	char cErr[512];	
	
	char cmd[256];

	
		
	strcpy(cmd,"\r");
//	rc = ipRfp_libssh_WriteAndRead( cmd, gResponse, sizeof(gResponse), "omm@?*.?*.?*.*? > |Omm@?*.?*.?*.*? > |# ", 2.5 );
//	rc = ipRfp_libssh_WriteAndRead( cmd, gResponse, sizeof(gResponse), "omm@|Omm@?*.?*.?*.*? > |# ", 2.5 );
	rc = ipRfp_libssh_WriteAndRead( cmd, gResponse, sizeof(gResponse), "omm@|rfp4g*", 2.5 );
#ifdef _CONSOLE	
	fprintf(stderr,"\nWriteAndRead Ret=%d\n",rc);
#endif		
	if ( rc != 0 )
	{
		return(rc);
	}		

		
	sprintf(cmd,"su %s\r",rootUser);   // su root
	rc = ipRfp_libssh_WriteAndRead( cmd, gResponse, sizeof(gResponse), "Password: |# ", 2.5 );
#ifdef _CONSOLE	
	fprintf(stderr,"\nWriteAndRead Ret=%d\n",rc);
#endif		
	if ( rc != 0 )
	{
		return(rc);
	}		

	if ( strstr(gResponse,"Password: ") != NULL )
	{	
		
		sprintf(cmd,"%s\r",rootPasswd);   // 22222
		rc = ipRfp_libssh_WriteAndRead( cmd, gResponse, sizeof(gResponse), "# ", 2.5 );
#ifdef _CONSOLE	
		fprintf(stderr,"\nWriteAndRead Ret=%d\n",rc);
#endif		
		if ( rc != 0 )
		{
			return(rc);
		}		
	}	

	strcpy(cmd,"cd /\r");
	rc = ipRfp_libssh_WriteAndRead( cmd, gResponse, sizeof(gResponse), "# ", 2.5 );
#ifdef _CONSOLE	
	fprintf(stderr,"\nWriteAndRead Ret=%d\n",rc);
#endif		
	if ( rc != 0 )
	{
		return(rc);
	}		
		
	return(0);		// ok
}	


/*
**   getIpAddress per libssh
*/
int ipRfp_libssh_getIpAddress(char *respBuffer)
{

	int i,retry;
	int rc;
	int n1,n2,n3,n4;
	char *p,*pStart,*pEnd;



	strcpy(respBuffer,"0.0.0.0");
	
	//ifconfig
	//br0       Link encap:Ethernet  HWaddr 00:30:42:12:90:00
	//          inet addr:192.168.99.254  Bcast:192.168.99.255  Mask:255.255.255.0
	//          UP BROADCAST RUNNING MULTICAST  MTU:1500  Metric:1
	//          RX packets:41 errors:0 dropped:0 overruns:0 frame:0
	//          TX packets:15 errors:0 dropped:0 overruns:0 carrier:0
	//          collisions:0 txqueuelen:0
	//          RX bytes:5859 (5.7 KiB)  TX bytes:7263 (7.0 KiB)
	//
	//eth0      Link encap:Ethernet  HWaddr 00:30:42:12:90:00
	//          UP BROADCAST RUNNING MULTICAST  MTU:1500  Metric:1
	//          RX packets:41 errors:0 dropped:0 overruns:0 frame:0
	//          TX packets:17 errors:0 dropped:0 overruns:0 carrier:0
	//          collisions:0 txqueuelen:1000
	//          RX bytes:6597 (6.4 KiB)  TX bytes:6811 (6.6 KiB)
	//          Interrupt:11
	//
	//eth1      Link encap:Ethernet  HWaddr 02:30:42:4B:49:52
	//          UP BROADCAST RUNNING MULTICAST  MTU:1500  Metric:1
	//          RX packets:201 errors:0 dropped:1 overruns:0 frame:0
	//          TX packets:200 errors:0 dropped:0 overruns:0 carrier:0
	//          collisions:0 txqueuelen:1000
	//          RX bytes:12864 (12.5 KiB)  TX bytes:131547 (128.4 KiB)
	//          Interrupt:15
	//
	//lo        Link encap:Local Loopback
	//          inet addr:127.0.0.1  Mask:255.0.0.0
	//          UP LOOPBACK RUNNING  MTU:16436  Metric:1
	//          RX packets:135 errors:0 dropped:0 overruns:0 frame:0
	//          TX packets:135 errors:0 dropped:0 overruns:0 carrier:0
	//          collisions:0 txqueuelen:0
	//          RX bytes:6772 (6.6 KiB)  TX bytes:6772 (6.6 KiB)
		  
	//  Wenn der RFP mit WLan Karte Bestückt ist, wird ein Netzwerk "br0" (Bridge) erzeugt, dann steht dort die HWaddr und IP Addr.
	//  ist keine WLAN Karte bestückt oder nicht erkannt worden wird nur ein Netzwerk "eth0" erzeugt , dann steht dort die HWaddr und IP Addr.
	//  
	//  kommando einzeln absetzen "ifconfig br0" oder "ifconfig eth0" funktioniert nicht, da die ssh mit Fehler zurueck kommt.
	//  also nur "ifconfig" absetzten und den Text filtern.
	//
	//  eth1 ist die Interne Netzwerkschnitstelle zum BMC
	

	if ( (rc=ipRfp_libssh_WriteAndRead( "cd /\r", gResponse, sizeof(gResponse), "# ", 2.5 )) != 0 ) return(-1);

	if ( (rc=ipRfp_libssh_WriteAndRead( "/sbin/ifconfig\r", gResponse, sizeof(gResponse), "# ", 5.0 )) != 0 ) return(-2);
		
	if ( (pStart=strstr(gResponse,"br0")) != NULL )			// Device "br0" vorhanden ?
	{
	    if ( (pEnd=strstr(pStart,"eth0")) != NULL )			// nur bis Device "eth0" suchen
		{	
			*pEnd='\0';                                     // ende setzen
		}	
				
	}		
	else if ( (pStart=strstr(gResponse,"eth0")) != NULL )	// Device "eth0" vorhanden ?
	{
    	if ( (pEnd=strstr(pStart,"eth1")) != NULL )			// nur bis Device "eth1 suchen
		{	
			*pEnd='\0';                                     // ende setzen
		}	
			
	}
	else
	{	
		pStart=gResponse;
	}	

	p = strstr(pStart,"inet addr:");
	if ( p != NULL )
	{
		if ( sscanf( p ,"inet addr:%d.%d.%d.%d",&n1,&n2,&n3,&n4) == 4 )
		{	
			// 0.0.0.0 bis 255.255.255.255
			sprintf(respBuffer,"%d.%d.%d.%d",(n1&0xff),(n2&0xff),(n3&0xff),(n4&0xff));
  			return(0);	// ok					
		}	
	}

	return(-3);  // fail						
	
}



int ipRfp_libssh_sendShellCmd(char *strCmd)
{

	char cmdBuffer[512];
	
	// send a simple sh command, without anny error 

	if ( ipRfp_libssh_WriteAndRead( "cd /\r", gResponse, sizeof(gResponse), "# ", 2.5 ) != 0 ) return(-1);
	
	sprintf(cmdBuffer,"%s\r",strCmd);
	if ( ipRfp_libssh_WriteAndRead( cmdBuffer , gResponse, sizeof(gResponse), "# ", 2.5 ) != 0 ) return(-2);
	
	return(0);						
}




/*
**   getMacAddress per libssh
*/
int ipRfp_libssh_getMacAddress(char *respBuffer)
{

	int i,retry;
	int n1,n2,n3,n4,n5,n6;
	char *p,*pStart,*pEnd;



	strcpy(respBuffer,"00:00:00:00:00:00");

	
	//ifconfig
	//br0       Link encap:Ethernet  HWaddr 00:30:42:12:90:00
	//          inet addr:192.168.99.254  Bcast:192.168.99.255  Mask:255.255.255.0
	//          UP BROADCAST RUNNING MULTICAST  MTU:1500  Metric:1
	//          RX packets:41 errors:0 dropped:0 overruns:0 frame:0
	//          TX packets:15 errors:0 dropped:0 overruns:0 carrier:0
	//          collisions:0 txqueuelen:0
	//          RX bytes:5859 (5.7 KiB)  TX bytes:7263 (7.0 KiB)
	//
	//eth0      Link encap:Ethernet  HWaddr 00:30:42:12:90:00
	//          UP BROADCAST RUNNING MULTICAST  MTU:1500  Metric:1
	//          RX packets:41 errors:0 dropped:0 overruns:0 frame:0
	//          TX packets:17 errors:0 dropped:0 overruns:0 carrier:0
	//          collisions:0 txqueuelen:1000
	//          RX bytes:6597 (6.4 KiB)  TX bytes:6811 (6.6 KiB)
	//          Interrupt:11
	//
	//eth1      Link encap:Ethernet  HWaddr 02:30:42:4B:49:52
	//          UP BROADCAST RUNNING MULTICAST  MTU:1500  Metric:1
	//          RX packets:201 errors:0 dropped:1 overruns:0 frame:0
	//          TX packets:200 errors:0 dropped:0 overruns:0 carrier:0
	//          collisions:0 txqueuelen:1000
	//          RX bytes:12864 (12.5 KiB)  TX bytes:131547 (128.4 KiB)
	//          Interrupt:15
	//
	//lo        Link encap:Local Loopback
	//          inet addr:127.0.0.1  Mask:255.0.0.0
	//          UP LOOPBACK RUNNING  MTU:16436  Metric:1
	//          RX packets:135 errors:0 dropped:0 overruns:0 frame:0
	//          TX packets:135 errors:0 dropped:0 overruns:0 carrier:0
	//          collisions:0 txqueuelen:0
	//          RX bytes:6772 (6.6 KiB)  TX bytes:6772 (6.6 KiB)
		  
	//  Wenn der RFP mit WLan Karte Bestückt ist, wird ein Netzwerk "br0" (Bridge) erzeugt, dann steht dort die HWaddr und IP Addr.
	//  ist keine WLAN Karte bestückt oder nicht erkannt worden wird nur ein Netzwerk "eth0" erzeugt , dann steht dort die HWaddr und IP Addr.
	//  
	//  kommando einzeln absetzen "ifconfig br0" oder "ifconfig eth0" funktioniert nicht, da die ssh mit Fehler zuruek kommt.
	//  also nur "ifconfig" absetzten und den Text filtern.
	//
	//  eth1 ist die Interne Netzwerkschnitstelle zum BMC
	
	if ( ipRfp_libssh_WriteAndRead( "cd /\r", gResponse, sizeof(gResponse), "# ", 2.5 ) != 0 ) return(-1);
	
	if ( ipRfp_libssh_WriteAndRead( "/sbin/ifconfig\r" , gResponse, sizeof(gResponse), "# ", 5.0 ) != 0 ) return(-2);


	if ( (pStart=strstr(gResponse,"br0")) != NULL )			// Device "br0" vorhanden ?
	{
	    if ( (pEnd=strstr(pStart,"eth0")) != NULL )			// nur bis Device "eth0" suchen
		{	
			*pEnd='\0';                                     // ende setzen
		}	
				
	}		
	else if ( (pStart=strstr(gResponse,"eth0")) != NULL )	// Device "eth0" vorhanden ?
	{
	    if ( (pEnd=strstr(pStart,"eth1")) != NULL )			// nur bis Device "eth1 suchen
		{	
			*pEnd='\0';                                     // ende setzen
		}	
				
	}
	else
	{	
		pStart=gResponse;
	}	
			
	p = strstr(pStart,"HWaddr ");
	if ( p != NULL )
	{
		if ( sscanf( p ,"HWaddr %x:%x:%x:%x:%x:%x",&n1,&n2,&n3,&n4,&n5,&n6) == 6 )
		{	
			// 00:00:00:00:00:00 bis FF:FF:FF:FF:FF:FF
			sprintf(respBuffer,"%02X:%02X:%02X:%02X:%02X:%02X",(n1&0xff),(n2&0xff),(n3&0xff),(n4&0xff),(n5&0xff),(n6&0xff));
			return(0);						
		}	
	}
	return(0);
}



int ipRfp_libssh_writePARK(char *strParkHexNumber, char *strDECTrDomain, char *strWLANrDomain)
{
/*
** Generation of PARK.xml file
**
** @GC 10.09.2013
**
*/

char *p;
char cmdBuffer[512];

	

	// Generation of PARK.xml file 
	// Testinterface: (6.2 Serial interface)
	// Generates a PARK-xml file for RFP SL35 SI35 SX35 IP
	// All data must be typed as below example
	//
	//	PARK in HEX notation 	XXXXXXXXXX
	//	DECT regulatory domain  US  | EU
	//  WLAN regulatory domain  US  | DE

	// manu genpark "XXXXXXXXXX" "US" "US"
	// manu genpark "XXXXXXXXXX" "EU" "DE"

   	// # manu genpark 1F101873A2 US US
	// park = 1F101873A2, DectRegDomain=2, WlanRegDomain=111
	// successfully created /var/opt/omm/PARK.xml
	//
	//#

	if ( ipRfp_libssh_WriteAndRead( "cd /\r", gResponse, sizeof(gResponse), "# ", 2.5 ) != 0 ) return(-1);

	// manu genpark "1F101873A2" "US" "US"
	// hier darf kein Semicolon; zwischen die beiden Kommandos
	// dann ist die PATH Variable nur fuer diese Zeile gueltig
	sprintf(cmdBuffer,"PATH=$PATH:/var/run/tmpdir manu genpark \"%s\" \"%s\" \"%s\";echo Ret=[$?]\r",strParkHexNumber,strDECTrDomain,strWLANrDomain);
	if ( ipRfp_libssh_WriteAndRead( cmdBuffer , gResponse, sizeof(gResponse), "# ", 2.5 ) != 0 ) return(-2);



	// FATAL ERROR dann Fehler
	if ( (p = strstr(gResponse,"FATAL ERROR:")) != NULL )
	{
		return(-3);
	}

	// successfully created  abfragen
	if ( (p = strstr(gResponse,"successfully created")) == NULL ) return(-4);

	
	// Ret=[0] abfragen
	if ( strstr( gResponse,"Ret=[0]") == NULL ) return(-5);

	
	// Damit die PARK auch im Flash-Filesystem gespeichert wird
	sprintf(cmdBuffer,"sync\r");
	if ( ipRfp_libssh_WriteAndRead( cmdBuffer , gResponse, sizeof(gResponse), "# ", 2.5 ) != 0 ) return(-6);

	
	
	// all ok
	return(0);

}




int ipRfp_libssh_readPARK(char *strParkHexNumber)
{

/*
** read PARK Numbers mit Kommando "manu bit"
**
** Return Strings
** parkNumber: XXXXXXXXXX  
**
** @GC 22.10.2012
**
**
*/


int i,n;
char *p;
char *searchStr;
char cmdBuffer[512];
char cTemp[256];
	

	strcpy(strParkHexNumber,"");

	if ( ipRfp_libssh_WriteAndRead( "cd /\r", gResponse, sizeof(gResponse), "# ", 2.5 ) != 0 ) return(-1);

	// 	# fmanu bit
	// USB STORAGE                              UNPLUGGED
	// BMC CALIBRATION DATA                     OK
	// BMC STATUS                               RUNNING
	// TEMPERATURE SENSOR                       36 C
	// BOOTDEVICE                               bootimage 0 (via link)
	// RTC (Real Time Clock)                    OK (BOOTTIME: Thu Sep 27 08:37:43 UTC 2012)
	// WLAN MODULE                              NOT FOUND
	// SNTP                                     OK (192.168.99.2)
	// DECT LICENSE INFO                        PARK 26C01873A3, Regulatory Domain US
	// #
	
	// # fmanu bit
	// USB STORAGE                              UNPLUGGED
	// BMC CALIBRATION DATA                     OK
	// BMC STATUS                               RUNNING
	// TEMPERATURE SENSOR                       31 C
	// DECT LICENSE INFO                        ERROR -2
	// BOOTDEVICE                               bootimage 0 (via link)
	// RTC (Real Time Clock)                    OK (BOOTTIME: Thu Sep 27 06:29:11 UTC 2012)
	// WLAN MODULE                              NOT FOUND
	// SNTP                                     OK (192.168.99.2)		
	//
	// Achtung!
	// bei dem "manu bit" oder "fmanu bit" Kommando dauert es 
	// ca 20-30Sec bis die geaenderten Wert angezeigt wird. 
	//
	// nach einem Boot wird dieser String auch erst nach 30 Sec angezeigt
	// DECT LICENSE INFO                        ERROR -2
	// 
	//
	// also geht write und read nicht gleichzeitig
	//
	// In der Ausgabe fehlt eine von beiden Regulatory Domain (ist so gewollt)
	// also kann nur die PARK ueberprueft werden
	//
	//
	sprintf(cmdBuffer,"fmanu bit\r");

	if ( ipRfp_libssh_WriteAndRead( cmdBuffer , gResponse, sizeof(gResponse), "# ", 2.5 ) != 0 ) return(-2);
	
	
	p=strstr(gResponse,"DECT LICENSE INFO");
	if ( p != NULL)
	{	
		p=p+strlen("DECT LICENSE INFO");
		while ( *p == ' ' || *p == '\t') p++;      // alle Blanks ueberspringen
		for ( i=0; *p != '\n' && *p != '\r' && *p != '\0' && *p != ',' ; i++,p++ ) cTemp[i]=*p;	// kopieren bis ',' oder blank oder ende zeile
		cTemp[i] = '\0';

		// PARK 26C01873A3, Regulatory Domain US
		if ( sscanf( cTemp ,"PARK %s",strParkHexNumber) != 1 )
		{
			strcpy(strParkHexNumber,cTemp);			// "ERROR -2"
		}	
		
	}
	

	
	// all ok
	return(0);

	
}


int ipRfp_libssh_readPARKxml(char *strParkHexNumber, char *strParkOctNumber, char *strDECTrDomain, char *strWLANrDomain)
{

/*
** read PARK Numbers mit Kommando "cat /var/opt/omm/PARK.xml"
**
** Return Strings
** parkHexNumber: XXXXXXXXXX  	    1F101873A2
** parkOctNumber: oooooooooooooo	31100303472103
** DECTregulationDomain:            US EU
** WLANregulationDomain:            US DE
**
** @GC 22.10.2012
** @GC 18.03.2014  vorher Auswertung ob PARK.xml vorhanden
**
**
*/

int i;
int n;
char *p;
char *searchStr;
char cmdBuffer[512];
char cTemp[256];
	

	if ( strParkHexNumber != NULL ) strcpy(strParkHexNumber,"");
	if ( strParkOctNumber != NULL ) strcpy(strParkOctNumber,"");
	if ( strDECTrDomain != NULL ) strcpy(strDECTrDomain,"");
	if ( strWLANrDomain != NULL ) strcpy(strWLANrDomain,"");

	
	if ( ipRfp_libssh_WriteAndRead( "cd /\r", gResponse, sizeof(gResponse), "# ", 2.5 ) != 0 ) return(-1);
		
	// Example output of the cat command:
	// # cat /var/opt/omm/PARK.xml
	// <SRFPLicenseImport>
	// <!--The PARK for the SRFP (31100303472103)-->
	// <HW PARK="1F-10-18-73-A2" />
	// <!--SRFP license + description-->
	// <LICENSE desc="SRFP DECT reg. domain : US - WLAN reg. domain : US" key="7X3TS-RDLZF-78NDE-PWXTE-F66HG" />
	// <!--Checksum-->
	// <MD5>848134C48667F694EBFEC1C091CD2422</MD5>
	// </SRFPLicenseImport>
	// #	

	
    // testen ob irgend eine PARK.xml vorhanden ist
	sprintf(cmdBuffer,"ls -l /var/opt/omm\r");
	if ( ipRfp_libssh_WriteAndRead( cmdBuffer , gResponse, sizeof(gResponse), "# ", 2.5 ) != 0 ) return(-2);
	if ( (p = strstr(gResponse,"PARK.xml")) != NULL )
	{
		//vorhanden, also mit dummy fuellen
		if ( strParkHexNumber != NULL ) strcpy(strParkHexNumber,"PARK.xml");
		//if ( strParkHexNumber != NULL ) strcpy(strParkHexNumber,"XXXXXXXXXX");
		//if ( strParkOctNumber != NULL ) strcpy(strParkOctNumber,"oooooooooooooo");
	}



    // jetzt die PARK.xml auswerten
	sprintf(cmdBuffer,"cat /var/opt/omm/PARK.xml\r");

	if ( ipRfp_libssh_WriteAndRead( cmdBuffer , gResponse, sizeof(gResponse), "# ", 2.5 ) != 0 ) return(-2);
	

	
	//------------------------------------------------------
	// <!--The PARK for the SRFP (31100303472103)-->
	searchStr="The PARK for the SRFP (";
	if ( (p = strstr(gResponse,searchStr)) != NULL )
	{
		for ( p=p+strlen(searchStr) ; *p == ' ' || *p == '\t' ; p++ );			// anfang suchen
		for ( i=0; *p != '\n' && *p != '\r' && *p != '\0' && *p != '\t' && *p != ' ' && *p != ')' ; p++ )	// kopieren bis ')' oder blank oder ende zeile
		{	
			cTemp[i]=*p;
			i++;
		}
		cTemp[i]='\0';
		if ( strParkOctNumber != NULL ) strcpy(strParkOctNumber,cTemp);   
	}
	
	//------------------------------------------------------
	// <HW PARK="1F-10-18-73-A2" />
	searchStr="<HW PARK=\"";
	if ( (p = strstr(gResponse,searchStr)) != NULL )
	{
		for ( p=p+strlen(searchStr) ; *p == ' ' || *p == '\t' ; p++ );			// anfang suchen
		for ( i=0; *p != '\n' && *p != '\r' && *p != '\0' && *p != '\t' && *p != ' ' && *p != '"'; p++ )	// kopieren bis '"' oder blank oder ende zeile
		{	
			if ( *p != '-' && *p != ':' )              //aus "1F-10-18-73-A2" wird "1F101873A2"
			{	
				cTemp[i]=*p;
				i++;
			}	
		}
		cTemp[i]='\0';
		if ( strParkHexNumber != NULL ) strcpy(strParkHexNumber,cTemp);   
	}
	

	//------------------------------------------------------
	// <LICENSE desc="SRFP DECT reg. domain : US - WLAN reg. domain : US" key="7X3TS-RDLZF-78NDE-PWXTE-F66HG" />
	searchStr="DECT reg. domain :";
	if ( (p = strstr(gResponse,searchStr)) != NULL )
	{
		for ( p=p+strlen(searchStr) ; *p == ' ' || *p == '\t' ; p++ );			// anfang suchen
		for ( i=0; *p != '\n' && *p != '\r' && *p != '\0' && *p != '\t' && *p != ' ' && *p != '"' ; p++ )	// kopieren bis '"' oder blank oder ende zeile
		{	
			cTemp[i]=*p;
			i++;
		}
		cTemp[i]='\0';
		if ( strDECTrDomain != NULL ) strcpy(strDECTrDomain,cTemp);   
	}
	
	//------------------------------------------------------
	// <LICENSE desc="SRFP DECT reg. domain : US - WLAN reg. domain : US" key="7X3TS-RDLZF-78NDE-PWXTE-F66HG" />
	searchStr="WLAN reg. domain :";
	if ( (p = strstr(gResponse,searchStr)) != NULL )
	{
		for ( p=p+strlen(searchStr) ; *p == ' ' || *p == '\t' ; p++ );			// anfang suchen
		for ( i=0; *p != '\n' && *p != '\r' && *p != '\0' && *p != '\t' && *p != ' ' && *p != '"' ; p++ )	// kopieren bis '"' oder blank oder ende zeile
		{	
			cTemp[i]=*p;
			i++;
		}
		cTemp[i]='\0';
		if ( strWLANrDomain != NULL ) strcpy(strWLANrDomain,cTemp);   
	}

	
	// for debug
	//if ( strParkHexNumber != NULL && strParkOctNumber != NULL && strDECTrDomain != NULL && strWLANrDomain != NULL )
	//{	
	//	sprintf(cTemp,"ParkHexNumber=[%s]\nParkOctNumber=[%s]\nDECTregulationDomain=[%s]\nWLANregulationDomain=[%s]",strParkHexNumber, strParkOctNumber, strDECTrDomain, strWLANrDomain);
	//    MessagePopup("ipRfp_libssh_readPARKxml", cTemp );
	//}
	//else if ( strParkHexNumber != NULL && strDECTrDomain != NULL && strWLANrDomain != NULL )
	//{	
	//	sprintf(cTemp,"ParkHexNumber=[%s]\nParkOctNumber=[----]\nDECTregulationDomain=[%s]\nWLANregulationDomain=[%s]",strParkHexNumber, strDECTrDomain, strWLANrDomain);
	//    MessagePopup("ipRfp_libssh_readPARKxml", cTemp );
	//}

	
	
	// all ok
	return(0);

}

	






ipRfp_libssh_writeConfig(char *mac, char *hwType, char *branding, char *sernr, char *exid)
{

int i;
int n;
char *p;
char cmdBuffer[512];

	
    // write config nur mit manu Kommando
    //fmanu Kommmando unterstützt kein write


	//
	//  Write Mac, hwType, Branding, SerNr, Exid
	//  in OTP Flash Memory
	//
	//  Type=(RFP35|RFP36|RFP43)
	//  Branding=(ffsip|none)
	//
	//manu conf "00:30:42:12:90:00" "RFP35" "ffsip" "R9 100000714C" "9500000001"
	//



	//manu conf "00:30:42:12:90:00" "RFP35" "ffsip" "R9 100000714C" "9500000001"
	// hier darf kein Semicolon; zwischen die beiden Kommandos
	// dann ist die PATH Variable nur fuer diese Zeile gueltig
	sprintf(cmdBuffer,"PATH=$PATH:/var/run/tmpdir manu conf \"%s\" \"%s\" \"%s\" \"%s\" \"%s\";echo Ret=[$?]\r",mac,hwType,branding,sernr,exid);

	
	if ( ipRfp_libssh_WriteAndRead( "cd /\r", gResponse, sizeof(gResponse), "# ", 2.5 ) != 0 ) return(-1);

	if ( ipRfp_libssh_WriteAndRead( cmdBuffer , gResponse, sizeof(gResponse), "# ", 2.5 ) != 0 ) return(-2);
	
	if ( (p = strstr(gResponse,"FATAL ERROR:")) != NULL )
	{
		return(-3);
	}
    
	// Ret=[0] abfragen
	if ( strstr( gResponse,"Ret=[0]") == NULL ) return(-4);
	
	// all ok
	return(0);

	
}



int ipRfp_libssh_readConfig(char *mac, char *hwType, char *branding, char *sernr, char *exid)
{

/*
** Return Strings
**
** mac:        00:30:42:12:69:9f  --> 00:30:42:12:69:9F   wird nach UpCase gewandelt, da UpCase geschrieben wird
** hwType:     RFP 43             --> RFP43      das Blank wird entfernt, da auch ohne Blank geschrieben wird  nur UpCase zulässig
** hwBranding: FFSIP              --> ffsip      wird nach LowCase gewandelt, da auch nur mit LowCase geschrieben wird
**             FFSIP(SINGLE CELL) --> ffsip_sc   aus FFSIP(SINGLE CELL) wird ffsip_sc  
** string 1:   R9 100000714C            so wie es ist
** string 2:   9500848378			    so wie es ist
*/


int i;
int n;
char *p;
char cmdBuffer[512];
char *searchStr;


	mac[0]='\0';
	hwType[0]='\0';
	branding[0]='\0';
	sernr[0]='\0';
	exid[0]='\0';

		
		
	// "if type manu 2>/dev/null >/dev/null; then manu conf; else fmanu conf; fi" 
	// "if type manu 2>/dev/null >/dev/null; then { manu conf; } else  { fmanu conf; } fi" 
		
	strcpy(cmdBuffer, "if type manu 2>/dev/null 1>/dev/null; then manu conf; else fmanu conf; fi\r");  	
		
	if ( ipRfp_libssh_WriteAndRead( "cd /\r", gResponse, sizeof(gResponse), "# ", 2.5 ) != 0 ) return(-1);
	
	if ( ipRfp_libssh_WriteAndRead( cmdBuffer , gResponse, sizeof(gResponse), "# ", 2.5 ) != 0 ) return(-2);



	//# manu conf
	//current mac:       00:30:42:12:69:92
	//current hw:        RFP 43
	//current branding:  FFSIP
	//current string 1:  R9 100000714C
	//current string 2:  9500848378
	//# 
	
	
	//----------------------------------
	//current mac:       00:30:42:12:69:92
	//current mac:       00:30:42:12:69:9f
	searchStr= "current mac:";
	
	if ( (p = strstr(gResponse,searchStr)) != NULL )
	{
		for ( p=p+strlen(searchStr) ; *p == ' ' ; p++ );			// anfang suchen
		for ( i=0; *p != '\n' && *p != '\r' && *p != '\0'; p++ )	// kopieren bis ende zeile
		{	
	    	mac[i]=toupper(*p);						// mac wird mit UpCchar geschrieben, in ifconfig steht auch UpChar
			i++;									// hier wird beim lesen nur LowChar ausgegeben
	    	mac[i]='\0';							// also nach Up wandeln
		}	
	}	

	
	//----------------------------------
	//current hw:        RFP 43
	searchStr="current hw:";
	
	if ( (p = strstr(gResponse,searchStr)) != NULL )
	{
		for ( p=p+strlen(searchStr) ; *p == ' ' ; p++ );			// anfang suchen
		for ( i=0; *p != '\n' && *p != '\r' && *p != '\0'; p++ )	// kopieren bis ende zeile
		{	
			if ( *p != ' ' )                        //aus "RFP 43" wird "RFP43"
			{	
				hwType[i]=*p;
				i++;
				hwType[i]='\0';
			}	
		}	
	}
	//----------------------------------
	//current branding:  FFSIP
	searchStr="current branding:";
	
	if ( (p = strstr(gResponse,searchStr)) != NULL )
	{
		for ( p=p+strlen(searchStr) ; *p == ' ' ; p++ );			// anfang suchen
		for ( i=0; *p != '\n' && *p != '\r' && *p != '\0'; p++ )	// kopieren bis ende zeile
		{	
			branding[i]=tolower(*p);                 // ffsip,none wird in LowChar geschrieben  zurück wandeln, FFSIP=>ffsip NONE->none
			i++;									 // nur LoChar zulässig
			branding[i]='\0';
		}
		
		// @GC 22.10.2012
		// aus FFSIP(SINGLE CELL)  wird ffsip_sc
		if ( strcmp(branding,"ffsip(single cell)") == 0 )
		{	
			strcpy(branding,"ffsip_sc");
		}	

	}	
	
	//----------------------------------
	//current string 1:  R9 100000714C
	searchStr="current string 1:";
	
	if ( (p = strstr(gResponse,searchStr)) != NULL )
	{
		for ( p=p+strlen(searchStr) ; *p == ' ' ; p++ );				// anfang suchen
		for ( i=0; *p != '\n' && *p != '\r' && *p != '\0'; p++)			// kopieren bis ende zeile
		{	
	    	sernr[i]=*p;
			i++;
	    	sernr[i]='\0';
		}	
	}	

	//----------------------------------
	//current string 2:  9500848378
	searchStr="current string 2:";
	
	if ( (p = strstr(gResponse,searchStr)) != NULL )
	{
		for ( p=p+strlen(searchStr) ; *p == ' ' ; p++ );				// anfang suchen
		for ( i=0; *p != '\n' && *p != '\r' && *p != '\0'; p++)			// kopieren bis ende zeile
		{	
	    	exid[i]=*p;
			i++;
	    	exid[i]='\0';
		}	
	}	
	
	
	// all ok
	return(0);

	
}




/*
**   die Software Version vom manu Tool lesen
**	Return "SIP-DECT 3.0 (May 10, 2011)"
**
*/
int ipRfp_libssh_getManuVersion(char *pManuVersionFW,char *pManuVersionSW)
{

	char *p1;
	char *p2;
	int  i;
	int  flag;


	char cmdBuffer[256];


	strcpy(pManuVersionFW,"xxxxx");         //Firmware Version   (onboard)
	strcpy(pManuVersionSW,"xxxxx");         //Software Version   (usb stick)


	// # manu version
	//version of the manu package
	//SIP-DECT 3.0 manufacturing release (May 10, 2011)
	//
	//version of the running sw
	//SIP-DECT 3.0 manufacturing release (May 10, 2011)
	//May 10 2011 14:58:30
	//# 


	// /media/usbStick/rfp43/sbin/manu
	// /sbin/fmanu

//version of the manu package
//SIP-DECT 3.0RC2 Build 2
//
//version of the running sw
//SIP-DECT 3.0RC2 Build 2
//Jun  9 2011 11:50:32
	
	//	PATH=/media/usbStick/rfp43/sbin:/sbin:/usr/sbin:$PATH
	// "if type manu 2>/dev/null >/dev/null; then manu version; else fmanu version; fi" 
	// "if type manu 2>/dev/null >/dev/null; then { manu version; } else  { fmanu version; } fi" 
	
	
	if ( ipRfp_libssh_WriteAndRead( "cd /\r", gResponse, sizeof(gResponse), "# ", 2.5 ) != 0 ) return(-1);
	
	sprintf(cmdBuffer,"%s\r","if type manu 2>/dev/null 1>/dev/null; then manu version; else fmanu version; fi");  	
	if ( ipRfp_libssh_WriteAndRead( cmdBuffer, gResponse, sizeof(gResponse), "# ", 2.5 ) != 0 ) return(-2);

	
	
	
	//
	// gesammte Zeile
	//

	
    // erste Zeile SIP-DECT
	p1 = strstr(gResponse,"SIP-DECT");
	if ( p1 != NULL )
	{	
		// bis ZeileEnde oder StringEnde
		for (i=0 ; *p1 != '\n' && *p1 != '\r' && *p1 != '\0' ; p1++ , i++)
		{
			pManuVersionFW[i] = *p1;              // erste Zeile SIP-DECT
			pManuVersionFW[i+1] = '\0';
		}
		
	    // zweite Zeile SIP-DECT
		p2 = strstr(p1,"SIP-DECT");
		if ( p2 != NULL )
		{	
			// bis ZeileEnde oder StringEnde
			for (i=0 ; *p2 != '\n' && *p2 != '\r' && *p2 != '\0' ; p2++ , i++)
			{
				pManuVersionSW[i] = *p2;              // zweite Zeile SIP-DECT
				pManuVersionSW[i+1] = '\0';
			}
		
		}
	}

	
	return(0);						
}




/*
**     get USB Device if found
**     new      @GC 10.07.2012
**     modified @GC 27.09.2012
**     modified @GC 22.10.2012
**     modified @GC 20.03.2014
*/
int ipRfp_libssh_getUsbDev(char *usbdevStr)
{

int  i,n;
int  retry;
char *p;
char cTemp[256];


	// Achtung:
	// diese Funktion testet ob ein USB-Stick vorhanden ist
	// Wenn USB-Stick vorhanden ist.
	// wird eine Datei "uImage" mit Size 0 auf dem Stick erzeugt, (wenn noch nicht vorhanden)
	// und loescht die Datei "PARK.xml" vom Stick (wenn vorhanden)
	//
	

	strcpy(usbdevStr,"UNPLUGGED");
	
	
	
	if ( ipRfp_libssh_WriteAndRead( "cd /\r", gResponse, sizeof(gResponse), "# ", 2.5 ) != 0 ) return(-1);
	
	//
	// alte Software
	//
	// # ls -l /dev/usb*
	// lrwxrwxrwx    1 root     root            4 Jul 10 10:20 /dev/usbStick -> sda1
	// crw-rw----    1 root     root     189,   0 Jul  7 03:57 /dev/usbdev1.1
	// crw-rw----    1 root     root     189,   2 Jul 10 10:20 /dev/usbdev1.3
	// # 
	

	//
	// ab Software 3.1 Lite
	//
	// # ls -lR /dev/bus/usb*
	// /dev/bus/usb:
	// drwxr-xr-x    2 root     root           80 Jan 15 03:27 001
	// 
	// /dev/bus/usb/001:
	// crw-rw-r--    1 root     root     189,   0 Jan 15 03:27 001
	// crw-rw-r--    1 root     root     189,   1 Jan 15 03:27 002
	
	
	// # ls -lR /dev/usb*
	// lrwxrwxrwx    1 root     root            4 Jan 15 03:27 /dev/usbStick -> sda1
	
	
	// # ls -lR /dev/sd*
	// brw-rw----    1 root     disk       8,   0 Jan 15 03:27 /dev/sda
	// brw-rw----    1 root     disk       8,   1 Jan 15 03:27 /dev/sda1
	// #
	
	
	// 	# ls -lR /dev/disk/
	// /dev/disk/:
	// drwxr-xr-x    2 root     root           80 Jan 15 03:27 by-id
	// drwxr-xr-x    2 root     root           80 Jan 15 03:27 by-path
	// drwxr-xr-x    2 root     root           60 Jan 15 03:27 by-uuid
	// 
	// /dev/disk/by-id:
	// lrwxrwxrwx    1 root     root            9 Jan 15 03:27 usb-058f_6387_12083060000247-0:0 -> ../../sda
	// lrwxrwxrwx    1 root     root           10 Jan 15 03:27 usb-058f_6387_12083060000247-0:0-part1 -> ../../sda1
	// 
	// /dev/disk/by-path:
	// lrwxrwxrwx    1 root     root            9 Jan 15 03:27 platform-orion-ehci.0-usb-0:1:1.0-scsi-0:0:0:0 -> ../../sda
	// lrwxrwxrwx    1 root     root           10 Jan 15 03:27 platform-orion-ehci.0-usb-0:1:1.0-scsi-0:0:0:0-part1 -> ../../sda1
	// 
	// /dev/disk/by-uuid:
	// lrwxrwxrwx    1 root     root           10 Jan 15 03:27 B476-C767 -> ../../sda1
	// #
	

	// # df
	// Filesystem           1K-blocks      Used Available Use% Mounted on
	// tmpfs                    16384        40     16344   0% /tmp
	// udev                      1024        48       976   5% /dev
	// /dev/usbStick           983760     21728    962032   2% /media/usbStick
	// ubi3:flash               57832     16428     38464  30% /flash
	// ubi4:var                 56884       228     53764   0% /var
	// varlock                   1024         0      1024   0% /var/lock
	// varrun                   32768        28     32740   0% /var/run
	// shmfs                    62184         0     62184   0% /dev/shm
	// #

	
	// der mount der Filesystem kann bis zu 5 Sec dauern
	for ( p=NULL, retry=10;  retry > 0 ; retry-- )
	{

		// ab FW SIP-DECT 6.0 SP2-BA11   
		// benoetigt der RFP bis zu 12Sek. bis der Prompt zurueck kommt.
		// speziell bei dem "df" Kommando wenn der USB Stick gemountet wird
		// 18.01.2016 @ GC
	
		//if ( ipRfp_libssh_WriteAndRead( "df\r", gResponse, sizeof(gResponse), "# ", 2.5 ) != 0 ) return(-3);
		if ( ipRfp_libssh_WriteAndRead( "df\r", gResponse, sizeof(gResponse), "# ", 15.0 ) != 0 ) return(-3);

		// /dev/usbStick vorhanden ?
		if ( (p=strstr(gResponse,"/dev/usb")) != NULL )
		{
			break;       // usbStick found
		}
		
		Sleep(1000);           // wait 1s       
		
	}

	if ( p != NULL )
	{
		if ( sscanf( p ,"/dev/%s %d",usbdevStr,&n) != 2 )   // "usbStick"    // es darf kein "/" zurück gegeben werden
		{
	  		strcpy(usbdevStr,"UNPLUGGED");
		}			
	}

	// nur als Debug Output
	if ( ipRfp_libssh_WriteAndRead( "ls -lR /dev/bus/usb*\r", gResponse, sizeof(gResponse), "# ", 2.5 ) != 0 ) return(-4);
	if ( ipRfp_libssh_WriteAndRead( "ls -lR /dev/usb*\r", gResponse, sizeof(gResponse), "# ", 2.5 ) != 0 ) return(-5);
	if ( ipRfp_libssh_WriteAndRead( "ls -lR /dev/sd*\r", gResponse, sizeof(gResponse), "# ", 2.5 ) != 0 ) return(-6);
	if ( ipRfp_libssh_WriteAndRead( "ls -lR /dev/disk\r", gResponse, sizeof(gResponse), "# ", 2.5 ) != 0 ) return(-7);

	

	// @GC neu 12.03.2014 
	// wenn PARK.xml auf usbstick vorhanden ist dann
	// kommt als Zusatz ein "+" hinter dem usbStick
	// zur Erkennung in den LogFiles ob PARK.xml auf dem Stick vorhanden ist 
	
	// der RFP kopiert sich die "/media/usbStick/PARK.xml" nach "/var/opt/omm/PARK.xml"
	// und auch wieder zurueck, damit ist sie dann auf dem Stick

	
	// Hier die eMail zur Erklärung von Detlef Schoof Aastra Berlin
	// wir müssen der Ursache auf den Grund kommen.
	// Zuerst einmal habe ich hier das Statement der Entwicklung:
	// wir liefern zwei ZIP Container an die Fertigung, einer ist für SIP-DECT, der zweite für SIP-DECT-Lite.
	// Mit den ZIP Containern wird ein USB-Stick beschrieben, der keine anderen Daten enthält, als die des
	// jeweiligen Containers. Der USB-Stick muss also vor dem Entpacken leer sein.
	// Die USB-Sticks sind ausschließlich für die gleiche SW zu verwenden, also SIP-DECT-Lite USB-Stick
	// für SIP-DECT-Lite und SIP-DECT USB-Sticks für SIP-DECT!
	// Das gilt für alle Schritte der Fertigung, Flachbaugruppentest, Finaltest und Reparatur.
	// Wenn man sich an die oben beschriebene Prozedur hält, dann kann ein PARK.xml Stick niemals
	// auf ein SIP-DECT RFP gelangen!
	// 
	// Wann wird ein PARK.xml File kopiert bzw. nicht kopiert?
	// Wenn der RFP nicht vom internen Flash gebootet wurde, wird nicht kopiert, weder von internem
	// Flash auf den USB-Stick, noch umgekehrt.
	// 
	// Wenn auf dem USB-Stick ein File uImage oder uImageFailSafe existiert wird nicht kopiert,
	// weder von internem Flash auf den USB-Stick, noch umgekehrt.
	// Eine Kopie wird nur erzeugt, wenn auf dem Ziel keine oder eine ungültige PARK.xml liegt.
    //
	// Wann und wie wird eine PARK.xml Datei erzeugt?
	// Das Kommano „manu genpark“ erzeugt eine Datei PARK.xml im internen Flash.
	// Das Kommando steht nur auf den SIP-DECT-Lite USB-Stick zur Verfügung.

	// Information von Carsten Lange Aastra Berlin
	// Ich möchte aber vorschlagen, dass auf dem Stick, der in dem Finaltest verwendet wird, eine Datei der Länge 0 mit dem Namen uImage angelegt wird.
	// Dann wird die auf den SIP-DECT-Lite RFPs vorhandene PARK.xml Datei nicht auf den Stick kopiert.

	//
	// Damit erst gar keine PARK.xml erzeugt wird, soll auf dem Stick eine uImage Datei mit Size 0 sein
	//
	
	// usb stick vorhanden
	if ( strcmp(usbdevStr,"UNPLUGGED") != 0 )  
	{	
		// List /var/opt/omm und /media/usbStick
		if ( ipRfp_libssh_WriteAndRead( "ls -l /var/opt/omm\r", gResponse, sizeof(gResponse), "# ", 2.5 ) != 0 ) return(-9);
		if ( ipRfp_libssh_WriteAndRead( "ls -l /media/usbStick\r", gResponse, sizeof(gResponse), "# ", 2.5 ) != 0 ) return(-9);
	
		if ( (p = strstr(gResponse,"PARK.xml")) != NULL )
		{
			strcat(usbdevStr,"+");   // "PARK.xml" vorhanden
		}
		
		// Datei uImage mit size 0 auf usb-stick erzeugen, wenn noch nicht vorhanden
		//test -e /media/usbStick/uImage || touch /media/usbStick/uImage
		if ( ipRfp_libssh_WriteAndRead( "test -e /media/usbStick/uImage || touch /media/usbStick/uImage\r", gResponse, sizeof(gResponse), "# ", 2.5 ) != 0 ) return(-9);
	
	
		// Datei PARK.xml vom usb-stick loeschen, wenn vorhanden
		//test -e /media/usbStick/PARK.xml && rm -rf /media/usbStick/PARK.xml
		if ( ipRfp_libssh_WriteAndRead( "test -e /media/usbStick/PARK.xml && rm -rf /media/usbStick/PARK.xml\r", gResponse, sizeof(gResponse), "# ", 2.5 ) != 0 ) return(-9);

		
		// List /var/opt/omm und /media/usbStick
		if ( ipRfp_libssh_WriteAndRead( "ls -l /var/opt/omm\r", gResponse, sizeof(gResponse), "# ", 2.5 ) != 0 ) return(-9);
		if ( ipRfp_libssh_WriteAndRead( "ls -l /media/usbStick\r", gResponse, sizeof(gResponse), "# ", 2.5 ) != 0 ) return(-9);
		
	}
	
	
	
	
	
	//
	// neu 27.09.2012 
	// ab FW "SIP-DECT-Lite_3_1RC3" hat das manu Kommando auch eine Erkennung des USB-Stick
	// wird vorerst nur bei RFP SL35 eingesetzt, 
	// kann also nicht immer abgefragt werden.
	//
	// der Output ist:
	// USB STORAGE                             PLUGGED|UNPLUGGED
	//	
	//	# fmanu bit
	//USB STORAGE                              PLUGGED
	//BMC CALIBRATION DATA                     OK
	//BMC STATUS                               RUNNING
	//TEMPERATURE SENSOR                       36 C
	//BOOTIMAGE DOWNLOAD                       IMAGE (file:///media/usbStick/iprfp3G_Addons.dnld) is already local
	//BOOTIMAGE 0                              UPTODATE
	//BOOTDEVICE                               bootimage 0 (via link)
	//RTC (Real Time Clock)                    OK (BOOTTIME: Thu Sep 27 06:58:06 UTC 2012)
	//WLAN MODULE                              NOT FOUND
	//SNTP                                     OK (192.168.99.2)
	//DECT LICENSE INFO                        PARK 26C01873A3, Regulatory Domain US
	//#

    // muss auf jeden Fall über "fmanu" gemacht werden, denn das "manu" Kommando ist ohne USB-Stick nicht vorhanden.	
	if ( ipRfp_libssh_WriteAndRead( "fmanu bit\r", gResponse, sizeof(gResponse), "# ", 2.5 ) != 0 ) return(-8);

	// der String "USB STORAGE" im Output gefunden also ist das manu Kommando neu
	// und kann ausgwertet werden
	if ( (p=strstr(gResponse,"USB STORAGE")) != NULL )
	{
		p = p+strlen("USB STORAGE");
		while ( *p == ' ' || *p == '\t') p++;      // alle Blanks ueberspringen
		for ( i=0 ; *p != '\0' && *p != '\n' && *p != '\r' ; p++,i++)	   //bis Zeilenende kopieren
		{	
			cTemp[i] = *p;
		}	
		cTemp[i]='\0';
		if ( strcmp(cTemp,"PLUGGED") != 0 )  
		{
			// doch nicht gefunden	
			strcpy(usbdevStr,cTemp);        // UNPLUGGED
		}	
	}
	
	
	
	return(0);						
}





/*
**   den Temperature Sensor lesen
*/
int ipRfp_libssh_getTemperature_func(double *temperature)
{

char *p;
int  n;


	*temperature = 1;    // 1 grad c

	// # manu bit
	// BMC CALIBRATION DATA                     OK
	// BMC STATUS                               RUNNING
	// TEMPERATURE SENSOR                       43 C
	// # 

	// "if type manu 2>/dev/null >/dev/null; then manu bit; else fmanu bit; fi" 
	// "if type manu 2>/dev/null >/dev/null; then { manu bit; } else  { fmanu bit; } fi" 
	
	if ( ipRfp_libssh_WriteAndRead( "cd /\r", gResponse, sizeof(gResponse), "# ", 2.5 ) != 0 ) return(-1);
	
	if ( ipRfp_libssh_WriteAndRead( "fmanu bit\r", gResponse, sizeof(gResponse), "# ", 2.5 ) != 0 ) return(-2);
	
	p = strstr(gResponse,"TEMPERATURE");
	if ( p != NULL )
	{	
		sscanf(p,"TEMPERATURE SENSOR %d",&n);
		*temperature =(double)n;
	}
	return(0);						
}
/******************************************************************
**	@GC 15.05.2014 mit SSH Reopen
*/
int ipRfp_libssh_getTemperature(double *temperature)
{
	int rc;
	
	if ( (rc=ipRfp_libssh_getTemperature_func( temperature )) != 0 )
	{
		rc=ipRfp_libssh_Reopen();
		if ( rc == 0 )
		{
			rc=ipRfp_libssh_getTemperature_func( temperature );	
		}	
	}

	return(rc);
	
}



/*****************************************************************
**   enable BMC Testmode
*/
int ipRfp_libssh_enableBMCTestmode_func(void)
{

int  retry;
char bmcStatus[80];



	// # manu bit
	// BMC CALIBRATION DATA                     OK
	// BMC STATUS                               RUNNING (TESTMODE)
	// TEMPERATURE SENSOR                       43 C
	// # 


	// (f)manu testmode [on|off]
    //    turn rfp testmode on or off
	
	//
	// ab FW SIP-DECT-Lite 3.1RC4
	//
	// wenn der RFP komplett konfiguriert ist, (mit MAC und PARK)
	// echte MAC Addresse (nicht default)
	// PARK Nummer
	// geht der BMC nicht mehr in den Testmode.
	// dann benutzt er nicht mehr die Test RFPI Nummer "1000000000"
	// sondern eine RFPI nummer die aus der PARK nummer generiert wurde.
	//
	// durch das Kommando "(f)manu testmode on" 
	// geht der BMC wieder in Testmode (RFPI Nummer "1000000000")
	// nach dem Kommando wird der BMC neu gestartet.
	// dies muss mit "(f)manu bit" abgefragt werden.
	//
	// Dies Kommando hat nichts mit dem RSX Kommando "EnterTestmode" zu tun.
	//
	
	
	if ( ipRfp_libssh_WriteAndRead( "cd /\r", gResponse, sizeof(gResponse), "# ", 2.5 ) != 0 ) return(-1);
	
	if ( ipRfp_libssh_WriteAndRead( "fmanu testmode on;echo Ret=[$?]\r" , gResponse, sizeof(gResponse), "# ", 2.5 ) != 0 ) return(-2);

	// Ret=[0] abfragen
	if ( strstr( gResponse,"Ret=[0]") != NULL )  // ok
	{
		// Kommando vorhanden und ohne fehler ausgefuehrt
		// warten bis BMC neu geladen wird (not RUNNING)
		for ( retry=10 ; retry > 0 ; retry-- )
		{
			if ( ipRfp_libssh_getBMCStatus(bmcStatus,NULL,NULL) ) return(-3);
			
			if ( strcmp(bmcStatus,"RUNNING") != 0 )
				break;				 // kein RUNNING 
			Sleep(500);   // 500ms
		}
		
	}
	else
	{
		// die alten SW Versionen haben das kommando nicht
		//return(-1);   // fail
	}	

    return(0);
	
}
/******************************************************************
**	@GC 15.05.2014 mit SSH Reopen
*/
int ipRfp_libssh_enableBMCTestmode(void)
{
	int rc;
	
	if ( (rc=ipRfp_libssh_enableBMCTestmode_func( )) != 0 )
	{
		rc=ipRfp_libssh_Reopen();
		if ( rc == 0 )
		{
			rc=ipRfp_libssh_enableBMCTestmode_func( );	
		}	
	}

	return(rc);
	
}
	
	
	


/*
**   den BMC STATUS lesen
*/
int ipRfp_libssh_getBMCStatus_func(char *bmcStatus, char *bmcTestmode ,char *bmcCalibStatus)
{

char *p,*pa;
int  n;

	if ( bmcStatus != NULL ) strcpy(bmcStatus,"xxxxx");
	if ( bmcTestmode != NULL ) strcpy(bmcTestmode,"");
	if ( bmcCalibStatus != NULL ) strcpy(bmcCalibStatus,"xx");


	// # manu bit
	// BMC CALIBRATION DATA                     OK
	// BMC STATUS                               RUNNING
	// BMC STATUS                               RUNNING (TESTMODE)
	// TEMPERATURE SENSOR                       43 C
	// # 

	
	
	//manu testmode [on|off]
    // turn rfp testmode on or off
	// BMC STATUS                               RUNNING (TESTMODE)
	

	// der BMC STATUS            geht von LOADING nach RUNNING
	// der BMC CALIBRATION DATA  Status "OK" wird nach dem finalisieren gesetzt
	//                           wird aber erst nach einem Reboot aktualisiert
	
	// "if type manu 2>/dev/null >/dev/null; then manu bit; else fmanu bit; fi" 
	// "if type manu 2>/dev/null >/dev/null; then { manu bit; } else  { fmanu bit; } fi" 
	
	
	if ( ipRfp_libssh_WriteAndRead( "cd /\r", gResponse, sizeof(gResponse), "# ", 2.5 ) != 0 ) return(-1);
	
	if ( ipRfp_libssh_WriteAndRead( "fmanu bit\r" , gResponse, sizeof(gResponse), "# ", 2.5 ) != 0 ) return(-2);
	
	if ( bmcStatus != NULL )
    {
        p = strstr(gResponse,"BMC STATUS");
        if ( p != NULL )
        {	
            //sscanf(p,"BMC STATUS %s",bmcStatus);
			p=p+strlen("BMC STATUS");
			while ( *p == ' ' || *p == '\t') p++;      // alle Blanks ueberspringen
			
			for ( pa=bmcStatus ; *p != '\0' && *p != '\n' && *p != '\r' ; pa++, p++)	   //bis Zeilenende kopieren
			{	
				*pa = *p;
			}	
			*pa='\0';       // ende str
			if ( (p=strstr(bmcStatus," (TESTMODE)")) != NULL  ) 
			{	
				*p='\0';    // nur bis TESTMODE
				if (  bmcTestmode != NULL )
				{
					strcpy(bmcTestmode,p+1);       // Testmode 
				}	
			}	
			
				
        }
    }    
        
	if ( bmcCalibStatus != NULL )
    {
        p = strstr(gResponse,"BMC CALIBRATION DATA");
        if ( p != NULL )
        {	
            //sscanf(p,"BMC CALIBRATION DATA %s",bmcCalibStatus);
			p=p+strlen("BMC CALIBRATION DATA");
			while ( *p == ' ' || *p == '\t') p++;      // alle Blanks ueberspringen
			
			for ( pa=bmcCalibStatus ; *p != '\0' && *p != '\n' && *p != '\r' ; pa++, p++)	   //bis Zeilenende kopieren
			{	
				*pa = *p;
			}	
			*pa='\0';       // ende str
			
        }
    }    
        
    return(0);						
}
/******************************************************************
**	@GC 15.05.2014 mit SSH Reopen
*/
int ipRfp_libssh_getBMCStatus(char *bmcStatus, char *bmcTestmode ,char *bmcCalibStatus)
{
	int rc;
	
	if ( (rc=ipRfp_libssh_getBMCStatus_func(bmcStatus, bmcTestmode, bmcCalibStatus)) != 0 )
	{
		rc=ipRfp_libssh_Reopen();
		if ( rc == 0 )
		{
			rc=ipRfp_libssh_getBMCStatus_func(bmcStatus, bmcTestmode, bmcCalibStatus);	
		}	
	}

	return(rc);
	
}






/*
**   den RTC STATUS lesen
**   
**   1.) RTC Status
**   2.) NTP Status
**   3.) DateTime Check
**
**   neu @GC 11.11.2011
*/
int ipRfp_libssh_getRTCStatus(char *rtcStatus, char *ntpStatus, char *dateString)
{

	int  n;
	char *p;
	char *pResp;
	char cmdBuffer[256];

	if ( rtcStatus != NULL ) 	strcpy(rtcStatus,"xx");
	if ( ntpStatus != NULL ) 	strcpy(ntpStatus,"xx");
	if ( dateString != NULL )   strcpy(dateString,"xx");

	//	# fmanu bit; echo "ACTUAL_DATE $(date -u -Iseconds)"
	//BMC CALIBRATION DATA                     OK
	//BMC STATUS                               RUNNING
	//TEMPERATURE SENSOR                       35 C
	//BOOTDEVICE                               bootimage 0 (via link)
	//RTC (Real Time Clock)                    OK (BOOTTIME: Thu Oct 10 11:01:23 UTC 1929)
	//RTC (Real Time Clock)                    FAILED
	//WLAN MODULE                              NOT FOUND
	//IP Configuration                         DHCP (server IP: 192.168.99.2)
	//SNTP                                     OK (192.168.99.2)
	//BOOTIMAGE DOWNLOAD                       FAILED (tftp://192.168.99.2/) (Error)
	//ACTUAL_DATE 2011-11-11T09:02:39
	//UPTIME  17:24:25 up 20 min, load average: 0.00, 0.01, 0.01
	//#

	
	// ab Firmware SIP-DECT 6.0SP2-BA11 wird die Zeile SNTP nicht mehr ausgegeben.
	// NTP ist anscheinend abgeschaltet.
	// damit kann auch das aktuelle Datum nicht mehr abgefragt werden.
	// Laut Carten Lange ist die Abfrage nicht nötig.
	// 02.02.2016 @G.Cordes
	//
	//# fmanu bit;date -u +"ACTUAL_DATE %Y-%m-%dT%H:%M:%S";echo "UPTIME $(uptime)"
	//fmanu bit;date -u +"ACTUAL_DATE %Y-%m-%dT%H:%M:%S";echo "UPTIME $(uptime)"
	//BOOTIMAGE 0                              UPTODATE
	//BOOTIMAGE 1                              COPIED from BOOTIMAGE 0
	//BOOTDEVICE                               USB Stick
	//RTC (Real Time Clock)                    OK (BOOTTIME: Sat Jan 1 00:00:12 UTC 2000)
	//WLAN MODULE                              FOUND
	//BMC STATUS                               RUNNING (TESTMODE)
	//USB STORAGE                              PLUGGED
	//TEMPERATURE SENSOR                       27 C
	//BOOTIMAGE DOWNLOAD                       IMAGE (file:///media/usbStick/iprfp3G.dnld) is already local
	//ACTUAL_DATE 2000-01-01T00:01:29
	//UPTIME  00:01:29 up 1 min,  load average: 0.87, 0.33, 0.12
	
	
	
	//fmanu bit; date -u +"ACTUAL_DATE %Y-%m-%dT%H:%M:%S"
	

	// "if type manu 2>/dev/null >/dev/null; then manu bit; else fmanu bit; fi" 
	// "if type manu 2>/dev/null >/dev/null; then { manu bit; } else  { fmanu bit; } fi" 
	// "if type manu 2>/dev/null >/dev/null; then { manu bit;date -u +\"ACTUAL_DATE %Y-%m-%dT%H:%M:%S\"; } else  { fmanu bit;date -u +\"ACTUAL_DATE %Y-%m-%dT%H:%M:%S\"; } fi"
	//
	// "fmanu bit;date -u +\"ACTUAL_DATE %Y-%m-%dT%H:%M:%S\";echo \"UPTIME $(uptime)\""
	//
	
	if ( ipRfp_libssh_WriteAndRead( "cd /\r", gResponse, sizeof(gResponse), "# ", 2.5 ) != 0 ) return(-1);
	
	strcpy(cmdBuffer,"fmanu bit;date -u +\"ACTUAL_DATE %Y-%m-%dT%H:%M:%S\";echo \"UPTIME $(uptime)\"\r");  	
	if ( ipRfp_libssh_WriteAndRead( cmdBuffer, gResponse, sizeof(gResponse), "# ", 2.5 ) != 0 ) return(-2);
	

	// erste Zeile weglassen
	// erste Zeile ueberspringen
	pResp=gResponse;
	while ( *pResp != '\n' && *pResp != '\r'  && *pResp != '\0' )  pResp++;

	
	if ( pResp != NULL )
	{	
		if ( rtcStatus != NULL )
		{	
			p = strstr(pResp,"RTC (Real Time Clock)");
			if ( p != NULL )
			{	
				sscanf(p,"RTC (Real Time Clock) %s",rtcStatus);
			}
		}	
		
		if ( ntpStatus != NULL )
		{	
			p = strstr(pResp,"SNTP");
			if ( p != NULL )
			{	
				sscanf(p,"SNTP %s",ntpStatus);
			}
		}	
		

		if ( dateString != NULL )
		{	
			p = strstr(pResp,"ACTUAL_DATE");
			if ( p != NULL )
			{	
				// UTC Universal Time entsricht GMT
				//ACTUAL_DATE 2011-11-11T09:02:39
				sscanf(p,"ACTUAL_DATE %s",dateString);
			}
		}	
	
	}
	
	
	return(0);						
}



/*
**  check Boot Device
**
**  new      @GC 10.07.2012
**  modified @GC 17.01.2013
**
**  Return: "USB Stick"
**          "BOOTIMAGE 0"  
**          "BOOTIMAGE 0 (via link)"  
**          "BOOTIMAGE 1"
**          "BOOTIMAGE 1 (via link)"  
**
**  RetValue  0 = ok
**  RetValue >0 = output not complete
**  RetValue <0 = fail ssh don't work
**
**  
*/

int ipRfp_libssh_checkBootDevice(char *bootDevice)
{
int  i;	
char *p;


	if ( bootDevice != NULL ) strcpy(bootDevice,"NOT FOUND");

	// Ausgabe nicht komplett
	//
	// # manu bit
	// USB STORAGE                              PLUGGED
	// DECT MIGRATION LICENSE                   NOT FOUND
	// BMC CALIBRATION DATA                     OK
	// BMC STATUS                               RUNNING
	// #
	
	// Ausgabe komplett Funktion
	//
	// # manu bit
	// BMC CALIBRATION DATA                     OK
	// BMC STATUS                               RUNNING
	// TEMPERATURE SENSOR                       37 C
	// IP Configuration                         DHCP (server IP: 192.168.99.2)
	// BOOTDEVICE                               bootimage 0 (via link)
	// RTC (Real Time Clock)                    OK (BOOTTIME: Sat Jul 7 03:57:29 UTC 2012)
	// WLAN MODULE                              NOT FOUND
	// SNTP                                     OK (192.168.99.2)
	// BOOTIMAGE 0                              UPTODATE
	// BOOTIMAGE DOWNLOAD                       FAILED (tftp://192.168.99.2/) (Error)
	// #
	
	
	// Ausgabe komplett Funktion
	//
	// 	# manu bit
	// BOOTIMAGE 0                              INVALID
	// BOOTIMAGE 1                              INVALID
	// BMC STATUS                               RUNNING
	// TEMPERATURE SENSOR                       28 C
	// IP Configuration                         DHCP (server IP: 192.168.99.2)
	// PARTITION /dev/mtd3 on flash             INITIALIZED
	// PARTITION /dev/mtd4 on var               INITIALIZED
	// BOOTDEVICE                               USB Stick
	// RTC (Real Time Clock)                    OK (BOOTTIME: Wed Jun 5 21:31:03 UTC 1940)
	// WLAN MODULE                              NOT FOUND
	// SNTP                                     OK (192.168.99.2)
	// #
	
	// Ausgabe komplett Final 
	//
	// 	# manu bit
	// USB STORAGE                              PLUGGED
	// DECT MIGRATION LICENSE                   NOT FOUND
	// BMC CALIBRATION DATA                     OK
	// BMC STATUS                               RUNNING
	// BOOTDEVICE                               bootimage 0 (via link)
	// RTC (Real Time Clock)                    OK (BOOTTIME: Tue Jan 15 16:11:02 UTC 2013)
	// WLAN MODULE                              NOT FOUND
	// SNTP                                     OK (192.168.99.2)
	// DECT LICENSE INFO                        PARK 1F1026319E, Regulatory Domain US


	
	
	// Info:
	// BOOTIMAGE 0 UPTODATE kommt nur wenn auf dem USB Stick das BOOTIMAGE vorhanden ist.
	
	
	
	// "if type manu 2>/dev/null >/dev/null; then manu bit; else fmanu bit; fi" 
	// "if type manu 2>/dev/null >/dev/null; then { manu bit; } else  { fmanu bit; } fi" 
	
	
	if ( ipRfp_libssh_WriteAndRead( "cd /\r", gResponse, sizeof(gResponse), "# ", 2.5 ) != 0 ) return(-1);
	
	if ( ipRfp_libssh_WriteAndRead( "ls -lR /flash/boot\r", gResponse, sizeof(gResponse), "# ", 2.5 ) != 0 ) return(-2);
		
	if ( ipRfp_libssh_WriteAndRead( "fmanu bit\r", gResponse, sizeof(gResponse), "# ", 2.5 ) != 0 ) return(-3);

	if ( (p=strstr(gResponse,"BOOTDEVICE")) != NULL )
	{
		if ( bootDevice != NULL )
		{	
			// BOOTDEVICE                               bootimage 0 (via link) 
			p=p+strlen("BOOTDEVICE");
			while ( *p == ' ' || *p == '\t') p++;      // alle Blanks ueberspringen

			// "bootimage 0 (via link)"
       		for (i=0  ; *p != '\n' && *p != '\r' && *p != '\0' ; i++, p++ ) 	   //bis Zeilenende kopieren
       		{
           		bootDevice[i] = *p;
       		}        
       		bootDevice[i] = '\0';        // ende str
		}	
		return(0);       // BOOTDEVICE found
	}
		
	return(1);	         // output not complete
}	






/*
**   den WLAN Module Status lesen
*/
int ipRfp_libssh_getWLanStatus(char *respBuffer)
{

char *p;
int  i;


	strcpy(respBuffer,"xx");


	// manu bit
	// BMC CALIBRATION DATA                     OK
	// BMC STATUS                               RUNNING
	// IP Configuration                         DHCP (server IP: 192.168.99.2)
	// BOOTDEVICE                               bootimage 0
	// RTC (Real Time Clock)                    OK (BOOTTIME: Tue Jun 14 11:01:55 UTC 2011)
	// WLAN MODULE                              FOUND
	// SNTP                                     OK (192.168.99.2)
	// TEMPERATURE SENSOR                       41 C
	// BOOTIMAGE DOWNLOAD                       FAILED (tftp://192.168.72.2/) (Error)


	if ( ipRfp_libssh_WriteAndRead( "cd /\r", gResponse, sizeof(gResponse), "# ", 2.5 ) != 0 ) return(-1);
	
	if ( ipRfp_libssh_WriteAndRead( "fmanu bit\r", gResponse, sizeof(gResponse), "# ", 2.5 ) != 0 ) return(-2);

	p = strstr(gResponse,"WLAN MODULE");
	if ( p != NULL )
	{	
        //sscanf funktioniert nicht, da antwort entweder "FOUND" oder "NOT FOUND" 
		//sscanf(p,"WLAN MODULE %s",respBuffer);
        
        p= p + strlen("WLAN MODULE");    
        for (  ; *p == ' ' ; p++ ) ;      // bis zum ende der Blanks
        
        // bis Ende der Zeile kopieren
        for (i=0  ; *p != '\n' && *p != '\r' && *p != '\0' ; i++, p++ )
        {
            respBuffer[i] = *p;
            respBuffer[i+1] = '\0';
        }        
        
	}
	
	return(0);						
}

/*
**  FactoryReset 
**	The RFP will be reset to Factory delivery status
**
**  PARK.xml wird auch geloescht
**
*/
int ipRfp_libssh_FactoryReset(void)
{

char *p;
int  n;

char cmdBuffer[256];



	// "if type manu 2>/dev/null >/dev/null; then manu factoryreset; else fmanu factoryreset; fi" 
	// "if type manu 2>/dev/null >/dev/null; then { manu factoryreset; } else  { fmanu factoryreset; } fi" 

	if ( ipRfp_libssh_WriteAndRead( "cd /\r", gResponse, sizeof(gResponse), "# ", 2.5 ) != 0 ) return(-1);
	
	strcpy(cmdBuffer,"fmanu factoryreset;echo Ret=[$?]\r");  	
	if ( ipRfp_libssh_WriteAndRead( cmdBuffer, gResponse, sizeof(gResponse), "# ", 2.5 ) != 0 ) return(-2);
	
	// Ret=[0] abfragen
	if ( strstr( gResponse,"Ret=[0]") == NULL ) return(-3);

	
	return(0);						
}


/*
**
**  FactoryCleanup 
**	The RFP will be reset to Factory delivery status
**
**  fmanu factoryreset
**    resets all data to system defaults and reboot
**
**	fmanu factorycleanup
**    cleanup all data from the system, keeping some important files and reboot
**	 
**
**	PARK.xml bleibt erhalten
**  
**  
**
*/
int ipRfp_libssh_FactoryCleanup(void)
{

char *p;
int  n;

char cmdBuffer[256];


	// fuer RFP SL35

	// Neue Kommandos und Abläufe:
	// #manu ssh [enable|disable]
	// #[fmanu|manu] factorycleanup
	// Bisher wurde in der Fertigung das Kommando
	// #[fmanu|manu] factoryreset
	// verwendet. Dieses Kommando löscht alle Daten im Flash Filesystem, auch PARK.xml.
	// Soll das File PARK.xml erhalten bleiben, so ist das Kommando
	// #[fmanu|manu] factorycleanup
	// zu verwenden.


	if ( ipRfp_libssh_WriteAndRead( "cd /\r", gResponse, sizeof(gResponse), "# ", 2.5 ) != 0 ) return(-1);
	
	strcpy(cmdBuffer,"fmanu factorycleanup;echo Ret=[$?]\r");  	
	if ( ipRfp_libssh_WriteAndRead( cmdBuffer, gResponse, sizeof(gResponse), "# ", 2.5 ) != 0 ) return(-2);
	
	// Ret=[0] abfragen
	if ( strstr( gResponse,"Ret=[0]") == NULL ) return(-3);

	
	return(0);						
}



/*
**   den Link Status auslesen
*/
int ipRfp_libssh_getLinkStatus_func(char *ethDev, char *linkStatus)				   // ethDev = eth0, eth1
{
int  i;
char *p;

char cmdBuffer[256];
char tempstr[256];



	strcpy(linkStatus,"link down");

    
    		//# ethtool eth1
		//Settings for eth1:
		//        Supported ports: [ TP AUI BNC MII FIBRE ]
		//        Supported link modes:   10baseT/Half 10baseT/Full
		//                                100baseT/Half 100baseT/Full
		//        Supports auto-negotiation: Yes
		//        Advertised link modes:  10baseT/Half 10baseT/Full
		//                                100baseT/Half 100baseT/Full
		//        Advertised pause frame use: No
		//        Advertised auto-negotiation: Yes
		//        Speed: 100Mb/s
		//        Duplex: Full
		//        Port: MII
		//        PHYAD: 1
		//        Transceiver: external
		//        Auto-negotiation: on
		//        Link detected: yes
		//#

		//# ethtool eth0
		//Settings for eth0:
		//        Supported ports: [ TP MII ]
		//        Supported link modes:   10baseT/Half 10baseT/Full
		//                                100baseT/Half 100baseT/Full
		//                                1000baseT/Full
		//        Supports auto-negotiation: Yes
		//        Advertised link modes:  10baseT/Half 10baseT/Full
		//                                100baseT/Half 100baseT/Full
		//                                1000baseT/Full
		//        Advertised pause frame use: No
		//        Advertised auto-negotiation: Yes
		//        Speed: 1000Mb/s
		//        Duplex: Full
		//        Port: MII
		//        PHYAD: 0
		//        Transceiver: external
		//        Auto-negotiation: on
		//        Link detected: yes
		//#
		//




    
	//link up, 100Mb's, full duplex
    //link up, 1000Mb's, full duplex
    //link down, 1000Mb's, full duplex

	// ethtool eth0
	// ethtool eth1
	sprintf(cmdBuffer,"/sbin/ethtool \"%s\"\r", ethDev);

	if ( ipRfp_libssh_WriteAndRead( "cd /\r", gResponse, sizeof(gResponse), "# ", 2.5 ) != 0 ) return(-1);
	
	if ( ipRfp_libssh_WriteAndRead( cmdBuffer, gResponse, sizeof(gResponse), "# ", 2.5 ) != 0 ) return(-2);

	p = strstr( gResponse,"Link detected: yes");
	if ( p != NULL )
		strcpy(linkStatus,"link up, ");
	else
		strcpy(linkStatus,"link down, ");

	//        Speed: 1000Mb/s
	p = strstr( gResponse,"Speed: ");
	if ( p != NULL )
    {
        if ( sscanf( p ,"Speed: %s",tempstr) == 1 )
        {
            strcat(linkStatus,tempstr);         // 1000Mb/s
            strcat(linkStatus,", ");
        }    
    }        
        
	p = strstr( gResponse,"Duplex: ");
	if ( p != NULL )
    {
        if ( sscanf( p ,"Duplex: %s",tempstr) == 1 )
        {
            stringToLow(tempstr);
            strcat(linkStatus,tempstr);         //full
            strcat(linkStatus," duplex");
        }   
    }        
    
    
    for ( p=linkStatus; *p != '\0' ; p++ )    // es darf kein / im string sein  
        if ( *p == '/' ) *p='\'';             // also gegen ' ersetzen
    
	return(0);
	
}
/******************************************************************
**	@GC 15.05.2014 mit SSH Reopen
*/
int ipRfp_libssh_getLinkStatus(char *ethDev, char *linkStatus)				   // ethDev = eth0, eth1
{
	int rc;
	
	if ( (rc=ipRfp_libssh_getLinkStatus_func(ethDev, linkStatus)) != 0 )
	{
		rc=ipRfp_libssh_Reopen();
		if ( rc == 0 )
		{
			rc=ipRfp_libssh_getLinkStatus_func(ethDev, linkStatus);	
		}	
	}

	return(rc);
	
}


/******************************************************************
**   die LEDs ein/aus schalten
**   ledCmd= on,red,green,off,system	
**
*/
int ipRfp_libssh_setLED_func(char *ledCmd)
{

int  n;

char cmdBuffer[256];

	//	manu led on
    //  all leds are turned on
	//manu led red
    //  all red leds are turned on
	//manu led green
    // all green leds are turned on
	//manu led off
    // all leds are turned off
	//manu led system
    // 	all leds are controlled by the normal system operation

	if ( strcmp(ledCmd,"on")     != 0 && 
		 strcmp(ledCmd,"red")    != 0 &&
		 strcmp(ledCmd,"green")  != 0 &&
		 strcmp(ledCmd,"blue")   != 0 &&
		 strcmp(ledCmd,"off")    != 0 &&
		 strcmp(ledCmd,"system") != 0)
	{
		return(-1);						
	}	

	// on,red,green,off,system	

	// "if type manu 2>/dev/null >/dev/null; then manu led red; else fmanu led red; fi" 
	// "if type manu 2>/dev/null >/dev/null; then { manu led red; } else  { fmanu led red; } fi" 

	sprintf(cmdBuffer,"if type manu 2>/dev/null 1>/dev/null; then manu led %s;echo Ret=[$?]; else fmanu led %s;echo Ret=[$?]; fi\r",
		ledCmd, ledCmd );  	

	
	if ( ipRfp_libssh_WriteAndRead( "cd /\r", gResponse, sizeof(gResponse), "# ", 2.5 ) != 0 ) return(-2);
	
	if ( ipRfp_libssh_WriteAndRead( cmdBuffer, gResponse, sizeof(gResponse), "# ", 2.5 ) != 0 ) return(-3);
	
	// Ret=[0] abfragen
	if ( strstr( gResponse,"Ret=[0]") == NULL ) return(-4);

	
	return(0);						
	
}
/******************************************************************
**	@GC 15.05.2014 mit SSH Reopen
*/
int ipRfp_libssh_setLED(char *ledCmd)
{
	int rc;
	
	
	if ( (rc=ipRfp_libssh_setLED_func( ledCmd )) != 0 )
	{
		rc=ipRfp_libssh_Reopen();
		if ( rc == 0 )
		{
			rc=ipRfp_libssh_setLED_func( ledCmd );	
		}	
	}

	return(rc);
	
}



/******************************************************************
**   Heating Elemnt ein/aus schalten
**   sCmd= on,off
**
*/
int ipRfp_libssh_setHeating_func(char *sCmd)
{

int  n;

char cmdBuffer[256];

	//	manu heater on
	//	manu heater off

	if ( strcmp(sCmd,"on") != 0 && strcmp(sCmd,"off") != 0 )
	{
		return(-1);						
	}	
		
	// "if type manu 2>/dev/null >/dev/null; then manu heater on; else fmanu heater on; fi" 
	// "if type manu 2>/dev/null >/dev/null; then { manu heater on; } else  { fmanu heater on; } fi" 

	sprintf(cmdBuffer,"if type manu 2>/dev/null 1>/dev/null; then manu heater %s;echo Ret=[$?]; else fmanu heater %s;echo Ret=[$?]; fi\r",
		sCmd, sCmd );  	

	if ( ipRfp_libssh_WriteAndRead( "cd /\r", gResponse, sizeof(gResponse), "# ", 2.5 ) != 0 ) return(-2);
	
	if ( ipRfp_libssh_WriteAndRead( cmdBuffer, gResponse, sizeof(gResponse), "# ", 2.5 ) != 0 ) return(-3);

	// Ret=[0] abfragen
	if ( strstr( gResponse,"Ret=[0]") == NULL ) return(-4);
	
	return(0);						
	
}
/******************************************************************
**	@GC 15.05.2014 mit SSH Reopen
*/
int ipRfp_libssh_setHeating(char *sCmd)
{
	int rc;
	
	if ( (rc=ipRfp_libssh_setHeating_func( sCmd )) != 0 )
	{
		rc=ipRfp_libssh_Reopen();
		if ( rc == 0 )
		{
			rc=ipRfp_libssh_setHeating_func( sCmd );	
		}	
	}

	return(rc);
	
}


/******************************************************************
**   set Wlan Mode
**   manu wlan off
**   manu wlan 2.4 g on
**   manu wlan 2.4 n on
**   manu wlan 5.18 n on
**
*/
int ipRfp_libssh_setWLAN(char *wlanMode)
{

	int  n;
	int  rc;
	int  retry;
	char *p;

	char cmdBuffer[1024];

	if ( strcmp(wlanMode,"off")       != 0 &&
		 strcmp(wlanMode,"2.4 g on")  != 0 &&
		 strcmp(wlanMode,"2.4 n on")  != 0 &&
		 strcmp(wlanMode,"5.18 n on") != 0 )
	{
		return(-1);						
	}	


        
        
	// "if type manu 2>/dev/null >/dev/null; then manu wlan off; else fmanu wlan off; fi" 
	// "if type manu 2>/dev/null >/dev/null; then { manu wlan off; } else  { fmanu wlan off; } fi" 
    
    // /sbin:/usr/sbin:/bin:/usr/bin:

    // "PATH=/sbin:/usr/sbin:/bin:/usr/bin:/media/usbStick/rfp43/bin:/media/usbStick/rfp43/usr/bin:/media/usbStick/rfp43/usr/local/bin:/media/usbStick/rfp43/sbin:/media/usbStick/rfp43/usr/sbin:/media/usbStick/rfp43/usr/local/sbin:/private/bin:/private/sbin:/private/usr/sbin:/private/usr/bin"
    // "PATH=/media/usbStick/rfp43/sbin:/sbin:/usr/sbin:$PATH"
    // ". /etc/profile"

	if ( ipRfp_libssh_WriteAndRead( "cd /\r", gResponse, sizeof(gResponse), "# ", 3.0 ) != 0 ) return(-2);
	
	// dmesg -c  loeschen
	if ( ipRfp_libssh_WriteAndRead( "dmesg -c\r", gResponse, sizeof(gResponse), "# ", 3.0 ) != 0 ) return(-3);

	
	sprintf(cmdBuffer,"if type manu 2>/dev/null 1>/dev/null; then manu wlan %s;echo Ret=[$?]; else fmanu wlan %s;echo Ret=[$?]; fi\r",
		wlanMode, wlanMode );  	
	
	
	for ( rc=1, retry=2 ; rc != 0 && retry > 0 ; retry-- )
	{	
	
		if ( ipRfp_libssh_WriteAndRead( cmdBuffer, gResponse, sizeof(gResponse), "# ", 15.0 ) != 0 ) return(-4);
	
		// Ret=[0] abfragen
		// Das manu wlan Kommando kommt leider immer mit Ret=[0] zurück
		if ( strstr( gResponse,"Ret=[0]") == NULL ) 
		{
			rc=-5;           // kein [0] 
			continue;
		}	

	
		//
		// wenn Kommando ungleich "off"
		//
		if ( strcmp(wlanMode,"off") != 0 )
		{
	
			//	nl80211: Failed to set interface wlan0 into AP mode
			//  nl80211 driver initialization failed.
			if ( strstr( gResponse,"Failed to set interface wlan") != NULL ) 
			{	
				rc=-6;			// fail
				continue;
			}	

			if ( strstr( gResponse,"driver initialization failed") != NULL )
			{	
				rc=-7;			// fail
				continue;
			}		
		
		

			// dmesg -c  Ringbuffer lesen und gleichzeitig loeschen
			// # dmesg -c
			// device wlan0 entered promiscuous mode
			// br0: port 2(wlan0) entering forwarding state
			// br0: port 2(wlan0) entering forwarding state
			if ( ipRfp_libssh_WriteAndRead( "dmesg -c\r", gResponse, sizeof(gResponse), "# ", 3.0 ) != 0 ) return(-8);
		
			// device wlan0 entered promiscuous mode
			if ( strstr( gResponse,"device wlan0 entered promiscuous mode") == NULL )
			{	
				rc=-9;			// fail
				continue;
			}	


			// ifconfig -a
			//# ifconfig -a
			//br0       Link encap:Ethernet  HWaddr 00:30:42:12:6A:B0
			//          inet addr:192.168.99.240  Bcast:192.168.99.255  Mask:255.255.255.0
			//          UP BROADCAST RUNNING MULTICAST  MTU:1500  Metric:1
			//          RX packets:1054 errors:0 dropped:0 overruns:0 frame:0
			//          TX packets:1016 errors:0 dropped:0 overruns:0 carrier:0
			//          collisions:0 txqueuelen:0
			//          RX bytes:116006 (113.2 KiB)  TX bytes:332697 (324.8 KiB)
			//
			//eth0      Link encap:Ethernet  HWaddr 00:30:42:12:6A:B0
			//          UP BROADCAST RUNNING MULTICAST  MTU:1500  Metric:1
			//          RX packets:1054 errors:0 dropped:0 overruns:0 frame:0
			//          TX packets:1025 errors:0 dropped:0 overruns:0 carrier:0
			//          collisions:0 txqueuelen:1000
			//          RX bytes:134978 (131.8 KiB)  TX bytes:332567 (324.7 KiB)
			//          Interrupt:11
			//
			//eth1      Link encap:Ethernet  HWaddr 02:30:42:4B:49:52
			//          UP BROADCAST RUNNING MULTICAST  MTU:1500  Metric:1
			//          RX packets:494 errors:0 dropped:1 overruns:0 frame:0
			//          TX packets:493 errors:0 dropped:0 overruns:0 carrier:0
			//          collisions:0 txqueuelen:1000
			//          RX bytes:31616 (30.8 KiB)  TX bytes:146035 (142.6 KiB)
			//          Interrupt:15
			//
			//lo        Link encap:Local Loopback
			//          inet addr:127.0.0.1  Mask:255.0.0.0
			//          UP LOOPBACK RUNNING  MTU:16436  Metric:1
			//          RX packets:709 errors:0 dropped:0 overruns:0 frame:0
			//          TX packets:709 errors:0 dropped:0 overruns:0 carrier:0
			//          collisions:0 txqueuelen:0
			//          RX bytes:35472 (34.6 KiB)  TX bytes:35472 (34.6 KiB)
			//
			//mon.wlan0 Link encap:UNSPEC  HWaddr 00-0E-8E-27-E3-16-12-E4-00-00-00-00-00-00-00-00
			//          UP BROADCAST RUNNING MULTICAST  MTU:1500  Metric:1
			//          RX packets:1537 errors:0 dropped:0 overruns:0 frame:0
			//          TX packets:0 errors:0 dropped:0 overruns:0 carrier:0
			//          collisions:0 txqueuelen:1000
			//          RX bytes:226896 (221.5 KiB)  TX bytes:0 (0.0 B)
			//
			//wlan0     Link encap:Ethernet  HWaddr 00:30:42:12:6A:B0
			//          UP BROADCAST RUNNING MULTICAST  MTU:1500  Metric:1
			//          RX packets:0 errors:0 dropped:0 overruns:0 frame:0
			//          TX packets:48 errors:0 dropped:0 overruns:0 carrier:0
			//          collisions:0 txqueuelen:1000
			//          RX bytes:0 (0.0 B)  TX bytes:4255 (4.1 KiB)
			//
		
			
			// mit retry @GC 14.03.2012
			for (p=NULL,n=6 ; n > 0 ; n-- )
			{	
				// 26.07.2013 @GC  Timeout von 2.5 auf 5.0 wegen neu Firmware SIP-DECT 4.0SP1 hat mehr Zeilen im Output,
				if ( ipRfp_libssh_WriteAndRead( "ifconfig -a\r", gResponse, sizeof(gResponse), "# ", 5.0 ) != 0 ) return(-10);
	
				//mon.wlan0 Link encap:UNSPEC  HWaddr 00-0E-8E-27-E3-16-12-E4-00-00-00-00-00-00-00-00
				//if ( strstr( gResponse,"mon.wlan0") == NULL ) return(-11);
				if ( (p=strstr( gResponse,"mon.wlan0")) != NULL ) break; 
				
				Sleep(500); // 500ms
			
			}
			if ( p == NULL )
			{	
				rc=-11;     // fail 
				continue;
			}	
			
		}	//strcmp(wlanMode,"off") != 0	

		
		rc=0;
		break;      //ok
		
		
	} // for retry	
		

	return(rc);						
	
}

/*
**   enable RSX over tcpip port 38477
**
**	 port 38477 open for communication with RSX
**
*/
int ipRfp_libssh_enableRSX(void)
{

char cmdBuffer[256];
        

    //
	//  ip_rfpconsole
	//	rsx on
	//  exit
	//


    //    Zum Testzugang im Finaltest:
    //    In der Systemshell bitte eingeben :
    //    # conclient -p 10318 -x "rsx on"
    //
    //   (Hinweis unserer SWer : Fehlermeldungen beim Aufruf bitte ignorieren)
    
    //   Fehlermeldungen nach /dev/null umleiten

	sprintf(cmdBuffer,"/usr/bin/conclient -p 10318 -x \"rsx on\" 2>/dev/null\r");

	if ( ipRfp_libssh_WriteAndRead( "cd /\r", gResponse, sizeof(gResponse), "# ", 2.5 ) != 0 ) return(-1);
	
	if ( ipRfp_libssh_WriteAndRead( cmdBuffer, gResponse, sizeof(gResponse), "# ", 2.5 ) != 0 ) return(-2);
	
	return(0);						
	
}

/**********************************************************************************************************
**    copy Files to DUT
**    set execute rights
**
**
** Es gibt drei Möglichkeiten, eine Datei zum RFP zu kopieren.
** 
** 1.) scp / pscp.exe  (ssh)
** copy vom PC zum RFP, dafür muss man das root login/passwd vom RFP benutzen,
** und der RFP muss den ssh Service geöffnet haben.
** Auf der PC Seite kann das Programm pscp.exe aus dem packet tputty benutzt werden.
** SyntaxPC:  
** pscp.exe" -scp -pw 22222 manupath\manu root@192.168.99.254:/var/tmp/manu
** 	
** 2.) FTP
** in diesem Fall holt sich der RFP mit dem Kommando ftpget die Datei vom PC ab.
** Dafür muss auf dem PC ein FTP Deamon/Service laufen und in dessen ftpRootPath muss die Datei liegen. 
** Die ftpdmin.exe kann als einfacher FTP Deamon genutzt werden.
** Syntax PC:
** ftpdmin.exe -ha 192.168.99.2 ftpRootPath
** Syntax RFP:
** ftpget 192.168.99.2 "manu" "manu"
** 
** 3.) TFTP
** In diesem fall holt sich der RFP mit dem Kommando tftp die Datei vom PC ab.
** Dafür muss auf dem PC ein TFTP Deamon/Service laufen und in dessen ftpRootPath muss die Datei liegen. 
** Der dhcpsrv für den Boot des RFPs unterstützt das laden von Dateien per TFTP
** Die Datei manu muss aber im Verzeichnis des dhcpsrv liegen
** Syntax RFP:
** tftp  -g -r "sbin\manu" 192.168.99.2 -l "/var/run/tmpdir/manu"
** 
**
**
** ipRfp_libssh_FtpGet(char *hostIpAddr, char *srcPath)
**
** Diese Version kopiert alle Dateien die unter dem srcPath liegen
** mit ftpget zum Testling
**
**
**
**********************************************************************************************************/
int ipRfp_libssh_FtpGet(char *hostIpAddr, char *srcPath, char *srcFileName)
{
	int  rc;
	char cmdBuffer[256];
	char searchPath[256];
	char fileName[256];


	
	// manu bit
	// BMC CALIBRATION DATA                     OK
	// BMC STATUS                               RUNNING
	// IP Configuration                         DHCP (server IP: 192.168.99.2)
	// BOOTDEVICE                               bootimage 0
	// RTC (Real Time Clock)                    OK (BOOTTIME: Tue Jun 14 11:01:55 UTC 2011)
	// WLAN MODULE                              FOUND
	// SNTP                                     OK (192.168.99.2)
	// TEMPERATURE SENSOR                       41 C
	// BOOTIMAGE DOWNLOAD                       FAILED (tftp://192.168.72.2/) (Error)
	
	
	// mit diesem script kann auf dem RFP aus der manu Information die Server ipAddr ermittelt werden.
	//fmanu bit | awk -v 'FS=:| |\\)' '/DHCP/ {print $(NF-1) }'
	//fmanu bit | awk '/DHCP/ {gsub(/)/,"");print $(NF) }'



	if ( ipRfp_libssh_WriteAndRead( "cd /\r", gResponse, sizeof(gResponse), "# ", 2.5 ) != 0 ) return(-1);
	
    // Verzeichniss erzeugen
	if ( ipRfp_libssh_WriteAndRead( "mkdir /var/run/tmpdir\r", gResponse, sizeof(gResponse), "# ", 2.5 ) != 0 ) return(-2);

    // execute Rechte setzen
	if ( ipRfp_libssh_WriteAndRead( "chmod 777 /var/run/tmpdir\r", gResponse, sizeof(gResponse), "# ", 2.5 ) != 0 ) return(-3);

    
	
	
	
	// srcPath = "G:\ProTest\rfp35_Software\20110609\rfp43-manu\rfp43\sbin"
	// srcFileName = "manu"  oder "*.*"
	// searchPath = "G:\ProTest\rfp35_Software\20110609\rfp43-manu\rfp43\sbin\manu"
	// searchPath = "G:\ProTest\rfp35_Software\20110609\rfp43-manu\rfp43\sbin\*.*"
	sprintf(searchPath,"%s\\%s",srcPath,srcFileName);          
	
	rc=GetFirstFile (searchPath, /*Normal*/1,/*ReadOnly*/1,/*System*/1,/*Hidden*/1,/*Archive*/0,/*Directory*/0, fileName);
	if ( rc == 0 )
	{	
		do
		{	
			// fuer jedes File welches unter sbin steht
			// MessagePopup("ipRfp_libssh_FtpGet", fileName);

#define USE_WGET			
#ifndef USE_WGET			
			
			//ftpget 192.168.99.2 "/var/run/tmpdir/manu" "manu"
			//ftpget 192.168.99.2 "/var/run/tmpdir/manu.map" "manu.map"

	
			//Usage: ftpget [options] remote-host local-file remote-file
			//Options:
    		//    -c, --continue
    		//    -v, --verbose
    		//    -u, --username
    		//    -p, --password
    		//    -P, --port

	    	// FTP get
			sprintf(cmdBuffer,"ftpget %s /var/run/tmpdir/%s %s;echo Ret=[$?]\r", hostIpAddr,fileName,fileName);
			if ( ipRfp_libssh_WriteAndRead( cmdBuffer, gResponse, sizeof(gResponse), "# ", 15.0 ) != 0 ) return(-4);
#else //USE_WGET
			
			// 01.02.2016 @G.Cordes
			// ab Firmware SIP-DECT 6.0SP2-BA11 ist kein ftpget mehr vorhanden,
			// als alternative wget
			
			// wget ftp://192.168.99.2//manu -P /var/run/tmpdir/ -O	/var/run/tmpdir/manu	
			// wget ftp://192.168.99.2//manu.map -P /var/run/tmpdir/ -O	/var/run/tmpdir/manu.map	
			
			
			//Usage: wget [-csq] [-O file] [-Y on/off] [-P DIR] [-U agent] url
			//
			//Retrieve files via HTTP or FTP
			//
			//Options:
			//        -s      Spider mode - only check file existence
			//        -c      Continue retrieval of aborted transfer
			//        -q      Quiet
			//        -P      Set directory prefix to DIR
			//        -O      Save to filename ('-' for stdout)
			//        -U      Adjust 'User-Agent' field
			//        -Y      Use proxy ('on' or 'off')			
	    	// wget
			// minimal: wget sagt beim zweiten mal fehler, file ist bereits vorhanden
			//sprintf(cmdBuffer,"wget ftp://%s/%s -P /var/run/tmpdir/;echo Ret=[$?]\r", hostIpAddr,fileName);
			// maximal: wget funktioniert immer
			sprintf(cmdBuffer,"wget ftp://%s/%s -P /var/run/tmpdir/ -O /var/run/tmpdir/%s;echo Ret=[$?]\r", hostIpAddr,fileName,fileName);
			if ( ipRfp_libssh_WriteAndRead( cmdBuffer, gResponse, sizeof(gResponse), "# ", 15.0 ) != 0 ) return(-4);
			
#endif //USE_WGET
	
			// Ret=[0] abfragen
			if ( strstr( gResponse,"Ret=[0]") == NULL ) return(-5); // fail


			
		    // execute Rechte setzen
			sprintf(cmdBuffer,"chmod 777 /var/run/tmpdir/%s\r", fileName);
			if ( ipRfp_libssh_WriteAndRead( cmdBuffer, gResponse, sizeof(gResponse), "# ", 2.5 ) != 0 ) return(-6);
			
			
		}
		while ( (rc=GetNextFile(fileName)) == 0 );

		return(0);   // ok, mindestens 1 File kopiert
		
	}
	
	return(1);	     // fail,  kein File gefunden					
	
}



/*
**    delete srcFile from DUT
**
*/
int ipRfp_libssh_deleteFilesFromDut(void)
{
    
    // file mit Verzeichniss loeschen
	if ( ipRfp_libssh_WriteAndRead( "cd /\r", gResponse, sizeof(gResponse), "# ", 2.5 ) != 0 ) return(-1);
	
	if ( ipRfp_libssh_WriteAndRead( "rm -rf /var/run/tmpdir\r", gResponse, sizeof(gResponse), "# ", 2.5 ) != 0 ) return(-2);
	
	return(0);						
	
}




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////



