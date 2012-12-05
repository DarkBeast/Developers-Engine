/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/

#ifndef _UI_H
#define _UI_H

#include "types.h"
#include "controls.h"

typedef struct
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

char clearbothwidgetarrays(widget *parent);
char clearhiddenarray(widget *parent);
char clearshownarray(widget *parent);
char widgetrectcontains(widget *control);
userinterface getui(void);
void addtowidget(widget *parent, widget *child, char hidden);
void hidewidget(widget *parent, size_t index);
void initdraw(void *wgt);
void initkeypressed(void *wgt, int key, int pressed);
void initmousepress(void *wgt, int button, int pressed);
void initmouserelease(void *wgt, int button, int pressed);
void initmousewheel(void *wgt, int moved);
void inituserinterface(void);//see function for details
void initwidget(widget *wgt);
void initwidgetarray(widget *parent, char opt);
void isonwidget(void);
void isonwidgetfocused(widget *focused);
void resizeid(size_t *id, size_t size);
void showwidget(widget *parent, size_t index);
void switchwidget(voidarray *wgt, size_t a, size_t b);
void unloadwidget(widget *parent);
void widgetmanager(void);
void widgetresize(widget *parent, char opt, size_t size);//for dynamic widget arrays, the system currently uses static.
void setmousepos(int x , int y);

#endif