/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/
#ifndef DE_WINSOCKET_H
#define DE_WINSOCKET_H

#include <stdlib.h>
#include <event2/listener.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include "integer.h"
#include "globals.h"
#include "buffer.h"

typedef struct desocket desocket;

struct desocket
{
	struct event_base *base;
	struct evconnlistener *listener;
	struct sockaddr_in host_address;
};

int initsocket(void *arg);

void accept_connection(struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr *address, int socklen, void *ctx);

void event_error_handler(struct evconnlistener *listener, void *ctx);

void socket_read(struct bufferevent *bev, void *ctx);

void socket_event(struct bufferevent *bev, short events, void *ctx);

void socketsend(buffer_t *data, struct bufferevent *bev);

void endsocket(void);

void clear_user_socket(struct bufferevent *bev);
#endif