/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/

#ifndef DE_PROGRAMPATH_H
#define DE_PROGRAMPATH_H

char* program_path;

char* get_path(char* target);
int get_program_directory(void);//get program directory
void path_destroy(void);

#endif
