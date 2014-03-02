/*******************************************************************************
* Credits:  S.J.R. van Schaik
*           Purlox
******************************************************************************/

#include "function.h"
#include <glfw3.h>

//does not need initialization client side due to client already init it during window load up.
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

	if(string1 == NULL || string2 == NULL)
		return FALSE;

	if(*(string1) != *(string2))
		return FALSE;

	while(*string2){
		if(*(string1 + i) == *(string2 + i)){
			if(*(string1 + i + 1) == ' ' || *(string1 + i + 1) == '\n' || *(string1 + i + 1) == NULL ){
				return TRUE;
			}
			i++;
		}
		else{
			if(*(string1 + i) == ' ' || *(string1 + i) == '\n'){
				return TRUE;
			}
			return FALSE;
		}	
	}

	return FALSE;
}