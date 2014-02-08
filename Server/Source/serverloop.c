/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/
#include "general.h"
#include "function.h"
#include "gamelogic.h"
#include "servertcp.h"
#include "path.h"
#include <stdio.h>
#include "serverloop.h"

sbool server_is_online = TRUE;
double  givenpchptimer;

uint8 secs = 0;

void server_loop(void)
{
	double  tick;
	double  tmr500 = 0;
	double  tmr1000 = 0;
	double  lastupdatesaveplayers = 0;
	double  lastupdatemapspawnitems = 0;
	double  lastupdateplayervitals = 0;

	while(server_is_online){
		tick = gettickcount();

		if(tick > tmr500){
			update_npc_ai();

			tmr500 = gettickcount() + .5;
		}

		if(tick > tmr1000){
			if(shutting_down())
				handle_shutdown();

			tmr1000 = gettickcount() + 1;
		}

		if(tick > lastupdateplayervitals){
			update_player_vitals();
			lastupdateplayervitals = gettickcount() + 5;
		}

		if(tick > lastupdatemapspawnitems){
			update_map_spawn_items();
			lastupdatemapspawnitems = gettickcount() + 30;
		}

		if(tick > lastupdatesaveplayers){
			update_save_players();
			lastupdatesaveplayers = gettickcount() + 60;
		}
	}
	return;
}

void update_map_spawn_items(void)
{
	uint8 x = 0;
	uint8 y = 0;

	for(y = 1; y < MAX_MAPS; y++){
		if(!players_on_map(y)){
			for( x = 0; x < MAX_MAP_ITEMS; x++){
				map(y)->items[x].num = 0;
				map(y)->items[x].dur = 0;
				map(y)->items[x].value = 0;
				map(y)->items[x].x = 0;
				map(y)->items[x].y = 0;
			}

			spawn_map_items(y);
			send_map_items_to_all(y);
		}
	}
}

void update_npc_ai(void)
{
	uint8 i = 0;
	uint32 mapnum = 1;
	uint8 mapnpcnum = 0;
	uint16 npcnum, target;
	double  tickcount;
	uint32 damage;
	char *string = NULL;

	for(mapnum = 1; mapnum < MAX_MAPS; mapnum++){
		if(players_on_map(mapnum)){
			tickcount = gettickcount();

			for(mapnpcnum = 0; mapnpcnum < MAX_MAP_NPCS; mapnpcnum++){
				npcnum = map(mapnum)->npc[mapnpcnum].num;

				if(npcnum > 0){
					target = map(mapnum)->npc[mapnpcnum].target;

					ai_attack_on_sight(mapnpcnum, target, mapnum);

					if(npc(npcnum)->behavior != BEHAVIOR_SHOPKEEPER){
						if(target > 0){
							if(temp_player(target)->loggedin && player(target)->map == mapnum){
								ai_npc_collision(ai_npc_move(mapnum, mapnpcnum, target), mapnum, mapnpcnum, target);

								if(can_npc_attack_player(mapnpcnum,target)){
									if(!can_player_block_hit(target)){
										damage = npc(npcnum)->stat[STAT_STRENGTH] - get_player_protection(target);
										npc_attack_player(mapnpcnum, target, damage);
									}
									else{
										string = comb_4str(item(player(target)->inv[player(target)->equipment[EQUIPMENT_SHIELD]].id)->name, " blocks the ", npc(npcnum)->name,"'s hit!");
										player_msg(target, string, 1); //bright cyan
									}
								}
								else
									map(mapnum)->npc[mapnpcnum].target = 0;
							}
							else
								map(mapnum)->npc[mapnpcnum].target = 0;
						}
					}
					else{
						if((rand() % 4) == 1){
							i = rand() % 4;

							if(can_npc_move(mapnum,mapnpcnum, i))
								npc_move(mapnum,mapnpcnum,i);
						}
					}

					ai_hp_regen(tickcount, mapnum, mapnpcnum);

					if(tickcount > map(mapnum)->npc[mapnpcnum].spawnwait + npc(map(mapnum)->npc[mapnpcnum].num)->spawnsecs * 1000)
						spawn_npc(mapnpcnum, mapnum);
				}
			}
		}
	}
}

