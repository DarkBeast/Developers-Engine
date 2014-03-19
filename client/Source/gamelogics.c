#include "gamelogics.h"
#include "general.h"
#include "function.h"
#include "socket.h"
#include "packetenum.h"
#include "clienttcp.h"
#include <stdio.h>

void GameLoop(void)
{
	uint32 i = 0;
	double frametime = 0;
	double tickfps = 0;
	uint32 fps = 0;
	double tick = 0;
	double walktimer = 0;
	double tmr25 = 0;
	double tmr10000 = 0;
	double elapsedtime = 0;
	//*** Start GameLoop ***
	while(tempdata()->ingame)
	{
		tick = gettickcount(); //Set the initial tick
		elapsedtime = tick - frametime;  //Set the time difference for time-based movement
		frametime = tick;               //Set the time second loop time to the first.

		if(tmr25 < tick){
			tempdata()->ingame = isconnected();

			//Call CheckInputKeys //Check which keys were pressed
			if(tempdata()->canmovenow){
				check_movement(); //Check if player is trying to move
				check_attack();  //Check to see if player is trying to attack
			}

			//Change map animation every 250 milliseconds
			if(tempdata()->mapanimtimer < tick){
				if(tempdata()->mapanim)
					tempdata()->mapanim = FALSE;
				else
					tempdata()->mapanim = TRUE;
				tempdata()->mapanimtimer = tick + 250;
			}

			tmr25 = tick + 25;
		}

		//Process movements (actually move them)
		if(walktimer < tick){
			for( i = 0; i < players_on_map(); i++)
				process_movement(player_on_map(i));

			for( i = 0; i < MAX_MAP_NPCS; i++){
				if(map()->npc[i].num){
					if(map()->npc[i].moving)
						process_npc_movement(i);
				}
			}

			walktimer = tick + 30;
		}

		//*********************
		//** Render Graphics **
		//*********************
		//Call Render_Graphics
	}

	//frmMirage.Visible = False

	//If isLogging Then
	//frmMirage.txtChat = vbNullString
	//frmMirage.txtMyChat = vbNullString
	//isLogging = False
	//frmMainMenu.Visible = True
	//GettingMap = True
	//Else
	//Shutdown the game
	//frmSendGetData.Visible = True
	//Call SetStatus("Destroying game data...")
	destroy_client();
	//End If
}

void process_movement(uint16 index)
{
	uint8 movementspeed;

	//Check if player is walking, and if so process moving them over
	switch(player(index)->moving){
	case MOVEMENT_NONE: return;
	case MOVEMENT_WALKING: movementspeed = WALK_SPEED; break;
	case MOVEMENT_RUNNING: movementspeed = RUN_SPEED; break;
	}

	switch(player(index)->dir){
	case DIR_UP: player(index)->offsety -= movementspeed; break;
	case DIR_DOWN: player(index)->offsety += movementspeed; break;
	case DIR_LEFT: player(index)->offsetx -= movementspeed; break;
	case DIR_RIGHT: player(index)->offsetx += movementspeed; break;
	}

	//Check if completed walking over to the next tile
	if(player(index)->offsetx == 0 && player(index)->offsety == 0)
		player(index)->moving = MOVEMENT_NONE;
}

void process_npc_movement(uint8 mapnpcnum)
{
	//Check if NPC is walking, and if so process moving them over

	if(map()->npc[mapnpcnum].moving == MOVEMENT_NONE)
		return;

	switch(map()->npc[mapnpcnum].dir){
	case DIR_UP: map()->npc[mapnpcnum].offsety -= WALK_SPEED; break;
	case DIR_DOWN: map()->npc[mapnpcnum].offsety += WALK_SPEED; break;
	case DIR_LEFT: map()->npc[mapnpcnum].offsetx -= WALK_SPEED; break;
	case DIR_RIGHT: map()->npc[mapnpcnum].offsetx += WALK_SPEED; break;
	}

	//Check if completed walking over to the next tile
	if(map()->npc[mapnpcnum].offsetx == 0 && map()->npc[mapnpcnum].offsety == 0)
		map()->npc[mapnpcnum].moving = MOVEMENT_NONE;
}

