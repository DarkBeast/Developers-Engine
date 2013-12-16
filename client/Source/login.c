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

login_t gui;

void login(void)
{
	uint32 time;
	uint32 lpstimer = 0;
	uint32 lps = 0;
	int running = TRUE;

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
		if(get_menu_state() != MENU_STATE_LOGIN)
			break;
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
	gui.btnclose.mousepress = &login_btnclose_press;
	gui.btnback.mousepress = &login_btnback_press;
	gui.lblback.action |= WIDGET_CAN_CLICK_BEHIND;
	gui.lbllogin.action |= WIDGET_CAN_CLICK_BEHIND;
	gui.lbllogintxt.action |= WIDGET_CAN_CLICK_BEHIND;
	gui.lblpasstxt.action |= WIDGET_CAN_CLICK_BEHIND;
	widget_manual_focused(&gui.wndlogin);
}

void login_btnclose_press(widget *control, int button, int pressed)
{
	set_menu_state(MENU_STATE_EXIT);
}

void login_btnback_press(widget *control, int button, int pressed)
{
	set_menu_state(MENU_STATE_MAIN);
}