/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/
#include "players.h"
#include "socket.h"
#include "path.h"
#include <stdio.h>
#include "error.h"
#include "function.h"

player_t *player_array;
player_index online;
used_charname_t used_name;
job_t *job_array;
uint16 *players_on_map_array;

uint16 players_on_map(uint32 mapnum)
{
	return players_on_map_array[mapnum];
}

void players_on_map_plus(uint32 mapnum)
{
	players_on_map_array[mapnum]++;
}

void players_on_map_minus(uint32 mapnum)
{
	players_on_map_array[mapnum]--;
}

void total_players_online_plus(void)
{
	online.players_online++;
}

void total_players_online_minus(void)
{
	online.players_online--;
}

int16 total_players_online(void)
{
	return online.players_online;
}

int16 players_online_count(void)
{
	return online.max_index;
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
	players_on_map_array = (uint16 *)calloc(MAX_MAPS, sizeof(uint16));
	job_array = (job_t *)calloc(MAX_JOBS, sizeof(job_t));

	for( i = 0; i < MAX_JOBS; i++){
		job_array[i].name = (char *)calloc(MAX_NAME_LENGTH,sizeof(char));
		job_array[i].stat = (uint8 *)calloc(STAT_COUNT, sizeof(uint8));
	}

	if(file_readable(USED_CHAR_LIST_PATH))
		read_usedchars_list();
	else{
		used_name.charname = (charname_t *)calloc(32 ,sizeof(charname_t));
		used_name.size = 32;

		for(i = 0; i < used_name.size; i++)
			used_name.charname->name = (char *)calloc(MAX_NAME_LENGTH, sizeof(char));

		write_usedchars_list();
	}

	for( i = 0; i < MAX_PLAYERS; i++){
		player_array[i].username = (char *)calloc(MAX_NAME_LENGTH, sizeof(char));
		player_array[i].charname = (char *)calloc(MAX_NAME_LENGTH, sizeof(char));
		player_array[i].password = (char *)calloc(MAX_PASS_LENGTH, sizeof(char));
		player_array[i].equipment = (uint16 *)calloc(EQUIPMENT_COUNT, sizeof(uint16));
		player_array[i].vitals = (uint16 *)calloc(VITAL_COUNT, sizeof(uint16));
		player_array[i].inv = (inventory_t *)calloc(MAX_INV, sizeof(inventory_t));
		player_array[i].spells = (uint16 *)calloc(MAX_PLAYER_SPELLS, sizeof(uint16));
		player_array[i].stat = (uint8 *)calloc(STAT_COUNT, sizeof(uint8));
	}

	online.index = (int16 *)calloc(MAX_PLAYERS,sizeof(int16));
	online.players_online = 0;
	online.max_index = 0;

	check_dir(PLAYER_PATH);
}

void update_players_online(void)
{
	uint16 i = 0;
	uint16 array_index = 0;
	buffer_t buffer;
	if(online.players_online < 0){
		online.max_index = 0; return;
	}

	for(i = 0; i < MAX_PLAYERS; i++){
		if(get_temp_player(i)->loggedin){
			online.max_index = i;
			online.index[array_index] = i;
			array_index++;

			if(array_index >= online.players_online) break;
		}
	}

	clear_buffer(&buffer);
	add_opcode(&buffer, SHIGHINDEX);
	add_buffer(&buffer, &online.max_index, SIZE16);
	send_data_to_all(&buffer);
}

int16 get_total_map_players(uint32 mapnum)
{
	uint16 i = 0;
	uint16 n = 0;

	for( i = 0; i < players_online_count(); i++){
		if(temp_player(player_online(i))->loggedin && player(player_online(i))->map == mapnum)
			n++;

		return n;
	}
	return 0;
}

sbool account_exists(char *name)
{
	char *path =  get_path_name(PLAYER_PATH, name, FILE_ENDING);

	if(file_exists(path))
		return TRUE;
	else
		return FALSE;
}

