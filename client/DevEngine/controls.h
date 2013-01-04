/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/

#ifndef DE_CONTROLS_H
#define DE_CONTROLS_H

#include "widget.h"
#include "integer.h"
#include "text.h"

#define EMPTY 0;
#define BUTTON 1;
#define LABEL 2;
//only need types for special widgets.

typedef struct
{
	sbool canuseevent;
	text *string;
}label;

//typedef struct
//{
//}window;

void createbutton(widget *data, uint16 x, uint16 y, uint16 height, uint16 width, char *image);
void drawbuttons(void *wgt);

void createlabel(widget *data, uint16 x, uint16 y, uint8 size, uint8 red, uint8 blue, uint8 green, uint8 alpha, int8 canusemouse, char *labeltext);
void drawlabel(void *wgt);
#endif