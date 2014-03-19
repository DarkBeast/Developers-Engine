/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/
#include "general.h"
#include "error.h"
#include "path.h"
#include "socket.h"
#include "widget.h"
#include "mainmenu.h"
#include "credits.h"
#include "main.h"
#include "login.h"
#include "newaccount.h"
#include "status.h"

uint32 tileset_count = 0;
uint32 sprite_count = 0;
uint32 spell_count = 0;
uint32 item_count = 0;
uint8 editor_t;
uint8 menu_state = MENU_STATE_STATUS;

void set_menu_state(uint8 state)
{
	menu_state = state;
}

uint8 get_menu_state(void)
{
	return menu_state;
}

uint8 editor(void)
{
	return editor_t;
}

void set_editor(uint8 editor)
{
	editor_t = editor;
}

uint32 tileset_max(void)
{
	return tileset_count;
}

uint32 sprite_max(void)
{
	return sprite_count;
}

uint32 spell_max(void)
{
	return spell_count;
}

uint32 item_max(void)
{
	return item_count;
}

void init_client(void)
{
	printf("Initializing Data. \n");

	check_dir(LOG_PATH);
	check_tiles();
	check_sprites();
	check_items();
	check_spells();

	init_items();
	init_maps();
	init_npcs();
	init_shops();
	init_spells();
	init_players();

	printf("Data Initialized. \n");
}

void destroy_client(void)
{
	printf("Clearing Data.\n");
	unload_shops();
	unload_spells();
	unload_items();
	unload_npcs();
	unload_players();
	unload_maps();
	unloadsocket();
	path_destroy();
}

void check_tiles(void)
{
	uint32 i = 1;
	char *path = NULL;

	path = get_path(TILE_PATH,i,IMAGE_ENDING);
	while(file_exists(path)){
		tileset_count++;
		i++;
		path = get_path(TILE_PATH,i,IMAGE_ENDING);
	}
}

void check_sprites(void)
{
	uint32 i = 1;
	char *path = NULL;

	path = get_path(SPRITE_PATH,i,IMAGE_ENDING);
	while(file_exists(path)){
		sprite_count++;
		i++;
		path = get_path(SPRITE_PATH,i,IMAGE_ENDING);
	}
}

void check_spells(void)
{
	uint32 i = 1;
	char *path = NULL;

	path = get_path(SPELL_PATH,i,IMAGE_ENDING);
	while(file_exists(path)){
		spell_count++;
		i++;
		path = get_path(SPELL_PATH,i,IMAGE_ENDING);
	}
}

void check_items(void)
{
	uint32 i = 1;
	char *path = NULL;

	path = get_path(ITEM_PATH,i,IMAGE_ENDING);
	while(file_exists(path)){
		item_count++;
		i++;
		path = get_path(ITEM_PATH,i,IMAGE_ENDING);
	}
}

void menustate(void)
{
	sbool run = TRUE;

	while(run){
		if(widget_get_uip()->root->shown.count > 0 || widget_get_uip()->root->hidden.count > 0)
			widget_unload(widget_get_uip()->root);

		switch(menu_state){
		case MENU_STATE_MAIN:
			init_main_menu();//initializes the first menu for use.
			main_menu();//Start the First Menu, Menu main loop.
			break;
		case MENU_STATE_CREDITS:
			init_credits();
			credits();
			break;
		case MENU_STATE_LOGIN:
			init_login();
			login();
			break;
		case MENU_STATE_CREATE:
			init_new_account();
			new_account();
			break;
		case MENU_STATE_STATUS:
			if(get_status_type() == STATUS_TYPE_SOCKET)
				init_status("");
			else
				init_status_loader("Widget System Initialized.");
			status();
			break;
		case MENU_STATE_EXIT:
			run = FALSE;
			break;
		}
	}
}