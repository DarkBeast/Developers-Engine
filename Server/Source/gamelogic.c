/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/
#include "gamelogic.h"
#include "general.h"
#include "socket.h"
#include "path.h"
#include "function.h"
#include "servertcp.h"
#include <stdlib.h>
#include <stdio.h>

uint16 get_player_max_vital(int16 index, uint8 vital)
{
	uint16 vitalmax;
	switch(vital){
	case VITAL_HP:
		vitalmax = (player(index)->level + (player(index)->stat[STAT_STRENGTH] / 2)); break;
	case VITAL_MP:
		vitalmax = (player(index)->level + (player(index)->stat[STAT_MAGIC] / 2)); break;
	case VITAL_SP:
		vitalmax = (player(index)->level + (player(index)->stat[STAT_SPEED] / 2)); break;
	}
	return vitalmax;
}

void attack_npc(int16 attacker, uint8 mapnpcnum, uint32 damage)
{
	char *string = NULL;
	uint32 exp;
	uint32 i;
	uint32 n;
	uint32 mapnum;
	uint32 npcnum;
	buffer_t buffer;

	if(temp_player(attacker)->loggedin == FALSE|| mapnpcnum >= MAX_MAP_NPCS)
		return;

	mapnum = player(attacker)->map;
	npcnum = map(mapnum)->npc[mapnpcnum].num;

	clear_buffer(&buffer);
	add_opcode(&buffer,SATTACK);
	add_buffer(&buffer,&attacker, SIZE16);

	send_data_to_map_but(&buffer, mapnum, attacker);

	if(player(attacker)->equipment[EQUIPMENT_WEAPON] > 0)
		n = player(attacker)->inv[player(attacker)->equipment[EQUIPMENT_WEAPON]].id;

	if(damage >= map(mapnum)->npc[mapnpcnum].vitals[VITAL_HP]){
		if(n){ //BrightRed
			string = comb_5str("You hit a ",npc(npcnum)->name, " for ", int_to_string(damage), " hit points, killing it.");
			player_msg(attacker, string, 1);
		}
		else{
			string = comb_7str("You hit a ",npc(npcnum)->name," with a ",item(n)->name, " for ", int_to_string(damage), " hit points, killing it.");
			player_msg(attacker, string, 1);
		}

		exp = npc(npcnum)->stat[STAT_STRENGTH] * npc(npcnum)->stat[STAT_DEFENSE] * 2;

		n = (rand() % (npc(npcnum)->dropchance * 2)) + 1;
		if(n == 1)
			spawn_item(npc(npcnum)->dropitem, npc(npcnum)->dropitemvalue, mapnum, map(mapnum)->npc[mapnpcnum].x, map(mapnum)->npc[mapnpcnum].y);

		map(mapnum)->npc[mapnpcnum].spawnwait = gettickcount();
		map(mapnum)->npc[mapnpcnum].vitals[VITAL_HP] = 0;

		clear_buffer(&buffer);
		add_opcode(&buffer, SNPCDEAD);
		add_buffer(&buffer, &mapnpcnum, SIZE8);
		send_data_to_map(&buffer,mapnum);

		check_player_level_up(attacker);

		if(temp_player(attacker)->targettype == TARGET_NPC){
			if(temp_player(attacker)->target == mapnpcnum){
				temp_player(attacker)->target = 0;
				temp_player(attacker)->targettype = TARGET_NONE;
			}
		}
	}
	else{
		map(mapnum)->npc[mapnpcnum].vitals[VITAL_HP] -= damage;

		if(n){ //BrightRed
			string = comb_5str("You hit a ",npc(npcnum)->name, " for ", int_to_string(damage), " hit points.");
			player_msg(attacker, string, 1);
		}
		else{
			string = comb_7str("You hit a ",npc(npcnum)->name," with a ",item(n)->name, " for ", int_to_string(damage), " hit points.");
			player_msg(attacker, string, 1);
		}

		if(map(mapnum)->npc[mapnpcnum].target == 0){
			if(strlen(npc(npcnum)->say) > 0){ //grey
				string = comb_5str("A ",npc(npcnum)->name," says, '",npc(npcnum)->say, "' to you.");
				player_msg(attacker,string,1);
			}
		}

		map(mapnum)->npc[mapnpcnum].target = attacker;

		if(npc(npcnum)->behavior == BEHAVIOR_GUARD){
			for(i = 0; i < MAX_MAP_NPCS; i++){
				if(map(mapnum)->npc[i].num == map(mapnum)->npc[mapnpcnum].num)
					map(mapnum)->npc[i].target = attacker;
			}
		}
	}

	damage_equipment(attacker, player(attacker)->equipment[EQUIPMENT_WEAPON]);

	temp_player(attacker)->attacktimer = gettickcount();
}

void attack_player(int16 attacker, int16 victim, uint32 damage)
{
	uint32 exp;
	uint32 n = 0;
	uint32 i = 0;
	buffer_t buffer;
	char *string = NULL;

	if(temp_player(attacker)->loggedin == FALSE || temp_player(victim)->loggedin == FALSE)
		return;

	if(player(attacker)->equipment[EQUIPMENT_WEAPON] > 0)
		n = player(attacker)->inv[player(attacker)->equipment[EQUIPMENT_WEAPON]].id;

	clear_buffer(&buffer);
	add_opcode(&buffer,SATTACK);
	add_buffer(&buffer,&attacker, SIZE16);

	send_data_to_map_but(&buffer, player(attacker)->map, attacker);

	damage_equipment(victim, EQUIPMENT_ARMOR);
	damage_equipment(victim, EQUIPMENT_HELMET);

	if(damage >= player(victim)->vitals[VITAL_HP]){
		if(n){ //BrightRed
			string = comb_5str("You hit ",player(victim)->charname, " for ", int_to_string(damage), " hit points.");
			player_msg(attacker, string, 1);
			string = comb_4str(player(attacker)->charname, " hit you for ", int_to_string(damage), " hit points.");
			player_msg(victim, string, 1);
		}
		else{
			string = comb_7str("You hit a ",player(victim)->charname," with a ",item(n)->name, " for ", int_to_string(damage), " hit points.");
			player_msg(attacker, string, 1);
			string = comb_7str(player(attacker)->charname," hit you with a ",item(n)->name, " for ", int_to_string(damage), " hit points.","");
			player_msg(victim, string, 1);
		}

		//bright red
		string = comb_3str(player(victim)->charname," has been killed by ",player(attacker)->charname);
		global_msg(string,1);

		exp = (player(victim)->exp / 10);

		if(exp == 0){//bright red then bright blue.
			player_msg(victim, "You lost no experience points.", 1);
			player_msg(attacker, "You received no experience points from that weak insignificant player.", 1);
		}
		else{//bright red and bright blue.
			player(victim)->exp -= exp;
			string = comb_3str("You lost ",int_to_string(exp)," experience points.");
			player_msg(victim, string, 1);
			player(attacker)->exp += exp;
			string = comb_5str("You got ",int_to_string(exp)," experience points for killing ",player(victim)->charname,".");
			player_msg(attacker, string, 1);
		}

		check_player_level_up(attacker);

		if(temp_player(attacker)->targettype == TARGET_PLAYER){
			if(temp_player(attacker)->target == victim){
				temp_player(attacker)->target = 0;
				temp_player(attacker)->targettype = TARGET_NONE;
			}
		}

		if(player(victim)->pk == 0){
			if(player(attacker)->pk == 0){
				player(attacker)->pk = 1;

				send_player_data(attacker);
				string = comb_2str(player(attacker)->charname," has been deemed a Player Killer!!!");
				global_msg(string, 1); //bright red
			}
		}
		else{
			string = comb_2str(player(victim)->charname," has paid the price for being a Player Killer!!!");
			global_msg(string, 1); //bright red
		}

		on_death(victim);
	}
	else{
		player(victim)->vitals[VITAL_HP] -= damage;
		send_vital(victim, VITAL_HP);

		if(n){
			string = comb_5str("You hit ",player(victim)->charname, " for ", int_to_string(damage), " hit points.");
			player_msg(attacker, string, 1);//white
			string = comb_4str(player(attacker)->charname, " hit you for ", int_to_string(damage), " hit points.");
			player_msg(victim, string, 1);//bright red
		}
		else{
			string = comb_7str("You hit a ",player(victim)->charname," with a ",item(n)->name, " for ", int_to_string(damage), " hit points.");
			player_msg(attacker, string, 1);
			string = comb_7str(player(attacker)->charname," hit you with a ",item(n)->name, " for ", int_to_string(damage), " hit points.","");
			player_msg(victim, string, 1);
		}
	}

	damage_equipment(attacker, EQUIPMENT_WEAPON);

	temp_player(attacker)->attacktimer = gettickcount();
}

