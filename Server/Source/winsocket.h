/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/
#ifndef DE_WINSOCKET_H
#define DE_WINSOCKET_H

#include <stdlib.h>
#include <winsock2.h>
#include "integer.h"
#include "globals.h"
#include "buffer.h"

typedef struct desocket desocket;

struct desocket
{
	SOCKET socket; // Socket descriptor
	SOCKET max_socket; // Socket select max descriptor
	SOCKET new_socket;
	struct timeval timeout;
	struct fd_set master_set;
	struct fd_set working_set;
	struct sockaddr_in host_address;
	buffer_t bufferin;
	buffer_t bufferout;
	sbool connected;
	int address_length;
	int bytes_read;
	int total_bytes_read;
	int socket_active;
	unsigned short host_port;
	char *host_ip;
};

void initsocket(void);

int socketconnect(void);

int socketlisten(void *arg);

void socketsend(buffer_t *data, int16 index);

void socketidsend(buffer_t *data, uint64 id);

void endsocket(void);

void clear_user_socket(uint64 socket_id);

#endif