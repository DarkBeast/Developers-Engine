/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/
#include "items.h"
#include <stdio.h>
#include <string.h>
#include "path.h"
#include "error.h"

item_t *item_array;

item_t *items(void)
{
	return item_array;
}

item_t *item(uint32 index)
{
	return &item_array[index];
}

void init_items(void)
{
	uint32 i = 0;
	char *path;

	item_array = (item_t *)calloc(MAX_ITEMS, sizeof(item_t));

	for(i = 0; i < MAX_ITEMS; i++){
		item_array[i].name = (char *)calloc(MAX_NAME_LENGTH, sizeof(char));

		path = get_path(ITEM_PATH, i, FILE_ENDING);

		check_dir(ITEM_PATH);

		if(file_readable(path)){
			read_item(path,i);
		}
		else{
			write_item(path,i);
		}
	}
}

void write_item(char *path, int32 i)
{
	FILE *fp;

	if((fp = fopen(path, "wb")) == NULL)
		error_handler(DE_ERROR_FILE_ERROR);

	fwrite(item_array[i].name, sizeof(char), MAX_NAME_LENGTH, fp);
	fwrite(&item_array[i].pic, sizeof(uint16), 1, fp);
	fwrite(&item_array[i].type, sizeof(uint8), 1, fp);
	fwrite(&item_array[i].data1, sizeof(uint16), 1, fp);
	fwrite(&item_array[i].data2, sizeof(uint16), 1, fp);
	fwrite(&item_array[i].data3, sizeof(uint16), 1, fp);

	fclose(fp);
}

void read_item(char *path, int32 i)
{
	FILE *fp;

	if((fp = fopen(path, "rb")) == NULL)
		error_handler(DE_ERROR_FILE_ERROR);

	fread(item_array[i].name, sizeof(char), MAX_NAME_LENGTH, fp);
	fread(&item_array[i].pic, sizeof(uint16), 1, fp);
	fread(&item_array[i].type, sizeof(uint8), 1, fp);
	fread(&item_array[i].data1, sizeof(uint16), 1, fp);
	fread(&item_array[i].data2, sizeof(uint16), 1, fp);
	fread(&item_array[i].data3, sizeof(uint16), 1, fp);

	fclose(fp);
}

void unload_items(void)
{
	uint32 i = 0;

	for(i = 0; i < MAX_ITEMS; i++){
		free(item_array[i].name);
	}

	free(item_array);
}

void clear_item(uint16 itemnum)
{
	item_array[itemnum].name = "";
	item_array[itemnum].pic = 0;
	item_array[itemnum].type = ITEM_TYPE_NONE;
	item_array[itemnum].data1 = 0;
	item_array[itemnum].data2 = 0;
	item_array[itemnum].data3 = 0;
}