void attack_npc_with_magic(int16 attacker, uint8 mapnpcnum, uint32 damage, uint16 spellnum)
{
	char *string = NULL;
	uint32 exp;
	uint32 i;
	uint32 n;
	uint32 mapnum;
	uint32 npcnum;
	buffer_t buffer;

	if(temp_player(attacker)->loggedin == FALSE|| mapnpcnum >= MAX_MAP_NPCS)
		return;

	mapnum = player(attacker)->map;
	npcnum = map(mapnum)->npc[mapnpcnum].num;

	clear_buffer(&buffer);
	add_opcode(&buffer,SATTACK);
	add_buffer(&buffer,&attacker, SIZE16);

	send_data_to_map_but(&buffer, mapnum, attacker);

	if(damage >= map(mapnum)->npc[mapnpcnum].vitals[VITAL_HP]){
		string = comb_7str("You casted ",spell(spellnum)->name," at ",npc(npcnum)->name , " dealing ", int_to_string(damage), " hit points, killing them.");
		player_msg(attacker, string, 1);

		exp = npc(npcnum)->stat[STAT_STRENGTH] * npc(npcnum)->stat[STAT_DEFENSE] * 2;

		n = (rand() % (npc(npcnum)->dropchance * 2)) + 1;
		if(n == 1)
			spawn_item(npc(npcnum)->dropitem, npc(npcnum)->dropitemvalue, mapnum, map(mapnum)->npc[mapnpcnum].x, map(mapnum)->npc[mapnpcnum].y);

		map(mapnum)->npc[mapnpcnum].spawnwait = gettickcount();
		map(mapnum)->npc[mapnpcnum].vitals[VITAL_HP] = 0;

		clear_buffer(&buffer);
		add_opcode(&buffer, SNPCDEAD);
		add_buffer(&buffer, &mapnpcnum, SIZE8);
		send_data_to_map(&buffer,mapnum);

		check_player_level_up(attacker);

		if(temp_player(attacker)->targettype == TARGET_NPC){
			if(temp_player(attacker)->target == mapnpcnum){
				temp_player(attacker)->target = 0;
				temp_player(attacker)->targettype = TARGET_NONE;
			}
		}
	}
	else{
		map(mapnum)->npc[mapnpcnum].vitals[VITAL_HP] -= damage;

		string = comb_7str("You casted ",spell(spellnum)->name," at ",npc(npcnum)->name , " dealing ", int_to_string(damage), " hit points");
		player_msg(attacker, string, 1);

		if(map(mapnum)->npc[mapnpcnum].target == 0){
			if(strlen(npc(npcnum)->say) > 0){ //grey
				string = comb_5str("A ",npc(npcnum)->name," says, '",npc(npcnum)->say, "' to you.");
				player_msg(attacker,string,1);
			}
		}

		map(mapnum)->npc[mapnpcnum].target = attacker;

		if(npc(npcnum)->behavior == BEHAVIOR_GUARD){
			for(i = 0; i < MAX_MAP_NPCS; i++){
				if(map(mapnum)->npc[i].num == map(mapnum)->npc[mapnpcnum].num)
					map(mapnum)->npc[i].target = attacker;
			}
		}
	}
}

void attack_player_with_magic(int16 attacker, int16 victim, uint32 damage, uint16 spellnum)
{
	uint32 exp;
	uint32 n = 0;
	uint32 i = 0;
	buffer_t buffer;
	char *string = NULL;

	if(temp_player(attacker)->loggedin == FALSE || temp_player(victim)->loggedin == FALSE)
		return;

	clear_buffer(&buffer);
	add_opcode(&buffer,SATTACK);
	add_buffer(&buffer,&attacker, SIZE16);

	send_data_to_map_but(&buffer, player(attacker)->map, attacker);

	damage_equipment(victim, EQUIPMENT_ARMOR);
	damage_equipment(victim, EQUIPMENT_HELMET);

	if(damage >= player(victim)->vitals[VITAL_HP]){
		string = comb_7str("You casted ",spell(spellnum)->name," at ",player(victim)->charname , " dealing ", int_to_string(damage), " hit points, killing them.");
		player_msg(attacker, string, 1);
		string = comb_7str(player(attacker)->charname," casted ",spell(spellnum)->name, " dealing ", int_to_string(damage), " hit points.","");
		player_msg(victim, string, 1);

		//bright red
		string = comb_3str(player(victim)->charname," has been killed by ",player(attacker)->charname);
		global_msg(string,1);

		exp = (player(victim)->exp / 10);

		if(exp == 0){//bright red then bright blue.
			player_msg(victim, "You lost no experience points.", 1);
			player_msg(attacker, "You received no experience points from that weak insignificant player.", 1);
		}
		else{//bright red and bright blue.
			player(victim)->exp -= exp;
			string = comb_3str("You lost ",int_to_string(exp)," experience points.");
			player_msg(victim, string, 1);
			player(attacker)->exp += exp;
			string = comb_5str("You got ",int_to_string(exp)," experience points for killing ",player(victim)->charname,".");
			player_msg(attacker, string, 1);
		}

		check_player_level_up(attacker);

		if(temp_player(attacker)->targettype == TARGET_PLAYER){
			if(temp_player(attacker)->target == victim){
				temp_player(attacker)->target = 0;
				temp_player(attacker)->targettype = TARGET_NONE;
			}
		}

		if(player(victim)->pk == 0){
			if(player(attacker)->pk == 0){
				player(attacker)->pk = 1;

				send_player_data(attacker);
				string = comb_2str(player(attacker)->charname," has been deemed a Player Killer!!!");
				global_msg(string, 1); //bright red
			}
		}
		else{
			string = comb_2str(player(victim)->charname," has paid the price for being a Player Killer!!!");
			global_msg(string, 1); //bright red
		}

		on_death(victim);
	}
	else{
		player(victim)->vitals[VITAL_HP] -= damage;
		send_vital(victim, VITAL_HP);

		string = comb_7str("You casted ",spell(spellnum)->name," at ",player(victim)->charname , " dealing ", int_to_string(damage), " hit points, killing them.");
		player_msg(attacker, string, 1);
		string = comb_7str(player(attacker)->charname," casted ",spell(spellnum)->name, " dealing ", int_to_string(damage), " hit points.","");
		player_msg(victim, string, 1);
	}
}

uint32 find_open_map_item_slot(uint32 mapnum)
{
	uint32 i = 1;

	if(mapnum >= MAX_MAPS)
		return 0;

	for(i = 1; i < MAX_MAP_ITEMS; i++){
		if(map(mapnum)->items[i].num == 0)
			return i;
	}

	return 0;
}

