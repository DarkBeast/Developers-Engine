/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
*           LibEvents, http://www.wangafu.net/~nickm/libevent-book/
******************************************************************************/

#include "winsocket.h"
#include "socket.h"
#include "error.h"
#include "handlepackets.h"
#include <stdio.h>
#include <errno.h>

desocket *sock; //short for socket
WSADATA wsadata;

int initsocket(void *arg)
{
	int on = 1;
	int length = sizeof(struct sockaddr_in);

	sock = (desocket *)calloc(1,sizeof(desocket));

		if (WSAStartup(0x0202, &wsadata) != 0)
			error_handler(DE_ERROR_SOCKET_WSASTARTUP);

	sock->base = event_base_new();
	if (!sock->base) {
		puts("Couldn't open event base");
		return 0;
	}

	//set socket address
	memset(&sock->host_address, 0, length);     /* Zero out structure */
	sock->host_address.sin_family      = AF_INET;             /* Internet address family */
	sock->host_address.sin_addr.s_addr = htonl(INADDR_ANY);   /* Server IP address */
	sock->host_address.sin_port        = htons(SERVER_PORT); /* Server port */

	//auto creates the socket via lib events it chooses the fastest method for the TCP socket.
	sock->listener = evconnlistener_new_bind(sock->base, accept_connection, NULL,LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE, -1,(struct sockaddr *) &sock->host_address, length);

	if (!sock->listener) {
		perror("Couldn't create listener");
		return 0;
	}

	//sets up the error event so if a error happens in the socket this allows us to link that error for handling.
	evconnlistener_set_error_cb(sock->listener, event_error_handler);

	event_base_dispatch(sock->base);
	return 0;
}

void accept_connection(struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr *address, int socklen, void *index)
{
	struct event_base *base = evconnlistener_get_base(listener);
	struct bufferevent *bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);

	bufferevent_setcb(bev, socket_read, NULL, socket_event, NULL);
	bufferevent_enable(bev, EV_READ | EV_WRITE | EV_PERSIST );
}

void event_error_handler(struct evconnlistener *listener, void *index)
{
	struct event_base *base = evconnlistener_get_base(listener);
	int err = EVUTIL_SOCKET_ERROR();
	fprintf(stderr, "Got an error %d (%s) on the listener. "
		"Shutting down.\n", err, evutil_socket_error_to_string(err));

	event_base_loopexit(base, NULL);
}

void socket_read(struct bufferevent *bev, void *index)
{
	struct evbuffer *input = bufferevent_get_input(bev);
	buffer_t buffer;
	uint32 size;

	while(evbuffer_get_length(input) > 4){

		clear_buffer(&buffer);
		evbuffer_remove(input,buffer.buff,4);
		take_buffer(&size, &buffer, SIZE32);

		clear_buffer(&buffer);
		evbuffer_remove(input,buffer.buff,size);

		handle_data(&buffer, bev, index);
	}
	puts("lost connection?");
}

void socket_event(struct bufferevent *bev, short events, void *index)
{
	if (events & BEV_EVENT_ERROR){
		//Error handler here.++++

	}
	clear_temp_player(bev);
	bufferevent_free(bev);
}

void socketsend(buffer_t *data, struct bufferevent *bev)
{
	struct evbuffer *output = bufferevent_get_output(bev);

	evbuffer_add(output,data->buff,data->offset);
}

void endsocket(void)
{
	event_base_loopexit(sock->base, NULL);
	event_base_free(sock->base);
	free(sock);
	libevent_global_shutdown();
	WSACleanup();
}

void clear_user_socket(struct bufferevent *bev)
{
	clear_temp_player(bev);
	bufferevent_free(bev);
}
