/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
*           Jacob/Dugar
******************************************************************************/
#include "handlepackets.h"
#include <stdio.h>
#include "globals.h"
#include "servertcp.h"
#include "path.h"
#include "general.h"
#include "function.h"
#include "gamelogic.h"
#include <string.h>

void(*packets[CMSG_COUNT])(buffer_t *data, int16 index);

//CLOGIN AND CACCOUNTCREATE ARE NOT IN THIS FUNCTION.
void initpackets(void)
{
	packets[CSAYMSG] = &handle_say_message;
	packets[CEMOTEMSG] = &handle_emote_message;
	packets[CBROADCASTMSG] = &handle_broadcast_message;
	packets[CGLOBALMSG] = &handle_global_message;
	packets[CADMINMSG] = &handle_admin_message;
	packets[CPLAYERMSG] = &handle_player_message;
	packets[CPLAYERMOVE] = &handle_player_move;
	packets[CPLAYERDIR] = &handle_player_dir;
	packets[CUSEITEM] = &handle_use_item;
	packets[CATTACK] = &handle_attack;
	packets[CUSESTATPOINT] = &handle_use_stat_points;
	packets[CWARPMETO] = &handle_warp_me_to;
	packets[CWARPTOME] = &handle_warp_to_me;
	packets[CWARPTO] = &handle_warp_to;
	packets[CSETSPRITE] = &handle_set_spirit;
	packets[CREQUESTNEWMAP] = &handle_request_new_map;
	packets[CMAPDATA] = &handle_map_data;
	packets[CNEEDMAP] = &handle_need_map;
	packets[CMAPGETITEM] = &handle_map_get_item;
	packets[CMAPDROPITEM] = &handle_map_drop_item;
	packets[CMAPRESPAWN] = &handle_map_respawn;
	packets[CKICKPLAYER] = &handle_kick_player;
	packets[CBANPLAYER] = &handle_ban_player;
	packets[CREQUESTEDITMAP] = &handle_request_map_edit;
	packets[CREQUESTEDITITEM] = &handle_request_edit_item;
	packets[CEDITITEM] = &handle_edit_item;
	packets[CDELETE] = &handle_delete;
	packets[CSAVEITEM] = &handle_save_item;
	packets[CREQUESTEDITNPC] = &handle_request_edit_npc;
	packets[CEDITNPC] = &handle_edit_npc;
	packets[CSAVENPC] = &handle_save_npc;
	packets[CREQUESTEDITSHOP] = &handle_request_edit_shop;
	packets[CEDITSHOP] = &handle_edit_shop;
	packets[CSAVESHOP] = &handle_save_shop;
	packets[CREQUESTEDITSPELL] = &handle_request_edit_spell;
	packets[CEDITSPELL] = &handle_edit_spell;
	packets[CSAVESPELL] = &handle_save_spell;
	packets[CSETGROUP] = &handle_set_group;
	packets[CWHOSONLINE] = &handle_who_is_online;
	packets[CTRADE] = &handle_trade;
	packets[CTRADEREQUEST] = &handle_trade_request;
	packets[CFIXITEM] = &handle_fix_item;
	packets[CSEARCH] = &handle_search;
	packets[CSPELLS] = &handle_spells;
	packets[CCAST] = &handle_cast;
	packets[CQUIT] = &handle_quit;
}

void handle_data(buffer_t *data, uint64 socket_id)
{
	uint8 id = SNONE;
	int16 index;
	take_buffer(&id,data,1);

	if(data->offset == 0){
		error_handler(DE_ERROR_INVALID_PACKET_OFFSET);
		return;
	}

	if(id == SNONE){
		error_handler(DE_ERROR_INVALID_PACKET_ID);
		return;
	}

	if(id > CNEWACCOUNT)
	{
		index = get_temp_player_index(socket_id);

		if(index != TEMP_NO_MATCH){
			packets[id](data, index);
		}
	}
	else{
		switch(id){
		case CLOGIN:
			handle_login(data, socket_id); break;
		case CNEWACCOUNT:
			handle_new_account(data,socket_id); break;
		}
	}
}

void handle_login(buffer_t *data, uint64 socket_id)
{
	char *name = (char *)calloc(MAX_NAME_LENGTH, sizeof(char));
	char *password = (char *)calloc(MAX_PASS_LENGTH, sizeof(char));
	char *string = NULL;
	char *path;
	uint8 client_major;
	uint8 client_minor;
	uint8 client_rev;
	int16 index;

	take_string(name,data);
	take_string(password,data);
	take_buffer(&client_major, data, SIZE8);
	take_buffer(&client_minor, data, SIZE8);
	take_buffer(&client_rev, data, SIZE8);

	if(client_major < VERSION_MAJOR || client_minor < VERSION_MINOR || client_rev < VERSION_REV){
		string = comb_2str("Version outdated, please visit ", GAME_WEBSTIE);
		alert_msg_socket(socket_id, string);
		return;
	}

	if(shutting_down()){
		alert_msg_socket(socket_id, "Server is either rebooting or being shutdown.");
		return;
	}

	index = get_temp_player_index(socket_id);

	if(index == TEMP_NO_MATCH){
		if((index = set_temp_player_index(socket_id)) == TEMP_FULL){
			alert_msg_socket(index, "Server is full.");
		}
	}
	else{
		if(temp_player(index)->loggedin)
			return;
	}

	if(strlen(name) < 3 || strlen(password) < 3){
		alert_msg(index, "Your name and password must be at least three characters in length");
		return;
	}

	if(account_exists(name)){
		if(is_multi_accounts(name)){
			alert_msg(index, "Multiple account logins is not authorized.");
			return;
		}

		path = get_path_name(PLAYER_PATH, name, FILE_ENDING);
		read_player(path, index);

		if(!strcmp(player(index)->password, password)){
			alert_msg(index, "Either your password or username where not correct. Or your account does not exist");
			clear_player(index);
			clear_user_socket(socket_id);
			return;
		}

		string = comb_2str(player(index)->charname, " has logged in.");

		add_log(string, PLAYER_LOG);
		printf("%s \n", string);

		join_game(index);
	}
	else{
		alert_msg(index, "Either your password or username where not correct. Or your account does not exist");
		return;
	}
}

