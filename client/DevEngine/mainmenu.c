#include "widget.h"
#include "mainmenu.h"
#include "controls.h"
#include "render.h"
#include <glfw.h>

widget *label1;

void mainmenu(void)
{
	int running = GL_TRUE;
	uint32 time;
	uint32 lpstimer = 0;
	uint32 lps = 0;

	while( running )
	{
		time = (uint32)glfwGetTime();

		clearscreen(1,1,1,1);

		drawstatereset();
		widgetmanager();
		//Clear information from last draw
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
}

void initmainmenu(void)
{
	widgetinit();
	label1 = (widget *)calloc(1,sizeof(widget));
	createlabel(label1,0,0,0,255,255,255,255,"hello test test jcxtueisjs'z/");
	addtowidget(NULL,label1,0);
}