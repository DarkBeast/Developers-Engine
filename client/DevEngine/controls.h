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
typedef struct radio radio;
typedef struct scrollbar scrollbar;
typedef struct textbox textbox;

enum control_types
{
	CONTROL_EMPTY = 0,
	CONTROL_BUTTON,
	CONTROL_LABEL,
	CONTROL_WINDOW,
	CONTROL_CHECKBOX,
	CONTROL_RADIO,
	CONTROL_PICTUREBOX,
	CONTROL_HSCROLL_BAR,
	CONTROL_VSCROLL_BAR,
	CONTROL_HPROGRESSBAR,
	CONTROL_VPROGRESSBAR,
	CONTROL_SPINNER,
	CONTROL_LISTBOX,
	CONTROL_TABBOX,
	CONTROL_TEXTBOX,
	CONTROL_TEXTBOX_TEXT
};

//only need structures for special widgets.
struct label{
	text *string;
};

struct window{
	framei frame;
};

struct radio{
	widget *main;
	widget **list;
	uint8 ammount;
	uint8 count;
};

struct scrollbar{
	uint32 max;
};

struct textbox{
	text *string;
	sbool cusorenabled;
	uint32 cursorblink;
	uint32 cursortime;
	uint32 cursorchar;
	uint16 cursorheight;
	uint16 cursorwidth;
	vector2i cursorpos;
};

void set_control_image(widget *control, char *path);

void create_button(widget *control, widget *parent, uint16 x, uint16 y, uint16 height, uint16 width, uint16 sizey, uint16 sizex, char *path);
void draw_buttons(widget *control);
void handle_button_click(widget *control, int button, int pressed);
void handle_button_release(widget *control, int button, int pressed);
void handle_button_move(widget *control);
void handle_button_mouse_over(widget *control);
void handle_button_mouse_exit(widget *control);

void create_label(widget *control, widget *parent, uint16 x, uint16 y, uint16 width, uint16 height, uint8 red, uint8 blue, uint8 green, uint8 alpha, sbool events, uint8 fontid, sbool multi_lined, char *string);
void draw_label(widget *control);
void handle_label_click(widget *control, int button, int pressed);
void handle_label_move(widget *control);
void update_label_string(widget *control, char *string);

void create_window(widget *control, widget *parent, uint16 x, uint16 y, uint16 height, uint16 width, uint16 sizey, uint16 sizex, char * image_path);
void create_window_framed(widget *control, widget *parent, uint16 x, uint16 y, uint16 height, uint16 width, uint16 sizey, uint16 sizex, uint16 framex, uint16 framey,uint16 frameh, uint16 framew, char *path);
void draw_windows(widget *control);
void handle_window_click(widget *control, int button, int pressed);
void handle_windows_move(widget *control);

void create_checkbox(widget *control, widget *parent, uint16 x, uint16 y, uint16 height, uint16 width, uint16 sizey, uint16 sizex, char *path);
void draw_checkbox(widget *control);
void handle_check_click(widget *control, int button, int pressed);
void handle_check_move(widget *control);

void create_radio(widget *control, widget *parent, uint16 x, uint16 y, uint16 height, uint16 width, uint16 sizey, uint16 sizex, sbool istrue, char *path);
void draw_radio(widget *control);
void reset_radio(widget *control);
void link_radio(widget *main, widget *control);
void handle_radio_click(widget *control, int button, int pressed);
void resize_radio_list(radio *controls, uint16 size);
void handle_radio_move(widget *control);

void create_hprogressbar(widget *control, widget *parent, uint16 x, uint16 y, uint16 height, uint16 width, uint16 sizey, uint16 sizex, uint8 value, char *path);
void draw_hprogressbar(widget *control);
void handle_hprogressbar_click(widget *control, int button, int pressed);
void handle_hprogressbar_move(widget *control);
void handle_hprogressbars_move(widget *control);

void create_picturebox(widget *control, widget *parent, uint16 x, uint16 y, uint16 height, uint16 width, uint16 sizey, uint16 sizex, char *path);
void draw_picturebox(widget *control);
void handle_picturebox_click(widget *control, int button, int pressed);
void update_picturebox(widget *control, uint16 x, uint16 y, uint16 imgposx, uint16 imgposy, uint16 height, uint16 width, uint16 sizex, uint16 sizey, char *path);
void handle_picturebox_move(widget *control);

void create_hscrollbar(widget *control, widget *parent, uint16 x, uint16 y, uint16 height, uint16 width, uint16 sizey, uint16 sizex, uint16 value, uint16 max_value, char *background,char *buttonleft, char *buttonright,char *scrollbar);
void draw_hscrollbar(widget *control);
void handle_hscrollbar_click(widget *control, int button, int pressed);
void handle_harrowleft_click(widget *control, int button, int pressed);
void handle_harrowright_click(widget *control, int button, int pressed);
void handle_hbar_click(widget *control, int button, int pressed);
void handle_hscrollbar_move(widget *control);

void create_vscrollbar(widget *control, widget *parent, uint16 x, uint16 y, uint16 height, uint16 width, uint16 sizey, uint16 sizex, uint16 value, uint16 max_value, char *background,char *buttontop, char *buttonbottom,char *scrollbar);
void draw_vscrollbar(widget *control);
void handle_vscrollbar_click(widget *control, int button, int pressed);
void handle_varrowtop_click(widget *control, int button, int pressed);
void handle_varrowbottom_click(widget *control, int button, int pressed);
void handle_vbar_click(widget *control, int button, int pressed);
void handle_vscrollbar_move(widget *control);

void create_vprogressbar(widget *control, widget *parent, uint16 x, uint16 y, uint16 height, uint16 width, uint16 sizey, uint16 sizex, uint8 value, char *path);
void draw_vprogressbar(widget *control);
void handle_vprogressbar_click(widget *control, int button, int pressed);
void handle_vprogressbar_move(widget *control);
void handle_vprogressbars_move(widget *control);

void create_textbox(widget *control, widget *parent, uint16 x, uint16 y, uint16 width, uint16 height, uint16 offsetx, uint16 offsety, uint16 sx, uint16 sy, uint8 red, uint8 blue, uint8 green, uint8 alpha, uint32 maxchars, uint8 fontid, sbool ispass, sbool ismulti, char *imgpath);
void draw_textbox(widget *control);
void handle_textbox_click(widget *control, int button, int pressed);
void handle_textbox_move(widget *control);
void handle_textbox_input(widget *control, int key, int mod);
void draw_textbox_text(widget *control);
void handle_textbox_text_move(widget *control);

#endif
