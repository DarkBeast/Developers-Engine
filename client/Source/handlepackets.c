/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
*           Jacob/Dugar
******************************************************************************/
#include "handlepackets.h"
#include <stdio.h>
#include "globals.h"
#include "clienttcp.h"
#include <stdlib.h>
#include "general.h"
#include "function.h"
#include "path.h"
#include "newaccount.h"

void(*packets[SMSG_COUNT])(buffer_t *);

void init_packets(void)
{
	packets[SLOGINOK] = handle_loginok;
	packets[SALERTMSG] = &handle_alert_message;
	packets[SINGAME] = &handle_in_game;
	packets[SPLAYERINV] = &handle_player_inv;
	packets[SPLAYERINVUPDATE] = &handle_player_inv_update;
	packets[SPLAYERWORNEQ] = &handle_player_worn_eq;
	packets[SPLAYERHP] = &handle_player_hp;
	packets[SPLAYERMP] = &handle_player_mp;
	packets[SPLAYERSP] = &handle_player_sp;
	packets[SPLAYERSTATS] = &handle_player_stats;
	packets[SPLAYERDATA] = &handle_player_data;
	packets[SPLAYERMOVE] = &handle_player_move;
	packets[SNPCMOVE] = &handle_npc_move;
	packets[SPLAYERDIR] = &handle_player_dir;
	packets[SNPCDIR] = &handle_npc_dir;
	packets[SPLAYERXY] = &handle_player_xy;
	packets[SATTACK] = &handle_attack;
	packets[SNPCATTACK] = &handle_npc_attack;
	packets[SCHECKFORMAP] = &handle_check_for_map;
	packets[SMAPDATA] = &handle_map_data;
	packets[SMAPITEMDATA] = &handle_map_item_data;
	packets[SMAPNPCDATA] = &handle_map_npc_data;
	packets[SMAPDONE] = &handle_map_done;
	packets[SGLOBALMSG] = &handle_global_msg;
	packets[SADMINMSG] = &handle_admin_msg;
	packets[SPLAYERMSG] = &handle_player_msg;
	packets[SMAPMSG] = &handle_map_msg;
	packets[SSPAWNITEM] = &handle_spawn_item;
	packets[SITEMEDITOR] = &handle_item_editor;
	packets[SUPDATEITEM] = &handle_update_item;
	packets[SEDITITEM] = &handle_edit_item;
	packets[SREDITOR] = &handle_refreash;
	packets[SSPAWNNPC] = &handle_spawn_npc;
	packets[SNPCDEAD] = &handle_npc_dead;
	packets[SNPCEDITOR] = &handle_npc_editor;
	packets[SUPDATENPC] = &handle_update_npc;
	packets[SEDITNPC] = &handle_edit_npc;
	packets[SEDITMAP] = &handle_edit_map;
	packets[SSHOPEDITOR] = &handle_shop_editor;
	packets[SUPDATESHOP] = &handle_update_shop;
	packets[SEDITSHOP] = &handle_edit_shop;
	packets[SSPELLEDITOR] = &handle_spell_editor;
	packets[SUPDATESPELL] = &handle_update_spell;
	packets[SEDITSPELL] = &handle_edit_spell;
	packets[STRADE] = &handle_trade;
	packets[SSPELLS] = &handle_spells;
	packets[SLEFT] = &handle_left;
	packets[SCASTSPELL] = &handle_spell_cast;
	packets[SHIGHINDEX] = &handle_high_index;
}

void incomming_packets(buffer_t *data)
{
	uint8 id = SNONE;

	if(data->offset > 0){
		error_handler(DE_ERROR_INVALID_PACKET_OFFSET);
		return;
	}

	take_buffer(&id,data,1);

	if(id == SNONE){
		error_handler(DE_ERROR_INVALID_PACKET_ID);
		return;
	}

	packets[id](data);
}

void handle_loginok(buffer_t *data)
{
	int16 index;

	take_buffer(&index,data,SIZE16);
	set_myindex(index);

	//frmSendGetData.Visible = True
	//frmChars.Visible = False
}

