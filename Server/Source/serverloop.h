/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/
#ifndef DE_SERVERLOOP_H
#define DE_SERVERLOOP_H

#include "integer.h"

void server_loop(void);

void update_map_spawn_items(void);

void update_npc_ai(void);

void ai_hp_regen(double  tickcount, uint32  mapnum, uint8 mapnpcnum);

void ai_attack_on_sight(uint8 mapnpcnum, uint16 target, uint32 mapnum);

sbool ai_npc_move(uint32 mapnum, uint8 mapnpcnum, uint16 target);

void ai_npc_collision(sbool didwalk, uint32 mapnum , uint8 mapnpcnum, uint16 target);

void update_player_vitals(void);

void update_save_players(void);

void set_server_offline(void);

void handle_shutdown(void);
#endif