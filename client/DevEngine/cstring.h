/*******************************************************************************
* Credits:  Anthony  Ianozi                                                         *
******************************************************************************/
#ifndef DE_CSTRING_H
#define DE_CSTRING_H

#include"integer.h"
#include"types.h"
#include "cstring.h"

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