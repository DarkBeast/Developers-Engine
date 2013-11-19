/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/

#ifndef DE_ERROR_H
#define DE_ERROR_H

// Fatal Error Flags
typedef enum error_flags_t error_flags_t;

enum error_flags_t
{
	DE_ERROR_MISC_ERROR = -1,
	DE_ERROR_NULL_ERROR = 0,
	DE_ERROR_POINTER_IN_USE,
	DE_ERROR_UNDEFINED_POINTER,
	DE_ERROR_OUT_OF_MEMORY,
	DE_ERROR_IO_ERROR,
	DE_ERROR_FILE_ERROR,
	DE_ERROR_OVERFLOW_ERROR,
	DE_ERROR_SOCKET_ADDR_ERROR,
	DE_ERROR_GLFWINIT_ERROR,
	DE_ERROR_GLFWWIN_ERROR,
	DE_ERROR_IMAGELOADER_ERROR,
	DE_ERROR_PATH_INCORRECT,
	DE_ERROR_PATH_NULL,
	DE_ERROR_GLEWINIT_ERROR,
	DE_ERROR_POINTER_NULL,
	DE_ERROR_NONE_ACSII,
	DE_ERROR_SOCKET_INIT,
	DE_ERROR_SOCKET_WSASTARTUP,
	DE_ERROR_SOCKET_SEND_SIZE,
	DE_ERROR_SOCKET_CONNECTION_LOSS,
	DE_ERROR_INVALID_PACKET_ID,
	DE_ERROR_INVALID_PACKET_OFFSET
};

void error_handler(int error_flag);
void shut_down(void);
#endif