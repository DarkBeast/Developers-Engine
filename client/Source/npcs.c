/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/
#include "npcs.h"
#include "error.h"

npc_t *npc_array;

npc_t *npcs(void)
{
	return npc_array;
}

npc_t *npc(uint32 index)
{
	return &npc_array[index];
}

void init_npcs(void)
{
	uint32 i = 0;
	char *path;

	npc_array = (npc_t *)calloc(MAX_NPCS, sizeof(npc_t));

	for( i = 0; i < MAX_NPCS; i++){
		npc_array[i].name = (char *)calloc(MAX_NAME_LENGTH, sizeof(char));
		npc_array[i].say = (char *)calloc(MAX_SAY_LENGTH, sizeof(char));
		npc_array[i].stat = (uint8 *)calloc(STAT_COUNT, sizeof(uint8));
	}
}

void unload_npcs(void)
{
	uint32 i = 0;

	for( i = 0; i < MAX_NPCS; i++){
		free(npc_array[i].name);
		free(npc_array[i].say);
		free(npc_array[i].stat);
	}

	free(npc_array);
}

void clear_npc(uint16 npcnum)
{
	npc_array[npcnum].name = "";
	npc_array[npcnum].say = "";
	npc_array[npcnum].sprite = 0;
	npc_array[npcnum].spawnsecs = 0;
	npc_array[npcnum].behavior = BEHAVIOR_FRIENDLY;
	npc_array[npcnum].range = 0;
	npc_array[npcnum].dropitem = 0;
	npc_array[npcnum].dropchance = 0;
	npc_array[npcnum].dropitemvalue = 0;
	npc_array[npcnum].stat[STAT_STRENGTH] = 0;
	npc_array[npcnum].stat[STAT_DEFENSE] = 0;
	npc_array[npcnum].stat[STAT_SPEED] = 0;
	npc_array[npcnum].stat[STAT_MAGIC] = 0;
}