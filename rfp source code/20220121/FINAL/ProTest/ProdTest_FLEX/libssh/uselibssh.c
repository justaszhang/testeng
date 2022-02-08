#ifndef _USELIBSSH_C
#define _USELIBSSH_C
#endif
//==============================================================================
//
// Title:       uselibssh.c
// Purpose:     
//
// Created on:  13.01.2013 by GC
// Modified:    20.01.2016 by GC  use libssh 0.7.2
//
// Copyright:   . All Rights Reserved.
//
//==============================================================================

// Achtung: uselibssh.c ssh.lib muss auch in rte.prj eingebunden werden.

//==============================================================================
// Include files
#include <windows.h>
#include <winsock.h>
#include <userint.h>
#include <utility.h>
#include <ansi_c.h>


#ifndef _STDINT_H_
typedef unsigned long uint32_t;		          // wird in stdint.h definiert, cvi 6.0 hat kein stdint.h
//typedef unsigned long long uint64_t; 		  // cvi 6.0 hat kein 64Bit
//typedef unsigned __int64 uint64_t; 
//typedef unsigned long uint64_t;
typedef struct {
    unsigned long n1;
    unsigned long n2;
} uint64_t;
#endif


typedef	int	mode_t;	

#include <libssh/libssh.h> 
#include <stdlib.h>
#include <stdio.h> 
#include <errno.h>
#include <string.h>
#include <lowlvlio.h>

#include "uselibssh.h"



//==============================================================================
// Global variables
int uselibssh_debug = 0;		// 0=no output, 1=status output 2=more status output
									// on stderr or on MessagePupup

//==============================================================================
// Static global variables
static ssh_session gl_session = NULL;
static ssh_channel gl_channel = NULL;



//==============================================================================
// Constants
#define	ssh_free(x)  if (x !=NULL) ssh_free(x),x=NULL

#define ssh_channel_free(x)  if (x !=NULL) ssh_channel_free(x),x=NULL



// new Check after read or write ssh @GC 14.05.2014
#define CHECK_SSH_ERR_ON_END 






//==============================================================================
// Global functions



