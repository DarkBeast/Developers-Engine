/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/

#ifndef DE_CLIENTTCP_H
#define DE_CLIENTTCP_H

#include "buffer.h"
#include "integer.h"
#include "packetenum.h"

void send_data(buffer_t *data);

void send_new_account(char *name, char *charname, char *password, uint8 job, uint8 sex);

void send_login(char *name, char *password);

void say_msg(char *text);

void global_msg(char *text);

void broadcast_msg(char *text);

void emote_msg(char *text);

void player_msg(char *text, char *name);

void admin_msg(char *text);

void send_player_move(void);

void send_player_dir(void);

void request_new_map(void);

void send_map(void);

void warp_me_to(char *name);

void warp_to_me(char *name);

void warp_to(uint32 mapnum);

void send_set_access(char *name, uint8 group);

void send_set_sprite(char *name, uint32 spritenum);

void send_kick(char *name);

void send_ban(char *name);

void send_request_edit_item(void);

void send_save_item(uint16 itemnum);

void send_request_edit_npc(void);

void send_save_npc(uint16 npcnum);

void send_map_respawn(void);

void send_use_item(uint16 invnum);

void send_drop_item(uint16 invnum,uint32 amount);

void send_whos_online(void);

void send_request_edit_shop(void);

void send_save_shop(uint16 shopnum);

void send_request_edit_spell(void);

void send_save_spell(uint16 spellnum);

void send_request_edit_map(void);

#endif