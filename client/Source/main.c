﻿/*******************************************************************************
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

#include "tinycthread.h"
#include "handlepackets.h"
#include "winsocket.h"
#include "clienttcp.h"

mtx_t gmutex;
cnd_t gcond;

int main(void)
{
	thrd_t t1;

	mtx_init(&gmutex, mtx_plain);
	cnd_init(&gcond);

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

	//init_socket();
	//init_packets();
	//sendlogin("me","you");
	init_main_menu();//initializes the first menu for use.
	//thrd_create(&t1, socket_loop, (void*)0);
	initsocket();
	socketconnect();
	main_menu();//Start the First Menu, Menu main loop.

	glfwDestroyWindow(get_the_window());
	glfwTerminate();// Close window and terminate GLFW
	path_destroy();
	//destory_client_socket();
	mtx_destroy(&gmutex);
	cnd_destroy(&gcond);
	exit(TRUE);// Exit program
}