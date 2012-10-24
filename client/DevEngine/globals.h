/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
*           Anthony Ianozi
******************************************************************************/

/* Static Variables defines and Public Variables*/
#ifndef _GLOBALS_H
#define _GLOBALS_H

#include "integer.h"

/* Public globals */
//CBool SYSBOOL;

/* static globals 
*  can be const Uint8 test = 5; or #defines
*/

/* client defines*/
#define TITLE     "DEV Engine"
#define COMPANY   "GGS"
#define COPYRIGHT "Copyright (C) Global Gamer Studios 2011"
#define VERSION_MAJOR   1
#define VERSION_MINOR   0
#define VERSION_REV   0

/* network defines*/
#define CLIENT_PORT        8001	//The port the server will use to connect 
#define PACKET_SIZE        8192	// Size of packet buffer (about 8kiB)

/* player defines*/
#define MAX_PLAYERS        50  // # of players the server can support 
#define MAX_USERNAME_SIZE  16  // Let's have a reasonable size for usernames 

/* map defines*/
#define TILE_SIZE_X     32	//magic pixels
#define TILE_SIZE_Y     32	//magic pixels

/* boolean defines*/
#define TRUE 0
#define FALSE -1

/*widget defines*/
#define WIDGET_MAX        100 //max ammont of widgets per array.


/* Always the end nothing past this point. */
#endif

