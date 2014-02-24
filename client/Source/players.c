/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/
#include "players.h"
#include "error.h"

player_t *player_array;
player_index online;
player_map_index onmap_t;
job_t *job_array;
sbool is_connected_b;
int16 user_index;
ingame_t *temp_data_t;

ingame_t *tempdata(void)
{
	return temp_data_t;
}

void set_myindex(int16 index)
{
	user_index = index;
}

int16 myindex(void)
{
	return user_index;
}

void set_connection(sbool flag)
{
	is_connected_b = flag;
}

sbool is_connected(void)
{
	return is_connected_b;
}

void set_players_on_map(int16 amount)
{
	onmap_t.map_players = amount;
}

int16 players_on_map(void)
{
	return onmap_t.map_players;
}

int16 player_on_map(int16 index)
{
	return onmap_t.index[index];
}

void set_total_players_online(int16 max_index)
{
	online.players_online = max_index;
}

int16 total_players_online(void)
{
	return online.players_online;
}

int16 player_online(uint16 index)
{
	return online.index[index];
}

player_t *players(void)
{
	return player_array;
}

player_t *player(int index)
{
	return &player_array[index];
}

void init_players(void)
{
	uint32 i = 0;
	player_array = (player_t *)calloc(MAX_PLAYERS,sizeof(player_t));
	job_array = (job_t *)calloc(MAX_JOBS, sizeof(job_t));
	temp_data_t = (ingame_t *)calloc(1, sizeof(ingame_t));

	for( i = 0; i < MAX_JOBS; i++){
		job_array[i].stat = (uint8 *)calloc(STAT_COUNT, sizeof(uint8));
	}

	for( i = 0; i < MAX_PLAYERS; i++){
		player_array[i].username = (char *)calloc(MAX_NAME_LENGTH, sizeof(char));
		player_array[i].charname = (char *)calloc(MAX_NAME_LENGTH, sizeof(char));
		player_array[i].password = (char *)calloc(MAX_PASS_LENGTH, sizeof(char));
		player_array[i].equipment = (uint16 *)calloc(EQUIPMENT_COUNT, sizeof(uint16));
		player_array[i].vitals = (uint16 *)calloc(VITAL_COUNT, sizeof(uint16));
		player_array[i].maxvitals = (uint16 *)calloc(VITAL_COUNT, sizeof(uint16));
		player_array[i].inv = (inventory_t *)calloc(MAX_INV, sizeof(inventory_t));
		player_array[i].spells = (uint16 *)calloc(MAX_PLAYER_SPELLS, sizeof(uint16));
		player_array[i].stat = (uint8 *)calloc(STAT_COUNT, sizeof(uint8));
	}

	online.index = (int16 *)calloc(MAX_PLAYERS,sizeof(int16));
	online.players_online = 0;
	init_jobs();
}

void clear_player(int32 index)
{
	player_array[index].username = "";
	player_array[index].charname = "";
	player_array[index].password = "";
	player_array[index].exp = 0;
	player_array[index].dir = 0;
	player_array[index].group = 0;
	player_array[index].job = 0;
	player_array[index].level = 0;
	player_array[index].map = 0;
	player_array[index].points = 0;
	player_array[index].sprite = 0;
	player_array[index].x = 0;
	player_array[index].y = 0;
}

void unload_players(void)
{
	uint32 i = 0;

	for( i = 0; i < MAX_JOBS; i++){
		free(job_array[i].stat);
	}

	for( i = 0; i < MAX_PLAYERS; i++){
		free(player_array[i].username);
		free(player_array[i].charname);
		free(player_array[i].password);
		free(player_array[i].equipment);
		free(player_array[i].vitals);
		free(player_array[i].maxvitals);
		free(player_array[i].inv);
		free(player_array[i].spells);
		free(player_array[i].stat);
	}
	free(temp_data_t);
	free(online.index);
	free(player_array);
	free(job_array);
}

sbool isnamelegal(char c)
{
	if( (c >= 65 && c <= 90) || (c >= 97 && c <= 122) || (c = 95) || (c = 32) || (c >= 48 && c <=57))
		return TRUE;
	else
		return FALSE;
}

void init_jobs(void)
{
	//job warrior:
	job_array[0].name = "Warrior";
	job_array[0].type = JOB_TYPE_WARRIOR;
	job_array[0].sprite = 1;
	job_array[0].stat[STAT_STRENGTH] = 8;
	job_array[0].stat[STAT_DEFENSE] = 6;
	job_array[0].stat[STAT_SPEED] = 5;
	job_array[0].stat[STAT_MAGIC] = 1;

	//job mage
	job_array[1].name = "Mage";
	job_array[1].type = JOB_TYPE_MAGE;
	job_array[1].sprite = 2;
	job_array[1].stat[STAT_STRENGTH] = 3;
	job_array[1].stat[STAT_DEFENSE] = 3;
	job_array[1].stat[STAT_SPEED] = 5;
	job_array[1].stat[STAT_MAGIC] = 9;

	//job monk
	job_array[2].name = "Monk";
	job_array[2].type = JOB_TYPE_MONK;
	job_array[2].sprite = 2;
	job_array[2].stat[STAT_STRENGTH] = 6;
	job_array[2].stat[STAT_DEFENSE] = 6;
	job_array[2].stat[STAT_SPEED] = 5;
	job_array[2].stat[STAT_MAGIC] = 3;
}

job_t *job(uint8 type)
{
	return &job_array[type];
}

void get_players_on_map(void)
{
	int16 i = 0;
	uint16 array_index = 0;

	onmap_t.map_players = 1;

	for(i = 0; i < online.players_online; i++){
		if(player_array[online.index[i]].map == player_array[user_index].map){
			onmap_t.index[array_index] = i;
			array_index++;
			onmap_t.map_players++;
		}
	}
}