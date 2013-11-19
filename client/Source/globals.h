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
#define MAX_PASSWORD_SIZE  32
#define MAX_USERNAME_SIZE  32  // Let's have a reasonable size for user names

/* map defines*/
#define TILE_SIZE_X     32	//magic pixels
#define TILE_SIZE_Y     32	//magic pixels

/*widget defines*/
#define WIDGET_MAX       64 //max amount of widgets per array.
#define FONTNAME   "arial.ttf"

/*to use debug commands set debug to TRUE to turn off set debug to FALSE
then compile the Program for it to go into Debug mode*/
#define DEBUG 0

/* Public global's */
//see globals.c it contains public global's types and a example.
void set_players_online(int players);
void increase_players_online(void);
void decrease_players_online(void);
int get_players_online(void);
/* Always the end nothing past this point. */
#endif
