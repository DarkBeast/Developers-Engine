//ignore this for now its crap.

/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
*           Freetype library 2
******************************************************************************/
#include "text.h"
#include "function.h"
#include "globals.h"
#include "program_path.h"
#include <glfw.h>
#include <string.h>

fonts font;
FT_Library library;
symbol *character; //holds individual character bitmap data etc.

//gets the character array if really needed lol
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
}

//initilizes character cache
void text_init_atlas(void) {
	uint16 i = 0;

	FT_Set_Char_Size( font.face, /* handle to face object */
		0, /* char_width in 1/64th of points */
		4*64, /* char_height in 1/64th of points */
		300, /* horizontal device resolution */
		300); /* vertical device resolution */

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

		glGenTextures(1, &character[i].tex);
		glBindTexture(GL_TEXTURE_2D, character[i].tex);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, character[i].glyph_bitmap->bitmap.width , character[i].glyph_bitmap->bitmap.rows , 0, GL_ALPHA, GL_UNSIGNED_BYTE, character[i].glyph_bitmap->bitmap.buffer);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		/* Linear filtering usually looks best for text */
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}
}

//initlizes the font the contains the characters and the system.
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

//draws the text from a widget.
void text_draw(text *str, widget *parent)
{
	char *p;
	uint16 x, y;
	uint16 x2, y2, w, h;
	uint16 textx,texty;

	textx = str->xy.x + parent->actualpos.x;
	texty = str->xy.y + parent->actualpos.y;

	x = textx;
	y = texty + 12;

	/* Loop through all characters in the string*/
	//TODO: optimize with Size and Current data type For faster optimizations on string rendering.

	glEnable (GL_TEXTURE_2D); // in our Drawing we initialize Drawing before we draw and disable when we are done.

	glColor4f(str->col.r , str->col.g , str->col.b , str->col.a);// set the image color properties, 1 being highest 0.0000 being lowest

	for(p = str->data; *p; p++){
		/* Try to load and render the character */

		glBindTexture(GL_TEXTURE_2D, character[*p].tex);

		x2 = (x + (uint16)character[*p].glyph_bitmap->left);

		y2 = (y - (uint16)character[*p].glyph_bitmap->top) ;

		w = ((uint16)character[*p].glyph_bitmap->bitmap.width + str->resize);

		h = ((uint16)character[*p].glyph_bitmap->bitmap.rows + str->resize);

		y = texty; //reset Y to the original other wise text becomes out of control XD.

		//Moves a certain amount of space After the symbol based on glyph set size.

		x +=  (character[*p].advancex >> 6) + str->resize;
		y += (character[*p].advancey >> 6) + 12;

		if (!character[*p].glyph_bitmap->bitmap.width || !character[*p].glyph_bitmap->bitmap.rows)
			continue;

		glBegin (GL_TRIANGLE_STRIP);

		glTexCoord2i (0, 0);
		glVertex2i (x2, y2);

		glTexCoord2i (1, 0);
		glVertex2i (x2 + w, y2);

		glTexCoord2i (0, 1);
		glVertex2i (x2, y2 + h);

		glTexCoord2i (1, 1);
		glVertex2i (x2 + w, y2 + h);

		glEnd ();
	}
}

//suppost to get the width and heigth but atm incorrect.
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