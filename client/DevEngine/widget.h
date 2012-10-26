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
	vector2i pos;
	vector2f imgpos; 
	image image;
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
}canvas;


typedef struct 
{
	widget **data;
	size_t size;
	size_t count;
}widgetarray;

void initwidget(void  *wgt);
void AddtoWidget(void  *wgt,void *sub);
void switchwidget(void  *wgt, int a, int b);

void initmousepress(void *wgt, int button, int pressed);//starter types, leave blank to prevent error on widget creation.
void initmouserelease(void *wgt, int button, int pressed);
void initmousewheel(void *wgt, int moved);
void initkeypressed(void *wgt, int key, int pressed);
void initdraw(void *wgt);// end of starter types

#endif