sbool is_trying_to_move(void)
{
	if(tempdata()->dir)
		return TRUE;
	else
		return FALSE;
}

void check_attack(void)
{
	buffer_t buffer;
	if(tempdata()->control){
		if(player(myindex())->attacktimer + 1000 < gettickcount()){
			if(player(myindex())->attacking){
				player(myindex())->attacking = 1;
				player(myindex())->attacktimer = gettickcount();

				clear_buffer(&buffer);
				add_opcode(&buffer, CATTACK);
				socketsend(&buffer);
			}
		}
	}
}

sbool can_move(void)
{
	uint32 d;
	sbool ismap = FALSE;

	if(player(myindex())->moving != 0)
		return FALSE;

	if(tempdata()->castedspell){
		if(gettickcount() > player(myindex())->attacktimer + 1000)
			tempdata()->castedspell = FALSE;
		else
			return FALSE;
	}

	d = player(myindex())->dir;

	switch(tempdata()->dir){
	case DIR_NONE: return FALSE;
	case DIR_UP:
		player(myindex())->dir = DIR_UP;

		if(player(myindex())->y <= 0){
			if(map()->up) ismap = TRUE;
		}
		break;
	case DIR_DOWN:
		player(myindex())->dir = DIR_DOWN;

		if(player(myindex())->y >= MAX_MAPY - 1){
			if(map()->down) ismap = TRUE;
		}
		break;
	case DIR_LEFT:
		player(myindex())->dir = DIR_LEFT;

		if(player(myindex())->x <= 0){
			if(map()->left) ismap = TRUE;
		}
		break;

	case DIR_RIGHT:
		player(myindex())->dir = DIR_RIGHT;

		if(player(myindex())->x >= MAX_MAPX - 1){
			if(map()->right) ismap = TRUE;
		}
		break;
	}

	if(ismap){
		//map_editor_leave_map();
		request_new_map();
		tempdata()->gettingmap = TRUE;
		tempdata()->canmovenow = FALSE;
		return FALSE;
	}

	if(check_direction(player(myindex())->dir)){
		if(d != player(myindex())->dir)
			send_player_dir();

		return FALSE;
	}

	return TRUE;
}

sbool check_direction(uint8 dir)
{
	uint32 x = player(myindex())->x;
	uint32 y = player(myindex())->y;
	uint32 i = 0;

	switch(dir){
	case DIR_UP: y--; break;
	case DIR_DOWN: y++; break;
	case DIR_LEFT: x--; break;
	case DIR_RIGHT: x++; break;
	}

	//Check to see if the map tile is blocked or not
	if(map()->tile[GETXY(x,y)].type == TILE_TYPE_BLOCKED)
		return TRUE;

	//Check to see if a player is already on that tile
	for(i = 0; i < players_on_map(); i++){
		if(player(player_on_map(i))->x == x && player(player_on_map(i))->y == y)
			return TRUE;
	}

	//Check to see if a npc is already on that tile
	for(i = 0; i < MAX_MAP_NPCS; i++){
		if(map()->npc[i].num){
			if(map()->npc[i].x == x && map()->npc[i].y == y)
				return TRUE;
		}
	}

	return FALSE;
}

