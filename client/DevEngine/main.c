/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/
// /ENTRY:"mainCRTStartup"
#include <glfw.h>
#include "input.h"
#include "program_path.h"
#include "render.h"
#include "text.h"
#include "mainmenu.h"
#include "bool.h"
#include "controls.h"

int main(void)
{
	get_program_directory();//gets the programs path so we can use ti to retrieve image, saved files etc.

	init_screen(800,600,GLFW_WINDOW);//initializes the program window.
	glfwEnable(GLFW_KEY_REPEAT);//allows repeating of held down keyboard keys.

	glfwSetWindowSizeCallback(handle_resize);//handles Window resize calls
	glfwSetKeyCallback(handle_key_press);//handles Key presses
	glfwSetMousePosCallback(handle_mouse_pos);//handles mouse move events
	glfwSetMouseButtonCallback(handle_mouse_press);//handles mouse button events
	glfwSetMouseWheelCallback(handle_mouse_wheel);//handles mouse wheel scrolling

	text_init_font("");//initializes the Font characters for use.

	widget_init_system();//initializes the widget system for storing widgets.

	load_control_images();//Loads the Default control images into an array.

	init_main_menu();//initializes the first menu for use.

	main_menu();//Start the First Menu, Menu main loop.

	glfwTerminate();// Close window and terminate GLFW

	exit(TRUE);// Exit program
}