void handle_new_account(buffer_t *data, uint64 socket_id)
{
	char *name = (char *)calloc(MAX_NAME_LENGTH, sizeof(char));
	char *charname = (char *)calloc(MAX_NAME_LENGTH, sizeof(char));
	char *password = (char *)calloc(MAX_PASS_LENGTH, sizeof(char));
	uint8 sex;
	uint8 type;
	uint32 i = 0;
	char *path;

	if(get_temp_player_index(socket_id) == TEMP_NO_MATCH){
		take_string(name, data);
		take_string(charname, data);
		take_string(password, data);
		take_buffer(&sex, data, SIZE8);
		take_buffer(&type, data, SIZE8);

		if(strlen(name) < 3 || strlen(password) < 3){
			alert_msg_socket(socket_id, "Your name and password must be at least three characters in length");
			return;
		}

		for( i = 0; i < strlen(name); i++){
			if(!isnamelegal(name[i])){
				alert_msg_socket(socket_id, "Invalid name, only letters, numbers, spaces, and _ allowed in names.");
				return;
			}
		}

		path =  get_path_name(PLAYER_PATH, name, FILE_ENDING);
		if(!file_exists(path)){
			char *string = NULL;
			add_account(name,password,charname,sex,type);
			printf("Account %s has been created \n", name);
			string = comb_3str("Account ", name, " has been created.");
			add_log(string, PLAYER_LOG);
			alert_msg_socket(socket_id, "Your account has been created!");
		}
		else{
			alert_msg_socket(socket_id, "Sorry, that account name is already taken!");
		}
	}
}

void handle_say_message(buffer_t *data, int16 index)
{
	char *msg;
	uint32 len;
	char *string = NULL;

	take_buffer(&len, data, SIZE32);
	msg = (char *)calloc(len, sizeof(char));
	take_string(msg, data);

	string = comb_4str(player(index)->charname," says, '",msg,"'");
	map_msg(player(index)->map,string,1); //grey
}

void handle_emote_message(buffer_t *data, int16 index)
{
	char *msg;
	uint32 len;
	char *string = NULL;

	take_buffer(&len, data, SIZE32);
	msg = (char *)calloc(len, sizeof(char));
	take_string(msg, data);
	//TODO: not complete yet still need Right function for strings.
	string = comb_4str(player(index)->charname," ",msg,"'");
	map_msg(player(index)->map,string,1); //grey
	//MapMsg(GetPlayerMap(Index), GetPlayerName(Index) & " " & Right$(MSG, Len(MSG) - 1), BrightCyan)
}

void handle_broadcast_message(buffer_t *data, int16 index)
{
	char *msg;
	char *string = NULL;
	uint32 len;

	take_buffer(&len, data, SIZE32);
	msg = (char *)calloc(len, sizeof(char));
	take_string(msg, data);

	string = comb_3str(player(index)->charname, ": ", msg);
	global_msg(string, 1); //pink
}

void handle_global_message(buffer_t *data, int16 index)
{
	char *msg;
	char *string = NULL;
	uint32 len;

	take_buffer(&len, data, SIZE32);
	msg = (char *)calloc(len, sizeof(char));
	take_string(msg, data);

	string = comb_4str("(global) ",player(index)->charname, ": ", msg);
	global_msg(string, 1); //bright blue
}

void handle_admin_message(buffer_t *data, int16 index)
{
	if(player(index)->group >= GROUP_MONITOR){
		char *msg;
		char *string = NULL;
		uint32 len;

		take_buffer(&len, data, SIZE32);
		msg = (char *)calloc(len, sizeof(char));
		take_string(msg, data);

		string = comb_4str("(global) ",player(index)->charname, ": ", msg);
		admin_msg(string,1); // bright cyan
	}
}

void handle_player_message(buffer_t *data, int16 index)
{
	char *msg;
	char *msgto = (char *)calloc(MAX_NAME_LENGTH, sizeof(char));
	char *string = NULL;
	int16 indexto;
	uint32 len;

	take_string(msgto,data);
	take_buffer(&len, data, SIZE32);
	msg = (char *)calloc(len, sizeof(char));
	take_string(msg, data);

	if(indexto = find_player(msgto)){
		if(indexto != index){
			string = comb_4str(player(index)->charname, " tells you, '", msgto, "'");
			player_msg(index,string,1);
			//BrightGreen

			string = comb_5str("You tell ",player(indexto)->charname, ", '", msgto, "'");
			player_msg(index,string,1);
		}
	}
	else{
		//white
		player_msg(index,"Player is not online.",1);
	}
}

void handle_player_move(buffer_t *data, int16 index)
{
	uint8 dir;
	uint8 movement;

	if(temp_player(index)->gettingmap)
		return;

	take_buffer(&dir, data, SIZE8);
	take_buffer(&movement, data, SIZE8);

	if( dir < DIR_DOWN || dir >= DIR_COUNT){
		hacking_attempt(index,"Invalid Direction");
		return;
	}

	if(movement < MOVEMENT_WALKING || movement >= MOVEMENT_COUNT){
		hacking_attempt(index,"Invalid Movement");
		return;
	}

	if(temp_player(index)->castedspell){
		if( gettickcount() > temp_player(index)->attacktimer + 1000){
			temp_player(index)->castedspell = 0;
		}
		else{
			send_player_xy(index);
			return;
		}
	}
	player_move(index,dir,movement);
}

void handle_player_dir(buffer_t *data, int16 index)
{
	uint8 dir;
	buffer_t buffer;

	if(temp_player(index)->gettingmap)
		return;

	take_buffer(&dir, data, SIZE8);

	if( dir < DIR_DOWN || dir >= DIR_COUNT){
		hacking_attempt(index,"Invalid Direction");
		return;
	}

	player(index)->dir = dir;

	clear_buffer(&buffer);

	add_opcode(&buffer, SPLAYERDIR);
	add_buffer(&buffer, &index, SIZE16);
	add_buffer(&buffer, &dir, SIZE8);
	send_data_to_map_but(&buffer,player(index)->map,index);
}

