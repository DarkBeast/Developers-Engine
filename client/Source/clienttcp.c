/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/
#include "clienttcp.h"
#include "socket.h"
#include "general.h"

void send_data(buffer_t *data)
{
	if(isconnected())
		socketsend(data);
}

void send_new_account(char *name, char *charname, char *password, uint8 job, uint8 sex)
{
	buffer_t buffer;

	clear_buffer(&buffer);
	add_opcode(&buffer,CNEWACCOUNT);
	add_encypt_string(&buffer, name);
	add_encypt_string(&buffer, charname);
	add_encypt_string(&buffer, password);
	add_buffer(&buffer, &job, SIZE8);
	add_buffer(&buffer, &sex, SIZE8);

	send_data(&buffer);
}

void send_login(char *name, char *password)
{
	buffer_t buffer;
	int major = VERSION_MAJOR, minor = VERSION_MINOR, rev = VERSION_REV;
	clear_buffer(&buffer);
	add_opcode(&buffer,CLOGIN);
	add_encypt_string(&buffer, name);
	add_encypt_string(&buffer, password);
	add_buffer(&buffer, &major, SIZE8);
	add_buffer(&buffer, &minor, SIZE8);
	add_buffer(&buffer, &rev, SIZE8);

	send_data(&buffer);
}

void say_msg(char *text)
{
	buffer_t buffer;
	uint32 len = strlen(text);

	clear_buffer(&buffer);
	add_opcode(&buffer,CSAYMSG);
	add_buffer(&buffer,&len, SIZE32);
	add_string(&buffer, text);

	send_data(&buffer);
}

void global_msg(char *text)
{
	buffer_t buffer;
	uint32 len = strlen(text);

	clear_buffer(&buffer);
	add_opcode(&buffer,CGLOBALMSG);
	add_buffer(&buffer,&len, SIZE32);
	add_string(&buffer, text);

	send_data(&buffer);
}

void broadcast_msg(char *text)
{
	buffer_t buffer;
	uint32 len = strlen(text);

	clear_buffer(&buffer);
	add_opcode(&buffer,CBROADCASTMSG);
	add_buffer(&buffer,&len, SIZE32);
	add_string(&buffer, text);

	send_data(&buffer);
}

void emote_msg(char *text)
{
	buffer_t buffer;
	uint32 len = strlen(text);

	clear_buffer(&buffer);
	add_opcode(&buffer,CEMOTEMSG);
	add_buffer(&buffer,&len, SIZE32);
	add_string(&buffer, text);

	send_data(&buffer);
}

void player_msg(char *text, char *name)
{
	buffer_t buffer;
	uint32 len = strlen(text);

	clear_buffer(&buffer);
	add_opcode(&buffer,CPLAYERMSG);
	add_string(&buffer, name);
	add_buffer(&buffer,&len, SIZE32);
	add_string(&buffer, text);

	send_data(&buffer);
}

void admin_msg(char *text)
{
	buffer_t buffer;
	uint32 len = strlen(text);

	clear_buffer(&buffer);
	add_opcode(&buffer,CADMINMSG);
	add_buffer(&buffer,&len, SIZE32);
	add_string(&buffer, text);

	send_data(&buffer);
}

void send_player_move(void)
{
	buffer_t buffer;

	clear_buffer(&buffer);
	add_opcode(&buffer,CPLAYERMOVE);
	add_buffer(&buffer,&player(myindex())->dir, SIZE8);
	add_buffer(&buffer,&player(myindex())->moving, SIZE8);

	send_data(&buffer);
}

void send_player_dir(void)
{
	buffer_t buffer;

	clear_buffer(&buffer);
	add_opcode(&buffer,CPLAYERDIR);
	add_buffer(&buffer,&player(myindex())->dir, SIZE8);

	send_data(&buffer);
}

void request_new_map(void)
{
	buffer_t buffer;

	clear_buffer(&buffer);
	add_opcode(&buffer,CREQUESTNEWMAP);
	add_buffer(&buffer,&player(myindex())->dir, SIZE8);

	send_data(&buffer);
}

void send_map(void)
{
	buffer_t buffer;
	uint32 x = 0;

	clear_buffer(&buffer);

	add_opcode(&buffer, CMAPDATA);
	add_string(&buffer, map()->name );
	add_buffer(&buffer, &map()->moral,SIZE8);
	add_buffer(&buffer, &map()->music,SIZE8);
	add_buffer(&buffer, &map()->left,SIZE16);
	add_buffer(&buffer, &map()->right,SIZE16);
	add_buffer(&buffer, &map()->up,SIZE16);
	add_buffer(&buffer, &map()->down,SIZE16);
	add_buffer(&buffer, &map()->shop,SIZE16);
	add_buffer(&buffer, &map()->bootmap,SIZE32);
	add_buffer(&buffer, &map()->bootx,SIZE8);
	add_buffer(&buffer, &map()->booty,SIZE8);

	for( x = 0; x < MAX_MAP_NPCS; x++){
		add_buffer(&buffer, &map()->npc[x].num,SIZE16);
	}

	add_buffer(&buffer, map()->tile,(MAX_MAPX * MAX_MAPY) * sizeof(tile_t));

	send_data(&buffer);
}

