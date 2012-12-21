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
#include "integer.h"

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
	int16 advancex;
	int16 advancey;
} symbol;		// character information

typedef struct
{
	char *data;
	color col; //color of text
	vector2ui xy; // X location and Y location on screen
	uint8 resize;
}text;

void initfont(char *fontname);
void initatlas(void);
void drawtext(text *str);
void settext(text *data, uint16 x, uint16 y, uint8 size, uint8 red,uint8 blue,uint8 green,uint8 alpha, char *string);
vector2ui getmaxstringhw(char *string, uint8 resize);
symbol* getcharacterarray(void);

#endif