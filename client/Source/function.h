/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/

#ifndef DE_FUNCTION_H
#define DE_FUNCTION_H

#include <stdlib.h>
#include "integer.h"

double gettickcount(void);

//gets the next power of 2.
size_t next_power_of_two(size_t input);
sbool comp_str(char *string1, char *string2);

#ifndef max
#define max( a, b) ( ((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min( a, b) ( ((a) < (b)) ? (a) : (b))
#endif

#endif