void handle_use_item(buffer_t *data, int16 index)
{
	uint8 invnum;
	uint16 itemnum;
	uint32 i;
	uint32 n;
	uint32 x;
	uint32 y;
	char *string = NULL;
	buffer_t buffer;

	take_buffer(&invnum, data, SIZE8);

	if(!temp_player(index)->loggedin){
		hacking_attempt(index, "Attempted hacking of use item");
		return;
	}

	if(invnum > MAX_INV){
		hacking_attempt(index, "Invalid InvNum");
		return;
	}

	itemnum = player(index)->inv[invnum].id;

	if(itemnum < MAX_ITEMS){
		switch(item(itemnum)->type){
		case ITEM_TYPE_ARMOR:
			if(invnum != player(index)->equipment[EQUIPMENT_ARMOR]){
				if(player(index)->stat[STAT_DEFENSE] < item(itemnum)->data2){
					string = comb_3str("Your defense is to low to wear this armor!  Required DEF (",int_to_string(item(itemnum)->data2),")");
					player_msg(index, string, 1); //bright red
					return;
				}
				player(index)->equipment[EQUIPMENT_ARMOR] = invnum;
			}
			else{
				player(index)->equipment[EQUIPMENT_ARMOR] = 0;
			}
			send_worn_equipment(index);
			break;

		case ITEM_TYPE_WEAPON:
			if(invnum != player(index)->equipment[EQUIPMENT_WEAPON]){
				if(player(index)->stat[STAT_STRENGTH] < item(itemnum)->data2){
					string = comb_3str("Your strength is to low to hold this weapon!  Required DEF (",int_to_string(item(itemnum)->data2),")");
					player_msg(index, string, 1); //bright red
					return;
				}
				player(index)->equipment[EQUIPMENT_WEAPON] = invnum;
			}
			else{
				player(index)->equipment[EQUIPMENT_WEAPON] = 0;
			}
			send_worn_equipment(index);
			break;

		case ITEM_TYPE_HELMET:
			if(invnum != player(index)->equipment[EQUIPMENT_HELMET]){
				if(player(index)->stat[STAT_DEFENSE] < item(itemnum)->data2){
					string = comb_3str("Your defense is to low to wear this helmet!  Required DEF (",int_to_string(item(itemnum)->data2),")");
					player_msg(index, string, 1); //bright red
					return;
				}
				player(index)->equipment[EQUIPMENT_HELMET] = invnum;
			}
			else{
				player(index)->equipment[EQUIPMENT_HELMET] = 0;
			}
			send_worn_equipment(index);
			break;

		case ITEM_TYPE_SHIELD:
			if(invnum != player(index)->equipment[EQUIPMENT_SHIELD]){
				if(player(index)->stat[STAT_DEFENSE] < item(itemnum)->data2){
					string = comb_3str("Your defense is to low to wear this helmet!  Required DEF (",int_to_string(item(itemnum)->data2),")");
					player_msg(index, string, 1); //bright red
					return;
				}
				player(index)->equipment[EQUIPMENT_SHIELD] = invnum;
			}
			else{
				player(index)->equipment[EQUIPMENT_SHIELD] = 0;
			}

			send_worn_equipment(index);
			break;

		case ITEM_TYPE_POTIONADDHP:
			player(index)->vitals[VITAL_HP] += item(itemnum)->data1;
			player(index)->inv[invnum].id = 0;
			player(index)->inv[invnum].dur = 0;
			player(index)->inv[invnum].value = 0;
			send_inventory_update(index, invnum);
			send_vital(index, VITAL_HP);
			break;

		case ITEM_TYPE_POTIONADDMP:
			player(index)->vitals[VITAL_MP] += item(itemnum)->data1;
			player(index)->inv[invnum].id = 0;
			player(index)->inv[invnum].dur = 0;
			player(index)->inv[invnum].value = 0;
			send_inventory_update(index, invnum);
			send_vital(index, VITAL_MP);
			break;

		case ITEM_TYPE_POTIONADDSP:
			player(index)->vitals[VITAL_SP] += item(itemnum)->data1;
			player(index)->inv[invnum].id = 0;
			player(index)->inv[invnum].dur = 0;
			player(index)->inv[invnum].value = 0;
			send_inventory_update(index, invnum);
			send_vital(index, VITAL_SP);
			break;

		case ITEM_TYPE_POTIONSUBHP:
			player(index)->vitals[VITAL_HP] -= item(itemnum)->data1;
			player(index)->inv[invnum].id = 0;
			player(index)->inv[invnum].dur = 0;
			player(index)->inv[invnum].value = 0;
			send_inventory_update(index, invnum);
			send_vital(index, VITAL_HP);
			break;

		case ITEM_TYPE_POTIONSUBMP:
			player(index)->vitals[VITAL_MP] -= item(itemnum)->data1;
			player(index)->inv[invnum].id = 0;
			player(index)->inv[invnum].dur = 0;
			player(index)->inv[invnum].value = 0;
			send_inventory_update(index, invnum);
			send_vital(index, VITAL_MP);
			break;

		case ITEM_TYPE_POTIONSUBSP:
			player(index)->vitals[VITAL_SP] -= item(itemnum)->data1;
			player(index)->inv[invnum].id = 0;
			player(index)->inv[invnum].dur = 0;
			player(index)->inv[invnum].value = 0;
			send_inventory_update(index, invnum);
			send_vital(index, VITAL_SP);
			break;

		case ITEM_TYPE_KEY:
			break;

		case ITEM_TYPE_SPELL:

			if(item(itemnum)->data1 == 0){
				player_msg(index, "This scroll is not connected to a spell, please inform an admin!", 1); //white
				return;
			}

			if(spell(item(itemnum)->data1)->jobreq == player(index)->job || spell(item(itemnum)->data1)->jobreq == 0){
				if(spell(item(itemnum)->data1)->levelreq <= player(index)->level){
					if( i = find_open_spell_slot(index)){
						if(!has_spell(index, item(itemnum)->data1)){
							player(index)->spells[i] = item(itemnum)->data1;
							player(index)->inv[invnum].id = 0;
							player(index)->inv[invnum].dur = 0;
							player(index)->inv[invnum].value = 0;
							player_msg(index, "You study the spell carefully...", 1); //yellow
							player_msg(index, "You have learned a new spell!", 1); //white
						}
						else{
							player_msg(index, "You have already learned this spell!", 1); //bright red
						}
					}
					else{
						player_msg(index,  "You have learned all that you can learn!", 1); //bright red
					}
				}
				else
					string = comb_3str("You must be level ",int_to_string(spell(item(itemnum)->data1)->levelreq)," to learn this spell.");
				player_msg(index, string, 1); //white
			}
			else
				string = comb_3str("This spell can only be learned by a ",job(spell(item(itemnum)->data1)->jobreq)->name,".");
			player_msg(index,  string, 1); //white
		}
	}
}

