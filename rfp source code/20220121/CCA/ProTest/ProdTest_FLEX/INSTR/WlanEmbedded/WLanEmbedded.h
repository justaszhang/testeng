#include <cvidef.h>

#ifndef WLANEMBEDDED_HEADER
#define WLANEMBEDDED_HEADER
#include <ansi_c.h>   


int WLanEmbedded_open (int comPort1,int comPort2, int comPort3);
int WLanEmbedded_write (int comPort, char write_char[]);
int WLanEmbedded_read (int comPort, char read[]);
void WLanEmbedded_close (void);
int WLanEmbedded_ListAP(int *ant1, int *ant2, int *ant3);
#endif



