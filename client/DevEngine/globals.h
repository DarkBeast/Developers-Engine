// Static Variables defines and Public Variables
#ifndef _GLOBALS_H
#define _GLOBALS_H
#include"Integer.h"

/* Public globals */
Uint8 DebugMode;

/* static globals */

// client defines
#define TITLE     "DEV Engine"
#define COMPANY   "GGS"
#define COPYRIGHT "Copyright (C) Global Gamer Studios 2011"
#define VERSION   "1.0"

// network defines
#define CLIENT_PORT        8001	/* The port the server will use to connect */
#define PACKET_SIZE        8192	/* Size of packet buffer (about 8kiB) */

// player defines
#define MAX_PLAYERS        50  /* # of players the server can support */
#define MAX_USERNAME_SIZE  16  /* Let's have a reasonable size for usernames */

// map defines
#define TILE_SIZE_X     32	//magic pixels
#define TILE_SIZE_Y     32	//magic pixels

#define true 0
#define false -1


// Custom Error flags
#define MISC_ERROR        -1
#define POINTER_IN_USE     1
#define UNDEFINED_POINTER  2
#define OUT_OF_MEMORY      3
#define IO_ERROR           4
#define FILE_ERROR         5
#define OVERFLOW_ERROR     6
#define SERVER_ADDR_ERROR  7

/* Always the end nothing past this point. */
#endif

