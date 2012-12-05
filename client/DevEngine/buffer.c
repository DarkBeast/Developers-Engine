/*******************************************************************************
* Credits:  Anthony Ianozi
******************************************************************************/

#include<string.h>
#include<stdint.h>
#include"buffer.h"
#include"integer.h"

//bytearry code
/*NOTE: All buffer functions return one on overflow*/
int addbuffer(buffer_t *buffer, const void* source, size_t size)
{
	if((buffer->offset >= PACKET_SIZE) && (buffer->offset+size > PACKET_SIZE)) return 1;
	memcpy(buffer->buff+buffer->offset, source, size);
	buffer->offset += size;
	return 0;
}

void takebuffer(void* dest, buffer_t* buffer, size_t size)
{
	memcpy(dest, buffer->buff+buffer->offset, size);
	buffer->offset += size;
}

int addstring( buffer_t* buffer, char* str)
{
	size_t len = strlen(str)+1;
	if(addbuffer(buffer, &len, sizeof(len))) return 1;
	if(addbuffer(buffer, str, len)) return 1;
	return 0;
}

void takestring(char* dest, buffer_t *buffer)
{
	size_t len;
	takebuffer(&len, buffer, sizeof(len));
	takebuffer(dest, buffer, len);
}

void clearbuffer(buffer_t *buffer)
{
	memset(buffer->buff, 0, PACKET_SIZE);
	buffer->offset=0;
}

int setrawstring(buffer_t *buffer, char* str)
{
	size_t len = strlen(str)+1;
	clearbuffer(buffer);
	if(addbuffer(buffer, str, len)) return 1;
	return 0;
}

int addopcode(buffer_t* buffer, uint8_t opcode)
{
	return addbuffer(buffer, &opcode, sizeof(opcode));
}

int  bufferfull(buffer_t* buffer, size_t size)
{
	return ((buffer->offset >= PACKET_SIZE) && (buffer->offset+size > PACKET_SIZE))? 1 : 0;
}