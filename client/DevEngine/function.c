/*******************************************************************************
* Credits:  Stephan    
******************************************************************************/

#include "function.h"

size_t next_power_of_two(size_t input) {
	size_t index;

	--input;

	for (index = 1; index < sizeof(size_t); index <<= 1)
		input |= input >> index;

	return ++input;
}