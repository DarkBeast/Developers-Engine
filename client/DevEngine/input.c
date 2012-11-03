/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/

#include <glfw.h>
#include <stdio.h>
#include <stdlib.h>
#include "input.h"
#include "image.h"
#include "render.h"
#include "widget.h"
#include "globals.h"

void GLFWCALL handlekeypress(int key,int press) //The key that was pressed
{
	//TODO: make HANDLE ARRAY FOR KEYS THAT ARE PUSHED SO IF MULTIPLE KEYS ARE PRESSED IT CAN HANDLE THEM ALL.
}

void GLFWCALL handlemousepos(int x,int y)
{
	setmousepos(x,y);

#if(DEBUG == TRUE)
	printf(" x = %i\n", x);
	printf(" Y = %i\n", y);
#endif
}

void GLFWCALL handlemousepress(int button,int pressed)
{
}

void GLFWCALL handlemousewheel(int moved)
{
}