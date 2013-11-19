#include "winsocket.h"
#include "socket.h"
#include "error.h"
#include <stdio.h>

desocket *sock; //short for socket

void initsocket(void)
{
	sock = (desocket *)calloc(1,sizeof(desocket));

	if (WSAStartup(0x0202, &sock->wsadata) != 0) /* Load Winsock 2.0 DLL */
		error_handler(DE_ERROR_SOCKET_WSASTARTUP);

	/* Create a reliable, stream socket using TCP */
	if ((sock->socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		error_handler(DE_ERROR_SOCKET_INIT);
	sock->host_ip = SERVER_IP;
	sock->host_port = CLIENT_PORT;
	/* Construct the server address structure */
	memset(&sock->host_address, 0, sizeof(struct sockaddr_in));     /* Zero out structure */
	sock->host_address.sin_family      = AF_INET;             /* Internet address family */
	sock->host_address.sin_addr.s_addr = inet_addr(sock->host_ip);   /* Server IP address */
	sock->host_address.sin_port        = htons(sock->host_port); /* Server port */
}

void socketlisten(void)
{
	sbool i = TRUE;
	sock->total_bytes_read = 0;

	while (i){
		if ((sock->bytes_read = recv(sock->socket, sock->bufferin.buff, PACKET_SIZE, 0)) <= 0)
			error_handler(DE_ERROR_SOCKET_CONNECTION_LOSS);

		sock->total_bytes_read += sock->bytes_read;   /* Keep tally of total bytes */
	}
}

int socketconnect(void)
{
	if (connect(sock->socket, (SOCKADDR *)&sock->host_address, sizeof(struct sockaddr_in)) == SOCKET_ERROR)
		return FALSE;
	else
		return TRUE;
}

void socketsend(buffer_t *data)
{
	buffer_t buffer;

	clear_buffer(&buffer);
	add_buffer(&buffer,&data->offset,SIZE32);
	add_buffer(&buffer,&data->buff,data->offset);

	if (send(sock->socket, buffer.buff, buffer.offset, 0) != buffer.offset)
		error_handler(DE_ERROR_SOCKET_SEND_SIZE);
}

void removesocket(void)
{
	closesocket(sock->socket);
	WSACleanup();
}