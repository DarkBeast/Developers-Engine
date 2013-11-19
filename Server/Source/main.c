/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/
#include "bool.h"
#include "path.h"
#include "general.h"
#include "tinycthread.h"
#include "socket.h"
#include "function.h"
#include <stdio.h>
#include "serverloop.h"

mtx_t gmutex;
cnd_t gcond;

int main(void)
{
	sbool n = TRUE;
	SetConsoleTitle(TITLE);

	mtx_init(&gmutex, mtx_plain);
	cnd_init(&gcond);

	init_timer();
	init_path();
	start_socket();
	init_server();
	server_loop();

	while(n){
		if(getchar() == 'q'){
			path_destroy();
			destroy_server();
			mtx_destroy(&gmutex);
			cnd_destroy(&gcond);
			n = FALSE;
		}
	}

	exit(TRUE);// Exit program
}