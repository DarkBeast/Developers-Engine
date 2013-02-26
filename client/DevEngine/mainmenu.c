/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/
#include "widget.h"
#include "mainmenu.h"
#include "controls.h"
#include "render.h"
#include <glfw.h>
#include "bool.h"

widget label1;
widget button1;
widget button2;
widget window1;
widget window2;
widget check1;
widget radio1;
widget radio2;

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
}

void init_main_menu(void)
{
	create_label(&label1, NULL, 5, 5, 8, 255, 255, 255, 255, FALSE, "click me!");
	create_window(&window1, NULL, 5, 5, 150, 150, 150, 150, 2);
	create_window(&window2, NULL, 100, 100, 150, 150, 150, 150, 2);
	create_button(&button1, &window1, 3, 18, 60, 100, 60, 100, 1);
	create_button(&button2, &window1, 3, 75, 60, 100, 60, 100, 1);
	create_checkbox(&check1, &window2, 40, 25, 16, 16, 16, 16, 3);
	create_radio(&radio1, &window2, 16, 16, 16, 16, 16, 16, TRUE, 4);
	create_radio(&radio2, &window2, 16, 32, 16, 16, 16, 16, FALSE, 4);

	set_control_image(&window2,"image\\window1.png");
	link_radio(&radio1,&radio2);

	label1.action |= WIDGET_CAN_CLICK_BEHIND;
	window1.action |= WIDGET_MOVEABLE;
	window1.action |= WIDGET_CAN_FOCUS;
	window2.action |= WIDGET_MOVEABLE;
	window2.action |= WIDGET_CAN_FOCUS;

	button1.mousepress = button1_press;
}

void button1_press(widget *wgt, int button, int pressed)
{
	label *z = (label *)label1.control;
	switch(i){
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