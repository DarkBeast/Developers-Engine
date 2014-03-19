/*******************************************************************************
* Credits:  Anthony Ianozi
*           Andrew Wheeler/Genusis
******************************************************************************/
#include<string.h>
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

int add_encypt_string( buffer_t* buffer, char* str)
{
	size_t len = strlen(str)+1;
	buffer_t string;

	clear_buffer(&string);

	if(add_buffer(buffer, &len, sizeof(len))) return 1;
	if(add_buffer(&string, str, len)) return 1;
	if(encrypt_packet(&string,len)) return 1;
	if(add_buffer(buffer, string.buff, string.offset)) return 1;
	return 0;
}

void take_encypt_string(char* dest, buffer_t *buffer)
{
	size_t len;
	buffer_t string;

	clear_buffer(&string);
	take_buffer(&len, buffer, sizeof(len));
	take_buffer(string.buff, buffer, len);
	decrypt_packet(&string,len);
	take_buffer(dest, &string, len);
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

int encrypt_packet(buffer_t* buffer, size_t size)
{
	uint32 x = 0;
	char *encrypted = (char *)calloc(size, sizeof(char));
	uint32 buffersize = size;

	for (x = 0; x < size; x++)
		encrypted[x] = (buffer->buff[x] ^ ENCRYPT_KEY[x]);

	clear_buffer(buffer);
	if(add_buffer(buffer, encrypted, buffersize)) return 1;
	return 0;
}

int decrypt_packet(buffer_t* buffer, size_t size)
{
	uint32 x = 0;
	char *decrypted = (char *)calloc(size, sizeof(char));
	uint32 buffersize = size;

	for (x = 0; x < size; x++)
		decrypted[x] = (buffer->buff[x] ^ ENCRYPT_KEY[x]);

	clear_buffer(buffer);
	if(add_buffer(buffer, decrypted, buffersize)) return 1;
	buffer->offset = 0;
	free(decrypted);
	return 0;
}