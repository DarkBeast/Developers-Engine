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
#include "main.h"
#include "widget.h"
#include "path.h"

//TODO: Add Error File Save and More Error Functions/FLAGS as well as safe Error types that just display an error but continue operation.

void error_handler(int error_flag, const char *string)
{
	switch(error_flag){
	case DE_ERROR_MISC_ERROR:
		add_error_log(comb_2str("ERROR: Misc! ::",string));
		break;
	case DE_ERROR_POINTER_IN_USE:
		add_error_log(comb_2str("ERROR: Pointer in use! ::",string));
		break;
	case DE_ERROR_UNDEFINED_POINTER:
		add_error_log(comb_2str("ERROR: Undefined Pointer! ::",string));
		break;
	case DE_ERROR_OUT_OF_MEMORY:
		add_error_log(comb_2str("ERROR: Server Out Of Memory! ::",string));
		break;
	case DE_ERROR_IO_ERROR:
		add_error_log(comb_2str("ERROR: I/O Error! ::",string));
		break;
	case DE_ERROR_FILE_ERROR:
		add_error_log(comb_2str("ERROR: File Error! ::",string));
		break;
	case DE_ERROR_OVERFLOW_ERROR:
		add_error_log("ERROR: Overflow!\n");
		break;
	case DE_ERROR_SOCKET_ADDR_ERROR:
		add_error_log("ERROR: Could not establish server address!\n");
		break;
	case DE_ERROR_GLFWINIT_ERROR:
		add_error_log("ERROR: GLFW Did Not initialize!\n");
		break;
	case DE_ERROR_GLFWWIN_ERROR:
		add_error_log("ERROR: GLFW Did Not Open Window!\n");
		break;
	case DE_ERROR_IMAGELOADER_ERROR:
		add_error_log(comb_2str("ERROR: Image Did Not Load! ::",string));
		break;
	case DE_ERROR_PATH_INCORRECT:
		add_error_log(comb_2str("ERROR: Could not establish Program_Path! ::",string));
		break;
	case DE_ERROR_PATH_NULL:
		add_error_log(comb_2str("ERROR: Path is NULL. ::",string));
		break;
	case DE_ERROR_GLEWINIT_ERROR:
		add_error_log("ERROR: GLEW Did Not initialize!\n");
		break;
	case DE_ERROR_POINTER_NULL:
		add_error_log(comb_2str("ERROR: Pointer is NULL. ::",string));
		break;
	case DE_ERROR_NONE_ACSII:
		add_error_log("MESSAGE: Does Not Support Unicode characters.\n");
		return;
	case DE_ERROR_SOCKET_INIT:
		add_error_log("ERROR: Socket did not initialize.\n");
		return;
	case DE_ERROR_SOCKET_WSASTARTUP:
		add_error_log("ERROR: windows socket startup error.\n");
		return;
	case DE_ERROR_SOCKET_SEND_SIZE:
		add_error_log(comb_2str("MESSAGE: send size was incorrect. ::",string));
		return;
	case DE_ERROR_SOCKET_CONNECTION_LOSS:
		add_error_log("MESSAGE: connection was lost\n");
		return;
	case DE_ERROR_INVALID_PACKET_ID:
		add_error_log(comb_2str("MESSAGE: Packet ID is invalid. ::",string));
		return;
	case DE_ERROR_INVALID_PACKET_OFFSET:
		add_error_log(comb_2str("MESSAGE: Packet offset equals zero. ::",string));
		return;
	default:
		add_error_log("ERROR: Unknown Error!\n");
	}

	widget_unload(widget_get_uip()->root);
	unload_functions();
}

void add_error_log(char *string)
{
	FILE *fp;
	time_t current_time;
	char* c_time_string;
	current_time = time(NULL);

	c_time_string = ctime(&current_time);

	if(!file_exists(".\\logs\\error.txt")){
		if((fp = fopen(".\\logs\\error.txt", "w")) == NULL)
			fputs("ERROR: File Error!\n", stderr);
		fclose(fp);
	}

	if((fp = fopen(".\\logs\\error.txt", "r+")) == NULL)
		fputs("ERROR: File Error!\n", stderr);

	c_time_string[strlen(c_time_string) - 1] = '\0';

	fseek(fp, 0, SEEK_END);
	fprintf(fp, "\n %s: %s", c_time_string, string);

	fclose(fp);
}