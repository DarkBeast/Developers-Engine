
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
	glMatrixMode( GL_PROJECTION ); //Switch to setting the camera perspective
	//Set the camera perspective
	glLoadIdentity(); //reset the camera
	gluPerspective( 45.0f,(GLfloat)swidth/(GLfloat)sheight, 1.0f, 100.0f ); 
}

void SetDrawViewToScreen(Screen s)
{
	glViewport( 0, 0, s.Width, s.Height );
	glMatrixMode( GL_PROJECTION ); //Switch to setting the camera perspective
	//Set the camera perspective
	glLoadIdentity(); //reset the camera
	gluPerspective( 45.0f,(GLfloat)s.Width/(GLfloat)s.Height, 1.0f, 100.0f );
}

//sets the screen, GLFW , and the Screen Title
void InitScreen(int swidth, int sheight, int mode)
{
	// Initialize GLFW
	if( !glfwInit() )
		RenderError(GLFWINIT_ERROR);

	// Finally we can Open an OpenGL window
	if(!glfwOpenWindow ( 1024, 768, 1,0,0,0,0,0, mode ))
		RenderError(GLFWWIN_ERROR);

	glfwSetWindowTitle( TITLE );//Sets the Windows Name
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
	gluPerspective( 45.0f,                      //camera angle
		(GLfloat)width/(GLfloat)height, //The width to height ratio
		1.0f,                          //The near z clipping coordinate
		100.0f );                       //The far z clipping coordinate
}

int LoadImage(char *name, Image* img, int type)
{
	GLuint Texture;

	img->BytesPerPixel =0;
	img->Format =0;
	img->Height=0;
	img ->Width = 0;
	img ->pixels = NULL;
	img ->texID = NULL;

	// Read image from file
	if( !load_png( name, img) )
	{
		return GL_FALSE;
	}

	glGenTextures( 1, &Texture );
	glBindTexture( GL_TEXTURE_2D, Texture );

	glTexImage2D( GL_TEXTURE_2D, 0, img ->Format,
		img->Width, img->Height, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, (void*) img->pixels );

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	
	img ->texID = Texture;

	return GL_TRUE;
}

int Draw(Image* img, SCoord pos)
{

	glEnable(GL_TEXTURE_2D);
     glBindTexture(GL_TEXTURE_2D, img ->texID);

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();


	glBegin( GL_QUADS );
	glTexCoord2f( -20.0f,  20.0f );
	glVertex3f( -50.0f, 0.0f, -50.0f );

	glTexCoord2f(  20.0f,  20.0f );
	glVertex3f(  50.0f, 0.0f, -50.0f );

	glTexCoord2f(  20.0f, -20.0f );
	glVertex3f(  50.0f, 0.0f,  50.0f );

	glTexCoord2f( -20.0f, -20.0f ); //Top left corner
	glVertex3f( -50.0f, 0.0f,  50.0f );
	glEnd();

}
