/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/

#ifndef _WIDGET_H
#define _WIDGET_H

#include "types.h"
#include "integer.h"

typedef struct
{
	void **data;
	uint16 size;
	uint16 count;
}widget_void_array;

typedef struct widget
{
	void *control;
	void *parent;
	void(*draw)(void *);
	void(*mousepress)(void *,int,int);
	void(*mouserelease)(void *,int,int);
	void(*mousewheel)(void *,int);
	void(*keypressed)(void *,int,int);
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
} widget;

typedef struct
{
	int8 clicked;
	int8 button;
	vector2ui mouseclick;
	vector2i mousepos;
	void *focused; //can hold any control # up to 65,535
}canvas;

typedef struct
{
	widget *root;
	canvas screen;
}userinterface;

userinterface getui(void);

char checkfocused(void);

void setmousepos(int16 x , int16 y);

void setuiclickevent(int8 button, int8 clicked);

void widgetinit(void);

void switchwidget(widget_void_array *wgt, uint16 a, uint16 b);

void hidewidget(widget *parent, uint16 index);

void showwidget(widget *parent, uint16 index);

void addtowidget(widget *parent, widget *child, char hidden);

void initwidget(widget *wgt);

void unloadwidget(widget *parent);

void initwidgetarray(widget *parent, char opt);

void widgetresize(widget *parent, char opt, uint16 size);//for dynamic widget arrays up to 65,535, the system currently uses static.

char clearbothwidgetarrays(widget *parent);

char clearhiddenarray(widget *parent);

char clearshownarray(widget *parent);

void resizeid(uint16 *id, uint16 size);

void widgetmanager(void);

char widgetrectcontains(widget *control);

char ismouseover(widget *control);

void focuswidget(widget * control);

//Used to set if a widget can be focused or not.
void setfocusable(widget *control, int8 boolean);

void setmoveable(widget *control, int8 boolean);

void isonwidgetfocused(void);

void isonmousereleasefocused(void);

void isonmousereleasewidget(void);

void isonwidget(void);

//temp initiate's. ignore these.//
void initdraw(void *wgt);
void initkeypressed(void *wgt, int key, int pressed);
void initmousepress(void *wgt, int button, int pressed);
void initmouserelease(void *wgt, int button, int pressed);
void initmousewheel(void *wgt, int moved);

#endif