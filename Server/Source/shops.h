/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/
#ifndef DE_SHOPS_H
#define DE_SHOPS_H

#include "integer.h"
#include "globals.h"
#include <stdlib.h>

typedef struct trade_item_t trade_item_t;
typedef struct shop_t shop_t;

struct trade_item_t{
	uint16 giveitem;
	uint32 givevalue;
	uint16 getitem;
	uint32 getvalue;
};

struct shop_t{
	char *name;
	char *joinsay;
	char *leavesay;
	sbool fixesitems;
	trade_item_t *tradeitem;
};

shop_t *shops(void);

shop_t *shop(uint32 index);

void init_shops(void);

void write_shop(char *path, int32 i);

void read_shop(char *path, int32 i);

void unload_shops(void);

void clear_shop(uint16 shopnum);
#endif