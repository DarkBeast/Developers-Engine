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
	struct sockaddr_in host_address;
	unsigned short host_port;
	char *host_ip;
	buffer_t bufferin;
	buffer_t bufferout;
	int bytes_read;
	int total_bytes_read;
	WSADATA wsadata;
};

void initsocket(void);
int socketconnect(void);
void socketlisten(void);
void socketsend(buffer_t *data, int16 index);
void removesocket(void);

#endif