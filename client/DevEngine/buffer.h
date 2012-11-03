/*******************************************************************************
* Credits:  Anthony Ianozi
******************************************************************************/

#ifndef _BUFFER_H
#define _BUFFER_H
#define PACKET_SIZE    8192   // Size of packet buffer

struct buffer_t
{
	char buff[PACKET_SIZE];
	int offset;
};

int  addbuffer(struct buffer_t *buffer, const void* source, size_t size);
int  addopcode(struct buffer_t* buffer, uint8_t opcode);
int  addstring(struct buffer_t* buffer, char* str);
int  bufferfull(struct buffer_t* buffer, size_t size);
int  setrawstring(struct buffer_t *buffer, char* str);
void clearbuffer(struct buffer_t *buffer);
void takebuffer(void* dest, struct buffer_t* buffer, size_t size);
void takestring(char* dest, struct buffer_t *buffer);
#endif
