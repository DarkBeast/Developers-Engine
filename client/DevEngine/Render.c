/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/

#include <glfw.h>
#include <stdlib.h>
#include "error.h"
#include "globals.h"
#include "image.h"
#include "integer.h"
#include "render.h"
#include "bool.h"
screen_size the_screen;

screen_size get_screen_size(void)
{
	return the_screen;
}

int get_screen_height(void)
{
	return the_screen.height;
}

int get_screen_width(void)
{
	return the_screen.width;
}

void draw_push(void)
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

void draw_state_reset(void)
{
	glDisable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);
	glDisable (GL_DEPTH_TEST);
	glEnable(GL_ALPHA_TEST);// makes images seen behind the transparent part of an Alpha image.
	glEnable(GL_TEXTURE_2D);
	//glEnable(GL_COLOR_MATERIAL);//sets a vertex color, migth slow down system some.
	glEnable(GL_BLEND); //Enable alpha blending for better image qulity, turn off for better performace.
	glAlphaFunc ( GL_GREATER, (GLclampf)0.2);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//Set the blend function

	glMatrixMode(GL_MODELVIEW);
}

void set_draw_view(int x, int y, int swidth, int sheight)
{
	//Tell OpenGL how to convert from coordinates to pixel values
	glViewport( x, y, swidth, sheight);

	glMatrixMode( GL_PROJECTION); //Switch to setting the camera perspective
	//Set the camera perspective
	glLoadIdentity(); //reset the camera

	glOrtho (0, swidth, sheight, 0, 0, 1);
}

//sets the screen, GLFW , and the Screen Title
void init_screen(int swidth, int sheight, int mode)
{
	// Initialize GLFW
	if( !glfwInit())
		render_error(ERROR_GLFWINIT_ERROR);

	// Finally we can Open an OpenGL window
	if(!glfwOpenWindow ( swidth, sheight, 0,0,0,0,0,0, mode))
		render_error(ERROR_GLFWWIN_ERROR);

	glfwSetWindowTitle( TITLE);//Sets the Windows Name
	the_screen.height = sheight;
	the_screen.width = swidth;
}

void clear_screen(int red, int blue, int green, int alpha)
{
	glClearColor(red / 255.f, green / 255.f, blue / 255.f, alpha / 255.f);
	glClear(GL_COLOR_BUFFER_BIT);
}

//Called when the window is resized
void GLFWCALL handle_resize(int width,int height)
{
	//Tell OpenGL how to convert from coordinates to pixel values
	glViewport( 0, 0, width, height);

	glMatrixMode( GL_PROJECTION); //Switch to setting the camera perspective
	//Set the camera perspective
	glLoadIdentity(); //reset the camera

	glOrtho (0, width, height, 0, 0, 1);

	//set the new screen size from the resize
	the_screen.height = height;
	the_screen.width = width;
}

void init_image(image* img)
{
	img->bpp = 0;
	img->format = GL_RGBA;
	img->height = 0;
	img ->width = 0;
	img ->texid = 0;

	if(img->pixels != NULL){
		free(img->pixels);
		img->pixels = NULL;
	}
}

void load_image(char *name, image* img)
{
	// Read image from file
	init_image(img);
	load_png( name, img);

	glGenTextures( 1, &img ->texid);
	glBindTexture( GL_TEXTURE_2D, img ->texid);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D( GL_TEXTURE_2D, 0, img ->format,
		img ->width, img ->height, 0, img ->format,
		GL_UNSIGNED_BYTE, (void*) img->pixels);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
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

void draw_widget(widget* control) //draws all the image widgets on the canvas.
{
	float x2,x1;
	float y2,y1;

	control->actualpos.x = control->pos.x + control->parent->pos.x;
	control->actualpos.y = control->pos.y + control->parent->pos.y;

	x1 =   (float) control->imgpos.x  / control->img.width;
	x2 =   (float) (control->imgpos.x + control->width) / control->img.width;
	y1 =   (float) control->imgpos.y / control->img.height;
	y2 =   (float)(control->imgpos.y +control->height) /control->img.height;

	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();

	glTranslatef(0.375, 0.375, 0);

	glEnable (GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, control->img.texid);
	glColor4f(1, 1, 1, 1);// set the image color properties, 1 being highest 0.0000 being lowest

	glBegin (GL_QUADS);

	glTexCoord2f (x1, y2);
	glVertex2i (control->actualpos.x, control->actualpos.y);

	glTexCoord2f (x2, y2);
	glVertex2i (control->actualpos.x + control->width, control->actualpos.y);

	glTexCoord2f (x2, y1);
	glVertex2i (control->actualpos.x + control->width, control->actualpos.y + control->height);

	glTexCoord2f (x1, y1);
	glVertex2i (control->actualpos.x, control->actualpos.y + control->height);

	glEnd ();
}