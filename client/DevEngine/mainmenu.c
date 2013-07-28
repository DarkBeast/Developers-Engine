/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/
#include "GL/glew.h"
#include "widget.h"
#include "mainmenu.h"
#include "controls.h"
#include "render.h"

#include "bool.h"

widget window1;
widget label1;
widget label2;
widget label3;
widget label4;
widget label5;
widget label6;
widget label7;

image img;
int i = 0;

void main_menu(void)
{
	int running = TRUE;
	uint32 time;
	uint32 lpstimer = 0;
	uint32 lps = 0;

	draw_state_reset();

	while(running){
		time = (uint32 )glfwGetTime();

		clear_screen(1,1,1,1);

		widget_manager();

		//Clear information from last draw

		glFlush();
		glfwSwapBuffers();

		if(lpstimer < time){//calculates the loops per second the code does, through everything
			printf("%i\n",lps);
			lpstimer = time + 1;

			lps = 0;
		}

		lps += 1;

		//glfwSleep(.005); //used to save cpu

		// Check if ESC key was pressed or window was closed
		running = glfwGetWindowParam(GLFW_OPENED);
	}

	widget_unload(&window1);
}

void init_main_menu(void)
{
	create_window(&window1, NULL, 0, 0, 600, 800, 600, 800, "image\\main.png");
	create_label(&label1,&window1,85,300,8,0,0,0,255,FALSE," Hello there this is a VBO test on strings");
	create_label(&label2,&window1,85,325,8,0,0,0,255,FALSE,"And this is a somewhat paragraph test for");
	create_label(&label3,&window1,85,350,8,0,0,0,255,FALSE,"Jason whom wanted to test the difference");
	create_label(&label4,&window1,85,375,8,0,0,0,255,FALSE,"in speed and so far it seems reliable.");
	create_label(&label5,&window1,85,400,8,0,0,0,255,FALSE,"in speed and so far it seems reliable.");
	create_label(&label6,&window1,85,425,8,0,0,0,255,FALSE,"in speed and so far it seems reliable.");
	create_label(&label7,&window1,85,450,8,0,0,0,255,FALSE,"a bleak understanding");
	window1.action |= WIDGET_CAN_FOCUS;
	//window1.action |= WIDGET_MOVEABLE;
	widget_manual_focused(&window1);
}

void button2_press(widget *wgt, int button, int pressed)
{
}

void button1_press(widget *wgt, int button, int pressed)
{
}

void button3_press(widget *wgt, int button, int pressed)
{
	glfwTerminate();// Close window and terminate GLFW
	exit(TRUE);// Exit program
}