/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/
#ifndef DE_GENERAL_H
#define DE_GENERAL_H
#include "items.h"
#include "maps.h"
#include "npcs.h"
#include "players.h"
#include "shops.h"
#include "spells.h"
#include "integer.h"

typedef enum menustates  menustates;

enum menustates{
	MENU_STATE_MAIN,
	MENU_STATE_CREDITS,
	MENU_STATE_LOGIN,
	MENU_STATE_CREATE,
	MENU_STATE_EXIT,
	MENU_STATE_STATUS,
	MENU_STATE_MAX
};

void set_menu_state(uint8 state);

uint8 get_menu_state(void);

uint8 editor(void);

void set_editor(uint8 editor);

uint32 tileset_max(void);

uint32 sprite_max(void);

uint32 spell_max(void);

uint32 item_max(void);

void init_client(void);

void destroy_client(void);

void check_tiles(void);

void check_sprites(void);

void check_spells(void);

void check_items(void);

void menustate(void);
#endif