void handle_alert_message(buffer_t *data)
{
	char *string;
	uint32 size;
	widget *statusbox1 = (widget *)calloc(1,sizeof(widget));

	take_buffer(&size,data,SIZE32);
	string = (char *)calloc(size,sizeof(char));
	take_string(string,data);

	printf("%s \n",string);
	
	switch(get_menu_state()){
		case MENU_STATE_MAIN:
			break;
		case MENU_STATE_CREDITS:
			break;
		case MENU_STATE_LOGIN:
			break;
		case MENU_STATE_CREATE:
			//newacc_status_box_open(string);
			break;
		case MENU_STATE_EXIT:
			break;
		}
}

void handle_in_game(buffer_t *data)
{
	tempdata()->ingame = TRUE;
	//game_init();
	//game_loop();
}

void handle_player_inv(buffer_t *data)
{
	int16 i = 0;

	for(i = 1; i < MAX_INV; i++){
		take_buffer(&player(myindex())->inv[i].id,data,SIZE16);
		take_buffer(&player(myindex())->inv[i].value,data,SIZE32);
		take_buffer(&player(myindex())->inv[i].dur,data,SIZE16);
	}
	//update_inventory();
}

void handle_player_inv_update(buffer_t *data)
{
	int16 n;

	take_buffer(&n,data,SIZE16);

	if(n == 0)
		return;

	take_buffer(&player(myindex())->inv[n].id,data,SIZE16);
	take_buffer(&player(myindex())->inv[n].value,data,SIZE32);
	take_buffer(&player(myindex())->inv[n].dur,data,SIZE16);

	//update_inventory();
}

void handle_player_worn_eq(buffer_t *data)
{
	int16 i = 0;

	for(i = 0; i < EQUIPMENT_COUNT; i++){
		take_buffer(&player(myindex())->equipment[i],data,SIZE16);
	}
	// update_inventory();
}

void handle_player_hp(buffer_t *data)
{
	take_buffer(&player(myindex())->maxvitals[VITAL_HP], data, SIZE16);
	take_buffer(&player(myindex())->vitals[VITAL_HP], data, SIZE16);

	if(player(myindex())->maxvitals[VITAL_HP] > 0)
		return;//frmMirage.lblHP.Caption = Int(GetPlayerVital(MyIndex, Vitals.HP) / GetPlayerMaxVital(MyIndex, Vitals.HP) * 100) & "%"
}

void handle_player_mp(buffer_t *data)
{
	take_buffer(&player(myindex())->maxvitals[VITAL_MP], data, SIZE16);
	take_buffer(&player(myindex())->vitals[VITAL_MP], data, SIZE16);

	if(player(myindex())->maxvitals[VITAL_MP] > 0)
		return;//frmMirage.lblMP.Caption = Int(GetPlayerVital(MyIndex, Vitals.MP) / GetPlayerMaxVital(MyIndex, Vitals.MP) * 100) & "%"
}

void handle_player_sp(buffer_t *data)
{
	take_buffer(&player(myindex())->maxvitals[VITAL_SP], data, SIZE16);
	take_buffer(&player(myindex())->vitals[VITAL_SP], data, SIZE16);

	if(player(myindex())->maxvitals[VITAL_SP] > 0)
		return;//frmMirage.lblSP.Caption = Int(GetPlayerVital(MyIndex, Vitals.SP) / GetPlayerMaxVital(MyIndex, Vitals.SP) * 100) & "%"
}

void handle_player_stats(buffer_t *data)
{
	int16 i = 0;

	for(i = 0; i < STAT_COUNT; i++){
		take_buffer(&player(myindex())->stat[i], data, SIZE8);
	}
}

void handle_player_data(buffer_t *data)
{
	int16 i;
	take_buffer(&i, data, SIZE16);

	take_string(player(i)->charname,data);
	take_buffer(&player(i)->sprite, data, SIZE16);
	take_buffer(&player(i)->map, data, SIZE16);
	take_buffer(&player(i)->x, data, SIZE8);
	take_buffer(&player(i)->y, data, SIZE8);
	take_buffer(&player(i)->dir, data, SIZE8);
	take_buffer(&player(i)->job, data, SIZE8);
	take_buffer(&player(i)->pk, data, SIZE8);

	//Check if the player is the client player, and if so reset directions
	if(i == myindex()){
		tempdata()->dir = DIR_NONE;
	}

	//Make sure they aren't walking
	player(i)->moving = MOVEMENT_NONE;
	player(i)->offsetx = 0;
	player(i)->offsety = 0;

	get_players_on_map();
}

