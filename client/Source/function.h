/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/

#ifndef DE_FUNCTION_H
#define DE_FUNCTION_H

#include <stdlib.h>
#include "integer.h"
#include "text.h"

//gets the next power of 2.
size_t next_power_of_two(size_t input);
char *trim_string(char * str) ;
sbool is_space(char c);
//integer to string.
char* int_to_string(int64 number);

#ifndef max
#define max( a, b) ( ((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min( a, b) ( ((a) < (b)) ? (a) : (b))
#endif

#endif