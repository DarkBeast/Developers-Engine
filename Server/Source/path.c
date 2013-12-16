/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
*           Stephan
******************************************************************************/
#include <stdlib.h>
#include <string.h>
#include "function.h"
#include "path.h"
#include "error.h"
#include <io.h>
#include <direct.h>

char *path;
char *string = NULL;
char *number_string;
#define MAX_NUMBER_LENGTH 12

char* get_path(char *file_path, int32 file_number, char *file_type)
{
	char* temp;
	size_t size = strlen(file_path) + strlen("\\") + MAX_NUMBER_LENGTH + strlen(file_type);

	if(size >= strlen(path)){
		temp = (char *)realloc(path, next_power_of_two(size));

		if(temp == NULL){
			error_handler(DE_ERROR_POINTER_NULL);
			return NULL;
		}

		path = temp;
	}

	if(path == NULL){
		error_handler(DE_ERROR_POINTER_NULL);
		return NULL;
	}

	strcpy(path ,file_path);
	strcat(path ,"\\");
	strcat(path ,int_to_string(file_number));
	strcat(path ,file_type);

	return path;
}

char* get_path_name(char *file_path, char *file_name, char *file_type)
{
	char* temp;
	size_t size = strlen(file_path) + strlen("\\") + strlen(file_name) + strlen(file_type);

	if(size >= strlen(path)){
		temp = (char *)realloc(path, next_power_of_two(size));

		if(temp == NULL){
			error_handler(DE_ERROR_POINTER_NULL);
			return NULL;
		}

		path = temp;
	}

	if(path == NULL){
		error_handler(DE_ERROR_POINTER_NULL);
		return NULL;
	}

	strcpy(path ,file_path);
	strcat(path ,"\\");
	strcat(path ,file_name);
	strcat(path ,file_type);

	return path;
}

void init_path(void)
{
	path = (char *)calloc(1024,sizeof(char));
	string = (char *)calloc(1024,sizeof(char));
	number_string = (char *)calloc(1024,sizeof(char));
}

void path_destroy(void)
{
	free(string);
	free(number_string);
	free(path);
}

sbool file_exists(char *path)
{
	if(_access(path, 0) == 0)
		return TRUE;

	return FALSE;
}

sbool file_writable(char *path)
{
	if(_access(path, 0) == 0 && _access(path, 2) == 0)
		return TRUE;

	return FALSE;
}

sbool file_readable(char *path)
{
	if(_access(path, 0) == 0 && _access(path, 4) == 0)
		return TRUE;

	return FALSE;
}

sbool file_rw(char *path)
{
	if(_access(path, 0) == 0 && _access(path, 6) == 0)
		return TRUE;

	return FALSE;
}

void check_dir(char *path)
{
	if(!file_exists(path)){
		if(_mkdir(path))
			error_handler(DE_ERROR_FILE_ERROR);
	}
}

char *comb_str(char *a, char *b, char *c, char *d, char *e, char *f, char *g)
{
	uint32 size;
	char* temp;

	size = strlen(a);
	if(b != "" || b != NULL)
		size += strlen(b);

	if(c != "" || c != NULL)
		size += strlen(c);

	if(d != "" || d != NULL)
		size += strlen(d);

	if(e != "" || e != NULL)
		size += strlen(e);

	if(f != "" || f != NULL)
		size += strlen(f);

	if(g != "" || g != NULL)
		size += strlen(g);

	if(size >= strlen(string)){
		temp = (char *)realloc(string, next_power_of_two(size));

		if(temp == NULL){
			error_handler(DE_ERROR_POINTER_NULL);
			return NULL;
		}

		string = temp;
	}

	if(string == NULL){
		error_handler(DE_ERROR_POINTER_NULL);
		return NULL;
	}

	strcpy(string ,a);

	if(b != "" || b != NULL)
		strcat(string ,b);

	if(c != "" || c != NULL)
		strcat(string ,c);

	if(d != "" || d != NULL)
		strcat(string ,d);

	if(e != "" || e != NULL)
		strcat(string ,e);

	if(f != "" || f != NULL)
		strcat(string ,f);

	if(g != "" || g != NULL)
		strcat(string ,g);

	return string;
}

char *comb_2str(char *a, char *b)
{
	uint32 size;
	char* temp;

	size = strlen(a);
	if(b != "" || b != NULL)
		size += strlen(b);

	if(size >= strlen(string)){
		temp = (char *)realloc(string, next_power_of_two(size));

		if(temp == NULL){
			error_handler(DE_ERROR_POINTER_NULL);
			return NULL;
		}

		string = temp;
	}

	if(string == NULL){
		error_handler(DE_ERROR_POINTER_NULL);
		return NULL;
	}

	strcpy(string ,a);

	if(b != "" || b != NULL)
		strcat(string ,b);

	return string;
}

