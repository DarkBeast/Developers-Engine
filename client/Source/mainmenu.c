/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/
#include "GL/glew.h"
#include "mainmenu.h"
#include "controls.h"
#include "render.h"
#include "clienttcp.h"
#include "bool.h"
#include "globals.h"
#include "general.h"

main_menu_t gui;

void main_menu(void)
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

		if(get_menu_state() != MENU_STATE_MAIN)
			break;
	}
}

void init_main_menu(void)
{
	create_window(&gui.wndmainmenu, NULL, 0, 0, 600, 800, 600, 800,"menuback.png", NULL);
	create_frame(&gui.frmmain, &gui.wndmainmenu,0,0,60,800,FALSE, TRUE);
	create_button(&gui.btnclose,&gui.frmmain,750,10,34,34,34,34,"closebutton.png", NULL);
	create_button(&gui.btnlogin,&gui.wndmainmenu,195,170,50,405,50,405,"buttonlarge.png", NULL);
	create_button(&gui.btncreate,&gui.wndmainmenu,195,240,50,405,50,405,NULL,&gui.btnlogin);
	create_button(&gui.btncredits,&gui.wndmainmenu,195,310,50,405,50,405,NULL,&gui.btnlogin);
	create_label(&gui.lbllogin, &gui.btnlogin,170,10,80,25,0,0,0,120,FALSE,2,8,FALSE,"LOGIN");
	create_label(&gui.lblcreate, &gui.btncreate,120,10,80,25,0,0,0,120,FALSE,2,8,FALSE,"NEW ACCOUNT");
	create_label(&gui.lblcredits, &gui.btncredits,155,10,80,25,0,0,0,120,FALSE,2,8,FALSE,"CREDITS");
	gui.btnclose.mousepress = &main_btnclose_press;
	gui.btncredits.mousepress = &main_btncredits_press;
	gui.btnlogin.mousepress = &main_btnlogin_press;
	gui.btncreate.mousepress = &main_btncreate_press;
	gui.lbllogin.action |= WIDGET_CAN_CLICK_BEHIND;
	gui.lblcreate.action |= WIDGET_CAN_CLICK_BEHIND;
	gui.lblcredits.action |= WIDGET_CAN_CLICK_BEHIND;
	gui.btnclose.action |= WIDGET_ALWAYS_USEABLE;
	widget_manual_focused(&gui.wndmainmenu);
}

void main_btnclose_press(widget *control, int button, int pressed)
{
	set_menu_state(MENU_STATE_EXIT);
}

void main_btncredits_press(widget *control, int button, int pressed)
{
	set_menu_state(MENU_STATE_CREDITS);
}

void main_btnlogin_press(widget *control, int button, int pressed)
{
	set_menu_state(MENU_STATE_LOGIN);
}

void main_btncreate_press(widget *control, int button, int pressed)
{
	set_menu_state(MENU_STATE_CREATE);
}