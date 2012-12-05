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

#define MAXWIDTH 1024
fonts font;

atlas *a48;

void initcache(atlas *cache)
{
	cache = (atlas *)calloc(1,sizeof(atlas));
	memset(cache->character, NULL, sizeof cache->character);
	cache->height = 0;
	cache->width = 0;
	cache->tex = NULL;
}

void initatlas(FT_Face face, int height, atlas *cache) {
	FT_GlyphSlot g = face->glyph;

	int i = 0;
	int ox = 0;
	int oy = 0;
	int roww = 0;
	int rowh = 0;
	printf("started initlizing text");
	initcache(cache);
	//memset(arr, NULL, sizeof arr);
	FT_Set_Pixel_Sizes(face, 0, height);
	/* Find minimum size for a texture holding all visible unicode characters */
	for (i = 0; i <= 0x0010FFFF; i++) {
		if (FT_Load_Char(face, i, FT_LOAD_RENDER)) {
			fprintf(stderr, "Loading character %c failed!\n", i);
			continue;
		}
		if (roww + g->bitmap.width + 1 >= MAXWIDTH) {
			cache->width = max(cache->width, roww);
			cache->height += rowh;
			roww = 0;
			rowh = 0;
		}
		roww += g->bitmap.width + 1;
		rowh = max(rowh, g->bitmap.rows);
	}

	cache->width = max(cache->width, roww);
	cache->height += rowh;

	/* Create a texture that will be used to hold all ASCII glyphs */
	glGenTextures(1, &cache->tex);
	glBindTexture(GL_TEXTURE_2D, cache->tex);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, cache->width, cache->height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, 0);

	/* We require 1 byte alignment when uploading texture data */
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	/* Clamping to edges is important to prevent artifacts when scaling */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	/* Linear filtering usually looks best for text */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//get the bitmap. pixels of the glyph.
	rowh = 0;
	printf("50% done");
	for (i = 0; i < 0x0010FFFF; i++) {
		if (FT_Load_Char(face, i, FT_LOAD_RENDER)) {
			fprintf(stderr, "Loading character %c failed!\n", i);
			continue;
		}
		g = face->glyph;
		if (ox + g->bitmap.width + 1 >= MAXWIDTH) {
			oy += rowh;
			rowh = 0;
			ox = 0;
		}

		glGenTextures(1, &cache->character[i].tex);
		glBindTexture(GL_TEXTURE_2D, cache->character[i].tex);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexSubImage2D(GL_TEXTURE_2D, 0, ox, oy, g->bitmap.width, g->bitmap.rows, GL_ALPHA, GL_UNSIGNED_BYTE, g->bitmap.buffer);

		/* Clamping to edges is important to prevent artifacts when scaling */
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		/* Linear filtering usually looks best for text */
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		cache->character[i].advancex = g->advance.x >> 6;
		cache->character[i].advancey = g->advance.y >> 6;

		cache->character[i].image.width = g->bitmap.width;
		cache->character[i].image.rows = g->bitmap.rows;

		cache->character[i].bitmapleft = g->bitmap_left;
		cache->character[i].bitmaptop = g->bitmap_top;

		cache->character[i].offsetx = ox / cache->width;
		cache->character[i].offsety = oy / cache->height;

		rowh = max(rowh, g->bitmap.rows);
		ox += g->bitmap.width + 1;
	}
	printf("100% done");
	//finally set the textures for quick drawing.
	//for (i = 0; i < 0x0010FFFF; i++) {
	//	glGenTextures(1, &cache->character[i].tex);
	//	glBindTexture(GL_TEXTURE_2D, cache->character[i].tex);

	/* We require 1 byte alignment when uploading texture data */
	//	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	//	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, cache->character[i].image.width, cache->character[i].image.rows, 0, GL_ALPHA, GL_UNSIGNED_BYTE, cache->character[i].image.buffer);

	/* Linear filtering usually looks best for text */
	//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	//}
}

void initfont(char *fontname)
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

	a48  = (atlas *)calloc(1,  sizeof (atlas));
	//a24  = (atlas *)calloc(1,  sizeof (atlas));
	//a12  = (atlas *)calloc(1,  sizeof (atlas));

	initatlas(font.face, 48, a48);
	//initatlas(font.face, 24, a24);
	//initatlas(font.face, 12, a12);
}

void rendertext(text *str)
{
	char *p;
	uint16 x, y;
	uint16 x2, y2, w, h;
	x = str->xy.x;
	y = str->xy.y;

	/* Create a texture that will be used to hold one "glyph" */

	//FT_Set_Char_Size( font.face, /* handle to face object */
	//	0, /* char_width in 1/64th of points */
	//	str->size*64, /* char_height in 1/64th of points */
	//	300, /* horizontal device resolution */
	//	300 ); /* vertical device resolution */

	/* Loop through all characters */
	glEnable (GL_TEXTURE_2D);
	glColor4f(str->col.r, str->col.g, str->col.b, str->col.a);// set the image color properties, 1 being highest 0.0000 being lowest

	for(p = str->data; *p; p++) {
		/* Try to load and render the character */

		glBindTexture(GL_TEXTURE_2D, a48->character[*p].tex);

		/* Calculate the vertex and texture coordinates */

		x2 = x ;

		y2 = y ;

		w = a48->character[*p].image.width ;

		h = a48->character[*p].image.rows;

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

		x += (a48->character[*p].advancex >> 6) * str->hw.x;

		y += (a48->character[*p].advancey >> 6) * str->hw.y;
	}
}

void render_text(text *str, atlas *cache)
{
	int x, y,w, h,x2,x1,y2,y1;
	char *p;
	//x1 = imgpos.x / img->width;
	//x2 = (imgpos.x + width) / img->width;
	//y1 = imgpos.y / img->height;
	//y2 = (imgpos.y +height) / img->height;
	x = str->xy.x;
	y = str->xy.y;
	glBindTexture(GL_TEXTURE_2D, cache->tex);
	for(p = str->data; *p; p++) {
		x2 = x ;

		y2 = y ;

		w = cache->character[*p].image.width * str->xy.x;

		h = cache->character[*p].image.rows * str->xy.y;

		//float x2 =  x + cache->c[*p].bl * sx;
		// float y2 = -y - cache->c[*p].bt * sy;
		// float w = cache->c[*p].bw * sx;
		// float h = cache->c[*p].bh * sy;

		/* Advance the cursor to the start of the next character */
		x += cache->character[*p].advancex * str->xy.x;
		y += cache->character[*p].advancey * str->xy.y;

		/* Skip glyphs that have no pixels */
		if(!w || !h)
		{
			continue;
		}

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