void handle_player_move(buffer_t *data)
{
	int16 i;

	take_buffer(&i, data, SIZE16);
	take_buffer(&player(i)->x, data, SIZE8);
	take_buffer(&player(i)->y, data, SIZE8);
	take_buffer(&player(i)->dir, data, SIZE8);
	take_buffer(&player(i)->moving, data, SIZE8);
	player(i)->offsetx = 0;
	player(i)->offsety = 0;

	switch(player(i)->dir){
	case DIR_UP: player(i)->offsety = PIC_SIZE; break;
	case DIR_DOWN: player(i)->offsety = PIC_SIZE * -1; break;
	case DIR_LEFT: player(i)->offsetx = PIC_SIZE; break;
	case DIR_RIGHT: player(i)->offsetx = PIC_SIZE * -1; break;
	}
}

void handle_npc_move(buffer_t *data)
{
	uint8 mapnpcnum;

	take_buffer(&mapnpcnum, data, SIZE8);
	take_buffer(&map()->npc[mapnpcnum].x, data, SIZE8);
	take_buffer(&map()->npc[mapnpcnum].y, data, SIZE8);
	take_buffer(&map()->npc[mapnpcnum].dir, data, SIZE8);

	map()->npc[mapnpcnum].offsetx = 0;
	map()->npc[mapnpcnum].offsety = 0;
	map()->npc[mapnpcnum].moving = MOVEMENT_WALKING;

	switch(map()->npc[mapnpcnum].dir){
	case DIR_UP: map()->npc[mapnpcnum].offsety = PIC_SIZE; break;
	case DIR_DOWN: map()->npc[mapnpcnum].offsety = PIC_SIZE * -1; break;
	case DIR_LEFT: map()->npc[mapnpcnum].offsetx = PIC_SIZE; break;
	case DIR_RIGHT: map()->npc[mapnpcnum].offsetx = PIC_SIZE * -1; break;
	}
}

void handle_player_dir(buffer_t *data)
{
	int16 i;

	take_buffer(&i, data, SIZE16);
	take_buffer(&player(i)->dir, data, SIZE8);

	player(i)->moving = MOVEMENT_NONE;
	player(i)->offsetx = 0;
	player(i)->offsety = 0;
}

void handle_npc_dir(buffer_t *data)
{
	uint8 i;

	take_buffer(&i, data, SIZE8);
	take_buffer(&map()->npc[i].dir, data, SIZE8);

	map()->npc[i].offsetx = 0;
	map()->npc[i].offsety = 0;
	map()->npc[i].moving = MOVEMENT_NONE;
}

void handle_player_xy(buffer_t *data)
{
	take_buffer(&player(myindex())->x, data, SIZE8);
	take_buffer(&player(myindex())->y, data, SIZE8);

	//Make sure they aren't walking
	player(myindex())->moving = MOVEMENT_NONE;
	player(myindex())->offsetx = 0;
	player(myindex())->offsety = 0;
}

void handle_attack(buffer_t *data)
{
	int16 i;
	take_buffer(&i, data, SIZE16);

	//Set player to attacking
	player(i)->attacking = TRUE;
	player(i)->attacktimer = gettickcount();
}

void handle_npc_attack(buffer_t *data)
{
	uint8 i;
	take_buffer(&i, data, SIZE8);

	//Set player to attacking
	map()->npc[i].attacking = TRUE;
	map()->npc[i].attacktimer = gettickcount();
}

void handle_check_for_map(buffer_t *data)
{
	uint16 mapnum;
	uint32 revision;
	int16 i = 0;
	sbool needmap = TRUE;
	char *path = NULL;
	buffer_t buffer;

	//Erase all players except self
	for(i = 0; i < total_players_online(); i++){
		if(i != myindex())
			player(i)->map = 0;
	}

	clear_map_npcs();
	clear_map_items();

	take_buffer(&mapnum, data, SIZE32);
	take_buffer(&revision, data, SIZE32);

	path = get_path(MAP_PATH,mapnum,FILE_ENDING);
	if(file_exists(path)){
		read_map(path);

		//Check to see if the revisions match
		if(map()->revision == revision)
			needmap = FALSE;
	}

	//Either the revisions didn't match or we don't have the map, so we need it
	clear_buffer(&buffer);
	add_opcode(&buffer, CNEEDMAP);
	add_buffer(&buffer, &needmap, SIZE8);
	send_data(&buffer);
}

