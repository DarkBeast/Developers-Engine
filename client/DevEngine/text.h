//ignore this for now its crap

/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
*           Freetype library 2
******************************************************************************/
#ifndef _TEXT_H
#define _TEXT_H

#include <ft2build.h>
#include FT_FREETYPE_H
#include <freetype/ftglyph.h>
#include "types.h"

typedef struct
{
	FT_Face face;
	const char *fontfilename;
}fonts;

typedef struct
{
	FT_Glyph glyph;
	FT_BitmapGlyph  glyph_bitmap;
	GLuint tex;
	int advancex;
	int advancey;
} symbol;		// character information

typedef struct
{
	char *data;
	size_t  count;
	size_t  size;
	color col; //color of text
	vector2i xy; // X location and Y location on screen
	int resize;
}text;

void initfont(char *fontname);
void initatlas();
void rendertext(text *str);
void settext(text *data, int x, int y, int size, int red, int blue, int green, int alpha, char* string);
#endif