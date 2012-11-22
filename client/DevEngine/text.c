/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
*           Freetype library 2
******************************************************************************/
#include "text.h"
#include "globals.h"
#include "program_path.h"
#include "render.h"
#include <glfw.h>

fonts font;

void initfont(const char *fontname)
{
	if(FT_Init_FreeType(&font.library))
	{
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

	if(FT_New_Face(font.library, font.fontfilename, 0, &font.face))
	{
		fprintf(stderr, "Could not open font %s\n", font.fontfilename);
	}
}

void rendertext(text *str)
{
	const char *p;
	uint16 x, y;
	uint16 x2, y2, w, h;
	FT_GlyphSlot g = font.face->glyph;
	x = str->xy.x;
	y = str->xy.y;

	/* Create a texture that will be used to hold one "glyph" */

	FT_Set_Char_Size( font.face, /* handle to face object */
		0, /* char_width in 1/64th of points */
		str->size*64, /* char_height in 1/64th of points */
		300, /* horizontal device resolution */
		300 ); /* vertical device resolution */
	glGenTextures(1, &font.tex);

	glBindTexture(GL_TEXTURE_2D, font.tex);

	/* We require 1 byte alignment when uploading texture data */

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	/* Clamping to edges is important to prevent artifacts when scaling */

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	/* Linear filtering usually looks best for text */

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	/* Loop through all characters */
	glColor4f(str->col.r, str->col.g, str->col.b, str->col.a);// set the image color properties, 1 being highest 0.0000 being lowest

	for(p = str->text; *p; p++) {
		/* Try to load and render the character */

		if(FT_Load_Char(font.face, *p, FT_LOAD_RENDER))

			continue;

		/* Upload the "bitmap", which contains an 8-bit grayscale image, as an alpha texture */

		glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, g->bitmap.width, g->bitmap.rows, 0, GL_ALPHA, GL_UNSIGNED_BYTE, g->bitmap.buffer);

		/* Calculate the vertex and texture coordinates */

		x2 = x ;

		y2 = y ;

		w = g->bitmap.width ;

		h = g->bitmap.rows;

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

		/* Advance the cursor to the start of the next character */

		x += (g->advance.x >> 6) * str->hw.x;

		y += (g->advance.y >> 6) * str->hw.y;
	}
}

void inittext()
{
}