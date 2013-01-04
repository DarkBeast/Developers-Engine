#include "widget.h"
#include "mainmenu.h"
#include "controls.h"
#include "render.h"
#include <glfw.h>
#include "bool.h"

widget *label1;
widget *button1;
int i = 0;

void mainmenu(void)
{
	sbool running = TRUE;
	uint32 time;
	uint32 lpstimer = 0;
	uint32 lps = 0;

	drawstatereset();

	while( running )
	{
		time = (uint32)glfwGetTime();

		clearscreen(1,1,1,1);

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
	button1 = (widget *)calloc(1,sizeof(widget));
	createlabel(label1,10,10,0,255,255,255,255,FALSE,"click me!");
	createbutton(button1,100,100,60,100,"image\\buttons.png");
	addtowidget(NULL,button1,FALSE);
	addtowidget(button1,label1,FALSE);

	button1->mousepress = button1press;
}

void button1press(void *wgt, int button, int pressed)
{
	label *z = (label *)label1->control;
	switch(i)
	{
	case 0:

		z->string->data = "fuck you!";
		i++;
		break;
	case 1:
		z->string->data = "You like me?";
		i++;
		break;
	case 2:
		z->string->data = "really hate you!";
		i++;
		break;
	case 3:
		z->string->data = "you know what!";
		i = 0;
		break;
	default:
		z->string->data = "trying to hack me ehh";
		i = 0;
	}
}