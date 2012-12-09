//ignore this for now its crap.

/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
*           Freetype library 2
******************************************************************************/
#include "text.h"
#include "function.h"
#include "globals.h"
#include "program_path.h"
#include "render.h"
#include <glfw.h>
#include <string.h>

fonts font;
FT_Library library;
symbol *character; //holds individual character bitmap data etc.

void settext(text *data, int x, int y, int size, int red, int blue, int green, int alpha, char* string)
{
	//set data
	data->resize = size;

	data->col.a = alpha;
	data->col.r = red;
	data->col.b = blue;
	data->col.g = green;

	data->xy.x = x;
	data->xy.y = y;
	data->data = string;
}

void initatlas() {
	int x = 0, y = 0;
	int i = 0;
	//printf("caching symbols.\n");

	FT_Set_Char_Size( font.face, /* handle to face object */
		0, /* char_width in 1/64th of points */
		4*64, /* char_height in 1/64th of points */
		300, /* horizontal device resolution */
		300 ); /* vertical device resolution */
	//FT_Set_Pixel_Sizes( font.face, 0, 16 ); /* font size */

	for (i = 0; i <= 255; i++)
	{
		if(FT_Load_Char( font.face, i, FT_LOAD_DEFAULT ))
		{
			//TODO: add error handler
			fprintf(stderr, "Loading character %c failed!\n", i);
		}

		if(FT_Get_Glyph( font.face->glyph, &character[i].glyph ))
		{
			//TODO: add error handler
			fprintf(stderr, "Getting Glyph %c failed!\n", i);
		}

		if ( character[i].glyph->format != FT_GLYPH_FORMAT_BITMAP )
		{
			if(FT_Glyph_To_Bitmap( &character[i].glyph, FT_RENDER_MODE_NORMAL, 0, 1 ))
			{
				//TODO: add error handler
				fprintf(stderr, "making character %c into a bitmap failed!\n", i);
			}
		}
		character[i].advancex = font.face->glyph->advance.x;
		character[i].advancey = font.face->glyph->advance.y;
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
	//printf("caching symbols is done\n");
}

void initfont(char *fontname)
{
	if(FT_Init_FreeType(&library))
	{
		//TODO: add error handler
		fprintf(stderr, "Could not init freetype library\n");
	}

	if(fontname == "")
	{
		font.fontfilename = getpath(FONTNAME);
	}
	else
	{
		font.fontfilename = getpath(fontname);
	}

	if(FT_New_Face(library, font.fontfilename, 0, &font.face))
	{
		//TODO: add error handler
		fprintf(stderr, "Could not open font %s\n", font.fontfilename);
	}

	character = (symbol *)calloc(1,0x0010FFFF * sizeof(symbol));
	initatlas();
}

void rendertext(text *str)
{
	char *p;
	int x, y;
	int x2, y2, w, h;

	if( str->resize > 1)
	{
		x = str->xy.x;
		y = str->xy.y + str->resize + 3;
	}
	else
	{
		x = str->xy.x + str->resize;
		y = str->xy.y;
	}
	/* Loop through all characters in the string*/
	//TODO: optimize with Size and Current data type For faster optimizations on string rendering.

	glEnable (GL_TEXTURE_2D); // in our Drawing we initialize Drawing before we draw and disable when we are done.

	glColor4f(str->col.r, str->col.g, str->col.b, str->col.a);// set the image color properties, 1 being highest 0.0000 being lowest

	for(p = str->data; *p; p++) {
		/* Try to load and render the character */

		glBindTexture(GL_TEXTURE_2D, character[*p].tex);

		x2 = (x + character[*p].glyph_bitmap->left);

		y2 = (y - character[*p].glyph_bitmap->top) ;

		w = (character[*p].glyph_bitmap->bitmap.width + str->resize);

		h = (character[*p].glyph_bitmap->bitmap.rows + str->resize);

		y = str->xy.y; //reset Y to the original other wise text becomes out of control XD.

		//Moves a certain ammont of space After the symbol based on glyph set size.
		if( str->resize > 1)
		{
			x += (character[*p].advancex >> 6)+ str->resize;
			y += (character[*p].advancey >> 6)+ str->resize +3;
		}
		else
		{
			x += character[*p].advancex >> 6;
			y += character[*p].advancey >> 6;
		}

		if (!character[*p].glyph_bitmap->bitmap.width || !character[*p].glyph_bitmap->bitmap.rows)
			continue;

		glBegin (GL_TRIANGLE_STRIP);

		glTexCoord2i (1, 1);
		glVertex2i (x2 + w, y2 + h);

		glTexCoord2i (1, 0);
		glVertex2i (x2 + w, y2);

		glTexCoord2i (0, 1);
		glVertex2i (x2, y2 + h);

		glTexCoord2i (0, 0);
		glVertex2i (x2, y2);

		glEnd ();
	}
}