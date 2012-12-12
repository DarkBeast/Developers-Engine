/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/

#ifndef _WIDGET_H
#define _WIDGET_H

#include "types.h"
#include "integer.h"

typedef struct widget
{
	void *control;
	void(*draw)(void *);
	void(*mousepress)(void *,int,int);
	void(*mouserelease)(void *,int,int);
	void(*mousewheel)(void *,int);
	void(*keypressed)(void *,int,int);
	voidarray widgets;
	voidarray hidden;
	void *parent;
	vector2i pos;
	vector2i imgpos;
	image img;
	int width;
	int height;
	uint8 focused;
	uint8 mouseover;
	uint8 clicked;
	char type;
} widget;

typedef struct
{
	uint8 clicked;
	vector2i mouseclick;
	vector2i mousepos;
	void *focused; //can hold any control # up to 65,535
}canvas;

typedef struct
{
	widget *root;
	canvas screen;
}userinterface;

userinterface getui(void);

void setmousepos(int x , int y);

void widgetinit(void);

void switchwidget(voidarray *wgt, size_t a, size_t b);

void hidewidget(widget *parent, size_t index);

void showwidget(widget *parent, size_t index);

void addtowidget(widget *parent, widget *child, char hidden);

void initwidget(widget *wgt);

void unloadwidget(widget *parent);

void initwidgetarray(widget *parent, char opt);

void widgetresize(widget *parent, char opt, size_t size);//for dynamic widget arrays, the system currently uses static.

char clearbothwidgetarrays(widget *parent);

char clearhiddenarray(widget *parent);

char clearshownarray(widget *parent);

void resizeid(size_t *id, size_t size);

void widgetmanager(void);

char widgetrectcontains(widget *control);

void isonwidgetfocused(widget *focused);

void isonwidget(void);

//tempory initiate's. ignore these.//
void initdraw(void *wgt);
void initkeypressed(void *wgt, int key, int pressed);
void initmousepress(void *wgt, int button, int pressed);
void initmouserelease(void *wgt, int button, int pressed);
void initmousewheel(void *wgt, int moved);

#endif