void ai_hp_regen(double  tickcount, uint32  mapnum, uint8 mapnpcnum)
{
	if(tickcount > givenpchptimer + 10000){
		if(map(mapnum)->npc[mapnpcnum].vitals[VITAL_HP]){
			map(mapnum)->npc[mapnpcnum].vitals[VITAL_HP] += get_npc_vital_regen(map(mapnum)->npc[mapnpcnum].num, VITAL_HP);

			if(map(mapnum)->npc[mapnpcnum].vitals[VITAL_HP] > get_npc_max_vital(map(mapnum)->npc[mapnpcnum].num,VITAL_HP)){
				map(mapnum)->npc[mapnpcnum].vitals[VITAL_HP] = get_npc_max_vital(map(mapnum)->npc[mapnpcnum].num,VITAL_HP);
			}
		}
		givenpchptimer = tickcount;
	}
}

void ai_attack_on_sight(uint8 mapnpcnum, uint16 target, uint32 mapnum)
{
	uint16 i = 0;
	uint16 n;
	int8 dx;
	int8 dy;
	char *string = NULL;

	if(npc(map(mapnum)->npc[mapnpcnum].num)->behavior == BEHAVIOR_ATTACKONSIGHT || npc(map(mapnum)->npc[mapnpcnum].num)->behavior == BEHAVIOR_GUARD){
		if(map(mapnum)->npc[mapnpcnum].targettype == TARGET_NONE){
			for(i = 0; i < total_players_online(); i++){
				if(player(player_online(i))->map == mapnum && player(player_online(i))->group <= GROUP_MONITOR){
					n = npc(map(mapnum)->npc[mapnpcnum].num)->range;

					dx = map(mapnum)->npc[mapnpcnum].x - player(player_online(i))->x;
					dy = map(mapnum)->npc[mapnpcnum].y - player(player_online(i))->y;

					if(dx < 0)
						dx *= -1;
					if(dy < 0)
						dy *= -1;

					if(dx <= n && dy <= n){
						if(npc(map(mapnum)->npc[mapnpcnum].num)->behavior == BEHAVIOR_ATTACKONSIGHT || player(player_online(i))->pk){
							if(strlen(npc(map(mapnum)->npc[mapnpcnum].num)->say)){
								string = comb_5str("A ", npc(map(mapnum)->npc[mapnpcnum].num)->name," says, '",npc(map(mapnum)->npc[mapnpcnum].num)->say,"' to you.");
								player_msg(player_online(i),string, 1); //grey
							}
							map(mapnum)->npc[mapnpcnum].target = player_online(i);
							map(mapnum)->npc[mapnpcnum].targettype = TARGET_PLAYER;
						}
					}
				}
			}
		}
	}
}

sbool ai_npc_move(uint32 mapnum, uint8 mapnpcnum, uint16 target)
{
	switch(rand() % 4){
	case 0:
		if(map(mapnum)->npc[mapnpcnum].y > player(target)->y && !ai_npc_move(mapnum,mapnpcnum,target)){
			if(can_npc_move(mapnum, mapnpcnum,DIR_UP)){
				npc_move(mapnum, mapnpcnum, DIR_UP);
				return TRUE;
			}
			break;
		}
	case 1:
		if(map(mapnum)->npc[mapnpcnum].y < player(target)->y && !ai_npc_move(mapnum,mapnpcnum,target)){
			if(can_npc_move(mapnum, mapnpcnum,DIR_DOWN)){
				npc_move(mapnum, mapnpcnum, DIR_DOWN);
				return TRUE;
			}
		}
		break;
	case 2:
		if(map(mapnum)->npc[mapnpcnum].x > player(target)->x && !ai_npc_move(mapnum,mapnpcnum,target)){
			if(can_npc_move(mapnum, mapnpcnum,DIR_LEFT)){
				npc_move(mapnum, mapnpcnum, DIR_LEFT);
				return TRUE;
			}
		}
		break;
	case 3:
		if(map(mapnum)->npc[mapnpcnum].x < player(target)->x && !ai_npc_move(mapnum,mapnpcnum,target)){
			if(can_npc_move(mapnum, mapnpcnum,DIR_RIGHT)){
				npc_move(mapnum, mapnpcnum, DIR_RIGHT);
				return TRUE;
			}
		}
		break;
	}
	return FALSE;
}

