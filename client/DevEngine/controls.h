/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/

#ifndef DE_CONTROLS_H
#define DE_CONTROLS_H

#include "widget.h"
#include "integer.h"
#include "text.h"

typedef enum
{
	EMPTY = 0,
	BUTTON,
	LABEL,
	WINDOW
}controltypes;

//only need structs for special widgets.

typedef struct
{
	sbool canuseevent;
	text *string;
}label;

typedef struct
{
	sbool minimized;
}window;

void createbutton(widget *data, uint16 x, uint16 y, uint16 height, uint16 width, char *image);
void drawbuttons(void *wgt);

void createlabel(widget *data, uint16 x, uint16 y, uint8 size, uint8 red, uint8 blue, uint8 green, uint8 alpha, int8 canusemouse, char *labeltext);
void drawlabel(void *wgt);

void createwindow(widget *data, uint16 x, uint16 y, uint16 height, uint16 width, char *image);
void drawwindows(void *wgt);
#endif