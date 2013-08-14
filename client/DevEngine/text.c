/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
*           Freetype library 2
******************************************************************************/
#include <GL/glew.h>
#include "text.h"
#include "error.h"
#include "function.h"
#include "globals.h"
#include "program_path.h"
#include <string.h>

fonts font;
FT_Library library;
symbol *character; //holds individual character bitmap data etc.
GLuint texture;

//gets the character array if really needed.
symbol* text_get_character_array(void)
{
	return character;
}

//sets the text of a objects Control.
void text_set(text *text_data, uint16 x, uint16 y, uint8 size, uint8 red,uint8 blue,uint8 green,uint8 alpha, char *string)
{
	text_data->resize = size;
	text_data->col.a = alpha /255.f;
	text_data->col.r = red /255.f;
	text_data->col.b = blue /255.f;
	text_data->col.g = green /255.f;
	text_data->xy.x = x;
	text_data->xy.y = y;
	text_data->data = string;
	text_data->textbuf.buffer = NULL;
	text_data->textbuf.index_buffer = 0;
	text_data->textbuf.vertex_buffer = 0;
}

//initializes character cache
void text_init_atlas(void)
{
	uint16 i = 0;
	GLubyte *image;
	uint16 x = 0,y = 0;

	image = (GLubyte *)calloc(1,256 * 128 * 2 * sizeof(GLubyte));

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, 256, 128, 0, GL_ALPHA, GL_UNSIGNED_BYTE, image);

	FT_Set_Char_Size( font.face,0,3*64,275,275);

	for (i = 0; i <= 127; i++){
		if(FT_Load_Char( font.face, i, FT_LOAD_DEFAULT)){
			//TODO: add error handler
			fprintf(stderr, "Loading character %c failed!\n", i);
		}

		if(FT_Get_Glyph( font.face->glyph, &character[i].glyph)){
			//TODO: add error handler
			fprintf(stderr, "Getting Glyph %c failed!\n", i);
		}

		if ( character[i].glyph->format != FT_GLYPH_FORMAT_BITMAP){
			if(FT_Glyph_To_Bitmap( &character[i].glyph, FT_RENDER_MODE_NORMAL, 0, 1)){
				//TODO: add error handler
				fprintf(stderr, "making character %c into a bitmap failed!\n", i);
			}
		}
		character[i].advancex = (int16)font.face->glyph->advance.x;
		character[i].advancey = (int16)font.face->glyph->advance.y;
		character[i].glyph_bitmap = (FT_BitmapGlyph)character[i].glyph;
		character[i].x = x * 15;
		character[i].y = y * 15;

		++x;

		if(x >= 16)
		{
			x = 0;
			++y;
		}

		//glBindTexture(GL_TEXTURE_2D, texture);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexSubImage2D(GL_TEXTURE_2D, 0, character[i].x, character[i].y, character[i].glyph_bitmap->bitmap.width, character[i].glyph_bitmap->bitmap.rows, GL_ALPHA, GL_UNSIGNED_BYTE, character[i].glyph_bitmap->bitmap.buffer);

		//glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, character[i].glyph_bitmap->bitmap.width , character[i].glyph_bitmap->bitmap.rows , 0, GL_ALPHA, GL_UNSIGNED_BYTE, character[i].glyph_bitmap->bitmap.buffer);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		/* Linear filtering usually looks best for text */
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}
}

//initializes the font the contains the characters and the system.
void text_init_font(char *fontname)
{
	if(FT_Init_FreeType(&library)){
		//TODO: add error handler
		fprintf(stderr, "Could not init freetype 2 library\n");
	}

	if(!strcmp(fontname,"")){
		font.fontfilename = get_path(FONTNAME);
	}
	else{
		font.fontfilename = get_path(fontname);
	}

	if(FT_New_Face(library, font.fontfilename, 0, &font.face)){
		//TODO: add error handler
		fprintf(stderr, "Could not open font %s\n", font.fontfilename);
	}

	character = (symbol *)calloc(1,127 * sizeof(symbol));
	text_init_atlas();
}

