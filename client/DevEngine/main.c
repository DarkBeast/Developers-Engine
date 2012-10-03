#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glfw.h>
#include "globals.h"
#include "image.h"
#include "program_path.h"
#include "Error.h"



int main( )
{
	int running = GL_TRUE;


	CDLGetCurrentDirectory();



	// Initialize GLFW
	if( !glfwInit() )
		RenderError(GLFWINIT_ERROR);


	// Finally we can Open an OpenGL window
	if(!glfwOpenWindow ( 1024, 768, 1,0,0,0,0,0, GLFW_WINDOW ))
		RenderError(GLFWWIN_ERROR);


	glfwSetWindowTitle( TITLE );//Sets the Windows Name


	// Main loop
	while( running )
	{

		//Clear information from last draw

		glClear( GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
		glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		glMatrixMode(GL_MODELVIEW); //Switch to the drawing perspective

		glLoadIdentity(); //Reset the drawing perspective



		glBegin(GL_QUADS);

		glVertex3f(-0.4, 0.4, -1);
		glVertex3f( 0.4, 0.4, -1);
		glVertex3f( 0.4,-0.4, -1);
		glVertex3f(-0.4,-0.4, -1);
		glEnd();



		//glDisable(GL_TEXTURE_2D);
		glPopAttrib();

		glFlush();

		glfwSwapBuffers();
		// Check if ESC key was pressed or window was closed
		running = glfwGetWindowParam( GLFW_OPENED );
	}

	// Close window and terminate GLFW
	glfwTerminate();

	// Exit program
	exit( EXIT_SUCCESS );
}

