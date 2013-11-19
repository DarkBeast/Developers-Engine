/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/
#include "GL/glew.h"
#include "widget.h"
#include "mainmenu.h"
#include "controls.h"
#include "render.h"
#include "clienttcp.h"
#include "bool.h"

widget window1;
widget button1;
widget label1;
widget label2;
widget textbox1;
widget textbox2;

void main_menu(void)
{
	int running = TRUE;
	uint32 time;
	uint32 lpstimer = 0;
	uint32 lps = 0;

	draw_state_reset();

	while(running){
		time = (uint32 )glfwGetTime();

		clear_screen(0,0,0,255);

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

		//_sleep(20);

		// Check if ESC key was pressed or window was closed
		running = is_window_open();
	}

	widget_unload(&window1);
}

void init_main_menu(void)
{
	create_window(&window1, NULL, 0, 0, 600, 800, 600, 800, "image\\main.png");
	create_label(&label1,&window1,60,95,120,200,0,0,0,255,FALSE,1,10,TRUE,"input text in text box then press the button");
	create_stextbox(&textbox1,&window1,140,395,100,32,2,2,100,32,0,255,0,255,0,1,FALSE,"image\\textbox.png");
	create_stextbox(&textbox2,&window1,140,335,100,32,2,2,100,32,0,255,0,255,0,1,TRUE,"image\\textbox.png");
	create_button(&button1,&window1,380,220,60,100,60,100,"image\\button.png");

	button1.mousepress = &button1_press;

	widget_manual_focused(&window1);
}

void button1_press(widget *control, int button, int pressed)
{
	textbox *data1 = (textbox *)textbox1.shown.data[0]->control;
	textbox *data2 = (textbox *)textbox2.shown.data[0]->control;

	if(data1->string->data && data2->string->data){
		sendlogin(data1->string->data, data2->string->data);
	}
	else{
		printf("Username or Password can not be empty \n");
	}
}