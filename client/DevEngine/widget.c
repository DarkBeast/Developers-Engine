/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/
#include <stdlib.h>
#include "widget.h"
#include "integer.h"
#include "globals.h"
#include "function.h"

userinterface ui;

//default get Main ui, probabaly never needed but there if someone needs everything.
userinterface getui(void)
{
	return ui;
}

//Prechecked if we are on a focused object to decide if we should check inside it or not.
sbool checkfocused(void)
{
	widget *control = (widget *)ui.screen.focused;

	if(control != NULL)
	{
		if(widgetrectcontains(control, (widget *)control->parent) == TRUE)
		{
			return TRUE;
		}
	}
	return FALSE;
}

//sets the mouses position form screen movement.
void setmousepos(int16 x , int16 y)
{
	ui.screen.mousepos.x = x;
	ui.screen.mousepos.y = y;
}

//sets the UI.events in case of fall back.
void setuiclickevent(int8 button, int8 clicked)
{
	ui.screen.clicked = clicked;
	ui.screen.mouseclick.x = ui.screen.mousepos.x;
	ui.screen.mouseclick.y = ui.screen.mousepos.y;
	ui.screen.button = button;
}

//initilizes the widget system.
void widgetinit(void)
{
	/*
	*use only once for initializing a new Root Widget.
	*root widget being that of entire screen entity's or a place holder for
	*a in game widgets. Always unload the root before reusing the root.
	*/

	// sets the ui.mouseclick to 0 to deture erroring.
	ui.screen.mouseclick.x = 0;
	ui.screen.mouseclick.y = 0;

	//sets to -1 to stop a mouse bug from accuring if something is in the top corner.
	ui.screen.mousepos.x = -1;
	ui.screen.mousepos.y = -1;

	//creates the focused items to contain widgets that are focused.
	ui.screen.focused = (void **)calloc(1, sizeof (widget));
	ui.screen.focused = NULL;
	ui.screen.clicked = 0;

	//create an initilize the ROOT which is the main screens hidden widget control.
	ui.root = (widget *)calloc(1, sizeof (widget));
	initwidget(ui.root);

	//sets the show widget array
	ui.root->widgets.data  = (void **)calloc(1, WIDGET_MAX * sizeof (widget)); //set the size of the widget array
	ui.root->widgets.count = (ui.root->widgets.count > WIDGET_MAX) ? WIDGET_MAX : ui.root->widgets.count;
	ui.root->widgets.size = WIDGET_MAX;

	//sets the hidden widget array
	ui.root->hidden.data  = (void **)calloc(1, WIDGET_MAX * sizeof (widget)); //set the size of the widget array
	ui.root->hidden.count = (ui.root->hidden.count > WIDGET_MAX) ? WIDGET_MAX : ui.root->hidden.count;
	ui.root->hidden.size = WIDGET_MAX;
}

//switchs widgets around in an array.
void switchwidget(widget_void_array *wgt, uint16 a, uint16 b)//switch's widget positions in the arrays. very useful.
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

