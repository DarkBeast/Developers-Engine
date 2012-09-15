#ifndef _PROGRAMPATH_H
#define _PROGRAMPATH_H

#include"integer.h"

char* Program_Path;
char* Path;
char* GetPath(char* target);
int GetCurrentDirectory( Uint32 iTarget, Uint32 *iRequired);
void PathDestroy(void);

#endif