void join_game(int16 index)
{
	uint8 i = 0;
	buffer_t buffer;
	char *string = NULL;
	temp_player(index)->loggedin = TRUE;

	total_players_online_plus();
	update_players_online();

	string = comb_3str(player(index)->charname," has joined ",GAME_NAME);

	if(player(index)->group <= GROUP_MONITOR){
		global_msg(string,1); //dark grey
	}
	else{
		global_msg(string,1); //white
	}

	clear_buffer(&buffer);
	add_opcode(&buffer,SLOGINOK);
	add_buffer(&buffer,&index,SIZE16);
	send_data(&buffer,index);

	check_equipped_items(index);
	send_items(index);
	send_npcs(index);
	send_shops(index);
	send_spells(index);
	send_inventory(index);
	send_worn_equipment(index);

	for(i = 0; i < VITAL_COUNT; i++){
		send_vital(index, i);
	}

	send_stats(index);
	player_warp(index, player(index)->map, player(index)->x, player(index)->y);
	send_welcome(index);

	clear_buffer(&buffer);
	add_opcode(&buffer,SINGAME);
	send_data(&buffer,index);
}

void left_game(int16 index)
{
	uint32 n = 0;
	char *string = NULL;

	if(temp_player(index)->loggedin){
		temp_player(index)->loggedin = FALSE;

		if(players_on_map(player(index)->map) > 0){
			players_on_map_minus(player(index)->map);
		}

		if(map(player(index)->map)->bootmap > -1){
			player(index)->x = map(player(index)->map)->bootx;
			player(index)->y = map(player(index)->map)->booty;
			player(index)->map = map(player(index)->map)->bootmap;
		}

		write_player(PLAYER_PATH,index);

		string = comb_3str(player(index)->charname," has left ",GAME_NAME);

		if(player(index)->group <= GROUP_MONITOR){
			global_msg(string,1); //dark grey
		}
		else{
			global_msg(string,1); //white
		}

		printf("%s has disconnected from %s \n", player(index)->charname, GAME_NAME);

		send_left_game(index);

		total_players_online_minus();
		update_players_online();
	}
	clear_player(index);
}

void spawn_item(uint16 itemnum, uint32 itemval, uint32 mapnum, uint8 x, uint8 y)
{
	uint8 i = 0;

	if(itemnum == 0 || itemnum >= MAX_ITEMS || mapnum >= MAX_MAPS)
		return;

	i = find_open_map_item_slot(mapnum);

	spawn_item_slot(i, itemnum, itemval, item(itemnum)->data1, mapnum, x, y);
}

void spawn_item_slot(uint8 mapitemslot, uint16 itemnum, uint32 itemval, uint32 itemdur, uint32 mapnum, uint8 x, uint8 y)
{
	buffer_t buffer;

	if( mapitemslot >= MAX_MAP_ITEMS || itemnum == 0 || itemnum >= MAX_ITEMS || mapnum >= MAX_MAPS)
		return;

	map(mapnum)->items[mapitemslot].num = itemnum;
	map(mapnum)->items[mapitemslot].value = itemval;

	if(item(itemnum)->type >= ITEM_TYPE_WEAPON && item(itemnum)->type <= ITEM_TYPE_SHIELD){
		map(mapnum)->items[mapitemslot].dur = itemdur;
	}
	else{
		map(mapnum)->items[mapitemslot].dur = 0;
	}

	map(mapnum)->items[mapitemslot].x = x;
	map(mapnum)->items[mapitemslot].y = y;

	clear_buffer(&buffer);

	add_opcode(&buffer,SSPAWNITEM);
	add_buffer(&buffer, &mapitemslot, SIZE8);
	add_buffer(&buffer, &itemnum, SIZE16);
	add_buffer(&buffer, &itemval, SIZE32);
	add_buffer(&buffer, &map(mapnum)->items[mapitemslot].dur, SIZE32);
	add_buffer(&buffer, &x, SIZE8);
	add_buffer(&buffer, &y, SIZE8);

	send_data_to_map(&buffer,mapnum);
}

void spawn_all_maps_items(void)
{
	uint32 i = 0;

	for(i = 1; i < MAX_ITEMS; i++){
		spawn_map_items(i);
	}
}

void spawn_map_items(uint32 mapnum)
{
	uint8 x;
	uint8 y;

	if(mapnum >= MAX_MAPS)
		return;

	for( x = 0; x < MAX_MAPX; x++){
		for( y = 0; y < MAX_MAPY; y++){
			if(map(mapnum)->tile[GETXY(x,y)].type == TILE_TYPE_ITEM){
				if(item(map(mapnum)->tile[GETXY(x,y)].data1)->type == ITEM_TYPE_CURRENCY && map(mapnum)->tile[GETXY(x,y)].data2 == 0){
					spawn_item(map(mapnum)->tile[GETXY(x,y)].data1, 1, mapnum, x, y);
				}
				else{
					spawn_item(map(mapnum)->tile[GETXY(x,y)].data1, map(mapnum)->tile[GETXY(x,y)].data2, mapnum, x, y);
				}
			}
		}
	}
}

void spawn_npc(uint8 mapnpcnum, uint32 mapnum)
{
	uint32 npcnum;
	uint32 i = 0;
	uint8 x;
	uint8 y;
	sbool spawned = FALSE;
	buffer_t buffer;

	if(mapnpcnum >= MAX_MAP_NPCS || mapnum >= MAX_MAPS)
		return;

	if(map(mapnum)->npc[mapnpcnum].num > 0){
		map(mapnum)->npc[mapnpcnum].target = 0;
		map(mapnum)->npc[mapnpcnum].vitals[VITAL_HP] = get_npc_max_vital(map(mapnum)->npc[mapnpcnum].num, VITAL_HP);
		map(mapnum)->npc[mapnpcnum].vitals[VITAL_MP] = get_npc_max_vital(map(mapnum)->npc[mapnpcnum].num, VITAL_MP);
		map(mapnum)->npc[mapnpcnum].vitals[VITAL_SP] = get_npc_max_vital(map(mapnum)->npc[mapnpcnum].num, VITAL_SP);
		map(mapnum)->npc[mapnpcnum].dir = (rand() % 4);

		for( i = 0; i < 100; i++){
			x = (rand() % (MAX_MAPX - 1));
			y = (rand() % (MAX_MAPY - 1));

			if(map(mapnum)->tile[GETXY(x,y)].type = TILE_TYPE_WALKABLE){
				map(mapnum)->npc[mapnpcnum].x = x;
				map(mapnum)->npc[mapnpcnum].y = y;
				spawned = TRUE;
				break;
			}
		}

		if(!spawned){
			for(x = 0; x < MAX_MAPX; x++){
				for(y = 0; y < MAX_MAPY; y++){
					if(map(mapnum)->tile[GETXY(x,y)].type = TILE_TYPE_WALKABLE){
						map(mapnum)->npc[mapnpcnum].x = x;
						map(mapnum)->npc[mapnpcnum].y = y;
						spawned = TRUE;
						break;
					}
				}
			}

			if(spawned){
				clear_buffer(&buffer);
				add_opcode(&buffer, SSPAWNNPC);
				add_buffer(&buffer, &mapnpcnum, SIZE8);
				add_buffer(&buffer, &map(mapnum)->npc[mapnpcnum].num, SIZE16);
				add_buffer(&buffer, &map(mapnum)->npc[mapnpcnum].x, SIZE8);
				add_buffer(&buffer, &map(mapnum)->npc[mapnpcnum].y, SIZE8);
				add_buffer(&buffer, &map(mapnum)->npc[mapnpcnum].dir, SIZE8);

				send_data_to_map(&buffer, mapnum);
			}
		}
	}
}

