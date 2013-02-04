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

int main(void)
{
	get_program_directory();

	init_screen(800,600,GLFW_WINDOW);
	glfwEnable(GLFW_KEY_REPEAT);

	glfwSetWindowSizeCallback(handle_resize);//handles Window resize calls
	glfwSetKeyCallback(handle_key_press);//handles Key presses
	glfwSetMousePosCallback(handle_mouse_pos);//handles mouse move events
	glfwSetMouseButtonCallback(handle_mouse_press);//handles mouse button events
	glfwSetMouseWheelCallback(handle_mouse_wheel);//handles mouse wheel scrolling

	init_font("");
	widget_init_system();
	init_main_menu();

	// Main loop
	main_menu();

	// Close window and terminate GLFW
	glfwTerminate();

	// Exit program
	exit(TRUE);
}