void handle_map_data(buffer_t *data)
{
	uint32 mapnum;
	char *path;
	take_buffer(&mapnum,data,SIZE32);

	if(mapnum <= 0 || mapnum >= MAX_MAPS)
		return;

	take_string(map()->name, data);
	take_buffer(&map()->moral,data,SIZE8);
	take_buffer(&map()->music,data,SIZE8);
	take_buffer(&map()->left,data,SIZE16);
	take_buffer(&map()->right,data,SIZE16);
	take_buffer(&map()->up,data,SIZE16);
	take_buffer(&map()->down,data,SIZE16);
	take_buffer(&map()->shop,data,SIZE16);
	take_buffer(&map()->bootmap,data,SIZE32);
	take_buffer(&map()->bootx,data,SIZE8);
	take_buffer(&map()->booty,data,SIZE8);
	take_buffer(&map()->revision,data,SIZE32);

	path = get_path(MAP_PATH, mapnum, FILE_ENDING);
	write_map(path);

	if(editor() == EDITOR_MAP){
		set_editor(0);
		//frmMirage.picMapEditor.Visible = False

		//If frmMapWarp.Visible Then
		//Unload frmMapWarp
		//End If

		//If frmMapProperties.Visible Then
		//Unload frmMapProperties
		//End If
	}
}

void handle_map_item_data(buffer_t *data)
{
	int16 i = 0;

	for(i = 1; i < MAX_MAP_ITEMS; i++){
		take_buffer(&map()->items[i].num,data,SIZE16);
		take_buffer(&map()->items[i].value,data,SIZE32);
		take_buffer(&map()->items[i].dur,data,SIZE16);
		take_buffer(&map()->items[i].x,data,SIZE8);
		take_buffer(&map()->items[i].y,data,SIZE8);
	}
}

void handle_map_npc_data(buffer_t *data)
{
	int16 i = 0;

	for(i = 1; i < MAX_MAP_NPCS; i++){
		take_buffer(&map()->npc[i].num,data,SIZE16);
		take_buffer(&map()->npc[i].x,data,SIZE8);
		take_buffer(&map()->npc[i].y,data,SIZE8);
		take_buffer(&map()->npc[i].dir,data,SIZE8);
	}
}

void handle_map_done(buffer_t *data)
{
	int16 i = 0;

	//MusicFile = Trim$(CStr(Map.Music)) & ".mid"

	//Play music
	//If Map.Music > 0 Then
	//If MusicFile <> CurrentMusic Then
	//DirectMusic_StopMidi
	//Call DirectMusic_PlayMidi(MusicFile)
	//CurrentMusic = MusicFile
	//End If
	//Else
	//DirectMusic_StopMidi
	////CurrentMusic = 0
	//End If

	//Call UpdateDrawMapName
	if(map()->shop == 0){
		//frmMirage.picTradeButton.Visible = False
	}
	else{
		//frmMirage.picTradeButton.Visible = True
	}

	//Call CalcTilePositions

	//Call InitTileSurf(Map.TileSet)

	tempdata()->gettingmap = FALSE;
	tempdata()->canmovenow = TRUE;
}

void handle_global_msg(buffer_t *data)
{
	char *string = NULL;
	uint32 size;
	uint8 color;

	take_buffer(&size,data,SIZE32);
	string = (char *)calloc(size,sizeof(char));
	take_string(string,data);
	take_buffer(&color, data, SIZE8);

	//Call AddText(Msg, Color)
}

void handle_player_msg(buffer_t *data)
{
	char *string = NULL;
	uint32 size;
	uint8 color;

	take_buffer(&size,data,SIZE32);
	string = (char *)calloc(size,sizeof(char));
	take_string(string,data);
	take_buffer(&color, data, SIZE8);

	//Call AddText(Msg, Color)
}

void handle_map_msg(buffer_t *data)
{
	char *string = NULL;
	uint32 size;
	uint8 color;

	take_buffer(&size,data,SIZE32);
	string = (char *)calloc(size,sizeof(char));
	take_string(string,data);
	take_buffer(&color, data, SIZE8);

	//Call AddText(Msg, Color)
}

