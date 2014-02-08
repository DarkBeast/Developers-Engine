/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/
#include "servertcp.h"
#include "path.h"
#include "gamelogic.h"

void global_msg(char * msg, uint8 color)
{
	buffer_t buffer;
	uint32 len;

	clear_buffer(&buffer);
	add_opcode(&buffer, SGLOBALMSG);

	len = strlen(msg);
	add_buffer(&buffer, &len, SIZE32);
	add_string(&buffer, msg);
	add_buffer(&buffer, &color, SIZE8);

	send_data_to_all(&buffer);
}

void admin_msg(char * msg, uint8 color)
{
	buffer_t buffer;
	uint32 len;
	uint32 i = 0;

	clear_buffer(&buffer);
	add_opcode(&buffer, SADMINMSG);

	len = strlen(msg);
	add_buffer(&buffer, &len, SIZE32);
	add_string(&buffer, msg);
	add_buffer(&buffer, &color, SIZE8);

	for( i = 0; i < players_online_count(); i++){
		if(player(player_online(i))->group > GROUP_USER)
			send_data(&buffer, player_online(i));
	}
}

void player_msg(int16 index, char * msg, uint8 color)
{
	buffer_t buffer;
	uint32 len;

	clear_buffer(&buffer);
	add_opcode(&buffer, SPLAYERMSG);

	len = strlen(msg);
	add_buffer(&buffer, &len, SIZE32);
	add_string(&buffer, msg);
	add_buffer(&buffer, &color, SIZE8);

	send_data(&buffer, index);
}

void map_msg(uint16 mapnum, char * msg, uint8 color)
{
	buffer_t buffer;
	uint32 len;

	clear_buffer(&buffer);
	add_opcode(&buffer, SMAPMSG);

	len = strlen(msg);
	add_buffer(&buffer, &len, SIZE32);
	add_string(&buffer, msg);
	add_buffer(&buffer, &color, SIZE8);

	send_data_to_map(&buffer, mapnum);
}

void alert_msg(int16 index, char * msg)
{
	buffer_t buffer;
	uint32 len;

	clear_buffer(&buffer);
	add_opcode(&buffer, SALERTMSG);

	len = strlen(msg);
	add_buffer(&buffer, &len, SIZE32);
	add_string(&buffer, msg);

	send_data(&buffer, index);
	clear_user_socket(get_temp_player_bufferevent(index));
}

void alert_msg_socket(struct bufferevent *bev, char * msg)
{
	buffer_t buffer;
	uint32 len;

	clear_buffer(&buffer);
	add_opcode(&buffer, SALERTMSG);

	len = strlen(msg);
	add_buffer(&buffer, &len, SIZE32);
	add_string(&buffer, msg);

	send_socket_data(&buffer, bev);
	clear_user_socket(bev);
}

void hacking_attempt(int16 index, char * msg)
{
	char *string = NULL;

	if(index > -1){
		if(temp_player(index)->loggedin){
			string = comb_3str(player(index)->username, "/ has been booted for", msg);
			global_msg(string, 0);
		}

		string = comb_3str("You have lost your connection with ",GAME_NAME ,".");
		alert_msg(index, string);
	}
}

void send_join_map(int16 index)
{
	int16 i = 0;
	int16 pindex;
	buffer_t buffer;
	// Send all players on current map to index
	for( i = 0; i < players_online_count(); i++){
		if( player_online(i) != index  && player(player_online(i))->map == player(index)->map){
			clear_buffer(&buffer);

			pindex = player_online(i);
			add_opcode(&buffer, SPLAYERDATA);
			add_buffer(&buffer, &pindex, SIZE16);
			add_string(&buffer, player(pindex)->charname);
			add_buffer(&buffer, &player(pindex)->sprite, SIZE16);
			add_buffer(&buffer, &player(pindex)->map, SIZE16);
			add_buffer(&buffer, &player(pindex)->x, SIZE8);
			add_buffer(&buffer, &player(pindex)->y, SIZE8);
			add_buffer(&buffer, &player(pindex)->dir, SIZE8);
			add_buffer(&buffer, &player(pindex)->group, SIZE8);
			add_buffer(&buffer, &player(pindex)->pk, SIZE8);

			send_data(&buffer, index);
		}
	}

	//send update to all players on the map about new player on map.
	clear_buffer(&buffer);

	add_opcode(&buffer, SPLAYERDATA);
	add_buffer(&buffer, &index, SIZE16);
	add_string(&buffer, player(index)->charname);
	add_buffer(&buffer, &player(index)->sprite, SIZE16);
	add_buffer(&buffer, &player(index)->map, SIZE16);
	add_buffer(&buffer, &player(index)->x, SIZE8);
	add_buffer(&buffer, &player(index)->y, SIZE8);
	add_buffer(&buffer, &player(index)->dir, SIZE8);
	add_buffer(&buffer, &player(index)->group, SIZE8);
	add_buffer(&buffer, &player(index)->pk, SIZE8);

	send_data_to_map(&buffer, player(index)->map);
}

