/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/
#ifndef DE_MAPS_H
#define DE_MAPS_H

#include "integer.h"
#include "globals.h"
#include "buffer.h"
#include <stdlib.h>

typedef struct tile_data_t tile_data_t;
typedef struct tile_t tile_t;
typedef struct map_item_t map_item_t;
typedef struct map_npc_t map_npc_t;
typedef struct map_t map_t;
typedef enum tile_type tile_type;
typedef enum map_type map_type;

#define GETXY(x, y) ((y << 7) + x) /* GETXY is faster, but may error */
#define GETXY_SAFE(x, y, max_x, max_y) ((x < max_x && y < max_y) ? ((y << 7) + x) : 0) /* Safe but slower */

struct tile_data_t{
	uint8 x;
	uint8 y;
	uint8 Tileset;
};

struct tile_t{
	tile_data_t ground;
	tile_data_t mask;
	tile_data_t anim;
	tile_data_t fringe;
	uint8 type;
	uint16 data1;
	uint16 data2;
	uint16 data3; //19bytes per tile
};

struct map_item_t{
	uint16 num;
	uint32 value;
	uint16 dur;
	uint8 x;
	uint8 y;
};

struct map_npc_t{
	uint16 num;
	uint16 target;
	uint8 targettype;
	uint16 *vitals;
	uint8 x;
	uint8 y;
	uint8 dir;
	double  spawnwait;
	double  attacktimer;
};

struct map_t{
	char *name; //32bytes
	uint32 revision;
	uint8 moral;
	uint16 up;
	uint16 down;
	uint16 left;
	uint16 right;
	uint8 music;
	int32 bootmap;
	uint8 bootx;
	uint8 booty;
	uint16 shop;
	map_item_t *items;
	tile_t *tile; //set to single tile array MAX_X * MAX_Y; 3135 bytes
	map_npc_t *npc;
};

enum map_type{
	MAP_TYPE_NONE,
	MAP_TYPE_SAFE,
	MAP_TYPE_COUNT
};

enum tile_type{
	TILE_TYPE_WALKABLE,
	TILE_TYPE_BLOCKED,
	TILE_TYPE_WARP,
	TILE_TYPE_ITEM,
	TILE_TYPE_NPCAVOID,
	TILE_TYPE_KEY,
	TILE_TYPE_KEYOPEN,
	TILE_TYPE_COUNT
};

map_t *maps(void);
map_t *map(uint32 index);
void init_maps(void);
void write_map(char *path, int32 i);
void read_map(char *path, int32 i);
void unload_maps(void);
void mapcache_create(uint32 mapnum);
buffer_t *map_cache(uint32 mapnum);
#endif