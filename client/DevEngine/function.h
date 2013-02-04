/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/

#ifndef DE_FUNCTION_H
#define DE_FUNCTION_H

#include <stdlib.h>

size_t next_power_of_two(size_t input);

#ifndef max
#define max( a, b) ( ((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min( a, b) ( ((a) < (b)) ? (a) : (b))
#endif

#endif