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
typedef struct job_t job_t;
typedef struct ingame_t ingame_t;
typedef struct player_map_index player_map_index;
typedef enum group_t group_t;
typedef enum job_type_t job_type_t;

struct player_index{
	int16 players_online;
	int16 *index;
};

struct player_map_index{
	int16 map_players;
	int16 *index;
};

struct ingame_t{
	sbool castedspell;
	sbool ingame;
	sbool gettingmap;
	sbool canmovenow;
	sbool mapanim;
	double mapgettimer;
	double mapanimtimer;
	uint8 dir; //Directional buttons down.
	sbool control;
	sbool shift;
	uint8 curx;
	uint8 cury;
	uint32 inv_item_selected;
	uint32 spell_selected;
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
	int16 offsety;
	int16 offsetx;
	sbool attacking;
	double attacktimer;
	uint8 moving;
	uint16 *maxvitals;
};

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

ingame_t *tempdata(void);

void set_myindex(int16 index);

int16 myindex(void);

void set_connection(sbool flag);

sbool is_connected(void);

int16 total_players_online(void);

void set_total_players_online(int16 max_index);

int16 player_online(uint16 index);

player_t *players(void);

player_t *player(int index);

void init_players(void);

sbool isnamelegal(char c);

void clear_player(int32 index);

void init_jobs(void);

job_t *job(uint8 type);

void unload_players(void);

void set_players_on_map(int16 amount);

int16 players_on_map(void);

int16 player_on_map(int16 index);

void get_players_on_map(void);

int16 player_on_map(int16 index);
#endif