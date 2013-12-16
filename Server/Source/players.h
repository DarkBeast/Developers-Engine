/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/
#ifndef DE_PLAYERS_H
#define DE_PLAYERS_H

#include "globals.h"
#include "integer.h"
#include "types.h"
#include <stdlib.h>

typedef struct player_t player_t;
typedef struct player_index player_index;
typedef struct inventory_t inventory_t;
typedef struct used_charname_t used_charname_t;
typedef struct charname_t charname_t;
typedef struct job_t job_t;
typedef enum group_t group_t;
typedef enum job_type_t job_type_t;

struct used_charname_t
{
	charname_t *charname;
	uint32 size;
	uint32 count;
};

struct charname_t
{
	char *name;
};

struct player_index{
	int16 players_online;
	int16 max_index;
	int16 *index;
};

struct player_t{
	char *username;
	char *password;
	char *charname;
	uint8 sex;
	uint8 job;
	uint16 sprite;
	uint8 level;
	uint32 exp;
	uint8 group;
	uint8 pk;
	uint8 points;
	uint16 map;
	uint8 x;
	uint8 y;
	uint8 dir;
	uint16 *spells;
	uint8 *stat;
	uint16 *vitals;
	uint16 *equipment;
	inventory_t *inv;
	sbool banned;
};//553

struct inventory_t{
	uint16 id;
	uint32 value;
	uint16 dur;
};//8

struct job_t{
	char *name;
	uint8 type;
	uint16 sprite;
	uint8 *stat;
};

enum job_type_t{
	JOB_TYPE_WARRIOR,
	JOB_TYPE_MAGE,
	JOB_TYPE_MONK
};

enum group_t{
	GROUP_USER,
	GROUP_MONITOR,
	GROUP_MAPPER,
	GROUP_DEVELOPER,
	GROUP_CREATOR,
	GROUP_COUNT
};

uint16 players_on_map(uint32 mapnum);

void players_on_map_plus(uint32 mapnum);

void players_on_map_minus(uint32 mapnum);

void total_players_online_plus(void);

void total_players_online_minus(void);

int16 total_players_online(void);

int16 players_online_count(void);

int16 player_online(uint16 index);

player_t *players(void);

player_t *player(int index);

void init_players(void);

sbool account_exists(char *name);

sbool isnamelegal(char c);

void write_player(char *path, int32 i);

void read_player(char *path, int32 i);

void clear_player(int32 index);

void save_online_players(void);

void write_usedchars_list(void);

void read_usedchars_list(void);

void add_to_charname_list(char *name);

sbool is_multi_accounts( char *name);

int16 find_player(char *name);

void init_jobs(void);

job_t *job(uint8 type);

int16 get_total_map_players(uint32 mapnum);

void update_players_online(void);

void unload_players(void);

void add_account(char *name, char *password, char *charname, uint8 sex, uint8 type);

#endif