/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/

#ifndef _WIDGET_H
#define _WIDGET_H

#include "types.h"
#include "integer.h"

//the Void array to store widgets via pointer
typedef struct
{
	void **data;
	uint16 size;
	uint16 count;
}widget_void_array;

//A UI control structure.
typedef struct widget
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
	vector2i imgpos;
	image img;
	uint16 width;
	uint16 height;
	uint8 type;
	int8 focused;
	int8 canfocus;
	int8 mouseover;
	int8 clicked;
	int8 moveable;
	int8 moving;
	int8 canclickbehind;
} widget;

//The main screens Settings
typedef struct
{
	int8 clicked;
	int8 button;
	vector2ui mouseclick;
	vector2i mousepos;
	void *focused; //can hold any control # up to 65,535
}canvas;

//the UI system Struture.
typedef struct
{
	widget *root;
	canvas screen;
}userinterface;

//used to Obtain the UI system.
userinterface getui(void);

//checks focused object to see if we need to check inside of it or not.
char checkfocused(void);

//sets the mouses XY position during mouse move via event system.
void setmousepos(int16 x , int16 y);

//sets the uis clickevents for fallback
void setuiclickevent(int8 button, int8 clicked);

//initilizes the User interface
void widgetinit(void);

//switchs widgets within the same array.
void switchwidget(widget_void_array *wgt, uint16 a, uint16 b);

//moves a shown widget to the hidden array
void hidewidget(widget *parent, uint16 index);

//moves a hidden widget to the shown array
void showwidget(widget *parent, uint16 index);

//adds a widget to a Parent in either the hidden or shown array.
void addtowidget(widget *parent, widget *child, char hidden);

//initilizes a widgets Data before use.
void initwidget(widget *wgt);

//unloads a widget and its arrays
void unloadwidget(widget *parent);

//initilizes a widgets dormant arrays, used to save ram.
void initwidgetarray(widget *parent, char opt);

//resizes a widgets arrays Max to allow more widgets.
void widgetresize(widget *parent, char opt, uint16 size);//for dynamic widget arrays up to 65,535, the system currently uses static.

// clears the widgets arrays
char clearbothwidgetarrays(widget *parent);

//clears the hidden array
char clearhiddenarray(widget *parent);

//clears the shown array
char clearshownarray(widget *parent);

//resizes the ID for more Z depth of Deep ui systems for widget manager.
void resizeid(uint16 *id, uint16 size);

//Draws all the shown widgets to the screen.
void widgetmanager(void);

//checks if widget is in the mouses location when click event happens
char widgetrectcontains(widget *control, widget *parent);

//checks if the mouse is over a widget if so do event
char ismouseover(widget *control);

//focuses a widget manually
void focuswidget(widget * control);

//Used to set if a widget can be focused or not.
void setfocusable(widget *control, int8 boolean);

//Used to set if the widget is draggable or not
void setmoveable(widget *control, int8 boolean);

//checks if the widget on the focused widget is clicked if so sets as focused if it can and does event
void isonwidgetfocused(void);

//checks if mouse was on widget released if so does event
void isonmousereleasefocused(void);

//defualt check if widget was released, does event
void isonmousereleasewidget(void);

//defualt event if widget was clicked, does event
void isonwidget(void);

//temp initiate's. ignore these.//

//Blank Drawing call
void initdraw(void *wgt);
//blank keypressed
void initkeypressed(void *wgt, int key, int pressed);
//default eventful mouse press
void initmousepress(void *wgt, int button, int pressed);
//defualt eventful mouse release
void initmouserelease(void *wgt, int button, int pressed);
//blank mouse wheel event
void initmousewheel(void *wgt, int moved);

#endif