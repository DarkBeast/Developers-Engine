/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/
#ifndef DE_ITEMS_H
#define DE_ITEMS_H

#include "integer.h"
#include "globals.h"
#include <stdlib.h>

typedef struct item_t item_t;
typedef enum item_type item_type;

struct item_t{
	char *name;
	uint16 pic;
	uint8 type;
	uint16 data1;
	uint16 data2;
	uint16 data3;
};

enum item_type{
	ITEM_TYPE_NONE,
	ITEM_TYPE_WEAPON,
	ITEM_TYPE_ARMOR,
	ITEM_TYPE_HELMET,
	ITEM_TYPE_SHIELD,
	ITEM_TYPE_POTIONADDHP,
	ITEM_TYPE_POTIONADDMP,
	ITEM_TYPE_POTIONADDSP,
	ITEM_TYPE_POTIONSUBHP,
	ITEM_TYPE_POTIONSUBMP,
	ITEM_TYPE_POTIONSUBSP,
	ITEM_TYPE_KEY,
	ITEM_TYPE_CURRENCY,
	ITEM_TYPE_SPELL,
	ITEM_TYPE_COUNT
};

item_t *items(void);

item_t *item(uint32 index);

void init_items(void);

void clear_item(uint16 itemnum);

//used to unload the items before closing server.
void unload_items(void);

#endif