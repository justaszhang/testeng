//==============================================================================
//
// Title:       uselibssh.h
// Purpose:     
//
// Created on:  13.01.2012 by G.Cordes
// Copyright:   . All Rights Reserved.
//
//==============================================================================

#ifndef _USELIBSSH_H
#define _USELIBSSH_H


int openSSH(char *host, char *user, char *password, char *err);
int closeSSH(void);

//int writeReadSSH(char *writeBuffer, char *readBuffer, int maxread, int *nread, char *searchText, double timeoutSec );

int writeSSH(char *writeBuffer, int nwrite, char *err);
int readSSH(char *readBuffer, int maxread, int *nread, char *searchText, double timeoutSec , char *err);




//==============================================================================
// only subfunction for uselibssh.c

#ifdef _USELIBSSH_C

int verify_knownhost(ssh_session session, char *err);
int authenticate_kbdint(ssh_session session,char *user, char *password);

int interactive_shell_session(ssh_channel channel);


int or_wildcmp(const char *wild, const char *string);
int wildcmp(const char *wild, const char *string);


#endif




#endif //_USELIBSSH_H
