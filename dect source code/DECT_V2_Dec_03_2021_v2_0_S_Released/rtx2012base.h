#include <cvidef.h>

















int __stdcall rtx2012_Write (char *command);	   
int __stdcall rtx2012_Query (char *command, char *output)     ;
int __stdcall rtx2012_Init (char *output);    
int __stdcall rtx2012_Loopback_Mode_EU_Setting (void);   
int __stdcall rtx2012_Nonsignal_Connect (void); 

int __stdcall rtx2012_Nosignal_Disconnect (void);
int __stdcall rtx2012_Reset (void) ;

int __stdcall rtx2012_Read_NTP (char *power);   

int __stdcall rtx2012_Read_FrequencyOffset (char *freqoffset);  

int __stdcall rtx2012_Read_Deviation (char *BField);

int __stdcall rtx2012_Read_FrequencyDriftSlot (char *freqDriftSlot);  
int __stdcall rtx2012_Read_ReceiverSensitivity (char *BERFER,double fLev); 
int __stdcall rtx2012_Read_TemplateMatch (char *matching);    