/*==============================================================================
**
**  openSSH(char *host, char *user, char *password, char *errResp)   
**
**  return:
**	=  0  == ok
**  <> 0  == fail
**
*/
int openSSH(char *host, char *user, char *password, char *errResp)
{
	int rc;
	int method;
	
  	int port = 22;

  	long timeSec =  6;
  	long timeUSec = 0;
	
  	int verbosity = SSH_LOG_NOLOG;
	
#ifdef _CONSOLE	
	if ( uselibssh_debug & 2 )	verbosity = SSH_LOG_PROTOCOL;
	if ( uselibssh_debug & 4 )	verbosity = SSH_LOG_PACKET;
	if ( uselibssh_debug & 8 )	verbosity = SSH_LOG_FUNCTIONS;

#endif
	
	if ( errResp != NULL ) strcpy(errResp,"");	
	
	
	if ( ssh_is_connected(gl_session) )
	{
		ssh_disconnect(gl_session);
		ssh_free(gl_session);
	}	
	
  	gl_session = ssh_new();
	
  	if (gl_session == NULL)
	{	
		if ( errResp != NULL ) sprintf(errResp, "openSSH(): can't create a new session" );	

		return(1);
	}	

	

	
  	ssh_options_set(gl_session, SSH_OPTIONS_HOST, host );   // host or ipadr
  	ssh_options_set(gl_session, SSH_OPTIONS_LOG_VERBOSITY, &verbosity);
  	ssh_options_set(gl_session, SSH_OPTIONS_PORT, &port);
  	ssh_options_set(gl_session, SSH_OPTIONS_USER, user);
  	ssh_options_set(gl_session, SSH_OPTIONS_TIMEOUT, &timeSec );		// timeout in Sec
  	ssh_options_set(gl_session, SSH_OPTIONS_TIMEOUT_USEC, &timeUSec);   // timeout in us

	
	rc = ssh_connect(gl_session);
  	if (rc != SSH_OK)
  	{
		if ( errResp != NULL ) sprintf(errResp, "openSSH(): can't connect to localhost: %s", ssh_get_error(gl_session) );	
		
		ssh_free(gl_session);
    	return(2);
  	}


	// Verify the server's identity
	// For the source code of verify_knowhost(), check previous example
  	if (verify_knownhost(gl_session,errResp) < 0)
  	{
    	ssh_disconnect(gl_session);
    	ssh_free(gl_session);
    	return(3);
  	}



	rc=SSH_AUTH_ERROR;
	// Try to authenticate
    rc=ssh_userauth_none(gl_session, NULL);
#ifdef _CONSOLE	
	if ( uselibssh_debug & 2 ) fprintf(stderr, "USERAUTH_NONE rc=%d\n",rc);
#endif		
	
	
	// Authenticate ourselves
  	if ( rc != SSH_AUTH_SUCCESS )
	{
		// This requires the function ssh_userauth_none() to be called before the methods are available.
		// The server MAY return a list of methods that may continue
		method = ssh_userauth_list(gl_session, NULL);
#ifdef _CONSOLE	
		if ( uselibssh_debug & 2 ) fprintf(stderr, "METHOD=0x%02x\n",method);
#endif		
	}	
	
  	if ( rc != SSH_AUTH_SUCCESS && (method == SSH_AUTH_METHOD_UNKNOWN) )
  	{ // ?????	
#ifdef _CONSOLE	
	    if ( uselibssh_debug & 2 ) fprintf(stderr, "METHOD_UNKNOWN\n");
#endif		
  	}
	
	// Try to authenticate
  	if ( rc != SSH_AUTH_SUCCESS && (method & SSH_AUTH_METHOD_NONE) )
  	{ // For the source code of function authenticate_none(),
      // refer to the corresponding example
#ifdef _CONSOLE	
	    if ( uselibssh_debug & 2 ) fprintf(stderr, "METHOD_NONE\n");
#endif		
		rc = ssh_userauth_none(gl_session, NULL);
  	}

	// Try to authenticate with keyboard interactive
  	if ( rc != SSH_AUTH_SUCCESS && (method & SSH_AUTH_METHOD_INTERACTIVE) )
  	{ // For the source code of function authenticate_kbdint(),
      // refer to the corresponding example
#ifdef _CONSOLE	
	    if ( uselibssh_debug & 2 ) fprintf(stderr, "METHOD_INTERACTIVE\n");
#endif
    	rc = authenticate_kbdint(gl_session, user , password);
    }
	
	// Try to authenticate with password
  	if ( rc != SSH_AUTH_SUCCESS && (method & SSH_AUTH_METHOD_PASSWORD) )
  	{ // For the source code of function authenticate_password(),
      // refer to the corresponding example
#ifdef _CONSOLE	
	    if ( uselibssh_debug & 2 ) fprintf(stderr, "METHOD_PASSWORD\n");
#endif
		rc = ssh_userauth_password(gl_session, user, password);
  	}
	
	// Try to authenticate with public key
	if ( rc != SSH_AUTH_SUCCESS && (method & SSH_AUTH_METHOD_PUBLICKEY) )
  	{ // For the source code of function authenticate_pubkey(),
      // refer to the corresponding example
#ifdef _CONSOLE	
	    if ( uselibssh_debug & 2 ) fprintf(stderr, "METHOD_PUBLICKEY\n");
#endif
        rc = ssh_userauth_autopubkey(gl_session, NULL);
  	}

	if ( rc != SSH_AUTH_SUCCESS && (method & SSH_AUTH_METHOD_HOSTBASED) )
  	{ // ?????  
#ifdef _CONSOLE	
	    if ( uselibssh_debug & 2 ) fprintf(stderr, "METHOD_HOSTBASED\n");
#endif
  	}
	
#if LIBSSH_VERSION_MINOR >= 7
  	if ( rc != SSH_AUTH_SUCCESS && (method & SSH_AUTH_METHOD_GSSAPI_MIC) )
  	{
#ifdef _CONSOLE	
	    if ( uselibssh_debug & 2 ) fprintf(stderr, "METHOD_GSSAPI_MIC\n");
#endif
		rc = ssh_userauth_gssapi(gl_session);
		
  	}
#endif
	
	
  	if (rc != SSH_AUTH_SUCCESS)
  	{
		if ( errResp != NULL ) sprintf(errResp, "openSSH(): authenticating fails: %s", ssh_get_error(gl_session) );	

    	ssh_disconnect(gl_session);
    	ssh_free(gl_session);
    	return(4);
	}

	
	
	// Opening a remote shell

	gl_channel = ssh_channel_new(gl_session);
  	if (gl_channel == NULL)
	{
		if ( errResp != NULL ) sprintf(errResp, "openSSH(): can't create new channel: %s", ssh_get_error(gl_session) );	

		ssh_disconnect(gl_session);
		ssh_free(gl_session);
    	return(5);
	}	

  	rc = ssh_channel_open_session(gl_channel);
  	if (rc != SSH_OK)
  	{
		if ( errResp != NULL ) sprintf(errResp, "openSSH(): can't open channel: %s", ssh_get_error(gl_session) );

    	ssh_channel_free(gl_channel);
		ssh_disconnect(gl_session);
		ssh_free(gl_session);
    	return(6);
  	}


	// Interactive and non-interactive sessions
	
	rc = interactive_shell_session(gl_channel);
  	if (rc != SSH_OK)
  	{
		if ( errResp != NULL ) sprintf(errResp, "openSSH(): can't create interactive shell: %s", ssh_get_error(gl_session) );

    	ssh_channel_free(gl_channel);
		ssh_disconnect(gl_session);
		ssh_free(gl_session);
    	return(7);
  	}

	
	
   	//if ( uselibssh_debug & 2 ) fprintf(stderr, "X show_remote_proc\n");
	//rc=show_remote_processes( gl_session );
    //rc=interactive_shell_session(gl_session);
	

	return(rc);

}

