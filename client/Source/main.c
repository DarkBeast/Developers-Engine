/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/
// /ENTRY:"mainCRTStartup"
#include <GL/glew.h>
#include <glfw3.h>
#include "input.h"
#include "path.h"
#include "render.h"
#include "text.h"
#include "bool.h"
#include "main.h"
#include "handlepackets.h"
#include "winsocket.h"
#include "general.h"
#include "status.h"

#define _DEBUG 0

#if _DEBUG
#include <vld.h>
#endif

mtx_t gmutex;
cnd_t gcond;
thrd_t t1;

mtx_t *get_mutex(void)
{
	return &gmutex;
}

int main(void)
{
	mtx_init(&gmutex, mtx_plain);
	cnd_init(&gcond);

	init_path(); //allocates the path size.

	init_screen(800,600,NULL,NULL);//initializes the program window.
	glfwSetWindowPos(get_the_window(),400,400);
	glfwSetWindowCloseCallback(get_the_window(),handle_window_close);//checks if the window closed or not.
	//glfwSetWindowSizeCallback(get_the_window(),handle_resize);//handles Window resize calls
	glfwSetCharCallback(get_the_window(),handle_char_callback);//handles character presses
	glfwSetKeyCallback(get_the_window(),handle_key_callback);
	glfwSetCursorPosCallback(get_the_window(),handle_mouse_pos);//handles mouse move events
	glfwSetMouseButtonCallback(get_the_window(),handle_mouse_press);//handles mouse button events
	glfwSetScrollCallback(get_the_window(),handle_mouse_wheel);//handles mouse wheel scrolling
	text_init_font("");//initializes the Font characters for use.

	widget_init_system();//initializes the widget system for storing widgets.

	set_status_type(STATUS_TYPE_LOADER);
	thrd_create(&t1, load_data, (void*)0);
	menustate();

	unload_functions();
}

int load_data(void *arg)
{
	_sleep(700);
	status_message("Loading Packet Cache");
	init_packets();
	_sleep(700);
	status_message("Loading Client Resources");
	init_client();
	_sleep(700);
	status_message("Loading Sockets");
	initsocket();
	_sleep(700);
	status_message("Checking if server is online.");

	if(socketconnect())
		thrd_create(&t1, socketlisten, (void*)0);

	set_menu_state(MENU_STATE_MAIN);
	return TRUE;
}

void unload_functions(void)
{
	glfwDestroyWindow(get_the_window());
	glfwTerminate();// Close window and terminate GLFW
	destroy_client();
	widget_unload_system();
	unload_fonts();
	mtx_destroy(&gmutex);
	cnd_destroy(&gcond);

#if _DEBUG
	VLDReportLeaks();
#endif

	exit(TRUE);// Exit program
}