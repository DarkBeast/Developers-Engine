/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/
#include "maps.h"
#include <stdio.h>
#include "path.h"
#include "error.h"
#include "types.h"
#include "socket.h"

map_t map_data;

map_t *map(void)
{
	return &map_data;
}

void init_maps(void)
{
	uint32 n = 0;

	map_data.name = (char *)calloc(MAX_NAME_LENGTH, sizeof(char));
	map_data.npc = (map_npc_t *)calloc(MAX_MAP_NPCS, sizeof(map_npc_t));
	map_data.tile = (tile_t *)calloc(MAX_MAPX * MAX_MAPY, sizeof(tile_t));
	map_data.items = (map_item_t *)calloc(MAX_MAP_ITEMS, sizeof(map_item_t));

	for( n = 0; n < MAX_MAP_NPCS; n++){
		map_data.npc[n].vitals = (uint16 *)calloc(VITAL_COUNT, sizeof(uint16));
	}

	check_dir(MAP_PATH);
}

void write_map(char *path)
{
	FILE *fp;

	if((fp = fopen(path, "wb")) == NULL)
		error_handler(DE_ERROR_FILE_ERROR);

	fwrite(map_data.name, sizeof(char), MAX_NAME_LENGTH, fp);
	fwrite(&map_data.revision, sizeof(uint32), 1, fp);
	fwrite(&map_data.moral, sizeof(uint8), 1, fp);
	fwrite(&map_data.up, sizeof(uint16), 1, fp);
	fwrite(&map_data.down, sizeof(uint16), 1, fp);
	fwrite(&map_data.left, sizeof(uint16), 1, fp);
	fwrite(&map_data.right, sizeof(uint16), 1, fp);
	fwrite(&map_data.music, sizeof(uint8), 1, fp);
	fwrite(&map_data.bootmap, sizeof(int32), 1, fp);
	fwrite(&map_data.bootx, sizeof(uint8), 1, fp);
	fwrite(&map_data.booty, sizeof(uint8), 1, fp);
	fwrite(&map_data.shop, sizeof(uint16), 1, fp);
	fwrite(map_data.tile, sizeof(tile_t), MAX_MAPX * MAX_MAPY, fp);

	fclose(fp);
}

void read_map(char *path)
{
	FILE *fp;

	if((fp = fopen(path, "rb")) == NULL)
		error_handler(DE_ERROR_FILE_ERROR);

	fread(map_data.name, sizeof(char), MAX_NAME_LENGTH, fp);
	fread(&map_data.revision, sizeof(uint32), 1, fp);
	fread(&map_data.moral, sizeof(uint8), 1, fp);
	fread(&map_data.up, sizeof(uint16), 1, fp);
	fread(&map_data.down, sizeof(uint16), 1, fp);
	fread(&map_data.left, sizeof(uint16), 1, fp);
	fread(&map_data.right, sizeof(uint16), 1, fp);
	fread(&map_data.music, sizeof(uint8), 1, fp);
	fread(&map_data.bootmap, sizeof(int32), 1, fp);
	fread(&map_data.bootx, sizeof(uint8), 1, fp);
	fread(&map_data.booty, sizeof(uint8), 1, fp);
	fread(&map_data.shop, sizeof(uint16), 1, fp);
	fread(map_data.tile, sizeof(tile_t), MAX_MAPX * MAX_MAPY, fp);

	fclose(fp);
}

void unload_maps(void)
{
	uint32 n = 0;

	for( n = 0; n < MAX_MAP_NPCS; n++){
		free(map_data.npc[n].vitals);
	}
	free(map_data.name);
	free(map_data.npc);
	free(map_data.items);
	free(map_data.tile);
}

void clear_map_npcs(void)
{
	uint32 i = 0;

	for( i = 0; i < MAX_MAP_NPCS; i++){
		map()->npc[i].num = 0;
		map()->npc[i].target = 0;
		map()->npc[i].targettype = TARGET_NONE;
		map()->npc[i].vitals[VITAL_HP] = 0;
		map()->npc[i].vitals[VITAL_MP] = 0;
		map()->npc[i].vitals[VITAL_SP] = 0;
		map()->npc[i].attacktimer = FALSE;
		map()->npc[i].offsetx = 0;
		map()->npc[i].offsety = 0;
		map()->npc[i].moving = 0;
	}
}

void clear_map_items(void)
{
	uint32 x = 0;

	for(x = 1; x < MAX_MAP_ITEMS; x++){
		map()->items[x].num = 0;
		map()->items[x].dur = 0;
		map()->items[x].value = 0;
		map()->items[x].x = 0;
		map()->items[x].y = 0;
	}
}

void clear_map_npc(uint8 i)
{
	map()->npc[i].num = 0;
	map()->npc[i].vitals[VITAL_HP] = 0;
	map()->npc[i].vitals[VITAL_MP] = 0;
	map()->npc[i].vitals[VITAL_SP] = 0;
}