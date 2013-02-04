/*******************************************************************************
* Credits:  Anthony Ianozi
******************************************************************************/

#include<string.h>
#include<stdint.h>
#include"buffer.h"
#include"integer.h"

//bytearry code
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

int set_raws_tring(buffer_t *buffer, char* str)
{
	size_t len = strlen(str)+1;
	clear_buffer(buffer);
	if(add_buffer(buffer, str, len)) return 1;
	return 0;
}

int add_opcode(buffer_t* buffer, uint8_t opcode)
{
	return add_buffer(buffer, &opcode, sizeof(opcode));
}

int  buffer_full(buffer_t* buffer, size_t size)
{
	return ((buffer->offset >= PACKET_SIZE) && (buffer->offset+size > PACKET_SIZE))? 1 : 0;
}