/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis      
******************************************************************************/

#ifndef _ERROR_H
#define _ERROR_H

// Fatal Error Flags
#define MISC_ERROR        -1
#define NULL_ERROR         0
#define POINTER_IN_USE     1
#define UNDEFINED_POINTER  2
#define OUT_OF_MEMORY      3
#define IO_ERROR           4
#define FILE_ERROR         5
#define OVERFLOW_ERROR     6
#define SOCKET_ADDR_ERROR  7
#define GLFWINIT_ERROR     8
#define GLFWWIN_ERROR	   9
#define IMAGELOADER_ERROR  10

void fatalerror(int error_flag);
void rendererror(int error_flag);
void shutdown();
#endif