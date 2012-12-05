/*******************************************************************************
* Credits:  Anthony  Ianozi                                                         *
******************************************************************************/
#ifndef _CSTRING_H
#define _CSTRING_H

#include"integer.h"
#include"types.h"
#include "text.h"

/* Creates a new String */
extern text* String_Create(uint32 iCapacity);

/* Creates a new String based off an ASCII string */
extern text *String_CreateFromASCIIString(const char *sString);

/* Deletes a formally-created String */
extern void string_destroy(text* objstring);

/* Returns the length of a String */
extern uint32 string_Length(text* objstring);

/* Returns the size of an ASCII-string */
extern uint32 getASCIIstringlength(const char* sString);

/* Copies a String objSource to objTarget */
extern text* string_copy(text* objtarget, text* objsource);

/* Copies an ASCII string to a String */
extern text* string_copyfromASCIIstring(text* objtarget, const char* ssource);

/* Concatenates two String strings. */
extern text* string_concatenate(text* objtarget, text* objsource);

/* Concatenates a String with an ASCII string */
extern text* string_concatenatefromASCIIstring(text* objtarget, const char* ssource);

#endif