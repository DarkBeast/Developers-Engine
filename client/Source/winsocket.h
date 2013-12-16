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
};

void initsocket(void);
int socketconnect(void);
int socketlisten(void *arg);
void socketsend(buffer_t *data);
void unloadsocket(void);
sbool isconnected(void);
void set_isconnected(sbool flag);

#endif