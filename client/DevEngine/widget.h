/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/

#ifndef DE_WIDGET_H
#define DE_WIDGET_H

#include "types.h"
#include "integer.h"
#include "bool.h"

typedef enum widget_flags_t widget_flags_t;
typedef struct widget_void_array widget_void_array;
typedef struct widget widget;
typedef struct canvas canvas;
typedef struct user_interface user_interface;

//the Void array to store widgets via pointer
struct widget_void_array
{
	void **data;
	uint16 size;
	uint16 count;
};

//toggles for the sbool action.

enum widget_flags_t
{
	WIDGET_IS_FOCUSED = (1 << 0),
	WIDGET_CAN_FOCUS = (1 << 1),
	WIDGET_MOUSE_OVER = (1 << 2),
	WIDGET_CLICKED = (1 << 3),
	WIDGET_MOVEABLE = (1 << 4),
	WIDGET_MOVING = (1 << 5),
	WIDGET_CAN_CLICK_BEHIND = (1 << 6),
	WIDGET_ALWAYS_USEABLE= (1 << 7),
};

//A UI control structure.
struct widget
{
	//contains control special data
	void *control;

	//contains the parent of the widget.
	void *parent;

	//contains events for quick calling
	void(*draw)(void *);
	void(*mousepress)(void *,int,int);
	void(*mouserelease)(void *,int,int);
	void(*mousewheel)(void *,int);
	void(*keypressed)(void *,int,int);

	//hidden and shown arrays
	widget_void_array widgets;
	widget_void_array hidden;

	vector2ui pos;
	vector2ui actualpos;
	vector2ui originalpos;
	vector2i imgpos;
	image img;
	uint16 width;
	uint16 height;
	uint8 type;
	sbool action;//focused,WIDGET_CAN_FOCUS,mouseover,clicked,moveable,moving,canclickbehind
};

//The main screens Settings
struct canvas
{
	sbool clicked;
	uint8 button;
	vector2ui mouseclick;
	vector2i mousepos;
	void *focused; //can hold any widget
};

//the UI system Struture.
struct user_interface
{
	widget *root;
	canvas screen;
};

//used to Obtain the UI system.
user_interface widget_get_ui(void);

//checks if mouse is in the set move frame.
sbool widget_frame_contains(widget *control, widget *parent);

//changes the moving widgets position via mouse pos.
void widget_move(int16 x, int16 y);

//checks if the parent is focused to do click events.
sbool widget_is_parent_focused(widget *control);

//sets the ison controls to focus and does action.
void widget_set_focused(widget *control, widget *focused, uint32 index);

//sets the isonrelease control release and does action.
void widget_set_release(widget *control);

//checks focused object to see if we need to check inside of it or not.
sbool widget_check_focus(void);

//sets the mouses XY position during mouse move via event system.
void widget_set_mouse_pos(int16 x , int16 y);

//sets the uis clickevents for fallback
void widget_set_ui_click_event(int8 button, int8 clicked);

//initilizes the User interface system.
void widget_init_system(void);

//switchs widgets within the same array.
void widget_switch(widget_void_array *wgt, uint16 a, uint16 b);

//moves a shown widget to the hidden array
void widget_hide(widget *parent, uint16 index);

//moves a hidden widget to the shown array
void widget_show(widget *parent, uint16 index);

//adds a widget to a Parent in either the hidden or shown array.
void widget_add(widget *parent, widget *child, char hidden);

//initilizes a widgets Data before use.
void widget_init(widget *wgt);

//unloads a widget and its arrays
void widget_unload(widget *parent);

//initilizes a widgets dormant arrays, used to save ram.
void widget_init_array(widget *parent, char opt);

//resizes a widgets arrays Max to allow more widgets.
void widget_array_resize(widget *parent, char opt, uint16 size);//for dynamic widget arrays up to 65,535, the system currently uses static.

// clears the widgets arrays
sbool widget_clear_arrays(widget *parent);

//clears the hidden array
sbool widget_clear_hidden(widget *parent);

//clears the shown array
sbool widget_clear_shown(widget *parent);

//resizes the ID for more Z depth of Deep ui systems for widget manager.
void widget_resize_id(uint16 *id, uint16 size);

//Draws all the shown widgets to the screen.
void widget_manager(void);

//checks if widget is in the mouses location when click event happens
sbool widget_rect_contains(widget *control, widget *parent);

//checks if the mouse is over a widget if so do event
sbool widget_has_mouse_over(widget *control);

//focuses a widget manually
void widget_manual_focused(widget * control);

//Used to set if a widget can be focused or not.
void widget_set_focusable(widget *control, int8 boolean);

//Used to set if the widget is draggable or not
void widget_set_moveable(widget *control, int8 boolean);

//checks if the widget on the focused widget is clicked if so sets as focused if it can and does event
void widget_focused_mouse_press(void);

//checks if mouse was on widget released if so does event
void widget_focused_mouse_release(void);

//defualt check if widget was released, does event
void widget_mouse_release(void);

//defualt event if widget was clicked, does event
void widget_mouse_press(void);

//temp initiate's. ignore these.//
//Blank Drawing call
void widget_init_draw(void *wgt);
//blank keypressed
void widget_init_key_pressed(void *wgt, int key, int pressed);
//default eventful mouse press
void widget_init_mouse_press(void *wgt, int button, int pressed);
//defualt eventful mouse release
void widget_init_mouse_release(void *wgt, int button, int pressed);
//blank mouse wheel event
void widget_init_mouse_wheel(void *wgt, int moved);

#endif