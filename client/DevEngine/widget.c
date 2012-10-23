/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis    
******************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include "widget.h"
#include "integer.h"
#include "globals.h"

void switchwidget(widget *wgt, int a, int b)
{
	widget *clone;

clone = (widget *)wgt->widgets[a];
wgt->widgets[a] = wgt->widgets[b];
wgt->widgets[b] = clone;

}

void AddtoWidget(widget *wgt,widget *sub)
{


}

void initwidget(widget *wgt)
	{
	  if (wgt == NULL)
	    return;          //TODO: add error checker

	wgt->draw = &initdraw;
	wgt->mousepress = &initmousepress;
	wgt->mouserelease = &initmouserelease;
	wgt->mousewheel = &initmousewheel;
	wgt->keypressed = &initkeypressed;
	wgt->widgets = NULL;
	wgt->pos.x = 0;
	wgt->pos.y = 0;
	wgt->imgpos.x = 0;
	wgt->imgpos.y = 0;
	wgt->visisble = false;
    wgt->canuse = false;
    wgt->draggable = false;
	wgt->count = 0;
	wgt->size = 0;
	}

void initmousepress(widget *wgt, int button, int pressed){}
void initmouserelease(widget *wgt, int button, int pressed){}
void initmousewheel(widget *wgt, int moved){}
void initkeypressed(widget *wgt, int key, int pressed){}
void initdraw(widget *wgt){}