void write_player(char *path, int32 i)
{
	FILE *fp;

	if((fp = fopen(path, "wb")) == NULL)
		error_handler(DE_ERROR_FILE_ERROR);

	fwrite(player_array[i].username, sizeof(char), MAX_NAME_LENGTH, fp);
	fwrite(player_array[i].password, sizeof(char), MAX_PASS_LENGTH, fp);
	fwrite(player_array[i].charname, sizeof(char), MAX_NAME_LENGTH, fp);
	fwrite(&player_array[i].sex, sizeof(uint8), 1, fp);
	fwrite(&player_array[i].job, sizeof(uint8), 1, fp);
	fwrite(&player_array[i].sprite, sizeof(uint16), 1, fp);
	fwrite(&player_array[i].level, sizeof(uint8), 1, fp);
	fwrite(&player_array[i].exp, sizeof(uint32), 1, fp);
	fwrite(&player_array[i].group, sizeof(uint8), 1, fp);
	fwrite(&player_array[i].pk, sizeof(uint8), 1, fp);
	fwrite(&player_array[i].points, sizeof(uint8), 1, fp);
	fwrite(&player_array[i].map, sizeof(uint16), 1, fp);
	fwrite(&player_array[i].x, sizeof(uint8), 1, fp);
	fwrite(&player_array[i].y, sizeof(uint8), 1, fp);
	fwrite(&player_array[i].dir, sizeof(uint8), 1, fp);
	fwrite(player_array[i].spells, sizeof(uint16), MAX_PLAYER_SPELLS, fp);
	fwrite(player_array[i].stat, sizeof(uint8), STAT_COUNT, fp);
	fwrite(player_array[i].vitals, sizeof(uint16), VITAL_COUNT, fp);
	fwrite(player_array[i].equipment, sizeof(uint16), EQUIPMENT_COUNT, fp);
	fwrite(player_array[i].inv, sizeof(inventory_t), MAX_INV, fp);
	fwrite(&player_array[i].banned, sizeof(sbool), 1, fp);

	fclose(fp);
}

void read_player(char *path, int32 i)
{
	FILE *fp;

	if((fp = fopen(path, "rb")) == NULL)
		error_handler(DE_ERROR_FILE_ERROR);

	if(player_array[i].username == NULL)
		player_array[i].username = (char *)calloc(MAX_NAME_LENGTH, sizeof(char));
	if(player_array[i].charname == NULL)
		player_array[i].charname = (char *)calloc(MAX_NAME_LENGTH, sizeof(char));
	if(player_array[i].password == NULL)
		player_array[i].password = (char *)calloc(MAX_PASS_LENGTH, sizeof(char));

	fread(player_array[i].username, sizeof(char), MAX_NAME_LENGTH, fp);
	fread(player_array[i].password, sizeof(char), MAX_PASS_LENGTH, fp);
	fread(player_array[i].charname, sizeof(char), MAX_NAME_LENGTH, fp);
	fread(&player_array[i].sex, sizeof(uint8), 1, fp);
	fread(&player_array[i].job, sizeof(uint8), 1, fp);
	fread(&player_array[i].sprite, sizeof(uint16), 1, fp);
	fread(&player_array[i].level, sizeof(uint8), 1, fp);
	fread(&player_array[i].exp, sizeof(uint32), 1, fp);
	fread(&player_array[i].group, sizeof(uint8), 1, fp);
	fread(&player_array[i].pk, sizeof(uint8), 1, fp);
	fread(&player_array[i].points, sizeof(uint8), 1, fp);
	fread(&player_array[i].map, sizeof(uint16), 1, fp);
	fread(&player_array[i].x, sizeof(uint8), 1, fp);
	fread(&player_array[i].y, sizeof(uint8), 1, fp);
	fread(&player_array[i].dir, sizeof(uint8), 1, fp);
	fread(player_array[i].spells, sizeof(uint16), MAX_PLAYER_SPELLS, fp);
	fread(player_array[i].stat, sizeof(uint8), STAT_COUNT, fp);
	fread(player_array[i].vitals, sizeof(uint16), VITAL_COUNT, fp);
	fread(player_array[i].equipment, sizeof(uint16), EQUIPMENT_COUNT, fp);
	fread(player_array[i].inv, sizeof(inventory_t), MAX_INV, fp);
	fread(&player_array[i].banned, sizeof(sbool), 1, fp);

	fclose(fp);
}

