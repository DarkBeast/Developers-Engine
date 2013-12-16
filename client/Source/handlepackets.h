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

void init_packets(void);

void incomming_packets(buffer_t *data);

void handle_loginok(buffer_t *data);

void handle_alert_message(buffer_t *data);

void handle_in_game(buffer_t *data);

void handle_player_inv(buffer_t *data);

void handle_player_inv_update(buffer_t *data);

void handle_player_worn_eq(buffer_t *data);

void handle_player_hp(buffer_t *data);

void handle_player_mp(buffer_t *data);

void handle_player_sp(buffer_t *data);

void handle_player_stats(buffer_t *data);

void handle_player_data(buffer_t *data);

void handle_player_move(buffer_t *data);

void handle_npc_move(buffer_t *data);

void handle_player_dir(buffer_t *data);

void handle_npc_dir(buffer_t *data);

void handle_player_xy(buffer_t *data);

void handle_attack(buffer_t *data);

void handle_npc_attack(buffer_t *data);

void handle_check_for_map(buffer_t *data);

void handle_map_data(buffer_t *data);

void handle_map_item_data(buffer_t *data);

void handle_map_npc_data(buffer_t *data);

void handle_map_done(buffer_t *data);

void handle_say_msg(buffer_t *data);

void handle_broadcast_msg(buffer_t *data);

void handle_global_msg(buffer_t *data);

void handle_player_msg(buffer_t *data);

void handle_map_msg(buffer_t *data);

void handle_admin_msg(buffer_t *data);

void handle_refreash(buffer_t *data);

void handle_spawn_item(buffer_t *data);

void handle_item_editor(buffer_t *data);

void handle_update_item(buffer_t *data);

void handle_edit_item(buffer_t *data);

void handle_spawn_npc(buffer_t *data);

void handle_npc_dead(buffer_t *data);

void handle_npc_editor(buffer_t *data);

void handle_update_npc(buffer_t *data);

void handle_edit_npc(buffer_t *data);

void handle_edit_map(buffer_t *data);

void handle_shop_editor(buffer_t *data);

void handle_update_shop(buffer_t *data);

void handle_edit_shop(buffer_t *data);

void handle_spell_editor(buffer_t *data);

void handle_update_spell(buffer_t *data);

void handle_edit_spell(buffer_t *data);

void handle_trade(buffer_t *data);

void handle_spells(buffer_t *data);

void handle_left(buffer_t *data);

void handle_high_index(buffer_t *data);

void handle_spell_cast(buffer_t *data);
#endif