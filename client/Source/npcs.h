/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/
#ifndef DE_NPCS_H
#define DE_NPCS_H

#include "types.h"
#include "globals.h"
#include <stdlib.h>

typedef struct npc_t npc_t;
typedef enum behavior_t behavior_t;

struct npc_t{
	char *name;
	char *say;
	uint16 sprite;
	uint32 spawnsecs;
	uint8 behavior;
	uint8 range;
	uint16 dropitem;
	uint16 dropchance;
	uint16 dropitemvalue;
	uint8 *stat;
};

enum behavior_t{
	BEHAVIOR_ATTACKONSIGHT,
	BEHAVIOR_ATTACKWHENATTACKED,
	BEHAVIOR_FRIENDLY,
	BEHAVIOR_SHOPKEEPER,
	BEHAVIOR_GUARD,
	BEHAVIOR_COUNT
};

npc_t *npcs(void);

npc_t *npc(uint32 index);

void init_npcs(void);

void unload_npcs(void);

void clear_npc(uint16 npcnum);

#endif