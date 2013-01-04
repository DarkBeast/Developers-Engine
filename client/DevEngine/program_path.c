/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
*           Stephan
******************************************************************************/
#include <direct.h>
#include <stdlib.h>
#include <string.h>
#include "function.h"
#include "integer.h"
#include "program_path.h"
#include "bool.h"
//hex example 0x0010FFFF = 1,114,111
//example 0x00000000 = 0

uint32 ipath   = 0;
uint32 isize   = 1024;
char* path;

char* getpath(char* target)
{
	//TODO: Set Cache for string sizes
	if(strlen(program_path)  + strlen(target) >= strlen(path))
		path = (char *)realloc(path, next_power_of_two(strlen(program_path)  + strlen(target)));

	strcpy(path ,program_path);
	strcat(path ,target);

	return path;
}

int getprogramdirectory(void)
{//get program directory
	char  *spath   = NULL;
	char  *sresult = NULL;

	path = NULL;
	path = (char *)malloc(1024);
	program_path = (char *)malloc(isize  + 1);
	spath = (char *)malloc(isize);

	if (program_path == NULL)
	{
		return FALSE;
	}

	if (spath == NULL)

	{
		return FALSE;
	}

	sresult = _getcwd(spath, isize);

	while (sresult == NULL)

	{
		isize *= 2;

		spath = (char *)realloc(spath, isize);

		if (spath == NULL)

		{
			return FALSE;
		}

		sresult = _getcwd(spath, isize);
	}

	ipath = strlen(spath) + 1;

	if (program_path != NULL && isize  >= ipath)

	{
		memcpy(program_path, spath, ipath);
	}

	free(spath);
	strcat(program_path ,"\\");
	if (program_path == NULL || isize  < ipath)
	{
		return FALSE;
	}

	return TRUE;
}

void pathdestroy(void)
{
	program_path =(char *) malloc (sizeof(program_path+1024));

	free(program_path);
	return;
}