//==============================================================================
//
// Title:       useftpd.h
// Purpose:     
//
// Created on:  11.12.2011 by G.Cordes
// Copyright:   . All Rights Reserved.
//
//==============================================================================

#ifndef _USEFTPD_H
#define _USEFTPD_H

int startFtpDeamon(char *ipAddr,char *rootPath);
int checkFtpDeamon(char *ipAddr,int timeOut);   //timeOut in ms
int stopFtpDeamon(void);



#endif //_USEFTPD_H
