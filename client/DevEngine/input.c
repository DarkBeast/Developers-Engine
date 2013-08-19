/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/

#include <stdio.h>
#include "render.h"
#include "input.h"
#include "widget.h"
#include "globals.h"

void handle_key_press(GLFWwindow * screen, int key, int scancode, int press, int mods) //The key that was pressed
{
	//TODO: make HANDLE ARRAY FOR KEYS THAT ARE PUSHED SO IF MULTIPLE KEYS ARE PRESSED IT CAN HANDLE THEM ALL.
}

void handle_mouse_pos(GLFWwindow * screen, double x, double y)
{
	widget_move((int16)x,(int16)y);
	widget_set_mouse_pos((int16)x,(int16)y);

#if(DEBUG)
	printf(" x = %i\n", x);
	printf(" Y = %i\n", y);
#endif
}

void handle_mouse_press(GLFWwindow * screen, int button, int pressed, int mods)
{
	widget_set_ui_click_event((int8)button, (int8)pressed);

	if( widget_check_focus()){
		if(pressed == 1){
			widget_focused_mouse_press();
		}
		else{
			widget_focused_mouse_release();
		}
	}
	else{
		if(pressed == 1){
			widget_mouse_press();
		}
		else{
			widget_mouse_release();
		}
	}
}

void handle_mouse_wheel(GLFWwindow * screen, double x, double y)
{
}

void handle_window_close(GLFWwindow * screen)
{
	set_if_window_should_close();
}