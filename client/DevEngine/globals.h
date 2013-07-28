/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
*           Anthony Ianozi
******************************************************************************/

/* Static Variables defines and Public Variables*/
#ifndef DE_GLOBALS_H
#define DE_GLOBALS_H

#include "bool.h"

/* Public global's */
//see globals.c it contains public global's types and a example.

/* static global's
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

/*widget defines*/
#define WIDGET_MAX        100 //max amount of widgets per array.
#define FONTNAME   "arial.ttf" // the font we will use globally. TODO: allow extra fonts at once.
/*to use debug commands set debug to TRUE to turn off set debug to FALSE
then compile the Program for it to go into Debug mode*/
#define DEBUG 0

/* Always the end nothing past this point. */
#endif
