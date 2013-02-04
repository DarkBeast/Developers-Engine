/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/

#include <glfw.h>
#include <stdio.h>
#include "input.h"
#include "widget.h"
#include "globals.h"

void GLFWCALL handle_key_press(int key,int press) //The key that was pressed
{
	//TODO: make HANDLE ARRAY FOR KEYS THAT ARE PUSHED SO IF MULTIPLE KEYS ARE PRESSED IT CAN HANDLE THEM ALL.
}

void GLFWCALL handle_mouse_pos(int x,int y)
{
	widget_move((int16)x,(int16)y);
	widget_set_mouse_pos((int16)x,(int16)y);

#if(DEBUG)
	printf(" x = %i\n", x);
	printf(" Y = %i\n", y);
#endif
}

void GLFWCALL handle_mouse_press(int button,int pressed)
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

void GLFWCALL handle_mouse_wheel(int moved)
{
}