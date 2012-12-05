//ignore this for now its crap

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
	GLuint texture;
	FT_GlyphSlot g;
}glyph_array;

typedef struct
{
	char* data;
	size_t  count;
	size_t  size;
	color col; //color of text
	int charactersize;
	vector2i xy; // X location and Y location on screen
	vector2i hw; //height width
}text;

typedef struct  smallbitmap
{
	int             rows;
	int             width;
	unsigned char*  buffer;
} smallbitmap;

typedef struct
{
	smallbitmap image; //hold the Bitmap details, pixels, width etc.
	GLuint tex; //holds the character loaded image.
	int advancex;	// advance.x
	int advancey;	// advance.y
	int bitmapleft;	// bitmap_left;
	int bitmaptop;	// bitmap_top;
	int offsetx;	// x offset of glyph in texture coordinates
	int offsety;	// y offset of glyph in texture coordinates
} symbol;		// character information

typedef struct
{
	GLuint tex;		// texture object Hold the entire font image.
	int width;			// width of texture in pixels
	int height;			// height of texture in pixels
	symbol character[0x0010FFFF]; //holds individual character bitmap data etc.
}atlas;

void initfont(char *fontname);
void initatlas(FT_Face face, int height, atlas *cache);
void rendertext(text *str);
#endif