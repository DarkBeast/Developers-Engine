/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/

#ifndef DE_CONTROLS_H
#define DE_CONTROLS_H

#include "widget.h"
#include "integer.h"
#include "text.h"

typedef enum control_types control_types;
typedef struct label label;
typedef struct window window;

enum control_types
{
	CONTROL_EMPTY = 0,
	CONTROL_BUTTON,
	CONTROL_LABEL,
	CONTROL_WINDOW,
	CONTROL_CHECKBOX,
	CONTROL_RADIO,
	CONTROL_TEXTBOX,
	CONTROL_PICTUREBOX,
	CONTROL_HSCROLL_BAR,
	CONTROL_VSCROLL_BAR,
	CONTROL_PROGRESSBAR,
	CONTROL_FRAME,
	CONTROL_SPINNER,
	CONTROL_DROP_DOWN_LIST,
	CONTROL_LISTBOX,
	CONTROL_SLIDER,
	CONTROL_TABBOX
};

//only need structs for special widgets.

struct label{
	text *string;
};

struct window{
	framei frame;
};

void create_button(widget *data, uint16 x, uint16 y, uint16 height, uint16 width, char *image_path);
void draw_buttons(void *wgt);

void create_label(widget *data, uint16 x, uint16 y, uint8 size, uint8 red, uint8 blue, uint8 green, uint8 alpha, int8 can_use_mouse, char *label_text);
void draw_label(void *wgt);

void create_window(widget *data, uint16 x, uint16 y, uint16 height, uint16 width, char *image_path);
void create_window_framed(widget *data, uint16 x, uint16 y, uint16 height, uint16 width, uint16 framex, uint16 framey,uint16 frameh, uint16 framew, char *image_path);
void draw_windows(void *wgt);

void create_checkbox(widget *data, uint16 x, uint16 y, uint16 height, uint16 width, char *image_path);
void draw_checkbox(void *wgt);

#endif