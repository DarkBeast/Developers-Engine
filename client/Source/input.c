/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/

#include <stdio.h>
#include "render.h"
#include "input.h"
#include "widget.h"
#include "globals.h"
#include "error.h"

void handle_char_callback(GLFWwindow *screen, unsigned int key) //The character that was pressed.
{
	if(widget_get_focused()->type == CONTROL_TEXTBOX ){
		if(key > 128){
			error_handler(DE_ERROR_NONE_ACSII);
			return;
		}
		widget_get_focused()->controlkeypressed(widget_get_focused(),key);
	}
}

void handle_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) //The key that was pressed.
{
	if(widget_get_focused()->type == CONTROL_TEXTBOX && key == GLFW_KEY_BACKSPACE && action == GLFW_PRESS || action == GLFW_REPEAT){
		switch(key){
		case GLFW_KEY_BACKSPACE:
			if(action == GLFW_PRESS || action == GLFW_REPEAT){
				widget_get_focused()->controlkeypressed(widget_get_focused(),key);
			}

			break;
		default:
			break;
		}
	}
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
			widget_mouse_press(widget_get_focused());
		}
		else{
			widget_mouse_release(widget_get_focused());
		}
	}
	else{
		if(pressed == 1){
			widget_mouse_press(widget_get_ui().root);
		}
		else{
			widget_mouse_release(widget_get_ui().root);
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