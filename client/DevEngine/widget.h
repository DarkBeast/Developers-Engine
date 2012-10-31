/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/

#ifndef _UI_H
#define _UI_H

#include "types.h"
#include "controls.h"

typedef struct
{
	void *type;
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
	char visisble;
	char canuse;
	char draggable;
} widget;

typedef struct
{
	vector2i mouseclick;
	vector2i mousepos;
	uint16 focusedcontrol; //can hold any control # up to 65,535
	uint8 clicked;
}canvas;

void widgetinit(canvas *screen, widget *parent);//see function for details
void switchwidget(voidarray *wgt, size_t a, size_t b);
void hidewidget(widget *parent, size_t index);
void showwidget(widget *parent, size_t index);
void addtowidget(widget *parent, widget *child, char hidden);
void initwidget(widget *wgt);
char clearbothwidgetarrays(widget *parent);
char clearhiddenarray(widget *parent);
char clearshownarray(widget *parent);
void initwidgetarray(widget *parent, char opt);
void widgetarrayresize(widget *parent, char opt, size_t size);//for dynamic widget arrays, the system currently uses static.
void unloadwidget(widget *parent, size_t index);

void initmousepress(void *wgt, int button, int pressed);
void initmouserelease(void *wgt, int button, int pressed);
void initmousewheel(void *wgt, int moved);
void initkeypressed(void *wgt, int key, int pressed);
void initdraw(void *wgt);

widget *root;
canvas *screen;
#endif