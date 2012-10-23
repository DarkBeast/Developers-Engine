/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis    
******************************************************************************/

#include <glfw.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "globals.h"
#include "input.h"
#include "image.h"
#include "program_path.h"
#include "render.h"
#include "types.h"


int main( )
{
	int running = GL_TRUE;
	image img;
	vector2f imgpos;

	gpd();
	initscreen(800,600,GLFW_WINDOW);
	glfwEnable(GLFW_KEY_REPEAT);

	glfwSetWindowSizeCallback(handleresize); //handles Window resize calls
	glfwSetKeyCallback(handlekeypress); //handles Key presses
	glfwSetMousePosCallback(handlemousepos);//handles mouse move events
	glfwSetMouseButtonCallback(handlemousepress);//handles mouse button events
	glfwSetMouseWheelCallback(handlemousewheel);//handles mouse wheel scrolling

	
	// Main loop
	while( running )
	{

		clearscreen(255,255,0,0);

		drawstatereset();

		//Clear information from last draw
		glFlush();
		glfwSwapBuffers();
		glfwSleep(5);
		// Check if ESC key was pressed or window was closed
		running = glfwGetWindowParam( GLFW_OPENED );
	}

	// Close window and terminate GLFW
	glfwTerminate();

	// Exit program
	exit(true);
}

