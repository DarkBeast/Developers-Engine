/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/
#ifndef DE_HANDLEPACKETS_H
#define DE_HANDLEPACKETS_H

#include "error.h"
#include "types.h"
#include "buffer.h"
#include "bool.h"
#include "packetenum.h"


//CLOGIN and CNEWACCOUNT are not used in this function.
void initpackets(void);

void handle_data(buffer_t *data, uint64 socket_id);

void handle_login(buffer_t *data, uint64 socket_id);

void handle_new_account(buffer_t *data, uint64 socket_id);

void handle_say_message(buffer_t *data, int16 index);

void handle_emote_message(buffer_t *data, int16 index);

void handle_broadcast_message(buffer_t *data, int16 index);

void handle_global_message(buffer_t *data, int16 index);

void handle_admin_message(buffer_t *data, int16 index);

void handle_player_message(buffer_t *data, int16 index);

void handle_player_move(buffer_t *data, int16 index);

void handle_player_dir(buffer_t *data, int16 index);

void handle_use_item(buffer_t *data, int16 index);

void handle_attack(buffer_t *data, int16 index);

void handle_use_stat_points(buffer_t *data, int16 index);

void handle_warp_me_to(buffer_t *data, int16 index);

void handle_warp_to_me(buffer_t *data, int16 index);

void handle_warp_to(buffer_t *data, int16 index);

void handle_set_spirit(buffer_t *data, int16 index);

void handle_request_new_map(buffer_t *data, int16 index);

void handle_map_data(buffer_t *data, int16 index);

void handle_need_map(buffer_t *data, int16 index);

void handle_map_get_item(buffer_t *data, int16 index);

void handle_map_drop_item(buffer_t *data, int16 index);

void handle_map_respawn(buffer_t *data, int16 index);

void handle_kick_player(buffer_t *data, int16 index);

void handle_ban_player(buffer_t *data, int16 index);

void handle_request_map_edit(buffer_t *data, int16 index);

void handle_request_edit_item(buffer_t *data, int16 index);

void handle_edit_item(buffer_t *data, int16 index);

void handle_delete(buffer_t *data, int16 index);

void handle_save_item(buffer_t *data, int16 index);

void handle_request_edit_npc(buffer_t *data, int16 index);

void handle_edit_npc(buffer_t *data, int16 index);

void handle_save_npc(buffer_t *data, int16 index);

void handle_request_edit_shop(buffer_t *data, int16 index);

void handle_edit_shop(buffer_t *data, int16 index);

void handle_save_shop(buffer_t *data, int16 index);

void handle_request_edit_spell(buffer_t *data, int16 index);

void handle_edit_spell(buffer_t *data, int16 index);

void handle_save_spell(buffer_t *data, int16 index);

void handle_set_group(buffer_t *data, int16 index);

void handle_who_is_online(buffer_t *data, int16 index);

void handle_trade(buffer_t *data, int16 index);

void handle_trade_request(buffer_t *data, int16 index);

void handle_fix_item(buffer_t *data, int16 index);

void handle_search(buffer_t *data, int16 index);

void handle_spells(buffer_t *data, int16 index);

void handle_cast(buffer_t *data, int16 index);

void handle_quit(buffer_t *data, int16 index);
#endif