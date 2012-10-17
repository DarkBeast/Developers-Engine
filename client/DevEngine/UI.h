/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis    
******************************************************************************/

#ifndef _UI_H
#define _UI_H

#include "Types.h"

typedef struct gui_object
{
	void(*Draw)(void*); 
	void* object;
	Vector2i position;
} gui_object;

#endif