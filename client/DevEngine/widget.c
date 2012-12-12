/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include "widget.h"
#include "integer.h"
#include "globals.h"
#include "function.h"

userinterface ui;

userinterface getui(void)
{
	return ui;
}

void setmousepos(int x , int y)
{
	ui.screen.mousepos.x = x;
	ui.screen.mousepos.y = y;
}

void widgetinit(void)
{
	/*
	*use only once for initializing a new Root Widget.
	*root widget being that of entire screen entity's or a place holder for
	*a in game widgets. Always unload the root before reusing the root.
	*/

	ui.screen.mouseclick.x = 0;
	ui.screen.mouseclick.y = 0;
	ui.screen.mousepos.x = 0;
	ui.screen.mousepos.y = 0;
	ui.screen.focused = (void *)calloc(1, sizeof (widget*));
	ui.screen.clicked = 0;

	ui.root = (widget *)calloc(1, sizeof (widget));
	initwidget(ui.root);
	ui.root->widgets.data  = (void **)calloc(1, WIDGET_MAX * sizeof (widget)); //set the size of the widget array
	ui.root->widgets.count = (ui.root->widgets.count > WIDGET_MAX) ? WIDGET_MAX : ui.root->widgets.count;
	ui.root->widgets.size = WIDGET_MAX;

	ui.root->hidden.data  = (void **)calloc(1, WIDGET_MAX * sizeof (widget)); //set the size of the widget array
	ui.root->hidden.count = (ui.root->hidden.count > WIDGET_MAX) ? WIDGET_MAX : ui.root->hidden.count;
	ui.root->hidden.size = WIDGET_MAX;
}

void switchwidget(voidarray *wgt, size_t a, size_t b)//switch's widget positions in the arrays. very useful.
{
	widget *clone;

	if(wgt == NULL || a >= wgt->size || b >= wgt->size)
		return;

	clone = (widget *)wgt->data[a];
	wgt->data[a] = wgt->data[b];
	wgt->data[b] = clone;
	clone = NULL;
	free(clone);
}

void hidewidget(widget *parent, size_t index)// Parent: the holder of the widgets to be moved, Index the widget to be moved.
{
	size_t i;

	if( parent == NULL || parent->widgets.data[index] == NULL)
		return;

	if(parent->hidden.size == 0)
	{
		initwidgetarray(parent, 0);
	}

	if(parent->hidden.count == 0)
	{//if we are starting a new array then create the first object.
		parent->hidden.data[0] = parent->widgets.data[index];
		parent->hidden.count += 1;
		parent->widgets.data[index] = NULL;
		return;
	}

	for( i = 0; i < parent->hidden.count; ++i)
	{//search through existing count for Nulled data if Null then use.
		if(parent->hidden.data[i] = NULL)
		{
			parent->hidden.data[i] = parent->widgets.data[index];
			parent->widgets.data[index] = NULL;
			return;
		}
	}

	if( parent->hidden.count < parent->hidden.size)
	{// if none are nulled then check to see if we reach limit if not add to the count.
		parent->hidden.data[parent->hidden.count] = parent->widgets.data[index];
		parent->hidden.count += 1;
		parent->widgets.data[index] = NULL;
		return;
	}
}

void showwidget(widget *parent, size_t index) // Parent: the holder of the widgets to be moved, Index the widget to be moved.
{
	size_t i;

	if(parent == NULL || parent->hidden.data[index] == NULL)
		return;

	if(parent->widgets.size == 0)
	{
		initwidgetarray(parent, 1);
	}

	if(parent->widgets.count == 0)
	{
		parent->widgets.data[0] = parent->hidden.data[index];
		parent->widgets.count += 1;
		parent->hidden.data[index] = NULL;
		return;
	}

	for( i = 0; i < parent->widgets.count; i++)
	{
		if(parent->widgets.data[i] = NULL)
		{
			parent->widgets.data[i] = parent->hidden.data[index];
			parent->hidden.data[index] = NULL;
			return;
		}
	}

	if( parent->widgets.count < parent->widgets.size)
	{
		parent->widgets.data[parent->widgets.count] = parent->hidden.data[index];
		parent->widgets.count += 1;
		parent->hidden.data[index] = NULL;
		return;
	}
}

