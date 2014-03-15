/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/
#include "GL/glew.h"
#include "widget.h"
#include "credits.h"
#include "controls.h"
#include "render.h"
#include "clienttcp.h"
#include "bool.h"
#include "globals.h"
#include "general.h"

credits_t gui;

void credits(void)
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

		_sleep(30);

		// Check if ESC key was pressed or window was closed
		running = is_window_open();
		if(get_menu_state() != MENU_STATE_CREDITS)
			break;
	}
}

void init_credits(void)
{
	create_window(&gui.wndcredits, NULL, 0, 0, 600, 800, 600, 800,"menuback.png", NULL);
	create_frame(&gui.frmmain, &gui.wndcredits,0,0,60,800,FALSE, TRUE);
	create_button(&gui.btnclose,&gui.frmmain,750,10,34,34,34,34,"closebutton.png", NULL);
	create_picturebox(&gui.imgcredits,&gui.wndcredits,176,121,361,451,361,451,comb_2str(GUI_PATH,"devcredits.png"),NULL);
	create_button(&gui.btnback,&gui.imgcredits,22,290,50,405,50,405,"buttonlarge.png", NULL);
	create_label(&gui.lblback, &gui.btnback,170,10,80,25,0,0,0,120,FALSE,2,8,FALSE,"BACK");
	gui.btnclose.mousepress = &credits_btnclose_press;
	gui.btnback.mousepress = &credits_btnback_press;
	gui.lblback.action |= WIDGET_CAN_CLICK_BEHIND;
	gui.btnclose.action |= WIDGET_ALWAYS_USEABLE;
	widget_manual_focused(&gui.wndcredits);
}

void credits_btnclose_press(widget *control, int button, int pressed)
{
	set_menu_state(MENU_STATE_EXIT);
}

void credits_btnback_press(widget *control, int button, int pressed)
{
	set_menu_state(MENU_STATE_MAIN);
}