/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/
#ifndef DE_MAIN_H
#define DE_MAIN_H

#include "tinycthread.h"

int load_data(void *arg);
void unload_functions(void);
mtx_t *get_mutex(void);

#endif