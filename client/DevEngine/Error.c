/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/

#include <conio.h>
#include <GL/glew.h>
#include <glfw.h>
#include <stdio.h> //For file reading/writing and other functions. open document for more info.
#include <stdlib.h>
#include "error.h"
#include "bool.h"

//TODO: Add Error File Save and More Error Functions/FLAGS as well as safe Error types that just display an error but continue operation.

void fatal_error(int error_flag)
{
	switch(error_flag){
	case ERROR_MISC_ERROR:
		fputs("An error has occurred!\n", stderr);
		break;
	case ERROR_POINTER_IN_USE:
		fputs("An error has occurred: Pointer in use!\n", stderr);
		break;
	case ERROR_UNDEFINED_POINTER:
		fputs("An error has occurred: Undefined Pointer!\n", stderr);
		break;
	case ERROR_OUT_OF_MEMORY:
		fputs("An error has occurred: Server Out Of Memory!\n", stderr);
		break;
	case ERROR_IO_ERROR:
		fputs("An error has occurred: I/O Error!\n", stderr);
		break;
	case ERROR_FILE_ERROR:
		fputs("An error has occurred: File Error!\n", stderr);
		break;
	case ERROR_OVERFLOW_ERROR:
		fputs("An error has occurred: Overflow!\n", stderr);
		break;
	case ERROR_SOCKET_ADDR_ERROR:
		fputs("Could not establish server address!\n", stderr);
		break;
	case ERROR_PATH_INCORRECT:
		fputs("Could not establish Program_Path!\n", stderr);
		break;
	case ERROR_PATH_NULL:
		fputs("ERROR:Path is NULL.\n", stderr);
		break;
	case ERROR_POINTER_NULL:
		fputs("ERROR:Pointer is NULL.\n", stderr);
		break;
	default:
		fputs("An error has occurred!\n", stderr);
	}
	fputs("The Application will now close.", stderr);
	getchar();
	shut_down();
}

void render_error(int error_flag)
{
	switch(error_flag){
	case ERROR_MISC_ERROR:
		fputs("An error has occurred!\n", stderr);
		break;
	case ERROR_POINTER_IN_USE:
		fputs("An error has occurred: Pointer in use!\n", stderr);
		break;
	case ERROR_UNDEFINED_POINTER:
		fputs("An error has occurred: Undefined Pointer!\n", stderr);
		break;
	case ERROR_OUT_OF_MEMORY:
		fputs("An error has occurred: Server Out Of Memory!\n", stderr);
		break;
	case ERROR_IO_ERROR:
		fputs("An error has occurred: I/O Error!\n", stderr);
		break;
	case ERROR_FILE_ERROR:
		fputs("An error has occurred: File Error!\n", stderr);
		break;
	case ERROR_OVERFLOW_ERROR:
		fputs("An error has occurred: Overflow!\n", stderr);
		break;
	case ERROR_GLFWINIT_ERROR:
		fputs("An error has occurred: GLFW Did Not initialize!\n", stderr);
		break;
	case ERROR_GLFWWIN_ERROR:
		fputs("An error has occurred: GLFW Did Not Open Window!\n", stderr);
		break;
	case ERROR_IMAGELOADER_ERROR:
		fputs("An error has occurred: Image Did Not Load!\n", stderr);
		break;
	case ERROR_GLEWINIT_ERROR:
		fputs("An error has occurred: GLEW Did Not initialize!\n", stderr);
		break;
	default:
		fputs("An error has occurred!\n", stderr);
	}
	fputs("The Application will now close.", stderr);
	getchar();
	shut_down();
}

void shut_down(void)
{
	//TODO: Clean up memory, shut down sockets, etc....?
	glfwTerminate();
	exit( FALSE);
}