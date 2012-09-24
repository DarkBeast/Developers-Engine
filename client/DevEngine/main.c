#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glfw.h>
#include "globals.h"
#include "graphics/image.h"
#include "program_path.h"
//#define GLFW_INCLUDE_GL3

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

    glViewport( 0, 0, width, height );

    glMatrixMode( GL_PROJECTION ); //Switch to setting the camera perspective

    //Set the camera perspective

    glLoadIdentity(); //reset the camera

		gluPerspective( 45.0f,                      //camera angle

			(GLfloat)width/(GLfloat)height, //The width to height ratio
			1.0f,                          //The near z clipping coordinate
			100.0f );                       //The far z clipping coordinate

}


int main( )
{
	int running = GL_TRUE;
	GLuint Texture ;
	int size = 60;
	//int* width = &size;
	//int* hieght = 60;
	glGenTextures(1,&Texture);
CDLGetCurrentDirectory();


	//Texture= loadTexture("image\1.png", size, size);
	read_png_file(GetPath("pngtest.png"));
	//write_png_file("silly.png");
	// Initialize GLFW
	if( !glfwInit() )
	{
		exit( EXIT_FAILURE );
	}
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	


	// Finally we can Open an OpenGL window
	if(!glfwOpenWindow ( 1024, 768, 1,0,0,0,0,0, GLFW_WINDOW ))
	{
		glfwTerminate();
		exit( EXIT_FAILURE );
	}


	glfwSetWindowTitle( TITLE );//Sets the Windows Name
	glfwSetWindowSizeCallback(handleResize); //callback function of GLFW to handle window resize
	glfwSetKeyCallback(handleKeypress); //callback function to handle key press

	// Main loop
	while( running )
	{
		glClearColor( 0.0f, 0.0f, 0.0f, 0.0f ); //clear background screen to black

 

		//Clear information from last draw

		glClear( GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

 

		glMatrixMode(GL_MODELVIEW); //Switch to the drawing perspective

		glLoadIdentity(); //Reset the drawing perspective
 
		glBindTexture(GL_TEXTURE_2D, Texture);

		glBegin(GL_QUADS); //Begin quadrilateral coordinates

 

			//Trapezoid

			glVertex3f(-0.7f, -1.5f, -5.0f);

			glVertex3f(0.7f, -1.5f, -5.0f);

			glVertex3f(0.4f, -0.5f, -5.0f);

			glVertex3f(-0.4f, -0.5f, -5.0f);

 

		glEnd(); //End quadrilateral coordinates

 

		glBegin(GL_TRIANGLES); //Begin triangle coordinates

 

		//Pentagon

			glVertex3f(0.5f, 0.5f, -5.0f);

			glVertex3f(1.5f, 0.5f, -5.0f);

			glVertex3f(0.5f, 1.0f, -5.0f);

 

			glVertex3f(0.5f, 1.0f, -5.0f);

			glVertex3f(1.5f, 0.5f, -5.0f);

			glVertex3f(1.5f, 1.0f, -5.0f);

 

			glVertex3f(0.5f, 1.0f, -5.0f);

			glVertex3f(1.5f, 1.0f, -5.0f);

			glVertex3f(1.0f, 1.5f, -5.0f);

 

			//Triangle

			glVertex3f(-0.5f, 0.5f, -5.0f);

			glVertex3f(-1.0f, 1.5f, -5.0f);

			glVertex3f(-1.5f, 0.5f, -5.0f);

 

		glEnd(); //End triangle coordinates

		glfwSwapBuffers();
	// Check if ESC key was pressed or window was closed
		running = glfwGetWindowParam( GLFW_OPENED );
	}

	// Close window and terminate GLFW
	glfwTerminate();

	// Exit program
	exit( EXIT_SUCCESS );
}

