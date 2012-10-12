
#include "Render.h"
#include "Error.h"
#include "Image.h"
#include "integer.h"
#include "globals.h"
#include <glfw.h>
#include <stdio.h>
#include <stdlib.h>

void DrawPush()
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

void DrawStateReset()
{

	glDisable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);
	glDisable(GL_ALPHA_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable (GL_DEPTH_TEST);
	//glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_BLEND);
	glMatrixMode(GL_MODELVIEW);

	SetDrawViewToScreen(screen);
}

void SetDrawView(int swidth, int sheight)
{
	//Tell OpenGL how to convert from coordinates to pixel values
	glViewport( 0, 0, swidth, sheight );

	glMatrixMode( GL_PROJECTION ); //Switch to setting the camera perspective
	//Set the camera perspective
	glLoadIdentity(); //reset the camera
	gluPerspective (90, swidth / sheight, 1, 9999);
}

void SetDrawViewToScreen(Screen s)
{
	//Tell OpenGL how to convert from coordinates to pixel values
	glViewport( 0, 0, s.Width, s.Height );

	glMatrixMode( GL_PROJECTION ); //Switch to setting the camera perspective
	//Set the camera perspective
	glLoadIdentity(); //reset the camera
	gluPerspective (90, s.Width / s.Height, 1, 9999);
}

//sets the screen, GLFW , and the Screen Title
void InitScreen(int swidth, int sheight, int mode)
{
	// Initialize GLFW
	if( !glfwInit() )
		RenderError(GLFWINIT_ERROR);

	// Finally we can Open an OpenGL window
	if(!glfwOpenWindow ( swidth, sheight, 1,0,0,0,0,0, mode ))
		RenderError(GLFWWIN_ERROR);

	glfwSetWindowTitle( TITLE );//Sets the Windows Name
	screen.Height = sheight;
	screen.Width = swidth;
}

void ClearScreen(int red, int blue, int green, int alpha)
{
	glClearColor(red / 255.f, green / 255.f, blue / 255.f, alpha / 255.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

//Called when the window is resized
void GLFWCALL handleResize(int width,int height)
{
	//Tell OpenGL how to convert from coordinates to pixel values
	glViewport( 0, 0, width, height );

	glMatrixMode( GL_PROJECTION ); //Switch to setting the camera perspective
	//Set the camera perspective
	glLoadIdentity(); //reset the camera
	gluPerspective (90, width / height, 1, 9999);
	screen.Height = height;
	screen.Width = width;
}

int LoadImage(char *name, Image* img, int type)
{
	GLuint Texture;

	img->BytesPerPixel =0;
	img->Format =  GL_RGBA;
	img->Height=0;
	img ->Width = 0;
	img ->pixels = NULL;
	img ->texID = NULL;

	// Read image from file
	if( load_png( name, img) )
	{
		return false;
	}

	glGenTextures( 1, &Texture );
	glBindTexture( GL_TEXTURE_2D, Texture );
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D( GL_TEXTURE_2D, 0, img ->Format,
		img ->Width, img ->Height, 0, img ->Format,
		GL_UNSIGNED_BYTE, (void*) img->pixels );


	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	img ->texID = Texture;

	return true;
}

int Draw(Image* img, SCoord pos)
{

	//glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	ClearScreen(0,0,0,0);
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
	//glTranslatef (0, 0, 0); /* eye position */

	glEnable (GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, img ->texID);

	glBegin (GL_QUADS);
	glColor4f(1.0f, 1.0f, 0.0f, -500.0f);
	glTexCoord3f (0.0f, 0.0f, -100.0f); /* lower left corner of image */
	glVertex3f (-10.0f, -10.0f, -100.0f);
	glTexCoord3f (1.0f, 0.0f, -100.0f); /* lower right corner of image */
	glVertex3f (10.0f, -10.0f, -100.0f);
	glTexCoord3f (1.0f, 1.0f, -100.0f); /* upper right corner of image */
	glVertex3f (10.0f, 10.0f, -100.0f);
	glTexCoord3f (0.0f, 1.0f, -100.0f); /* upper left corner of image */
	glVertex3f (-10.0f, 10.0f, -100.0f);
	glEnd ();

	return true;
}
