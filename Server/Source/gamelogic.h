/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/
#ifndef DE_GAMELOGIC_H
#define DE_GAMELOGIC_H
#include "integer.h"

uint16 get_player_max_vital(int16 index, uint8 vital);

void attack_npc(int16 attacker, uint8 mapnpcnum, uint32 damage);

void attack_player(int16 attacker, int16 victim, uint32 damage);

void attack_npc_with_magic(int16 attacker, uint8 mapnpcnum, uint32 damage, uint16 spellnum);

void attack_player_with_magic(int16 attacker, int16 victim, uint32 damage, uint16 spellnum);

uint32 find_open_map_item_slot(uint32 mapnum);

void join_game(int16 index);

void left_game(int16 index);

void spawn_item(uint16 itemnum, uint32 itemval, uint32 mapnum, uint8 x, uint8 y);

void spawn_item_slot(uint8 mapitemslot, uint16 itemnum, uint32 itemval, uint32 itemdur, uint32 mapnum, uint8 x, uint8 y);

void spawn_all_maps_items(void);

void spawn_map_items(uint32 mapnum);

void spawn_npc(uint8 mapnpcnum, uint32 mapnum);

void spawn_map_npcs(uint32 mapnum);

void spawn_all_map_npcs(void);

sbool can_attack_player(int16 attacker, int16 victim);

sbool can_attack_npc(int16 attacker, uint8 mapnpcnum);

sbool can_npc_attack_player(uint8 mapnpcnum, int16 index);

void npc_attack_player(uint8 mapnpcnum, int16 victim, uint32 damage);

sbool can_npc_move(uint32 mapnum, uint8 mapnpcnum, uint8 dir);

void npc_move(uint32 mapnum, uint8 mapnpcnum, uint8 dir);

void npc_dir(uint32 mapnum, uint8 mapnpcnum, uint8 dir);

uint32 get_npc_max_vital(uint32 npcnum, uint8 vital);

uint32 get_npc_vital_regen(uint32 npcnum, uint8 vital);

uint32 get_player_damage(int16 index);

uint32 get_player_protection(int16 index);

sbool can_player_critical_hit(int16 index);

sbool can_player_block_hit(int16 index);

void cast_spell(int16 index, uint8 spellslot);

void player_warp(int16 index, uint32 mapnum, uint8 x, uint8 y);

void player_move(int16 index, uint8 dir, uint8 movement);

void check_equipped_items(int16 index);

uint32 find_open_inv_slot(int16 index, uint16 itemnum);

uint32 has_item(int16 index, uint16 itemnum);

void take_item(int16 index, uint16 itemnum, uint32 itemval);

void give_item(int16 index, uint16 itemnum, uint32 itemval);

sbool has_spell(int16 index, uint32 spellnum);

uint16 find_open_spell_slot(int16 index);

void player_map_get_item(int16 index);

void player_map_drop_item(int16 index, uint8 invnum, uint32 amount);

uint64 get_player_next_level(int16 index);

void check_player_level_up(int16 index);

uint32 get_player_vital_regen(int16 index, uint8 vital);

void on_death(int16 index);

void damage_equipment(int16 index, uint8 equipmentslot);

#endif