void handle_admin_msg(buffer_t *data)
{
	char *string = NULL;
	uint32 size;
	uint8 color;

	take_buffer(&size,data,SIZE32);
	string = (char *)calloc(size,sizeof(char));
	take_string(string,data);
	take_buffer(&color, data, SIZE8);

	//Call AddText(Msg, Color))
}

void handle_refreash(buffer_t *data)
{
	int16 i = 0;

	//frmIndex.lstIndex.Clear

	switch(editor()){
	case EDITOR_ITEM:
		for(i = 1; i < MAX_ITEMS; i++){
			//frmIndex.lstIndex.AddItem i & ": " & Trim$(Item(i).Name)
		}
		break;
	case EDITOR_NPC:
		for(i = 1; i < MAX_NPCS; i++){
			//frmIndex.lstIndex.AddItem i & ": " & Trim$(Npc(i).Name)
		}
		break;
	case EDITOR_SHOP:
		for(i = 1; i < MAX_SHOPS; i++){
			//frmIndex.lstIndex.AddItem i & ": " & Trim$(Shop(i).Name)
		}
		break;
	case EDITOR_SPELL:
		for(i = 1; i < MAX_SPELLS; i++){
			//frmIndex.lstIndex.AddItem i & ": " & Trim$(Spell(i).Name)
		}
		break;
	}

	//frmIndex.lstIndex.ListIndex = 0
}

void handle_spawn_item(buffer_t *data)
{
	int16 n;

	take_buffer(&n, data, SIZE16);

	if(n == 0 || n >= MAX_MAP_ITEMS)
		return;

	take_buffer(&map()->items[n].num, data, SIZE16);
	take_buffer(&map()->items[n].value, data, SIZE32);
	take_buffer(&map()->items[n].dur, data, SIZE16);
	take_buffer(&map()->items[n].x, data, SIZE8);
	take_buffer(&map()->items[n].y, data, SIZE8);
}

void handle_item_editor(buffer_t *data)
{
	int16 i = 0;

	//frmIndex.Caption = "Item Index"
	set_editor(EDITOR_ITEM);

	//frmIndex.lstIndex.Clear

	//Add the names
	for(i = 1; i < MAX_ITEMS; i++){
		//	frmIndex.lstIndex.AddItem i & ": " & Trim$(Item(i).Name)
	}

	//frmIndex.lstIndex.ListIndex = 0
	//frmIndex.Show vbModal
}

void handle_update_item(buffer_t *data)
{
	int16 n;

	take_buffer(&n, data, SIZE16);

	if(n == 0 || n >= MAX_ITEMS)
		return;

	//Update the item
	take_string(item(n)->name, data);
	take_buffer(&item(n)->pic, data, SIZE16);
	take_buffer(&item(n)->type, data, SIZE8);
	take_buffer(&item(n)->data1, data, SIZE16);
	take_buffer(&item(n)->data2, data, SIZE16);
	take_buffer(&item(n)->data3, data, SIZE16);
}

void handle_edit_item(buffer_t *data)
{
	uint16 n;

	take_buffer(&n, data, SIZE16);

	if(n <= 0 || n >= MAX_ITEMS)
		return;

	//Update the item
	take_string(item(n)->name, data);
	take_buffer(&item(n)->type, data, SIZE8);
	take_buffer(&item(n)->pic, data, SIZE16);
	take_buffer(&item(n)->data1, data, SIZE16);
	take_buffer(&item(n)->data2, data, SIZE16);
	take_buffer(&item(n)->data3, data, SIZE16);

	//Initialize the item editor
	//Call ItemEditorInit
}

void handle_spawn_npc(buffer_t *data)
{
	uint8 n;

	take_buffer(&n, data, SIZE8);

	if(n <= 0 || n >= MAX_MAP_NPCS)
		return;

	take_buffer(&map()->npc[n].num, data, SIZE16);
	take_buffer(&map()->npc[n].x, data, SIZE8);
	take_buffer(&map()->npc[n].y, data, SIZE8);
	take_buffer(&map()->npc[n].dir, data, SIZE8);

	//Client use only
	map()->npc[n].offsetx = 0;
	map()->npc[n].offsety = 0;
	map()->npc[n].moving = MOVEMENT_NONE;
}