void send_leave_map(int16 index, uint32 mapnum)
{
	buffer_t buffer;

	clear_buffer(&buffer);
	add_opcode(&buffer,SLEFT);
	add_buffer(&buffer, &index, SIZE16);

	send_data_to_map_but(&buffer,mapnum,index);
}

void send_player_data(int16 index)
{
	buffer_t buffer;

	clear_buffer(&buffer);

	add_opcode(&buffer, SPLAYERDATA);
	add_buffer(&buffer, &index, SIZE16);
	add_string(&buffer, player(index)->charname);
	add_buffer(&buffer, &player(index)->sprite, SIZE16);
	add_buffer(&buffer, &player(index)->map, SIZE16);
	add_buffer(&buffer, &player(index)->x, SIZE8);
	add_buffer(&buffer, &player(index)->y, SIZE8);
	add_buffer(&buffer, &player(index)->dir, SIZE8);
	add_buffer(&buffer, &player(index)->group, SIZE8);
	add_buffer(&buffer, &player(index)->pk, SIZE8);

	send_data_to_map(&buffer,player(index)->map);
}

void send_map(int16 index, uint32 mapnum)
{
	send_data(map_cache(mapnum), index);
}

void send_map_items_to(int16 index, uint32 mapnum)
{
	uint32 i = 1;
	buffer_t buffer;

	clear_buffer(&buffer);

	add_opcode(&buffer, SMAPITEMDATA);
	for(i = 1; i < MAX_MAP_ITEMS; i++){
		add_buffer(&buffer, &map(mapnum)->items[i].num, SIZE16);
		add_buffer(&buffer, &map(mapnum)->items[i].value, SIZE32);
		add_buffer(&buffer, &map(mapnum)->items[i].dur, SIZE16);
		add_buffer(&buffer, &map(mapnum)->items[i].x, SIZE8);
		add_buffer(&buffer, &map(mapnum)->items[i].y, SIZE8);
	}

	send_data(&buffer,index);
}

void send_map_items_to_all(uint32 mapnum)
{
	uint32 i = 1;
	buffer_t buffer;

	clear_buffer(&buffer);

	add_opcode(&buffer, SMAPITEMDATA);
	for(i = 1; i < MAX_MAP_ITEMS; i++){
		add_buffer(&buffer, &map(mapnum)->items[i].num, SIZE16);
		add_buffer(&buffer, &map(mapnum)->items[i].value, SIZE32);
		add_buffer(&buffer, &map(mapnum)->items[i].dur, SIZE16);
		add_buffer(&buffer, &map(mapnum)->items[i].x, SIZE8);
		add_buffer(&buffer, &map(mapnum)->items[i].y, SIZE8);
	}

	send_data_to_map(&buffer,mapnum);
}

void send_map_npcs_to(int16 index, uint32 mapnum)
{
	uint32 i = 1;
	buffer_t buffer;

	clear_buffer(&buffer);

	add_opcode(&buffer, SMAPNPCDATA);

	for( i = 0; i < MAX_MAP_NPCS; i++){
		add_buffer(&buffer, &map(mapnum)->npc[i].num, SIZE16);
		add_buffer(&buffer, &map(mapnum)->npc[i].x, SIZE8);
		add_buffer(&buffer, &map(mapnum)->npc[i].y, SIZE8);
		add_buffer(&buffer, &map(mapnum)->npc[i].dir, SIZE8);
	}

	send_data(&buffer,index);
}

void send_map_npcs_to_map(uint32 mapnum)
{
	uint32 i = 1;
	buffer_t buffer;

	clear_buffer(&buffer);

	add_opcode(&buffer, SMAPNPCDATA);

	for( i = 0; i < MAX_MAP_NPCS; i++){
		add_buffer(&buffer, &map(mapnum)->npc[i].num, SIZE16);
		add_buffer(&buffer, &map(mapnum)->npc[i].x, SIZE8);
		add_buffer(&buffer, &map(mapnum)->npc[i].y, SIZE8);
		add_buffer(&buffer, &map(mapnum)->npc[i].dir, SIZE8);
	}

	send_data_to_map(&buffer,mapnum);
}

