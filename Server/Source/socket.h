/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/
#ifndef DE_SOCKET_H
#define DE_SOCKET_H

#include "types.h"
#include "packetenum.h"

#define TEMP_NO_MATCH -1
#define TEMP_FULL -2

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__)
#include "winsocket.h"
#define DE_WINDOWS_PLATFORM 1
#else
#include "linsocket.h"
#endif

typedef struct temp_index temp_index;
typedef struct temp_t temp_t;

struct temp_index{
	int32 count;
	int32 used;
	temp_t *index;
};

struct temp_t{
	struct bufferevent *bev; //used for index,sending packets etc.
	sbool loggedin;
	double  attacktimer;
	double  datatimer;
	uint32 databytes;
	uint32 datapackets;
	uint8 targettype;
	uint16 target;
	uint8 castedspell;
	uint8 gettingmap;
};

struct banned_t
{
	uint32 serial;
};

temp_index *get_temp_player_array(void);

temp_t *temp_players(void);

temp_t *temp_player(uint16 index);

int32 *temp_player_count(uint16 index);

void init_temp_player_index(void);

int16 set_temp_player_index(struct bufferevent *bev);

sbool clear_temp_player(struct bufferevent *bev);

void clear_temp_player_onindex(int16 index);

int16 get_temp_player_index(struct bufferevent *bev);

struct bufferevent * get_temp_player_bufferevent(int16 index);

temp_t *get_temp_player(int16 index);

void send_data(buffer_t *data, int16 index);

void send_socket_data(buffer_t *data, struct bufferevent *bev);

void send_data_to_all(buffer_t *data);

void send_data_to_all_but(buffer_t *data, int16 index);

void send_data_to_map(buffer_t *data, uint32 mapnum);

void send_data_to_map_but(buffer_t *data, uint32 mapnum, int16 index);

void unload_socket(void);

#endif