void handle_npc_dead(buffer_t *data)
{
	int8 n;

	take_buffer(&n, data, SIZE8);

	if(n <= 0 || n >= MAX_MAP_NPCS)
		return;

	clear_map_npc(n);
}

void handle_npc_editor(buffer_t *data)
{
	int16 i = 0;

	//frmIndex.Caption = "NPC Index"
	set_editor(EDITOR_NPC);

	//frmIndex.lstIndex.Clear

	//Add the names
	for(i = 1; i < MAX_NPCS; i++){
		//frmIndex.lstIndex.AddItem i & ": " & Trim$(Npc(i).Name)
	}

	//frmIndex.lstIndex.ListIndex = 0
	//frmIndex.Show vbModal
}

void handle_update_npc(buffer_t *data)
{
	int16 n;

	take_buffer(&n, data, SIZE16);

	if(n <= 0 || n >= MAX_NPCS)
		return;

	take_string(npc(n)->name, data);
	npc(n)->say = NULL;
	take_buffer(&npc(n)->sprite, data, SIZE16);
	npc(n)->behavior = 0;
	npc(n)->range = 0;
	npc(n)->dropitem = 0;
	npc(n)->dropchance = 0;
	npc(n)->dropitemvalue = 0;
	npc(n)->spawnsecs = 0;
	npc(n)->stat[STAT_STRENGTH] = 0;
	npc(n)->stat[STAT_DEFENSE] = 0;
	npc(n)->stat[STAT_SPEED] = 0;
	npc(n)->stat[STAT_MAGIC] = 0;
}

void handle_edit_npc(buffer_t *data)
{
	int16 n;

	take_buffer(&n, data, SIZE16);

	if(n <= 0 || n >= MAX_NPCS)
		return;

	//Update the Npc
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

	//Initialize the npc editor
	//Call NpcEditorInit
}

void handle_edit_map(buffer_t *data)
{
	//Call MapEditorInit
}

void handle_shop_editor(buffer_t *data)
{
	int16 i = 0;

	//frmIndex.Caption = "Shop Index"
	set_editor(EDITOR_SHOP);

	//frmIndex.lstIndex.Clear

	//Add the names
	for(i = 1; i < MAX_SHOPS; i++){
		//	frmIndex.lstIndex.AddItem i & ": " & Trim$(Shop(i).Name)
	}

	//frmIndex.lstIndex.ListIndex = 0
	//	frmIndex.Show vbModal
}

void handle_update_shop(buffer_t *data)
{
	int16 n;

	take_buffer(&n, data, SIZE16);

	if(n <= 0 || n >= MAX_SHOPS)
		return;

	//Update the shop name
	take_string(shop(n)->name,data);
}

void handle_edit_shop(buffer_t *data)
{
	int16 n;

	take_buffer(&n, data, SIZE16);

	if(n <= 0 || n >= MAX_SHOPS)
		return;

	//Update the Shop
	take_string(shop(n)->name, data);
	take_string(shop(n)->joinsay, data);
	take_string(shop(n)->leavesay, data);
	take_buffer(&shop(n)->fixesitems, data, SIZE8);
	take_buffer(shop(n)->tradeitem, data, MAX_TRADES * sizeof(trade_item_t));

	//Initialize the shop editor
	//Call ShopEditorInit
}

void handle_spell_editor(buffer_t *data)
{
	int16 i = 0;

	//frmIndex.Caption = "Spell Index"
	set_editor(EDITOR_SPELL);

	//frmIndex.lstIndex.Clear

	//Add the names
	for(i = 1; i < MAX_SPELLS; i++){
		//frmIndex.lstIndex.AddItem i & ": " & Trim$(Spell(i).Name)
	}

	//frmIndex.lstIndex.ListIndex = 0
	//frmIndex.Show vbModal
}

void handle_update_spell(buffer_t *data)
{
	int16 n;

	take_buffer(&n, data, SIZE16);

	if(n <= 0 || n >= MAX_SPELLS)
		return;

	//Update the spell name
	take_string(spell(n)->name, data);
	take_buffer(&spell(n)->mpreq, data, SIZE16);
	take_buffer(&spell(n)->pic, data, SIZE16);
}

