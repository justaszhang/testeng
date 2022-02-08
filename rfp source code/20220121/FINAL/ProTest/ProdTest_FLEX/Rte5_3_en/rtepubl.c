
#include <utility.h>
#include <ansi_c.h>
#include <userint.h>
#include "rtepubl.h"

void add_days_to_dateStr(char *dateStr,int days)
{
	
	time_t ltime;
	
	struct tm lt;	
	
	// mit default fuellen
	lt.tm_hour=0;
	lt.tm_min=0;
	lt.tm_sec=0;
	lt.tm_mday= 1;     
    lt.tm_mon = 1 - 1;
	lt.tm_year= 2010 - 1900;

	//(void)time(&ltime);
	//ut=localtime(&ltime); 
	
	
	// 0123456789
    // DD-MM-YYYY
	sscanf(dateStr,"%2d-%2d-%4d", &lt.tm_mday, &lt.tm_mon, &lt.tm_year);
	lt.tm_mday= lt.tm_mday;     
    lt.tm_mon = lt.tm_mon - 1;
	lt.tm_year= lt.tm_year - 1900;
	
	// aus struct tm  ltime (sekunden seit 1900) erzeugen 
	ltime = mktime( &lt );
	
	ltime = ltime + days*(24*60*60);   // Tage dazu (in Sekunden)
	
	lt = * localtime(&ltime);
	

	// zurueck schreiben
	sprintf(dateStr,"%02d-%02d-%04d",lt.tm_mday, lt.tm_mon+1, lt.tm_year+1900) ;
	
	return;		
	
}	
	
int days_between_now_and_dateStr(char *dateStr)
{

	int    days;
	time_t ltime_last;
	time_t ltime_now;
	
	struct tm lt;	
	
	// mit default fuellen
	lt.tm_hour=0;
	lt.tm_min=0;
	lt.tm_sec=0;
	lt.tm_mday= 1;     
    lt.tm_mon = 1 - 1;
	lt.tm_year= 2010 - 1900;


	
	// 0123456789
    // DD-MM-YYYY
	sscanf(dateStr,"%2d-%2d-%4d", &lt.tm_mday, &lt.tm_mon, &lt.tm_year);
	lt.tm_mday= lt.tm_mday;     
    lt.tm_mon = lt.tm_mon - 1;
	lt.tm_year= lt.tm_year - 1900;

	// aus struct tm  ltime (sekunden seit 1900) erzeugen 
	ltime_last = mktime( &lt );

	
	// aktuelles Datum in Sekunden seit 1900
	(void)time(&ltime_now);

	
	days = (int) (difftime(ltime_now, ltime_last) / (24*60*60));

	return(days);		
	
}	
	



int adapter_counter_clear(struct rte_globals *globals, int intervalnr)
{

	// new @GC 21.03.2012
	// Der DeviceCounter wird geloescht und
	// das Service Datum wird auf das aktuelle Datum gesetzt


	FILE *fp;
	int  i;
	int  devcnt=0;
	char *pDate=NULL;
	char *pLine=NULL;
	
	
	char cBuffer[2048];
	char cEntry[128];
	char cTemp[20];

	
	// default wenn noch kein File vorhanden
	strcpy(cBuffer,"interval_0_days=90\ninterval_0_lastdate=01-01-2012\ninterval_0_maxcnt=5000\ninterval_0_text=Adapter Wartung!!\ninterval_0_devcnt=0\nglobal_devcnt=0\n");
	

	// komplett einlesen
	if ( (fp=fopen( globals->fixtureServiceFile ,"r")) != NULL )
	{
		i=fread( cBuffer, 1, sizeof(cBuffer)-1, fp );
		cBuffer[i]='\0';
		fclose(fp);
	}


	// und zurueckschreiben	
	if ( (fp=fopen( globals->fixtureServiceFile ,"w")) != NULL )
	{

		pLine = cBuffer;	
		while ( (pLine=strtok( pLine , "\n" )) != NULL )
		{
			
			// interval_1_devcnt = 1000
			sprintf(cEntry,"interval_%d_devcnt",intervalnr);
			if ( strncmp(pLine, cEntry, strlen(cEntry) ) == 0 )
			{

				devcnt=0;
				fprintf(fp,"%s=%d\n", cEntry,devcnt);
				pLine=NULL;
				continue;      // next Line
			}
			
			// interval_1_lastdate = DD-MM-YYYY
			sprintf(cEntry,"interval_%d_lastdate",intervalnr);
			if ( strncmp(pLine, cEntry, strlen(cEntry) ) == 0 )
			{
				// MM-DD-YYYY  --> nach DD-MM-YYYY
				pDate = DateStr();
				cTemp[0]=pDate[3];
				cTemp[1]=pDate[4];
				cTemp[2]=pDate[2];
				cTemp[3]=pDate[0];
				cTemp[4]=pDate[1];
				cTemp[5]=pDate[5];
				cTemp[6]=pDate[6];
				cTemp[7]=pDate[7];
				cTemp[8]=pDate[8];
				cTemp[9]=pDate[9];
				cTemp[10]='\0';
				fprintf(fp,"%s=%s\n", cEntry, cTemp);
				pLine=NULL;
				continue;      // next Line
			}
			
			fprintf(fp,"%s\n", pLine);
			pLine=NULL;      // next Line
		}		 
		
		fclose(fp);
		return(0);  //ok
		
	}	
	return(1); // fail
}



