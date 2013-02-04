//ignore this for now its crap

/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
*           Freetype library 2
******************************************************************************/
#ifndef DE_TEXT_H
#define DE_TEXT_H

#include <ft2build.h>
#include FT_FREETYPE_H
#include <freetype/ftglyph.h>
#include "types.h"
#include "integer.h"
#include "widget.h"

typedef struct fonts fonts;
typedef struct symbol symbol;
typedef struct text text;

struct fonts
{
	FT_Face face;
	const char *fontfilename;
};

struct symbol
{
	FT_Glyph glyph;
	FT_BitmapGlyph  glyph_bitmap;
	GLuint tex;
	int16 advancex;
	int16 advancey;
};		// character information

struct text
{
	char *data;
	color col; //color of text
	vector2ui xy; // X location and Y location on screen
	uint8 resize;
};

void init_font(char *fontname);
void init_atlas(void);
void draw_text(text *str, widget *parent);
void set_text(text *text_data, uint16 x, uint16 y, uint8 size, uint8 red,uint8 blue,uint8 green,uint8 alpha, char *string);
vector2ui get_max_string_hw(char *string, uint8 resize);
symbol* get_character_array(void);

#endif