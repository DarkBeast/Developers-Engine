/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/
#ifndef DE_GENERAL_H
#define DE_GENERAL_H
#include "items.h"
#include "maps.h"
#include "npcs.h"
#include "players.h"
#include "shops.h"
#include "spells.h"

sbool shutting_down(void);

void set_shut_down(void);

void init_server(void);

void add_log(char *string, char *path);

void destroy_server(void);

#endif