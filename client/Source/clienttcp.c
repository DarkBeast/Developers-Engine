/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/
#include "clienttcp.h"

void sendlogin(char *name, char *password)
{
	buffer_t buffer;

	clear_buffer(&buffer);
	add_opcode(&buffer,CLOGIN);
	add_string(&buffer,name);
	add_string(&buffer,password);
}