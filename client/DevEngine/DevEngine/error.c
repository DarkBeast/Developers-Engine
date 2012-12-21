/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/

#include <conio.h>
#include <glfw.h>
#include <stdio.h>
#include <stdlib.h>
#include "error.h"
#include "globals.h"

//TODO: Add Error File Save

void fatalerror(int error_flag)
{
	switch(error_flag)
	{
	case MISC_ERROR:
		fputs("An error has occurred!\n", stderr);
		break;
	case POINTER_IN_USE:
		fputs("An error has occurred: Pointer in use!\n", stderr);
		break;
	case UNDEFINED_POINTER:
		fputs("An error has occurred: Undefined Pointer!\n", stderr);
		break;
	case OUT_OF_MEMORY:
		fputs("An error has occurred: Server Out Of Memory!\n", stderr);
		break;
	case IO_ERROR:
		fputs("An error has occurred: I/O Error!\n", stderr);
		break;
	case FILE_ERROR:
		fputs("An error has occurred: File Error!\n", stderr);
		break;
	case OVERFLOW_ERROR:
		fputs("An error has occurred: Overflow!\n", stderr);
		break;
	case SOCKET_ADDR_ERROR:
		fputs("Could not establish server address!\n", stderr);
		break;
	default:
		fputs("An error has occurred!\n", stderr);
	}
	fputs("The Application will now close.", stderr);
	_getch();
	shutdown();
}

void rendererror(int error_flag)
{
	switch(error_flag)
	{
	case MISC_ERROR:
		fputs("An error has occurred!\n", stderr);
		break;
	case POINTER_IN_USE:
		fputs("An error has occurred: Pointer in use!\n", stderr);
		break;
	case UNDEFINED_POINTER:
		fputs("An error has occurred: Undefined Pointer!\n", stderr);
		break;
	case OUT_OF_MEMORY:
		fputs("An error has occurred: Server Out Of Memory!\n", stderr);
		break;
	case IO_ERROR:
		fputs("An error has occurred: I/O Error!\n", stderr);
		break;
	case FILE_ERROR:
		fputs("An error has occurred: File Error!\n", stderr);
		break;
	case OVERFLOW_ERROR:
		fputs("An error has occurred: Overflow!\n", stderr);
		break;
	case GLFWINIT_ERROR:
		fputs("An error has occurred: GLFW Did Not Intalize!\n", stderr);
		break;
	case GLFWWIN_ERROR:
		fputs("An error has occurred: GLFW Did Not Open Window!\n", stderr);
		break;
	case IMAGELOADER_ERROR:
		fputs("An error has occurred: Image Did Not Load!\n", stderr);
		break;
	default:
		fputs("An error has occurred!\n", stderr);
	}
	fputs("The Application will now close.", stderr);
	_getch();
	shutdown();
}

void shutdown(void)
{
	//TODO: Clean up memory, shut down sockets, etc....?
	glfwTerminate();
	exit( FALSE );
}