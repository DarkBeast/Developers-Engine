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
widget textbox2;
widget textbox3;
widget hscrollbar1;
widget vscrollbar1;
widget hprogressbar1;
widget vprogressbar1;
widget checkbox1;
widget radio1[2];
widget pic1;

void main_menu(void)
{
	int running = TRUE;
	uint32 time;
	uint32 lpstimer = 0;
	uint32 lps = 0;

	draw_state_reset();

	while(running){
		time = (uint32 )glfwGetTime();

		clear_screen(255,255,255,255);

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
	create_mtextbox(&textbox1,&window1,260,195,100,32,2,2,100,200,0,255,0,255,0,7,1,"image\\textbox.png");
	create_stextbox(&textbox2,&window1,140,395,100,32,2,2,100,32,0,255,0,255,0,1,FALSE,"image\\textbox.png");
	create_stextbox(&textbox3,&window1,140,335,100,32,2,2,100,32,0,255,0,255,0,1,TRUE,"image\\textbox.png");
	create_button(&button1,&window1,380,220,60,100,60,100,"image\\button.png");
	create_hscrollbar(&hscrollbar1, &window1,500,200,22,10,22,100,2,100,"image\\scrollbar.png","image\\arrowleft.png","image\\arrowright.png", "image\\bar.png");
	create_vscrollbar(&vscrollbar1, &window1,500,300,10,22,100,22,2,100,"image\\vscrollbar.png","image\\arrowtop.png","image\\arrowbottom.png", "image\\Vbar.png");
	create_hprogressbar(&hprogressbar1,&window1,500,240,24,150,24,150,50,"image\\progress.png", "image\\progressbar.png");
	create_vprogressbar(&vprogressbar1,&window1,530,300,150,24,150,24,50,"image\\vprogress.png", "image\\vprogressbar.png");
	create_checkbox(&checkbox1,&window1,300,400,16,16,32,32,"image\\check.png");
	create_radio(&radio1[0],&window1,700,200,16,16,32,32,TRUE,"image\\radio.png");
	create_radio(&radio1[1],&window1,700,234,16,16,32,32,FALSE,"image\\radio.png");
	create_picturebox(&pic1,&window1,32,32,50,50,50,50,"image\\pictest.png");
	link_radio(&radio1[0], &radio1[1]);
	button1.mousepress = &button1_press;
	set_scrollbar_buttons(&vscrollbar1,&vscrollbar1_press);
	set_scrollbar_buttons(&hscrollbar1,&hscrollbar1_press);
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

void vscrollbar1_press(widget *control, int button, int pressed)
{
	update_progressbar_value(&hprogressbar1,control->parent->value);
}

void hscrollbar1_press(widget *control, int button, int pressed)
{
	update_progressbar_value(&vprogressbar1,control->parent->value);
}