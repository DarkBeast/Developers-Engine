#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glfw.h>
#include "globals.h"
#include "image.h"
#include "program_path.h"
#include "Error.h"
#include "Types.h"


int main( )
{
	int running = GL_TRUE;
	Image img;

	CDLGetCurrentDirectory();

	// Main loop
	while( running )
	{
		


        //Clear information from last draw
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

