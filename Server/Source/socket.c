/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/
#include "socket.h"
#include "tinycthread.h"
#include "players.h"
#include "error.h"

temp_index temp_player_array;

temp_index *get_temp_player_array(void)
{
	return &temp_player_array;
}

temp_t *temp_players(void)
{
	return temp_player_array.index;
}

temp_t *temp_player(uint16 index)
{
	return &temp_player_array.index[index];
}

int32 *temp_player_count(uint16 index)
{
	return &temp_player_array.count;
}

void init_temp_player_index(void)
{
	int16 i = 0;

	temp_player_array.count = 0;
	temp_player_array.used = 0;
	temp_player_array.index = (temp_t *)calloc(MAX_PLAYERS,sizeof(temp_t));

	for(i = 0; i < MAX_PLAYERS; i++){
		temp_player_array.index[i].loggedin = FALSE;
		temp_player_array.index[i].bev = NULL;
	}
}

int16 set_temp_player_index(struct bufferevent *bev)
{
	int16 *i = (int16 *)calloc(1, sizeof(int16));

	if(i == NULL)
		error_handler(DE_ERROR_POINTER_NULL);

	if( temp_player_array.used < temp_player_array.count){
		for( *i = 0; *i < temp_player_array.count; *i++){
			if(!temp_player_array.index[*i].loggedin){
				temp_player_array.index[*i].loggedin = TRUE;
				temp_player_array.index[*i].bev = bev;
				temp_player_array.used++;
				bufferevent_setcbarg(bev, i);
				return *i; //return the index
			}
		}
	}

	if( temp_player_array.count < MAX_PLAYERS){
		for( *i = temp_player_array.count; *i < MAX_PLAYERS; *i++){
			if(!temp_player_array.index[*i].loggedin){
				temp_player_array.index[*i].loggedin = TRUE;
				temp_player_array.index[*i].bev = bev;
				temp_player_array.used++;
				temp_player_array.count++;
				bufferevent_setcbarg(bev, i);
				return *i; //return the index
			}
		}
	}

	return TEMP_FULL; //could not find open slot.
}

sbool clear_temp_player(struct bufferevent *bev)
{
	int16 i = get_temp_player_index(bev);

	if(i != TEMP_NO_MATCH){
		temp_player_array.index[i].loggedin = FALSE;
		free_temp_index(bev);
		temp_player_array.index[i].bev = NULL;
		temp_player_array.used--;
		clear_player(i);
		return TRUE;
	}

	return TEMP_NO_MATCH;//no player to clear
}

void clear_temp_player_onindex(int16 index)
{
	temp_player_array.index[index].loggedin = FALSE;
	free_temp_index(temp_player_array.index[index].bev);
	temp_player_array.index[index].bev = NULL;
	clear_player(index);
	temp_player_array.used--;
}

void free_temp_index(struct bufferevent *bev)
{
	void *i = NULL;

	if(bev != NULL){
		bufferevent_getcb(bev,NULL,NULL,NULL,&i);

		if( i != NULL)
			free(i);

		i = NULL;
	}
}

int16 get_temp_player_index(struct bufferevent *bev)
{
	void *i;
	int16 *index;

	bufferevent_getcb(bev,NULL,NULL,NULL,&i);

	if(i != NULL){
		index = (int16 *)i;
		return *index;
	}

	return TEMP_NO_MATCH; //return -1 if none matched
}

struct bufferevent * get_temp_player_bufferevent(int16 index)
{
	return 	temp_player_array.index[index].bev;
}

temp_t *get_temp_player(int16 index)
{
	return 	&temp_player_array.index[index];
}

void send_data(buffer_t *data, int16 index)
{
	buffer_t buff;

	clear_buffer(&buff);
	add_buffer(&buff, &data->offset, SIZE32);
	add_buffer(&buff, &data->buff, data->offset);

	socketsend(&buff, get_temp_player_bufferevent(index));
}

void send_socket_data(buffer_t *data, struct bufferevent *bev)
{
	buffer_t buff;

	clear_buffer(&buff);
	add_buffer(&buff, &data->offset, SIZE32);
	add_buffer(&buff, &data->buff, data->offset);

	socketsend(&buff, bev);
}

void send_data_to_all(buffer_t *data)
{
	uint32 i = 0;

	for( i = 0; i < total_players_online(); i++){
		if(get_temp_player(player_online(i))->loggedin)
			send_data(data, player_online(i));
	}
}

void send_data_to_all_but(buffer_t *data, int16 index)
{
	uint32 i = 0;

	for( i = 0; i < total_players_online(); i++){
		if(player_online(i) != index && get_temp_player(player_online(i))->loggedin)
			send_data(data, player_online(i));
	}
}

void send_data_to_map(buffer_t *data, uint32 mapnum)
{
	uint32 i = 0;

	for( i = 0; i < total_players_online(); i++){
		if(get_temp_player(player_online(i))->loggedin && player(player_online(i))->map == mapnum)
			send_data(data, player_online(i));
	}
}

void send_data_to_map_but(buffer_t *data, uint32 mapnum, int16 index)
{
	uint32 i = 0;

	for( i = 0; i < total_players_online(); i++){
		if(player_online(i) != index && get_temp_player(player_online(i))->loggedin && player(player_online(i))->map == mapnum)
			send_data(data, player_online(i));
	}
}

void unload_socket(void)
{
	int32 i = 0;
	void *data = NULL;

	for(i = 0; i < MAX_PLAYERS; i++){
		if(temp_player_array.index[i].bev != NULL){
			bufferevent_getcb(temp_player_array.index[i].bev,NULL,NULL,NULL,&data);

			if(data != NULL)
				free(data);

			data = NULL;
		}
	}

	free(temp_player_array.index);
	endsocket();
}