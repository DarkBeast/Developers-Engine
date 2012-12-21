/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/
// /ENTRY:"mainCRTStartup"
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
#include "mainmenu.h"

int main(void)
{
	getprogramdirectory();

	initscreen(800,600,GLFW_WINDOW);
	glfwEnable(GLFW_KEY_REPEAT);

	glfwSetWindowSizeCallback(handleresize); //handles Window resize calls
	glfwSetKeyCallback(handlekeypress); //handles Key presses
	glfwSetMousePosCallback(handlemousepos);//handles mouse move events
	glfwSetMouseButtonCallback(handlemousepress);//handles mouse button events
	glfwSetMouseWheelCallback(handlemousewheel);//handles mouse wheel scrolling

	initfont("");

	// Main loop
	initmainmenu();
	mainmenu();
	// Close window and terminate GLFW
	glfwTerminate();

	// Exit program
	exit(TRUE);
}