void text_resize_buffer(vertex_array **buffer, size_t size)
{
	vertex_array *data;

	if(buffer == NULL)
		return;//TODO: add error handler here.

	data = (vertex_array *)realloc(*buffer, size * sizeof(vertex_array));

	if (data == NULL)
		fatal_error(ERROR_POINTER_NULL);

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

	texty = str->xy.y + control->actualpos.y;
	x = str->xy.x + control->actualpos.x;
	y = texty + 12;

	str->textbuf.buffer = (vertex_array *)calloc(1, 16 * sizeof(vertex_array));
	str->size = 16;
	str->count = 0;

	while(*p){
		texx = (float)character[*p].x / 256;
		texw = (float)(character[*p].x + character[*p].glyph_bitmap->bitmap.width)/ 256;
		texy = (float)character[*p].y / 128;
		texh = (float)(character[*p].y + character[*p].glyph_bitmap->bitmap.rows)/ 128;
		x2 = (x + character[*p].glyph_bitmap->left);
		y2 = (y - character[*p].glyph_bitmap->top);

		switch(*p){
		case '.':
			h = (character[*p].glyph_bitmap->bitmap.rows + (str->resize * .1));
			w = (character[*p].glyph_bitmap->bitmap.width + (str->resize * .1));

			if(str->resize > 15){
				y2 += (str->resize * .16);
			}
			else{
				y2 += (str->resize * .08);
			}
			break;
		case 'l':
		case 'j':
		case 'i':
		case 'I':
		case 't':
			w = (character[*p].glyph_bitmap->bitmap.width + (str->resize * .045));
			h = (character[*p].glyph_bitmap->bitmap.rows + (str->resize * .25));
			break;
		case '-':
		case '_':
			w = (character[*p].glyph_bitmap->bitmap.width + (str->resize * .05));
			h = (character[*p].glyph_bitmap->bitmap.rows + (str->resize * .05));
			y2 += (str->resize * .13);
			break;
		default:
			w = (character[*p].glyph_bitmap->bitmap.width + (str->resize * .10));
			h = (character[*p].glyph_bitmap->bitmap.rows + (str->resize * .25));
			break;
		}

		y = texty; //reset Y to the original other wise text becomes out of control XD.

		//Moves a certain amount of space After the symbol based on glyph set size.

		x += (character[*p].advancex >> 6) + (str->resize * .20);
		y += (character[*p].advancey >> 6) + 12;

		if (!character[*p].glyph_bitmap->bitmap.width || !character[*p].glyph_bitmap->bitmap.rows){
			p++; continue;
		}

		if(str->count >= str->size){
			str->size = str->size * 2;
			text_resize_buffer(&str->textbuf.buffer, str->size);
		}

		/*index 0*/
		str->textbuf.buffer[str->count].vertex[0].u = texx; str->textbuf.buffer[str->count].vertex[0].v = texy;
		str->textbuf.buffer[str->count].vertex[0].x = x2; str->textbuf.buffer[str->count].vertex[0].y = y2;
		str->textbuf.buffer[str->count].vertex[0].r = str->col.r; str->textbuf.buffer[str->count].vertex[0].b = str->col.b; str->textbuf.buffer[str->count].vertex[0].g = str->col.g; str->textbuf.buffer[str->count].vertex[0].a = str->col.a;

		/*index 1*/
		str->textbuf.buffer[str->count].vertex[1].u = texw; str->textbuf.buffer[str->count].vertex[1].v = texy;
		str->textbuf.buffer[str->count].vertex[1].x = x2 + w; str->textbuf.buffer[str->count].vertex[1].y = y2;
		str->textbuf.buffer[str->count].vertex[1].r = str->col.r; str->textbuf.buffer[str->count].vertex[1].b = str->col.b; str->textbuf.buffer[str->count].vertex[1].g = str->col.g; str->textbuf.buffer[str->count].vertex[1].a = str->col.a;

		/*index 2*/
		str->textbuf.buffer[str->count].vertex[2].u = texw; str->textbuf.buffer[str->count].vertex[2].v = texh;
		str->textbuf.buffer[str->count].vertex[2].x = x2 + w; str->textbuf.buffer[str->count].vertex[2].y = y2 + h;
		str->textbuf.buffer[str->count].vertex[2].r = str->col.r; str->textbuf.buffer[str->count].vertex[2].b = str->col.b; str->textbuf.buffer[str->count].vertex[2].g = str->col.g; str->textbuf.buffer[str->count].vertex[2].a = str->col.a;

		/*index 3*/
		str->textbuf.buffer[str->count].vertex[3].u = texx; str->textbuf.buffer[str->count].vertex[3].v = texh;
		str->textbuf.buffer[str->count].vertex[3].x = x2; str->textbuf.buffer[str->count].vertex[3].y = y2 + h;
		str->textbuf.buffer[str->count].vertex[3].r = str->col.r; str->textbuf.buffer[str->count].vertex[3].b = str->col.b; str->textbuf.buffer[str->count].vertex[3].g = str->col.g; str->textbuf.buffer[str->count].vertex[3].a = str->col.a;
		str->count++;
		p++;
	}

	str->max = str->count * 4;
	index = (GLuint *)calloc(1, next_power_of_two(str->max) * sizeof(GLuint));

	for( i = 0; i <= str->max; i++){
		index[i] = i;
	}

	str->textbuf.vbuffer_count = str->count * 4;
	str->textbuf.vbuffer_size = str->size * 4;

	glGenBuffers(1,&str->textbuf.vertex_buffer);
	glGenBuffers(1,&str->textbuf.index_buffer);

	glBindBuffer(GL_ARRAY_BUFFER,str->textbuf.vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER,str->size *( 4 * sizeof(vertex_t)),str->textbuf.buffer,GL_STREAM_DRAW);//fill up the array with vertex and color-data

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,str->textbuf.index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, str->max * sizeof(GLuint),index,GL_STATIC_DRAW);//this one with indices
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void text_position_update(text *str, widget *parent)
{
	float x2,x;
	float y2,y;
	char *p = str->data;
	float w, h;
	float texty;
	uint16 count = 0;

	texty = str->xy.y + parent->actualpos.y;
	x = str->xy.x + parent->actualpos.x;
	y = texty + 12;

	while(*p){
		x2 = (x + character[*p].glyph_bitmap->left);
		y2 = (y - character[*p].glyph_bitmap->top);

		switch(*p){
		case '.':
			h = (character[*p].glyph_bitmap->bitmap.rows + (str->resize * .1));
			w = (character[*p].glyph_bitmap->bitmap.width + (str->resize * .1));

			if(str->resize > 15){
				y2 += (str->resize * .16);
			}
			else{
				y2 += (str->resize * .08);
			}
			break;
		case 'l':
		case 'j':
		case 'i':
		case 'I':
		case 't':
			w = (character[*p].glyph_bitmap->bitmap.width + (str->resize * .045));
			h = (character[*p].glyph_bitmap->bitmap.rows + (str->resize * .25));
			break;
		case '-':
		case '_':
			w = (character[*p].glyph_bitmap->bitmap.width + (str->resize * .05));
			h = (character[*p].glyph_bitmap->bitmap.rows + (str->resize * .05));
			y2 += (str->resize * .13);
			break;
		default:
			w = (character[*p].glyph_bitmap->bitmap.width + (str->resize * .10));
			h = (character[*p].glyph_bitmap->bitmap.rows + (str->resize * .25));
			break;
		}

		y = texty; //reset Y to the original other wise text becomes out of control XD.

		//Moves a certain amount of space After the symbol based on glyph set size.

		x += (character[*p].advancex >> 6) + (str->resize * .20);
		y += (character[*p].advancey >> 6) + 12;

		if (!character[*p].glyph_bitmap->bitmap.width || !character[*p].glyph_bitmap->bitmap.rows){
			p++; continue;
		}

		/*index 0*/
		str->textbuf.buffer[count].vertex[0].x = x2; str->textbuf.buffer[count].vertex[0].y = y2;
		/*index 1*/
		str->textbuf.buffer[count].vertex[1].x = x2 + w; str->textbuf.buffer[count].vertex[1].y = y2;
		/*index 2*/
		str->textbuf.buffer[count].vertex[2].x = x2 + w; str->textbuf.buffer[count].vertex[2].y = y2 + h;
		/*index 3*/
		str->textbuf.buffer[count].vertex[3].x = x2; str->textbuf.buffer[count].vertex[3].y = y2 + h;
		count++;
		p++;
	}

	glBindBuffer(GL_ARRAY_BUFFER,str->textbuf.vertex_buffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, str->size *(4 * sizeof(struct vertex_t)), &str->textbuf.buffer[0]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void text_draw(text *str)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glBindTexture(GL_TEXTURE_2D, texture);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, str->textbuf.index_buffer);
	glIndexPointer(GL_UNSIGNED_INT,sizeof(GLuint),0);

	glBindBuffer(GL_ARRAY_BUFFER, str->textbuf.vertex_buffer);

	glVertexPointer(2, GL_FLOAT, sizeof(struct vertex_t), 0);
	glTexCoordPointer(2, GL_FLOAT, sizeof(struct vertex_t), (GLvoid *)offsetof(struct vertex_t, u));
	glColorPointer(4, GL_FLOAT, sizeof(struct vertex_t), (GLvoid *)offsetof(struct vertex_t, r));

	glDrawElements(GL_QUADS, str->max ,GL_UNSIGNED_INT,0);

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//suppose to get the width and height but at the moment incorrect.
vector2ui text_get_string_hw(char *string, uint8 resize)
{
	vector2ui size;
	char *p;

	size.y = 0;
	size.x = 0;

	for(p = string; *p; p++){
		size.y = max(size.y,(uint16)character[*p].glyph_bitmap->bitmap.rows);
		size.x += (character[*p].advancex >> 6) + (uint16)character[*p].glyph_bitmap->left + resize;
	}
	size.y += resize;

	return size;
}