//takes a shown widget and hides it.
void hidewidget(widget *parent, uint16 index)// Parent: the holder of the widgets to be moved, Index the widget to be moved.
{
	uint16 i;

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
		if(parent->hidden.data[i] == NULL)
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

//takes a hidden widget and shows it
void showwidget(widget *parent, uint16 index) // Parent: the holder of the widgets to be moved, Index the widget to be moved.
{
	uint16 i;

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
		if(parent->widgets.data[i] == NULL)
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

//adds a widget to another widgets array either Roots or another widget.
void addtowidget(widget *container, widget *child, char hidden)
{
	uint16 i;
	widget *parent;

	if(child == NULL)
		return;

	if(container == NULL)
	{
		parent = ui.root;
	}
	else
	{
		parent = container;
	}

	child->parent = parent;

	if(hidden == TRUE)
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
			if(parent->hidden.data[i] == NULL)
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
		else
		{
			//TODO: add error handler for overloaded array.
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
			if(parent->widgets.data[i] == NULL)
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
		else
		{
			//TODO: add error handler for overloaded array.
		}
	}
	return;
}

//used to initilize the array so when the program runs no UI Errors will happen becuase of invalid data.
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
	wgt->action = 0;
	wgt->type = 0;
}

//unloads a widget from memory
void unloadwidget(widget *parent)
{
	if(!clearbothwidgetarrays(parent))
		return;// add error message

	free(parent);
	parent = NULL;
}

//initilizes a widgets dormant arrays, we set them to dormant to save ram.
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

//resizes the ammont of widgets the show, or hidden array can hold, use if you need more than defualt.
void widgetarrayresize(widget *parent, char opt, uint16 size)
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

//Frees all the widgets in the widget arrays shown/hidden so we can free the parent.
sbool clearbothwidgetarrays(widget *parent)
{
	//will clear all widgets within the widget and the widgets of widgets. clear them in reverse
	widget *child;
	uint16 z = 0;

	if (parent == NULL){return FALSE;}

	if (parent->hidden.data != NULL)
	{
		uint16 index = 0;
		uint16 index2 = 0;

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
		uint16 index = 0;
		uint16 index2 = 0;

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

//clears just the hidden array.
sbool clearhiddenarray(widget *parent)
{
	widget *child;
	uint16 z = 0;

	if (parent == NULL){return FALSE;}

	if (parent->hidden.data != NULL)
	{
		uint16 index = 0;
		uint16 index2 = 0;

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

//clears jsut the shown array.
sbool clearshownarray(widget *parent)
{
	widget *child;
	uint16 z = 0;

	if (parent == NULL){return FALSE;}

	if (parent->widgets.data != NULL)
	{
		uint16 index = 0;
		uint16 index2 = 0;

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

//resizes the ID for the widget manager when handling deep UI systems, defualt Z is 32.
void resizeid(uint16 *id, uint16 size)
{
	uint16 *data = NULL;

	if(id == NULL)
		return;//TODO: add error handler here.

	data = (uint16 *)realloc(id, size * sizeof(uint16*));

	if (data == NULL)
		return;

	id = data;
}

//This is the Main widget That displays All the widgets in the Widget array aka shown. it Displays them via Tree branch style.
void widgetmanager(void)//used to draw the widgets onto the screen.
{
	widget *child;
	uint16 *id = NULL;
	uint16 index = 0;
	uint16 idindex;
	uint16 idsize;

	id = (uint16 *)calloc(1, 32);
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

						if(idindex + 1 >= idsize)//make sure there is not too many layers for the id array.
						{
							idsize = (uint16)next_power_of_two(idsize);
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

//checks if the widget is in the area the mouse clicked.
sbool widgetrectcontains(widget *control, widget *parent)
{
	if(ui.screen.mousepos.x < control->pos.x + parent->pos.x){return FALSE;}
	if(ui.screen.mousepos.x > control->pos.x + parent->pos.x + control->width){return FALSE;}
	if(ui.screen.mousepos.y < control->pos.y + parent->pos.y) {return FALSE;}
	if(ui.screen.mousepos.y > control->pos.y + parent->pos.y + control->height){return FALSE;}

	return TRUE;
}

//checks if the mouse is in the area the mouse moved onto.
sbool ismouseover(widget *control)
{
	widget *parent = (widget *)control->parent;
	int8 i = TRUE;
	if(ui.screen.mousepos.x < control->pos.x  + parent->pos.x){return FALSE;}
	if(ui.screen.mousepos.x > control->pos.x  + parent->pos.x + control->width){return FALSE;}
	if(ui.screen.mousepos.y < control->pos.y  + parent->pos.y) {return FALSE;}
	if(ui.screen.mousepos.y > control->pos.y  + parent->pos.y + control->height){return FALSE;}

	if(bitget(control->action,canfocus) == TRUE)
	{
		if(bitget(control->action,isfocused) != TRUE)
		{
			while(i == TRUE)
			{
				if(parent != ui.root)
				{
					if(bitget(parent->action,canfocus) == TRUE && bitget(parent->action,isfocused) == TRUE)
					{
						i = FALSE;
						return TRUE;
					}
					else
					{
						parent = (widget *)parent->parent;
					}
				}
				else
				{
					i = FALSE;
					return FALSE;
				}
			}
		}
		else
		{
			return TRUE;
		}
	}
	else
	{
		parent = (widget *)control->parent;

		if(parent == ui.root)
		{
			return TRUE;
		}

		while(i == TRUE)
		{
			if(parent != ui.root)
			{
				if(bitget(parent->action,canfocus) == TRUE && bitget(parent->action,isfocused) == TRUE)
				{
					i = FALSE;
					return TRUE;
				}
				else
				{
					parent = (widget *)parent->parent;
				}
			}
			else
			{
				i = FALSE;
				return FALSE;
			}
		}
	}
	return FALSE; //if can't find any then FALSE
}

//Manually sets a widget to Focus
void focuswidget(widget * control)
{
	if(control != ui.root)
	{
		widget *parent = (widget *)control->parent;
		uint16 i = 0;
		if(bitget(control->action,canfocus) == TRUE)
		{
			for (i = 0; i < parent->widgets.count; i++)
			{
				if(control == parent->widgets.data[i])
				{
					if(i != parent->widgets.count -1)
					{
						switchwidget(&parent->widgets, i, parent->widgets.count - 1);
					}
				}
			}
			bitset(control->action,isfocused);
			ui.screen.focused = control;
		}
	}
}

//function to allow change of fucusability not needed but helpful to some. Default is FALSE.
void setfocusable(widget *control, int8 boolean)
{
	if(boolean == TRUE)
	{
		bitset(control->action,canfocus);
	}
	else
	{
		bitclear(control->action,canfocus);
	}
}

//allows you to set the widget to draggable stat or to not drag, Default is FALSE.
void setmoveable(widget *control, int8 boolean)
{
	if(boolean == TRUE)
	{
		bitset(control->action, moveable);
	}
	else
	{
		bitclear(control->action, moveable);
	}
}

//checks if the Click was on a widget inside of the focused widget.
void isonwidgetfocused(void)
{
	widget *focused = (widget *)ui.screen.focused;
	widget *child;
	widget *child2; // used to determine if focusable
	uint16 index = 0;
	uint16 index2 = 0;

	if(focused->widgets.data != NULL)
	{
		for( index = 0; index < focused->widgets.count; ++index)
		{
			child2 = (widget *)focused->widgets.data[index];
			if(bitget(child2->action,canfocus) == TRUE)        //next child if focusable
			{
				if(widgetrectcontains(child2, (widget *)child2->parent)== TRUE)// if focusable check if in range
				{
					child = (widget *)focused->widgets.data[index];

					if(child->widgets.data != NULL)
					{
						for( index2 = 0; index2 < child->widgets.count; ++index2)
						{
							child2 = (widget *)child->widgets.data[index2];
							if(bitget(child2->action,canfocus) == TRUE)        //next child if focusable
							{
								if(widgetrectcontains(child2, (widget *)child2->parent)== TRUE)// if focusable check if in range
								{
									child = (widget *)child->widgets.data[index2];

									if(child->widgets.data == NULL) //if in range check if widget array is null or not.
									{
										child2 = (widget *)child->parent;

										if(child != child2->widgets.data[child2->widgets.count - 1])
										{
											switchwidget(&child2->widgets,index2,child2->widgets.count - 1);
										}
										bitclear(focused->action,isfocused);
										bitset(child->action,isfocused);
										ui.screen.focused = child;
										bitset(child->action,clicked);
										child->mousepress(child,ui.screen.button, ui.screen.clicked);
										return;
									}

									index2 = 0; //if not null repeat with new child index till one is found.
								}
							}
							else
							{
								if(widgetrectcontains(child2, (widget *)child2->parent)== TRUE) //check to see if the non-focusable widget is in range.
								{
									child = (widget *)child->widgets.data[index2];

									if(child->widgets.data == NULL)//if in range make sure widget array is null
									{
										bitset(child->action,clicked);
										child->mousepress(child,ui.screen.button, ui.screen.clicked);
										return;
									}

									index2 = 0; //if not null repeat with new child index till one is found.
								}
							}
						}

						if(bitget(child->action,canfocus) == TRUE)
						{
							child2 = (widget *)child->parent;

							if(child != child2->widgets.data[child2->widgets.count - 1])
							{
								switchwidget(&child2->widgets,index,child2->widgets.count - 1);
							}
							bitclear(focused->action,isfocused);
							bitset(child->action,isfocused);
							ui.screen.focused = child;
							bitset(child->action,clicked);
							child->mousepress(child,ui.screen.button, ui.screen.clicked);
							return;
						}
						else
						{
							bitset(child->action,clicked);
							child->mousepress(child,ui.screen.button, ui.screen.clicked);
							return;
						}
					}
					else
					{
						child2 = (widget *)child->parent;

						if(child != child2->widgets.data[child2->widgets.count - 1])
						{
							switchwidget(&child2->widgets,index,child2->widgets.count - 1);
						}
						bitclear(focused->action,isfocused);
						bitset(child->action,isfocused);
						ui.screen.focused = child;
						bitset(child->action,clicked);
						child->mousepress(child,ui.screen.button, ui.screen.clicked);
						return;
					}
				}
			}
			else
			{
				if(widgetrectcontains(child2, (widget *)child2->parent)== TRUE)
				{
					child = (widget *)focused->widgets.data[index];

					if(child->widgets.data != NULL)
					{
						for( index2 = 0; index2 < child->widgets.count; ++index2)
						{
							child2 = (widget *)child->widgets.data[index2];
							if(bitget(child2->action,canfocus) == TRUE)        //next child if focusable
							{
								if(widgetrectcontains(child2, (widget *)child2->parent)== TRUE)// if focusable check if in range
								{
									child = (widget *)child->widgets.data[index2];

									if(child->widgets.data == NULL) //if in range check if widget array is null or not.
									{
										child2 = (widget *)child->parent;

										if(child != child2->widgets.data[child2->widgets.count - 1])
										{
											switchwidget(&child2->widgets,index2,child2->widgets.count - 1);
										}
										bitclear(focused->action,isfocused);
										bitset(child->action,isfocused);
										ui.screen.focused = child;
										bitset(child->action,clicked);
										child->mousepress(child,ui.screen.button, ui.screen.clicked);
										return;
									}

									index2 = 0; //if not null repeat with new child index till one is found.
								}
							}
							else
							{
								if(widgetrectcontains(child2, (widget *)child2->parent)== TRUE) //check to see if the non-focusable widget is in range.
								{
									child = (widget *)child->widgets.data[index2];

									if(child->widgets.data == NULL)//if in range make sure widget array is null
									{
										bitset(child->action,clicked);
										child->mousepress(child,ui.screen.button, ui.screen.clicked);
										return;
									}

									index2 = 0; //if not null repeat with new child index till one is found.
								}
							}
						}

						if(bitget(child->action,canfocus) == TRUE)
						{
							child2 = (widget *)child->parent;

							if(child != child2->widgets.data[child2->widgets.count - 1])
							{
								switchwidget(&child2->widgets,index,child2->widgets.count - 1);
							}
							bitclear(focused->action,isfocused);
							bitset(child->action,isfocused);
							ui.screen.focused = child;
							bitset(child->action,clicked);
							child->mousepress(child,ui.screen.button, ui.screen.clicked);
							return;
						}
						else
						{
							bitset(child->action,clicked);
							child->mousepress(child,ui.screen.button, ui.screen.clicked);
							return;
						}
					}
					else
					{
						bitset(child->action,clicked);
						child->mousepress(child,ui.screen.button, ui.screen.clicked);
						return;
					}
				}
			}
		}
		bitset(focused->action,clicked);
		focused->mousepress(focused,ui.screen.button, ui.screen.clicked);
		return;
	}
}

//checkls to see if the focused widget was unclicked.
void isonmousereleasefocused(void)
{
	widget *focused = (widget *)ui.screen.focused;
	widget *child;
	widget *child2; // used to determine if focusable
	uint16 index = 0;
	uint16 index2 = 0;

	if(focused->widgets.data != NULL)
	{
		for( index = 0; index < focused->widgets.count; ++index)
		{
			if(widgetrectcontains((widget *)focused->widgets.data[index],(widget *)focused)== TRUE)// if focusable check if in range
			{
				child = (widget *)focused->widgets.data[index];

				if(child->widgets.data != NULL)
				{
					for( index2 = 0; index2 < child->widgets.count; ++index2)
					{
						child2 = (widget *)child->widgets.data[index2];

						if(widgetrectcontains(child2, (widget *)child2->parent)== TRUE)// if focusable check if in range
						{
							child = (widget *)child->widgets.data[index2];

							if(child->widgets.data == NULL) //if in range check if widget array is null or not.
							{
								bitclear(child->action,clicked);
								child->mouserelease(child,ui.screen.button, ui.screen.clicked);
								return;
							}

							index2 = 0; //if not null repeat with new child index till one is found.
						}
					}
					bitclear(child->action,clicked);
					child->mouserelease(child,ui.screen.button, ui.screen.clicked);
					return;
				}
				else
				{
					bitclear(child->action,clicked);
					child->mouserelease(child,ui.screen.button, ui.screen.clicked);
					return;
				}
			}
		}
	}
	bitclear(focused->action,clicked);
	focused->mouserelease(focused,ui.screen.button, ui.screen.clicked);
	return;
}

//checks if a widget was unclicked Default check, looks through any widget shown.if unclicked then do event.
void isonmousereleasewidget(void)
{
	widget *child;
	widget *child2; // used to determine if focusable
	uint16 index = 0;
	uint16 index2 = 0;

	if(ui.root->widgets.data != NULL)
	{
		for( index = 0; index < ui.root->widgets.count; ++index)
		{
			if(widgetrectcontains((widget *)ui.root->widgets.data[index], (widget *)ui.root)== TRUE)// if focusable check if in range
			{
				child = (widget *)ui.root->widgets.data[index];

				if(child->widgets.data != NULL)
				{
					for( index2 = 0; index2 < child->widgets.count; ++index2)
					{
						child2 = (widget *)child->widgets.data[index2];

						if(widgetrectcontains(child2, (widget *)child2->parent)== TRUE)// if focusable check if in range
						{
							child = (widget *)child->widgets.data[index2];

							if(child->widgets.data == NULL) //if in range check if widget array is null or not.
							{
								bitclear(child->action,clicked);
								child->mouserelease(child,ui.screen.button, ui.screen.clicked);
								return;
							}

							index2 = 0; //if not null repeat with new child index till one is found.
						}
					}
					bitclear(child->action,clicked);
					child->mouserelease(child,ui.screen.button, ui.screen.clicked);
					return;
				}
				else
				{
					bitclear(child->action,clicked);
					child->mouserelease(child,ui.screen.button, ui.screen.clicked);
					return;
				}
			}
		}
	}
	bitclear(ui.root->action,clicked);
	ui.root->mouserelease(ui.root,ui.screen.button, ui.screen.clicked);
	return;
}

//checks if the widget was clicked Default, if clicked set as focused if focusable or set parent if clickable then do event.
void isonwidget(void)
{
	widget *child;
	widget *child2; // used to determine if focusable
	uint16 index = 0;
	uint16 index2 = 0;
	widget *focused = (widget *)ui.screen.focused; //used to set focus to FALSE for the old focus before switch.

	if(ui.root->widgets.data != NULL)
	{
		for( index = 0; index < ui.root->widgets.count; ++index)
		{
			child2 = (widget *)ui.root->widgets.data[index];
			if(bitget(child2->action,canfocus) == TRUE)        //next child if focusable
			{
				if(widgetrectcontains(child2, (widget *)child2->parent)== TRUE)// if focusable check if in range
				{
					child = (widget *)ui.root->widgets.data[index];

					if(child->widgets.data != NULL)
					{
						for( index2 = 0; index2 < child->widgets.count; ++index2)
						{
							child2 = (widget *)child->widgets.data[index2];
							if(bitget(child2->action,canfocus) == TRUE)        //next child if focusable
							{
								if(widgetrectcontains(child2, (widget *)child2->parent)== TRUE)// if focusable check if in range
								{
									child = (widget *)child->widgets.data[index2];

									if(child->widgets.data == NULL) //if in range check if widget array is null or not.
									{
										child2 = (widget *)child->parent;

										if(child != child2->widgets.data[child2->widgets.count - 1])
										{
											switchwidget(&child2->widgets,index2,child2->widgets.count - 1);
										}
										if(focused != NULL)
										{
											bitclear(focused->action,isfocused);
										}

										bitset(child->action,isfocused);
										ui.screen.focused = child;
										bitset(child->action,clicked);
										child->mousepress(child,ui.screen.button, ui.screen.clicked);
										return;
									}

									index2 = 0; //if not null repeat with new child index till one is found.
								}
							}
							else
							{
								if(widgetrectcontains(child2, (widget *)child2->parent)== TRUE) //check to see if the non-focusable widget is in range.
								{
									child = (widget *)child->widgets.data[index2];

									if(child->widgets.data == NULL)//if in range make sure widget array is null
									{
										bitset(child->action,clicked);
										child->mousepress(child,ui.screen.button, ui.screen.clicked);
										return;
									}

									index2 = 0; //if not null repeat with new child index till one is found.
								}
							}
						}

						if(bitget(child->action,canfocus) == TRUE)
						{
							child2 = (widget *)child->parent;

							if(child != child2->widgets.data[child2->widgets.count - 1])
							{
								switchwidget(&child2->widgets,index,child2->widgets.count - 1);
							}
							if(focused != NULL)
							{
								bitclear(focused->action,isfocused);
							}
							bitset(child->action,isfocused);
							ui.screen.focused = child;
							bitset(child->action,clicked);
							child->mousepress(child,ui.screen.button, ui.screen.clicked);
							return;
						}
						else
						{
							bitset(child->action,clicked);
							child->mousepress(child,ui.screen.button, ui.screen.clicked);
							return;
						}
					}
					else
					{
						child2 = (widget *)child->parent;

						if(child != child2->widgets.data[child2->widgets.count - 1])
						{
							switchwidget(&child2->widgets,index,child2->widgets.count - 1);
						}
						if(focused != NULL)
						{
							bitclear(focused->action,isfocused);
						}
						bitset(child->action,isfocused);
						ui.screen.focused = child;
						bitset(child->action,clicked);
						child->mousepress(child,ui.screen.button, ui.screen.clicked);
						return;
					}
				}
			}
			else
			{
				if(widgetrectcontains(child2, (widget *)child2->parent)== TRUE)
				{
					child = (widget *)ui.root->widgets.data[index];
					if(child->widgets.data != NULL)
					{
						for( index2 = 0; index2 < child->widgets.count; ++index2)
						{
							child2 = (widget *)child->widgets.data[index2];
							if(bitget(child2->action,canfocus) == TRUE)        //next child if focusable
							{
								if(widgetrectcontains(child2, (widget *)child2->parent)== TRUE)// if focusable check if in range
								{
									child = (widget *)child->widgets.data[index2];

									if(child->widgets.data == NULL) //if in range check if widget array is null or not.
									{
										child2 = (widget *)child->parent;

										if(child != child2->widgets.data[child2->widgets.count - 1])
										{
											switchwidget(&child2->widgets,index2,child2->widgets.count - 1);
										}
										if(focused != NULL)
										{
											bitclear(focused->action,isfocused);
										}
										bitset(child->action,isfocused);
										ui.screen.focused = child;
										bitset(child->action,clicked);
										child->mousepress(child,ui.screen.button, ui.screen.clicked);
										return;
									}

									index2 = 0; //if not null repeat with new child index till one is found.
								}
							}
							else
							{
								if(widgetrectcontains(child2, (widget *)child2->parent)== TRUE) //check to see if the non-focusable widget is in range.
								{
									child = (widget *)child->widgets.data[index2];

									if(child->widgets.data == NULL)//if in range make sure widget array is null
									{
										bitset(child->action,clicked);
										child->mousepress(child,ui.screen.button, ui.screen.clicked);
										return;
									}

									index2 = 0; //if not null repeat with new child index till one is found.
								}
							}
						}

						if(bitget(child->action,canfocus) == TRUE)
						{
							child2 = (widget *)child->parent;

							if(child != child2->widgets.data[child2->widgets.count - 1])
							{
								switchwidget(&child2->widgets,index,child2->widgets.count - 1);
							}
							if(focused != NULL)
							{
								bitclear(focused->action,isfocused);
							}
							bitset(child->action,isfocused);
							ui.screen.focused = child;
							bitset(child->action,clicked);
							child->mousepress(child,ui.screen.button, ui.screen.clicked);
							return;
						}
						else
						{
							bitset(child->action,clicked);
							child->mousepress(child,ui.screen.button, ui.screen.clicked);
							return;
						}
					}
					else
					{
						bitset(child->action,clicked);
						child->mousepress(child,ui.screen.button, ui.screen.clicked);
						return;
					}
				}
			}
		}
	}
	bitset(ui.root->action,clicked);
	ui.root->mousepress(ui.root,ui.screen.button, ui.screen.clicked);
	return;
}

//only used to initiate a empty call to avoid Errors when there events are not setup.
//initpress and release have background clickable events to parent objects if turned on.
void initmousepress(void *wgt, int button, int pressed)
{
	widget *control = (widget *)wgt;
	if(bitget(control->action,canclickbehind) == TRUE)
	{
		widget *parent = (widget *)control->parent;

		if(parent != NULL && parent != ui.root)
		{
			//TODO: finish widget background button press
			parent->mousepress(parent,button,pressed);
		}
	}
}

void initmouserelease(void *wgt, int button, int pressed){}
void initmousewheel(void *wgt, int moved){}
void initkeypressed(void *wgt, int key, int pressed){}
void initdraw(void *wgt){}