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
void Takebuffer(void* dest, struct buffer_t* buffer, size_t size);
int  addstring(struct buffer_t* buffer, char* str);
void Takestring(char* dest, struct buffer_t *buffer);
int  setrawstring(struct buffer_t *buffer, char* str);
void clearbuffer(struct buffer_t *buffer);
int  addopcode(struct buffer_t* buffer, uint8_t opcode);
int  bufferfull(struct buffer_t* buffer, size_t size);
#endif
