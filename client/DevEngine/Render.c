/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis    
******************************************************************************/

#include <glfw.h>
#include <stdio.h>
#include <stdlib.h>
#include "Error.h"
#include "globals.h"
#include "Image.h"
#include "integer.h"
#include "Render.h"


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
	glDisable (GL_DEPTH_TEST);
	glEnable(GL_ALPHA_TEST);// makes images seen behind the transparent part of an Alpha image.
	glEnable(GL_TEXTURE_2D);
	//glEnable(GL_COLOR_MATERIAL);//sets a vertex color, migth slow down system some.
	glEnable(GL_BLEND); //Enable alpha blending for better image qulity, turn off for better performace.

	glAlphaFunc ( GL_GREATER, 0.2 );
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//Set the blend function

	glMatrixMode(GL_MODELVIEW);

}

void SetDrawView(int x, int y, int swidth, int sheight)
{
	//Tell OpenGL how to convert from coordinates to pixel values
	glViewport( x, y, swidth, sheight );

	glMatrixMode( GL_PROJECTION ); //Switch to setting the camera perspective
	//Set the camera perspective
	glLoadIdentity(); //reset the camera

	glOrtho (0, swidth, sheight, 0, 0, 1);

}

//sets the screen, GLFW , and the Screen Title
void InitScreen(int swidth, int sheight, int mode)
{
	// Initialize GLFW
	if( !glfwInit() )
		RenderError(GLFWINIT_ERROR);

	// Finally we can Open an OpenGL window
	if(!glfwOpenWindow ( swidth, sheight, 0,0,0,0,0,0, mode ))
		RenderError(GLFWWIN_ERROR);

	glfwSetWindowTitle( TITLE );//Sets the Windows Name
	screen.Height = sheight;
	screen.Width = swidth;

}

void ClearScreen(int red, int blue, int green, int alpha)
{
	glClearColor(red / 255.f, green / 255.f, blue / 255.f, alpha / 255.f);
	glClear(GL_COLOR_BUFFER_BIT);
}

//Called when the window is resized
void GLFWCALL handleResize(int width,int height)
{
	//Tell OpenGL how to convert from coordinates to pixel values
	glViewport( 0, 0, width, height );

	glMatrixMode( GL_PROJECTION ); //Switch to setting the camera perspective
	//Set the camera perspective
	glLoadIdentity(); //reset the camera

	glOrtho (0, width, height, 0, 0, 1);

	//set the new screen size from the resize
	screen.Height = height;
	screen.Width = width;

}

void initimage(Image* img)
{
	img->BytesPerPixel = 0;
	img->Format = GL_RGBA;
	img->Height = 0;
	img ->Width = 0;
	img ->texID = NULL;
	img->reload = true;
}

void reloadimage(Image* img)
{
	img->reload = true;
	img ->texID = NULL;
}

int LoadImage(char *name, Image* img, int type)
{

	if(img->reload == true)//check if its a new image or the first load.
	{
		GLuint Texture;

		// Read image from file
		if( load_png( name, img) )
		{
			return false;
		}

		glGenTextures( 1, &img ->texID );
		glBindTexture( GL_TEXTURE_2D, img ->texID );
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D( GL_TEXTURE_2D, 0, img ->Format,
			img ->Width, img ->Height, 0, img ->Format,
			GL_UNSIGNED_BYTE, (void*) img->pixels );


		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		img->reload = false;
	}
	return true;
}

int Draw(Image* img, Vector2i vecpos, Vector2f imgpos,int width, int height)
{
	float X2,X1;
	float Y2,Y1;

	X1 = imgpos.x / img->Width;
	X2 = (imgpos.x + width) / img->Width;
	Y1 = imgpos.y / img->Height;
	Y2 = (imgpos.y +height) / img->Height;

	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();

	glTranslatef(0.375, 0.375, 0);

	glEnable (GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, img ->texID);
	glColor4f(1, 1, 1, 1);// set the image color properties, 1 being highest 0.0000 being lowest

	glBegin (GL_QUADS);


	glTexCoord2f (X1, Y2); 
	glVertex2i (vecpos.x, vecpos.y);

	glTexCoord2f (X2, Y2); 
	glVertex2i (vecpos.x + width, vecpos.y);

	glTexCoord2f (X2, Y1); 
	glVertex2i (vecpos.x + width, vecpos.y +height);

	glTexCoord2f (X1, Y1); 
	glVertex2i (vecpos.x, vecpos.y +height);

	glEnd ();


	return true;
}