/*==============================================================================
**
**  closeSSH()   
**
**  return:
**	=  0  == ok
**  <> 0  == fail
**
*/
int closeSSH(void)
{
	
	
	if ( ssh_is_connected(gl_session) )
	{	

		if (gl_channel != NULL)
		{	
			ssh_channel_close(gl_channel);
  			ssh_channel_send_eof(gl_channel);
		}	
  		ssh_channel_free(gl_channel);

		ssh_disconnect(gl_session);
	}	
  	ssh_free(gl_session);
	return(0);
}  


/*==============================================================================
**
**  int writeSSH(char *writeBuffer, int nwrite, char *errResp )
**
**  return:
**	=  0  == ok
**  <> 0  == fail
**
*/
int writeSSH(char *writeBuffer, int nwrite, char *errResp )
{
	
  	int  nbytes;
	
	
	if ( errResp != NULL ) strcpy(errResp,"");	

	// is connected?
	if ( !ssh_is_connected(gl_session) )
	{	
		if ( errResp != NULL ) strcpy(errResp, "writeSSH(): ssh session isn't connected" );	
		
		return(1);
	}	
	
	
	// is channel open?
	if ( !ssh_channel_is_open(gl_channel) )
	{	
		if ( errResp != NULL ) strcpy(errResp, "writeSSH(): ssh channel isn't open" );	
		
		return(2);
	}	

	// is channel EOF ?
    if ( ssh_channel_is_eof(gl_channel) )
	{
		if ( errResp != NULL ) strcpy(errResp, "writeSSH(): ssh channel is EOF" );	
		
		return(3);
	}	
	
    if (nwrite > 0)
    {
    	nbytes = ssh_channel_write(gl_channel, writeBuffer, nwrite);
		
#ifdef _CONSOLE	
	    if ( uselibssh_debug & 1 ) 
		{	
			int i;
			fprintf(stderr,"\nWRITE[");
			//fwrite(writeBuffer,1,nbytes,stderr);
			for(i=0 ; i < nbytes ; i++ )
			{
				if ( *(writeBuffer+i) >= ' ' && *(writeBuffer+i) <= '~' )
					fwrite(writeBuffer+i,1,1,stderr);
				else
					fprintf(stderr," 0x%02x", (int)((unsigned char)*(writeBuffer+i)) );
						
				}	
			fprintf(stderr,"]\n");
		}	
#endif		

      	if (nwrite != nbytes) 
		{	
			if ( errResp != NULL ) strcpy(errResp, "writeSSH(): ssh can't write to channel" );	
		
			return(4);
		}	
		
	
// new @GC 14.05.2014
#ifdef CHECK_SSH_ERR_ON_END		
			// is connected?
		if ( !ssh_is_connected(gl_session) )
		{	
			if ( errResp != NULL ) strcpy(errResp, "writeSSH(): ssh session isn't connected, after write" );	
		
			return(5);
		}	
	
	
		// is channel open?
		if ( !ssh_channel_is_open(gl_channel) )
		{	
			if ( errResp != NULL ) strcpy(errResp, "writeSSH(): ssh channel isn't open, after write" );	
		
			return(6);
		}	

		// is channel EOF ?
    	if ( ssh_channel_is_eof(gl_channel) )
		{
			if ( errResp != NULL ) strcpy(errResp, "writeSSH(): ssh channel is EOF, after write" );	
		
			return(7);
		}	
#endif

		
		
    }

	return(0);		// ok
}	
	




