#include "winsocket.h"
#include "socket.h"
#include "error.h"
#include <stdio.h>
#include "handlepackets.h"

desocket *sock; //short for socket
WSADATA wsadata;
sbool isconnected_t;

sbool isconnected(void)
{
	return isconnected_t;
}

void set_isconnected(sbool flag)
{
	isconnected_t = flag;
}

void initsocket(void)
{
	sock = (desocket *)calloc(1,sizeof(desocket));

	if (WSAStartup(0x0202, &wsadata) != 0) /* Load Winsock 2.0 DLL */
		error_handler(DE_ERROR_SOCKET_WSASTARTUP);

	/* Create a reliable, stream socket using TCP */
	if ((sock->socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		error_handler(DE_ERROR_SOCKET_INIT);

	/* Construct the server address structure */
	memset(&sock->host_address, 0, sizeof(struct sockaddr_in));     /* Zero out structure */
	sock->host_address.sin_family      = AF_INET;             /* Internet address family */
	sock->host_address.sin_addr.s_addr = inet_addr(SERVER_IP);   /* Server IP address */
	sock->host_address.sin_port        = htons(CLIENT_PORT); /* Server port */
}

int socketlisten(void *arg)
{
	sbool i = TRUE;
	int size;
	buffer_t buffer;

	while (i){
		clear_buffer(&buffer);

		if (recv(sock->socket, buffer.buff, SIZE32, 0) <= 0){
			puts("Connection to Server Lost");
			isconnected_t = FALSE;
			return FALSE;
		}

		take_buffer(&size,&buffer,SIZE32);

		clear_buffer(&buffer);

		if (recv(i, buffer.buff, size, 0) <= 0){
			puts("Connection to Server Lost");
			isconnected_t = FALSE;
			return FALSE;
		}

		incomming_packets(&buffer);
	}
	return FALSE;
}

int socketconnect(void)
{
	if (connect(sock->socket, (SOCKADDR *)&sock->host_address, sizeof(struct sockaddr_in)) == SOCKET_ERROR){
		isconnected_t = FALSE;
		return FALSE;
	}
	else{
		isconnected_t = TRUE;
		return TRUE;
	}
}

void socketsend(buffer_t *data)
{
	buffer_t buff;

	clear_buffer(&buff);
	add_buffer(&buff, &data->offset, SIZE32);
	add_buffer(&buff, &data->buff, data->offset);

	if (send(sock->socket, buff.buff, buff.offset, 0) != buff.offset)
		error_handler(DE_ERROR_SOCKET_SEND_SIZE);
}

void unloadsocket(void)
{
	closesocket(sock->socket);
	WSACleanup();
	free(sock);
}