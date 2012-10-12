#ifndef _PROGRAMPATH_H
#define _PROGRAMPATH_H

char* Program_Path;
char* Path;

char* GetPath(char* target);
int CDLGetCurrentDirectory();
void PathDestroy(void);

#endif

