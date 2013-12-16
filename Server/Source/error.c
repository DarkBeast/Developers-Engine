/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/
#include "bool.h"
#include <stdio.h>
#include <stdlib.h>
#include "error.h"

//TODO: Add Error File Save and More Error Functions/FLAGS as well as safe Error types that just display an error but continue operation.

void error_handler(int error_flag)
{
	int i = 0;
	switch(error_flag){
	case DE_ERROR_MISC_ERROR:
		printf("ERROR: Misc!\n");
		break;
	case DE_ERROR_POINTER_IN_USE:
		printf("ERROR: Pointer in use!\n");
		break;
	case DE_ERROR_UNDEFINED_POINTER:
		printf("ERROR: Undefined Pointer!\n");
		break;
	case DE_ERROR_OUT_OF_MEMORY:
		printf("ERROR: Server Out Of Memory!\n");
		break;
	case DE_ERROR_IO_ERROR:
		printf("ERROR: I/O Error!\n");
		break;
	case DE_ERROR_FILE_ERROR:
		printf("ERROR: File Error!\n");
		break;
	case DE_ERROR_OVERFLOW_ERROR:
		printf("ERROR: Overflow!\n");
		break;
	case DE_ERROR_SOCKET_ADDR_ERROR:
		printf("ERROR: Could not establish server address!\n");
		break;
	case DE_ERROR_PATH_INCORRECT:
		printf("ERROR: Could not establish Program_Path!\n");
		break;
	case DE_ERROR_PATH_NULL:
		printf("ERROR: Path is NULL.\n");
		break;
	case DE_ERROR_POINTER_NULL:
		printf("ERROR: Pointer is NULL.\n");
		break;
	case DE_ERROR_SOCKET_INIT:
		printf("ERROR: Socket did not initialize.\n");
		return;
	case DE_ERROR_SOCKET_LISTEN:
		printf("ERROR: Socket could not listen to port.\n");
		return;
	case DE_ERROR_SOCKET_SELECT_FAILED:
		printf("ERROR: Socket SELECT initialization failed.\n");
		return;
	case DE_ERROR_SOCKET_SELECT_TIMED_OUT:
		printf("ERROR: Socket timed out.\n");
		return;
	case DE_ERROR_SOCKET_ACCEPT:
		printf("ERROR: Socket could not accept. Must be full.\n");
		return;
	case DE_ERROR_SOCKET_RECEIVE:
		printf("ERROR: Socket could not receive data.\n");
		return;
	case DE_ERROR_SOCKET_WSASTARTUP:
		printf("ERROR: windows socket startup error.\n");
		return;
	case DE_ERROR_SOCKET_SEND_SIZE:
		printf("MESSAGE: send size was incorrect\n");
		return;
	case DE_ERROR_SOCKET_CONNECTION_LOSS:
		printf("MESSAGE: connection was lost\n");
		return;
	case DE_ERROR_SOCKET_OPTION_SET_REUSE:
		printf("MESSAGE: could not set socket option reuse.\n");
		return;
	case DE_ERROR_SOCKET_OPTION_SET_NONBLOCK:
		printf("MESSAGE: could not set socket to NONBLOCK mode.\n");
		return;
	case DE_ERROR_INVALID_PACKET_ID:
		printf("MESSAGE: Packet ID is invalid.\n");
		return;
	case DE_ERROR_INVALID_PACKET_OFFSET:
		printf("MESSAGE: Packet offset equals zero.\n");
		return;
	default:
		printf("ERROR: Unknown Error!\n");
	}
	printf("The Application will now close.\n");
	i = getchar();
	shut_down();
}

void shut_down(void)
{
	exit( FALSE);
}