void handle_attack(buffer_t *data, int16 index)
{
	uint8 i = 0;
	uint32 n;
	uint32 damage;
	uint32 tempindex;
	char *string = NULL;

	for( i = 0; i < players_online_count(); i++){
		tempindex = player_online(i);
		if( tempindex != index){
			if(can_attack_player(index,tempindex)){
				if(!can_player_block_hit(tempindex)){
					if(!can_player_critical_hit(index)){
						damage = get_player_damage(index) - get_player_protection(tempindex);
					}
					else{
						n = get_player_damage(index);
						damage = n + (rand() % (n / 2)) + 1 - get_player_protection(tempindex);
						player_msg(index,  "You feel a surge of energy upon swinging!", 1); //BrightCyan
						string = comb_2str(player(index)->charname," swings with enormous might!");
						player_msg(tempindex, string, 1); //BrightCyan
					}

					attack_player(index, tempindex, damage);
				}
				else{
					string = comb_4str(player(tempindex)->charname,"'s ",item(player(tempindex)->inv[player(tempindex)->equipment[EQUIPMENT_SHIELD]].id)->name, " has blocked your hit!");
					player_msg(tempindex, string, 1); //BrightCyan
					string = comb_5str("Your ",item(player(tempindex)->inv[player(tempindex)->equipment[EQUIPMENT_SHIELD]].id)->name," has blocked ",player(index)->charname,"'s hit!");
					player_msg(tempindex, string, 1); //BrightCyan
				}
				return;
			}
		}
	}

	for(i = 0; i < MAX_MAP_NPCS; i++){
		if(can_attack_npc(index, i)){
			if(!can_player_critical_hit(index)){
				damage = get_player_damage(index) - (npc(map(player(index)->map)->npc[i].num)->stat[STAT_DEFENSE] / 2);
			}
			else{
				n = get_player_damage(index);
				damage = n + (rand() % (n / 2)) + 1 - (npc(map(player(index)->map)->npc[i].num)->stat[STAT_DEFENSE] / 2);
				player_msg(index,  "You feel a surge of energy upon swinging!", 1); //BrightCyan
			}

			if(damage > 0){
				attack_npc(index,i,damage);
			}
			else
				player_msg(tempindex, "Your attack does nothing.", 1); //Bright red
		}
	}
}

void handle_use_stat_points(buffer_t *data, int16 index)
{
	uint8 point_type;

	take_buffer(&point_type,data,SIZE8);

	if(point_type >= STAT_COUNT){
		hacking_attempt(index, "Invalid Point Type");
		return;
	}

	if(player(index)->points > 0){
		player(index)->points--;

		switch(point_type){
		case STAT_STRENGTH:
			player(index)->stat[STAT_STRENGTH]++;
			player_msg(index, "You have gained more strength!", 1); //white
			break;
		case STAT_DEFENSE:
			player(index)->stat[STAT_DEFENSE]++;
			player_msg(index, "You have gained more defense!", 1); //white
			break;
		case STAT_SPEED:
			player(index)->stat[STAT_SPEED]++;
			player_msg(index, "You have gained more speed!", 1); //white
			break;
		case STAT_MAGIC:
			player(index)->stat[STAT_MAGIC]++;
			player_msg(index, "You have gained more magic!", 1); //white
			break;
		}
	}
	else{
		player_msg(index, "You have no skill points to train with!", 1); //bright red
		return;
	}

	send_stats(index);
}

void handle_warp_me_to(buffer_t *data, int16 index)
{
	int16 n;
	char *string = NULL;

	if(player(index)->group < GROUP_MAPPER){
		hacking_attempt(index, "Admin Cloning");
		return;
	}

	take_string(string,data);

	n = find_player(string);

	if(n != index ){
		if( n < 0){
			player_warp(index,player(n)->map,player(n)->x,player(n)->y);
			string = comb_3str("You have been warped to ",player(n)->charname,".");
			player_msg(index, string, 1); //Bright Blue
			string = comb_3str(player(index)->charname, " has warped to ",player(n)->charname);
			add_log(string, ADMIN_LOG);
		}
		else{
			player_msg(index, "Player is not online.", 1); //white
		}
	}
	else{
		player_msg(index, "You cannot warp to yourself!", 1); //white
	}
}

void handle_warp_to_me(buffer_t *data, int16 index)
{
	int16 n;
	char *string = NULL;

	if(player(index)->group < GROUP_MAPPER){
		hacking_attempt(index, "Admin Cloning");
		return;
	}

	take_string(string,data);

	n = find_player(string);

	if(n != index ){
		if( n < 0){
			player_warp(n,player(index)->map,player(index)->x,player(index)->y);
			string = comb_3str("You have been summoned by ",player(index)->charname,".");
			player_msg(n, string, 1); //Bright Blue
			string = comb_2str(player(n)->charname," has been summoned.");
			player_msg(index, string, 1); //Bright Blue
			string = comb_4str(player(index)->charname, " has warped ",player(n)->charname," to self.");
			add_log(string, ADMIN_LOG);
		}
		else{
			player_msg(index, "Player is not online.", 1); //white
		}
	}
	else{
		player_msg(index, "You cannot warp yourself to yourself!", 1); //white
	}
}

void handle_warp_to(buffer_t *data, int16 index)
{
	uint32 n;
	char *string = NULL;

	if(player(index)->group < GROUP_MAPPER){
		hacking_attempt(index, "Admin Cloning");
		return;
	}

	take_buffer(&n,data,SIZE32);

	if( n == 0 || n > MAX_MAPS){
		hacking_attempt(index, "Invalid map");
		return;
	}

	player_warp(index, n,player(index)->x,player(index)->y);
	string = comb_2str("You have been warped to map #",int_to_string(n));
	player_msg(index, string, 1); //Bright Blue
	string = comb_4str(player(index)->charname, " warped to map #", int_to_string(n),".");
	add_log(string, ADMIN_LOG);
}

void handle_set_spirit(buffer_t *data, int16 index)
{
	uint32 n;

	if(player(index)->group < GROUP_MAPPER){
		hacking_attempt(index, "Admin Cloning");
		return;
	}
	take_buffer(&n,data,SIZE32);
	player(index)->sprite = n;
	send_player_data(index);
}

void handle_request_new_map(buffer_t *data, int16 index)
{
	uint8 dir;

	take_buffer(&dir,data,SIZE8);

	if(dir >= DIR_COUNT){
		hacking_attempt(index, "Invalid Direction");
		return;
	}

	player_move(index,dir,1);
}