/*==============================================================================
**
**  int readSSH(char *readBuffer, int maxread, int *nread, char *wildString, double timeoutSec , char *errResp ) 
**
**  return:
**	=  0  == ok
**  <> 0  == fail
**
*/
int readSSH(char *readBuffer, int maxread, int *nread, char *wildString, double timeoutSec , char *errResp )
{
	
	char buffer[256];
  	int  nbytes;
  	int  rbytes;
	char *pFound=NULL;
	
	time_t tStart;
	time_t t;
	

	*nread=0;
	
	if ( errResp != NULL ) strcpy(errResp,"");	

	// is connected?
	if ( !ssh_is_connected(gl_session) )
	{	
		if ( errResp != NULL ) strcpy(errResp, "readSSH(): ssh session isn't connected" );	
		
		return(1);
	}	
	
	
	// is channel open?
	if ( !ssh_channel_is_open(gl_channel) )
	{	
		if ( errResp != NULL ) strcpy(errResp, "readSSH(): ssh channel isn't open" );	
		
		return(2);
	}	

	// is channel EOF ?
    if ( ssh_channel_is_eof(gl_channel) )
	{
		if ( errResp != NULL ) strcpy(errResp, "readSSH(): ssh channel is EOF" );	
		
		return(3);
	}	
	
	

    if (maxread > 0 && timeoutSec > 0 )
	{
		time(&tStart);                                // aktuelle Time

		rbytes=0; 
		*nread=rbytes;
		readBuffer[rbytes]='\0';
		
		
		while (ssh_channel_is_open(gl_channel) && !ssh_channel_is_eof(gl_channel))
  		{
    		nbytes = ssh_channel_read_nonblocking(gl_channel, buffer, sizeof(buffer), 0);
    		if (nbytes < 0) 
			{
				//if ( errResp != NULL ) strcpy(errResp, "readSSH(): ssh can't read from channel" );	
				if ( errResp != NULL )
				{
					void *error;
					const char *p;
					error = gl_session;
					p = ssh_get_error(error);
					sprintf(errResp, "readSSH(%d): ssh can't read from channel: %s", nbytes , p );	
				}	
				
		
				return(4);
			}	
    		if (nbytes > 0)
    		{
				if ( (rbytes+nbytes) < maxread )
				{	
					memcpy( &readBuffer[rbytes] ,buffer,nbytes);
					rbytes=rbytes+nbytes;
					*nread=rbytes;
					readBuffer[rbytes]='\0';
				}

#ifdef _CONSOLE				
			    if ( uselibssh_debug & 1 )
				{	
					int i;
					fprintf(stderr,"\nREAD[");
					for(i=0 ; i < nbytes ; i++ )
					{
						if ( *(buffer+i) >= ' ' && *(buffer+i) <= '~' )
							fwrite(buffer+i,1,1,stderr);
						else
							fprintf(stderr," 0x%02x", (int)((unsigned char)*(buffer+i)) );
							
					}	
					fprintf(stderr,"]\n");
				}	
#endif				
			}
			
			if ( or_wildcmp( wildString, readBuffer) )
			{   // wild compare found
				pFound = readBuffer;
				break;
			}	

			/*
			if ( (pFound = strstr(readBuffer,searchText)) != NULL)
				break;
			*/	
			
			time(&t);                                      // aktuelle Time
			if ( timeoutSec < difftime(t, tStart) )        
				break;
			
			Sleep(200);   //200ms  
			
		}
		
	
// new @GC 14.05.2014
#ifdef CHECK_SSH_ERR_ON_END		
		// is connected?
		if ( !ssh_is_connected(gl_session) )
		{	
			if ( errResp != NULL ) strcpy(errResp, "readSSH(): ssh session isn't connected, after read" );	
		
			return(6);
		}	
	
	
		// is channel open?
		if ( !ssh_channel_is_open(gl_channel) )
		{	
			if ( errResp != NULL ) strcpy(errResp, "readSSH(): ssh channel isn't open, after read" );	
		
			return(7);
		}	

		// is channel EOF ?
    	if ( ssh_channel_is_eof(gl_channel) )
		{
			if ( errResp != NULL ) strcpy(errResp, "readSSH(): ssh channel is EOF, after read" );	
		
			return(8);
		}	

#endif
		
		if ( pFound == NULL )
		{	
			if ( errResp != NULL ) strcpy(errResp, "readSSH(): ssh timeout text not found");	
		
			return(5);		// fail text not found
		}
			
		
	}
	return(0);		// ok
}	
	


	
	




