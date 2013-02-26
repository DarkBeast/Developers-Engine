/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/
#include "globals.h"

/*examples of a global variable*/
int players_online;

void set_players_online(int players){ players_online = players; }

void increase_players_online(void){ players_online += 1; }

void decrease_players_online(void){ players_online -= 1; }

int get_players_online(void){ return players_online; }