void addtowidget(widget *parent, widget *child, char hidden)
{
	size_t i;

	if(child == NULL)
		return;

	if(parent == NULL)
	{
		parent = ui.root;
	}

	if(hidden)
	{
		if(parent->hidden.size == 0)
		{
			initwidgetarray(parent, 0);
		}

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
		if(parent->widgets.size == 0)
		{
			initwidgetarray(parent, 1);
		}

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

void initwidget(widget *wgt)//initializes a widget so we can then use it error free.
{
	if(wgt == NULL)
		return;

	wgt->control = NULL;
	wgt->draw = &initdraw;
	wgt->mousepress = &initmousepress;
	wgt->mouserelease = &initmouserelease;
	wgt->mousewheel = &initmousewheel;
	wgt->keypressed = &initkeypressed;
	wgt->widgets.data  = NULL; //set the size of the widget array
	wgt->widgets.count = 0;
	wgt->widgets.size = 0;
	wgt->hidden.data  = NULL;
	wgt->hidden.count = 0;
	wgt->hidden.size = 0;
	wgt->parent = NULL;
	wgt->pos.x = 0;
	wgt->pos.y = 0;
	wgt->imgpos.x = 0;
	wgt->imgpos.y = 0;
	wgt->width = 0;
	wgt->height = 0;
	wgt->focused = FALSE;
	wgt->mouseover = FALSE;
	wgt->clicked = FALSE;
	wgt->type = 0;
}

void unloadwidget(widget *parent)
{
	if(!clearbothwidgetarrays(parent))
		return;// add error message

	free(parent->parent);
	free(parent->keypressed);
	free(parent->mousepress);
	free(parent->mouserelease);
	free(parent->mousewheel);
	free(parent->draw);
	free(parent);
	parent = NULL;
}

void initwidgetarray(widget *parent, char opt)// only use once and if only we wanted to add a widget to a Array with no size.
{
	if(opt == 1)
	{
		parent->widgets.data  = (void **)calloc(1, WIDGET_MAX * sizeof (widget*)); //set the size of the widget array
		parent->widgets.count = (parent->widgets.count > WIDGET_MAX) ? WIDGET_MAX : parent->widgets.count;
		parent->widgets.size = WIDGET_MAX;
	}
	else
	{
		parent->hidden.data  = (void **)calloc(1, WIDGET_MAX * sizeof (widget*)); //set the size of the widget array
		parent->hidden.count = (parent->hidden.count > WIDGET_MAX) ? WIDGET_MAX : parent->hidden.count;
		parent->hidden.size = WIDGET_MAX;
	}
}

void widgetarrayresize(widget *parent, char opt, size_t size)
{
	void **data;
	if(opt == 1)
	{
		data = (void **)realloc(parent->widgets.data, size * sizeof(widget*));

		if (data == NULL)
			return;

		parent->widgets.data = data;
		parent->widgets.count = (parent->widgets.count > size) ? size : parent->widgets.count;
		parent->widgets.size =  size;
	}
	else
	{
		data = (void **)realloc(parent->hidden.data, size * sizeof(widget*));

		if (data == NULL)
			return;

		parent->hidden.data = data;
		parent->hidden.count = (parent->hidden.count > size) ? size : parent->hidden.count;
		parent->hidden.size =  size;
	}
}

char clearbothwidgetarrays(widget *parent)
{
	//will clear all widgets within the widget and the widgets of widgets. clear them in reverse
	widget *child;
	size_t z = 0;

	if (parent == NULL){return FALSE;}

	if (parent->hidden.data != NULL)
	{
		size_t index = 0;
		size_t index2 = 0;

		for (index = 0; index < parent->hidden.count; ++index)
		{
			if ( parent->hidden.data[index] != NULL)
			{
				child = (widget *)parent->hidden.data[index];

				for(index2 = 0; index2 < child->hidden.count; ++index2)
				{
					if(child ->hidden.data != NULL)
					{
						if( index2 + 1 == child->hidden.count)
						{
							free(child->hidden.data);
							child->hidden.data = NULL;
							child->hidden.count = 0;

							if (child->parent != NULL && z != 0)
							{
								child = (widget *)child->parent;
								--z;
								index2 = 0;
							}
						}
						else
						{
							if (child->hidden.data[index2] != NULL)
							{
								child = (widget *)child->hidden.data[index2];
								index2 = 0;
								++z;
							}
						}
					}
					else
					{
						free(child->hidden.data);
						child->hidden.data = NULL;
						child->hidden.count = 0;

						if (child->parent != NULL && z != 0)
						{
							child = (widget *)child->parent;
							--z;
							index2 = 0;
						}
					}
				}
			}
			free(parent->hidden.data[index]);//TODO: child->hidden.data = NULL;
		}
	}

	if (parent->widgets.data != NULL)
	{
		size_t index = 0;
		size_t index2 = 0;

		for (index = 0; index < parent->widgets.count; ++index)
		{
			if ( parent->widgets.data[index] != NULL)
			{
				child = (widget *)parent->widgets.data[index];

				for(index2 = 0; index2 < child->widgets.count; ++index2)
				{
					if(child ->widgets.data != NULL)
					{
						if( index2 + 1 == child->widgets.count)
						{
							free(child->widgets.data);
							child->widgets.data = NULL;
							child->widgets.count = 0;

							if (child->parent != NULL && z != 0)
							{
								child = (widget *)child->parent;
								--z;
								index2 = 0;
							}
						}
						else
						{
							if (child->widgets.data[index2] != NULL)
							{
								child = (widget *)child->widgets.data[index2];
								index2 = 0;
								++z;
							}
						}
					}
					else
					{
						free(child->widgets.data);
						child->widgets.data = NULL;

						if (child->parent != NULL && z != 0)
						{
							child = (widget *)child->parent;
							--z;
							index2 = 0;
						}
					}
				}
			}
			free(parent->widgets.data[index]);
		}
	}

	free(parent->widgets.data);
	free(parent->hidden.data);

	parent->hidden.data = NULL;
	parent->widgets.data = NULL;
	parent->hidden.size = 0;
	parent->widgets.size = 0;
	parent->hidden.count = 0;
	parent->widgets.count = 0;

	return TRUE;
}

char clearhiddenarray(widget *parent)
{
	widget *child;
	size_t z = 0;

	if (parent == NULL){return FALSE;}

	if (parent->hidden.data != NULL)
	{
		size_t index = 0;
		size_t index2 = 0;

		for (index = 0; index < parent->hidden.count; ++index)
		{
			if ( parent->hidden.data[index] != NULL)
			{
				child = (widget *)parent->hidden.data[index];

				for(index2 = 0; index2 < child->hidden.count; ++index2)
				{
					if(child ->hidden.data != NULL)
					{
						if( index2 + 1 == child->hidden.count)
						{
							free(child->hidden.data);
							child->hidden.data = NULL;
							child->hidden.count = 0;

							if (child->parent != NULL && z != 0)
							{
								child = (widget *)child->parent;
								--z;
								index2 = 0;
							}
						}
						else
						{
							if (child->hidden.data[index2] != NULL)
							{
								child = (widget *)child->hidden.data[index2];
								index2 = 0;
								++z;
							}
						}
					}
					else
					{
						free(child->hidden.data);
						child->hidden.data = NULL;
						child->hidden.count = 0;

						if (child->parent != NULL && z != 0)
						{
							child = (widget *)child->parent;
							--z;
							index2 = 0;
						}
					}
				}
			}
			free(parent->hidden.data[index]);//TODO: child->hidden.data = NULL;
		}
	}

	free(parent->hidden.data);

	parent->hidden.data = NULL;
	parent->hidden.size = 0;
	parent->hidden.count = 0;

	return TRUE;
}

char clearshownarray(widget *parent)
{
	widget *child;
	size_t z = 0;

	if (parent == NULL){return FALSE;}

	if (parent->widgets.data != NULL)
	{
		size_t index = 0;
		size_t index2 = 0;

		for (index = 0; index < parent->widgets.count; ++index)
		{
			if ( parent->widgets.data[index] != NULL)
			{
				child = (widget *)parent->widgets.data[index];

				for(index2 = 0; index2 < child->widgets.count; ++index2)
				{
					if(child ->widgets.data != NULL)
					{
						if( index2 + 1 == child->widgets.count)
						{
							free(child->widgets.data);
							child->widgets.data = NULL;
							child->widgets.count = 0;

							if (child->parent != NULL && z != 0)
							{
								child = (widget *)child->parent;
								--z;
								index2 = 0;
							}
						}
						else
						{
							if (child->widgets.data[index2] != NULL)
							{
								child = (widget *)child->widgets.data[index2];
								index2 = 0;
								++z;
							}
						}
					}
					else
					{
						free(child->widgets.data);
						child->widgets.data = NULL;

						if (child->parent != NULL && z != 0)
						{
							child = (widget *)child->parent;
							--z;
							index2 = 0;
						}
					}
				}
			}
			free(parent->widgets.data[index]);
		}
	}

	free(parent->widgets.data);

	parent->widgets.data = NULL;
	parent->widgets.size = 0;
	parent->widgets.count = 0;

	return TRUE;
}

void resizeid(size_t *id, size_t size)
{
	size_t *data = NULL;

	if(id == NULL)
		return;//TODO: add error handler here.

	data = (size_t *)realloc(id, size * sizeof(size_t*));

	if (data == NULL)
		return;

	id = data;
}

void widgetmanager(void)//used to draw the widgets onto the screen.
{
	widget *child;
	size_t *id = NULL;
	size_t index = 0;
	size_t idindex;
	size_t idsize;

	id = (size_t *)calloc(1, 32);
	idsize = 32;
	idindex = 0;

	for( index = 0; index < ui.root->widgets.count; ++index)
	{
		child = (widget *)ui.root->widgets.data[index];
		child->draw(child);//then draw there parent.

		for(id[idindex] = 0; id[idindex] < child->widgets.count; ++id[idindex])
		{
			if(child ->widgets.data != NULL)
			{
				if(id[idindex] + 1 >= child->widgets.count && idindex != 0)
				{
					id[idindex] = 0;

					--idindex;
					child = (widget *)child->parent;
				}
				else
				{
					if (child->widgets.data[id[idindex]] != NULL)
					{
						child = (widget *)child->widgets.data[id[idindex]];
						child->draw(child);//then draw the child.

						++idindex;//we set the z buffer index to know which layer we are in

						if(idindex < idsize)//make sure there is not too many layers for the id array.
						{
							idsize = next_power_of_two(idsize);
							resizeid(id,idsize);
						}
						id[idindex] = 0;//set the new ID index to 0
					}
				}
			}
			else
			{
				if(idindex != 0)
				{
					id[idindex] = 0;
					--idindex;
					child = (widget *)child->parent;
				}
			}
		}
	}

	free(id);
}

char widgetrectcontains(widget *control)
{
	if(ui.screen.mousepos.x < control->pos.x ){return FALSE;}
	if(ui.screen.mousepos.x > control->pos.x + control->width){return FALSE;}
	if(ui.screen.mousepos.y < control->pos.y ) {return FALSE;}
	if(ui.screen.mousepos.y > control->pos.y + control->height){return FALSE;}

	return TRUE;
}

void isonwidgetfocused(widget *focused)
{
	widget *child;
	size_t index = 0;
	size_t index2 = 0;

	if(focused->widgets.data != NULL)
	{
		for( index = 0; index < focused->widgets.count; ++index)
		{
			child = (widget *)focused->widgets.data[index];

			if(widgetrectcontains(child)== TRUE)
			{
				if(child->widgets.data != NULL)
				{
					for( index2 = 0; index2 < child->widgets.count; ++index2)
					{
						if(widgetrectcontains((widget *)child->widgets.data[index2])== TRUE)
						{
							child = (widget *)child->widgets.data[index2];

							if(child->widgets.data == NULL)
							{
								ui.screen.focused = child;
								return;
							}

							index2 = 0;
						}
					}

					ui.screen.focused = child;
					return;
				}
				else
				{
					ui.screen.focused = child;
					return;
				}
			}
		}
	}
}

void isonwidget(void)
{
	widget *child;
	size_t index = 0;
	size_t index2 = 0;

	if(ui.root->widgets.data != NULL)
	{
		for( index = 0; index < ui.root->widgets.count; ++index)
		{
			child = (widget *)ui.root->widgets.data[index];

			if(widgetrectcontains(child)== TRUE)
			{
				if(child->widgets.data != NULL)
				{
					for( index2 = 0; index2 < child->widgets.count; ++index2)
					{
						if(widgetrectcontains((widget *)child->widgets.data[index2])== TRUE)
						{
							child = (widget *)child->widgets.data[index2];

							if(child->widgets.data == NULL)
							{
								ui.screen.focused = child;
								return;
							}

							index2 = 0;
						}
					}

					ui.screen.focused = child;
					return;
				}
				else
				{
					ui.screen.focused = child;
					return;
				}
			}
		}
	}
}

void initmousepress(void *wgt, int button, int pressed){}
void initmouserelease(void *wgt, int button, int pressed){}
void initmousewheel(void *wgt, int moved){}
void initkeypressed(void *wgt, int key, int pressed){}
void initdraw(void *wgt){}