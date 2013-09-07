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
widget button1;
widget label1;
widget textbox1;

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
		glfwSwapBuffers(get_the_window());
		glfwPollEvents();

		if(lpstimer < time){//calculates the loops per second the code does, through everything
			printf("%i\n",lps);
			lpstimer = time + 1;

			lps = 0;
		}

		lps += 1;

		//glfwSleep(.005); //used to save cpu

		// Check if ESC key was pressed or window was closed
		running = is_window_open();
	}

	widget_unload(&window1);
}

void init_main_menu(void)
{
	create_window(&window1, NULL, 0, 0, 600, 800, 600, 800, "image\\main.png");
	create_label(&label1,&window1,60,95,120,200,0,0,0,255,FALSE,1,10,TRUE,"input text in text box then press the button");
	create_mtextbox(&textbox1,&window1,260,195,100,32,2,2,100,200,0,255,0,255,0,7,1,"image\\textbox.png");
	create_button(&button1,&window1,380,220,60,100,60,100,"image\\button.png");
	button1.mousepress = button1_press;
	textbox1.action |= WIDGET_CAN_FOCUS;
	window1.action |= WIDGET_CAN_FOCUS;
	window1.action |= WIDGET_MOVEABLE;
	widget_manual_focused(&window1);
}

void button1_press(widget *control, int button, int pressed)
{
	textbox *data = (textbox *)textbox1.shown.data[0]->control;

	if(data->string->data){
		update_label_string(&label1, data->string->data);
	}
}