void handle_map_data(buffer_t *data, int16 index)
{
	uint8 x = 0;
	char *string = NULL;

	if(player(index)->group < GROUP_MAPPER){
		hacking_attempt(index, "Admin Cloning");
		return;
	}

	take_string(map(player(index)->map)->name ,data);
	take_buffer(&map(player(index)->map)->moral,data,SIZE8);
	take_buffer(&map(player(index)->map)->music,data,SIZE8);
	take_buffer(&map(player(index)->map)->left,data,SIZE16);
	take_buffer(&map(player(index)->map)->right,data,SIZE16);
	take_buffer(&map(player(index)->map)->up,data,SIZE16);
	take_buffer(&map(player(index)->map)->down,data,SIZE16);
	take_buffer(&map(player(index)->map)->shop,data,SIZE16);
	take_buffer(&map(player(index)->map)->bootmap,data,SIZE32);
	take_buffer(&map(player(index)->map)->bootx,data,SIZE8);
	take_buffer(&map(player(index)->map)->booty,data,SIZE8);

	for( x = 0; x < MAX_MAP_NPCS; x++){
		take_buffer(&map(player(index)->map)->npc[x].num,data,SIZE16);
		map(player(index)->map)->npc[x].target = 0;
		map(player(index)->map)->npc[x].targettype = TARGET_NONE;
		map(player(index)->map)->npc[x].vitals[VITAL_HP] = get_npc_max_vital(map(player(index)->map)->npc[x].num,VITAL_HP);
		map(player(index)->map)->npc[x].vitals[VITAL_MP] = get_npc_max_vital(map(player(index)->map)->npc[x].num,VITAL_MP);
		map(player(index)->map)->npc[x].vitals[VITAL_SP] = get_npc_max_vital(map(player(index)->map)->npc[x].num,VITAL_SP);
	}

	take_buffer(map(player(index)->map)->tile,data,(MAX_MAPX * MAX_MAPY) * sizeof(tile_t));

	map(player(index)->map)->revision++;

	spawn_map_npcs(player(index)->map);

	for(x = 1; x < MAX_MAP_ITEMS; x++){
		spawn_item_slot(x, 0, 0, 0,player(index)->map, map(player(index)->map)->items[x].x , map(player(index)->map)->items[x].y);

		map(player(index)->map)->items[x].num = 0;
		map(player(index)->map)->items[x].dur = 0;
		map(player(index)->map)->items[x].value = 0;
		map(player(index)->map)->items[x].x = 0;
		map(player(index)->map)->items[x].y = 0;
	}

	spawn_map_items(player(index)->map);
	string = get_path(MAP_PATH,player(index)->map,FILE_ENDING);
	write_map(string,player(index)->map);

	mapcache_create(player(index)->map);

	for(x = 0; x < total_players_online(); x++){
		if(player(player_online(x))->map == player(index)->map){
			player_warp(player_online(x),player(index)->map,player(player_online(x))->x,player(player_online(x))->y);
		}
	}
}

void handle_need_map(buffer_t *data, int16 index)
{
	sbool s;
	buffer_t buffer;

	take_buffer(&s,data,SIZE8);

	if(s == TRUE)
		send_map(index, player(index)->map);

	send_map_items_to(index, player(index)->map);
	send_map_npcs_to(index, player(index)->map);
	send_join_map(index);
	temp_player(index)->gettingmap = FALSE;

	clear_buffer(&buffer);
	add_opcode(&buffer,SMAPDONE);
	send_data(&buffer,index);
}

void handle_map_get_item(buffer_t *data, int16 index)
{
	player_map_get_item(index);
}

void handle_map_drop_item(buffer_t *data, int16 index)
{
	uint16 invnum;
	uint32 amount;
	buffer_t buffer;

	take_buffer(&invnum, data,SIZE16);
	take_buffer(&amount, data,SIZE32);

	if(invnum < 1 || invnum > MAX_INV){
		hacking_attempt(index,"Invalid InvNum");
	}

	if(amount > player(index)->inv[invnum].value){
		hacking_attempt(index,"Item amount modification");
	}

	if(item(player(index)->inv[invnum].id)->type == ITEM_TYPE_CURRENCY){
		if(amount <= 0){
			take_item(index, player(index)->inv[invnum].id, 0);
			return;
		}
	}

	player_map_drop_item(index,invnum,amount);
}

void handle_map_respawn(buffer_t *data, int16 index)
{
	uint8 i = 0;

	if(player(index)->group < GROUP_MAPPER){
		hacking_attempt(index, "Admin Cloning");
		return;
	}

	for(i = 0; i < MAX_MAP_ITEMS; i++){
		spawn_item_slot(i, 0, 0, 0,player(index)->map, map(player(index)->map)->items[i].x , map(player(index)->map)->items[i].y);

		map(player(index)->map)->items[i].num = 0;
		map(player(index)->map)->items[i].dur = 0;
		map(player(index)->map)->items[i].value = 0;
		map(player(index)->map)->items[i].x = 0;
		map(player(index)->map)->items[i].y = 0;
	}

	spawn_map_items(player(index)->map);

	for(i = 0; i < MAX_MAP_NPCS; i++){
		spawn_npc(i, player(index)->map);
	}

	player_msg(index,"Map Respawned", 1); //blue
}

void handle_kick_player(buffer_t *data, int16 index)
{
	char *string = NULL;
	int16 n;

	if(player(index)->group < GROUP_MONITOR){
		hacking_attempt(index, "Admin Cloning");
		return;
	}

	take_string(string, data);

	n = find_player(string);

	if(n != index){
		if(n > 0){
			if(player(n)->group < player(index)->group){
				string = comb_4str(player(index)->charname," has kicked ",player(n)->charname, ".");
				add_log(string, ADMIN_LOG);
				alert_msg(n, "You have been kicked from the game!");
			}
			else{
				player_msg(index,"That is a higher or same access admin then you!", 1);//white
			}
		}
		else{
			player_msg(index,"Player is not online.", 1);//white
		}
	}
	else{
		player_msg(index,"You cannot kick yourself!", 1);//white
	}
}

