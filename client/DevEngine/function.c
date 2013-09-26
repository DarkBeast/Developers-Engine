/*******************************************************************************
* Credits:  Stephan
*           Purlox
******************************************************************************/

#include "function.h"

size_t next_power_of_two(size_t input)
{
	size_t index;

	for (index = 1; index < sizeof(size_t); index <<= 1) {
		input |= input >> index;
	}

	return ++input;
}

char* int_to_string(int64 number) {
	char *result = NULL;
	long double help = (long double)number;
	uint16 number_of_digits = 0;
	uint16 index = 0;
	char negative_number = 0; // pseudo bool variable holding whether or not the number is negative

	if((int64)help == 0) {
		return "0";
	}

	if((int64)help < 0) {
		negative_number = 1;
		help = -help;
	}

	while((int64)help > 0) {
		help /= 10;
		number_of_digits++;
	}

	help *= 10;

	if(negative_number == 1) {
		result = (char *)calloc(1, number_of_digits + 2 * sizeof(char)); // needs 1 more char for end of string
		result[index] = '-';

		index++;
	} else {
		result = (char *)calloc(1, number_of_digits + 1 * sizeof(char)); // needs 1 more char for end of string
	}

	while(number_of_digits > 0) {
		result[index] = (char)((int)'0' + (int)help);
		help = (help - (int)help) * 10;

		number_of_digits--;
		index++;
	}

	result[index] = NULL;

	return result;
}