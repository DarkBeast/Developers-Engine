/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/

#ifndef DE_SERVERTCP_H
#define DE_SERVERTCP_H

#include "buffer.h"
#include "integer.h"
#include "packetenum.h"
#include "socket.h"
#include "general.h"

void global_msg(char *msg, uint8 color);

void admin_msg(char *msg, uint8 color);

void player_msg(int16 index, char *msg, uint8 color);

void map_msg(uint16 mapnum, char *msg, uint8 color);

void alert_msg(int16 index, char *msg);

void alert_msg_socket(uint64 id, char * msg);

void hacking_attempt(int16 index, char *msg);

void send_join_map(int16 index);

void send_leave_map(int16 index, uint32 mapnum);

void send_player_data(int16 index);

void send_map(int16 index, uint32 mapnum);

void send_map_items_to(int16 index, uint32 mapnum);

void send_map_items_to_all(uint32 mapnum);

void send_map_npcs_to(int16 index, uint32 mapnum);

void send_map_npcs_to_map(uint32 mapnum);

void send_items(int16 index);

void send_npcs(int16 index);

void send_inventory(int16 index);

void send_inventory_update(int16 index, uint8 invslot);

void send_worn_equipment(int16 index);

void send_vital(int16 index, uint8 vital);

void send_stats(int16 index);

void send_welcome(int16 index);

void send_left_game(int16 index);

void send_player_xy(int16 index);

void send_update_item_to_all(uint16 itemnum);

void send_update_item_to(int16 index, uint16 itemnum);

void send_edit_item_to(int16 index, uint16 itemnum);

void send_update_npc_to_all(uint16 npcnum);

void send_update_npc_to(int16 index, uint16 npcnum);

void send_edit_npc_to(int16 index, uint16 npcnum);

void send_shops(int16 index);

void send_update_shop_to_all(uint16 shopnum);

void send_update_shop_to(int16 index, uint16 shopnum);

void send_edit_shop_to(int16 index, uint16 shopnum);

void send_spells(int16 index);

void send_update_spell_to_all(uint16 spellnum);

void send_update_spell_to(int16 index, uint16 spellnum);

void send_edit_spell_to(int16 index, uint16 spellnum);

void send_trade(int16 index, uint16 shopnum);

void send_players_spells(int16 index);

void send_door_data(int16 index);

#endif