int adapter_counter_update(struct rte_globals *globals, int count)
{

	// new @GC 21.03.2012
	// adapter service counter wird um anzahl count erhoeht.


	FILE *fp;
	int  i;
	int  devcnt=0;
	char *p=NULL;
	char *pLine=NULL;
	
	
	char cBuffer[2048];
	char cEntry[128];

	return 0;


}



int adapter_counter_read(struct rte_globals *globals, int intervalnr, int *serviceCount, int *devCount, int *serviceDays, char *lastServiceDate, char *nextServiceDate, char *serviceText)
{


	// new @GC 21.03.2012
	// adapter service counter und datum werden ausgelesen, und zurueck gegeben


	FILE *fp;
	int  i;
	int  rc=0;       
	int  days;
	char *p = NULL;
	char *pLine;
	
	char cBuffer[2048];
	char cTemp[128];
	char cEntry[128];
	
	strcpy(cBuffer,"");
	
	*serviceCount=0;          // wann muss der naechste Service gemacht werden (anzahl DUT)
	*devCount=0;              // aktueller Device Count momentan 
	*serviceDays=0;			  // wann muss der naechste Service gemacht werden (anzahl Tage)
	// DD-MM-YYYY
	strcpy(lastServiceDate, "01-01-2012" );  // Datum des letzten Service
	strcpy(nextServiceDate, "01-01-2012" );  // Datum des naechsten Service
	
	if (serviceText != NULL ) strcpy(serviceText, "Wartung!" );                // Text 
	

	
	if ( (fp=fopen( globals->fixtureServiceFile ,"r")) != NULL )
	{
		i=fread( cBuffer, 1, sizeof(cBuffer)-1, fp );
		cBuffer[i]='\0';
		fclose(fp);
	}
	else
	{
		return(-3);    // file missing
	}
	
	
	
	
	
	pLine = cBuffer;	
	while ( (pLine=strtok( pLine , "\n" )) != NULL )
	{
		// interval_1_maxcnt = 1000
		sprintf(cEntry,"interval_%d_maxcnt",intervalnr);
		if ( strncmp(pLine, cEntry, strlen(cEntry) ) == 0 )
		{
			p=pLine+strlen(cEntry);
			while ( *p == '=' || *p == ' ' ) p++;
			*serviceCount=atoi( p );
			rc++;
		}	

		// interval_1_devcnt = 1000
		sprintf(cEntry,"interval_%d_devcnt",intervalnr);
		if ( strncmp(pLine, cEntry, strlen(cEntry) ) == 0 )
		{
			p=pLine+strlen(cEntry);
			while ( *p == '=' || *p == ' ' ) p++;
			*devCount=atoi( p );
			rc++;
		}

		// interval_1_days = 90
		sprintf(cEntry,"interval_%d_days",intervalnr);
		if ( strncmp(pLine, cEntry, strlen(cEntry) ) == 0 )
		{
			p=pLine+strlen(cEntry);
			while ( *p == '=' || *p == ' ' ) p++;
			*serviceDays=atoi( p );
			rc++;
		}	

		// interval_1_lastdate = DD-MM-YYYY
		sprintf(cEntry,"interval_%d_lastdate",intervalnr);
		if ( strncmp(pLine, cEntry, strlen(cEntry) ) == 0 )
		{
			p=pLine+strlen(cEntry);
			while ( *p == '=' || *p == ' ' ) p++;
		
			//DD-MM-YYYY
			for ( i=0 ; *p != '\0' && *p != '\r' && *p != '\n' && i < sizeof(cTemp) ; p++,i++ )
			{
				cTemp[i]=*p;
			}	
			cTemp[i]='\0';
		
			// DD-MM-YYYY
			if ( strlen(cTemp) == 10 )
			{
				// DD-MM-YYYY
				strcpy(lastServiceDate, cTemp );
				rc++;
			}	
		}	

		// interval_1_text = Wartung!! Service anrufen
		sprintf(cEntry,"interval_%d_text",intervalnr);
		if ( strncmp(pLine, cEntry, strlen(cEntry) ) == 0 )
		{
			p=pLine+strlen(cEntry);
			while ( *p == '=' || *p == ' ' ) p++;
		
			// Wartung!! Service anrufen
			for ( i=0 ; *p != '\0' && *p != '\r' && *p != '\n' && i < (sizeof(cTemp)-1) ; p++,i++ )
			{
				cTemp[i]=*p;
			}	
			cTemp[i]='\0';
		
			if (serviceText != NULL ) strcpy(serviceText, cTemp );                // Text 
			rc++;
		}	

			
		pLine=NULL;      // next Line
	}		 

	
	if ( rc == 0 )      // Intervall nicht vorhanden ?
	    return(-1);     // Interval not used
	
	if ( rc != 5 )       // Intervall hat keine 5 Eintraege gelesen?
	    return(-2);     // fail	


	
	// DD-MM-YYYY
	days = days_between_now_and_dateStr(lastServiceDate);	 // abgelaufene Tage

	
	// DD-MM-YYYY
	strcpy(nextServiceDate,lastServiceDate);
	add_days_to_dateStr(nextServiceDate,*serviceDays);
	

	
	
	
	if ( *devCount > *serviceCount ) 
		return(1);      // anzahl DUT ist groesse service Intervall
		
	if ( days > *serviceDays ) 
		return(2);      // anzahl Tage ist groesse service Intervall

	
	return(0);	   // ok kein service noetig
}