void warp_me_to(char *name)
{
	buffer_t buffer;

	clear_buffer(&buffer);
	add_opcode(&buffer,CWARPMETO);
	add_string(&buffer,name);

	send_data(&buffer);
}

void warp_to_me(char *name)
{
	buffer_t buffer;

	clear_buffer(&buffer);
	add_opcode(&buffer,CWARPTOME);
	add_string(&buffer,name);

	send_data(&buffer);
}

void warp_to(uint32 mapnum)
{
	buffer_t buffer;

	clear_buffer(&buffer);
	add_opcode(&buffer,CWARPTO);
	add_buffer(&buffer,&mapnum, SIZE32);

	send_data(&buffer);
}

void send_set_access(char *name, uint8 group)
{
	buffer_t buffer;

	clear_buffer(&buffer);
	add_opcode(&buffer,CSETGROUP);
	add_string(&buffer,name);
	add_buffer(&buffer,&group, SIZE8);

	send_data(&buffer);
}

void send_set_sprite(char *name, uint32 spritenum)
{
	buffer_t buffer;

	clear_buffer(&buffer);
	add_opcode(&buffer,CSETSPRITE);
	add_buffer(&buffer,&spritenum, SIZE32);

	send_data(&buffer);
}

void send_kick(char *name)
{
	buffer_t buffer;

	clear_buffer(&buffer);
	add_opcode(&buffer,CKICKPLAYER);
	add_string(&buffer,name);

	send_data(&buffer);
}

void send_ban(char *name)
{
	buffer_t buffer;

	clear_buffer(&buffer);
	add_opcode(&buffer,CBANPLAYER);
	add_string(&buffer,name);

	send_data(&buffer);
}

void send_request_edit_item(void)
{
	buffer_t buffer;

	clear_buffer(&buffer);
	add_opcode(&buffer,CREQUESTEDITITEM);

	send_data(&buffer);
}

void send_save_item(uint16 itemnum)
{
	buffer_t buffer;

	clear_buffer(&buffer);

	add_opcode(&buffer, CSAVEITEM);
	add_buffer(&buffer, &itemnum, SIZE16);
	add_string(&buffer,item(itemnum)->name);
	add_buffer(&buffer, &item(itemnum)->type, SIZE8);
	add_buffer(&buffer, &item(itemnum)->pic, SIZE16);
	add_buffer(&buffer, &item(itemnum)->data1, SIZE16);
	add_buffer(&buffer, &item(itemnum)->data2, SIZE16);
	add_buffer(&buffer, &item(itemnum)->data3, SIZE16);

	send_data(&buffer);
}

void send_request_edit_npc(void)
{
	buffer_t buffer;

	clear_buffer(&buffer);
	add_opcode(&buffer,CREQUESTEDITNPC);

	send_data(&buffer);
}

void send_save_npc(uint16 npcnum)
{
	buffer_t buffer;

	clear_buffer(&buffer);

	add_opcode(&buffer, CSAVENPC);
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

	send_data(&buffer);
}

void send_map_respawn(void)
{
	buffer_t buffer;

	clear_buffer(&buffer);
	add_opcode(&buffer,CMAPRESPAWN);

	send_data(&buffer);
}

void send_use_item(uint16 invnum)
{
	buffer_t buffer;

	clear_buffer(&buffer);
	add_opcode(&buffer,CUSEITEM);
	add_buffer(&buffer, &invnum, SIZE8);

	send_data(&buffer);
}

void send_drop_item(uint16 invnum,uint32 amount)
{
	buffer_t buffer;

	clear_buffer(&buffer);
	add_opcode(&buffer,CMAPDROPITEM);
	add_buffer(&buffer, &invnum, SIZE8);
	add_buffer(&buffer, &amount, SIZE32);

	send_data(&buffer);
}

void send_whos_online(void)
{
	buffer_t buffer;

	clear_buffer(&buffer);
	add_opcode(&buffer,CWHOSONLINE);

	send_data(&buffer);
}

void send_request_edit_shop(void)
{
	buffer_t buffer;

	clear_buffer(&buffer);
	add_opcode(&buffer,CREQUESTEDITSHOP);

	send_data(&buffer);
}

void send_save_shop(uint16 shopnum)
{
	buffer_t buffer;

	clear_buffer(&buffer);

	add_opcode(&buffer, CSAVESHOP);
	add_buffer(&buffer, &shopnum, SIZE16);
	add_string(&buffer,shop(shopnum)->name);
	add_string(&buffer,shop(shopnum)->joinsay);
	add_string(&buffer,shop(shopnum)->leavesay);
	add_buffer(&buffer, &shop(shopnum)->fixesitems, SIZE8);
	add_buffer(&buffer, &shop(shopnum)->tradeitem, MAX_TRADES * sizeof(trade_item_t));

	send_data(&buffer);
}

void send_request_edit_spell(void)
{
	buffer_t buffer;

	clear_buffer(&buffer);
	add_opcode(&buffer,CREQUESTEDITSPELL);

	send_data(&buffer);
}

void send_save_spell(uint16 spellnum)
{
	buffer_t buffer;

	clear_buffer(&buffer);

	add_opcode(&buffer, CSAVESPELL);
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

	send_data(&buffer);
}

void send_request_edit_map(void)
{
	buffer_t buffer;

	clear_buffer(&buffer);
	add_opcode(&buffer,CREQUESTEDITMAP);

	send_data(&buffer);
}