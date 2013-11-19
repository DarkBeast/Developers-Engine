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

spells_t *spells(void);

spells_t *spell(uint32 index);

void init_spells(void);

void write_spell(char *path, int32 i);

void read_spell(char *path, int32 i);

void unload_spells(void);

void clear_spell(uint16 spellnum);
#endif