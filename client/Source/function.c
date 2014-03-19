/*******************************************************************************
* Credits:  S.J.R. van Schaik
*           Purlox
******************************************************************************/

#include "function.h"
#include <glfw3.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include "path.h"

//does not need initialization client side due to client already init it during window load up.
double gettickcount(void)
{
	return glfwGetTime();
}

size_t next_power_of_two(size_t input)
{
	size_t index;

	for (index = 1; index < sizeof(size_t); index <<= 1)
		input |= input >> index;

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
			if(*(string1 + i + 1) == ' ' || *(string1 + i + 1) == '\n' || *(string1 + i + 1) == NULL )
				return TRUE;

			i++;
		}
		else{
			if(*(string1 + i) == ' ' || *(string1 + i) == '\n')
				return TRUE;

			return FALSE;
		}
	}

	return FALSE;
}

void add_log(char *string, char *path)
{
	FILE *fp;
	time_t current_time;
	char* c_time_string;
	current_time = time(NULL);

	c_time_string = ctime(&current_time);

	if(!file_exists(path)){
		if((fp = fopen(path, "w")) == NULL)
			fputs("ERROR: File Error!\n", stderr);
		fclose(fp);
	}

	if((fp = fopen(path, "r+")) == NULL)
		fputs("ERROR: File Error!\n", stderr);

	c_time_string[strlen(c_time_string) - 1] = '\0';

	fseek(fp, 0, SEEK_END);
	fprintf(fp, "\n %s: %s", c_time_string, string);

	fclose(fp);
}