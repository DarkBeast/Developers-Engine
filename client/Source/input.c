/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/

#include <stdio.h>
#include "render.h"
#include "input.h"
#include "widget.h"
#include "globals.h"
#include "error.h"
#include "general.h"

void handle_char_callback(GLFWwindow *screen, unsigned int key) //The character that was pressed.
{
	if(widget_get_focused()->type == CONTROL_TEXTBOX ){
		if(key > 128){
			error_handler(DE_ERROR_NONE_ACSII,"\n");
			return;
		}
		widget_get_focused()->controlkeypressed(widget_get_focused(),key);
	}
}

void handle_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) //The key that was pressed.
{
	if(widget_get_focused()->type == CONTROL_TEXTBOX && key == GLFW_KEY_BACKSPACE && action == GLFW_PRESS || widget_get_focused()->type == CONTROL_TEXTBOX && action == GLFW_REPEAT){
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
	else{
		check_input_keys(key,action);
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

	if(widget_check_focus()){
		if(pressed == 1){
			widget_mouse_press(widget_get_focused());
		}
		else{
			widget_mouse_release(widget_get_focused());
		}
	}
	else{
		if(pressed == 1){
			widget_mouse_press(widget_get_uip()->root);
		}
		else{
			widget_mouse_release(widget_get_uip()->root);
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

void check_input_keys(int button, int action)
{
	// clear anything pressed if unpressed

	if(action == GLFW_PRESS || action == GLFW_REPEAT){
		switch(button){
		case GLFW_KEY_RIGHT: tempdata()->dir = DIR_RIGHT; break;
		case GLFW_KEY_LEFT: tempdata()->dir = DIR_LEFT; break;
		case GLFW_KEY_UP: tempdata()->dir = DIR_UP; break;
		case GLFW_KEY_DOWN: tempdata()->dir = DIR_DOWN; break;
		case GLFW_KEY_LEFT_CONTROL:
		case GLFW_KEY_RIGHT_CONTROL: tempdata()->control = TRUE; break;
		case GLFW_KEY_LEFT_SHIFT:
		case GLFW_KEY_RIGHT_SHIFT: tempdata()->shift = TRUE; break;
			//case GLFW_KEY_ENTER: check_map_get_item(); break;
		default: return;
		}
	}
	else{
		switch(button){
		case GLFW_KEY_RIGHT:
		case GLFW_KEY_LEFT:
		case GLFW_KEY_UP:
		case GLFW_KEY_DOWN: tempdata()->dir = DIR_NONE; break;
		case GLFW_KEY_LEFT_CONTROL:
		case GLFW_KEY_RIGHT_CONTROL: tempdata()->control = FALSE; break;
		case GLFW_KEY_LEFT_SHIFT:
		case GLFW_KEY_RIGHT_SHIFT: tempdata()->shift = FALSE; break;
		default: return;
		}
	}
}