/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/

#ifndef DE_WIDGET_H
#define DE_WIDGET_H

#include "types.h"
#include "bool.h"

typedef enum widget_flags_t widget_flags_t;
typedef struct widget_array widget_array;
typedef struct widget widget;
typedef struct canvas canvas;
typedef struct user_interface user_interface;

//the Void array to store widgets via pointer
struct widget_array
{
	widget **data;
	uint16 size;
	uint16 count;
};

//toggles for the action.

enum widget_flags_t
{
	WIDGET_IS_FOCUSED = (1 << 0),
	WIDGET_CAN_FOCUS = (1 << 1),
	WIDGET_MOUSE_OVER = (1 << 2),
	WIDGET_CLICKED = (1 << 3),
	WIDGET_MOVEABLE = (1 << 4),
	WIDGET_MOVING = (1 << 5),
	WIDGET_CAN_CLICK_BEHIND = (1 << 6),
	WIDGET_ALWAYS_USEABLE = (1 << 7),
	WIDGET_MINIMIZED = (1 << 8),
	WIDGET_CHECKED = (1 << 9),
	WIDGET_CAN_USE_EVENT = (1 << 10),
	WIDGET_FLIPPED = (1 << 11),
	WIDGET_FOCUS_CLICK = (1 << 12),
	WIDGET_FUNCTION_FOCUS = (1 << 13),
	WIDGET_IS_PASSWORD = (1 << 14),
	WIDGET_AVOID_BUFFER_UPDATE = (1 << 15),
	WIDGET_BUFFER_RESIZE = (1 << 16),
	WIDGET_IS_MULTI_LINED = (1 << 17)
};

//A UI control structure.
struct widget
{
	widget *parent;
	widget_array shown; //widgets drawn
	widget_array hidden; //widgets not drawn.
	vector2ui pos;//default position.
	vector2ui actualpos;//offset position
	vector2i imgpos;//texture position
	image *img; //image data
	sbuffer buf;// opengl buffer
	uint16 width;
	uint16 height;
	uint16 sizex;
	uint16 sizey;
	uint8 type; //control_types
	int32 value;
	lbool action;//widget_flags_t
	void *control; //special control data

	//User Events
	void(*draw)(widget *);
	void(*mouseover)(widget *);
	void(*mouseexit)(widget *);
	void(*mousepress)(widget *,int,int);
	void(*mouserelease)(widget *,int,int);
	void(*mousewheel)(widget *,int);
	void(*keypressed)(widget *,int,int);

	//system Events.
	void(*controlmousepress)(widget *,int,int);
	void(*controlmouserelease)(widget *,int,int);
	void(*controlmousewheel)(widget *,int);
	void(*controlkeypressed)(widget *,int);
	void(*controlupdatepos)(widget *);
	void(*controlmouseover)(widget *);
	void(*controlmouseexit)(widget *);
};

//The main screens Settings
struct canvas
{
	sbool clicked;
	uint8 button;
	vector2ui mouseclick;
	vector2i mousepos;
};

//the UI system Structure.
struct user_interface
{
	widget *root;
	canvas screen;
};

//used to Obtain the UI system.
user_interface widget_get_ui(void);

widget *widget_get_focused(void);
//checks if mouse is in the set move frame.
sbool widget_frame_contains(widget *control, widget *parent);

//changes the moving widgets position via mouse position.
void widget_move(int16 x, int16 y);
void widget_mouse_over(widget *control);
void widget_position_update(widget *parent);
sbool widget_usable(widget *control);

//checks if the parent is focused to do click events.
sbool widget_is_parent_focused(widget *control);

void set_widget_mouse_press_event(widget *control,uint16 index);

//sets the is on controls to focus and does action.
void widget_set_focused(widget *control, uint32 index);

//sets the is on release control release and does action.
void widget_set_release(widget *control);

//checks focused object to see if we need to check inside of it or not.
sbool widget_check_focus(void);

//sets the mouse's XY position during mouse move via event system.
void widget_set_mouse_pos(int16 x , int16 y);

//sets the user interface's click events for fall back
void widget_set_ui_click_event(int8 button, int8 clicked);

//initializes the User interface system.
void widget_init_system(void);

//switches widgets within the same array.
void widget_switch(widget_array *wgt, uint32 a, uint32 b);

//moves a shown widget to the hidden array
void widget_hide(widget *parent, uint16 index);

//moves a hidden widget to the shown array
void widget_show(widget *parent, uint16 index);

//adds a widget to a Parent in either the hidden or shown array.
void widget_add(widget *parent, widget *child);

void widget_add_hidden(widget *parent, widget *child);

//initializes a widgets Data before use.
void widget_init(widget *wgt);

//unloads a widget and its arrays
void widget_unload(widget *parent);

//initializes a widgets dormant arrays, used to save ram.
void widget_init_shown(widget *parent);

void widget_init_hidden(widget *parent);

//resizes a widgets arrays Max to allow more widgets.
void widget_hidden_resize(widget *parent, uint16 size);//for dynamic widget arrays up to 65,535, the system currently uses static.

void widget_shown_resize(widget *parent, uint16 size);//for dynamic widget arrays up to 65,535, the system currently uses static.
// clears the widgets arrays
sbool widget_clear_arrays(widget *parent);

//clears the hidden array
sbool widget_clear_hidden(widget *parent);

//clears the shown array
sbool widget_clear_shown(widget *parent);

//resizes the ID for more Z depth of Deep user interface systems for widget manager.
void widget_resize_id(uint16 **id, uint16 size);

//Draws all the shown widgets to the screen.
void widget_manager(void);

//checks if widget is in the mouse's location when click event happens
sbool widget_rect_contains(widget *control, widget *parent);

//checks if the mouse is over a widget if so do event
sbool widget_has_mouse_over(widget *control);

//focuses a widget manually
void widget_manual_focused(widget * control);

//Used to set if a widget can be focused or not.
void widget_set_focusable(widget *control, int8 boolean);

//Used to set if the widget is drag able or not
void widget_set_moveable(widget *control, int8 boolean);
//used to set a widgets click event.
void widget_set_clicked(widget *control);

//Mouse events
void widget_mouse_release(widget * control);
void widget_mouse_press(widget * control);

//default initiates. ignore these.//
//Blank Drawing call
void widget_init_draw(widget *control);
//blank key pressed
void widget_init_key_pressed(widget *control, int key, int pressed);
//default eventful mouse press
void widget_init_mouse_press(widget *control, int button, int pressed);
//default eventful mouse release
void widget_init_mouse_release(widget *control, int button, int pressed);
//blank mouse wheel event
void widget_init_mouse_wheel(widget *control, int moved);
void widget_init_mouse_over(widget *control);
void widget_init_mouse_exit(widget *control);

void widget_init_control_mouse_press(widget *control, int button, int pressed);
void widget_init_control_mouse_release(widget *control, int button, int pressed);
void widget_init_control_mouse_wheel(widget *control, int moved);
void widget_init_control_key_pressed(widget *control, int key);
void widget_init_control_update_pos(widget *control);
void widget_init_control_mouse_over(widget *control);
void widget_init_control_mouse_exit(widget *control);

#endif