/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/

#ifndef DE_PROGRAMPATH_H
#define DE_PROGRAMPATH_H

char* program_path;

char* getpath(char* target);
int getprogramdirectory(void);//get program directory
void pathdestroy(void);

#endif
