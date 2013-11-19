#ifndef DE_LINSOCKET_H
#define DE_LINSOCKET_H

#include "integer.h"
#include "globals.h"

typedef struct desocket desocket;

struct desocket
{
	int socket; // Socket descriptor
	//struct sockaddr_in host_addrress;
	unsigned short host_port;
	char *host_ip;
	char buffer[SOCKET_REC_SIZE];
	int buffer_size; //length of the packet
	int bytes_read;
	int total_bytes_read;
};

#endif