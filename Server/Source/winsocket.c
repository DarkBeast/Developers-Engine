/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/

//TODO: change socket to use libevents.
#include "winsocket.h"
#include "socket.h"
#include "error.h"
#include "handlepackets.h"
#include <stdio.h>
#include <errno.h>

desocket *sock; //short for socket
WSADATA wsadata;

void initsocket(void)
{
	int on = 1;

	sock = (desocket *)calloc(1,sizeof(desocket));

	//load windows socket library 2.2.
	if (WSAStartup(0x0202, &wsadata) != 0)
		error_handler(DE_ERROR_SOCKET_WSASTARTUP);

	//set socket to TCP.
	if ((sock->socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		error_handler(DE_ERROR_SOCKET_INIT);

	//Allow reuse of address.
	if(setsockopt(sock->socket, SOL_SOCKET,  SO_REUSEADDR, (char *)&on, sizeof(on)) < 0)
		error_handler(DE_ERROR_SOCKET_OPTION_SET_REUSE);

	//set socket to none blocking mode.
	if(ioctlsocket(sock->socket, FIONBIO, (u_long *)&on) != 0)
		error_handler(DE_ERROR_SOCKET_OPTION_SET_NONBLOCK);

	//set socket address
	sock->host_port = SERVER_PORT;
	sock->address_length = sizeof(struct sockaddr_in);

	memset(&sock->host_address, 0, sock->address_length);     /* Zero out structure */
	sock->host_address.sin_family      = AF_INET;             /* Internet address family */
	sock->host_address.sin_addr.s_addr = htonl(INADDR_ANY);   /* Server IP address */
	sock->host_address.sin_port        = htons(sock->host_port); /* Server port */

	bind(sock->socket, (struct sockaddr*)&sock->host_address, sock->address_length);

	//listen for connections.
	if(listen(sock->socket, SOMAXCONN) == -1)
		error_handler(DE_ERROR_SOCKET_LISTEN);

	//initialize master set for select.
	FD_ZERO(&sock->master_set);
	FD_ZERO(&sock->working_set);
	sock->max_socket = sock->socket;
	FD_SET(sock->socket, &sock->master_set);
}

int socketlisten(void *arg)
{
	sbool end_listen = FALSE;
	struct sockaddr_in temp_address;
	int temp_length = sock->address_length;
	uint64 i = 0;
	sock->connected = TRUE;

	do
	{
		memcpy(&sock->working_set, &sock->master_set, sizeof(sock->master_set));

		//sock->working_set = sock->master_set;
		sock->socket_active = 0;
		if(select(sock->max_socket + 1, &sock->working_set, NULL, NULL, NULL) == -1){
			error_handler(DE_ERROR_SOCKET_SELECT_FAILED);
			break;
		}

		for (i=0; i <= sock->max_socket; ++i)
		{
			if (FD_ISSET(i, &sock->working_set))
			{
				sock->connected = TRUE;

				if (i == sock->socket){
					sock->new_socket = accept(sock->socket, NULL, NULL);
					if (sock->new_socket == -1){
						if (errno != EWOULDBLOCK){
							error_handler(DE_ERROR_SOCKET_ACCEPT);
							end_listen = TRUE;
						}
						break;
					}

					//getpeername(i, (struct sockaddr*)&temp_address, &temp_length); TODO use to get ip address of user for banning purposes.

					printf("accepting new user! \n");

					FD_SET(sock->new_socket, &sock->master_set);
					if (sock->new_socket > sock->max_socket)
						sock->max_socket = sock->new_socket;

					if (!sock->connected){
						clear_temp_player(i);
						closesocket(i);
						FD_CLR(i, &sock->master_set);
					}
				}
				else{
					int size;
					char op;
					clear_buffer(&sock->bufferin);
					//get packet size
					sock->bytes_read = recv(i, sock->bufferin.buff, SIZE32, 0);
					take_buffer(&size,&sock->bufferin,SIZE32);

					clear_buffer(&sock->bufferin);
					sock->bytes_read = recv(i, sock->bufferin.buff, size, 0);

					if (sock->bytes_read == 0){
						if (errno != EWOULDBLOCK){
							error_handler(DE_ERROR_SOCKET_RECEIVE);
							sock->connected = FALSE;
						}
					}

					if (sock->bytes_read == -1){
						error_handler(DE_ERROR_SOCKET_CONNECTION_LOSS);
						sock->connected = FALSE;
					}

					if (!sock->connected){
						clear_temp_player(i);
						closesocket(i);
						FD_CLR(i, &sock->master_set);
					}
					else{
						handle_data(&sock->bufferin, i);
					}
				}
			}
		}
	} while (end_listen == FALSE);
	return TRUE;
}

void socketsend(buffer_t *data, int16 index)
{
	if (send(get_temp_player_socket(index), data->buff, data->offset, 0) != data->offset)
		error_handler(DE_ERROR_SOCKET_SEND_SIZE);
}

void socketidsend(buffer_t *data, uint64 id)
{
	if (send(id, data->buff, data->offset, 0) != data->offset)
		error_handler(DE_ERROR_SOCKET_SEND_SIZE);
}

void endsocket(void)
{
	closesocket(sock->socket);
	WSACleanup();
	free(sock);
}

void clear_user_socket(uint64 socket_id)
{
	clear_temp_player(socket_id);
	closesocket(socket_id);
	FD_CLR(socket_id, &sock->master_set);
}