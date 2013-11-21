/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
*           Anthony Ianozi
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
thrd_t t2;

int commands(void *arg);

int main(void)
{
	char *input = (char *)calloc(2000, sizeof(char));
	sbool n = TRUE;
	SetConsoleTitle(TITLE);

	mtx_init(&gmutex, mtx_plain);
	cnd_init(&gcond);

	init_timer();
	init_path();
	start_socket();
	init_server();
	thrd_create(&t2, commands, (void*)0);
	server_loop();
	mtx_destroy(&gmutex);
	cnd_destroy(&gcond);
	exit(TRUE);// Exit program
}

int commands(void *arg)
{
	char *p = (char *)calloc(256, sizeof(char));
	sbool running = TRUE;

	puts("Type /help for a list of commands");
reloop:
	while(running){
		if(gets(p)){
			switch(*p){
			case '/':
				{
					switch(*(++p)){
					case 's':
						{
							if( *(p + 1) == 'e' && *(p + 2) == 't' && *(p + 3) == 'g' && *(p + 4) == 'r' && *(p + 5) == 'o' && *(p + 6) == 'u' && *(p + 7) == 'p'){
								char user[MAX_NAME_LENGTH];
								int i = 0;
								int group;
								p += 8; //move to name.

								while(*p){
									if(i > MAX_NAME_LENGTH){
										puts("Invalid User Name");
										goto reloop;
									}
									user[i] = *(++p);
									if(user[i] == ' '){
										user[i] = '\0';
										break;
									}
									i++;
								}

								switch(*(++p)){
								case '0': group = GROUP_USER; break;
								case '1': group = GROUP_MONITOR; break;
								case '2': group = GROUP_MAPPER; break;
								case '3': group = GROUP_DEVELOPER; break;
								case '4': group = GROUP_CREATOR; break;
								default: goto reloop;
								}

								if(i = find_player(user) > -1){
									player(i)->group = group;
									puts("Group was changed");
								}
								break;
							}

							if( *(p + 1) == 'h' && *(p + 2) == 'u' && *(p + 3) == 't' && *(p + 4) == 'd' && *(p + 5) == 'o' && *(p + 6) == 'w' && *(p + 7) == 'n')
								set_shut_down();

							break;
						}
					case 'c':
						if( *(p + 1) == 'l' && *(p + 2) == 'o' && *(p + 3) == 's' && *(p + 4) == 'e'){
							destroy_server();
							return;
						}

					default:
						puts("Invalid command, Type /help for a list of commands"); break;
					}
					break;
				}
			default:
				puts("invalid command, type /help for a list of commands"); break;
			}
		}
	}
}