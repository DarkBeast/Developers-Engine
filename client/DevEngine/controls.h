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
typedef struct listbox listbox;

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
	CONTROL_LISTBOX,
	CONTROL_TEXTBOX
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
	uint8 amount;
	uint8 count;
};

struct scrollbar{
	uint32 max;
};

struct textbox{
	text *string;
	sbool cusorenabled;
	double cursorblink;
	double cursortime;
	uint16 cursorheight;
	uint16 cursorwidth;
	vector2i cursorpos;
	line_buffer cursorbuffer;
};

struct listbox{
	widget *control;
	widget **list;
	widget vbar;
	widget *select;
	widget *selectover;
	uint16 offsetx;
	uint16 offsety;
	uint16 size;
	uint16 count;
	uint16 voffset;
	uint16 max;
};

void set_control_image(widget *control, char *path);

void create_button(widget *control, widget *parent, uint16 x, uint16 y, uint16 height, uint16 width, uint16 sizey, uint16 sizex, char *path);
void draw_buttons(widget *control);
void handle_button_click(widget *control, int button, int pressed);
void handle_button_release(widget *control, int button, int pressed);
void handle_button_move(widget *control);
void handle_button_mouse_over(widget *control);
void handle_button_mouse_exit(widget *control);

void create_label(widget *control, widget *parent, uint16 x, uint16 y, uint16 width, uint16 height, uint8 red, uint8 blue, uint8 green, uint8 alpha, sbool events, uint8 fontid, uint16 maxcharspl, sbool multi_lined, char *string);
void draw_label(widget *control);
void handle_label_click(widget *control, int button, int pressed);
void handle_slabel_move(widget *control);
void handle_mlabel_move(widget *control);
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

void create_hprogressbar(widget *control, widget *parent, uint16 x, uint16 y, uint16 height, uint16 width, uint16 sizey, uint16 sizex, uint8 value, char *background, char *hbar);
void draw_hprogressbar(widget *control);
void handle_hprogressbar_click(widget *control, int button, int pressed);
void handle_hprogressbar_move(widget *control);
void handle_hprogressbars_move(widget *control);
void update_progressbar_value(widget *control,uint32 value);

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
void set_scrollbar_buttons(widget *control,void(*mousepress)(widget *,int,int));

void create_vscrollbar(widget *control, widget *parent, uint16 x, uint16 y, uint16 height, uint16 width, uint16 sizey, uint16 sizex, uint16 value, uint16 max_value, char *background,char *buttontop, char *buttonbottom,char *scrollbar);
void draw_vscrollbar(widget *control);
void handle_vscrollbar_click(widget *control, int button, int pressed);
void handle_varrowtop_click(widget *control, int button, int pressed);
void handle_varrowbottom_click(widget *control, int button, int pressed);
void handle_vbar_click(widget *control, int button, int pressed);
void handle_vscrollbar_move(widget *control);

void create_vprogressbar(widget *control, widget *parent, uint16 x, uint16 y, uint16 height, uint16 width, uint16 sizey, uint16 sizex, uint8 value, char *background, char *vbar);
void draw_vprogressbar(widget *control);
void handle_vprogressbar_click(widget *control, int button, int pressed);
void handle_vprogressbar_move(widget *control);
void handle_vprogressbars_move(widget *control);

void create_stextbox(widget *control, widget *parent, uint16 x, uint16 y, uint16 width, uint16 height, uint8 offsetx, uint8 offsety, uint16 sx, uint16 sy, uint8 red, uint8 blue, uint8 green, uint8 alpha, uint16 maxchars, uint8 fontid, sbool ispass, char *imgpath);
void draw_stextbox(widget *control);
void handle_stextbox_click(widget *control, int button, int pressed);
void handle_stextbox_move(widget *control);
void draw_stextbox_text(widget *control);
void handle_stextbox_input(widget *control, int key);
void handle_stextbox_text_move(widget *control);

void create_mtextbox(widget *control, widget *parent, uint16 x, uint16 y, uint16 width, uint16 height, uint8 offsetx, uint8 offsety, uint16 sx, uint16 sy, uint8 red, uint8 blue, uint8 green, uint8 alpha, uint16 maxchars, uint16 maxcharpl, uint8 fontid, char *imgpath);
void draw_mtextbox(widget *control);
void handle_mtextbox_click(widget *control, int button, int pressed);
void handle_mtextbox_move(widget *control);
void draw_mtextbox_text(widget *control);
void handle_mtextbox_input(widget *control, int key);
void handle_mtextbox_text_move(widget *control);

void create_listbox(widget *control, widget *parent, uint16 x, uint16 y, uint16 height, uint16 width, uint16 sizey, uint16 sizex, uint8 offsetx, uint8 offsety, uint32 amount, uint8 maxchars, uint8 fontid, uint8 red, uint8 green, uint8 blue, uint8 alpha, char *imglistbg, char *imgbg, char *imgup, char *imgdown, char *bar, char *select, char *mouseover);
void draw_listbox(widget *control);
void handle_listbox_release(widget *control, int button, int pressed);
void handle_listbox_click(widget *control, int button, int pressed);
void handle_listbox_mouse_over(widget *control);
void handle_listbox_mouse_exit(widget *control);
void handle_listbox_label_click(widget *control, int button, int pressed);
void handle_listbox_label_mouse_over(widget *control);
void handle_listbox_label_mouse_exit(widget *control);
void handle_listbox_label_move(widget *control);
void handle_listbox_scroll(widget *control, int button, int pressed);
void * get_list_data(widget *control);

#endif