void handle_edit_spell(buffer_t *data)
{
	int16 n;

	take_buffer(&n, data, SIZE16);

	if(n <= 0 || n >= MAX_SPELLS)
		return;

	//Update the Spell
	take_string(spell(n)->name,data);
	take_buffer(&spell(n)->pic, data, SIZE16);
	take_buffer(&spell(n)->mpreq, data, SIZE16);
	take_buffer(&spell(n)->jobreq, data, SIZE8);
	take_buffer(&spell(n)->levelreq, data, SIZE8);
	take_buffer(&spell(n)->type, data, SIZE8);
	take_buffer(&spell(n)->data1, data, SIZE16);
	take_buffer(&spell(n)->data2, data, SIZE16);
	take_buffer(&spell(n)->data3, data, SIZE16);

	//Initialize the spell editor
	//Call SpellEditorInit
}

void handle_trade(buffer_t *data)
{
	uint32 i = 1;
	int16 n;

	take_buffer(&n, data, SIZE16);

	if(n <= 0 || n >= MAX_SPELLS)
		return;

	take_buffer(&shop(n)->fixesitems, data, SIZE8);

	if(shop(n)->fixesitems){
		//frmTrade.lblFixItem.Visible = True
	}
	else{
		//frmTrade.lblFixItem.Visible = False
	}

	for(i = 1; i < MAX_TRADES; i++){
		shop(n)->tradeitem[i].giveitem = 0;
		shop(n)->tradeitem[i].givevalue = 0;
		shop(n)->tradeitem[i].getitem = 0;
		shop(n)->tradeitem[i].getvalue = 0;
	}

	for(i = 1; i < MAX_TRADES; i++){
		take_buffer(&shop(n)->tradeitem[i].giveitem, data, SIZE16);
		take_buffer(&shop(n)->tradeitem[i].givevalue, data, SIZE32);
		take_buffer(&shop(n)->tradeitem[i].getitem, data, SIZE16);
		take_buffer(&shop(n)->tradeitem[i].getvalue, data, SIZE32);

		if(shop(n)->tradeitem[i].giveitem > 0 && shop(n)->tradeitem[i].getitem > 0){
			//frmTrade.lstTrade.AddItem "Give " & Trim$(Shop(ShopNum).Name) & " " & GiveValue & " " & Trim$(Item(GiveItem).Name) & " for " & GetValue & " " & Trim$(Item(GetItem).Name)
		}
		else{
			//	frmTrade.lstTrade.AddItem vbNullString
		}
	}

	//If frmTrade.lstTrade.ListCount > 0 Then frmTrade.lstTrade.ListIndex = 0
	//	frmTrade.Show vbModal
}

void handle_spells(buffer_t *data)
{
	int16 i = 1;
	uint8 spellnum;

	//frmMirage.picSpellsList.Visible = True
	//frmMirage.lstSpells.Clear

	for(i = 1; i < MAX_PLAYER_SPELLS; i++){
		take_buffer(&player(myindex())->spells[i], data, SIZE16);

		if(player(myindex())->spells[i] != 0){
			//frmMirage.lstSpells.AddItem i & ": " & Trim$(Spell(PlayerSpells(i)).Name)
		}
		else{
			//frmMirage.lstSpells.AddItem "<free spells slot>"
		}
	}

	//frmMirage.lstSpells.ListIndex = 0
}

void handle_left(buffer_t *data)
{
	int16 index;

	take_buffer(&index,data,SIZE16);
	clear_player(index);
	get_players_on_map();
}

void handle_high_index(buffer_t *data)
{
	int16 high_index;

	take_buffer(&high_index,data,SIZE16);
	set_total_players_online(high_index);
}

void handle_spell_cast(buffer_t *data)
{
	int16 i = 0;
	uint8 targettype;
	uint8 x, y;
	uint16 spellnum;

	take_buffer(&spellnum,data,SIZE16);
	take_buffer(&x,data,SIZE8);
	take_buffer(&y,data,SIZE8);

	if(spellnum <= 0 || spellnum >= MAX_SPELLS)
		return;

	for(i = 1; i < MAX_SPELLANIM; i++){
		if(spell_anim(i)->spellnum == 0){
			spell_anim(i)->spellnum = spellnum;
			spell_anim(i)->timer = gettickcount() + 120;
			spell_anim(i)->framepointer = 0;
			return;
		}
	}
}