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
	int length = sizeof(struct sockaddr_in);

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
	memset(&sock->host_address, 0, length);     /* Zero out structure */
	sock->host_address.sin_family      = AF_INET;             /* Internet address family */
	sock->host_address.sin_addr.s_addr = htonl(INADDR_ANY);   /* Server IP address */
	sock->host_address.sin_port        = htons(SERVER_PORT); /* Server port */

	bind(sock->socket, (struct sockaddr*)&sock->host_address, length);

	//listen for connections.
	if(listen(sock->socket, SOMAXCONN) == -1)
		error_handler(DE_ERROR_SOCKET_LISTEN);

	//initialize master set for select.
	FD_ZERO(&sock->master_set);
	sock->max_socket = sock->socket;
	FD_SET(sock->socket, &sock->master_set);
}

int socketlisten(void *arg)
{
	sbool end_listen = FALSE;
	struct sockaddr_in temp_address;
	uint64 i = 0;
	sbool connected = TRUE;
	buffer_t buffer;
	int ready_count;

	do
	{
		memcpy(&sock->working_set, &sock->master_set, sizeof(sock->master_set));

		//sock->working_set = sock->master_set;

		if((ready_count = select(sock->max_socket + 1, &sock->working_set, NULL, NULL, NULL)) == -1){
			error_handler(DE_ERROR_SOCKET_SELECT_FAILED);
			break;
		}

		for (i=0; i <= sock->max_socket && ready_count > 0; ++i){
			if (FD_ISSET(i, &sock->working_set)){
				connected = TRUE;
				ready_count--;

				if (i == sock->socket){
					do{
						sock->new_socket = accept(sock->socket, NULL, NULL);
						if (sock->new_socket < 0){
							if (errno != EWOULDBLOCK){
								error_handler(DE_ERROR_SOCKET_ACCEPT);
								end_listen = TRUE;
							}
							break;
						}

						printf("accepting new user! \n");

						FD_SET(sock->new_socket, &sock->master_set);
						if (sock->new_socket > sock->max_socket)
							sock->max_socket = sock->new_socket;
					} while (sock->new_socket != -1);
				}
				else{
					int size;
					int bytes_read;

					clear_buffer(&buffer);
					bytes_read = recv(i, buffer.buff, SIZE32, 0); //get packet size.
					take_buffer(&size,&buffer,SIZE32);

					clear_buffer(&buffer);
					bytes_read = recv(i, buffer.buff, size, 0);  //get the packet.

					if (bytes_read < 0){
						if (errno != EWOULDBLOCK){
							error_handler(DE_ERROR_SOCKET_RECEIVE);
							connected = FALSE;
						}
					}

					if (bytes_read == 0){
						error_handler(DE_ERROR_SOCKET_CONNECTION_LOSS);
						connected = FALSE;
					}

					if (!connected){
						clear_temp_player(i);
						closesocket(i);
						FD_CLR(i, &sock->master_set);

						if (i == sock->max_socket){
							while (FD_ISSET(sock->max_socket, &sock->master_set) == FALSE)
								sock->max_socket -= 1;
						}
					}
					else{
						handle_data(&buffer, i);
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
	uint32 i = 0;

	for (i = 0; i <= sock->max_socket; ++i) {
		if (FD_ISSET(i, &sock->master_set))
			closesocket(i);
	}

	WSACleanup();
	free(sock);
}

void clear_user_socket(uint64 socket_id)
{
	clear_temp_player(socket_id);
	closesocket(socket_id);
	FD_CLR(socket_id, &sock->master_set);

	if (socket_id == sock->max_socket){
		while (FD_ISSET(sock->max_socket, &sock->master_set) == FALSE)
			sock->max_socket -= 1;
	}
}