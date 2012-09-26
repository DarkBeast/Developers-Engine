#ifndef _PROGRAMPATH_H
#define _PROGRAMPATH_H

#include"integer.h"
#include "globals.h"

char* Program_Path;
char* Path;

char* GetPath(char* target);
int CDLGetCurrentDirectory();
void PathDestroy(void);

#endif

