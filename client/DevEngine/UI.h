#ifndef _UI_H
#define _UI_H
#include "Types.h"

typedef struct gui_object
{
  void(*Draw)(void*); 
  void* object;
  Coords position;
} gui_object;



#endif