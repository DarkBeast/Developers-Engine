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
#include "text.h"

text word;

int main(void)
{
	int running = GL_TRUE;
	uint32 time;
	uint32 lpstimer = 0;
	uint32 lps = 0;

	gpd();

	initscreen(800,600,GLFW_WINDOW);
	glfwEnable(GLFW_KEY_REPEAT);

	glfwSetWindowSizeCallback(handleresize); //handles Window resize calls
	glfwSetKeyCallback(handlekeypress); //handles Key presses
	glfwSetMousePosCallback(handlemousepos);//handles mouse move events
	glfwSetMouseButtonCallback(handlemousepress);//handles mouse button events
	glfwSetMouseWheelCallback(handlemousewheel);//handles mouse wheel scrolling

	initfont("");
	//word =(text *)calloc(1, sizeof(text));
	settext(&word,0,100,16,1,1,1,1,"The Brown Fox Jumps Over The Fence");

	// Main loop
	while( running )
	{
		time = (uint32)glfwGetTime();

		clearscreen(1,1,1,1);

		drawstatereset();
		//Clear information from last draw
		rendertext(&word);
		glFlush();
		glfwSwapBuffers();

		if(lpstimer < time)
		{//calculates the loops per second the code does, through everything
			printf("%i\n",lps);
			lpstimer = time + 1;

			lps = 0;
		}

		lps += 1;

		//glfwSleep(.005); //used to save cpu

		// Check if ESC key was pressed or window was closed
		running = glfwGetWindowParam( GLFW_OPENED );
	}

	// Close window and terminate GLFW
	glfwTerminate();

	// Exit program
	exit(TRUE);
}