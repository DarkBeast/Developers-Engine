/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/
#include "bool.h"
#include <conio.h>
#include <GL/glew.h>
#include <glfw3.h>
#include <stdio.h> //For file reading/writing and other functions. open document for more info.
#include <stdlib.h>
#include "error.h"

//TODO: Add Error File Save and More Error Functions/FLAGS as well as safe Error types that just display an error but continue operation.

void error_handler(int error_flag)
{
	int i = 0;
	switch(error_flag){
	case DE_ERROR_MISC_ERROR:
		fputs("ERROR: Misc!\n", stderr);
		break;
	case DE_ERROR_POINTER_IN_USE:
		fputs("ERROR: Pointer in use!\n", stderr);
		break;
	case DE_ERROR_UNDEFINED_POINTER:
		fputs("ERROR: Undefined Pointer!\n", stderr);
		break;
	case DE_ERROR_OUT_OF_MEMORY:
		fputs("ERROR: Server Out Of Memory!\n", stderr);
		break;
	case DE_ERROR_IO_ERROR:
		fputs("ERROR: I/O Error!\n", stderr);
		break;
	case DE_ERROR_FILE_ERROR:
		fputs("ERROR: File Error!\n", stderr);
		break;
	case DE_ERROR_OVERFLOW_ERROR:
		fputs("ERROR: Overflow!\n", stderr);
		break;
	case DE_ERROR_SOCKET_ADDR_ERROR:
		fputs("ERROR: Could not establish server address!\n", stderr);
		break;
	case DE_ERROR_GLFWINIT_ERROR:
		fputs("ERROR: GLFW Did Not initialize!\n", stderr);
		break;
	case DE_ERROR_GLFWWIN_ERROR:
		fputs("ERROR: GLFW Did Not Open Window!\n", stderr);
		break;
	case DE_ERROR_IMAGELOADER_ERROR:
		fputs("ERROR: Image Did Not Load!\n", stderr);
		break;
	case DE_ERROR_PATH_INCORRECT:
		fputs("ERROR: Could not establish Program_Path!\n", stderr);
		break;
	case DE_ERROR_PATH_NULL:
		fputs("ERROR: Path is NULL.\n", stderr);
		break;
	case DE_ERROR_GLEWINIT_ERROR:
		fputs("ERROR: GLEW Did Not initialize!\n", stderr);
		break;
	case DE_ERROR_POINTER_NULL:
		fputs("ERROR: Pointer is NULL.\n", stderr);
		break;
	case DE_ERROR_NONE_ACSII:
		fputs("MESSAGE: Does Not Support Unicode characters.\n", stderr);
		return;
		break;
	case DE_ERROR_SOCKET_INIT:
		fputs("ERROR: Socket did not initialize.\n", stderr);
		return;
		break;
	case DE_ERROR_SOCKET_WSASTARTUP:
		fputs("ERROR: windows socket startup error.\n", stderr);
		return;
		break;
	case DE_ERROR_SOCKET_SEND_SIZE:
		fputs("MESSAGE: send size was incorrect\n", stderr);
		return;
		break;
	case DE_ERROR_SOCKET_CONNECTION_LOSS:
		fputs("MESSAGE: connection was lost\n", stderr);
		return;
		break;
	case DE_ERROR_INVALID_PACKET_ID:
		fputs("MESSAGE: Packet ID is invalid.\n", stderr);
		return;
		break;
	case DE_ERROR_INVALID_PACKET_OFFSET:
		fputs("MESSAGE: Packet offset equals zero.\n", stderr);
		return;
		break;
	default:
		fputs("ERROR: Unknown Error!\n", stderr);
	}
	fputs("The Application will now close.", stderr);
	i = getchar();
	shut_down();
}

void shut_down(void)
{
	//TODO: Clean up memory, shut down sockets, etc....?
	glfwTerminate();
	exit( FALSE);
}