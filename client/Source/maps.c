/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/

#include "maps.h"
#include <stdio.h>
#include "path.h"
#include "error.h"
#include "socket.h"

map_t map_data;
map_images map_img;
tile_layout tile_buffer_t;

map_t *map(void)
{
	return &map_data;
}

tile_layout *tile_buffers(void)
{
	return &tile_buffer_t;
}

void preload_map_tiles(void)
{
	uint32 x = 0, y = 0;

	for(x = 0; x < MAX_MAPX; x++){
		for(y = 0; y < MAX_MAPY; y++){
			uint32 x2 = x * TILE_SIZE, y2 = y * TILE_SIZE;

			if(x == 0)
				x2 = 0;

			if(y == 0)
				y2 = 0;

			if(map_data.tile[GETXY(x,y)].ground.Tileset > 0)
				add_new_map_image(map_data.tile[GETXY(x,y)].ground.Tileset);

			if(map_data.tile[GETXY(x,y)].mask.Tileset > 0)
				add_new_map_image(map_data.tile[GETXY(x,y)].mask.Tileset);

			if(map_data.tile[GETXY(x,y)].anim.Tileset > 0)
				add_new_map_image(map_data.tile[GETXY(x,y)].anim.Tileset);

			if(map_data.tile[GETXY(x,y)].fringe.Tileset > 0)
				add_new_map_image(map_data.tile[GETXY(x,y)].fringe.Tileset);

			create_tile_vertex_buffer(&tile_buffer_t.ground[GETXY(x,y)],x2,y2,map_data.tile[GETXY(x,y)].ground.x,map_data.tile[GETXY(x,y)].ground.y);
			create_tile_vertex_buffer(&tile_buffer_t.mask[GETXY(x,y)],x2,y2,map_data.tile[GETXY(x,y)].mask.x,map_data.tile[GETXY(x,y)].mask.y);
			create_tile_vertex_buffer(&tile_buffer_t.anim[GETXY(x,y)],x2,y2,map_data.tile[GETXY(x,y)].anim.x,map_data.tile[GETXY(x,y)].anim.y);
			create_tile_vertex_buffer(&tile_buffer_t.fringe[GETXY(x,y)],x2,y2,map_data.tile[GETXY(x,y)].fringe.x,map_data.tile[GETXY(x,y)].fringe.y);
		}
	}
}

image *get_map_image(uint8 id)
{
	return &map_img.img[id];
}

uint8 add_new_map_image(uint8 id)
{
	uint8 i = 0;

	for( i = 0; i < map_img.max; i++){
		if(id = map_img.id[i])
			return i;

		if(map_img.img[i].texid == 0){
			char *path = get_path(TILE_PATH,id,IMAGE_ENDING);
			load_image(path,&map_img.img[i]);
			map_img.id[i] = id;
			map_img.count++;
			return i;
		}
	}

	//could not find a spot or duplicate so lets resize and place into the next spot.
	if( map_img.max * 2 >= 254)
		return 0;

	map_image_resize();

	{
		char *path = get_path(TILE_PATH,id,IMAGE_ENDING);
		map_img.count++;
		load_image(path,&map_img.img[map_img.count]);
		map_img.id[map_img.count] = id;
		return map_img.count;
	}
}

void clear_map_img(void)
{
	uint8 i = 0;

	for( i = 0; i < map_img.max; i++){
		map_img.img[i].texid = 0;
		map_img.id[i] = 0;
	}
	map_img.count = 0;
}

void map_image_resize(void)
{
	image **data;
	uint8 **data2;
	map_img.max *= 2;

	data = (image **)realloc(map_img.img, map_img.max * sizeof(image));
	data2 = (uint8 **)realloc(map_img.id, map_img.max * sizeof(uint8));

	if (data == NULL)
		return;

	if (data2 == NULL)
		return;
}

void init_maps(void)
{
	uint32 n = 0;

	map_data.name = (char *)calloc(MAX_NAME_LENGTH, sizeof(char));
	map_data.npc = (map_npc_t *)calloc(MAX_MAP_NPCS, sizeof(map_npc_t));
	map_data.tile = (tile_t *)calloc(MAX_MAPX * MAX_MAPY, sizeof(tile_t));
	map_data.items = (map_item_t *)calloc(MAX_MAP_ITEMS, sizeof(map_item_t));

	map_img.img = (image *)calloc(5, sizeof(image));
	map_img.id = (uint8 *)calloc(5, sizeof(uint8));
	map_img.count = 0;
	map_img.max = 5;

	tile_buffer_t.ground = (tile_layout_data *)calloc(MAX_MAPX * MAX_MAPY, sizeof(tile_layout_data));
	tile_buffer_t.mask = (tile_layout_data *)calloc(MAX_MAPX * MAX_MAPY, sizeof(tile_layout_data));
	tile_buffer_t.anim = (tile_layout_data *)calloc(MAX_MAPX * MAX_MAPY, sizeof(tile_layout_data));
	tile_buffer_t.fringe = (tile_layout_data *)calloc(MAX_MAPX * MAX_MAPY, sizeof(tile_layout_data));

	for( n = 0; n < MAX_MAP_NPCS; n++)
		map_data.npc[n].vitals = (uint16 *)calloc(VITAL_COUNT, sizeof(uint16));

	check_dir(MAP_PATH);
}

void write_map(char *path)
{
	FILE *fp;

	if((fp = fopen(path, "wb")) == NULL)
		error_handler(DE_ERROR_FILE_ERROR,"Could Not Write Map.\n");

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
		error_handler(DE_ERROR_FILE_ERROR,"Map Could not be Read.\n");

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

	for( n = 0; n < MAX_MAP_NPCS; n++)
		free(map_data.npc[n].vitals);

	free(map_data.name);
	free(map_data.npc);
	free(map_data.items);
	free(map_data.tile);
	free(tile_buffer_t.ground);
	free(tile_buffer_t.mask);
	free(tile_buffer_t.anim);
	free(tile_buffer_t.fringe);
	free(map_img.img);
	free(map_img.id);
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