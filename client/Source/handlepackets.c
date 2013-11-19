/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
*           Jacob/Dugar
******************************************************************************/
#include "handlepackets.h"
#include <stdio.h>
#include "globals.h"
#include "clienttcp.h"
#include <stdlib.h>

void(*packets[SMSG_COUNT])(buffer_t *);

void init_packets(void)
{
	packets[SLOGINOK] = handle_loginok;
}

void incomming_packets(buffer_t *data)
{
	uint8 id = SNONE;
	take_buffer(&id,data,1);

	if(data->offset == 0){
		error_handler(DE_ERROR_INVALID_PACKET_OFFSET);
		return;
	}

	if(id == SNONE){
		error_handler(DE_ERROR_INVALID_PACKET_ID);
		return;
	}

	packets[id](data);
}

void handle_loginok(buffer_t *data)
{
	char *name = (char *)calloc(MAX_USERNAME_SIZE, sizeof(char));
	char *password = (char *)calloc(MAX_PASSWORD_SIZE, sizeof(char));

	take_string(name,data);
	take_string(password,data);

	printf("name is: %c. \n,password is: %c.\n",name,password);
}