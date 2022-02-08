#include "WLanEmbedded.h"
#include "test.h"
#include <rs232.h>
#include <windows.h> // for Sleep 



int WLanEmbedded_Port1, WLanEmbedded_Port2, WLanEmbedded_Port3;
unsigned char wlanResultStr[0x800];

int WLanEmbedded_open (int comPort1,int comPort2, int comPort3)
{
int i = 0;
	WLanEmbedded_Port1 = comPort1;
	WLanEmbedded_Port2 = comPort2;
	WLanEmbedded_Port3 = comPort3;
	i += OpenComConfig (comPort1, "",115200, 0, 8, 1, 512, 512);
	i += OpenComConfig (comPort2, "",115200, 0, 8, 1, 512, 512);
	i += OpenComConfig (comPort3, "",115200, 0, 8, 1, 512, 512);
// setup CLIENT MODE:
	i+= WLanEmbedded_write (WLanEmbedded_Port1, "AT+CWMODE=1");
	i+= WLanEmbedded_write (WLanEmbedded_Port2, "AT+CWMODE=1");
	i+= WLanEmbedded_write (WLanEmbedded_Port3, "AT+CWMODE=1");
	return(i);
}


int WLanEmbedded_ListAP(int *ant1, int *ant2, int *ant3)
{
int j,i = 0;
char	*chrptr;

// write command to all modules:

	i+= WLanEmbedded_write (WLanEmbedded_Port1, "AT+CWLAP");
	i+= WLanEmbedded_write (WLanEmbedded_Port2, "AT+CWLAP");
	i+= WLanEmbedded_write (WLanEmbedded_Port3, "AT+CWLAP");
	Sleep(3000);
// read answers:
/*
AT=CWLAP
+CWLAP:(4,"EasyBox-823546",-88,"60:e3:27:3b:13:39",1,6,0)
+CWLAP:(3,"WLAN-644276",-90,"18:83:bf:7f:08:5a",1,-6,0)
+CWLAP:(3,"Auerhahn",-93,"d0:05:2a:7d:d2:c0",1,5,0)
+CWLAP:(3,"WLAN-831005",-84,"74:31:70:83:10:fb",1,-2,0)
+CWLAP:(3,"rfp4gManu",-6,"00:15:61:20:5d:7a",6,1,0)
+CWLAP:(4,"... tut auch Weh - Lan",-71,"24:65:11:52:3e:cd",6,-57,0)
+CWLAP:(3,"... tut Weh LAN",-92,"e0:28:6d:fa:c9:0d",6,-4,0)
+CWLAP:(3,"WLAN-188439",-92,"e4:3e:d7:cc:a1:50",6,-2,0)
+CWLAP:(4,"EasyBox-609437",-93,"74:31:70:60:94:77",1,-47,0)
+CWLAP:(3,"FRITZ!Box 7590 OD",-91,"44:4e:6d:3a:e4:71",8,-12,0)
+CWLAP:(3,"GE-Elektronik",-68,"a2:05:43:b4:bb:aa",12,-26,0)
+CWLAP:(3,"GE-Elektronik",-75,"a0:e4:cb:78:9e:62",12,-2,0)

*/	
	j = WLanEmbedded_read (WLanEmbedded_Port1, wlanResultStr);
	if (wlanResultStr != NULL && (chrptr = strstr(wlanResultStr,"rfp4gManu")) != NULL )           // sucht den bootsring
	{// gefunden
	 	chrptr +=11; //rfp4gManu", = 11char
	 	i = atoi(chrptr);
	 	*ant1 = i;
	}
	j = WLanEmbedded_read (WLanEmbedded_Port2, wlanResultStr);
	if (wlanResultStr != NULL && (chrptr = strstr(wlanResultStr,"rfp4gManu")) != NULL )           // sucht den bootsring
	{// gefunden
	 	chrptr +=11; //rfp4gManu", = 11char
	 	i = atoi(chrptr);
	 	*ant2 = i;
	}
	j = WLanEmbedded_read (WLanEmbedded_Port3, wlanResultStr);
	if (wlanResultStr != NULL && (chrptr = strstr(wlanResultStr,"rfp4gManu")) != NULL )           // sucht den bootsring
	{// gefunden
	 	chrptr +=11; //rfp4gManu", = 11char
	 	i = atoi(chrptr);
	 	*ant3 = i;
	}
    	
	
	return (0);
}




int WLanEmbedded_write (int comPort, char write_char[])
{
int bytesWritten, count, localError=0;
char rxBuffer[0x200];


	if(gDebugWlanEmbedded)
	{
   		sprintf(rxBuffer,"Port: %d:", comPort);
   		printf(rxBuffer); 
   		printf(write_char);
   	}
	count = strlen(write_char);
    bytesWritten = ComWrt (comPort, write_char, count);
    bytesWritten += ComWrtByte (comPort,0x0d);
    bytesWritten += ComWrtByte (comPort,0x0a);
// check mirrored data:	
	count = ComRd (comPort,rxBuffer,bytesWritten);
    if(count != bytesWritten) localError++;
/* 
 	Sleep(50);
	FlushInQ (comPort);	
*/	
    return(localError);
    
}



int WLanEmbedded_read (int comPort, char read[])
{
int bytesRead;
char info[0x20];

    Sleep(50);
    bytesRead = GetInQLen (comPort);
    ComRd (comPort, read, bytesRead);
    read[bytesRead] = 0; //terminate string

	if(gDebugWlanEmbedded)
	{
   		sprintf(info,"Port: %d:", comPort);
   		printf(info); 
		printf(read);
	}
	return(bytesRead);
}




void WLanEmbedded_close (void)
{
   CloseCom (WLanEmbedded_Port1); 
   CloseCom (WLanEmbedded_Port2); 
   CloseCom (WLanEmbedded_Port3); 
}

