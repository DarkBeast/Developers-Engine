#ifndef DE_LINSOCKET_H
#define DE_LINSOCKET_H

#include "integer.h"
#include "globals.h"

typedef struct desocket desocket;

struct desocket
{
	int socket; // Socket descriptor
	//struct sockaddr_in host_addrress;
};

#endif