void spawn_map_npcs(uint32 mapnum)
{
	uint8 i = 0;

	for( i = 0; i < MAX_MAP_NPCS; i++){
		spawn_npc(i, mapnum);
	}
}

void spawn_all_map_npcs(void)
{
	uint32 i = 0;

	for(i = 0; i < MAX_MAPS; i++){
		spawn_map_npcs(i);
	}
}

sbool can_attack_player(int16 attacker, int16 victim)
{
	char *string = NULL;

	if(gettickcount() < temp_player(attacker)->attacktimer + 1000)
		return FALSE;

	if(!temp_player(victim)->loggedin || !temp_player(attacker)->loggedin)
		return FALSE;

	if(player(victim)->map != player(attacker)->map)
		return FALSE;

	if(temp_player(victim)->gettingmap)
		return FALSE;

	switch(player(attacker)->dir){
	case DIR_UP:
		if(player(victim)->y + 1 != player(attacker)->y && player(victim)->x == player(attacker)->x)
			return FALSE;
		break;
	case DIR_DOWN:
		if(player(victim)->y - 1 != player(attacker)->y && player(victim)->x == player(attacker)->x)
			return FALSE;
		break;
	case DIR_LEFT:
		if(player(victim)->y == player(attacker)->y && player(victim)->x + 1 != player(attacker)->x)
			return FALSE;
		break;
	case DIR_RIGHT:
		if(player(victim)->y == player(attacker)->y && player(victim)->x - 1 != player(attacker)->x)
			return FALSE;
		break;
	default:
		return FALSE;
	}

	if(map(player(attacker)->map)->moral == MAP_TYPE_SAFE){
		if(player(victim)->pk){
			player_msg(attacker, "This is a safe zone!", 1);//bright red
			return FALSE;
		}
	}

	if(player(victim)->vitals[VITAL_HP] == 0)
		return FALSE;

	if(player(attacker)->group > GROUP_MONITOR){
		player_msg(attacker, "You cannot attack any player for thou art an admin!", 1); //bright blue
		return FALSE;
	}

	if(player(victim)->group > GROUP_MONITOR){
		string = comb_3str("You cannot attack ",player(victim)->charname,"!");
		player_msg(attacker, string, 1); //bright red
		return FALSE;
	}

	if(player(attacker)->level < 10 ){
		player_msg(attacker, "You are below level 10, you cannot attack another player yet!", 1); //bright red
		return FALSE;
	}

	if(player(victim)->level < 10 ){
		string = comb_2str(player(victim)->charname," is below level 10, you cannot attack this player yet!");
		player_msg(attacker,string, 1); //bright red
		return FALSE;
	}

	return TRUE;
}

sbool can_attack_npc(int16 attacker, uint8 mapnpcnum)
{
	char *string = NULL;
	uint8 npcx;
	uint8 npcy;

	if(!temp_player(attacker)->loggedin || mapnpcnum >= MAX_MAP_NPCS)
		return FALSE;

	if(map(player(attacker)->map)->npc[mapnpcnum].num == 0)
		return FALSE;

	if(map(player(attacker)->map)->npc[mapnpcnum].vitals[VITAL_HP] == 0)
		return FALSE;

	if(gettickcount() >temp_player(attacker)->attacktimer + 1000){
		switch(player(attacker)->dir){
		case DIR_UP:
			npcx = map(player(attacker)->map)->npc[mapnpcnum].x;
			npcy = map(player(attacker)->map)->npc[mapnpcnum].y + 1;
			break;
		case DIR_DOWN:
			npcx = map(player(attacker)->map)->npc[mapnpcnum].x;
			npcy = map(player(attacker)->map)->npc[mapnpcnum].y - 1;
			break;
		case DIR_LEFT:
			npcx =map(player(attacker)->map)->npc[mapnpcnum].x + 1;
			npcy = map(player(attacker)->map)->npc[mapnpcnum].y;
			break;
		case DIR_RIGHT:
			npcx = map(player(attacker)->map)->npc[mapnpcnum].x - 1;
			npcy = map(player(attacker)->map)->npc[mapnpcnum].y;
			break;
		default:
			return FALSE;
		}

		if(npcx == player(attacker)->x && npcy == player(attacker)->y){
			if(npc(map(player(attacker)->map)->npc[mapnpcnum].num)->behavior != BEHAVIOR_FRIENDLY && npc(map(player(attacker)->map)->npc[mapnpcnum].num)->behavior != BEHAVIOR_SHOPKEEPER)
				return TRUE;
		}
		else{
			string = comb_3str("You cannot attack a ",npc(map(player(attacker)->map)->npc[mapnpcnum].num)->name,"!");
			player_msg(attacker, string, 1); //bright blue
			return FALSE;
		}
	}
	return FALSE;
}

sbool can_npc_attack_player(uint8 mapnpcnum, int16 index)
{
	uint8 playerx;
	uint8 playery;

	if(!temp_player(index)->loggedin || mapnpcnum >= MAX_MAP_NPCS)
		return FALSE;

	if(map(player(index)->map)->npc[mapnpcnum].num == 0)
		return FALSE;

	if(map(player(index)->map)->npc[mapnpcnum].vitals[VITAL_HP] == 0)
		return FALSE;

	if(gettickcount() < map(player(index)->map)->npc[mapnpcnum].attacktimer + 1000)
		return FALSE;

	if(temp_player(index)->gettingmap)
		return FALSE;

	map(player(index)->map)->npc[mapnpcnum].attacktimer = gettickcount();

	switch(map(player(index)->map)->npc[mapnpcnum].dir){
	case DIR_UP:
		playerx = player(index)->x;
		playery = player(index)->y + 1;
		break;
	case DIR_DOWN:
		playerx = player(index)->x;
		playery = player(index)->y - 1;
		break;
	case DIR_LEFT:
		playerx = player(index)->x + 1;
		playery = player(index)->y;
		break;
	case DIR_RIGHT:
		playerx = player(index)->x - 1;
		playery = player(index)->y;
		break;
	default:
		return FALSE;
	}

	if(playerx ==map(player(index)->map)->npc[mapnpcnum].x && playery == map(player(index)->map)->npc[mapnpcnum].y)
		return TRUE;

	return FALSE;
}

void npc_attack_player(uint8 mapnpcnum, int16 victim, uint32 damage)
{
	char *string = NULL;
	uint32 exp;
	buffer_t buffer;

	if(mapnpcnum >= MAX_MAP_NPCS || !temp_player(victim)->loggedin)
		return;

	if(map(player(victim)->map)->npc[mapnpcnum].num == 0)
		return;

	clear_buffer(&buffer);
	add_opcode(&buffer, SNPCATTACK);
	add_buffer(&buffer, &mapnpcnum, SIZE8);
	send_data_to_map(&buffer, player(victim)->map);

	damage_equipment(victim, EQUIPMENT_ARMOR);
	damage_equipment(victim, EQUIPMENT_HELMET);

	if(damage >= player(victim)->vitals[VITAL_HP]){
		string = comb_5str("A ",npc(map(player(victim)->map)->npc[mapnpcnum].num)->name, " hit you for ", int_to_string(damage)," hit points.");
		player_msg(victim,string, 1); //bright red

		string = comb_3str(player(victim)->charname," has been killed by a ",npc(map(player(victim)->map)->npc[mapnpcnum].num)->name);
		global_msg(string, 1); //bright red

		exp = player(victim)->exp / 3;

		if(exp == 0){
			player_msg(victim, "You lost no experience points.", 1); //bright red
		}
		else{
			player(victim)->exp -= exp;
			string = comb_3str("You lost ",int_to_string(exp), " experience points.");
			player_msg(victim, string, 1); //bright red
		}

		map(player(victim)->map)->npc[mapnpcnum].target = 0;
		map(player(victim)->map)->npc[mapnpcnum].targettype = TARGET_NONE;
		on_death(victim);
	}
	else{
		player(victim)->vitals[VITAL_HP] -= damage;

		send_vital(victim, VITAL_HP);

		string = comb_5str("A ",npc(map(player(victim)->map)->npc[mapnpcnum].num)->name, " hit you for ", int_to_string(damage)," hit points.");
		player_msg(victim, string, 1); //bright red
	}
}

