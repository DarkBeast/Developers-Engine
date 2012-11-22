/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
*           Freetype library 2
******************************************************************************/
#ifndef _TEXT_H
#define _TEXT_H

#include <ft2build.h>
#include FT_FREETYPE_H
#include "types.h"

typedef struct
{
	FT_Library library;
	FT_Face face;
	const char *fontfilename;
	GLuint tex;
}fonts;

typedef struct
{
	char *text;
	color col;
	int size;
	vector2i xy;
	vector2i hw;
}text;

void initfont(const char *fontname);
void rendertext(text *str);
#endif