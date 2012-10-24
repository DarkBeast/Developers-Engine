/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis    
******************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include "widget.h"
#include "integer.h"
#include "globals.h"

void widgetcreate(canvas *screen, widgetarray *parent)
{

	screen->mouseclick.x = 0;
	screen->mouseclick.y = 0;
	screen->mousepos.x = 0;
	screen->mousepos.y = 0;
	screen->focusedcontrol = 0; //can hold any control # up to 65,535 

	parent->data  = (widget **)calloc(WIDGET_MAX, WIDGET_MAX * sizeof(widget)); //set the size of the widget array
	parent->count = (parent->count > WIDGET_MAX) ? WIDGET_MAX : parent->count;
	parent->size = WIDGET_MAX;
}

void switchwidget(voidarray *wgt, int a, int b)
{
	widget *clone;

	if(wgt == NULL || a < 0 || b < 0 || a >= wgt->size || b >= wgt->size)
		return;

	clone = (widget *)wgt->data[a];
	wgt->data[a] = wgt->data[b];
	wgt->data[b] = clone;

}

void hidewidget(widget *parent, widget *child, size_t index)
{
	int i;

	if(child == NULL || parent == NULL || index < 0)
		return;

	if(parent->hidden.count == 0)
	{//if we are starting a new array then create the first object.
		parent->hidden.data[0] = child; 
		parent->hidden.count += 1;
		parent->widgets.data[index] = NULL;
		return;
	}

	for( i = 0; i < parent->hidden.count; ++i)
	{//search through existing count for Nulled data if Null then use.
		if(parent->hidden.data[i] = NULL)
		{
			parent->hidden.data[i] = child; 
			parent->widgets.data[index] = NULL;
			return;
		}
	}

	if( parent->hidden.count < parent->hidden.size)
	{// if none are nulled then check to see if we reach limit if not add to the count.
		parent->hidden.data[parent->hidden.count] = child;
		parent->hidden.count += 1;
		parent->widgets.data[index] = NULL;
		return;
	}

}


void showwidget(widget *parent, widget *child, size_t index)
{
	int i;

	if(child == NULL || parent == NULL || index < 0)
		return;

	if(parent->widgets.count == 0)
	{
		parent->widgets.data[0] = child; 
		parent->widgets.count += 1;
		parent->hidden.data[index] == NULL;
		return;
	}
	
	for( i = 0; i < parent->widgets.count; i++)
	{
		if(parent->widgets.data[i] = NULL)
		{
			parent->widgets.data[i] = child;
			parent->hidden.data[index] == NULL;
			return;
		}
	}

	if( parent->widgets.count < parent->widgets.size)
	{
		parent->widgets.data[parent->widgets.count] = child;
		parent->widgets.count += 1;
		parent->hidden.data[index] == NULL;
		return;
	}


}



void AddtoWidget(widget *parent, widget *child, char hidden)
{
	int i;

	if(child == NULL || parent == NULL)
		return;

	if(hidden)
	{
		if(parent->hidden.count == 0)
		{//if we are starting a new array then create the first object.
			parent->hidden.data[0] = child; 
			parent->hidden.count += 1;
			return;
		}

		for( i = 0; i < parent->hidden.count; ++i)
		{//search through existing count for Nulled data if Null then use.
			if(parent->hidden.data[i] = NULL)
			{
				parent->hidden.data[i] = child; 
				return;
			}
		}

		if( parent->hidden.count +1 < parent->hidden.size)
		{// if none are nulled then check to see if we reach limit if not add to the count.
			parent->hidden.data[parent->hidden.count] = child;
			parent->hidden.count += 1;
			return;
		}
	}else{
		if(parent->widgets.count == 0)
		{
			parent->widgets.data[0] = child; 
			parent->widgets.count += 1;
			return;
		}

		for( i = 0; i < parent->widgets.count; i++)
		{
			if(parent->widgets.data[i] = NULL)
			{
				parent->widgets.data[i] = child;
				return;
			}
		}

		if( parent->widgets.count +1 < parent->widgets.size)
		{
			parent->widgets.data[parent->widgets.count] = child;
			parent->widgets.count += 1;
			return;
		}
	}
	return; //TODO: add error handler for overloaded array.
}

void initwidget(widget *wgt)//intilizes the widget so we can then use it error free.
{
	if (wgt == NULL)
		return;          //TODO: add error checker
	wgt->type = NULL;
	wgt->draw = &initdraw;
	wgt->mousepress = &initmousepress;
	wgt->mouserelease = &initmouserelease;
	wgt->mousewheel = &initmousewheel;
	wgt->keypressed = &initkeypressed;
	wgt->widgets.data = NULL;
	wgt->widgets.count = 0;
	wgt->widgets.size = 0;
	wgt->hidden.data = NULL;
	wgt->hidden.count = 0;
	wgt->hidden.size = 0;
	wgt->pos.x = 0;
	wgt->pos.y = 0;
	wgt->imgpos.x = 0;
	wgt->imgpos.y = 0;
	wgt->visisble = FALSE;
	wgt->canuse = FALSE;
	wgt->draggable = FALSE;
}

void initmousepress(widget *wgt, int button, int pressed){}
void initmouserelease(widget *wgt, int button, int pressed){}
void initmousewheel(widget *wgt, int moved){}
void initkeypressed(widget *wgt, int key, int pressed){}
void initdraw(widget *wgt){}