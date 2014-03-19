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
thrd_t t1, t2;

#define _DEBUG 1

#if _DEBUG
#include <vld.h>
#endif

int commands(void *arg);

int main(void)
{
	SetConsoleTitle(TITLE);

	mtx_init(&gmutex, mtx_plain);
	cnd_init(&gcond);

	init_timer();
	init_path();
	init_server();
	thrd_create(&t1, initsocket, (void*)0);
	thrd_create(&t2, commands, (void*)0);
	server_loop();
	destroy_server();
	//TerminateThread(t2,0);
	mtx_destroy(&gmutex);
	cnd_destroy(&gcond);
#if _DEBUG
	VLDReportLeaks();
#endif

	exit(TRUE);// Exit program
}

int commands(void *arg)
{
	char *p = NULL;
	char c[500];
	sbool running = TRUE;
	p = c;

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
							if(comp_str(p, "etgroup")){
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

							if( comp_str(++p, "hutdown")){
								set_shut_down();
							}

							break;
						}
					case 'c':
						if( comp_str(++p, "lose")){
							set_server_offline();
							return 0;
						}
						break;
					case 'h':
						if( comp_str(++p, "elp")){
							puts("::::Server Commands::::");
							puts("/close: Instant shutdown of server.");
							puts("/shutdown: Timed shutdown of server.");
							puts("/setgroup username groupid: Sets a players group via server.");
						}
						break;
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
	return 0;
}