void check_movement(void)
{
	if(is_trying_to_move()){
		if(can_move()){
			//Check if player has the shift key down for running
			if(tempdata()->shift)
				player(myindex())->moving = MOVEMENT_RUNNING;
			else
				player(myindex())->moving = MOVEMENT_WALKING;

			switch(player(myindex())->dir){
			case DIR_UP:
				send_player_move();
				player(myindex())->offsety = PIC_SIZE;
				player(myindex())->y--;
				break;
			case DIR_DOWN:
				send_player_move();
				player(myindex())->offsety = PIC_SIZE * -1;
				player(myindex())->y++;
				break;
			case DIR_LEFT:
				send_player_move();
				player(myindex())->offsetx = PIC_SIZE;
				player(myindex())->x--;
				break;
			case DIR_RIGHT:
				send_player_move();
				player(myindex())->offsetx = PIC_SIZE * -1;
				player(myindex())->x++;
				break;
			}

			if(player(myindex())->offsetx == 0 && player(myindex())->offsety == 0){
				if(map()->tile[GETXY(player(myindex())->x,player(myindex())->y)].type == TILE_TYPE_WARP){
					tempdata()->gettingmap = TRUE;
					tempdata()->canmovenow = FALSE;
				}
			}
		}
	}
}

void update_inventory(void)
{
	uint32 i = 0;

	//frmMirage.lstInv.Clear

	//Show the inventory
	for(i = 1; i < MAX_INV; i++){
		if(player(myindex())->inv[i].id && player(myindex())->inv[i].id < MAX_ITEMS){
			if(item(player(myindex())->inv[i].id)->type == ITEM_TYPE_CURRENCY){
				//frmMirage.lstInv.AddItem i & ": " & Trim$(Item(GetPlayerInvItemNum(i)).Name) & " (" & GetPlayerInvItemValue(i) & ")"
			}
			else{
				//Check if this item is being worn
				uint8 x = 0;
				for(x = 0; x < EQUIPMENT_COUNT; x++){
					if(player(myindex())->equipment[x] == i){
						//frmMirage.lstInv.AddItem i & ": " & Trim$(Item(GetPlayerInvItemNum(i)).Name) & " (worn)"
					}
					else{
						//frmMirage.lstInv.AddItem i & ": " & Trim$(Item(GetPlayerInvItemNum(i)).Name)
					}
				}
			}
		}
		else{
			//frmMirage.lstInv.AddItem "<free inventory slot>"
		}
	}

	//frmMirage.lstInv.ListIndex = 0
}

void player_search(uint16 curx, uint16 cury)
{
	buffer_t buffer;

	if(is_in_bounds()){
		clear_buffer(&buffer);
		add_opcode(&buffer, CSEARCH);
		add_buffer(&buffer, &tempdata()->curx, SIZE8);
		add_buffer(&buffer, &tempdata()->cury, SIZE8);
		socketsend(&buffer);
	}
}

sbool is_in_bounds(void)
{
	if(tempdata()->curx < MAX_MAPX && tempdata()->cury < MAX_MAPY)
		return TRUE;

	return FALSE;
}

void use_item(void)
{
	//Check for subscript out of range
	if(!tempdata()->inv_item_selected || tempdata()->inv_item_selected >= MAX_INV)
		return;

	send_use_item(tempdata()->inv_item_selected);
}

void cast_spell(void)
{
	buffer_t buffer;

	//Check for subscript out of range
	if(!tempdata()->spell_selected || tempdata()->spell_selected >= MAX_PLAYER_SPELLS)
		return;

	//Check if player has enough MP
	if(player(myindex())->spells[tempdata()->spell_selected]){
		if(player(myindex())->vitals[VITAL_MP] < spell(player(myindex())->spells[tempdata()->spell_selected])->mpreq){
			//	Call AddText("Not enough MP to cast " & Trim$(Spell(SpellSelected).Name) & ".", 12)
			return;
		}
		if(gettickcount() > player(myindex())->attacktimer + 1000){
			if(player(myindex())->moving == 0){
				clear_buffer(&buffer);
				add_opcode(&buffer,CCAST);
				add_buffer(&buffer,&tempdata()->spell_selected, SIZE16);
				socketsend(&buffer);
				player(myindex())->attacking = TRUE;
				player(myindex())->attacktimer = gettickcount();
				tempdata()->castedspell = TRUE;
			}
			else{
				//Call AddText("Cannot cast while walking!", 12)
			}
		}
	}
	else{
		//Call AddText("No spell here.", 12)
	}
}

