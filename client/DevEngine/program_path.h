/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis    
******************************************************************************/

#ifndef _PROGRAMPATH_H
#define _PROGRAMPATH_H
//#include "String.h"

char* Program_Path;

char* GetPath(const char* target);
int CDLGetCurrentDirectory();
void PathDestroy(void);

#endif