void send_items(int16 index)
{
	uint32 i = 1;

	for(i = 1 ; i < MAX_ITEMS; i++){
		if(strlen(item(i)->name) > 0)
			send_update_item_to(index, i);
	}
}

void send_npcs(int16 index)
{
	uint32 i = 1;

	for(i = 1 ; i < MAX_NPCS; i++){
		if(strlen(npc(i)->name) > 0)
			send_update_npc_to(index, i);
	}
}

void send_inventory(int16 index)
{
	uint32 i = 1;
	buffer_t buffer;

	clear_buffer(&buffer);

	add_opcode(&buffer, SPLAYERINV);

	for( i = 1; i < MAX_INV; i++){
		add_buffer(&buffer, &player(index)->inv[i].id, SIZE16);
		add_buffer(&buffer, &player(index)->inv[i].value, SIZE32);
		add_buffer(&buffer, &player(index)->inv[i].dur, SIZE16);
	}

	send_data(&buffer,index);
}

void send_inventory_update(int16 index, uint8 invslot)
{
	buffer_t buffer;

	clear_buffer(&buffer);

	add_opcode(&buffer, SPLAYERINVUPDATE);

	add_buffer(&buffer, &invslot, SIZE8);
	add_buffer(&buffer, &player(index)->inv[invslot].id, SIZE16);
	add_buffer(&buffer, &player(index)->inv[invslot].value, SIZE32);
	add_buffer(&buffer, &player(index)->inv[invslot].dur, SIZE16);

	send_data(&buffer,index);
}

void send_worn_equipment(int16 index)
{
	uint32 i = 0;
	buffer_t buffer;

	clear_buffer(&buffer);

	add_opcode(&buffer, SPLAYERWORNEQ);

	for(i = 0; i < EQUIPMENT_COUNT; i++){
		add_buffer(&buffer, &player(index)->equipment[i], SIZE16);
	}

	send_data(&buffer,index);
}

void send_vital(int16 index, uint8 vital)
{
	buffer_t buffer;
	uint16 max = get_player_max_vital(index, vital);
	clear_buffer(&buffer);

	switch(vital){
	case VITAL_HP: add_opcode(&buffer, SPLAYERHP); break;
	case VITAL_MP: add_opcode(&buffer, SPLAYERMP); break;
	case VITAL_SP: add_opcode(&buffer, SPLAYERSP); break;
	}

	add_buffer(&buffer, &max, SIZE16);
	add_buffer(&buffer, &player(index)->vitals[vital], SIZE16);

	send_data(&buffer,index);
}

void send_stats(int16 index)
{
	uint32 i = 0;
	buffer_t buffer;

	clear_buffer(&buffer);

	add_opcode(&buffer, SPLAYERSTATS);

	for(i = 0; i < STAT_COUNT; i++){
		add_buffer(&buffer, &player(index)->stat[i], SIZE8);
	}

	send_data(&buffer,index);
}

void send_welcome(int16 index)
{
	char *string = NULL;

	// ' Send them welcome, Cyan
	player_msg(index, "Type /help for help on commands.  Use arrow keys to move, hold down shift to run, and use ctrl to attack.", 1);

	// ' Send them MOTD, BrightCyan
	if(strlen(MOTD) > 0){
		string = comb_2str("MOTD: ",MOTD);
		player_msg(index, string, 1);
	}

	// TODO: make better who is on-line for Administrators only.
	//  Call SendWhosOnline(Index)
}

void send_left_game(int16 index)
{
	buffer_t buffer;

	clear_buffer(&buffer);

	add_opcode(&buffer, SLEFT);

	add_buffer(&buffer, &index, SIZE16);

	send_data_to_all_but(&buffer, index);
}

void send_player_xy(int16 index)
{
	buffer_t buffer;

	clear_buffer(&buffer);

	add_opcode(&buffer, SPLAYERXY);

	add_buffer(&buffer, &player(index)->x, SIZE8);
	add_buffer(&buffer, &player(index)->y, SIZE8);

	send_data(&buffer,index);
}

