/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/

#ifndef _PROGRAMPATH_H
#define _PROGRAMPATH_H

char* program_path;

char* getpath(char* target);
int getprogramdirectory(void);//get program directory
void pathdestroy(void);

#endif