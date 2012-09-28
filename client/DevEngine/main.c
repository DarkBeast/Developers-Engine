#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glfw.h>
#include "globals.h"
#include "graphics/image.h"
#include "program_path.h"
//#define GLFW_INCLUDE_GL3
GLuint Texture;
void GLFWCALL handleKeypress(int key,int press) //The key that was pressed

{

    switch (key) {

        case GLFW_KEY_ESC: //Escape key

            exit(0); //Exit the program

    }

}


void GLFWCALL handleResize(int width,int height)

{
    //Tell OpenGL how to convert from coordinates to pixel values
	glEnable( GL_TEXTURE_2D );
    glViewport( 0, 0, width, height );

    glMatrixMode( GL_PROJECTION ); //Switch to setting the camera perspective

    //Set the camera perspective

    glLoadIdentity(); //reset the camera

		
		

}


int main( )
{
	int running = GL_TRUE;
	
	
CDLGetCurrentDirectory();


	
	// Initialize GLFW
	if( !glfwInit() )
	{
		exit( EXIT_FAILURE );
	}
	glShadeModel(GL_SMOOTH);
 
     glEnable(GL_LIGHTING);
     glEnable(GL_LIGHT0);
     glDisable(GL_DEPTH_TEST);
 
   
	//glEnable(GL_TEXTURE_2D);
	

	
	// Finally we can Open an OpenGL window
	if(!glfwOpenWindow ( 1024, 768, 1,0,0,0,0,0, GLFW_WINDOW ))
	{
		glfwTerminate();
		exit( EXIT_FAILURE );
	}

	glfwSetWindowTitle( TITLE );//Sets the Windows Name
	glfwSetWindowSizeCallback(handleResize); //callback function of GLFW to handle window resize
	glfwSetKeyCallback(handleKeypress); //callback function to handle key press
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f ); //clear background screen to black

	// Main loop
	while( running )
	{
		

 

		//Clear information from last draw

		glClear( GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
		glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 

		glMatrixMode(GL_MODELVIEW); //Switch to the drawing perspective

		glLoadIdentity(); //Reset the drawing perspective
 

		
		glBegin(GL_QUADS);
glTexCoord2f(0,1);
glVertex3f(-1, 1, -1);
glTexCoord2f(1,1);
glVertex3f( 1, 1, -1);
glTexCoord2f(1,0);
glVertex3f( 1,-1, -1);
glTexCoord2f(0,0);
glVertex3f(-1,-1, -1);
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

