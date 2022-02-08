#include "MiniSystem.h"
#include "test.h"
#include <rs232.h>



int MiniSysPort;
unsigned char ResultStr[0x100];

int MiniSys_open (int comPort)
{
int i;
	MiniSysPort = comPort;
	i = OpenComConfig (comPort, "",921600, 0, 8, 1, 512, 512); //

	return(i);
}

int MiniSys_write (char write_char[])
{
int bytesWritten, count, localError=0;
char rxBuffer[0x200];

//	FlushOutQ (MiniSysPort); 
	if(gDebugMiniSys)
	   printf(write_char);
	count = strlen(write_char);
	FlushInQ (MiniSysPort);	
    bytesWritten = ComWrt (MiniSysPort, write_char, count);
//    bytesWritten += ComWrtByte (MiniSysPort,0x0a);
    bytesWritten += ComWrtByte (MiniSysPort,0x0d);
// check mirrored data:	
	count = ComRd (MiniSysPort,rxBuffer,bytesWritten);
    if(count != bytesWritten) localError++;
    return(localError);
    
}



int MiniSys_read (char read[])
{
int bytesRead;
    Sleep(50); // USB needs time for answer ;(
    bytesRead = GetInQLen (MiniSysPort);
//    ComRdTerm (MiniSysPort, read, 10, '\r');
    ComRd (MiniSysPort, read, bytesRead);
    read[bytesRead] = 0; //terminate string
	if(gDebugMiniSys)
	   printf((char*)read);
	return(bytesRead);
}


int MiniSys_getLightSensor (void)
{

   MiniSys_write ("RGB");
   if(!MiniSys_read (ResultStr))
   {
      MiniSys_write ("RGB");
	  if(!MiniSys_read (ResultStr))
   		return(-1);
   	}
	return(atoi(ResultStr));
}

int MiniSys_get_SID (char* systemID)
{
int i;
char *p;
//   MiniSys_write ("SID?");
   MiniSys_write ("EEPR:1");
   if(!MiniSys_read (ResultStr))
   		return(-1);
	if(gDebugMiniSys)
	   printf(ResultStr);
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


int MiniSys_get_AdapterID (char* adapterID)  //Achtung:hexadecimal. high-byte für porta (links), lowbyte für portb (rechts)
{
int i;

   MiniSys_write ("EEPR:2");
   if(!MiniSys_read (ResultStr))
   		return(-1);
	sscanf(ResultStr,"%x", &i);
	return(i);
}


int MiniSys_get_IN(void)  
{
int i;
	ProcessSystemEvents();
	MiniSys_write ("IN?");
   	if(!MiniSys_read (ResultStr))
   		return(-1);
   	sscanf(ResultStr,"%x", &i);
	return(i);
}


int MiniSys_set_OC(int number, int on)  
{
char tempStr[0x20];
	on &= 0x01;
	if((number>8) || (number<1)) return(-1);
	sprintf(tempStr,"OC:%d:%d", number, on);
	MiniSys_write (tempStr);
	return(0);
}

//ADC-section:
int MiniSys_ADC_RaSe(int DcAc, int range)  
//RaSe:[DC|AC]:[DC:2,20,200; AC: 1,10,100] >> set the Range
{
char tempStr[0x20];

	if(!DcAc) 
	{
		if((range ==2)||(range ==20)||(range ==200)) 
			sprintf(tempStr,"RaSe:DC:%d", range);
		else return(-1);
	}
	else   
	{
		if((range ==1)||(range ==10)||(range ==100))
			sprintf(tempStr,"RaSe:AC:%d", range);
		else  return(-2);
	}
	MiniSys_write (tempStr);
	return(0);
}


int MiniSys_ADC_InpS(int gnd, int channel)  
//InpS:[GND:1..8]:[Channel:1..16] >> set the selcted inputs
{
char tempStr[0x20];

	if((gnd<1)||(gnd>8)) return(-1);
	if((channel<1)||(channel > 16)) return(-2);
	sprintf(tempStr,"InpS:%d:%d", gnd, channel);
	MiniSys_write (tempStr);
	return(0);
}

int MiniSys_ADC_getVdc(double *result)  
{

	MiniSys_write ("Vdc?");
   	if(!MiniSys_read (ResultStr))
   		return(-1);
   	*result = atof(&ResultStr[4]); //remove the ':'
	return(0);
}

int MiniSys_ADC_getVac(double *result)  
{

	MiniSys_write ("Vac?");
   	if(!MiniSys_read (ResultStr))
   		return(-1);
   	*result = atof(&ResultStr[4]); //remove the ':'
	return(0);
}


void MiniSys_close (void)
{
   CloseCom (MiniSysPort); 
}

