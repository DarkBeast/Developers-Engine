/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/

#include <glfw.h>
#include <stdio.h>
#include <stdlib.h>
#include "error.h"
#include "globals.h"
#include "image.h"
#include "integer.h"
#include "render.h"

screensize thescreen;

screensize getscreensize(void)
{
	return thescreen;
}

int getscreenheight(void)
{
	return thescreen.height;
}

int getscreenwidth(void)
{
	return thescreen.width;
}

void drawpush(void)
{
	glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
}

void drawstatereset(void)
{
	glDisable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);
	glDisable (GL_DEPTH_TEST);
	glEnable(GL_ALPHA_TEST);// makes images seen behind the transparent part of an Alpha image.
	glEnable(GL_TEXTURE_2D);
	//glEnable(GL_COLOR_MATERIAL);//sets a vertex color, migth slow down system some.
	glEnable(GL_BLEND); //Enable alpha blending for better image qulity, turn off for better performace.

	glAlphaFunc ( GL_GREATER, (GLclampf)0.2 );
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//Set the blend function

	glMatrixMode(GL_MODELVIEW);
}

void setdrawview(int x, int y, int swidth, int sheight)
{
	//Tell OpenGL how to convert from coordinates to pixel values
	glViewport( x, y, swidth, sheight );

	glMatrixMode( GL_PROJECTION ); //Switch to setting the camera perspective
	//Set the camera perspective
	glLoadIdentity(); //reset the camera

	glOrtho (0, swidth, sheight, 0, 0, 1);
}

//sets the screen, GLFW , and the Screen Title
void initscreen(int swidth, int sheight, int mode)
{
	// Initialize GLFW
	if( !glfwInit() )
		rendererror(GLFWINIT_ERROR);

	// Finally we can Open an OpenGL window
	if(!glfwOpenWindow ( swidth, sheight, 0,0,0,0,0,0, mode ))
		rendererror(GLFWWIN_ERROR);

	glfwSetWindowTitle( TITLE );//Sets the Windows Name
	thescreen.height = sheight;
	thescreen.width = swidth;
}

void clearscreen(int red, int blue, int green, int alpha)
{
	glClearColor(red / 255.f, green / 255.f, blue / 255.f, alpha / 255.f);
	glClear(GL_COLOR_BUFFER_BIT);
}

//Called when the window is resized
void GLFWCALL handleresize(int width,int height)
{
	//Tell OpenGL how to convert from coordinates to pixel values
	glViewport( 0, 0, width, height );

	glMatrixMode( GL_PROJECTION ); //Switch to setting the camera perspective
	//Set the camera perspective
	glLoadIdentity(); //reset the camera

	glOrtho (0, width, height, 0, 0, 1);

	//set the new screen size from the resize
	thescreen.height = height;
	thescreen.width = width;
}

void initimage(image* img)
{
	img->bpp = 0;
	img->format = GL_RGBA;
	img->height = 0;
	img ->width = 0;
	img ->texid = 0;
	img->reload = TRUE;
}

void reloadimage(image* img)
{
	img->reload = TRUE;
	img ->texid = 0;
}

void loadimage(char *name, image* img)
{
	if(img->reload == TRUE)//check if its a new image or the first load.
	{
		// Read image from file
		load_png( name, img);

		glGenTextures( 1, &img ->texid );
		glBindTexture( GL_TEXTURE_2D, img ->texid );
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D( GL_TEXTURE_2D, 0, img ->format,
			img ->width, img ->height, 0, img ->format,
			GL_UNSIGNED_BYTE, (void*) img->pixels );

		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		img->reload = FALSE;
	}
}

void draw(image* img, vector2i vecpos, vector2i imgpos,int width, int height)
{
	int x2,x1;
	int y2,y1;

	x1 = imgpos.x / img->width;
	x2 = (imgpos.x + width) / img->width;
	y1 = imgpos.y / img->height;
	y2 = (imgpos.y +height) / img->height;

	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();

	glTranslatef(0.375, 0.375, 0);

	glEnable (GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, img ->texid);
	glColor4f(1, 1, 1, 1);// set the image color properties, 1 being highest 0.0000 being lowest

	glBegin (GL_QUADS);

	glTexCoord2i (x1, y2);
	glVertex2i (vecpos.x, vecpos.y);

	glTexCoord2i (x2, y2);
	glVertex2i (vecpos.x + width, vecpos.y);

	glTexCoord2i (x2, y1);
	glVertex2i (vecpos.x + width, vecpos.y +height);

	glTexCoord2i (x1, y1);
	glVertex2i (vecpos.x, vecpos.y +height);

	glEnd ();
}

void drawwidget(widget* widget) //draws all the widgets on the canvas.
{
	int x2,x1;
	int y2,y1;

	x1 = widget->imgpos.x / widget->img.width;
	x2 = (widget->imgpos.x + widget->width) / widget->img.width;
	y1 = widget->imgpos.y / widget->img.height;
	y2 = (widget->imgpos.y +widget->height) / widget->img.height;

	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();

	glTranslatef(0.375, 0.375, 0);

	glEnable (GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, widget->img.texid);
	glColor4f(1, 1, 1, 1);// set the image color properties, 1 being highest 0.0000 being lowest

	glBegin (GL_QUADS);

	glTexCoord2i (x1, y2);
	glVertex2i (widget->pos.x, widget->pos.y);

	glTexCoord2i (x2, y2);
	glVertex2i (widget->pos.x + widget->width, widget->pos.y);

	glTexCoord2i (x2, y1);
	glVertex2i (widget->pos.x + widget->width, widget->pos.y + widget->height);

	glTexCoord2i (x1, y1);
	glVertex2i (widget->pos.x, widget->pos.y + widget->height);

	glEnd ();
}