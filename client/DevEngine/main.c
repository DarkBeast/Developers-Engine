#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glfw.h>
#include "globals.h"
#include "image.h"
#include "program_path.h"
#include "Error.h"
#include "Types.h"
#include "Render.h"

int main( )
{
	int running = GL_TRUE;
	Image img;
	SCoord pos;
	CDLGetCurrentDirectory();
	InitScreen(800,600,GLFW_WINDOW);
	
	glfwSetWindowSizeCallback(handleResize); 
	// Main loop
	while( running )
	{
		glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );

		if(LoadImage( GetPath("pngtest.png"), &img, 0))
		{
			RenderError(IMAGELOADER_ERROR);
		}

		DrawStateReset();
		pos.x1 = 0;
		pos.x2 = 0;
		pos.y1 = 0;
		pos.y2 = 0;

		Draw(&img, pos);
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

