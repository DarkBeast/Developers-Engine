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
	uint16 x;
	uint16 y;
	int16 advancex;
	int16 advancey;
};		// character information

struct text
{
	char *data;
	multi_buffer textbuf; //opengl buffer for text.
	color col; //color of text
	vector2ui xy; // X location and Y location on screen
	uint16 size;
	uint16 count;
	uint8 resize;
};

void text_init_font(char *fontname);
void text_init_atlas(void);
void text_draw(text *str, widget *parent);
void text_set(text *text_data, uint16 x, uint16 y, uint8 size, uint8 red,uint8 blue,uint8 green,uint8 alpha, char *string);
vector2ui text_get_string_hw(char *string, uint8 resize);
symbol* text_get_character_array(void);

void text_draw_beta(text *str, widget *parent);
void text_resize_buffer(openext *buffer, uint16 size);
void create_text_vertex(text *str, widget *control);
#endif