char *comb_3str(char *a, char *b, char *c)
{
	uint32 size;
	char* temp;

	size = strlen(a);
	if(b != "" || b != NULL)
		size += strlen(b);

	if(c != "" || c != NULL)
		size += strlen(c);

	if(size >= strlen(string)){
		temp = (char *)realloc(string, next_power_of_two(size));

		if(temp == NULL){
			error_handler(DE_ERROR_POINTER_NULL);
			return NULL;
		}

		string = temp;
	}

	if(string == NULL){
		error_handler(DE_ERROR_POINTER_NULL);
		return NULL;
	}

	strcpy(string ,a);

	if(b != "" || b != NULL)
		strcat(string ,b);

	if(c != "" || c != NULL)
		strcat(string ,c);

	return string;
}

char *comb_4str(char *a, char *b, char *c, char *d)
{
	uint32 size;
	char* temp;

	size = strlen(a);
	if(b != "" || b != NULL)
		size += strlen(b);

	if(c != "" || c != NULL)
		size += strlen(c);

	if(d != "" || d != NULL)
		size += strlen(d);

	if(size >= strlen(string)){
		temp = (char *)realloc(string, next_power_of_two(size));

		if(temp == NULL){
			error_handler(DE_ERROR_POINTER_NULL);
			return NULL;
		}

		string = temp;
	}

	if(string == NULL){
		error_handler(DE_ERROR_POINTER_NULL);
		return NULL;
	}

	strcpy(string ,a);

	if(b != "" || b != NULL)
		strcat(string ,b);

	if(c != "" || c != NULL)
		strcat(string ,c);

	if(d != "" || d != NULL)
		strcat(string ,d);

	return string;
}

char *comb_5str(char *a, char *b, char *c, char *d, char *e)
{
	uint32 size;
	char* temp;

	size = strlen(a);
	if(b != "" || b != NULL)
		size += strlen(b);

	if(c != "" || c != NULL)
		size += strlen(c);

	if(d != "" || d != NULL)
		size += strlen(d);

	if(e != "" || e != NULL)
		size += strlen(e);

	if(size >= strlen(string)){
		temp = (char *)realloc(string, next_power_of_two(size));

		if(temp == NULL){
			error_handler(DE_ERROR_POINTER_NULL);
			return NULL;
		}

		string = temp;
	}

	if(string == NULL){
		error_handler(DE_ERROR_POINTER_NULL);
		return NULL;
	}

	strcpy(string ,a);

	if(b != "" || b != NULL)
		strcat(string ,b);

	if(c != "" || c != NULL)
		strcat(string ,c);

	if(d != "" || d != NULL)
		strcat(string ,d);

	if(e != "" || e != NULL)
		strcat(string ,e);

	return string;
}

char *comb_7str(char *a, char *b, char *c, char *d, char *e, char *f, char *g)
{
	uint32 size;
	char* temp;

	size = strlen(a);
	if(b != "" || b != NULL)
		size += strlen(b);

	if(c != "" || c != NULL)
		size += strlen(c);

	if(d != "" || d != NULL)
		size += strlen(d);

	if(e != "" || e != NULL)
		size += strlen(e);

	if(f != "" || f != NULL)
		size += strlen(f);

	if(g != "" || g != NULL)
		size += strlen(g);

	if(size >= strlen(string)){
		temp = (char *)realloc(string, next_power_of_two(size));

		if(temp == NULL){
			error_handler(DE_ERROR_POINTER_NULL);
			return NULL;
		}

		string = temp;
	}

	if(string == NULL){
		error_handler(DE_ERROR_POINTER_NULL);
		return NULL;
	}

	strcpy(string ,a);

	if(b != "" || b != NULL)
		strcat(string ,b);

	if(c != "" || c != NULL)
		strcat(string ,c);

	if(d != "" || d != NULL)
		strcat(string ,d);

	if(e != "" || e != NULL)
		strcat(string ,e);

	if(f != "" || f != NULL)
		strcat(string ,f);

	if(g != "" || g != NULL)
		strcat(string ,g);

	return string;
}

char* int_to_string(int64 number)
{
	char *result;
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

	strcpy(number_string, "");
	if(negative_number == 1) {
		number_string[index] = '-';
		index++;
	}

	while(number_of_digits > 0) {
		number_string[index] = (char)((int)'0' + (int)help);
		help = (help - (int)help) * 10;

		number_of_digits--;
		index++;
	}

	number_string[index] = NULL;

	return number_string;
}

char *trim_string(char *str)
{
	uint16 first_nonspace = 0;  // First non-space character
	uint16 last_nonspace = 0;   // Last non-space character
	int i;

	if(str == NULL)
		return NULL;

	for(i = 0; is_space(str[i]) == 1; i++)
		;

	first_nonspace = i;

	for( ; str[i] != '\0'; i++)
		;

	for(i--; is_space(str[i]) == 1; i--)
		;

	last_nonspace = i;

	if(first_nonspace != 0) {
		for(i = 0; i != last_nonspace - first_nonspace + 1; i++) {
			str[i] = str[i + first_nonspace];
		}

		str[i] = '\0';
	} else {
		str[last_nonspace + 1] = '\0';
	}

	return str;
}

sbool is_space(char c)
{
	switch(c){
	case ' ':
	case '\n':
	case '\t':
	case '\v':
	case '\f':
	case '\r':
		return TRUE;
	default:
		return FALSE;
	}
}