void handle_ban_player(buffer_t *data, int16 index)
{
	char *string = NULL;
	int16 n;

	if(player(index)->group < GROUP_MAPPER){
		hacking_attempt(index, "Admin Cloning");
		return;
	}

	take_string(string, data);

	n = find_player(string);

	if(n != index){
		if(n > 0){
			if(player(n)->group < player(index)->group){
				player(n)->banned = TRUE;
				alert_msg(n, "You have been banned from the game!");
			}
			else{
				player_msg(index,"That is a higher or same access admin then you!", 1);//white
			}
		}
		else{
			player_msg(index,"Player is not online.", 1);//white
		}
	}
	else{
		player_msg(index,"You cannot ban yourself!", 1);//white
	}
}

void handle_request_map_edit(buffer_t *data, int16 index)
{
	buffer_t buffer;

	if(player(index)->group < GROUP_MAPPER){
		hacking_attempt(index, "Admin Cloning");
		return;
	}
	clear_buffer(&buffer);
	add_opcode(&buffer, SEDITMAP);
	send_data(&buffer, index);
}

void handle_request_edit_item(buffer_t *data, int16 index)
{
	buffer_t buffer;

	if(player(index)->group < GROUP_DEVELOPER){
		hacking_attempt(index, "Admin Cloning");
		return;
	}

	clear_buffer(&buffer);
	add_opcode(&buffer, SITEMEDITOR);
	send_data(&buffer, index);
}

void handle_edit_item(buffer_t *data, int16 index)
{
	uint16 n;
	buffer_t buffer;
	char *string = NULL;

	if(player(index)->group < GROUP_DEVELOPER){
		hacking_attempt(index, "Admin Cloning");
		return;
	}

	take_buffer(&n,data,SIZE16);

	if(n <= 0 || n >= MAX_ITEMS){
		hacking_attempt(index,  "Invalid Item Index");
		return;
	}

	string = comb_4str(player(index)->charname, " is editing item #", int_to_string(n), ".");
	add_log(string, ADMIN_LOG);
	send_edit_item_to(index, n);
}

void handle_delete(buffer_t *data, int16 index)
{
	uint8 editor;
	uint16 n;
	buffer_t buffer;
	char *string = NULL;

	if(player(index)->group < GROUP_DEVELOPER){
		hacking_attempt(index, "Admin Cloning");
		return;
	}

	take_buffer(&editor, data, SIZE8);
	take_buffer(&n, data, SIZE16);

	switch(editor){
	case EDITOR_ITEM:
		if(n == 0 || n > MAX_ITEMS)
			hacking_attempt(index, "Invalid Item Index"); return;

		clear_item(n);
		send_update_item_to_all(n);

		write_item(get_path(ITEM_PATH,n,FILE_ENDING),n);
		string = comb_4str(player(index)->charname, "Deleted item #", int_to_string(n), ".");
		add_log(string, ADMIN_LOG);

	case EDITOR_NPC:
		if(n == 0 || n > MAX_NPCS)
			hacking_attempt(index, "Invalid Npc Index"); return;

		clear_npc(n);
		send_update_npc_to_all(n);
		write_npc(get_path(NPC_PATH,n,FILE_ENDING),n);
		string = comb_4str(player(index)->charname, "Deleted Npc #", int_to_string(n), ".");
		add_log(string, ADMIN_LOG);

	case EDITOR_SPELL:
		if(n == 0 || n > MAX_SPELLS)
			hacking_attempt(index, "Invalid Spell Index"); return;

		clear_spell(n);
		send_update_spell_to_all(n);
		write_spell(get_path(SPELL_PATH,n,FILE_ENDING),n);
		string = comb_4str(player(index)->charname, "Deleted Spell #", int_to_string(n), ".");
		add_log(string, ADMIN_LOG);

	case EDITOR_SHOP:
		if(n == 0 || n > MAX_SHOPS)
			hacking_attempt(index, "Invalid Shop Index"); return;

		clear_shop(n);
		send_update_shop_to_all(n);
		write_shop(get_path(SHOP_PATH,n,FILE_ENDING),n);
		string = comb_4str(player(index)->charname, "Deleted Shop #", int_to_string(n), ".");
		add_log(string, ADMIN_LOG);
	}

	clear_buffer(&buffer);
	add_opcode(&buffer,SREDITOR);
	send_data(&buffer, index);
}

void handle_save_item(buffer_t *data, int16 index)
{
	uint16 n;
	buffer_t buffer;
	char *string = NULL;

	if(player(index)->group < GROUP_DEVELOPER){
		hacking_attempt(index, "Admin Cloning");
		return;
	}

	take_buffer(&n,data,SIZE16);

	if(n == 0 || n > MAX_ITEMS)
		hacking_attempt(index, "Invalid Item Index"); return;

	take_string(item(n)->name, data);
	take_buffer(&item(n)->type, data, SIZE8);
	take_buffer(&item(n)->pic, data, SIZE16);
	take_buffer(&item(n)->data1, data, SIZE16);
	take_buffer(&item(n)->data2, data, SIZE16);
	take_buffer(&item(n)->data3, data, SIZE16);

	send_update_item_to_all(n);
	write_item(get_path(ITEM_PATH,n,FILE_ENDING),n);
	string = comb_4str(player(index)->charname, " Saving item #", int_to_string(n), ".");
	add_log(string, ADMIN_LOG);
}

void handle_request_edit_npc(buffer_t *data, int16 index)
{
	buffer_t buffer;

	if(player(index)->group < GROUP_DEVELOPER){
		hacking_attempt(index, "Admin Cloning");
		return;
	}

	clear_buffer(&buffer);
	add_opcode(&buffer, SNPCEDITOR);
	send_data(&buffer, index);
}

void handle_edit_npc(buffer_t *data, int16 index)
{
	uint16 n;
	buffer_t buffer;
	char *string = NULL;

	if(player(index)->group < GROUP_DEVELOPER){
		hacking_attempt(index, "Admin Cloning");
		return;
	}

	take_buffer(&n,data,SIZE16);

	if(n == 0 || n > MAX_NPCS)
		hacking_attempt(index, "Invalid Npc Index"); return;

	string = comb_4str(player(index)->charname, " editing Npc #", int_to_string(n), ".");
	add_log(string, ADMIN_LOG);
	send_edit_npc_to(index,n);
}

