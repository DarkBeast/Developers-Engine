/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis    
******************************************************************************/

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
	double  t;
	Vector2f imgpos;

	CDLGetCurrentDirectory();
	InitScreen(800,600,GLFW_WINDOW);
	glfwEnable(GLFW_KEY_REPEAT);

	glfwSetWindowSizeCallback(handleResize); //handles resize calls
	glfwSetKeyCallback(handleKeypress); //handles Key presses
	initimage(&img);
	pos.x = 0;
	pos.y = 0;
	// Main loop
	while( running )
	{
		t = glfwGetTime();
		glfwWaitEvents();
		ClearScreen(255,255,0,0);

		LoadImage( GetPath("pngtest.png"), &img);

		DrawStateReset();

		imgpos.x = 0;
		imgpos.y = 0;

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
	exit( true );
}

