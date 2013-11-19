/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/
#include "general.h"
#include <stdio.h>
#include "error.h"
#include "path.h"
#include <time.h>
#include <string.h>
#include "gamelogic.h"
#include "socket.h"

sbool shutting_down_b = FALSE;

sbool shutting_down(void)
{
	return shutting_down_b;
}

void set_shut_down(void)
{
	shutting_down_b = TRUE;
}

void init_server(void)
{
	printf("Initializing Data. \n");
	check_dir(LOG_PATH);

	printf("Loading Items. \n");
	init_items();

	printf("Loading Maps. \n");
	init_maps();

	printf("Loading Npcs. \n");
	init_npcs();

	printf("Loading Shops. \n");
	init_shops();

	printf("Loading Spells. \n");
	init_spells();

	printf("Initializing Player Data. \n");
	init_players();

	printf("Data Initialized. \n");
}

void add_log(char *string, char *path)
{
	FILE *fp;
	time_t current_time;
	char* c_time_string;
	current_time = time(NULL);

	c_time_string = ctime(&current_time);

	if(!file_exists(path)){
		if((fp = fopen(path, "w")) == NULL)
			error_handler(DE_ERROR_FILE_ERROR);
		fclose(fp);
	}

	if((fp = fopen(path, "r+")) == NULL)
		error_handler(DE_ERROR_FILE_ERROR);

	c_time_string[strlen(c_time_string) - 1] = '\0';

	fseek(fp, 0, SEEK_END);
	fprintf(fp, "\n %s: %s", c_time_string, string);

	fclose(fp);
}

void destroy_server(void)
{
	uint32 i = 0;

	printf("Saving online players.");

	for(i = 0; i < total_players_online(); i++){
		left_game(player_online(i));
	}

	printf("Clearing Data.");
	unload_shops();
	unload_spells();
	unload_items();
	unload_npcs();
	unload_players();
	unload_maps();
	unload_socket();
	path_destroy();
}