sbool can_npc_move(uint32 mapnum, uint8 mapnpcnum, uint8 dir)
{
	uint32 i = 0;
	uint8 x;
	uint8 y;

	if(mapnum >= MAX_MAPS || mapnpcnum >= MAX_MAP_NPCS|| dir >= DIR_COUNT || dir == 0)
		return FALSE;

	switch(dir){
	case DIR_UP:
		if(map(mapnum)->npc[mapnpcnum].y == 0)
			return FALSE;

		x = map(mapnum)->npc[mapnpcnum].x;
		y = map(mapnum)->npc[mapnpcnum].y - 1;
		break;
	case DIR_DOWN:
		if(map(mapnum)->npc[mapnpcnum].y >= MAX_MAPY - 1)
			return FALSE;

		x = map(mapnum)->npc[mapnpcnum].x;
		y = map(mapnum)->npc[mapnpcnum].y + 1;
		break;
	case DIR_LEFT:
		if(map(mapnum)->npc[mapnpcnum].x == 0)
			return FALSE;

		x = map(mapnum)->npc[mapnpcnum].x - 1;
		y = map(mapnum)->npc[mapnpcnum].y;
		break;
	case DIR_RIGHT:
		if(map(mapnum)->npc[mapnpcnum].x >= MAX_MAPX - 1)
			return FALSE;

		x = map(mapnum)->npc[mapnpcnum].x + 1;
		y = map(mapnum)->npc[mapnpcnum].y;
		break;
	default:
		return FALSE;
	}

	if( map(mapnum)->tile[GETXY(x,y)].type != TILE_TYPE_WALKABLE && map(mapnum)->tile[GETXY(x,y)].type != TILE_TYPE_ITEM)
		return FALSE;

	for(i = 0; i < total_players_online(); i++){
		if(player(player_online(i))->map == mapnum && player(player_online(i))->x == x && player(player_online(i))->y == y)
			return FALSE;
	}

	for(i = 0; i < MAX_MAP_NPCS; i++){
		if( i != mapnpcnum && map(mapnum)->npc[i].num > 0 && map(mapnum)->npc[i].x == x && map(mapnum)->npc[i].y == y)
			return FALSE;
	}

	return TRUE;
}

void npc_move(uint32 mapnum, uint8 mapnpcnum, uint8 dir)
{
	buffer_t buffer;

	if(mapnum >= MAX_MAPS || mapnpcnum >= MAX_MAP_NPCS)
		return;

	switch(dir){
	case DIR_UP:
		map(mapnum)->npc[mapnpcnum].y--;
		break;
	case DIR_DOWN:
		map(mapnum)->npc[mapnpcnum].y++;
		break;
	case DIR_LEFT:
		map(mapnum)->npc[mapnpcnum].x--;
		break;
	case DIR_RIGHT:
		map(mapnum)->npc[mapnpcnum].x++;
		break;
	default:
		return;
	}

	map(mapnum)->npc[mapnpcnum].dir = dir;

	clear_buffer(&buffer);
	add_opcode(&buffer, SNPCMOVE);
	add_buffer(&buffer, &mapnpcnum, SIZE8);
	add_buffer(&buffer, &map(mapnum)->npc[mapnpcnum].x, SIZE8);
	add_buffer(&buffer, &map(mapnum)->npc[mapnpcnum].y, SIZE8);
	add_buffer(&buffer, &map(mapnum)->npc[mapnpcnum].dir, SIZE8);

	send_data_to_map(&buffer, mapnum);
}

void npc_dir(uint32 mapnum, uint8 mapnpcnum, uint8 dir)
{
	buffer_t buffer;

	if(mapnum >= MAX_MAPS || mapnpcnum >= MAX_MAP_NPCS || dir >= DIR_COUNT || dir == 0)
		return;

	map(mapnum)->npc[mapnpcnum].dir = dir;

	clear_buffer(&buffer);
	add_opcode(&buffer,SNPCDIR);
	add_buffer(&buffer, &mapnpcnum, SIZE8);
	add_buffer(&buffer, &dir, SIZE8);

	send_data_to_map(&buffer, mapnum);
}

uint32 get_npc_max_vital(uint32 npcnum, uint8 vital)
{
	if(npcnum == 0 || npcnum >= MAX_NPCS)
		return 0;

	switch(vital){
	case VITAL_HP:
		return npc(npcnum)->stat[STAT_STRENGTH] * npc(npcnum)->stat[STAT_DEFENSE];
	case VITAL_MP:
		return npc(npcnum)->stat[STAT_MAGIC] * 2;
	case VITAL_SP:
		return npc(npcnum)->stat[STAT_SPEED] * 2;
	default:
		return 0;
	}
}

uint32 get_npc_vital_regen(uint32 npcnum, uint8 vital)
{
	uint32 i = 0;

	if(npcnum == 0 || npcnum >= MAX_NPCS)
		return 0;

	switch(vital){
	case VITAL_HP:
		i = npc(npcnum)->stat[STAT_DEFENSE] / 3;
		if( i <= 0)
			return 1;

		return i;
	default:
		return 0;
	}
}

uint32 get_player_damage(int16 index)
{
	uint32 damage;
	if(!temp_player(index)->loggedin)
		return 0;

	damage = player(index)->stat[STAT_STRENGTH] / 2;

	if(damage == 0)
		damage = 1;

	if(player(index)->equipment[EQUIPMENT_WEAPON] > 0)
		damage += item(player(index)->inv[player(index)->equipment[EQUIPMENT_WEAPON]].id)->data2;

	return damage;
}

uint32 get_player_protection(int16 index)
{
	uint32 protection;

	if(!temp_player(index)->loggedin)
		return 0;

	protection = player(index)->stat[STAT_DEFENSE] / 5;

	if(player(index)->equipment[EQUIPMENT_ARMOR] > 0)
		protection += item(player(index)->inv[player(index)->equipment[EQUIPMENT_ARMOR]].id)->data2;

	if(player(index)->equipment[EQUIPMENT_HELMET] > 0)
		protection += item(player(index)->inv[player(index)->equipment[EQUIPMENT_HELMET]].id)->data2;

	return protection;
}

sbool can_player_critical_hit(int16 index)
{
	uint32 i = 0;
	uint32 n = 0;

	if(player(index)->equipment[EQUIPMENT_WEAPON] > 0){
		n = rand() % 2;
		if(n == 1){
			i = (player(index)->stat[STAT_STRENGTH] / 2) + (player(index)->level / 2);

			n = (rand() % 100) + 1;

			if(n <= i)
				return TRUE;
		}
	}
	return FALSE;
}

sbool can_player_block_hit(int16 index)
{
	uint32 i = 0;
	uint32 n = 0;

	if(player(index)->equipment[EQUIPMENT_SHIELD] > 0){
		n = rand() % 2;
		if(n == 1){
			i = (player(index)->stat[STAT_DEFENSE] / 2) + (player(index)->level / 2);

			n = (rand() % 100) + 1;

			if(n <= i)
				return TRUE;
		}
	}
	return FALSE;
}

