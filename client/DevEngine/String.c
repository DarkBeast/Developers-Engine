/*******************************************************************************
* Credits:  Anthony  Ianozi                                                         *
******************************************************************************/
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "String.h"
#include "function.h"
#include "integer.h"

String* String_Create(Uint32 iCapacity)
{
	String* objString = NULL;
	objString = (String*)calloc(1, sizeof(String) + iCapacity * 
		sizeof(Uint32));
	if(objString == NULL)
	{
		return NULL;
	}
	objString->data = (char*)(objString + sizeof(String));
	objString->size = iCapacity;
	objString->count = 0;
	return objString;
}

String *String_CreateFromASCIIString(const char *sString)
{
	String *objString = NULL;
	Uint32     *iOffset   = NULL;
	char       *sOffset   = (char*)sString;
	Uint32      iCapacity = 0;
	/* Get length of string */
	while (*sOffset > 0)
	{
		++sOffset;
	}
	iCapacity = sOffset - sString;
	objString = (String*)calloc(1, sizeof(String) + iCapacity * 
		sizeof(Uint32));
	if (objString == NULL)
	{
		return NULL;
	}
	/* Copy ascii string to new String */
	objString->data = (char *)objString + sizeof(*objString);
	objString->count = iCapacity;
	objString->size = iCapacity;
	sOffset = (char *)sString;
	iOffset = (Uint32 *)objString->data;
	while (*sOffset > 0)
	{
		*iOffset = *sOffset;
		++sOffset;
		++iOffset;
	}
	return objString;
} 


void String_Destroy(String* objString)
{
	free(objString);
}

String* String_Concatenate(String* objTarget, String* objSource)
{
	if(objSource->count > objTarget->size -
		objTarget->count)
	{
		return NULL;
	}
	memcpy(objTarget->data+objTarget->count, 
		objSource->data, objSource->count * sizeof(Uint32));
	objTarget->count += objSource->count;
	return objTarget;
}
extern String* String_ConcatenateFromASCIIString(String* objTarget, const char* sSource)
{
	Uint32     *iOffset   = NULL;
	char       *sOffset   = (char*)sSource;
	Uint32      iSize = 0;
	/* Get the length of the source string */
	while (*sOffset > 0)
	{
		++sOffset;
	}
	iSize = sOffset - sSource;
	/* Add the ascii string to the String */
	sOffset = (char *)sSource;
	iOffset = (Uint32 *)(objTarget->data+objTarget->count);
	objTarget->count += iSize;
	while (*sOffset > 0)
	{
		*iOffset = *sOffset;
		++sOffset;
		++iOffset;
	}
	return objTarget;
	return objTarget;
}
Uint32 String_Length(String* objString)
{
	if(objString == NULL)
	{
		return -1;
	}
	return objString->count;
}

Uint32 GetASCIIStringLength(const char* sString)
{
	/* There _must_ be a faster way than this! */
	const char* sStart = (char*)sString;
	while(*sString)
	{
		++sString;
	}
	return sString - sStart;
}

String* String_CopyFromASCIIString(String* objTarget, const char* sSource)
{
	Uint32     *iOffset   = NULL;
	char       *sOffset   = (char*)sSource;
	Uint32      iSize = 0;
	/* Get the length of the source string */
	while (*sOffset > 0)
	{
		++sOffset;
	}
	iSize = sOffset - sSource;
	if(iSize > objTarget->size - objTarget->count)
	{
		return NULL;
	}
	/* Copy ascii string to new String */
	objTarget->count = iSize;
	sOffset = (char *)sSource;
	iOffset = (Uint32 *)objTarget->data;
	while (*sOffset > 0)
	{
		*iOffset = *sOffset;
		++sOffset;
		++iOffset;
	}
	return objTarget;
}

String* String_Copy(String* objTarget, String* objSource)
{
	if(objSource->count > objTarget->size)
	{
		return NULL;
	}
	memcpy(objTarget->data, objSource->data,
		objSource->count * sizeof(Uint32));
	return objTarget;
}

