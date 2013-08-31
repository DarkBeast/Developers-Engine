/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/
// /ENTRY:"mainCRTStartup"
#include <GL/glew.h>
#include <glfw3.h>
#include "input.h"
#include "program_path.h"
#include "render.h"
#include "text.h"
#include "mainmenu.h"
#include "bool.h"
#include "controls.h"
#include "error.h"

int main(void)
{
	get_program_directory();//gets the programs path.

	init_screen(800,600,NULL,NULL);//initializes the program window.

	glfwSetWindowCloseCallback(get_the_window(),handle_window_close);//checks if the window closed or not.
	glfwSetWindowSizeCallback(get_the_window(),handle_resize);//handles Window resize calls
	glfwSetCharCallback(get_the_window(),handle_char_callback);//handles character presses
	glfwSetKeyCallback(get_the_window(),handle_key_callback);
	glfwSetCursorPosCallback(get_the_window(),handle_mouse_pos);//handles mouse move events
	glfwSetMouseButtonCallback(get_the_window(),handle_mouse_press);//handles mouse button events
	glfwSetScrollCallback(get_the_window(),handle_mouse_wheel);//handles mouse wheel scrolling

	text_init_font("");//initializes the Font characters for use.

	widget_init_system();//initializes the widget system for storing widgets.

	init_main_menu();//initializes the first menu for use.

	main_menu();//Start the First Menu, Menu main loop.

	glfwDestroyWindow(get_the_window());
	glfwTerminate();// Close window and terminate GLFW
	path_destroy();
	exit(TRUE);// Exit program
}