void cast_spell(int16 index, uint16 spellslot)
{
	uint16 spellnum;
	uint16 n = 0;
	uint32 damage;
	sbool casted;
	sbool cancast = FALSE;
	char *targetname = NULL;
	char *string = NULL;
	buffer_t buffer;
	uint8 x;
	uint8 y;

	if(spellslot >= MAX_PLAYER_SPELLS)
		return;

	spellnum = player(index)->spells[spellslot];

	if(!has_spell(index, spellnum))
		return;

	if(player(index)->vitals[VITAL_MP] < spell(spellnum)->mpreq){
		player_msg(index, "Not enough mana to cast this spell", 1); //light blue.
		return;
	}

	if(gettickcount() < temp_player(index)->attacktimer + 1000)
		return;

	if(spell(spellnum)->type == SPELL_GIVEITEM){
		n = find_open_inv_slot(index, spell(spellnum)->data1);

		if(n){
			give_item(index, spell(spellnum)->data1, spell(spellnum)->data2);

			string = comb_4str(player(index)->charname, " casts ", spell(spellnum)->name, ".");
			map_msg(player(index)->map, string, 1); //bright blue

			player(index)->vitals[VITAL_MP] -= spell(spellnum)->mpreq;

			send_vital(index, VITAL_MP);
			casted = TRUE;
		}
		else{
			player_msg(index, "Your inventory is full!", 1); //bright red
			casted = TRUE;
		}
	}

	n = temp_player(index)->target;

	switch(temp_player(index)->targettype){
	case TARGET_PLAYER:
		if(temp_player(n)->loggedin && player(index)->map == player(n)->map && player(index)->group < GROUP_MONITOR && player(n)->group < GROUP_MONITOR){
			if(player(n)->vitals[VITAL_HP] != 0)
				cancast = TRUE;
		}

		if(cancast){
			targetname = comb_2str(player(n)->charname,"");

			switch(spell(spellnum)->type){
			case SPELL_ADDHP:
				player(n)->vitals[VITAL_HP] += spell(spellnum)->data1;
				send_vital(n, VITAL_HP);
				break;
			case SPELL_ADDSP:
				player(n)->vitals[VITAL_SP] += spell(spellnum)->data1;
				send_vital(n, VITAL_SP);
				break;
			case SPELL_SUBHP:
				if(map(player(index)->map)->moral == MAP_TYPE_NONE){
					damage = (player(index)->stat[STAT_MAGIC] / 4) + spell(spellnum)->data1 - get_player_protection(n);

					if(damage){
						attack_player_with_magic(index, n, damage, spellnum);
					}
					else{
						string = comb_3str("The spell was to weak to hurt ", player(n)->charname, "!");
						player_msg(index, string, 1); //bright red
					}
				}
				break;
			case SPELL_SUBMP:
				if(map(player(index)->map)->moral == MAP_TYPE_NONE){
					player(n)->vitals[VITAL_MP] -= spell(spellnum)->data1;
					send_vital(n, VITAL_MP);
				}
				break;
			case SPELL_SUBSP:
				if(map(player(index)->map)->moral == MAP_TYPE_NONE){
					player(n)->vitals[VITAL_SP] -= spell(spellnum)->data1;
					send_vital(n, VITAL_SP);
				}
				break;
			default:
				return;
			}

			x = player(n)->x;
			y = player(n)->y;

			casted = TRUE;
		}
		break;
	case TARGET_NPC:

		if(npc(map(player(index)->map)->npc[n].num)->behavior != BEHAVIOR_FRIENDLY && npc(map(player(index)->map)->npc[n].num)->behavior != BEHAVIOR_SHOPKEEPER)
			cancast = TRUE;

		if(cancast){
			targetname = comb_2str(npc(map(player(index)->map)->npc[n].num)->name,"");

			switch(spell(spellnum)->type){
			case SPELL_ADDHP:
				map(player(index)->map)->npc[n].vitals[VITAL_HP] += spell(spellnum)->data1;
				break;
			case SPELL_ADDSP:
				map(player(index)->map)->npc[n].vitals[VITAL_SP] += spell(spellnum)->data1;
				break;
			case SPELL_SUBHP:
				damage = (player(index)->stat[STAT_MAGIC] / 4) + spell(spellnum)->data1 - (npc(map(player(index)->map)->npc[n].num)->stat[STAT_DEFENSE] / 2);

				if(damage){
					attack_npc_with_magic(index, n, damage, spellnum);
				}
				else{
					string = comb_3str("The spell was to weak to hurt ", npc(map(player(index)->map)->npc[n].num)->name, "!");
					player_msg(index, string, 1); //bright red
				}

				break;
			case SPELL_SUBMP:
				map(player(index)->map)->npc[n].vitals[VITAL_MP] -= spell(spellnum)->data1;
				break;
			case SPELL_SUBSP:
				map(player(index)->map)->npc[n].vitals[VITAL_SP] -= spell(spellnum)->data1;
				break;
			default:
				return;
			}

			x = map(player(index)->map)->npc[n].x;
			y = map(player(index)->map)->npc[n].y;

			casted = TRUE;
		}
		break;

	default:
		return;
	}

	if(casted){
		string = comb_7str(player(index)->charname," casts ",spell(spellnum)->name, " on ", targetname,".","");
		map_msg(player(index)->map, string, 1); //bright blue

		clear_buffer(&buffer);

		add_opcode(&buffer,SCASTSPELL);
		add_buffer(&buffer, &spellnum, SIZE16);
		add_buffer(&buffer, &x,SIZE8);
		add_buffer(&buffer, &y,SIZE8);

		send_data_to_map(&buffer, player(index)->map);

		player(index)->vitals[VITAL_MP] -= spell(spellnum)->mpreq;
		send_vital(index, VITAL_MP);

		temp_player(index)->attacktimer = gettickcount();
		temp_player(index)->castedspell = 1;
	}
}

void player_warp(int16 index, uint32 mapnum, uint8 x, uint8 y)
{
	char *string = NULL;
	uint32 oldmap;
	uint32 i = 0;
	buffer_t buffer;

	if(!temp_player(index)->loggedin ||  mapnum >= MAX_MAPS)
		return;

	temp_player(index)->target = 0;
	temp_player(index)->targettype = TARGET_NONE;

	if(map(player(index)->map)->shop){
		if(strlen(shop(map(player(index)->map)->shop)->leavesay)){
			string = comb_4str(shop(map(player(index)->map)->shop)->name," says, '",shop(map(player(index)->map)->shop)->leavesay,"'");
			player_msg(index, string, 1); //grey
		}
	}

	oldmap = player(index)->map;

	if(oldmap != mapnum)
		send_leave_map(index, oldmap);

	player(index)->map = mapnum;
	player(index)->x = x;
	player(index)->y = y;

	if(map(player(index)->map)->shop){
		if(strlen(shop(map(player(index)->map)->shop)->joinsay)){
			string = comb_4str(shop(map(player(index)->map)->shop)->name," says, '",shop(map(player(index)->map)->shop)->joinsay,"'");
			player_msg(index, string, 1); //grey
		}
	}

	players_on_map_minus(oldmap);
	if(players_on_map(oldmap) == 0){
		for(i = 0; i < MAX_MAP_NPCS; i++){
			if(map(oldmap)->npc[i].num > 0)
				map(oldmap)->npc[i].vitals[VITAL_HP] = get_npc_max_vital(map(oldmap)->npc[i].num, VITAL_HP);
		}

		players_on_map_plus(mapnum);
		temp_player(index)->gettingmap = TRUE;

		clear_buffer(&buffer);
		add_opcode(&buffer, SCHECKFORMAP);
		add_buffer(&buffer, &mapnum, SIZE32);
		add_buffer(&buffer, &map(mapnum)->revision, SIZE32);
		send_data(&buffer,index);
	}
}

