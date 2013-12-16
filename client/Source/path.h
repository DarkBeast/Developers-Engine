/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/
#ifndef DE_PATH_H
#define DE_PATH_H

#include "integer.h"

char* get_path(char *file_path, int32 file_number, char *file_type);
char* get_path_name(char *file_path, char *file_name, char *file_type);
void init_path(void);
void path_destroy(void);
sbool file_exists(char *path);
sbool file_writable(char *path);
sbool file_readable(char *path);
sbool file_rw(char *path);
void check_dir(char *path);
// use "" for anything you do not need to insert.
char *comb_2str(char *a, char *b);
char *comb_3str(char *a, char *b, char *c);
char *comb_4str(char *a, char *b, char *c, char *d);
char *comb_5str(char *a, char *b, char *c, char *d, char *e);
char *comb_7str(char *a, char *b, char *c, char *d, char *e, char *f, char *g);
char* int_to_string(int64 number);
char *trim_string(char * str) ;
sbool is_space(char c);
#endif