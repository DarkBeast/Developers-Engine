/*******************************************************************************
 * Credits:  Anthony                                                           *
 ******************************************************************************/
#ifndef _STRING_H
#define _STRING_H
#include"Integer.h"

typedef struct String
{
  Uint32* data;
  Uint32  count;
  Uint32  size;
} String;

/* Creates a new String */
extern String* String_Create(Uint32 iCapacity);

/* Creates a new String based off an ASCII string */
extern String *String_CreateFromASCIIString(const char *sString);

/* Deletes a formally-created String */
extern void String_Destroy(String* objString);

/* Returns the length of a String */
extern Uint32 String_Length(String* objString);

/* Returns the size of an ASCII-string */
extern Uint32 GetASCIIStringLength(const char* sString);

/* Copies a String objSource to objTarget */
extern String* String_Copy(String* objTarget, String* objSource);

/* Copies an ASCII string to a String */
extern String* String_CopyFromASCIIString(String* objTarget, const char* sSource);

/* Concatenates two String strings. */
extern String* String_Concatenate(String* objTarget, String* objSource);

/* Concatenates a String with an ASCII string */
extern String* String_ConcatenateFromASCIIString(String* objTarget, const char* sSource);

#endif