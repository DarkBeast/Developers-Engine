/*******************************************************************************
* Credits:  Anthony  Ianozi                                                         *
******************************************************************************/
#ifndef _CSTRING_H
#define _CSTRING_H

#include"integer.h"

typedef struct cstring
{
	char* data;
	size_t  count;
	size_t  size;
} cstring;

/* Creates a new String */
extern cstring* String_Create(uint32 iCapacity);

/* Creates a new String based off an ASCII string */
extern cstring *String_CreateFromASCIIString(const char *sString);

/* Deletes a formally-created String */
extern void string_destroy(cstring* objstring);

/* Returns the length of a String */
extern uint32 string_Length(cstring* objstring);

/* Returns the size of an ASCII-string */
extern uint32 getASCIIstringlength(const char* sString);

/* Copies a String objSource to objTarget */
extern cstring* string_copy(cstring* objtarget, cstring* objsource);

/* Copies an ASCII string to a String */
extern cstring* string_copyfromASCIIstring(cstring* objtarget, const char* ssource);

/* Concatenates two String strings. */
extern cstring* string_concatenate(cstring* objtarget, cstring* objsource);

/* Concatenates a String with an ASCII string */
extern cstring* string_concatenatefromASCIIstring(cstring* objtarget, const char* ssource);

#endif