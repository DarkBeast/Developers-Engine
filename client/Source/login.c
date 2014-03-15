/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/
#include "GL/glew.h"
#include "widget.h"
#include "login.h"
#include "controls.h"
#include "render.h"
#include "clienttcp.h"
#include "bool.h"
#include "globals.h"
#include "general.h"
#include "winsocket.h"
#include "tinycthread.h"

login_t gui;
thrd_t t1;
char *string_login;
sbool login_update_status = FALSE;

void login(void)
{
	int running = TRUE;

	draw_state_reset();

	while(running){
		clear_screen(0,0,0,255);

		widget_manager(widget_get_uip()->root);

		//Clear information from last draw
		glFlush();
		glfwSwapBuffers(get_the_window());
		glfwPollEvents();

		if(login_update_status){
			status_box_text(&gui.status,string_login);
			widget_show(&gui.status);
			widget_manual_focused(&gui.status);
			login_update_status = FALSE;
		}

		_sleep(30);//keep her at 60lps.

		// Check if ESC key was pressed or window was closed
		running = is_window_open();
		if(get_menu_state() != MENU_STATE_LOGIN){
			TerminateThread(&t1,0);
			break;
		}
	}
}

void init_login(void)
{
	create_window(&gui.wndlogin, NULL, 0, 0, 600, 800, 600, 800,"menuback.png", NULL);
	create_frame(&gui.frmmain, &gui.wndlogin,0,0,60,800,FALSE, TRUE);
	create_button(&gui.btnclose,&gui.frmmain,750,10,34,34,34,34,"closebutton.png", NULL);
	create_button(&gui.btnback,&gui.wndlogin,200,433,50,405,50,405,"buttonlarge.png", NULL);
	create_button(&gui.btnlogin,&gui.wndlogin,200,370,50,405,50,405,NULL, &gui.btnback);
	create_stextbox(&gui.txtname,&gui.wndlogin,210,189,375,37,4,3,375,37,0,0,0,120,32,2,FALSE,"textbox.png",NULL);
	create_stextbox(&gui.txtpass,&gui.wndlogin,210,265,375,37,4,3,375,37,0,0,0,120,32,2,TRUE,NULL,&gui.txtname);
	create_label(&gui.lblback, &gui.btnback,170,10,80,25,0,0,0,120,FALSE,2,8,FALSE,"BACK");
	create_label(&gui.lbllogin, &gui.btnlogin,166,10,80,25,0,0,0,120,FALSE,2,8,FALSE,"LOGIN");
	create_label(&gui.lblpasstxt, &gui.wndlogin,212,242,120,25,0,0,0,120,FALSE,1,9,FALSE,"Password");
	create_label(&gui.lbllogintxt, &gui.wndlogin,212,164,120,25,0,0,0,120,FALSE,1,9,FALSE,"Login");
	create_statusbox(&gui.status,NULL,(get_screen_width()/2) - 189,(get_screen_height()/2) - 85,"test",NULL, NULL);
	status_box_set_click_event(&gui.status,&login_status_button_press);

	gui.btnclose.mousepress = &login_btnclose_press;
	gui.btnback.mousepress = &login_btnback_press;
	gui.lblback.action |= WIDGET_CAN_CLICK_BEHIND;
	gui.lbllogin.action |= WIDGET_CAN_CLICK_BEHIND;
	gui.lbllogintxt.action |= WIDGET_CAN_CLICK_BEHIND;
	gui.lblpasstxt.action |= WIDGET_CAN_CLICK_BEHIND;
	widget_manual_focused(&gui.wndlogin);
	widget_hide(&gui.status);

	if(socketconnect()){ //just to test for server connection.
		thrd_create(&t1, socketlisten, (void*)0);
	}
}

void login_btnclose_press(widget *control, int button, int pressed)
{
	set_menu_state(MENU_STATE_EXIT);
}

void login_btnback_press(widget *control, int button, int pressed)
{
	set_menu_state(MENU_STATE_MAIN);
}

void login_btnlogin_press(widget *control, int button, int pressed)
{
	textbox *name = (textbox *)gui.txtname.shown.data[0]->control;
	textbox *password = (textbox *)gui.txtpass.shown.data[0]->control;

	if(get_str_size(name->string->data) < MIN_NAME_LENGTH){
		login_status_message("User name Name must be 4 or more syllables");
		return;
	}

	if(get_str_size(password->string->data) < MIN_PASS_LENGTH){
		login_status_message("Password Name must be 4 or more syllables");
		return;
	}

	if(isconnected())
		send_login(name->string->data,password->string->data);
	else
		login_status_message("Not Connected To The Server");
}

void login_status_button_press(widget *control, int button, int pressed)
{
	widget_hide(&gui.status);
	widget_manual_focused(&gui.wndlogin);
}

void login_status_message(char *text)
{
	string_login = text;
	login_update_status = TRUE;
}