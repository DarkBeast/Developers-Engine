/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
*           Anthony Ianozi
******************************************************************************/

/* Static Variables defines and Public Variables*/
#ifndef DE_GLOBALS_H
#define DE_GLOBALS_H
/* Public global's */
//see globals.c it contains public global's types and a example.

/* static global's
*  #defines
*/

/* client defines*/
#define TITLE     "DEV Engine"
#define COMPANY   "Spirean"
#define COPYRIGHT "Copyright (C) MMORPG Creation 2013"
#define GAME_NAME "Developers Engine"
#define GAME_WEBSTIE "http://mmorpgcreation.com"
#define MOTD "developers working hard for you!"
//path constants
#define ADMIN_LOG  ".\\logs\\admin.log"
#define PLAYER_LOG  ".\\logs\\player.log"
#define LOG_PATH ".\\logs"
#define FILE_ENDING ".dat"
#define ITEM_PATH ".\\items"
#define MAP_PATH ".\\maps"
#define NPC_PATH ".\\npcs"
#define PLAYER_PATH ".\\players"
#define SHOP_PATH ".\\shops"
#define SPELL_PATH ".\\spells"
#define USED_CHAR_LIST_PATH  ".\\usedchars.dat"

//Version constants
#define VERSION_MAJOR   1
#define VERSION_MINOR   0
#define VERSION_REV   0

//Data constants
#define MAX_ITEMS  255
#define MAX_NPCS   255
#define MAX_INV   50
#define MAX_MAP_ITEMS   20
#define MAX_MAP_NPCS   5
#define MAX_SHOPS   50
#define MAX_PLAYER_SPELLS   20
#define MAX_SPELLS   255
#define MAX_TRADES   8
#define MAX_LEVELS   100
#define MAX_JOBS  3

//encrypt key must be string and be at minimal 32 characters long
//Server and Client encrypt key must match.
#define ENCRYPT_KEY  "Aab456sdfg5gf89d5h41r68d49f8gh4rhrt651dr8tg41d6h5g1h"

/* network defines*/
#define SERVER_IP     "localhost"
#define SERVER_PORT        8001	//The port the server will use to connect
#define PACKET_SIZE        8192	// Size of packet buffer (about 8kiB)

/* player defines*/
#define MAX_PLAYERS        64  // # of players the server can support
#define MAX_PASS_LENGTH  32
#define MAX_NAME_LENGTH  32  // Let's have a reasonable size for user names, npc names etc.
#define MAX_SAY_LENGTH  100  // Let's have a reasonable size for user names, npc names etc.
#define MAX_PLAYER_LEVEL 100

/* map defines*/
#define MAX_MAPS   100
#define MAX_MAPX   15
#define MAX_MAPY   11
#define TILE_SIZE_X     32	//magic pixels
#define TILE_SIZE_Y     32	//magic pixels

#define START_MAP    1
#define START_X    4
#define START_Y    4

/*widget defines*/
#define WIDGET_MAX       64 //max amount of widgets per array.
#define FONTNAME   "arial.ttf" // the font we will use globally. TODO: allow extra fonts at once.
/*to use debug commands set debug to TRUE to turn off set debug to FALSE
then compile the Program for it to go into Debug mode*/
#define DEBUG 0
/* Always the end nothing past this point. */
#endif