//==============================================================================
// Local functions

int or_wildcmp(const char *wild, const char *string)
{
	int  rc;
	char *pWild;	
	char stringToSeparate[512];
	char cmpWild[512];
		
	sprintf(stringToSeparate,"%s",wild);

	pWild = strtok( stringToSeparate , "|" );	
    if ( pWild != NULL ) 
	{
		do
		{
			strcpy(cmpWild,"*");
			strcat(cmpWild,pWild);
			strcat(cmpWild,"*");
			if ( (rc=wildcmp(cmpWild,string)) ) return(rc);   // found
		}	
		while ( (pWild=strtok( NULL , "|" )) != NULL );
	}	
	
	return(0);   // not found		
}

/*==============================================================================
**
** int wildcmp(const char *wild, const char *string)
**
**
** This is a fast, lightweight, and simple pattern matching function.
**
** Matches a string against a wildcard string such as "*.*" or "bl?h.*" etc.
** This is good for file globbing or to match hostmasks.
**
*/
int wildcmp(const char *wild, const char *string)
{
	// Written by Jack Handy - jakkhandy@hotmail.com
	const char *cp = NULL, *mp = NULL;
	
	while ((*string) && (*wild != '*'))
	{
    	if ((*wild != *string) && (*wild != '?'))
		{
			return 0;
    	}
    	wild++;
    	string++;
	}

  	while (*string)
	{
    	if (*wild == '*')
		{
      		if (!*++wild)
			{
        		return 1;
      		}
			mp = wild;
			cp = string+1;
		}
		else if ((*wild == *string) || (*wild == '?'))
		{
			wild++;
			string++;
	    }
		else
		{
      		wild = mp;
      		string = cp++;
    	}
  	}

  	while (*wild == '*')
	{
    	wild++;
  	}
	return !*wild;
	
}



