/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/

#ifndef DE_GAMELOGICS_H
#define DE_GAMELOGICS_H
#include "integer.h"

void GameLoop(void);

void process_movement(uint16 index);

void process_npc_movement(uint8 mapnpcnum);

sbool is_trying_to_move(void);

void check_attack(void);

sbool can_move(void);

sbool check_direction(uint8 dir);

void check_movement(void);

void update_inventory(void);

void player_search(uint16 curx, uint16 cury);

sbool is_in_bounds(void);

void use_item(void);

void cast_spell(void);

void init_map_data(void);

void dev_msg(char *text, uint8 color);
#endif