void send_update_item_to_all(uint16 itemnum)
{
	buffer_t buffer;

	clear_buffer(&buffer);

	add_opcode(&buffer, SUPDATEITEM);
	add_buffer(&buffer, &itemnum, SIZE16);
	add_string(&buffer,item(itemnum)->name);
	add_buffer(&buffer, &item(itemnum)->pic, SIZE16);
	add_buffer(&buffer, &item(itemnum)->type, SIZE8);
	add_buffer(&buffer, &item(itemnum)->data1, SIZE16);
	add_buffer(&buffer, &item(itemnum)->data2, SIZE16);
	add_buffer(&buffer, &item(itemnum)->data3, SIZE16);

	send_data_to_all(&buffer);
}

void send_update_item_to(int16 index, uint16 itemnum)
{
	buffer_t buffer;

	clear_buffer(&buffer);

	add_opcode(&buffer, SUPDATEITEM);
	add_buffer(&buffer, &itemnum, SIZE16);
	add_string(&buffer,item(itemnum)->name);
	add_buffer(&buffer, &item(itemnum)->pic, SIZE16);
	add_buffer(&buffer, &item(itemnum)->type, SIZE8);
	add_buffer(&buffer, &item(itemnum)->data1, SIZE16);
	add_buffer(&buffer, &item(itemnum)->data2, SIZE16);
	add_buffer(&buffer, &item(itemnum)->data3, SIZE16);

	send_data(&buffer,index);
}

void send_edit_item_to(int16 index, uint16 itemnum)
{
	buffer_t buffer;

	clear_buffer(&buffer);

	add_opcode(&buffer, SEDITITEM);
	add_buffer(&buffer, &itemnum, SIZE16);
	add_string(&buffer,item(itemnum)->name);
	add_buffer(&buffer, &item(itemnum)->type, SIZE8);
	add_buffer(&buffer, &item(itemnum)->pic, SIZE16);
	add_buffer(&buffer, &item(itemnum)->data1, SIZE16);
	add_buffer(&buffer, &item(itemnum)->data2, SIZE16);
	add_buffer(&buffer, &item(itemnum)->data3, SIZE16);

	send_data(&buffer,index);
}

void send_update_npc_to_all(uint16 npcnum)
{
	buffer_t buffer;

	clear_buffer(&buffer);

	add_opcode(&buffer, SUPDATENPC);
	add_buffer(&buffer, &npcnum, SIZE16);
	add_string(&buffer,npc(npcnum)->name);
	add_buffer(&buffer, &npc(npcnum)->sprite, SIZE16);

	send_data_to_all(&buffer);
}

void send_update_npc_to(int16 index, uint16 npcnum)
{
	buffer_t buffer;

	clear_buffer(&buffer);

	add_opcode(&buffer, SUPDATENPC);
	add_buffer(&buffer, &npcnum, SIZE16);
	add_string(&buffer,npc(npcnum)->name);
	add_buffer(&buffer, &npc(npcnum)->sprite, SIZE16);

	send_data(&buffer,index);
}

void send_edit_npc_to(int16 index, uint16 npcnum)
{
	buffer_t buffer;

	clear_buffer(&buffer);

	add_opcode(&buffer, SEDITNPC);
	add_buffer(&buffer, &npcnum, SIZE16);
	add_string(&buffer,npc(npcnum)->name);
	add_string(&buffer,npc(npcnum)->say);
	add_buffer(&buffer, &npc(npcnum)->sprite, SIZE16);
	add_buffer(&buffer, &npc(npcnum)->behavior, SIZE16);
	add_buffer(&buffer, &npc(npcnum)->range, SIZE16);
	add_buffer(&buffer, &npc(npcnum)->dropitem, SIZE16);
	add_buffer(&buffer, &npc(npcnum)->dropchance, SIZE16);
	add_buffer(&buffer, &npc(npcnum)->dropitemvalue, SIZE16);
	add_buffer(&buffer, &npc(npcnum)->spawnsecs, SIZE16);
	add_buffer(&buffer, npc(npcnum)->stat, STAT_COUNT * sizeof(uint8));

	send_data(&buffer,index);
}

void send_shops(int16 index)
{
	uint32 i = 1;

	for(i = 1; i < MAX_SHOPS; i++){
		if(strlen(shop(i)->name) > 0)
			send_update_shop_to(index, i);
	}
}

