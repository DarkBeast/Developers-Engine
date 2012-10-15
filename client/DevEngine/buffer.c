
#include<string.h>
#include<stdint.h>
#include"buffer.h"
#include"integer.h"

/*NOTE: All buffer functions return one on overflow*/
int AddBuffer(struct buffer_t *buffer, const void* source, size_t size)
{
	if((buffer->offset >= PACKET_SIZE) && (buffer->offset+size > PACKET_SIZE)) return 1;
	memcpy(buffer->buff+buffer->offset, source, size);
	buffer->offset += size;
	return 0;
}

void TakeBuffer(void* dest, struct buffer_t* buffer, size_t size)
{
	memcpy(dest, buffer->buff+buffer->offset, size);
	buffer->offset += size;
}

int AddString(struct buffer_t* buffer, char* str)
{ /*[str_len][str]*/
	Uint8 len = strlen(str)+1;
	if(AddBuffer(buffer, &len, sizeof(len))) return 1;
	if(AddBuffer(buffer, str, len)) return 1;
	return 0;
}

void TakeString(char* dest, struct buffer_t *buffer)
{
	Uint8 len;
	TakeBuffer(&len, buffer, sizeof(len));
	TakeBuffer(dest, buffer, len);
}

void ClearBuffer(struct buffer_t *buffer)
{
	memset(buffer->buff, 0, PACKET_SIZE);
	buffer->offset=0;
}

int SetRawString(struct buffer_t *buffer, char* str)
{
	Uint8 len = strlen(str)+1;
	ClearBuffer(buffer);
	if(AddBuffer(buffer, str, len)) return 1;
	return 0;
}

int AddOpcode(struct buffer_t* buffer, uint8_t opcode)
{
	return AddBuffer(buffer, &opcode, sizeof(opcode));
}

int  BufferFull(struct buffer_t* buffer, size_t size)
{
	return ((buffer->offset >= PACKET_SIZE) && (buffer->offset+size > PACKET_SIZE))? 1 : 0;
}