void handle_save_npc(buffer_t *data, int16 index)
{
	uint16 n;
	buffer_t buffer;
	char *string = NULL;

	if(player(index)->group < GROUP_DEVELOPER){
		hacking_attempt(index, "Admin Cloning");
		return;
	}

	take_buffer(&n,data,SIZE16);

	if(n == 0 || n > MAX_NPCS)
		hacking_attempt(index, "Invalid Npc Index"); return;

	take_string(npc(n)->name, data);
	take_string(npc(n)->say, data);
	take_buffer(&npc(n)->sprite, data, SIZE16);
	take_buffer(&npc(n)->behavior, data, SIZE8);
	take_buffer(&npc(n)->range, data, SIZE8);
	take_buffer(&npc(n)->dropitem, data, SIZE16);
	take_buffer(&npc(n)->dropchance, data, SIZE16);
	take_buffer(&npc(n)->dropitemvalue, data, SIZE16);
	take_buffer(&npc(n)->spawnsecs, data, SIZE32);
	take_buffer(&npc(n)->stat, data, STAT_COUNT * sizeof(uint8));

	send_update_npc_to_all(n);
	write_npc(get_path(NPC_PATH,n,FILE_ENDING),n);
	string = comb_4str(player(index)->charname, " Saving Npc #", int_to_string(n), ".");
	add_log(string, ADMIN_LOG);
}

void handle_request_edit_shop(buffer_t *data, int16 index)
{
	buffer_t buffer;

	if(player(index)->group < GROUP_DEVELOPER){
		hacking_attempt(index, "Admin Cloning");
		return;
	}

	clear_buffer(&buffer);
	add_opcode(&buffer, SSHOPEDITOR);
	send_data(&buffer, index);
}

void handle_edit_shop(buffer_t *data, int16 index)
{
	uint16 n;
	buffer_t buffer;
	char *string = NULL;

	if(player(index)->group < GROUP_DEVELOPER){
		hacking_attempt(index, "Admin Cloning");
		return;
	}

	take_buffer(&n,data,SIZE16);

	if(n == 0 || n > MAX_SHOPS)
		hacking_attempt(index, "Invalid Shop Index"); return;

	string = comb_4str(player(index)->charname, " editing Shop #", int_to_string(n), ".");
	add_log(string, ADMIN_LOG);
	send_edit_shop_to(index,n);
}

void handle_save_shop(buffer_t *data, int16 index)
{
	uint16 n;
	buffer_t buffer;
	char *string = NULL;

	if(player(index)->group < GROUP_DEVELOPER){
		hacking_attempt(index, "Admin Cloning");
		return;
	}

	take_buffer(&n,data,SIZE16);

	if(n == 0 || n > MAX_SHOPS)
		hacking_attempt(index, "Invalid Shop Index"); return;

	take_string(shop(n)->name, data);
	take_string(shop(n)->joinsay, data);
	take_string(shop(n)->leavesay, data);
	take_buffer(&shop(n)->fixesitems, data, SIZE8);
	take_buffer(shop(n)->tradeitem, data, MAX_TRADES * sizeof(trade_item_t));

	send_update_shop_to_all(n);
	write_shop(get_path(SHOP_PATH,n,FILE_ENDING),n);
	string = comb_4str(player(index)->charname, " Saving Shop #", int_to_string(n), ".");
	add_log(string, ADMIN_LOG);
}

void handle_request_edit_spell(buffer_t *data, int16 index)
{
	buffer_t buffer;

	if(player(index)->group < GROUP_DEVELOPER){
		hacking_attempt(index, "Admin Cloning");
		return;
	}

	clear_buffer(&buffer);
	add_opcode(&buffer, SSPELLEDITOR);
	send_data(&buffer, index);
}

void handle_edit_spell(buffer_t *data, int16 index)
{
	uint16 n;
	buffer_t buffer;
	char *string = NULL;

	if(player(index)->group < GROUP_DEVELOPER){
		hacking_attempt(index, "Admin Cloning");
		return;
	}

	take_buffer(&n,data,SIZE16);

	if(n == 0 || n > MAX_SPELLS)
		hacking_attempt(index, "Invalid Spell Index"); return;

	string = comb_4str(player(index)->charname, " editing Spell #", int_to_string(n), ".");
	add_log(string, ADMIN_LOG);
	send_edit_spell_to(index,n);
}

void handle_save_spell(buffer_t *data, int16 index)
{
	uint16 n;
	buffer_t buffer;
	char *string = NULL;

	if(player(index)->group < GROUP_DEVELOPER){
		hacking_attempt(index, "Admin Cloning");
		return;
	}

	take_buffer(&n,data,SIZE16);

	if(n == 0 || n > MAX_SPELLS)
		hacking_attempt(index, "Invalid Spell Index"); return;

	take_string(spell(n)->name,data);
	take_buffer(&spell(n)->pic, data, SIZE16);
	take_buffer(&spell(n)->mpreq, data, SIZE16);
	take_buffer(&spell(n)->jobreq, data, SIZE8);
	take_buffer(&spell(n)->levelreq, data, SIZE8);
	take_buffer(&spell(n)->type, data, SIZE8);
	take_buffer(&spell(n)->data1, data, SIZE16);
	take_buffer(&spell(n)->data2, data, SIZE16);
	take_buffer(&spell(n)->data3, data, SIZE16);

	send_update_spell_to_all(n);
	write_spell(get_path(SPELL_PATH,n,FILE_ENDING),n);
	string = comb_4str(player(index)->charname, " Saving Spell #", int_to_string(n), ".");
	add_log(string, ADMIN_LOG);
}

void handle_set_group(buffer_t *data, int16 index)
{
	uint16 n;
	uint8 i;
	char *name = NULL;
	char *string  = NULL;

	if(player(index)->group < GROUP_DEVELOPER){
		hacking_attempt(index, "Trying to use powers not available");
		return;
	}

	take_string(name, data);
	take_buffer(&i, data, SIZE8);
	n = find_player(name);

	if(i <= GROUP_CREATOR){
		if( n != -1){
			if(player(n)->group == player(index)->group)
				player_msg(index, "Invalid access level.",1); return; //red

			if(i > GROUP_USER)
				player_msg(n,"You have been blessed with powers.", 1); //bright blue

			player(n)->group = i;
			send_player_data(n);
			string = comb_4str(player(index)->charname, " has modified ", player(n)->charname, "'s group.");
			add_log(string, ADMIN_LOG);
		}
		else{
			player_msg(index,"Player is not online.",1); //white
		}
	}
	else{
		player_msg(index, "Invalid group.",1); //red
	}
}

