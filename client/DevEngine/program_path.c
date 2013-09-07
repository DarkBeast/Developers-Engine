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
#include "error.h"

uint32 ipath   = 0;
uint32 isize   = 1024;
char* path;

char* get_path(char* target)
{
	char* temp;

	if(strlen(program_path)  + strlen(target) >= strlen(path)){
		temp = (char *)realloc(path, next_power_of_two(strlen(program_path)  + strlen(target)));

		if(temp == NULL){
			fatal_error(ERROR_POINTER_NULL);
			return 0;
		}

		path = temp;
	}

	if(path == NULL){
		fatal_error(ERROR_POINTER_NULL);
		return 0;
	}

	strcpy(path ,program_path);
	strcat(path ,target);

	return path;
}

int get_program_directory(void)
{//get program directory
	char  *spath   = NULL;
	char  *sresult = NULL;
	char  *temp = NULL;

	path = NULL;
	path = (char *)malloc(1024);
	program_path = (char *)malloc(isize  + 1);
	spath = (char *)malloc(isize);

	if (program_path == NULL){
		return FALSE;
	}

	if (spath == NULL){
		return FALSE;
	}

	sresult = _getcwd(spath, isize);

	while (sresult == NULL){
		isize *= 2;

		temp = (char *)realloc(spath, isize);

		spath = temp;

		if (spath == NULL){
			return FALSE;
		}

		sresult = _getcwd(spath, isize);
	}

	ipath = strlen(spath) + 1;

	if (program_path && isize  >= ipath){
		memcpy(program_path, spath, ipath);
	}

	free(spath);
	strcat(program_path ,"\\");
	if (program_path == NULL || isize  < ipath){
		fatal_error(ERROR_PATH_INCORRECT);
		return FALSE;
	}

	return TRUE;
}

void path_destroy(void)
{
	program_path =(char *) malloc (sizeof(program_path+1024));

	free(program_path);
	return;
}