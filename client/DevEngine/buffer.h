/*******************************************************************************
* Credits:  Anthony Ianozi
******************************************************************************/

#ifndef _BUFFER_H
#define _BUFFER_H
#define PACKET_SIZE    8192   // Size of packet buffer

typedef struct buffer_t
{
	char buff[PACKET_SIZE];
	int offset;
}buffer_t;

int  addbuffer(buffer_t *buffer, const void* source, size_t size);
int  addopcode(buffer_t* buffer, uint8_t opcode);
int  addstring(buffer_t* buffer, char* str);
int  bufferfull(buffer_t* buffer, size_t size);
int  setrawstring(buffer_t *buffer, char* str);
void clearbuffer(buffer_t *buffer);
void takebuffer(void* dest, buffer_t* buffer, size_t size);
void takestring(char* dest, buffer_t *buffer);
#endif