void player_move(int16 index, uint8 dir, uint8 movement)
{
	uint32 mapnum;
	uint8 x = player(index)->x;
	uint8 y = player(index)->y;
	sbool moved = FALSE;
	buffer_t buffer;

	if(!temp_player(index)->loggedin)
		return;

	player(index)->dir = dir;

	switch(dir){
	case DIR_UP:
		if(y > 0){
			y--;
		}
		else{
			if(map(player(index)->map)->up)
				player_warp(index, map(player(index)->map)->up, player(index)->x, MAX_MAPY - 1); moved = TRUE;
		}
		break;
	case DIR_DOWN:
		if(y < MAX_MAPY - 1){
			y++;
		}
		else{
			if(map(player(index)->map)->down)
				player_warp(index, map(player(index)->map)->down, player(index)->x, 0); moved = TRUE;
		}
		break;
	case DIR_LEFT:
		if(x > 0){
			x--;
		}
		else{
			if(map(player(index)->map)->left)
				player_warp(index, map(player(index)->map)->left, MAX_MAPX - 1, player(index)->y); moved = TRUE;
		}
		break;
	case DIR_RIGHT:
		if(x < MAX_MAPX - 1){
			x++;
		}
		else{
			if(map(player(index)->map)->down)
				player_warp(index, map(player(index)->map)->down, 0, player(index)->y); moved = TRUE;
		}
		break;
	default: return;
	}

	if(map(player(index)->map)->tile[GETXY(x,y)].type != TILE_TYPE_BLOCKED && moved == FALSE){
		player(index)->x = x;
		player(index)->y = y;

		clear_buffer(&buffer);
		add_opcode(&buffer,SPLAYERMOVE);
		add_buffer(&buffer, &index, SIZE16);
		add_buffer(&buffer, &x, SIZE8);
		add_buffer(&buffer, &y, SIZE8);
		add_buffer(&buffer, &dir, SIZE8);
		add_buffer(&buffer, &movement, SIZE8);
		send_data_to_map_but(&buffer, player(index)->map, index);
		moved = TRUE;
	}

	if(map(player(index)->map)->tile[GETXY(x,y)].type == TILE_TYPE_WARP){
		player_warp(index, map(player(index)->map)->tile[GETXY(x,y)].data1, map(player(index)->map)->tile[GETXY(x,y)].data2, map(player(index)->map)->tile[GETXY(x,y)].data3);
		moved = TRUE;
	}

	if(!moved)
		hacking_attempt(index, "Position Modification");
}

void check_equipped_items(int16 index)
{
	uint8 slot;
	uint16 itemnum;
	uint32 i = 0;

	for(i = 0; i < EQUIPMENT_COUNT; i++){
		if(player(index)->equipment[i]){
			switch(i){
			case EQUIPMENT_WEAPON:
				if(item(player(index)->equipment[i])->type != ITEM_TYPE_WEAPON)
					player(index)->equipment[i] = 0;
				break;
			case EQUIPMENT_ARMOR:
				if(item(player(index)->equipment[i])->type != ITEM_TYPE_ARMOR)
					player(index)->equipment[i] = 0;
				break;
			case EQUIPMENT_HELMET:
				if(item(player(index)->equipment[i])->type != ITEM_TYPE_HELMET)
					player(index)->equipment[i] = 0;
				break;
			case EQUIPMENT_SHIELD:
				if(item(player(index)->equipment[i])->type != ITEM_TYPE_SHIELD)
					player(index)->equipment[i] = 0;
				break;
			}
		}
	}
}

uint32 find_open_inv_slot(int16 index, uint16 itemnum)
{
	uint32 i = 1;

	if(!temp_player(index)->loggedin || itemnum == 0 || itemnum >= MAX_ITEMS)
		return 0;

	if(item(itemnum)->type == ITEM_TYPE_CURRENCY){
		for(i = 1; i < MAX_INV; i++){
			if(player(index)->inv[i].id == itemnum)
				return i;
		}
	}
	for(i = 1; i < MAX_INV; i++){
		if(player(index)->inv[i].id == 0)
			return i;
	}
	return 0;
}

uint32 has_item(int16 index, uint16 itemnum)
{
	uint32 i = 1;

	if(!temp_player(index)->loggedin || itemnum == 0 || itemnum >= MAX_ITEMS)
		return 0;

	for(i = 1; i < MAX_INV; i++){
		if(player(index)->inv[i].id = itemnum){
			if(item(itemnum)->type == ITEM_TYPE_CURRENCY)
				return player(index)->inv[i].value;
			else
				return 1;
		}
	}
	return 0;
}

void take_item(int16 index, uint16 itemnum, uint32 itemval)
{
	uint8 i = 0;
	uint32 n = 0;
	sbool take_item;

	if(!temp_player(index)->loggedin || itemnum == 0 || itemnum >= MAX_ITEMS)
		return;

	for(i = 1; i < MAX_INV; i++){
		if(player(index)->inv[i].id == itemnum){
			switch(item(player(index)->inv[i].id)->type){
			case ITEM_TYPE_CURRENCY:

				if(itemval >= player(index)->inv[i].value){
					take_item = TRUE;
				}
				else{
					player(index)->inv[i].value -= itemval;
					send_inventory_update(index, i);
					return;
				}
				break;

			case ITEM_TYPE_WEAPON:
				if(player(index)->equipment[EQUIPMENT_WEAPON]){
					if(i == player(index)->equipment[EQUIPMENT_WEAPON]){
						player(index)->equipment[EQUIPMENT_WEAPON] = 0;
						send_worn_equipment(index);
						take_item = TRUE;
					}
					else
						take_item = TRUE;
				}
				else
					take_item = TRUE;
				break;
			case ITEM_TYPE_ARMOR:
				if(player(index)->equipment[EQUIPMENT_ARMOR]){
					if(i == player(index)->equipment[EQUIPMENT_ARMOR]){
						player(index)->equipment[EQUIPMENT_ARMOR] = 0;
						send_worn_equipment(index);
						take_item = TRUE;
					}
					else
						take_item = TRUE;
				}
				else
					take_item = TRUE;
				break;

			case ITEM_TYPE_HELMET:
				if(player(index)->equipment[EQUIPMENT_HELMET]){
					if(i == player(index)->equipment[EQUIPMENT_HELMET]){
						player(index)->equipment[EQUIPMENT_HELMET] = 0;
						send_worn_equipment(index);
						take_item = TRUE;
					}
					else
						take_item = TRUE;
				}
				else
					take_item = TRUE;
				break;

			case ITEM_TYPE_SHIELD:
				if(player(index)->equipment[EQUIPMENT_SHIELD]){
					if(i == player(index)->equipment[EQUIPMENT_SHIELD]){
						player(index)->equipment[EQUIPMENT_SHIELD] = 0;
						send_worn_equipment(index);
						take_item = TRUE;
					}
					else
						take_item = TRUE;
				}
				else
					take_item = TRUE;
				break;
			default:
				take_item = TRUE;
			}

			if(take_item){
				player(index)->inv[i].id = 0;
				player(index)->inv[i].value = 0;
				player(index)->inv[i].dur = 0;
				send_inventory_update(index, i);
				return;
			}
		}
	}
}

void give_item(int16 index, uint16 itemnum, uint32 itemval)
{
	uint8 i = 0;

	if(!temp_player(index)->loggedin || itemnum == 0 || itemnum >= MAX_ITEMS)
		return;

	if(i = find_open_inv_slot(index, itemnum)){
		player(index)->inv[i].id = itemnum;
		player(index)->inv[i].value += itemval;

		switch(item(itemnum)->type){
		case ITEM_TYPE_WEAPON: case ITEM_TYPE_ARMOR: case ITEM_TYPE_HELMET: case ITEM_TYPE_SHIELD:
			player(index)->inv[i].dur = item(itemnum)->data1;
		}

		send_inventory_update(index, i);
	}
	else
		player_msg(index, "Your inventory is full.", 1); //bright red
}

