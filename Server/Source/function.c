/*******************************************************************************
* Credits:  Stephan
*           Purlox
******************************************************************************/

#include "function.h"
#include <glfw3.h>

void init_timer(void)
{
	if(glfwInit() == FALSE)
		exit(FALSE);
}

double gettickcount(void)
{
	return glfwGetTime();
}

size_t next_power_of_two(size_t input)
{
	size_t index;

	for (index = 1; index < sizeof(size_t); index <<= 1) {
		input |= input >> index;
	}

	return ++input;
}

sbool comp_str(char *string1, char *string2)
{
	uint32 i = 1;

	while(*string2){
		if(*(string1 + i) == *(string2++)){
			if(*(string1 + i) == ' ' || *(string1 + i) == '\n')
				return TRUE;

			i++;
		}
		else{
			if(*(string1 + i) == ' ' || *(string1 + i) == '\n')
				return TRUE;

			return FALSE;
		}
		return FALSE;
	}
}