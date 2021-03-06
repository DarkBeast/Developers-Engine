/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
*           Freetype library 2
*           Guus Sliepen(http://en.wikibooks.org/wiki/OpenGL_Programming)
******************************************************************************/
#include <GL/glew.h>
#include "text.h"
#include "error.h"
#include "function.h"
#include "globals.h"
#include "path.h"
#include <string.h>
#include "render.h"

fonts font;
FT_Library library;

atlas a12;
atlas a18;
atlas a24;
atlas a48;

atlas get_atlas(uint8 fontid)
{
	switch(fontid){
	case 0: return a12; break;
	case 1: return a18; break;
	case 2: return a24; break;
	case 3: return a48; break;
	default: return a12; break;
	}
}

void unload_fonts(void)
{
	free(a12.c);
	free(a18.c);
	free(a24.c);
	free(a48.c);

	FT_Done_Face(font.face);
	FT_Done_FreeType(library);
}

//sets the text of a objects Control.
void text_set(text *data, uint16 x, uint16 y, uint16 width, uint16 height, uint8 offsetx, uint8 offsety, uint16 maxchars, uint16 maxcharspl, uint8 fontid, uint8 red, uint8 blue, uint8 green, uint8 alpha, char *string)
{
	data->col.a = alpha /255.f;
	data->col.r = red /255.f;
	data->col.b = blue /255.f;
	data->col.g = green /255.f;
	data->xy.x = x;
	data->xy.y = y;
	data->data = string;
	data->width = width;
	data->height = height;
	data->lines = 0;
	data->count = 0;
	data->size = 8;
	data->maxchars = maxchars;
	data->maxcharspl = maxcharspl;
	data->buf.data = NULL;
	data->buf.index = 0;
	data->buf.buffer = 0;
	data->buf.size = 8;
	data->buf.count = 0;
	data->buf.isize = 0;
	data->offsetx = offsetx;
	data->offsety = offsety;
	data->displayoffset = 0;
	data->textwidth = 0;
	data->textheight = 0;

	switch(fontid){
	case 0: data->font = a12; break;
	case 1: data->font = a18; break;
	case 2: data->font = a24; break;
	case 3: data->font = a48; break;
	default: data->font = a12; break;
	}
}

//initializes character cache
void text_init_atlas(atlas *data, uint8 textsize)
{
	FT_GlyphSlot g = font.face->glyph;

	uint16 i = 0;
	float x = 0;

	data->h = 0;
	data->w = 0;
	data->c = (symbol *)calloc(1,128 * sizeof(symbol));

	if(data->c == NULL)
		error_handler(DE_ERROR_POINTER_NULL,",data->c in text_init_atlas()\n");

	FT_Set_Pixel_Sizes(font.face, 0, textsize);

	for(i = 32; i < 128; i++) {
		if(FT_Load_Char(font.face, i, FT_LOAD_RENDER)) {
			fprintf(stderr, "Loading character %c failed!\n", i);
			continue;
		}

		data->w += g->bitmap.width + 1;
		data->h = max(data->h, g->bitmap.rows);
	}

	glGenTextures(1, &data->tex);
	glBindTexture(GL_TEXTURE_2D, data->tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, data->w, data->h, 0, GL_ALPHA, GL_UNSIGNED_BYTE, 0);

	glPixelStoref(GL_UNPACK_ALIGNMENT, (GLfloat)1);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	for (i = 32; i < 128; i++){
		if(FT_Load_Char( font.face, i, FT_LOAD_RENDER))
			fprintf(stderr, "Loading character %c failed!\n", i);

		glTexSubImage2D(GL_TEXTURE_2D, 0, x, 0, g->bitmap.width, g->bitmap.rows, GL_ALPHA, GL_UNSIGNED_BYTE, g->bitmap.buffer);

		data->c[i].ax = g->advance.x >> 6;
		data->c[i].ay = g->advance.y >> 6;
		data->c[i].width = g->bitmap.width;
		data->c[i].height = g->bitmap.rows;
		data->c[i].left = g->bitmap_left;
		data->c[i].top = g->bitmap_top;
		data->c[i].x = x ;
		data->c[i].y = 0;
		data->c[i].mheight = g->face->size->metrics.height >> 6;
		x += g->bitmap.width + 1 ;
	}
}

//initializes the font the contains the characters and the system.
void text_init_font(char *fontname)
{
	if(FT_Init_FreeType(&library))
		fprintf(stderr, "Could not initialize freetype 2 library\n");

	if(!strcmp(fontname,""))
		font.fontfilename = comb_2str(".\\",FONTNAME);
	else
		font.fontfilename = comb_2str(".\\",fontname);

	if(FT_New_Face(library, font.fontfilename, 0, &font.face))
		fprintf(stderr, "Could not open font %s\n", font.fontfilename);

	text_init_atlas(&a12,12);
	text_init_atlas(&a18,18);
	text_init_atlas(&a24,24);
	text_init_atlas(&a48,48);
}

void text_resize_buffer(vertex_array **buffer, size_t size)
{
	vertex_array *data;

	if(buffer == NULL)
		return;//TODO: add error handler here.

	data = (vertex_array *)realloc(*buffer, size * sizeof(vertex_array));

	if (data == NULL)
		error_handler(DE_ERROR_POINTER_NULL,",data in text_resize_buffers()\n");

	*buffer = data;
}

void create_text_vertex(text *str, widget *control)//must be done every text change
{
	float x2,x;
	float y2,y;
	char *p = str->data;
	float w, h;
	float texty;
	float texx,texy,texw,texh;
	GLuint *index;
	uint16 i = 0;
	uint32 total_line_width = 0;
	uint16 amount = 0;

	control->actualpos.x = control->pos.x + control->parent->actualpos.x;
	control->actualpos.y = control->pos.y + control->parent->actualpos.y;
	texty = str->xy.y + control->actualpos.y + str->font.h;
	x = str->xy.x + control->actualpos.x;
	y = texty;
	str->lines = 1;
	str->textheight = 0;

	str->buf.data = (vertex_array *)calloc(1, 16 * sizeof(vertex_array));

	if(str->buf.data == NULL)
		error_handler(DE_ERROR_POINTER_NULL,",str->buf.data in create_text_vertex()\n");

	str->buf.size = 16;
	str->buf.count = 0;

	while(*p){
		texx = (float)str->font.c[*p].x / str->font.w;
		texw = (float)(str->font.c[*p].x + str->font.c[*p].width)/ str->font.w;
		texy = (float)str->font.c[*p].y / str->font.h;
		texh = (float)(str->font.c[*p].y + str->font.c[*p].height)/ str->font.h;

		x2 = (x + str->font.c[*p].left);
		y2 = (y - str->font.c[*p].top);

		w = str->font.c[*p].width;
		h = str->font.c[*p].height;
		y = texty;

		//Moves a certain amount of space After the symbol based on glyph set size.
		x += str->font.c[*p].ax;
		y += str->font.c[*p].ay;

		if(control->action & WIDGET_IS_MULTI_LINED){
			amount++;

			if(str->font.c[*p].left >= 0)
				total_line_width += (str->font.c[*p].ax + str->font.c[*p].left);
			else
				total_line_width += (str->font.c[*p].ax);

			if (!str->font.c[*p].width || !str->font.c[*p].height){
				if(total_line_width <= str->font.c[*p].ax){
					total_line_width = 0;
					x = str->xy.x + control->actualpos.x + str->offsetx;
				}else{
					if(total_line_width >= str->width - str->offsetx){
						str->lines++;
						total_line_width = 0;
						amount = 0;
						texty = str->xy.y + control->actualpos.y + (str->font.h * str->lines);
						x = str->xy.x + control->actualpos.x + str->offsetx;
						y = texty + str->offsety;
						str->textheight = str->font.h * str->lines;
					}
				}

				p++; continue;
			}
			else{
				if(amount >= str->maxcharspl && str->maxcharspl > 0 || total_line_width >= str->width - str->offsetx){
					str->lines++;
					total_line_width = 0;
					amount = 0;
					texty = str->xy.y + control->actualpos.y + (str->font.h * str->lines);
					x = str->xy.x + control->actualpos.x + str->offsetx;
					y = texty + str->offsety;
					str->textheight = str->font.h * str->lines;
				}
			}

			if(str->textheight >= str->height - str->offsety)
				break;
		}
		else{
			if (!str->font.c[*p].width || !str->font.c[*p].height){
				p++; continue;
			}
		}

		if(str->buf.count >= str->buf.size){
			str->buf.size = str->buf.size * 2;
			text_resize_buffer(&str->buf.data, str->buf.size);
		}

		/*index 0*/
		str->buf.data[str->buf.count].vertex[0].u = texx; str->buf.data[str->buf.count].vertex[0].v = texy;
		str->buf.data[str->buf.count].vertex[0].x = x2; str->buf.data[str->buf.count].vertex[0].y = y2;
		str->buf.data[str->buf.count].vertex[0].r = str->col.r; str->buf.data[str->buf.count].vertex[0].b = str->col.b; str->buf.data[str->buf.count].vertex[0].g = str->col.g; str->buf.data[str->buf.count].vertex[0].a = str->col.a;

		/*index 1*/
		str->buf.data[str->buf.count].vertex[1].u = texw; str->buf.data[str->buf.count].vertex[1].v = texy;
		str->buf.data[str->buf.count].vertex[1].x = x2 + w; str->buf.data[str->buf.count].vertex[1].y = y2;
		str->buf.data[str->buf.count].vertex[1].r = str->col.r; str->buf.data[str->buf.count].vertex[1].b = str->col.b; str->buf.data[str->buf.count].vertex[1].g = str->col.g; str->buf.data[str->buf.count].vertex[1].a = str->col.a;

		/*index 2*/
		str->buf.data[str->buf.count].vertex[2].u = texw; str->buf.data[str->buf.count].vertex[2].v = texh;
		str->buf.data[str->buf.count].vertex[2].x = x2 + w; str->buf.data[str->buf.count].vertex[2].y = y2 + h;
		str->buf.data[str->buf.count].vertex[2].r = str->col.r; str->buf.data[str->buf.count].vertex[2].b = str->col.b; str->buf.data[str->buf.count].vertex[2].g = str->col.g; str->buf.data[str->buf.count].vertex[2].a = str->col.a;

		/*index 3*/
		str->buf.data[str->buf.count].vertex[3].u = texx; str->buf.data[str->buf.count].vertex[3].v = texh;
		str->buf.data[str->buf.count].vertex[3].x = x2; str->buf.data[str->buf.count].vertex[3].y = y2 + h;
		str->buf.data[str->buf.count].vertex[3].r = str->col.r; str->buf.data[str->buf.count].vertex[3].b = str->col.b; str->buf.data[str->buf.count].vertex[3].g = str->col.g; str->buf.data[str->buf.count].vertex[3].a = str->col.a;

		str->buf.count++;
		p++;
	}

	str->buf.isize = str->buf.count * 4;
	index = (GLuint *)calloc(1, next_power_of_two(str->buf.isize) * sizeof(GLuint));

	if(index == NULL)
		error_handler(DE_ERROR_POINTER_NULL,",index in create_text_vertex()\n");

	for( i = 0; i <= str->buf.isize; i++)
		index[i] = i;

	if(str->buf.buffer)
		glDeleteBuffers(1, &str->buf.buffer);
	if(str->buf.index)
		glDeleteBuffers(1, &str->buf.index);

	glGenBuffers(1,&str->buf.buffer);
	glGenBuffers(1,&str->buf.index);

	glBindBuffer(GL_ARRAY_BUFFER,str->buf.buffer);
	glBufferData(GL_ARRAY_BUFFER,str->buf.size *( 4 * sizeof(vertex_t)),str->buf.data,GL_STREAM_DRAW);//fill up the array with vertex and color-data

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,str->buf.index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, str->buf.isize * sizeof(GLuint),index,GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	free(index);
}

void update_text_vertex(text *str, widget *control)//must be done every text change
{
	float x2,x;
	float y2,y;
	char *p = str->data;
	float w, h;
	float texty;
	float texx,texy,texw,texh;
	GLuint *index;
	uint16 i = 0;
	uint32 total_line_width = 0;
	uint16 amount = 0;

	control->actualpos.x = control->pos.x + control->parent->actualpos.x;
	control->actualpos.y = control->pos.y + control->parent->actualpos.y;
	texty = str->xy.y + control->actualpos.y + str->font.h;
	x = str->xy.x + control->actualpos.x;
	y = texty;
	str->lines = 1;
	str->textheight = 0;

	if(str->buf.data && control->action & WIDGET_BUFFER_RESIZE){
		str->buf.size = str->buf.size * 2;
		text_resize_buffer(&str->buf.data, next_power_of_two(str->buf.size));
	}
	else{
		if(str->buf.data == NULL){
			str->buf.data = (vertex_array *)calloc(1, str->size * sizeof(vertex_array));

			if(str->buf.data == NULL)
				error_handler(DE_ERROR_POINTER_NULL,",str->buf.data in update_text_vertex()\n");

			str->buf.size = str->size;
		}
	}

	str->buf.count = 0;

	while(*p){
		texx = (float)str->font.c[*p].x / str->font.w;
		texw = (float)(str->font.c[*p].x + str->font.c[*p].width)/ str->font.w;
		texy = (float)str->font.c[*p].y / str->font.h;
		texh = (float)(str->font.c[*p].y + str->font.c[*p].height)/ str->font.h;

		x2 = (x + str->font.c[*p].left);
		y2 = (y - str->font.c[*p].top);

		w = str->font.c[*p].width;
		h = str->font.c[*p].height;
		y = texty;

		//Moves a certain amount of space After the symbol based on glyph set size.
		x += str->font.c[*p].ax;
		y += str->font.c[*p].ay;

		if(control->action & WIDGET_IS_MULTI_LINED){
			amount++;

			if(str->font.c[*p].left >= 0)
				total_line_width += (str->font.c[*p].ax + str->font.c[*p].left);
			else
				total_line_width += (str->font.c[*p].ax);

			if (!str->font.c[*p].width || !str->font.c[*p].height){
				if(total_line_width <= str->font.c[*p].ax){
					total_line_width = 0;
					x = str->xy.x + control->actualpos.x + str->offsetx;
				}else{
					if(total_line_width >= str->width - str->offsetx){
						str->lines++;
						total_line_width = 0;
						amount = 0;
						texty = str->xy.y + control->actualpos.y + (str->font.h * str->lines);
						x = str->xy.x + control->actualpos.x + str->offsetx;
						y = texty + str->offsety;
						str->textheight = str->font.h * str->lines;
					}
				}

				p++; continue;
			}
			else{
				if(amount >= str->maxcharspl && str->maxcharspl > 0 || total_line_width >= str->width - str->offsetx){
					str->lines++;
					total_line_width = 0;
					amount = 0;
					texty = str->xy.y + control->actualpos.y + (str->font.h * str->lines);
					x = str->xy.x + control->actualpos.x + str->offsetx;
					y = texty + str->offsety;
					str->textheight = str->font.h * str->lines;
				}
			}

			if(str->textheight >= str->height - str->offsety)
				break;
		}
		else{
			if (!str->font.c[*p].width || !str->font.c[*p].height){
				p++; continue;
			}
		}

		if(str->buf.count >= str->buf.size){
			str->buf.size = str->buf.size * 2;
			text_resize_buffer(&str->buf.data, str->buf.size);
		}

		/*index 0*/
		str->buf.data[str->buf.count].vertex[0].u = texx; str->buf.data[str->buf.count].vertex[0].v = texy;
		str->buf.data[str->buf.count].vertex[0].x = x2; str->buf.data[str->buf.count].vertex[0].y = y2;
		str->buf.data[str->buf.count].vertex[0].r = str->col.r; str->buf.data[str->buf.count].vertex[0].b = str->col.b; str->buf.data[str->buf.count].vertex[0].g = str->col.g; str->buf.data[str->buf.count].vertex[0].a = str->col.a;

		/*index 1*/
		str->buf.data[str->buf.count].vertex[1].u = texw; str->buf.data[str->buf.count].vertex[1].v = texy;
		str->buf.data[str->buf.count].vertex[1].x = x2 + w; str->buf.data[str->buf.count].vertex[1].y = y2;
		str->buf.data[str->buf.count].vertex[1].r = str->col.r; str->buf.data[str->buf.count].vertex[1].b = str->col.b; str->buf.data[str->buf.count].vertex[1].g = str->col.g; str->buf.data[str->buf.count].vertex[1].a = str->col.a;

		/*index 2*/
		str->buf.data[str->buf.count].vertex[2].u = texw; str->buf.data[str->buf.count].vertex[2].v = texh;
		str->buf.data[str->buf.count].vertex[2].x = x2 + w; str->buf.data[str->buf.count].vertex[2].y = y2 + h;
		str->buf.data[str->buf.count].vertex[2].r = str->col.r; str->buf.data[str->buf.count].vertex[2].b = str->col.b; str->buf.data[str->buf.count].vertex[2].g = str->col.g; str->buf.data[str->buf.count].vertex[2].a = str->col.a;

		/*index 3*/
		str->buf.data[str->buf.count].vertex[3].u = texx; str->buf.data[str->buf.count].vertex[3].v = texh;
		str->buf.data[str->buf.count].vertex[3].x = x2; str->buf.data[str->buf.count].vertex[3].y = y2 + h;
		str->buf.data[str->buf.count].vertex[3].r = str->col.r; str->buf.data[str->buf.count].vertex[3].b = str->col.b; str->buf.data[str->buf.count].vertex[3].g = str->col.g; str->buf.data[str->buf.count].vertex[3].a = str->col.a;

		str->buf.count++;
		p++;
	}

	str->buf.isize = str->buf.count * 4;
	index = (GLuint *)calloc(1, next_power_of_two(str->buf.isize) * sizeof(GLuint));

	if(index == NULL)
		error_handler(DE_ERROR_POINTER_NULL,",index in update_text_vertex()\n");

	for( i = 0; i <= str->buf.isize; i++)
		index[i] = i;

	if(str->buf.buffer == 0)
		glGenBuffers(1, &str->buf.buffer);

	if(str->buf.index == 0)
		glGenBuffers(1, &str->buf.index);

	glBindBuffer(GL_ARRAY_BUFFER,str->buf.buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,str->buf.index);

	if(control->action & WIDGET_BUFFER_RESIZE){
		glBufferData(GL_ARRAY_BUFFER,str->buf.size *( 4 * sizeof(vertex_t)),NULL,GL_STREAM_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, str->buf.isize * sizeof(GLuint),NULL,GL_STATIC_DRAW);
		control->action &= ~(WIDGET_BUFFER_RESIZE);
	}
	glBufferData(GL_ARRAY_BUFFER,str->buf.size *( 4 * sizeof(vertex_t)),str->buf.data,GL_STREAM_DRAW);//fill up the array with vertex and color-data
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, str->buf.isize * sizeof(GLuint),index,GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	free(index);
}

void text_position_mupdate(text *str, widget *control)
{
	float x2,x;
	float y2,y;
	char *p = str->data;
	float w, h;
	float texty;
	uint16 count = 0;
	uint32 total_line_width = 0;
	uint16 amount = 0;

	control->actualpos.x = control->pos.x + control->parent->actualpos.x;
	control->actualpos.y = control->pos.y + control->parent->actualpos.y;
	texty = str->xy.y + control->actualpos.y + str->font.h;
	x = str->xy.x + control->actualpos.x + str->offsetx;
	y = texty + str->offsety;
	str->lines = 1;
	str->textheight = 0;
	if(p){
		while(*p){
			x2 = (x + str->font.c[*p].left);
			y2 = (y - str->font.c[*p].top);

			h = str->font.c[*p].height;
			w = str->font.c[*p].width;

			y = texty + str->offsety; //reset Y to the original other wise text becomes out of control XD.

			//Moves a certain amount of space After the symbol based on glyph set size.
			x += str->font.c[*p].ax;
			y += str->font.c[*p].ay;

			amount++;
			if(str->font.c[*p].left >= 0)
				total_line_width += (str->font.c[*p].ax + str->font.c[*p].left);
			else
				total_line_width += (str->font.c[*p].ax);

			if (!str->font.c[*p].width || !str->font.c[*p].height){
				if(total_line_width <= str->font.c[*p].ax){
					total_line_width = 0;
					x = str->xy.x + control->actualpos.x + str->offsetx;
				}else{
					if(total_line_width >= str->width - str->offsetx){
						str->lines++;
						total_line_width = 0;
						amount = 0;
						texty = str->xy.y + control->actualpos.y + (str->font.h * str->lines);
						x = str->xy.x + control->actualpos.x + str->offsetx;
						y = texty + str->offsety;
						str->textheight = str->font.h * str->lines;
					}
				}

				p++; continue;
			}
			else{
				if(amount >= str->maxcharspl && str->maxcharspl > 0 || total_line_width >= str->width - str->offsetx){
					str->lines++;
					total_line_width = 0;
					amount = 0;
					texty = str->xy.y + control->actualpos.y + (str->font.h * str->lines);
					x = str->xy.x + control->actualpos.x + str->offsetx;
					y = texty + str->offsety;
					str->textheight = str->font.h * str->lines;
				}
			}

			if(str->textheight >= str->height - str->offsety)
				break;

			/*index 0*/
			str->buf.data[count].vertex[0].x = x2; str->buf.data[count].vertex[0].y = y2;
			/*index 1*/
			str->buf.data[count].vertex[1].x = x2 + w; str->buf.data[count].vertex[1].y = y2;
			/*index 2*/
			str->buf.data[count].vertex[2].x = x2 + w; str->buf.data[count].vertex[2].y = y2 + h;
			/*index 3*/
			str->buf.data[count].vertex[3].x = x2; str->buf.data[count].vertex[3].y = y2 + h;

			count++;
			p++;
		}

		glBindBuffer(GL_ARRAY_BUFFER,str->buf.buffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, str->buf.size *(4 * sizeof(struct vertex_t)), &str->buf.data[0]);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	if(control->parent->type == CONTROL_TEXTBOX){
		textbox *text = (textbox *)control->control;

		if(str->textheight <= str->height - str->offsety){
			if(str->count > 0){
				text->cursorbuffer.data[0].x = x + str->font.c[*p].left + 2;
				text->cursorbuffer.data[0].y = y;
				text->cursorbuffer.data[1].x = x + str->font.c[*p].left + 2;
				text->cursorbuffer.data[1].y = y - text->cursorheight + 2;
			}
			else{
				text->cursorbuffer.data[0].x = str->xy.x + control->actualpos.x + str->offsetx + str->offsetx;
				text->cursorbuffer.data[0].y = str->xy.y + control->actualpos.y + str->font.h + str->offsety;
				text->cursorbuffer.data[1].x = str->xy.x + control->actualpos.x + str->offsetx + str->offsetx;
				text->cursorbuffer.data[1].y = str->xy.y + control->actualpos.y + str->font.h + str->offsety - text->cursorheight + 2;
			}

			glBindBuffer(GL_ARRAY_BUFFER,text->cursorbuffer.buffer);
			glBufferSubData(GL_ARRAY_BUFFER, 0, 2 * sizeof(struct line_vertex_t), &text->cursorbuffer.data[0]);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
	}
}

void text_position_supdate(text *str, widget *control)
{
	float x2,x;
	float y2,y;
	char *p;
	float w, h;
	float texty;
	uint16 count = 0;
	uint16 i = 0;

	control->actualpos.x = control->pos.x + control->parent->actualpos.x;
	control->actualpos.y = control->pos.y + control->parent->actualpos.y;
	texty = str->xy.y + control->actualpos.y + str->font.h;
	x = str->xy.x + control->actualpos.x + str->offsetx;
	y = texty + str->offsety;

	if(control->action & WIDGET_IS_PASSWORD){
		p = (char *)calloc(1, str->size * sizeof(char));

		for( i = 0; i < str->count; i++)
			p[i]= '*';
	}
	else
		p = str->data;

	if(p){
		p += str->displayoffset;
		while(*p){
			x2 = (x + str->font.c[*p].left);
			y2 = (y - str->font.c[*p].top);

			h = str->font.c[*p].height;
			w = str->font.c[*p].width;

			y = texty + str->offsety; //reset Y to the original other wise text becomes out of control XD.

			//Moves a certain amount of space After the symbol based on glyph set size.

			x += str->font.c[*p].ax;
			y += str->font.c[*p].ay;

			if (!str->font.c[*p].width || !str->font.c[*p].height){
				p++; continue;
			}

			/*index 0*/
			str->buf.data[count].vertex[0].x = x2; str->buf.data[count].vertex[0].y = y2;
			/*index 1*/
			str->buf.data[count].vertex[1].x = x2 + w; str->buf.data[count].vertex[1].y = y2;
			/*index 2*/
			str->buf.data[count].vertex[2].x = x2 + w; str->buf.data[count].vertex[2].y = y2 + h;
			/*index 3*/
			str->buf.data[count].vertex[3].x = x2; str->buf.data[count].vertex[3].y = y2 + h;

			count++;
			p++;
		}

		glBindBuffer(GL_ARRAY_BUFFER,str->buf.buffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, str->buf.size *(4 * sizeof(struct vertex_t)), &str->buf.data[0]);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	if(control->parent->type == CONTROL_TEXTBOX){
		textbox *text = (textbox *)control->control;

		if(str->count > 0){
			text->cursorbuffer.data[0].x = x + str->font.c[*p].left + 2;
			text->cursorbuffer.data[0].y = y;
			text->cursorbuffer.data[1].x = x + str->font.c[*p].left + 2;
			text->cursorbuffer.data[1].y = y - text->cursorheight + 2;
		}
		else{
			text->cursorbuffer.data[0].x = str->xy.x + control->actualpos.x + str->offsetx + str->offsetx;
			text->cursorbuffer.data[0].y = str->xy.y + control->actualpos.y + str->font.h + str->offsety;
			text->cursorbuffer.data[1].x = str->xy.x + control->actualpos.x + str->offsetx + str->offsetx;
			text->cursorbuffer.data[1].y = str->xy.y + control->actualpos.y + str->font.h + str->offsety - text->cursorheight + 2;
		}

		glBindBuffer(GL_ARRAY_BUFFER,text->cursorbuffer.buffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, 2 * sizeof(struct line_vertex_t), &text->cursorbuffer.data[0]);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

void text_draw(text *str)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glBindTexture(GL_TEXTURE_2D, str->font.tex);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, str->buf.index);
	glIndexPointer(GL_UNSIGNED_INT,sizeof(GLuint),0);

	glBindBuffer(GL_ARRAY_BUFFER, str->buf.buffer);
	glVertexPointer(2, GL_FLOAT, sizeof(struct vertex_t), 0);
	glTexCoordPointer(2, GL_FLOAT, sizeof(struct vertex_t), (GLvoid *)offsetof(struct vertex_t, u));
	glColorPointer(4, GL_FLOAT, sizeof(struct vertex_t), (GLvoid *)offsetof(struct vertex_t, r));

	glDrawElements(GL_QUADS, str->buf.isize ,GL_UNSIGNED_INT,0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void string_resize(text *str, uint32 size)
{
	char *zdata;
	int i = 0;

	if(str->data == NULL)
		return;//TODO: add error handler here.

	zdata = (char *)realloc(str->data, next_power_of_two(size) * sizeof(char));

	if(zdata == NULL)
		error_handler(DE_ERROR_POINTER_NULL,",zdata in string_resize()\n");

	for(i = str->count; i <= str->size; i++)
		zdata[i] = '\0';

	str->data = zdata;
}

void stextbox_text_update(text *str, widget *control){
	float x2,x;
	float y2,y;
	char *p;
	char *password;
	float w, h;
	float texty;
	float texx,texy,texw,texh;
	GLuint *index;
	uint16 i = 0;
	textbox *text = (textbox *)control->control;

	control->actualpos.x = control->pos.x + control->parent->actualpos.x;
	control->actualpos.y = control->pos.y + control->parent->actualpos.y;
	texty = str->xy.y + control->actualpos.y + str->font.h;
	x = str->xy.x + control->actualpos.x + str->offsetx;
	y = texty + str->offsety;

	if(str->buf.data && control->action & WIDGET_BUFFER_RESIZE){
		str->buf.size = str->buf.size * 2;
		text_resize_buffer(&str->buf.data, next_power_of_two(str->buf.size));
	}
	else{
		if(str->buf.data == NULL){
			str->buf.data = (vertex_array *)calloc(1, str->size * sizeof(vertex_array));

			if(str->buf.data == NULL)
				error_handler(DE_ERROR_POINTER_NULL,",str->buf.data in stextbox_text_update()\n");

			str->buf.size = str->size;
		}
	}

	str->buf.count = 0;

	if(control->action & WIDGET_IS_PASSWORD){
		password = (char *)calloc(1, str->size * sizeof(char));

		for( i = 0; i < str->count; i++)
			password[i]= '*';

		p = password;
	}
	else
		p = str->data;

	p += str->displayoffset;

	while(*p){
		texx = (float)str->font.c[*p].x / str->font.w;
		texw = (float)(str->font.c[*p].x + str->font.c[*p].width)/ str->font.w;
		texy = (float)str->font.c[*p].y / str->font.h;
		texh = (float)(str->font.c[*p].y + str->font.c[*p].height)/ str->font.h;

		x2 = (x + str->font.c[*p].left);
		y2 = (y - str->font.c[*p].top);

		w = str->font.c[*p].width;
		h = str->font.c[*p].height;

		y = texty + str->offsety; //reset Y to the original other wise text becomes out of control XD.

		//Moves a certain amount of space After the symbol based on glyph set size.
		x += str->font.c[*p].ax;
		y += str->font.c[*p].ay;

		if (!str->font.c[*p].width || !str->font.c[*p].height){
			p++; continue;
		}

		if(str->buf.count >= str->buf.size){
			str->buf.size = str->buf.size * 2;
			text_resize_buffer(&str->buf.data, str->buf.size);
		}

		/*index 0*/
		str->buf.data[str->buf.count].vertex[0].u = texx; str->buf.data[str->buf.count].vertex[0].v = texy;
		str->buf.data[str->buf.count].vertex[0].x = x2; str->buf.data[str->buf.count].vertex[0].y = y2;
		str->buf.data[str->buf.count].vertex[0].r = str->col.r; str->buf.data[str->buf.count].vertex[0].b = str->col.b; str->buf.data[str->buf.count].vertex[0].g = str->col.g; str->buf.data[str->buf.count].vertex[0].a = str->col.a;

		/*index 1*/
		str->buf.data[str->buf.count].vertex[1].u = texw; str->buf.data[str->buf.count].vertex[1].v = texy;
		str->buf.data[str->buf.count].vertex[1].x = x2 + w; str->buf.data[str->buf.count].vertex[1].y = y2;
		str->buf.data[str->buf.count].vertex[1].r = str->col.r; str->buf.data[str->buf.count].vertex[1].b = str->col.b; str->buf.data[str->buf.count].vertex[1].g = str->col.g; str->buf.data[str->buf.count].vertex[1].a = str->col.a;

		/*index 2*/
		str->buf.data[str->buf.count].vertex[2].u = texw; str->buf.data[str->buf.count].vertex[2].v = texh;
		str->buf.data[str->buf.count].vertex[2].x = x2 + w; str->buf.data[str->buf.count].vertex[2].y = y2 + h;
		str->buf.data[str->buf.count].vertex[2].r = str->col.r; str->buf.data[str->buf.count].vertex[2].b = str->col.b; str->buf.data[str->buf.count].vertex[2].g = str->col.g; str->buf.data[str->buf.count].vertex[2].a = str->col.a;

		/*index 3*/
		str->buf.data[str->buf.count].vertex[3].u = texx; str->buf.data[str->buf.count].vertex[3].v = texh;
		str->buf.data[str->buf.count].vertex[3].x = x2; str->buf.data[str->buf.count].vertex[3].y = y2 + h;
		str->buf.data[str->buf.count].vertex[3].r = str->col.r; str->buf.data[str->buf.count].vertex[3].b = str->col.b; str->buf.data[str->buf.count].vertex[3].g = str->col.g; str->buf.data[str->buf.count].vertex[3].a = str->col.a;

		str->buf.count++;

		if(str->buf.count >= str->count)
			break;

		p++;
	}

	str->buf.isize = str->buf.count * 4;
	index = (GLuint *)calloc(1, next_power_of_two(str->buf.isize) * sizeof(GLuint));

	if(index == NULL)
		error_handler(DE_ERROR_POINTER_NULL,",index in stextbox_text_update()\n");

	for( i = 0; i <= str->buf.isize; i++)
		index[i] = i;

	if(str->buf.buffer == 0)
		glGenBuffers(1, &str->buf.buffer);

	if(str->buf.index == 0)
		glGenBuffers(1, &str->buf.index);

	glBindBuffer(GL_ARRAY_BUFFER,str->buf.buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,str->buf.index);

	if(control->action & WIDGET_BUFFER_RESIZE){
		glBufferData(GL_ARRAY_BUFFER,str->buf.size *( 4 * sizeof(vertex_t)),NULL,GL_STREAM_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, str->buf.isize * sizeof(GLuint),NULL,GL_STATIC_DRAW);
		control->action &= ~(WIDGET_BUFFER_RESIZE);
	}
	glBufferData(GL_ARRAY_BUFFER,str->buf.size *( 4 * sizeof(vertex_t)),str->buf.data,GL_STREAM_DRAW);//fill up the array with vertex and color-data
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, str->buf.isize * sizeof(GLuint),index,GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	if(str->count > 0){
		text->cursorbuffer.data[0].x = x + str->font.c[*p].left + 2;
		text->cursorbuffer.data[0].y = y;
		text->cursorbuffer.data[1].x = x + str->font.c[*p].left + 2;
		text->cursorbuffer.data[1].y = y - text->cursorheight + 2;
	}
	else{
		text->cursorbuffer.data[0].x = str->xy.x + control->actualpos.x + str->offsetx + str->offsetx;
		text->cursorbuffer.data[0].y = str->xy.y + control->actualpos.y + str->font.h + str->offsety;
		text->cursorbuffer.data[1].x = str->xy.x + control->actualpos.x + str->offsetx + str->offsetx;
		text->cursorbuffer.data[1].y = str->xy.y + control->actualpos.y + str->font.h + str->offsety - text->cursorheight + 2;
	}

	glBindBuffer(GL_ARRAY_BUFFER,text->cursorbuffer.buffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 2 * sizeof(struct line_vertex_t), &text->cursorbuffer.data[0]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	if(control->action & WIDGET_IS_PASSWORD)
		free(password);

	p = NULL;
	free(index);
}

void mtextbox_text_update(text *str, widget *control){
	float x2,x;
	float y2,y;
	char *p = str->data;
	float w, h;
	float texty;
	float texx,texy,texw,texh;
	GLuint *index;
	GLuint index2[2] = {0,1};
	uint16 i = 0;
	float total_line_width = 0;
	uint16 amount = 0;
	textbox *text = (textbox *)control->control;

	control->actualpos.x = control->pos.x + control->parent->actualpos.x;
	control->actualpos.y = control->pos.y + control->parent->actualpos.y;
	texty = str->xy.y + control->actualpos.y + str->font.h;
	x = str->xy.x + control->actualpos.x + str->offsetx;
	y = texty + str->offsety;
	str->lines = 1;
	str->textheight = 0;
	if(str->buf.data && control->action & WIDGET_BUFFER_RESIZE){
		str->buf.size = str->buf.size * 2;
		text_resize_buffer(&str->buf.data, next_power_of_two(str->buf.size));
	}
	else{
		if(str->buf.data == NULL){
			str->buf.data = (vertex_array *)calloc(1, str->size * sizeof(vertex_array));
			str->buf.size = str->size;
		}
	}

	str->buf.count = 0;

	while(*p){
		texx = (float)str->font.c[*p].x / str->font.w;
		texw = (float)(str->font.c[*p].x + str->font.c[*p].width)/ str->font.w;
		texy = (float)str->font.c[*p].y / str->font.h;
		texh = (float)(str->font.c[*p].y + str->font.c[*p].height)/ str->font.h;

		x2 = (x + str->font.c[*p].left);
		y2 = (y - str->font.c[*p].top);

		w = str->font.c[*p].width;
		h = str->font.c[*p].height;

		y = texty + str->offsety; //reset Y to the original other wise text becomes out of control XD.

		//Moves a certain amount of space After the symbol based on glyph set size.

		x += str->font.c[*p].ax;
		y += str->font.c[*p].ay;

		amount++;
		if(str->font.c[*p].left >= 0)
			total_line_width += (str->font.c[*p].ax + str->font.c[*p].left);
		else
			total_line_width += (str->font.c[*p].ax);

		if (!str->font.c[*p].width || !str->font.c[*p].height){
			if(total_line_width <= str->font.c[*p].ax){
				total_line_width = 0;
				x = str->xy.x + control->actualpos.x + str->offsetx;
			}else{
				if(total_line_width >= str->width - str->offsetx){
					str->lines++;
					total_line_width = 0;
					amount = 0;
					texty = str->xy.y + control->actualpos.y + (str->font.h * str->lines);
					x = str->xy.x + control->actualpos.x + str->offsetx;
					y = texty + str->offsety;
					str->textheight = str->font.h * str->lines;
				}
			}

			p++; continue;
		}
		else{
			if(amount >= str->maxcharspl && str->maxcharspl > 0 || total_line_width >= str->width - str->offsetx){
				str->lines++;
				total_line_width = 0;
				amount = 0;
				texty = str->xy.y + control->actualpos.y + (str->font.h * str->lines);
				x = str->xy.x + control->actualpos.x + str->offsetx;
				y = texty + str->offsety;
				str->textheight = str->font.h * str->lines;
			}
		}

		if(str->textheight >= str->height - str->offsety)
			break;

		if(str->buf.count >= str->buf.size){
			str->buf.size = str->buf.size * 2;
			text_resize_buffer(&str->buf.data, str->buf.size);
		}

		/*index 0*/
		str->buf.data[str->buf.count].vertex[0].u = texx; str->buf.data[str->buf.count].vertex[0].v = texy;
		str->buf.data[str->buf.count].vertex[0].x = x2; str->buf.data[str->buf.count].vertex[0].y = y2;
		str->buf.data[str->buf.count].vertex[0].r = str->col.r; str->buf.data[str->buf.count].vertex[0].b = str->col.b; str->buf.data[str->buf.count].vertex[0].g = str->col.g; str->buf.data[str->buf.count].vertex[0].a = str->col.a;

		/*index 1*/
		str->buf.data[str->buf.count].vertex[1].u = texw; str->buf.data[str->buf.count].vertex[1].v = texy;
		str->buf.data[str->buf.count].vertex[1].x = x2 + w; str->buf.data[str->buf.count].vertex[1].y = y2;
		str->buf.data[str->buf.count].vertex[1].r = str->col.r; str->buf.data[str->buf.count].vertex[1].b = str->col.b; str->buf.data[str->buf.count].vertex[1].g = str->col.g; str->buf.data[str->buf.count].vertex[1].a = str->col.a;

		/*index 2*/
		str->buf.data[str->buf.count].vertex[2].u = texw; str->buf.data[str->buf.count].vertex[2].v = texh;
		str->buf.data[str->buf.count].vertex[2].x = x2 + w; str->buf.data[str->buf.count].vertex[2].y = y2 + h;
		str->buf.data[str->buf.count].vertex[2].r = str->col.r; str->buf.data[str->buf.count].vertex[2].b = str->col.b; str->buf.data[str->buf.count].vertex[2].g = str->col.g; str->buf.data[str->buf.count].vertex[2].a = str->col.a;

		/*index 3*/
		str->buf.data[str->buf.count].vertex[3].u = texx; str->buf.data[str->buf.count].vertex[3].v = texh;
		str->buf.data[str->buf.count].vertex[3].x = x2; str->buf.data[str->buf.count].vertex[3].y = y2 + h;
		str->buf.data[str->buf.count].vertex[3].r = str->col.r; str->buf.data[str->buf.count].vertex[3].b = str->col.b; str->buf.data[str->buf.count].vertex[3].g = str->col.g; str->buf.data[str->buf.count].vertex[3].a = str->col.a;

		str->buf.count++;

		if(str->buf.count >= str->count)
			break;

		p++;
	}

	str->buf.isize = str->buf.count * 4;
	index = (GLuint *)calloc(1, next_power_of_two(str->buf.isize) * sizeof(GLuint));

	if(index == NULL)
		error_handler(DE_ERROR_POINTER_NULL,",index in mtextbox_text_update()\n");

	for( i = 0; i <= str->buf.isize; i++)
		index[i] = i;

	if(str->buf.buffer == 0)
		glGenBuffers(1, &str->buf.buffer);

	if(str->buf.index == 0)
		glGenBuffers(1, &str->buf.index);

	glBindBuffer(GL_ARRAY_BUFFER,str->buf.buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,str->buf.index);

	if(control->action & WIDGET_BUFFER_RESIZE){
		glBufferData(GL_ARRAY_BUFFER,str->buf.size *( 4 * sizeof(vertex_t)),NULL,GL_STREAM_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, str->buf.isize * sizeof(GLuint),NULL,GL_STATIC_DRAW);
		control->action &= ~(WIDGET_BUFFER_RESIZE);
	}
	glBufferData(GL_ARRAY_BUFFER,str->buf.size *( 4 * sizeof(vertex_t)),str->buf.data,GL_STREAM_DRAW);//fill up the array with vertex and color-data
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, str->buf.isize * sizeof(GLuint),index,GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	if(str->textheight <= str->height - str->offsety){
		if(str->count > 0){
			text->cursorbuffer.data[0].x = x + str->font.c[*p].left + 2;
			text->cursorbuffer.data[0].y = y;
			text->cursorbuffer.data[1].x = x + str->font.c[*p].left + 2;
			text->cursorbuffer.data[1].y = y - text->cursorheight + 2;
		}
		else{
			text->cursorbuffer.data[0].x = str->xy.x + control->actualpos.x + str->offsetx + str->offsetx;
			text->cursorbuffer.data[0].y = str->xy.y + control->actualpos.y + str->font.h + str->offsety;
			text->cursorbuffer.data[1].x = str->xy.x + control->actualpos.x + str->offsetx + str->offsetx;
			text->cursorbuffer.data[1].y = str->xy.y + control->actualpos.y + str->font.h + str->offsety - text->cursorheight + 2;
		}

		glBindBuffer(GL_ARRAY_BUFFER,text->cursorbuffer.buffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, 2 * sizeof(struct line_vertex_t), &text->cursorbuffer.data[0]);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	free(index);
}