/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/
#include "shops.h"
#include <stdio.h>
#include "path.h"
#include "error.h"

shop_t *shop_array;

shop_t *shops(void)
{
	return shop_array;
}

shop_t *shop(uint32 index)
{
	return &shop_array[index];
}

void init_shops(void)
{
	uint32 i = 0;
	char *path;

	shop_array = (shop_t *)calloc(MAX_SHOPS, sizeof(shop_t));

	for(i = 0; i < MAX_SHOPS; i++){
		shop_array[i].name = (char *)calloc(MAX_NAME_LENGTH, sizeof(char));
		shop_array[i].joinsay = (char *)calloc(MAX_SAY_LENGTH, sizeof(char));
		shop_array[i].leavesay = (char *)calloc(MAX_SAY_LENGTH, sizeof(char));
		shop_array[i].tradeitem = (trade_item_t *)calloc(MAX_TRADES, sizeof(trade_item_t));

		path = get_path(SHOP_PATH, i, FILE_ENDING);

		check_dir(SHOP_PATH);

		if(file_readable(path)){
			read_shop(path,i);
		}
		else{
			write_shop(path,i);
		}
	}
}

void write_shop(char *path, int32 i)
{
	FILE *fp;

	if((fp = fopen(path, "wb")) == NULL)
		error_handler(DE_ERROR_FILE_ERROR);

	fwrite(shop_array[i].name, sizeof(char), MAX_NAME_LENGTH, fp);
	fwrite(shop_array[i].joinsay, sizeof(char), MAX_SAY_LENGTH, fp);
	fwrite(shop_array[i].leavesay, sizeof(char), MAX_SAY_LENGTH, fp);
	fwrite(&shop_array[i].fixesitems, sizeof(uint8), 1, fp);
	fwrite(shop_array[i].tradeitem, sizeof(trade_item_t), MAX_TRADES, fp);

	fclose(fp);
}

void read_shop(char *path, int32 i)
{
	FILE *fp;

	if((fp = fopen(path, "rb")) == NULL)
		error_handler(DE_ERROR_FILE_ERROR);

	fwrite(shop_array[i].name, sizeof(char), MAX_NAME_LENGTH, fp);
	fwrite(shop_array[i].joinsay, sizeof(char), MAX_SAY_LENGTH, fp);
	fwrite(shop_array[i].leavesay, sizeof(char), MAX_SAY_LENGTH, fp);
	fwrite(&shop_array[i].fixesitems, sizeof(uint8), 1, fp);
	fwrite(shop_array[i].tradeitem, sizeof(trade_item_t), MAX_TRADES, fp);

	fclose(fp);
}

void unload_shops(void)
{
	uint32 i = 0;

	for(i = 0; i < MAX_SHOPS; i++){
		free(shop_array[i].name);
		free(shop_array[i].joinsay);
		free(shop_array[i].leavesay);
		free(shop_array[i].tradeitem);
	}
	free(shop_array);
}

void clear_shop(uint16 shopnum)
{
	uint32 i = 0;

	shop_array[shopnum].name = "";
	shop_array[shopnum].joinsay = "";
	shop_array[shopnum].leavesay = "";
	shop_array[shopnum].fixesitems = 0;

	for(i = 0; i < MAX_TRADES; i++){
		shop_array[shopnum].tradeitem[i].getitem = 0;
		shop_array[shopnum].tradeitem[i].getvalue= 0;
		shop_array[shopnum].tradeitem[i].giveitem = 0;
		shop_array[shopnum].tradeitem[i].givevalue = 0;
	}
}