void write_usedchars_list(void)
{
	FILE *fp;
	uint32 i = 0;

	if((fp = fopen(USED_CHAR_LIST_PATH, "wb")) == NULL)
		error_handler(DE_ERROR_FILE_ERROR);

	fwrite(&used_name.size, sizeof(uint32), 1, fp);
	fwrite(&used_name.count, sizeof(uint32), 1, fp);

	for(i = 0; i < used_name.count; i++)
		fwrite(used_name.charname[i].name, sizeof(char), MAX_NAME_LENGTH, fp);

	fclose(fp);
}

void read_usedchars_list(void)
{
	FILE *fp;
	uint32 i = 0;
	if((fp = fopen(USED_CHAR_LIST_PATH, "rb")) == NULL)
		error_handler(DE_ERROR_FILE_ERROR);

	fread(&used_name.size, sizeof(uint32), 1, fp);
	fread(&used_name.count, sizeof(uint32), 1, fp);

	used_name.charname = (charname_t *)calloc(used_name.size ,sizeof(charname_t));

	for(i = 0; i < used_name.size; i++)
		used_name.charname[i].name = (char *)calloc(MAX_NAME_LENGTH, sizeof(char));

	for(i = 0; i < used_name.count; i++)
		fread(used_name.charname[i].name, sizeof(char), MAX_NAME_LENGTH, fp);

	fclose(fp);
}

void add_to_charname_list(char *name)
{
	if(used_name.count + 1 >= used_name.size){
		charname_t *strdata;
		int i = 0;

		if(used_name.charname == NULL)
			return;//TODO: add error handler here.

		used_name.size *= 2;

		strdata = (charname_t *)realloc(used_name.charname, next_power_of_two(used_name.size) * sizeof(charname_t));

		if(strdata == NULL){
			error_handler(DE_ERROR_POINTER_NULL); return;
		}

		for(i = used_name.count; i <= used_name.size; i++)
			strdata[i].name = NULL;

		used_name.charname = strdata;
	}

	memcpy(used_name.charname[used_name.count].name, name, MAX_NAME_LENGTH);
	used_name.count++;
}

void clear_player(int32 index)
{
	if(player_array[index].username != NULL)
		free(player_array[index].username);
	if(player_array[index].charname != NULL)
		free(player_array[index].charname);
	if(player_array[index].password != NULL)
		free(player_array[index].password);

	player_array[index].username = NULL;
	player_array[index].charname = NULL;
	player_array[index].password = NULL;
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
		free(job_array[i].name);
		free(job_array[i].stat);
	}

	for(i = 0; i < used_name.size; i++)
		free(used_name.charname[i].name);

	for( i = 0; i < MAX_PLAYERS; i++){
		if(player_array[i].username != NULL)
			free(player_array[i].username);
		if(player_array[i].charname != NULL)
			free(player_array[i].charname);
		if(player_array[i].password != NULL)
			free(player_array[i].password);
		free(player_array[i].equipment);
		free(player_array[i].vitals);
		free(player_array[i].inv);
		free(player_array[i].spells);
		free(player_array[i].stat);
	}

	free(online.index);
	free(used_name.charname);
	free(player_array);
	free(players_on_map_array);
	free(job_array);
}

void save_online_players(void)
{
	uint32 i = 0;
	char *path;

	for(i = 0; i < online.max_index; i++){
		if(temp_player(player_online(i))->loggedin){
			path =  get_path_name(PLAYER_PATH, player(player_online(i))->username, FILE_ENDING);
			write_player(path, i);
		}
	}

	printf("Online Players Saved.\n");
}

sbool isnamelegal(char c)
{
	if( (c >= 65 && c <= 90) || (c >= 97 && c <= 122) || (c = 95) || (c = 32) || (c >= 48 && c <=57))
		return TRUE;
	else
		return FALSE;
}

sbool is_multi_accounts( char *name)
{
	uint32 i = 0;

	for(i = 0; i < online.max_index; i++){
		if(strcmp(player(player_online(i))->username, name)) return TRUE;
	}

	return FALSE;
}