void send_update_shop_to_all(uint16 shopnum)
{
	buffer_t buffer;

	clear_buffer(&buffer);

	add_opcode(&buffer, SUPDATESHOP);
	add_buffer(&buffer, &shopnum, SIZE16);
	add_string(&buffer,shop(shopnum)->name);

	send_data_to_all(&buffer);
}

void send_update_shop_to(int16 index, uint16 shopnum)
{
	buffer_t buffer;

	clear_buffer(&buffer);

	add_opcode(&buffer, SUPDATESHOP);
	add_buffer(&buffer, &shopnum, SIZE16);
	add_string(&buffer,shop(shopnum)->name);

	send_data(&buffer,index);
}

void send_edit_shop_to(int16 index, uint16 shopnum)
{
	buffer_t buffer;

	clear_buffer(&buffer);

	add_opcode(&buffer, SEDITSHOP);
	add_buffer(&buffer, &shopnum, SIZE16);
	add_string(&buffer,shop(shopnum)->name);
	add_string(&buffer,shop(shopnum)->joinsay);
	add_string(&buffer,shop(shopnum)->leavesay);
	add_buffer(&buffer, &shop(shopnum)->fixesitems, SIZE8);
	add_buffer(&buffer, &shop(shopnum)->tradeitem, MAX_TRADES * sizeof(trade_item_t));

	send_data(&buffer,index);
}

void send_spells(int16 index)
{
	uint32 i = 1;

	for(i = 1; i < MAX_SPELLS; i++){
		if(strlen(spell(i)->name) > 0)
			send_update_spell_to(index, i);
	}
}

void send_update_spell_to_all(uint16 spellnum)
{
	buffer_t buffer;

	clear_buffer(&buffer);

	add_opcode(&buffer, SUPDATESPELL);
	add_buffer(&buffer, &spellnum, SIZE16);
	add_string(&buffer,spell(spellnum)->name);
	add_buffer(&buffer, &spell(spellnum)->mpreq, SIZE16);
	add_buffer(&buffer, &spell(spellnum)->pic, SIZE16);

	send_data_to_all(&buffer);
}

void send_update_spell_to(int16 index, uint16 spellnum)
{
	buffer_t buffer;

	clear_buffer(&buffer);

	add_opcode(&buffer, SUPDATESPELL);
	add_buffer(&buffer, &spellnum, SIZE16);
	add_string(&buffer,spell(spellnum)->name);
	add_buffer(&buffer, &spell(spellnum)->mpreq, SIZE16);
	add_buffer(&buffer, &spell(spellnum)->pic, SIZE16);

	send_data(&buffer,index);
}

void send_edit_spell_to(int16 index, uint16 spellnum)
{
	buffer_t buffer;

	clear_buffer(&buffer);

	add_opcode(&buffer, SEDITSPELL);
	add_buffer(&buffer, &spellnum, SIZE16);
	add_string(&buffer,spell(spellnum)->name);
	add_buffer(&buffer, &spell(spellnum)->pic, SIZE16);
	add_buffer(&buffer, &spell(spellnum)->mpreq, SIZE16);
	add_buffer(&buffer, &spell(spellnum)->jobreq, SIZE8);
	add_buffer(&buffer, &spell(spellnum)->levelreq, SIZE8);
	add_buffer(&buffer, &spell(spellnum)->type, SIZE8);
	add_buffer(&buffer, &spell(spellnum)->data1, SIZE16);
	add_buffer(&buffer, &spell(spellnum)->data2, SIZE16);
	add_buffer(&buffer, &spell(spellnum)->data3, SIZE16);

	send_data(&buffer,index);
}

void send_trade(int16 index, uint16 shopnum)
{
	uint32 i = 0;
	buffer_t buffer;

	clear_buffer(&buffer);

	add_opcode(&buffer, STRADE);

	add_buffer(&buffer, &shopnum, SIZE16);

	add_buffer(&buffer, &shop(shopnum)->fixesitems, SIZE16);
	add_buffer(&buffer, &shop(shopnum)->tradeitem, MAX_TRADES * sizeof(trade_item_t));

	send_data(&buffer,index);
}

void send_players_spells(int16 index)
{
	uint8 i = 1;
	buffer_t buffer;

	clear_buffer(&buffer);

	add_opcode(&buffer, SSPELLS);
	for(i = 1; i > MAX_PLAYER_SPELLS; i++){
		add_buffer(&buffer, &player(index)->spells[i], SIZE16);
	}

	send_data(&buffer,index);
}