void handle_who_is_online(buffer_t *data, int16 index)
{
	char *string = NULL;
	string = comb_3str("there is ", int_to_string(total_players_online())," players online");
	player_msg(index,string,1); //dark grey
}

void handle_trade(buffer_t *data, int16 index)
{
	if(map(player(index)->map)->shop > 0)
		send_trade(index, map(player(index)->map)->shop);
	else
		player_msg(index, "There is no shop here.", 1); //Bright Red
}

void handle_trade_request(buffer_t *data, int16 index)
{
	uint16 n;
	uint16 i;
	int8 x;

	take_buffer(&n, data, SIZE16);

	if(n == 0 || n > MAX_TRADES)
		hacking_attempt(index, "Trade request modification"); return;

	i = map(player(index)->map)->shop;

	if(x = find_open_inv_slot(index, shop(i)->tradeitem[n].getitem) == 0)
		player_msg(index, "Trade unsuccessful, inventory full.",1); return;//bright red

	if(has_item(index,shop(i)->tradeitem[n].giveitem) >= shop(i)->tradeitem[n].givevalue){
		take_item(index,shop(i)->tradeitem[n].giveitem,shop(i)->tradeitem[n].givevalue);
		give_item(index, shop(i)->tradeitem[n].getitem, shop(i)->tradeitem[n].getvalue);
		player_msg(index, "The trade was successful!",1); return;//yellow
	}
	else
		player_msg(index, "Trade unsuccessful.",1);//bright red
}

void handle_fix_item(buffer_t *data, int16 index)
{
	uint16 n, i, itemnum;
	uint32 durneeded, goldneeded, hasitem;
	char *string = NULL;

	take_buffer(&n,data,SIZE16);

	if(n == 0 || n > MAX_INV)
		hacking_attempt(index, "Fix Item Modification."); return;

	if(player(index)->inv[n].id == 0 || player(index)->inv[n].id >= MAX_ITEMS)
		return;
	itemnum = player(index)->inv[n].id;

	if(item(itemnum)->type < ITEM_TYPE_WEAPON || item(itemnum)->type > ITEM_TYPE_SHIELD)
		player_msg(index, "You can only fix weapons, armors, helmets, and shields.",1);  return;//bright red

	if(find_open_inv_slot(index,itemnum) <= 0)
		player_msg(index, "You have no inventory space left!",1);  return;//bright red

	i = (item(itemnum)->data2 /5);

	if(i == 0) i = 1;

	durneeded = item(itemnum)->data1 - player(index)->inv[n].dur;
	goldneeded =(durneeded * i /2);

	if(goldneeded == 0) goldneeded = 1;

	if(durneeded == 0)
		player_msg(index,"This item is in perfect condition!",1); return;//white

	if(hasitem = has_item(index,1)){
		if(hasitem >= goldneeded){
			take_item(index,1,goldneeded);
			player(index)->inv[n].dur = item(itemnum)->data1;
			string = comb_3str("Item has been totally restored for ",int_to_string(goldneeded), " gold!");
			player_msg(index,string,1); return;//bright blue
		}
		else{
			durneeded = (hasitem / i);
			goldneeded = (durneeded * i / 2);

			if(goldneeded == 0) goldneeded = 1;

			take_item(index,1, goldneeded);
			player(index)->inv[n].dur += durneeded;
			string = comb_3str("Item has been partially fixed for ",int_to_string(goldneeded), " gold!");
			player_msg(index,string,1); return;//bright blue
		}
	}
	else
		player_msg(index, "Insufficient gold to fix this item!", 1); //bright red
}

void handle_search(buffer_t *data, int16 index)
{
	uint8 x;
	uint8 y;
	int16 i = 0;
	char *string = NULL;

	take_buffer(&x, data, SIZE8);
	take_buffer(&y, data, SIZE8);

	if(x == 0 || x >= MAX_MAPX|| y == 0 || y >= MAX_MAPY)
		return;

	for(i = 0; i < total_players_online(); i++){
		if(player(index)->map == player(player_online(i))->map){
			if(player(player_online(i))->x == x && player(player_online(i))->y == y){
				if(player_online(i) != index){
					if( player(player_online(i))->level >= player(index)->level + 5)
						player_msg(index, "You wouldn't stand a chance.", 1); //bright red
					else{
						if(player(player_online(i))->level > player(index)->level)
							player_msg(index, "This one seems to have an advantage over you.", 1); //yellow
						else{
							if(player(player_online(i))->level == player(index)->level)
								player_msg(index, "This would be an even fight.", 1); //green
							else{
								if(player(player_online(i))->level + 5 <= player(index)->level)
									player_msg(index, "You could slaughter that player.", 1); //bright blue
								else{
									if(player(player_online(i))->level < player(index)->level)
										player_msg(index, "You would have an advantage over that player.", 1); //bright blue
									else{
									}
								}
							}
						}
					}

					temp_player(index)->target = player_online(i);
					temp_player(index)->targettype = TARGET_PLAYER;
					string = comb_3str("Your target is now ", player(player_online(i))->charname, ".");
					player_msg(index, string, 1); //yellow
					return;
				}
			}
		}
	}

	for(i = 0; i < MAX_MAP_ITEMS; i++){
		if(map(player(index)->map)->items[i].num > 0 && map(player(index)->map)->items[i].x == x && map(player(index)->map)->items[i].y == y){
			string = comb_3str("You see a ", item(map(player(index)->map)->items[i].num)->name, ".");
			player_msg(index, string, 1); //yellow
			return;
		}
	}

	for( i = 0; i < MAX_MAP_NPCS; i++){
		if(map(player(index)->map)->npc[i].num > 0 && map(player(index)->map)->npc[i].x == x && map(player(index)->map)->npc[i].y == y){
			temp_player(index)->target = i;
			temp_player(index)->targettype = TARGET_NPC;
			string = comb_3str("Your target is now a ", npc(map(player(index)->map)->npc[i].num)->name, ".");
			player_msg(index, string, 1); //yellow
			return;
		}
	}
}

void handle_spells(buffer_t *data, int16 index)
{
	send_players_spells(index);
}

void handle_cast(buffer_t *data, int16 index)
{
	uint16 n;
	take_buffer(&n,data, SIZE16);
	cast_spell(index,n);
}

void handle_quit(buffer_t *data, int16 index)
{
	left_game(index);

	clear_user_socket(get_temp_player_socket(index));
}