/*void calc_tile_positions(void)
{
uint32 x = 0;
uint32 y = 0;

For X = 0 To MAX_MAPX
For Y = 0 To MAX_MAPY
MapTilePosition(X, Y).PosX = X * PIC_SIZE
MapTilePosition(X, Y).PosY = Y * PIC_SIZE

MapTilePosition(X, Y).Ground.Top = (Map.Tile(X, Y).Ground \ TILESHEET_WIDTH) * PIC_SIZE
MapTilePosition(X, Y).Ground.Bottom = MapTilePosition(X, Y).Ground.Top + PIC_SIZE
MapTilePosition(X, Y).Ground.Left = (Map.Tile(X, Y).Ground Mod TILESHEET_WIDTH) * PIC_SIZE
MapTilePosition(X, Y).Ground.Right = MapTilePosition(X, Y).Ground.Left + PIC_SIZE

MapTilePosition(X, Y).Mask.Top = (Map.Tile(X, Y).Mask \ TILESHEET_WIDTH) * PIC_SIZE
MapTilePosition(X, Y).Mask.Bottom = MapTilePosition(X, Y).Mask.Top + PIC_SIZE
MapTilePosition(X, Y).Mask.Left = (Map.Tile(X, Y).Mask Mod TILESHEET_WIDTH) * PIC_SIZE
MapTilePosition(X, Y).Mask.Right = MapTilePosition(X, Y).Mask.Left + PIC_SIZE

MapTilePosition(X, Y).Anim.Top = (Map.Tile(X, Y).Anim \ TILESHEET_WIDTH) * PIC_SIZE
MapTilePosition(X, Y).Anim.Bottom = MapTilePosition(X, Y).Anim.Top + PIC_SIZE
MapTilePosition(X, Y).Anim.Left = (Map.Tile(X, Y).Anim Mod TILESHEET_WIDTH) * PIC_SIZE
MapTilePosition(X, Y).Anim.Right = MapTilePosition(X, Y).Anim.Left + PIC_SIZE

MapTilePosition(X, Y).Fringe.Top = (Map.Tile(X, Y).Fringe \ TILESHEET_WIDTH) * PIC_SIZE
MapTilePosition(X, Y).Fringe.Bottom = MapTilePosition(X, Y).Fringe.Top + PIC_SIZE
MapTilePosition(X, Y).Fringe.Left = (Map.Tile(X, Y).Fringe Mod TILESHEET_WIDTH) * PIC_SIZE
MapTilePosition(X, Y).Fringe.Right = MapTilePosition(X, Y).Fringe.Left + PIC_SIZE
Next
Next
}*/

void init_map_data(void)
{
	uint32 i = 0;

	//MusicFile = Trim$(CStr(Map.Music)) & ".mid"

	//Play music
	if(map()->music){
		//If Map.Music > 0 Then
		//If MusicFile <> CurrentMusic Then
		//DirectMusic_StopMidi
		//Call DirectMusic_PlayMidi(MusicFile)
		//CurrentMusic = MusicFile
	}
	else{
		//DirectMusic_StopMidi
		//CurrentMusic = 0
	}

	//	Call UpdateDrawMapName

	if(map()->shop){
		//frmMirage.picTradeButton.Visible = False
	}
	else{
		//frmMirage.picTradeButton.Visible = True
	}

	//Call CalcTilePositions

	//Call InitTileSurf(Map.TileSet)
}

void dev_msg(char *text, uint8 color)
{
	if(tempdata()->ingame){
		if(player(myindex())->group >= GROUP_DEVELOPER){
			//Call AddText(Text, Color)
			printf("%s", text);
		}
	}
}