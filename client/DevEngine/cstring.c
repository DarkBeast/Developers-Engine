/*******************************************************************************
* Credits:  Anthony  Ianozi                                                         *
******************************************************************************/
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "cstring.h"
#include "function.h"
#include "integer.h"

cstring* String_Create(uint32 icapacity)
{
	cstring* objstring = NULL;
	objstring = (cstring*)calloc(1, sizeof(cstring) + icapacity *
		sizeof(uint32));
	if(objstring == NULL)
	{
		return NULL;
	}
	objstring->data = (char*)(objstring + sizeof(cstring));
	objstring->size = icapacity;
	objstring->count = 0;
	return objstring;
}

cstring *string_createFromASCIIstring(const char *sstring)
{
	cstring *objstring = NULL;
	uint32     *ioffset   = NULL;
	char       *soffset   = (char*)sstring;
	uint32      icapacity = 0;
	/* Get length of string */
	while (*soffset > 0)
	{
		++soffset;
	}
	icapacity = soffset - sstring;
	objstring = (cstring*)calloc(1, sizeof(cstring) + icapacity *
		sizeof(uint32));
	if (objstring == NULL)
	{
		return NULL;
	}
	/* Copy ascii string to new String */
	objstring->data = (char *)objstring + sizeof(*objstring);
	objstring->count = icapacity;
	objstring->size = icapacity;
	soffset = (char *)sstring;
	ioffset = (uint32 *)objstring->data;
	while (*soffset > 0)
	{
		*ioffset = *soffset;
		++soffset;
		++ioffset;
	}
	return objstring;
}

void string_destroy(cstring* objstring)
{
	free(objstring);
}

cstring* String_Concatenate(cstring* objtarget, cstring* objsource)
{
	if(objsource->count > objtarget->size -
		objtarget->count)
	{
		return NULL;
	}
	memcpy(objtarget->data+objtarget->count,
		objsource->data, objsource->count * sizeof(uint32));
	objtarget->count += objsource->count;
	return objtarget;
}
extern cstring* string_concatenatefromASCIIstring(cstring* objtarget, const char* ssource)
{
	uint32     *ioffset   = NULL;
	char       *soffset   = (char*)ssource;
	uint32      isize = 0;
	/* Get the length of the source string */
	while (*soffset > 0)
	{
		++soffset;
	}
	isize = soffset - ssource;
	/* Add the ascii string to the String */
	soffset = (char *)ssource;
	ioffset = (uint32 *)(objtarget->data+objtarget->count);
	objtarget->count += isize;
	while (*soffset > 0)
	{
		*ioffset = *soffset;
		++soffset;
		++ioffset;
	}
	return objtarget;
	return objtarget;
}
uint32 string_length(cstring* objstring)
{
	if(objstring == NULL)
	{
		return -1;
	}
	return objstring->count;
}

uint32 GetASCIIstringlength(const char* sstring)
{
	/* There _must_ be a faster way than this! */
	const char* sstart = (char*)sstring;
	while(*sstring)
	{
		++sstring;
	}
	return sstring - sstart;
}

cstring* string_copyfromASCIIstring(cstring* objtarget, const char* ssource)
{
	uint32     *ioffset   = NULL;
	char       *soffset   = (char*)ssource;
	uint32      isize = 0;
	/* Get the length of the source string */
	while (*soffset > 0)
	{
		++soffset;
	}
	isize = soffset - ssource;
	if(isize > objtarget->size - objtarget->count)
	{
		return NULL;
	}
	/* Copy ascii string to new String */
	objtarget->count = isize;
	soffset = (char *)ssource;
	ioffset = (uint32 *)objtarget->data;
	while (*soffset > 0)
	{
		*ioffset = *soffset;
		++soffset;
		++ioffset;
	}
	return objtarget;
}

cstring* string_copy(cstring* objtarget, cstring* objsource)
{
	if(objsource->count > objtarget->size)
	{
		return NULL;
	}
	memcpy(objtarget->data, objsource->data,
		objsource->count * sizeof(uint32));
	return objtarget;
}