void ai_npc_collision(sbool didwalk, uint32 mapnum , uint8 mapnpcnum, uint16 target)
{
	uint8 i;

	if(didwalk)
		return;

	if(!didwalk){
		if(map(mapnum)->npc[mapnpcnum].x - 1 == player(target)->x && map(mapnum)->npc[mapnpcnum].y == player(target)->y)
			if(map(mapnum)->npc[mapnpcnum].dir != DIR_LEFT) npc_dir(mapnum, mapnpcnum, DIR_LEFT); didwalk = TRUE;
	}

	if(!didwalk){
		if(map(mapnum)->npc[mapnpcnum].x + 1 == player(target)->x && map(mapnum)->npc[mapnpcnum].y == player(target)->y)
			if(map(mapnum)->npc[mapnpcnum].dir != DIR_RIGHT) npc_dir(mapnum, mapnpcnum, DIR_RIGHT); didwalk = TRUE;
	}

	if(!didwalk){
		if(map(mapnum)->npc[mapnpcnum].x == player(target)->x && map(mapnum)->npc[mapnpcnum].y - 1 == player(target)->y)
			if(map(mapnum)->npc[mapnpcnum].dir != DIR_UP) npc_dir(mapnum, mapnpcnum, DIR_UP); didwalk = TRUE;
	}

	if(!didwalk){
		if(map(mapnum)->npc[mapnpcnum].x == player(target)->x && map(mapnum)->npc[mapnpcnum].y + 1 == player(target)->y)
			if(map(mapnum)->npc[mapnpcnum].dir != DIR_DOWN) npc_dir(mapnum, mapnpcnum, DIR_DOWN); didwalk = TRUE;
	}

	if(!didwalk){
		if((rand() % 2) == 1){
			i = rand() % 4;
			if(can_npc_move(mapnum, mapnpcnum, i))
				npc_move(mapnum, mapnpcnum, i);
		}
	}
}

void update_player_vitals(void)
{
	uint32 i = 0;

	for(i = 0; i < total_players_online(); i++){
		if(player(player_online(i))->vitals[VITAL_HP] != get_player_max_vital(player_online(i),VITAL_HP)){
			player(player_online(i))->vitals[VITAL_HP] += get_player_vital_regen(player_online(i),VITAL_HP);
			send_vital(player_online(i),VITAL_HP);
		}

		if(player(player_online(i))->vitals[VITAL_MP] != get_player_max_vital(player_online(i),VITAL_MP)){
			player(player_online(i))->vitals[VITAL_MP] += get_player_vital_regen(player_online(i),VITAL_MP);
			send_vital(player_online(i),VITAL_MP);
		}

		if(player(player_online(i))->vitals[VITAL_SP] != get_player_max_vital(player_online(i),VITAL_SP)){
			player(player_online(i))->vitals[VITAL_HP] += get_player_vital_regen(player_online(i),VITAL_SP);
			send_vital(player_online(i),VITAL_SP);
		}
	}
}

void update_save_players(void)
{
	uint32 i = 0;

	if(total_players_online()){
		printf("Saving all online players.\n");
		admin_msg("Saving all online players.",1); //pink
		save_online_players();
	}
}


void set_server_offline(void)
{
	server_is_online = FALSE;
	destroy_server();
}

void handle_shutdown(void)
{
	char *string = NULL;

	if(secs <= 0){
		secs = 30;
	}

	if( (secs % 5) == 0 || secs <= 5){
		string = comb_3str("Server Shutdown in ", int_to_string(secs)," seconds.");
		global_msg(string, 1); //bright blue
		printf("Automated Server Shutdown in  %i seconds. \n", secs);
	}
	secs--;

	if(secs <= 0){
		global_msg("Server Shutting down. \n", 1); //bright red
		server_is_online = FALSE;
		destroy_server();
	}
}