/*==============================================================================
**
**	int interactive_shell_session(ssh_channel channel)
**
**
** Interactive and non-interactive sessions
** you need to create a pseud-terminal on the remote side.
** A remote terminal is usually referred to as a "pty", for "pseudo-teletype".
**
*/
int interactive_shell_session(ssh_channel channel)
{
  int rc;

  rc = ssh_channel_request_pty(channel);
  if (rc != SSH_OK) return rc;

  rc = ssh_channel_change_pty_size(channel, 80, 24);
  if (rc != SSH_OK) return rc;

  rc = ssh_channel_request_shell(channel);
  if (rc != SSH_OK) return rc;

  
  return rc;
}


/*==============================================================================
**
**	int verify_knownhost(ssh_session session)
**
**
** Authenticating the server
** Once you're connected, the following step is mandatory:
** you must check that the server you just connected to is known and safe to use
** (remember, SSH is about security and authentication).
**
**
** hier ersteinmal alle Hosts zulaessig
**
**
*/
int verify_knownhost(ssh_session session, char *errResp)
{
	int state, hlen;
	unsigned char *hash = NULL;
	char *hexa;
	char buf[10];

	
	if ( errResp != NULL ) strcpy(errResp, "" );	

	
  	state = ssh_is_server_known(session);

  	hlen = ssh_get_pubkey_hash(session, &hash);
  	if (hlen < 0)
  	{	  
		if ( errResp != NULL ) sprintf(errResp, "Error hash-key: can't create hash: no memory" );	
    	return(-1);
	}	

	if ( uselibssh_debug & 2 ) 
  	{	  
  		hexa = ssh_get_hexa(hash, hlen);
#ifdef _CONSOLE	
  		fprintf(stderr, "Public key hash: %s\n", hexa);
#else
  		MessagePopup("Public key hash", hexa);
#endif	

#if LIBSSH_VERSION_MINOR >= 7
		//neu bei libssh 0.7.2
		ssh_string_free_char(hexa);
#else		
    	free(hexa);
#endif		
  	}
  
  
  	switch (state)
  	{
    case SSH_SERVER_KNOWN_OK:
      break; /* ok */

    case SSH_SERVER_KNOWN_CHANGED:
      break; /* not ok */

    case SSH_SERVER_FOUND_OTHER:
      break; /* not ok */

    case SSH_SERVER_FILE_NOT_FOUND:
      break; /* not ok */

    case SSH_SERVER_NOT_KNOWN:
      break; /* not ok */

    case SSH_SERVER_ERROR:
      if ( errResp != NULL ) sprintf(errResp, "Error hash-key: %s", ssh_get_error(session));
#if LIBSSH_VERSION_MINOR >= 7
	//neu bei libssh 0.7.2
	  if ( hash != NULL ) ssh_clean_pubkey_hash(&hash);
#else		
      if ( hash != NULL ) free(hash);
#endif		
      return -1;
  	}

  
  /*  
  switch (state)
  {
    case SSH_SERVER_KNOWN_OK:
      break; //

    case SSH_SERVER_KNOWN_CHANGED:
      fprintf(stderr, "Host key for server changed: it is now:\n");
      ssh_print_hexa("Public key hash", hash, hlen);
      fprintf(stderr, "For security reasons, connection will be stopped\n");
      free(hash);
      return -1;

    case SSH_SERVER_FOUND_OTHER:
      fprintf(stderr, "The host key for this server was not found but an other"
        "type of key exists.\n");
      fprintf(stderr, "An attacker might change the default server key to"
        "confuse your client into thinking the key does not exist\n");
      free(hash);
      return -1;

    case SSH_SERVER_FILE_NOT_FOUND:
      fprintf(stderr, "Could not find known host file.\n");
      fprintf(stderr, "If you accept the host key here, the file will be"
       "automatically created.\n");
      // fallback to SSH_SERVER_NOT_KNOWN behavior 

    case SSH_SERVER_NOT_KNOWN:
      hexa = ssh_get_hexa(hash, hlen);
      fprintf(stderr,"The server is unknown. Do you trust the host key?\n");
      fprintf(stderr, "Public key hash: %s\n", hexa);
      free(hexa);
      if (fgets(buf, sizeof(buf), stdin) == NULL)
      {
        free(hash);
        return -1;
      }
      if (strncmp(buf, "yes", 3) != 0)
      {
        free(hash);
        return -1;
      }
	  
      if (ssh_write_knownhost(session) < 0)
      {
        fprintf(stderr, "Error %s\n", strerror(errno));
        free(hash);
        return -1;
      }
      break;

    case SSH_SERVER_ERROR:
      fprintf(stderr, "Error %s", ssh_get_error(session));
      free(hash);
      return -1;
  }
  */
	
#if LIBSSH_VERSION_MINOR >= 7
	//neu bei libssh 0.7.2
	if ( hash != NULL ) ssh_clean_pubkey_hash(&hash);
#else		
    if ( hash != NULL ) free(hash);
#endif		

  
  
  return 0;
}