int16 find_player(char *name)
{
	uint32 i = 0;

	for(i = 0; i < online.max_index; i++){
		if(strcmp(player(player_online(i))->username, name)) return player_online(i);
	}

	return -1;
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
	job_array[1].sprite = 1;
	job_array[1].stat[STAT_STRENGTH] = 3;
	job_array[1].stat[STAT_DEFENSE] = 3;
	job_array[1].stat[STAT_SPEED] = 5;
	job_array[1].stat[STAT_MAGIC] = 9;

	//job monk
	job_array[2].name = "Monk";
	job_array[2].type = JOB_TYPE_MONK;
	job_array[2].sprite = 1;
	job_array[2].stat[STAT_STRENGTH] = 6;
	job_array[2].stat[STAT_DEFENSE] = 6;
	job_array[2].stat[STAT_SPEED] = 5;
	job_array[2].stat[STAT_MAGIC] = 3;
}

job_t *job(uint8 type)
{
	return &job_array[type];
}

void add_account(char *name, char *password, char *charname, uint8 sex, uint8 type)
{
	FILE *fp;
	player_t temp;
	char *path;

	temp.inv = (inventory_t *)calloc(MAX_INV,sizeof(inventory_t));
	temp.equipment = (uint16 *)calloc(EQUIPMENT_COUNT, sizeof(uint16));
	temp.spells = (uint16 *)calloc(MAX_PLAYER_SPELLS, sizeof(uint16));
	temp.stat = (uint8 *)calloc(STAT_COUNT, sizeof(uint8));
	temp.vitals = (uint16 *)calloc(VITAL_COUNT, sizeof(uint16));
	temp.username = name;
	temp.password = password;
	temp.charname = charname;
	temp.sex = sex;
	temp.job = type;
	temp.sprite = job(type)->sprite;
	temp.level = 1;
	temp.exp = 0;
	temp.group = GROUP_USER;
	temp.pk = FALSE;
	temp.points = 0;
	temp.map = START_MAP;
	temp.x = START_X;
	temp.y = START_Y;
	temp.dir = DIR_DOWN;
	temp.stat[STAT_STRENGTH] = job(type)->stat[STAT_STRENGTH];
	temp.stat[STAT_DEFENSE] = job(type)->stat[STAT_DEFENSE];
	temp.stat[STAT_SPEED] = job(type)->stat[STAT_SPEED];
	temp.stat[STAT_MAGIC] = job(type)->stat[STAT_MAGIC];
	temp.vitals[VITAL_HP] = 1 + (job(type)->stat[STAT_STRENGTH] / 2);
	temp.vitals[VITAL_MP] = 1 + (job(type)->stat[STAT_MAGIC] / 2);
	temp.vitals[VITAL_SP] = 1 + (job(type)->stat[STAT_SPEED] / 2);
	temp.banned = FALSE;

	path = get_path_name(PLAYER_PATH, name, FILE_ENDING);
	if((fp = fopen(path, "wb")) == NULL)
		error_handler(DE_ERROR_FILE_ERROR);

	fwrite(temp.username, sizeof(char), MAX_NAME_LENGTH, fp);
	fwrite(temp.password, sizeof(char), MAX_PASS_LENGTH, fp);
	fwrite(temp.charname, sizeof(char), MAX_NAME_LENGTH, fp);
	fwrite(&temp.sex, sizeof(uint8), 1, fp);
	fwrite(&temp.job, sizeof(uint8), 1, fp);
	fwrite(&temp.sprite, sizeof(uint16), 1, fp);
	fwrite(&temp.level, sizeof(uint8), 1, fp);
	fwrite(&temp.exp, sizeof(uint32), 1, fp);
	fwrite(&temp.group, sizeof(uint8), 1, fp);
	fwrite(&temp.pk, sizeof(uint8), 1, fp);
	fwrite(&temp.points, sizeof(uint8), 1, fp);
	fwrite(&temp.map, sizeof(uint16), 1, fp);
	fwrite(&temp.x, sizeof(uint8), 1, fp);
	fwrite(&temp.y, sizeof(uint8), 1, fp);
	fwrite(&temp.dir, sizeof(uint8), 1, fp);
	fwrite(temp.spells, sizeof(uint8), MAX_PLAYER_SPELLS, fp);
	fwrite(temp.stat, sizeof(uint8), STAT_COUNT, fp);
	fwrite(temp.vitals, sizeof(uint16), VITAL_COUNT, fp);
	fwrite(temp.equipment, sizeof(uint16), EQUIPMENT_COUNT, fp);
	fwrite(temp.inv, sizeof(inventory_t), MAX_INV, fp);
	fwrite(&temp.banned, sizeof(sbool), 1, fp);

	fclose(fp);
}