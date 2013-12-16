/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/
#ifndef DE_SPELLS_H
#define DE_SPELLS_H

#include "integer.h"
#include "globals.h"
#include <stdlib.h>

typedef struct spells_t spells_t;
typedef enum spell_type spell_type;
typedef struct spell_anim_t spell_anim_t;

struct spells_t{
	char *name;
	uint16 pic;
	uint16 mpreq;
	uint8 jobreq;
	uint8 levelreq;
	uint8 type;
	uint16 data1;
	uint16 data2;
	uint16 data3;
};

enum spell_type{
	SPELL_ADDHP,
	SPELL_ADDSP,
	SPELL_SUBHP,
	SPELL_SUBMP,
	SPELL_SUBSP,
	SPELL_GIVEITEM,
	SPELL_TYPE_COUNT
};

struct spell_anim_t{
	uint16 spellnum;
	double timer;
	uint32 framepointer;
	uint8 x;
	uint8 y;
};

spells_t *spells(void);

spells_t *spell(uint32 index);

spell_anim_t *spell_anim(uint32 index);

void init_spells(void);

void unload_spells(void);

void clear_spell(uint16 spellnum);
#endif