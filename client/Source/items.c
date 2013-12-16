/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/
#include "items.h"
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

	item_array = (item_t *)calloc(MAX_ITEMS, sizeof(item_t));

	for(i = 0; i < MAX_ITEMS; i++){
		item_array[i].name = (char *)calloc(MAX_NAME_LENGTH, sizeof(char));
	}
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