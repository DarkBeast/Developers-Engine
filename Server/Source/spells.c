/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/
#include "spells.h"
#include <stdio.h>
#include "path.h"
#include "error.h"

spells_t *spell_array;

spells_t *spells(void)
{
	return spell_array;
}

spells_t *spell(uint32 index)
{
	return &spell_array[index];
}

void init_spells(void)
{
	uint32 i = 0;
	char *path;

	spell_array = (spells_t *)calloc(MAX_SPELLS, sizeof(spells_t));

	for(i = 0; i < MAX_SPELLS; i++){
		spell_array[i].name = (char *)calloc(MAX_NAME_LENGTH, sizeof(char));

		path = get_path(SPELL_PATH, i, FILE_ENDING);

		check_dir(SPELL_PATH);

		if(file_readable(path))
			read_spell(path,i);
		else
			write_spell(path,i);
	}
}

void write_spell(char *path, int32 i)
{
	FILE *fp;

	if((fp = fopen(path, "wb")) == NULL)
		error_handler(DE_ERROR_FILE_ERROR);

	fwrite(spell_array[i].name, sizeof(char), MAX_NAME_LENGTH, fp);
	fwrite(&spell_array[i].pic, sizeof(uint16), 1, fp);
	fwrite(&spell_array[i].mpreq, sizeof(uint16), 1, fp);
	fwrite(&spell_array[i].jobreq, sizeof(uint8), 1, fp);
	fwrite(&spell_array[i].levelreq, sizeof(uint8), 1, fp);
	fwrite(&spell_array[i].type, sizeof(uint8), 1, fp);
	fwrite(&spell_array[i].data1, sizeof(uint16), 1, fp);
	fwrite(&spell_array[i].data2, sizeof(uint16), 1, fp);
	fwrite(&spell_array[i].data3, sizeof(uint16), 1, fp);

	fclose(fp);
}

void read_spell(char *path, int32 i)
{
	FILE *fp;

	if((fp = fopen(path, "rb")) == NULL)
		error_handler(DE_ERROR_FILE_ERROR);

	fread(spell_array[i].name, sizeof(char), MAX_NAME_LENGTH, fp);
	fread(&spell_array[i].pic, sizeof(uint16), 1, fp);
	fread(&spell_array[i].mpreq, sizeof(uint16), 1, fp);
	fread(&spell_array[i].jobreq, sizeof(uint8), 1, fp);
	fread(&spell_array[i].levelreq, sizeof(uint8), 1, fp);
	fread(&spell_array[i].type, sizeof(uint8), 1, fp);
	fread(&spell_array[i].data1, sizeof(uint16), 1, fp);
	fread(&spell_array[i].data2, sizeof(uint16), 1, fp);
	fread(&spell_array[i].data3, sizeof(uint16), 1, fp);

	fclose(fp);
}

void unload_spells(void)
{
	uint32 i = 0;

	for(i = 0; i < MAX_SPELLS; i++)
		free(spell_array[i].name);

	free(spell_array);
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