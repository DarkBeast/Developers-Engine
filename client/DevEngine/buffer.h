/*******************************************************************************
* Credits:  Anthony Ianozi
******************************************************************************/

#ifndef DE_BUFFER_H
#define DE_BUFFER_H

#define PACKET_SIZE    8192   // Size of packet buffer
typedef struct buffer_t buffer_t;
struct buffer_t
{
	char buff[PACKET_SIZE];
	int offset;
};

int  add_buffer(buffer_t *buffer, const void* source, size_t size);
int  add_opcode(buffer_t* buffer, uint8_t opcode);
int  add_string(buffer_t* buffer, char* str);
int  buffer_full(buffer_t* buffer, size_t size);
int  set_raw_string(buffer_t *buffer, char* str);
void clear_buffer(buffer_t *buffer);
void take_buffer(void* dest, buffer_t* buffer, size_t size);
void take_string(char* dest, buffer_t *buffer);
#endif
