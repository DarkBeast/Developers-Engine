/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/

#ifndef _UICONTROLS_H
#define _UICONTROLS_H

#include "types.h"

//only need types for special widgets.
typedef struct
{//TODO:Update to bit Boolean.
	uint8 click;
}button;

typedef struct
{
	uint8 toggle;
}label;

void drawbuttons(void * wgt);

#define Empty 0;
#define BUTTON 1;
#endif