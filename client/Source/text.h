/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
*           Freetype library 2
******************************************************************************/
#ifndef DE_TEXT_H
#define DE_TEXT_H
#include <ft2build.h>
#include FT_FREETYPE_H
#include <freetype/ftglyph.h>
#include "widget.h"
#include "types.h"
#include "integer.h"

typedef struct fonts fonts;
typedef struct atlas atlas;
typedef struct symbol symbol;
typedef struct text text;

struct fonts
{
	FT_Face face; //Font rendering data cache.
	const char *fontfilename;
};

struct atlas
{
	GLuint tex; //texture
	int16 h; //texture height
	int16 w; //texture width
	symbol *c; //holds individual character bitmap data etc.
};

struct symbol
{
	float width; // bitmap.width;
	float height; // bitmap.height;
	float left; // bitmap_left;
	float top; // bitmap_top;
	float x; //texture x
	float y; //texture y
	float ax; //advance x
	float ay; //advance y
	uint16 mheight; //metric height, space between lines.
};		// character information

struct text
{
	char *data; //the entire string.
	uint16 size; //string max size.
	uint16 count; //string count.
	mbuffer buf; //opengl buffer.
	color col; //color of text.
	vector2ui xy; // X and Y location on screen.
	uint16 width; //width of control.
	uint16 height; //height of control.
	uint16 lines; // amount of lines;
	uint16 maxchars; //maximum amount of characters allowed per line.
	uint16 maxcharspl; //maximum amount of characters per line.
	uint8 offsetx; //rendering offset x.
	uint8 offsety; //rendering offset y.
	uint16 textheight; //text height.
	uint16 textwidth; //text width.
	uint16 displayoffset; //variable used to update drawable.
	atlas *font; //font used for drawing.
};

void text_init_font(char *fontname);
void text_init_atlas(atlas *data, uint8 textsize);
void text_draw(text *str);
void text_set(text *data, uint16 x, uint16 y, uint16 width, uint16 height, uint8 offsetx, uint8 offsety, uint16 maxchars, uint16 maxcharspl, uint8 fontid, uint8 red, uint8 blue, uint8 green, uint8 alpha, char *string);
atlas *get_atlas(uint8 fontid);

void text_resize_buffer(vertex_array **buffer, size_t size);
void create_text_vertex(text *str, widget *control);
void text_position_supdate(text *str, widget *parent);
void text_position_mupdate(text *str, widget *parent);
void string_resize(text *str, uint32 size);
void stextbox_text_update(text *str, widget *control);
void mtextbox_text_update(text *str, widget *control);
#endif