
#include <glfw.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Error.h"
#include "globals.h"
#include "Keys.h"
#include "image.h"
#include "program_path.h"
#include "Render.h"
#include "Types.h"


int main( )
{
	int running = GL_TRUE;
	Image img;
	Vector2i pos;
	Vector2f imgpos;
	char* dapath;
	CDLGetCurrentDirectory();
	InitScreen(800,600,GLFW_WINDOW);

	glfwSetWindowSizeCallback(handleResize); //handles resize calls
	glfwSetKeyCallback(handleKeypress); //handles Key presses
	initimage(&img);
	// Main loop
	while( running )
	{
		
		ClearScreen(1,1,1,1);
	
		if(LoadImage( GetPath("pngtest.png"), &img, 0))
		{
			RenderError(IMAGELOADER_ERROR);
		}
		
		DrawStateReset();
		pos.x = -90;
		pos.y = 1;

		imgpos.x = 1;
		imgpos.y = 1;
		
		Draw(&img, pos, imgpos, 128, 128);
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

