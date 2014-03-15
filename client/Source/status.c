/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/
#include "GL/glew.h"
#include "widget.h"
#include "status.h"
#include "controls.h"
#include "render.h"
#include "clienttcp.h"
#include "bool.h"
#include "globals.h"
#include "general.h"
#include "winsocket.h"
#include "tinycthread.h"

status_t gui;
thrd_t t1;
char *string_status;
sbool status_update_status = FALSE;
uint8 status_type = STATUS_TYPE_SOCKET;

uint8 get_status_type(void)
{
	return status_type;
}

void set_status_type(uint8 type)
{
	status_type = type;
}

void status(void)
{
	int running = TRUE;

	draw_state_reset();

	while(running){
		clear_screen(0,0,0,255);

		widget_manager(widget_get_uip()->root);

		if(status_update_status){
			update_label_string(&gui.lblstatus, string_status);
			status_update_status = FALSE;
		}

		//Clear information from last draw
		glFlush();
		glfwSwapBuffers(get_the_window());
		glfwPollEvents();

		_sleep(30);

		// Check if ESC key was pressed or window was closed
		running = is_window_open();
		if(get_menu_state() != MENU_STATE_STATUS){
			if(status_type == STATUS_TYPE_SOCKET)
				TerminateThread(&t1,0);
			glfwSetWindowSize(get_the_window(),get_screen_width(),get_screen_height());
			set_draw_view(0,0,get_screen_width(),get_screen_height());
			break;
		}
	}
}

void init_status(char *text)
{
	create_window(&gui.wndstatus, NULL, 0, 0, 378, 170, 450, 202,"statuswidget.png", NULL);
	create_frame(&gui.frmmain, &gui.wndstatus,0,0,60,800,FALSE, TRUE);
	create_button(&gui.btnclose,&gui.frmmain,750,10,34,34,34,34,"closebutton.png", NULL);
	create_label(&gui.lblstatus, &gui.wndstatus,170,10,80,25,0,0,0,120,FALSE,2,8,FALSE,text);
	gui.btnclose.mousepress = &status_btnclose_press;
	gui.btnclose.action |= WIDGET_ALWAYS_USEABLE;
	widget_manual_focused(&gui.wndstatus);

	if(socketconnect()){ //just to test for server connection.
		thrd_create(&t1, socketlisten, (void*)0);
	}
}

//no thread or socket connect needed.
void init_status_loader(char *text)
{
	glfwSetWindowSize(get_the_window(),378,170);
	set_draw_view(0,0,378,170);
	create_window(&gui.wndstatus, NULL, 0, 0, 170, 378, 170, 378,"statuswidget.png", NULL);
	create_frame(&gui.frmmain, &gui.wndstatus,0,0,60,800,FALSE, TRUE);
	create_button(&gui.btnclose,&gui.frmmain,338,8,34,34,34,34,"closebutton.png", NULL);
	create_label(&gui.lblstatus, &gui.wndstatus,35,90,200,100,0,0,0,120,FALSE,2,30,FALSE,text);
	gui.btnclose.mousepress = &status_btnclose_press;
	gui.btnclose.action |= WIDGET_ALWAYS_USEABLE;
	widget_manual_focused(&gui.wndstatus);
}

void status_btnclose_press(widget *control, int button, int pressed)
{
	set_menu_state(MENU_STATE_EXIT);
}

void status_message(char *text)
{
	if(status_update_status == FALSE){
		string_status = text;
		status_update_status = TRUE;
	}
}