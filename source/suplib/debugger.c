

#include "debugger.h"

#ifndef _MITA_EXPORT_

#define _WIN32_WINNT    0x0500
#define _WIN32_IE       0x0500
#define CSIDL_MYMUSIC   0x000D
#define CSIDL_MYVIDEO   0x000E
#include <windows.h>
#include <shlobj.h>
#include <stdio.h>

//http://stackoverflow.com/questions/17933917/get-the-users-desktop-folder-using-windows-api
char* desktop_directory()
{
	static char path[MAX_PATH+1];
	if (SHGetSpecialFolderPathA(HWND_DESKTOP, path, CSIDL_DESKTOPDIRECTORY, FALSE))
		return path;
	else
		return "ERROR";
}

void DotxInternalDebug( const char *file, const char *function, const int line, const char *format, ... )
{
	char message[DIDMAXSIZE];
	va_list ap;
	static FILE * dotxfid = NULL;
	static istatus = 0;
	static char path[MAX_PATH+1];

	if (istatus == 0)
	{
		if ( dotxfid == NULL )
		{
			strcpy(path, desktop_directory());
			strcat(path,"\\dotxdebug.txt");
			dotxfid = fopen( path, "w+" );
		}
		if ( dotxfid != NULL )
		{
			fprintf(dotxfid, "file\tfunction\tline\tmessage\n");
			istatus = 1;
		}
	}
	if ( dotxfid == NULL )
	{
		istatus = 0;
		return;
	}

	va_start(ap, format);

	vsprintf(message, format, ap); // Prints the contents of ap, using format format, to the buffer message.
	fprintf(dotxfid, "%s\t%s\t%d\t%s\n",file, function, line, message);

	fflush(dotxfid);

	va_end(ap);

}

#endif
