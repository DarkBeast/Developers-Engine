
#ifndef _BUFFER_H
#define _BUFFER_H
#define PACKET_SIZE    8192   // Size of packet buffer

struct buffer_t
{
  char buff[PACKET_SIZE];
  int offset;
};

int  AddBuffer(struct buffer_t *buffer, const void* source, size_t size);
void TakeBuffer(void* dest, struct buffer_t* buffer, size_t size);
int  AddString(struct buffer_t* buffer, char* str);
void TakeString(char* dest, struct buffer_t *buffer);
int  SetRawString(struct buffer_t *buffer, char* str);
void ClearBuffer(struct buffer_t *buffer);
int  AddOpcode(struct buffer_t* buffer, uint8_t opcode);
int  BufferFull(struct buffer_t* buffer, size_t size);
#endif
