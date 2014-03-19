/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/
#include "spells.h"
#include "error.h"

spells_t *spell_array;
spell_anim_t *spell_anim_array;

spells_t *spells(void)
{
	return spell_array;
}

spells_t *spell(uint32 index)
{
	return &spell_array[index];
}

spell_anim_t *spell_anim(uint32 index)
{
	return &spell_anim_array[index];
}

void init_spells(void)
{
	uint32 i = 0;
	spell_anim_array = (spell_anim_t *)calloc(MAX_SPELLANIM, sizeof(spell_anim_t));
	spell_array = (spells_t *)calloc(MAX_SPELLS, sizeof(spells_t));

	for(i = 0; i < MAX_SPELLS; i++)
		spell_array[i].name = (char *)calloc(MAX_NAME_LENGTH, sizeof(char));
}

void unload_spells(void)
{
	uint32 i = 0;

	for(i = 0; i < MAX_SPELLS; i++)
		free(spell_array[i].name);

	free(spell_array);
	free(spell_anim_array);
}

void clear_spell(uint16 spellnum)
{
	spell_array[spellnum].name = "";
	spell_array[spellnum].pic = 0;
	spell_array[spellnum].mpreq = 0;
	spell_array[spellnum].jobreq = 0;
	spell_array[spellnum].levelreq = 0;
	spell_array[spellnum].type = 0;
	spell_array[spellnum].data1 = 0;
	spell_array[spellnum].data2 = 0;
	spell_array[spellnum].data3 = 0;
}