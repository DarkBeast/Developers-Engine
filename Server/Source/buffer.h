/*******************************************************************************
* Credits:  Anthony Ianozi
*           Andrew Wheeler
******************************************************************************/

#ifndef DE_BUFFER_H
#define DE_BUFFER_H

#include"integer.h"

#define PACKET_SIZE    8192   // Size of packet buffer

typedef struct buffer_t buffer_t;
typedef enum data_sizes data_sizes;

struct buffer_t
{
	char buff[PACKET_SIZE];
	uint16 offset;
};

enum data_sizes{
	NONE = 0,
	SIZE8 = 1,
	SIZE16 = 2,
	SIZE32 = 4,
	SIZE64 = 8,
	SIZEF = 4,
	SIZED = 8
};

int  add_buffer(buffer_t *buffer, const void* source, size_t size);

int  add_opcode(buffer_t* buffer, uint8 opcode);

int  add_string(buffer_t* buffer, char* str);

int  buffer_full(buffer_t* buffer, size_t size);

int  set_raw_string(buffer_t *buffer, char* str);

void clear_buffer(buffer_t *buffer);

void take_buffer(void* dest, buffer_t* buffer, size_t size);

void take_string(char* dest, buffer_t *buffer);

void encrypt_packet(buffer_t* buffer);

void decrypt_packet(buffer_t* buffer);

#endif
