#include <cvidef.h>

#ifndef MINISYSTEM_HEADER
#define MINISYSTEM_HEADER
#include <ansi_c.h>   

#define MINISYS_ADC_RANGE_DC	0
#define MINISYS_ADC_RANGE_AC	1

int MiniSys_open(int comPort);  
int MiniSys_write (char write_char[]);
int MiniSys_read (char read[]);  
int MiniSys_getLightSensor (void);
int MiniSys_get_SID (char* systemID);
int MiniSys_get_AdapterID (char* adapterID);
int MiniSys_get_IN(void); //read 8bit Input
int MiniSys_set_OC(int number, int on); //1..8:0|1
int MiniSys_ADC_RaSe(int DcAc, int range);
int MiniSys_ADC_InpS(int gnd, int channel);
int MiniSys_ADC_getVdc(double *result);
int MiniSys_ADC_getVac(double *result);
void MiniSys_close (void);      
#endif



