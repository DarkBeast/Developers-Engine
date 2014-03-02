/*******************************************************************************
* Credits:  Anthony Ianozi
*           Andrew Wheeler/Genusis
******************************************************************************/
#include<string.h>
#include<stdint.h>
#include<stdlib.h>
#include"globals.h"
#include"buffer.h"

//byte array code
/*NOTE: All buffer functions return one on overflow*/
int add_buffer(buffer_t *buffer, const void* source, size_t size)
{
	if((buffer->offset >= PACKET_SIZE) && (buffer->offset+size > PACKET_SIZE)) return 1;
	memcpy(buffer->buff+buffer->offset, source, size);
	buffer->offset += size;
	return 0;
}

void take_buffer(void* dest, buffer_t* buffer, size_t size)
{
	memcpy(dest, buffer->buff+buffer->offset, size);
	buffer->offset += size;
}

int add_string( buffer_t* buffer, char* str)
{
	size_t len = strlen(str)+1;
	if(add_buffer(buffer, &len, sizeof(len))) return 1;
	if(add_buffer(buffer, str, len)) return 1;
	return 0;
}

void take_string(char* dest, buffer_t *buffer)
{
	size_t len;
	take_buffer(&len, buffer, sizeof(len));
	take_buffer(dest, buffer, len);
}

void clear_buffer(buffer_t *buffer)
{
	memset(buffer->buff, 0, PACKET_SIZE);
	buffer->offset=0;
}

int set_raw_string(buffer_t *buffer, char* str)
{
	size_t len = strlen(str)+1;
	clear_buffer(buffer);
	if(add_buffer(buffer, str, len)) return 1;
	return 0;
}

int add_opcode(buffer_t* buffer, uint8 opcode)
{
	return add_buffer(buffer, &opcode, sizeof(opcode));
}

int  buffer_full(buffer_t* buffer, size_t size)
{
	return ((buffer->offset >= PACKET_SIZE) && (buffer->offset+size > PACKET_SIZE))? 1 : 0;
}

void encrypt_packet(buffer_t* buffer)
{
	uint32 x = 0;
	char *encrypted = (char *)calloc(buffer->offset, sizeof(char));
	uint32 buffersize = buffer->offset;

	for (x = 0; x < buffer->offset; x++){
		encrypted[x] = buffer->buff[x] ^ ((uint32)ENCRYPT_KEY + x) % 255;
		encrypted[x] ^= (1 << 5);
		encrypted[x] ^= (1 << 2);
		encrypted[x] ^= (1 << 7);
	}
	clear_buffer(buffer);
	add_buffer(buffer, encrypted, buffersize);
}

void decrypt_packet(buffer_t* buffer)
{
	uint32 x = 0;
	char *decrypted = (char *)calloc(buffer->offset, sizeof(char));
	uint32 buffersize = buffer->offset;

	for (x = 0; x < buffer->offset; x++){
		buffer->buff[x] ^= (1 << 7);
		buffer->buff[x] ^= (1 << 2);
		buffer->buff[x] ^= (1 << 5);
		decrypted[x] = buffer->buff[x] ^ ((uint32)ENCRYPT_KEY + x) % 255;
	}
	clear_buffer(buffer);
	add_buffer(buffer, decrypted, buffersize);
	buffer->offset = 0;
}