sbool has_spell(int16 index, uint32 spellnum)
{
	uint32 i = 1;

	for(i = 1; i < MAX_PLAYER_SPELLS; i++){
		if(player(index)->spells[i] == spellnum)
			return TRUE;
	}
	return FALSE;
}

uint16 find_open_spell_slot(int16 index)
{
	uint16 i = 1;

	for(i = 1; i < MAX_PLAYER_SPELLS; i++){
		if(player(index)->spells[i] == 0)
			return i;
	}

	return 0;
}

void player_map_get_item(int16 index)
{
	uint32 i = 1;
	uint8 n = 0;
	uint32 mapnum;
	char *string = NULL;

	if(!temp_player(index)->loggedin)
		return;

	mapnum =player(index)->map;

	for(i = 1; i < MAX_MAP_ITEMS; i++){
		if(map(mapnum)->items[i].num && map(mapnum)->items[i].num > MAX_ITEMS){
			if(map(mapnum)->items[i].x == player(index)->x && map(mapnum)->items[i].y == player(index)->y){
				if(n = find_open_inv_slot(index, map(mapnum)->items[i].num)){
					player(index)->inv[n].id = map(mapnum)->items[i].num;

					if(item(map(mapnum)->items[i].num)->type == ITEM_TYPE_CURRENCY){
						player(index)->inv[n].value = map(mapnum)->items[i].value;
						string = comb_5str("You picked up ", int_to_string(map(mapnum)->items[i].value), " ", item(player(index)->inv[n].id)->name, ".");
					}
					else{
						player(index)->inv[n].value = 0;
						string = comb_3str("You picked up ",item(player(index)->inv[n].id)->name, ".");
					}

					player(index)->inv[n].dur = map(mapnum)->items[i].dur;

					map(mapnum)->items[i].num = 0;
					map(mapnum)->items[i].value = 0;
					map(mapnum)->items[i].dur = 0;
					map(mapnum)->items[i].x = 0;
					map(mapnum)->items[i].y = 0;

					send_inventory_update(index, n);
					spawn_item_slot(i, 0, 0, 0, player(index)->map, 0, 0);
					player_msg(index, string, 1); //yellow
					return;
				}
				else{
					player_msg(index, "Your inventory is full.", 1); //bright red
					return;
				}
			}
		}
	}
}

void player_map_drop_item(int16 index, uint8 invnum, uint32 amount)
{
	uint8 i = 0;
	char *string = NULL;

	if(!temp_player(index)->loggedin || invnum == 0 || invnum >= MAX_INV)
		return;

	if(player(index)->inv[invnum].id > 0 && player(index)->inv[invnum].id < MAX_ITEMS){
		if(i = find_open_map_item_slot(player(index)->map)){
			map(player(index)->map)->items[i].dur = player(index)->inv[invnum].dur;
			map(player(index)->map)->items[i].num = player(index)->inv[invnum].id;
			map(player(index)->map)->items[i].x = player(index)->x;
			map(player(index)->map)->items[i].y = player(index)->y;

			if(item(player(index)->inv[invnum].id)->type == ITEM_TYPE_CURRENCY){
				if(amount >= player(index)->inv[invnum].value){
					map(player(index)->map)->items[i].value = player(index)->inv[invnum].value;
					string = comb_7str(player(index)->charname," drops ", int_to_string(player(index)->inv[invnum].value), " ", item(player(index)->inv[invnum].id)->name, ".", "");
					map_msg(player(index)->map, string, 1); //yellow
				}
				else{
					map(player(index)->map)->items[i].value = amount;
					string = comb_7str(player(index)->charname," drops ", int_to_string(amount), " ", item(player(index)->inv[invnum].id)->name, ".", "");
					map_msg(player(index)->map, string, 1); //yellow
				}
			}
			else{
				map(player(index)->map)->items[i].value = 0;
				string = comb_4str(player(index)->charname," drops a ", item(player(index)->inv[invnum].id)->name, ".");
				map_msg(player(index)->map, string, 1); //yellow
			}

			take_item(index,player(index)->inv[invnum].id,amount);

			spawn_item_slot(i, map(player(index)->map)->items[i].num, map(player(index)->map)->items[i].value, map(player(index)->map)->items[i].dur, player(index)->map, player(index)->x, player(index)->y);
		}
	}
	else
		player_msg(index, "To many items already on the ground.", 1); //bright red
}

uint64 get_player_next_level(int16 index)
{
	uint64 exp = 0;
	if(player(index)->level + 1 >= MAX_PLAYER_LEVEL){
		if(player(index)->level + 1 >= 50){
			return (player(index)->level + 1) * 10000;
		}
		return (player(index)->level + 1) * 100;
	}
	return 0;
}

void check_player_level_up(int16 index)
{
	uint32 i = 0;
	uint64 exprollover;
	uint64 needed = get_player_next_level(index);
	char *string = NULL;

	if(needed != 0){
		if(player(index)->exp >= needed){
			exprollover = player(index)->exp - needed;
			player(index)->level++;

			player(index)->points += 3;
			player(index)->exp = exprollover;

			string = comb_3str("You have gained a level!  You now have ", int_to_string(player(index)->points)," stat points to distribute.");
			player_msg(index, string, 1); //bright blue
		}
	}
}

uint32 get_player_vital_regen(int16 index, uint8 vital)
{
	uint32 i = 0;

	if(!temp_player(index)->loggedin)
		return 0;

	switch(vital){
	case VITAL_HP:
		i = (player(index)->stat[STAT_DEFENSE] / 2);
	case VITAL_MP:
		i = (player(index)->stat[STAT_MAGIC] / 2);
	case VITAL_SP:
		return (player(index)->stat[STAT_SPEED] / 2);
	}

	if(i == 0)
		i = 1;

	return i;
}

void on_death(int16 index)
{
	uint32 i = 0;

	player(index)->vitals[VITAL_HP] = 0;

	for(i = 0; i < EQUIPMENT_COUNT; i++){
		if(player(index)->equipment[i])
			player_map_drop_item(index, player(index)->equipment[i], 0);
	}

	player_warp(index, START_MAP, START_X, START_Y);

	player(index)->vitals[VITAL_HP] = get_player_max_vital(index, VITAL_HP);
	player(index)->vitals[VITAL_MP] = get_player_max_vital(index, VITAL_MP);
	player(index)->vitals[VITAL_SP] = get_player_max_vital(index, VITAL_SP);
	send_vital(index, VITAL_HP);
	send_vital(index, VITAL_MP);
	send_vital(index, VITAL_SP);

	if(player(index)->pk){
		player(index)->pk = 0;
		send_player_data(index);
	}
}

void damage_equipment(int16 index, uint8 equipmentslot)
{
	char *string = NULL;

	if(player(index)->equipment[equipmentslot]){
		if(player(index)->inv[player(index)->equipment[equipmentslot]].dur -1 > 0){
			player(index)->inv[player(index)->equipment[equipmentslot]].dur--;

			if(player(index)->inv[player(index)->equipment[equipmentslot]].dur <= 5){
				string = comb_3str("Your ", item(player(index)->inv[player(index)->equipment[equipmentslot]].id)->name," is about to break!");
				player_msg(index, string, 1); //yellow
			}
		}
		else{
			player(index)->inv[player(index)->equipment[equipmentslot]].dur = 0;
			string = comb_3str("Your ", item(player(index)->inv[player(index)->equipment[equipmentslot]].id)->name," has broken.");
			player_msg(index, string, 1); //yellow
			take_item(index, player(index)->inv[player(index)->equipment[equipmentslot]].id, 0);
		}
	}
}