/*==============================================================================
**
** int authenticate_kbdint(ssh_session session, char *user , char *password)
**
** The keyboard-interactive authentication method
** The keyboard-interactive method is, as its name tells,
** interactive. The server will issue one or more challenges that the user has to answer, until the server takes an authentication decision.
**
*/
int authenticate_kbdint(ssh_session session, char *user , char *password)
{
  int rc;

  rc = ssh_userauth_kbdint(session, NULL, NULL);
  while (rc == SSH_AUTH_INFO)
  {
    const char *name, *instruction;
    int nprompts, iprompt;

    name = ssh_userauth_kbdint_getname(session);
    instruction = ssh_userauth_kbdint_getinstruction(session);
    nprompts = ssh_userauth_kbdint_getnprompts(session);

#ifdef _CONSOLE	
    if (strlen(name) > 0)
      if ( uselibssh_debug & 2 ) fprintf(stderr,"%s\n", name);
    if (strlen(instruction) > 0)
      if ( uselibssh_debug & 2 ) fprintf(stderr,"%s\n", instruction);
#endif	  
	  
    for (iprompt = 0; iprompt < nprompts; iprompt++)
    {
      const char *prompt;
      char echo;

      prompt = ssh_userauth_kbdint_getprompt(session, iprompt, &echo);
	  
	  if (echo)
      {
#ifdef _CONSOLE	  
	    if ( uselibssh_debug & 2 ) fprintf(stderr,"%s %s\n", prompt, user);
#endif		
		
        if (ssh_userauth_kbdint_setanswer(session, iprompt, user) < 0)
          return SSH_AUTH_ERROR;
      }
      else
      {
#ifdef _CONSOLE	  
	    if ( uselibssh_debug & 2 ) fprintf(stderr,"%s %s\n", prompt, password);
#endif		
		
        if (ssh_userauth_kbdint_setanswer(session, iprompt, password) < 0)
          return SSH_AUTH_ERROR;
      }

	  
	  /*
      if (echo)
      {
        char buffer[128], *ptr;

        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL)
          return SSH_AUTH_ERROR;
        buffer[sizeof(buffer) - 1] = '\0';
        if ((ptr = strchr(buffer, '\n')) != NULL)
          *ptr = '\0';
        if (ssh_userauth_kbdint_setanswer(session, iprompt, buffer) < 0)
          return SSH_AUTH_ERROR;
        memset(buffer, 0, strlen(buffer));
      }
      else
      {
        char *ptr;

        ptr = getpass(prompt);
        if (ssh_userauth_kbdint_setanswer(session, iprompt, ptr) < 0)
          return SSH_AUTH_ERROR;
      }
	  */
    }
	
	
    rc = ssh_userauth_kbdint(session, NULL, NULL);
  }
  return rc;
}










