/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/
#include "maps.h"
#include <stdio.h>
#include "path.h"
#include "error.h"
#include "types.h"
#include "socket.h"

map_t *map_array;
buffer_t *map_array_cache;

buffer_t *map_cache(uint32 mapnum)
{
	return &map_array_cache[mapnum];
}

map_t *maps(void)
{
	return map_array;
}

map_t *map(uint32 index)
{
	return &map_array[index];
}

void init_maps(void)
{
	uint32 i = 0;
	uint32 n = 0;
	char *path;

	map_array = (map_t *)calloc(MAX_MAPS, sizeof(map_t));
	map_array_cache = (buffer_t *)calloc(MAX_MAPS, sizeof(buffer_t));

	for( i = 0; i < MAX_MAPS; i++){
		map_array[i].name = (char *)calloc(MAX_NAME_LENGTH, sizeof(char));
		map_array[i].npc = (map_npc_t *)calloc(MAX_MAP_NPCS, sizeof(map_npc_t));
		map_array[i].tile = (tile_t *)calloc(MAX_MAPX * MAX_MAPY, sizeof(tile_t));
		map_array[i].items = (map_item_t *)calloc(MAX_MAP_ITEMS, sizeof(map_item_t));

		for( n = 0; n < MAX_MAP_NPCS; n++)
			map_array[i].npc[n].vitals = (uint16 *)calloc(VITAL_COUNT, sizeof(uint16));

		path = get_path(MAP_PATH, i, FILE_ENDING);

		check_dir(MAP_PATH);

		if(file_readable(path))
			read_map(path,i);
		else
			write_map(path,i);

		mapcache_create(i);
	}
}

void write_map(char *path, int32 i)
{
	FILE *fp;

	if((fp = fopen(path, "wb")) == NULL)
		error_handler(DE_ERROR_FILE_ERROR);

	fwrite(map_array[i].name, sizeof(char), MAX_NAME_LENGTH, fp);
	fwrite(&map_array[i].revision, sizeof(uint32), 1, fp);
	fwrite(&map_array[i].moral, sizeof(uint8), 1, fp);
	fwrite(&map_array[i].up, sizeof(uint16), 1, fp);
	fwrite(&map_array[i].down, sizeof(uint16), 1, fp);
	fwrite(&map_array[i].left, sizeof(uint16), 1, fp);
	fwrite(&map_array[i].right, sizeof(uint16), 1, fp);
	fwrite(&map_array[i].music, sizeof(uint8), 1, fp);
	fwrite(&map_array[i].bootmap, sizeof(int32), 1, fp);
	fwrite(&map_array[i].bootx, sizeof(uint8), 1, fp);
	fwrite(&map_array[i].booty, sizeof(uint8), 1, fp);
	fwrite(&map_array[i].shop, sizeof(uint16), 1, fp);
	fwrite(map_array[i].tile, sizeof(tile_t), MAX_MAPX * MAX_MAPY, fp);
	fwrite(map_array[i].npc, sizeof(uint8), MAX_MAP_NPCS, fp);

	fclose(fp);
}

void read_map(char *path, int32 i)
{
	FILE *fp;

	if((fp = fopen(path, "rb")) == NULL)
		error_handler(DE_ERROR_FILE_ERROR);

	fread(map_array[i].name, sizeof(char), MAX_NAME_LENGTH, fp);
	fread(&map_array[i].revision, sizeof(uint32), 1, fp);
	fread(&map_array[i].moral, sizeof(uint8), 1, fp);
	fread(&map_array[i].up, sizeof(uint16), 1, fp);
	fread(&map_array[i].down, sizeof(uint16), 1, fp);
	fread(&map_array[i].left, sizeof(uint16), 1, fp);
	fread(&map_array[i].right, sizeof(uint16), 1, fp);
	fread(&map_array[i].music, sizeof(uint8), 1, fp);
	fread(&map_array[i].bootmap, sizeof(int32), 1, fp);
	fread(&map_array[i].bootx, sizeof(uint8), 1, fp);
	fread(&map_array[i].booty, sizeof(uint8), 1, fp);
	fread(&map_array[i].shop, sizeof(uint16), 1, fp);
	fread(map_array[i].tile, sizeof(tile_t), MAX_MAPX * MAX_MAPY, fp);
	fread(map_array[i].npc, sizeof(uint8), MAX_MAP_NPCS, fp);

	fclose(fp);
}

void unload_maps(void)
{
	uint32 i = 0;
	uint32 n = 0;

	for( i = 0; i < MAX_MAPS; i++){
		for( n = 0; n < MAX_MAP_NPCS; n++)
			free(map_array[i].npc[n].vitals);

		free(map_array[i].name);
		free(map_array[i].npc);
		free(map_array[i].items);
		free(map_array[i].tile);
	}
	free(map_array);
	free(map_array_cache);
}

void mapcache_create(uint32 mapnum)
{
	buffer_t buffer;
	uint32 x = 0;

	clear_buffer(&buffer);

	add_opcode(&buffer, SMAPDATA);
	add_buffer(&buffer, &mapnum, SIZE32);
	add_string(&buffer, map(mapnum)->name );
	add_buffer(&buffer, &map(mapnum)->moral,SIZE8);
	add_buffer(&buffer, &map(mapnum)->music,SIZE8);
	add_buffer(&buffer, &map(mapnum)->left,SIZE16);
	add_buffer(&buffer, &map(mapnum)->right,SIZE16);
	add_buffer(&buffer, &map(mapnum)->up,SIZE16);
	add_buffer(&buffer, &map(mapnum)->down,SIZE16);
	add_buffer(&buffer, &map(mapnum)->shop,SIZE16);
	add_buffer(&buffer, &map(mapnum)->bootmap,SIZE32);
	add_buffer(&buffer, &map(mapnum)->bootx,SIZE8);
	add_buffer(&buffer, &map(mapnum)->booty,SIZE8);
	add_buffer(&buffer, &map(mapnum)->revision,SIZE32);

	for( x = 0; x < MAX_MAP_NPCS; x++)
		add_buffer(&buffer, &map(mapnum)->npc[x].num,SIZE16);

	add_buffer(&buffer, map(mapnum)->tile,(MAX_MAPX * MAX_MAPY) * sizeof(tile_t));

	clear_buffer(&map_array_cache[mapnum]);
	map_array_cache[mapnum] = buffer;
}