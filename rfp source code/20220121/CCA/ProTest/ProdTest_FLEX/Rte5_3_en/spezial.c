#include <ansi_c.h>
//#include "spezial.h"

int RegistryReadKey(HKEY g_Key,LPTSTR lpValueName,LPTSTR szkey,char** value)
{

	 LONG	    Error;
	 DWORD      lpReserved=NULL;
	 DWORD	    lpType=NULL;
	 HKEY		phkey;
	 char*	    pBuffer=0;
	 DWORD		dwSize;
	
	 Error=RegOpenKeyEx(g_Key,szkey,0,KEY_ALL_ACCESS||KEY_QUERY_VALUE ,&phkey);
	 if(Error!=ERROR_SUCCESS)
		{
		return 0;
		}
	 Error=RegQueryValueEx(phkey, lpValueName, 0, &lpType, (BYTE*) pBuffer, &dwSize);
	 if(Error!=ERROR_SUCCESS)
		 return 0;
	 pBuffer = (char*)malloc(dwSize*sizeof(char));
	 dwSize = strlen(pBuffer);
     Error=RegQueryValueEx(phkey, lpValueName, 0, &lpType, (BYTE*) pBuffer, &dwSize);
	 if(Error!=ERROR_SUCCESS)
		 return 0;
	 Error=RegCloseKey(g_Key);
 	 if(Error!=ERROR_SUCCESS)
		 return 0;
	 *value=pBuffer;
	 return 1;

}
