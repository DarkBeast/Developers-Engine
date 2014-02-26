/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
*           Anthony Ianozi
******************************************************************************/

/* Static Variables defines and Public Variables*/
#ifndef DE_GLOBALS_H
#define DE_GLOBALS_H

#include "bool.h"
/* static global's
*  can be const Uint8 test = 5; or #defines
*  Keep defines in all cap format.
*/

/* client defines*/
#define TITLE     "DEV Engine"
#define COMPANY   "Spirean"
#define COPYRIGHT "Copyright (C) MMORPG Creation 2013"
#define VERSION_MAJOR   1
#define VERSION_MINOR   0
#define VERSION_REV   0

#define ADMIN_LOG  ".\\logs\\admin.log"
#define PLAYER_LOG  ".\\logs\\player.log"
#define LOG_PATH ".\\logs"
#define FILE_ENDING ".dat"
#define MAP_PATH ".\\maps"
#define GUI_PATH ".\\images\\gui\\"
#define ITEM_PATH ".\\images\\items"
#define TILE_PATH ".\\images\\tiles"
#define SPRITE_PATH ".\\images\\npcs"
#define SPELL_PATH ".\\images\\spells"
#define IMAGE_ENDING ".png"
#define PIC_SIZE 32

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
#define MAX_SPELLANIM 8

//player movement constants.
#define WALK_SPEED 4
#define RUN_SPEED 8

//encrypt key must be string!
//Server and Client encrypt key must match.
#define ENCRYPT_KEY  "Aab456"

/* network defines*/
#define SERVER_IP     "127.0.0.1"
#define CLIENT_PORT        8001	//The port the server will use to connect
#define PACKET_SIZE        8192	// Size of packet buffer (about 8kiB)
#define SOCKET_REC_SIZE    32

/* player defines*/
#define MAX_PLAYERS        64  // # of players the server can support
#define MAX_PASS_LENGTH  32
#define MAX_NAME_LENGTH  32  // Let's have a reasonable size for user names, npc names etc.
#define MAX_SAY_LENGTH  100  // Let's have a reasonable size for user names, npc names etc.

/* map defines*/
#define MAX_MAPS   100
#define MAX_MAPX   15
#define MAX_MAPY   11
#define TILE_SIZE     32	//magic pixels this is used for both width and height

/*widget defines*/
#define WIDGET_MAX       32 //max amount of widgets per array.
#define FONTNAME   "arial.ttf"

/*to use debug commands set debug to TRUE to turn off set debug to FALSE
then compile the Program for it to go into Debug mode*/
#define DEBUG 0

/* Always the end nothing past this point. */
#endif
