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

//#define _DEBUG 1
//#include <vld.h>

mtx_t gmutex;
cnd_t gcond;

mtx_t *get_mutex(void)
{
	return &gmutex;
}

int main(void)
{
	thrd_t t1;

	mtx_init(&gmutex, mtx_plain);
	cnd_init(&gcond);

	init_path(); //allocates the path size.

	init_screen(800,600,NULL,NULL);//initializes the program window.

	glfwSetWindowCloseCallback(get_the_window(),handle_window_close);//checks if the window closed or not.
	//glfwSetWindowSizeCallback(get_the_window(),handle_resize);//handles Window resize calls
	glfwSetCharCallback(get_the_window(),handle_char_callback);//handles character presses
	glfwSetKeyCallback(get_the_window(),handle_key_callback);
	glfwSetCursorPosCallback(get_the_window(),handle_mouse_pos);//handles mouse move events
	glfwSetMouseButtonCallback(get_the_window(),handle_mouse_press);//handles mouse button events
	glfwSetScrollCallback(get_the_window(),handle_mouse_wheel);//handles mouse wheel scrolling
	text_init_font("");//initializes the Font characters for use.

	widget_init_system();//initializes the widget system for storing widgets.
	mtx_lock(&gmutex);
	//init_status_window(FALSE,100000);
	//thrd_create(&t1, status, (void*)0);
	//set_status("UI SYSTEM LOADED...");

	//set_status("INITIALIZING PACKETS...");
	init_packets();
	//set_status("INITIALIZING RESOURCES...");
	init_client();
	//set_status("INITIALIZING SOCKET...");
	initsocket();
	//set_status("CONNECTING SOCKET...");
	//if(socketconnect()) //just to test for server connection.
	//	set_status("CONNECTION SUCCESSFUL...");
	//else
	//	set_status("CONNECTION UNSUCCESSFUL...");

	//thrd_create(&t1, socketlisten, (void*)0);
	//set_status("LOADING MENU...");

	//status_unload();
	mtx_unlock(&gmutex);
	menustate();
	unload_functions();
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
	exit(TRUE);// Exit program
}