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