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
#include "tinycthread.h"

status_t gui;
sbool accepted = FALSE;
sbool canaccept = FALSE;
double counter;
double timer;

uint32 sizex;
uint32 sizey;

sbool update_string = FALSE;
char *string;

void set_status(char *text)
{
	string = text;
	update_string = TRUE;
}

void status_unload(void)
{
	accepted = TRUE;
	canaccept = TRUE;

	widget_unload(widget_get_uip().root);
	glfwSetWindowSize(get_the_window(),sizex,sizey);
	handle_resize(get_the_window(),sizex,sizey);
}

int status(void *arg)
{
	int running = TRUE;

	draw_state_reset();

	while(running){
		timer = glfwGetTime();

		clear_screen(0,0,0,255);

		if(update_string){
			update_label_string(&gui.lbltext,string);
			update_string = FALSE;
		}

		widget_manager();

		//Clear information from last draw

		glFlush();
		glfwSwapBuffers(get_the_window());
		glfwPollEvents();

		//_sleep(20);

		// Check if ESC key was pressed or window was closed
		running = is_window_open();

		if(canaccept){
			if(accepted)
				break;
		}
		else{
			if(counter < timer && !canaccept){
				break;
			}
		}
	}

	return TRUE;
}

void draw_status(widget *control)
{
	sbool unload = FALSE;

	timer = glfwGetTime();

	draw_windows(control);

	if(canaccept){
		if(accepted){
			unload = TRUE;
		}
	}
	else{
		if(counter < timer && !canaccept){
			widget_set_disabled(FALSE);
			unload = TRUE;
		}
	}

	if(unload){
		if(widget_is_disabled())
			widget_unload(gui.wndhide.parent);
		else
			widget_unload(gui.wndstatus.parent);
	}
}

void init_status_window(sbool accept, double count)
{
	sizex = get_screen_width();
	sizey = get_screen_height();

	glfwSetWindowSize(get_the_window(),378,170);
	handle_resize(get_the_window(),378,170);

	create_window(&gui.wndstatus, NULL, 0, 0, 170, 378, 170, 378,"statuswidget.png", NULL);
	create_frame(&gui.frmmain, &gui.wndstatus,0,0,60,800,FALSE, TRUE);
	create_button(&gui.btnclose,&gui.frmmain,330,7,34,34,34,34,"closebutton.png", NULL);

	if(accept){
		create_button(&gui.btnok,&gui.wndstatus,100,115,50,180,50,180,"smallbutton.png", NULL);
		create_label(&gui.lblok, &gui.btnok,75,10,80,25,0,0,0,120,FALSE,2,8,FALSE,"OK");
		gui.btnok.mousepress = &status_btnok_press;
		gui.lblok.action |= WIDGET_CAN_CLICK_BEHIND;
	}

	create_label(&gui.lbltext, &gui.wndstatus,50,65,250,100,0,0,0,120,FALSE,1,25,TRUE,"...");

	gui.btnclose.mousepress = &status_btnclose_press;
	gui.btnclose.action |= WIDGET_ALWAYS_USEABLE;
	widget_manual_focused(&gui.wndstatus);
	timer = glfwGetTime();
	counter = count + timer;
}

//finish...
void init_status_widget(widget *parent, sbool accept, double count, sbool disable_controls)
{
	if(disable_controls){
		widget_set_disabled(TRUE);
		create_window(&gui.wndhide, parent, 0, 0, 32, 32, parent->height, parent->width,"hide.png", NULL);
		create_window(&gui.wndstatus, &gui.wndhide, (gui.wndhide.width - 378) / 2, (gui.wndhide.height - 170) / 2, 170, 378, 170, 378,"statuswidget.png", NULL);
	}
	else{
		create_window(&gui.wndstatus, parent, (parent->width - 378) / 2, (parent->height - 170) / 2, 170, 378, 170, 378,"statuswidget.png", NULL);
	}

	if(!disable_controls){
		gui.wndstatus.action |= WIDGET_MOVEABLE;
		create_button(&gui.btnclose,&gui.wndstatus,750,10,34,34,34,34,"closebutton.png", NULL);
		gui.btnclose.mousepress = &status_btnclose_press;
		gui.btnclose.action |= WIDGET_ALWAYS_USEABLE;
	}

	if(accept && !disable_controls){
		create_button(&gui.btnok,&gui.wndstatus,22,290,50,405,50,405,"smallbutton.png", NULL);
		create_label(&gui.lblok, &gui.btnok,170,10,80,25,0,0,0,120,FALSE,2,8,FALSE,"OK");
		gui.btnok.mousepress = &status_btnok_press;
		gui.lblok.action |= WIDGET_CAN_CLICK_BEHIND;
	}

	create_label(&gui.lbltext, &gui.wndstatus,50,95,200,100,0,0,0,120,FALSE,2,12,FALSE,"INTILIZING RESOURCES...");

	widget_manual_focused(&gui.wndstatus);
	gui.wndstatus.draw = &draw_status;
	timer = glfwGetTime();
	counter = count + timer;
}

void status_btnclose_press(widget *control, int button, int pressed)
{
	canaccept = TRUE;
	accepted = TRUE;
}

void status_btnok_press(widget *control, int button, int pressed)
{
	accepted = TRUE;
}