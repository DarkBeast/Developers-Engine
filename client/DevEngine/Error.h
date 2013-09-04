/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/

#ifndef DE_ERROR_H
#define DE_ERROR_H

// Fatal Error Flags
typedef enum error_flags_t error_flags_t;

enum error_flags_t
{
	ERROR_MISC_ERROR = -1,
	ERROR_NULL_ERROR = 0,
	ERROR_POINTER_IN_USE  = 1,
	ERROR_UNDEFINED_POINTER = 2,
	ERROR_OUT_OF_MEMORY = 3,
	ERROR_IO_ERROR = 4,
	ERROR_FILE_ERROR = 5,
	ERROR_OVERFLOW_ERROR = 6,
	ERROR_SOCKET_ADDR_ERROR = 7,
	ERROR_GLFWINIT_ERROR = 8,
	ERROR_GLFWWIN_ERROR = 9,
	ERROR_IMAGELOADER_ERROR = 10,
	ERROR_PATH_INCORRECT = 11,
	ERROR_PATH_NULL = 12,
	ERROR_GLEWINIT_ERROR = 13,
	ERROR_POINTER_NULL = 14,
	ERROR_NONE_ACSII = 15
};

void fatal_error(int error_flag);
void render_error(int error_flag);
void shut_down(void);
#endif