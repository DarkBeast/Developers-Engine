


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
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_ALPHA_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glMatrixMode(GL_MODELVIEW);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	SetDrawViewToScreen(screen);
}

void SetDrawView(int swidth, int sheight)
{
	glViewport( 0, 0, swidth, sheight );

	gluPerspective( 45.0f,(GLfloat)swidth/(GLfloat)sheight, 1.0f, 100.0f ); 
}

void SetDrawViewToScreen(Screen s)
{
	glViewport( 0, 0, s.Width, s.Height );
	gluPerspective( 45.0f,(GLfloat)s.Width/(GLfloat)s.Height, 1.0f, 100.0f );
}

void InitScreen(int swidth, int sheight, int mode)
{
	if(!glfwOpenWindow ( 1024, 768, 1,0,0,0,0,0, mode ))
	{
		glfwTerminate();
		exit( EXIT_FAILURE );
	}

	glfwSetWindowTitle( TITLE );//Sets the Windows Name
}

void ClearScreen(int red, int blue, int green, int alpha)
{
	glClearColor(red / 255.f, green / 255.f, blue / 255.f, alpha / 255.f);
	glClear(GL_COLOR_BUFFER_BIT);
}