/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/
#include <stdlib.h>
#include "widget.h"
#include "integer.h"
#include "globals.h"
#include "function.h"
#include "controls.h"
#include "Render.h"
user_interface ui;

//default get Main ui, probabaly never needed but there if someone needs everything.
user_interface widget_get_ui(void)
{
	return ui;
}

sbool widget_frame_contains(widget *control, widget *parent)
{
	switch(control->type){
	case CONTROL_BUTTON:
	case CONTROL_LABEL:

		if(ui.screen.mousepos.x < control->pos.x + parent->pos.x){return FALSE; break;}
		if(ui.screen.mousepos.x > control->pos.x + parent->pos.x + control->width){return FALSE; break;}
		if(ui.screen.mousepos.y < control->pos.y + parent->pos.y) {return FALSE; break;}
		if(ui.screen.mousepos.y > control->pos.y + parent->pos.y + control->height){return FALSE; break;}
		return TRUE;
		break;
	case CONTROL_WINDOW:{
		window *data = (window *) control->control;

		if(ui.screen.mousepos.x < control->pos.x + parent->pos.x + data->frame.x){return FALSE; break;}
		if(ui.screen.mousepos.x > control->pos.x + parent->pos.x + data->frame.w){return FALSE; break;}
		if(ui.screen.mousepos.y < control->pos.y + parent->pos.y + data->frame.y) {return FALSE; break;}
		if(ui.screen.mousepos.y > control->pos.y + parent->pos.y + data->frame.h){return FALSE; break;}
		return TRUE;
						}
						break;

	default: return FALSE; break;
	}
}

void widget_move(int16 x, int16 y)
{
	widget *control = (widget *)ui.screen.focused;
	int32 tempx;
	int32 tempy;
	uint16 dis;

	if(control != NULL){
		if(control->action & WIDGET_MOVING){
			tempx = x - ui.screen.mousepos.x ;
			tempy = y - ui.screen.mousepos.y ;

			if(control->pos.x + tempx <= 0 || control->pos.y + tempy <= 0){
				return;
			}

			if(control->pos.x + control->width + tempx>= get_screen_width() || control->pos.y + control->height + tempy >= get_screen_height()){
				return;
			}

			control->pos.x = control->pos.x + tempx ;

			control->pos.y = control->pos.y + tempy ;
		}
	}
}

//Prechecked if we are on a focused object to decide if we should check inside it or not.
sbool widget_check_focus(void)
{
	widget *control = (widget *)ui.screen.focused;

	if(control != NULL){
		if(control->action & WIDGET_MOVING){
			control->action &= ~(WIDGET_MOVING);
		}

		if(widget_rect_contains(control, (widget *)control->parent)){
			return TRUE;
		}
	}
	return FALSE;
}

//sets the mouses position form screen movement.
void widget_set_mouse_pos(int16 x , int16 y)
{
	ui.screen.mousepos.x = x;
	ui.screen.mousepos.y = y;
}

//sets the UI.events in case of fall back.
void widget_set_ui_click_event(int8 button, int8 clicked)
{
	ui.screen.clicked = clicked;
	ui.screen.mouseclick.x = ui.screen.mousepos.x;
	ui.screen.mouseclick.y = ui.screen.mousepos.y;
	ui.screen.button = button;
}

//initilizes the widget system.
void widget_init_system(void)
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
	widget_init(ui.root);

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
void widget_switch(widget_void_array *wgt, uint16 a, uint16 b)//switch's widget positions in the arrays. very useful.
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
void widget_hide(widget *parent, uint16 index)// Parent: the holder of the widgets to be moved, Index the widget to be moved.
{
	uint16 i;

	if( parent == NULL || parent->widgets.data[index] == NULL)
		return;

	if(parent->hidden.size == 0){
		widget_init_array(parent, 0);
	}

	if(parent->hidden.count == 0){//if we are starting a new array then create the first object.
		parent->hidden.data[0] = parent->widgets.data[index];
		parent->hidden.count += 1;
		parent->widgets.data[index] = NULL;
		return;
	}

	for( i = 0; i < parent->hidden.count; ++i){//search through existing count for Nulled data if Null then use.
		if(parent->hidden.data[i] == NULL){
			parent->hidden.data[i] = parent->widgets.data[index];
			parent->widgets.data[index] = NULL;
			return;
		}
	}

	if( parent->hidden.count < parent->hidden.size){// if none are nulled then check to see if we reach limit if not add to the count.
		parent->hidden.data[parent->hidden.count] = parent->widgets.data[index];
		parent->hidden.count += 1;
		parent->widgets.data[index] = NULL;
		return;
	}
}

//takes a hidden widget and shows it
void widget_show(widget *parent, uint16 index) // Parent: the holder of the widgets to be moved, Index the widget to be moved.
{
	uint16 i;

	if(parent == NULL || parent->hidden.data[index] == NULL)
		return;

	if(parent->widgets.size == 0){
		widget_init_array(parent, 1);
	}

	if(parent->widgets.count == 0){
		parent->widgets.data[0] = parent->hidden.data[index];
		parent->widgets.count += 1;
		parent->hidden.data[index] = NULL;
		return;
	}

	for( i = 0; i < parent->widgets.count; i++){
		if(parent->widgets.data[i] == NULL){
			parent->widgets.data[i] = parent->hidden.data[index];
			parent->hidden.data[index] = NULL;
			return;
		}
	}

	if( parent->widgets.count < parent->widgets.size){
		parent->widgets.data[parent->widgets.count] = parent->hidden.data[index];
		parent->widgets.count += 1;
		parent->hidden.data[index] = NULL;
		return;
	}
}

//adds a widget to another widgets array either Roots or another widget.
void widget_add(widget *container, widget *child, char hidden)
{
	uint16 i;
	widget *parent;

	if(child == NULL)
		return;

	if(container == NULL){
		parent = ui.root;
	}
	else{
		parent = container;
	}

	child->parent = parent;

	if(hidden){
		if(parent->hidden.size == 0){
			widget_init_array(parent, 0);
		}

		if(parent->hidden.count == 0){//if we are starting a new array then create the first object.
			parent->hidden.data[0] = child;
			parent->hidden.count += 1;
			return;
		}

		for( i = 0; i < parent->hidden.count; ++i){//search through existing count for Nulled data if Null then use.
			if(parent->hidden.data[i] == NULL){
				parent->hidden.data[i] = child;
				return;
			}
		}

		if( parent->hidden.count +1 < parent->hidden.size){// if none are nulled then check to see if we reach limit if not add to the count.
			parent->hidden.data[parent->hidden.count] = child;
			parent->hidden.count += 1;
			return;
		}
		else{
			//TODO: add error handler for overloaded array.
		}
	}else{
		if(parent->widgets.size == 0){
			widget_init_array(parent, 1);
		}

		if(parent->widgets.count == 0){
			parent->widgets.data[0] = child;
			parent->widgets.count += 1;
			return;
		}

		for( i = 0; i < parent->widgets.count; i++){
			if(parent->widgets.data[i] == NULL){
				parent->widgets.data[i] = child;
				return;
			}
		}

		if( parent->widgets.count +1 < parent->widgets.size){
			parent->widgets.data[parent->widgets.count] = child;
			parent->widgets.count += 1;
			return;
		}
		else{
			//TODO: add error handler for overloaded array.
		}
	}
	return;
}

//used to initilize the array so when the program runs no UI Errors will happen becuase of invalid data.
void widget_init(widget *wgt)//initializes a widget so we can then use it error free.
{
	if(wgt == NULL)
		return;

	wgt->control = NULL;
	wgt->draw = &widget_init_draw;
	wgt->mousepress = &widget_init_mouse_press;
	wgt->mouserelease = &widget_init_mouse_release;
	wgt->mousewheel = &widget_init_mouse_wheel;
	wgt->keypressed = &widget_init_key_pressed;
	wgt->widgets.data  = NULL; //set the size of the widget array
	wgt->widgets.count = 0;
	wgt->widgets.size = 0;
	wgt->hidden.data  = NULL;
	wgt->hidden.count = 0;
	wgt->hidden.size = 0;
	wgt->parent = NULL;
	wgt->pos.x = 0;
	wgt->pos.y = 0;
	wgt->actualpos.x = 0;
	wgt->actualpos.y = 0;
	wgt->originalpos.x = 0;
	wgt->originalpos.y = 0;
	wgt->imgpos.x = 0;
	wgt->imgpos.y = 0;
	wgt->width = 0;
	wgt->height = 0;
	wgt->action = 0;
	wgt->type = 0;
}

//unloads a widget from memory
void widget_unload(widget *parent)
{
	if(!widget_clear_arrays(parent))
		return;// add error message

	free(parent);
	parent = NULL;
}

//initilizes a widgets dormant arrays, we set them to dormant to save ram.
void widget_init_array(widget *parent, char opt)// only use once and if only we wanted to add a widget to a Array with no size.
{
	if(opt == 1){
		parent->widgets.data  = (void **)calloc(1, WIDGET_MAX * sizeof (widget*)); //set the size of the widget array
		parent->widgets.count = (parent->widgets.count > WIDGET_MAX) ? WIDGET_MAX : parent->widgets.count;
		parent->widgets.size = WIDGET_MAX;
	}
	else{
		parent->hidden.data  = (void **)calloc(1, WIDGET_MAX * sizeof (widget*)); //set the size of the widget array
		parent->hidden.count = (parent->hidden.count > WIDGET_MAX) ? WIDGET_MAX : parent->hidden.count;
		parent->hidden.size = WIDGET_MAX;
	}
}

//resizes the ammont of widgets the show, or hidden array can hold, use if you need more than defualt.
void widget_array_resize(widget *parent, char opt, uint16 size)
{
	void **data;
	if(opt == 1){
		data = (void **)realloc(parent->widgets.data, size * sizeof(widget*));

		if (data == NULL)
			return;

		parent->widgets.data = data;
		parent->widgets.count = (parent->widgets.count > size) ? size : parent->widgets.count;
		parent->widgets.size =  size;
	}
	else{
		data = (void **)realloc(parent->hidden.data, size * sizeof(widget*));

		if (data == NULL)
			return;

		parent->hidden.data = data;
		parent->hidden.count = (parent->hidden.count > size) ? size : parent->hidden.count;
		parent->hidden.size =  size;
	}
}

//Frees all the widgets in the widget arrays shown/hidden so we can free the parent.
sbool widget_clear_arrays(widget *parent)
{
	//will clear all widgets within the widget and the widgets of widgets. clear them in reverse
	widget *child;
	uint16 z = 0;

	if (parent == NULL){return FALSE;}

	if (parent->hidden.data != NULL){
		uint16 index = 0;
		uint16 index2 = 0;

		for (index = 0; index < parent->hidden.count; ++index){
			if ( parent->hidden.data[index] != NULL){
				child = (widget *)parent->hidden.data[index];

				for(index2 = 0; index2 < child->hidden.count; ++index2){
					if(child ->hidden.data != NULL){
						if( index2 + 1 == child->hidden.count){
							free(child->hidden.data);
							child->hidden.data = NULL;
							child->hidden.count = 0;

							if (child->parent != NULL && z != 0){
								child = (widget *)child->parent;
								--z;
								index2 = 0;
							}
						}
						else{
							if (child->hidden.data[index2] != NULL){
								child = (widget *)child->hidden.data[index2];
								index2 = 0;
								++z;
							}
						}
					}
					else{
						free(child->hidden.data);
						child->hidden.data = NULL;
						child->hidden.count = 0;

						if (child->parent != NULL && z != 0){
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

	if (parent->widgets.data != NULL){
		uint16 index = 0;
		uint16 index2 = 0;

		for (index = 0; index < parent->widgets.count; ++index){
			if ( parent->widgets.data[index] != NULL){
				child = (widget *)parent->widgets.data[index];

				for(index2 = 0; index2 < child->widgets.count; ++index2){
					if(child ->widgets.data != NULL){
						if( index2 + 1 == child->widgets.count){
							free(child->widgets.data);
							child->widgets.data = NULL;
							child->widgets.count = 0;

							if (child->parent != NULL && z != 0){
								child = (widget *)child->parent;
								--z;
								index2 = 0;
							}
						}
						else{
							if (child->widgets.data[index2] != NULL){
								child = (widget *)child->widgets.data[index2];
								index2 = 0;
								++z;
							}
						}
					}
					else{
						free(child->widgets.data);
						child->widgets.data = NULL;

						if (child->parent != NULL && z != 0){
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
sbool widget_clear_hidden(widget *parent)
{
	widget *child;
	uint16 z = 0;

	if (parent == NULL){return FALSE;}

	if (parent->hidden.data != NULL){
		uint16 index = 0;
		uint16 index2 = 0;

		for (index = 0; index < parent->hidden.count; ++index){
			if ( parent->hidden.data[index] != NULL){
				child = (widget *)parent->hidden.data[index];

				for(index2 = 0; index2 < child->hidden.count; ++index2){
					if(child ->hidden.data != NULL){
						if( index2 + 1 == child->hidden.count){
							free(child->hidden.data);
							child->hidden.data = NULL;
							child->hidden.count = 0;

							if (child->parent != NULL && z != 0){
								child = (widget *)child->parent;
								--z;
								index2 = 0;
							}
						}
						else{
							if (child->hidden.data[index2] != NULL){
								child = (widget *)child->hidden.data[index2];
								index2 = 0;
								++z;
							}
						}
					}
					else{
						free(child->hidden.data);
						child->hidden.data = NULL;
						child->hidden.count = 0;

						if (child->parent != NULL && z != 0){
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
sbool widget_clear_shown(widget *parent)
{
	widget *child;
	uint16 z = 0;

	if (parent == NULL){return FALSE;}

	if (parent->widgets.data != NULL){
		uint16 index = 0;
		uint16 index2 = 0;

		for (index = 0; index < parent->widgets.count; ++index){
			if ( parent->widgets.data[index] != NULL){
				child = (widget *)parent->widgets.data[index];

				for(index2 = 0; index2 < child->widgets.count; ++index2){
					if(child ->widgets.data != NULL){
						if( index2 + 1 == child->widgets.count){
							free(child->widgets.data);
							child->widgets.data = NULL;
							child->widgets.count = 0;

							if (child->parent != NULL && z != 0){
								child = (widget *)child->parent;
								--z;
								index2 = 0;
							}
						}
						else{
							if (child->widgets.data[index2] != NULL){
								child = (widget *)child->widgets.data[index2];
								index2 = 0;
								++z;
							}
						}
					}
					else{
						free(child->widgets.data);
						child->widgets.data = NULL;

						if (child->parent != NULL && z != 0){
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
void widget_resize_id(uint16 *id, uint16 size)
{
	uint16 *data = NULL;

	if(id == NULL)
		return;//TODO: add error handler here.

	data = (uint16 *)realloc(id, size * sizeof(uint16));

	if (data == NULL)
		return;

	id = data;
}

//This is the Main widget That displays All the widgets in the Widget array aka shown. it Displays them via Tree branch style.
void widget_manager(void)//used to draw the widgets onto the screen.
{
	widget *child;
	uint16 *id;
	uint16 index = 0;
	uint16 idindex;
	uint16 idsize;

	id = (uint16 *)calloc(1, 32 * sizeof(uint16));
	idsize = 32;
	idindex = 0;
	id[idindex] = 0;

	for( index = 0; index < ui.root->widgets.count; ++index){
		child = (widget *)ui.root->widgets.data[index];
		child->draw(child);//then draw there parent.

		while(id[idindex] <= child->widgets.count){
			if(child ->widgets.data != NULL){
				if(id[idindex]  >= child->widgets.count && idindex != 0){
					id[idindex] = 0;

					--idindex;
					child = (widget *)child->parent;
				}
				else{
					if(!id[idindex] >= child->widgets.count){
						if (child->widgets.data[id[idindex]] != NULL){
							child = (widget *)child->widgets.data[id[idindex]];
							child->draw(child);//then draw the child.
							++id[idindex];
							++idindex;//we set the z buffer index to know which layer we are in

							if(idindex + 1 >= idsize){//make sure there is not too many layers for the id array.
								idsize = (uint16)next_power_of_two(idsize);
								widget_resize_id(id,idsize);
							}
							id[idindex] = 0;//set the new ID index to 0
						}
					}
					else{
						++id[idindex];
					}
				}
			}
			else{
				if(idindex != 0){
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
sbool widget_rect_contains(widget *control, widget *parent)
{
	if(ui.screen.mousepos.x < control->pos.x + parent->pos.x){return FALSE;}
	if(ui.screen.mousepos.x > control->pos.x + parent->pos.x + control->width){return FALSE;}
	if(ui.screen.mousepos.y < control->pos.y + parent->pos.y) {return FALSE;}
	if(ui.screen.mousepos.y > control->pos.y + parent->pos.y + control->height){return FALSE;}

	return TRUE;
}

sbool widget_is_parent_focused(widget *control)
{
	widget *parent = (widget *)control->parent;
	int8 i = TRUE;

	if(control->action & WIDGET_ALWAYS_USEABLE){
		return TRUE;
	}

	while(i){
		if(parent != ui.root){
			if(parent->action & WIDGET_CAN_FOCUS){
				if(parent->action & WIDGET_IS_FOCUSED){
					i = FALSE;
					return TRUE;
				}
				else{
					widget_manual_focused(parent);
					parent->action |= WIDGET_CLICKED;

					parent->mousepress(parent,ui.screen.button, ui.screen.clicked);
				}
			}
			else{
				parent = (widget *)parent->parent;
			}
		}
		else{
			i = FALSE;
			return FALSE;
		}
	}
}

void widget_set_release(widget *control)
{
	control->action &= ~(WIDGET_CLICKED);
	control->mouserelease(control,ui.screen.button, ui.screen.clicked);
}

void widget_set_focused(widget *control, widget *focused, uint32 index)
{
	widget *parent = (widget *)control->parent;

	if(control != parent->widgets.data[parent->widgets.count - 1]){
		widget_switch(&parent->widgets,index,parent->widgets.count - 1);
	}

	if(focused != NULL){
		focused->action &= ~(WIDGET_IS_FOCUSED);
	}

	control->action |= WIDGET_IS_FOCUSED;
	ui.screen.focused = control;
	control->action |= WIDGET_CLICKED;

	if(control->action & WIDGET_MOVEABLE){
		if(widget_frame_contains(control,parent)){
			control->action |= WIDGET_MOVING;
		}
	}
	control->mousepress(control,ui.screen.button, ui.screen.clicked);
}

//checks if the mouse is in the area the mouse moved onto.
sbool widget_has_mouse_over(widget *control)
{
	widget *parent = (widget *)control->parent;
	int8 i = TRUE;

	if(!widget_rect_contains(control,parent)){
		return FALSE;
	}

	if(control->action & WIDGET_ALWAYS_USEABLE){
		return TRUE;
	}

	if(control->action & WIDGET_CAN_FOCUS){
		if(!control->action & WIDGET_IS_FOCUSED){
			while(i){
				if(parent != ui.root){
					if(parent->action & WIDGET_CAN_FOCUS && parent->action & WIDGET_IS_FOCUSED){
						i = FALSE;
						return TRUE;
					}
					else{
						parent = (widget *)parent->parent;
					}
				}
				else{
					i = FALSE;
					return FALSE;
				}
			}
		}
		else{
			return TRUE;
		}
	}
	else{
		parent = (widget *)control->parent;

		if(parent == ui.root){
			return TRUE;
		}

		while(i){
			if(parent != ui.root){
				if(parent->action & WIDGET_CAN_FOCUS && parent->action & WIDGET_IS_FOCUSED){
					i = FALSE;
					return TRUE;
				}
				else{
					parent = (widget *)parent->parent;
				}
			}
			else{
				i = FALSE;
				return FALSE;
			}
		}
	}
	return FALSE; //if can't find any then FALSE
}

//Manually sets a widget to Focus
void widget_manual_focused(widget * control)
{
	widget *focused = (widget *)ui.screen.focused;

	if(control != ui.root){
		widget *parent = (widget *)control->parent;
		uint16 i = 0;
		if(control->action & WIDGET_CAN_FOCUS){
			for (i = 0; i < parent->widgets.count; i++){
				if(control == parent->widgets.data[i]){
					if(i != parent->widgets.count -1){
						widget_switch(&parent->widgets, i, parent->widgets.count - 1);
					}
				}
			}

			if(focused != NULL){
				focused->action &= ~(WIDGET_IS_FOCUSED);
			}

			control->action |= WIDGET_IS_FOCUSED;
			ui.screen.focused = control;
		}
	}
}

//function to allow change of fucusability not needed but helpful to some. Default is FALSE.
void widget_set_focusable(widget *control, int8 boolean)
{
	if(boolean){
		control->action |= WIDGET_CAN_FOCUS;
	}
	else{
		control->action &= ~(WIDGET_CAN_FOCUS);
	}
}

//allows you to set the widget to draggable stat or to not drag, Default is FALSE.
void widget_set_moveable(widget *control, int8 boolean)
{
	if(boolean){
		control->action |= WIDGET_MOVEABLE;
	}
	else{
		control->action &= ~(WIDGET_MOVEABLE);
	}
}

//checks if the Click was on a widget inside of the focused widget.
void widget_focused_mouse_press(void)
{
	widget *focused = (widget *)ui.screen.focused;
	widget *child;
	widget *child2; // used to determine if focusable
	uint16 index = 0;
	uint16 index2 = 0;

	if(focused->widgets.data != NULL){
		for( index = 0; index < focused->widgets.count; ++index){
			child2 = (widget *)focused->widgets.data[index];
			if(child2->action & WIDGET_CAN_FOCUS){//next child if focusable
				if(widget_rect_contains(child2, (widget *)child2->parent)){// if focusable check if in range
					child = (widget *)focused->widgets.data[index];

					if(child->widgets.data != NULL){
						while(index2 < child->widgets.count){
							child2 = (widget *)child->widgets.data[index2];
							if(child2->action & WIDGET_CAN_FOCUS){//next child if focusable
								if(widget_rect_contains(child2, (widget *)child2->parent)){// if focusable check if in range
									child = (widget *)child->widgets.data[index2];

									if(child->widgets.data == NULL){//if in range check if widget array is null or not.
										widget_set_focused(child,focused,index2);
										return;
									}

									index2 = 0; //if not null repeat with new child index till one is found.
									continue;
								}
								++index2;
							}
							else{
								if(widget_rect_contains(child2, (widget *)child2->parent)){//check to see if the non-focusable widget is in range.
									child = (widget *)child->widgets.data[index2];

									if(child->widgets.data == NULL){//if in range make sure widget array is null
										child->action |= WIDGET_CLICKED;
										child->mousepress(child,ui.screen.button, ui.screen.clicked);
										return;
									}

									index2 = 0; //if not null repeat with new child index till one is found.
									continue;
								}
								++index2;
							}
						}

						if(child->action & WIDGET_CAN_FOCUS){
							widget_set_focused(child,focused,index2);
							return;
						}
						else{
							child->action |= WIDGET_CLICKED;
							child->mousepress(child,ui.screen.button, ui.screen.clicked);
							return;
						}
					}
					else{
						widget_set_focused(child,focused,index2);
						return;
					}
				}
			}
			else{
				if(widget_rect_contains(child2, (widget *)child2->parent)){
					child = (widget *)focused->widgets.data[index];

					if(child->widgets.data != NULL){
						while(index2 < child->widgets.count){
							child2 = (widget *)child->widgets.data[index2];
							if(child2->action & WIDGET_CAN_FOCUS){//next child if focusable
								if(widget_rect_contains(child2, (widget *)child2->parent)){// if focusable check if in range
									child = (widget *)child->widgets.data[index2];

									if(child->widgets.data == NULL){//if in range check if widget array is null or not.
										widget_set_focused(child,focused,index2);
										return;
									}

									index2 = 0; //if not null repeat with new child index till one is found.
									continue;
								}
								++index2;
							}
							else{
								if(widget_rect_contains(child2, (widget *)child2->parent)){//check to see if the non-focusable widget is in range.
									child = (widget *)child->widgets.data[index2];

									if(child->widgets.data == NULL){//if in range make sure widget array is null
										child->action |= WIDGET_CLICKED;
										child->mousepress(child,ui.screen.button, ui.screen.clicked);
										return;
									}

									index2 = 0; //if not null repeat with new child index till one is found.
									continue;
								}
								++index2;
							}
						}

						if(child->action & WIDGET_CAN_FOCUS){
							widget_set_focused(child,focused,index2);
							return;
						}
						else{
							child->action |= WIDGET_CLICKED;
							child->mousepress(child,ui.screen.button, ui.screen.clicked);
							return;
						}
					}
					else{
						child->action |= WIDGET_CLICKED;
						child->mousepress(child,ui.screen.button, ui.screen.clicked);
						return;
					}
				}
			}
		}
		focused->action &= ~(WIDGET_CLICKED);
		if(focused->action & WIDGET_MOVEABLE){
			if(widget_frame_contains(focused,(widget *)focused->parent)){
				focused->action |= WIDGET_MOVING;
			}
		}
		focused->mousepress(focused,ui.screen.button, ui.screen.clicked);
		return;
	}
}

//checkls to see if the focused widget was unclicked.
void widget_focused_mouse_release(void)
{
	widget *focused = (widget *)ui.screen.focused;
	widget *child;
	widget *child2; // used to determine if focusable
	uint16 index = 0;
	uint16 index2 = 0;

	if(focused->widgets.data != NULL){
		for( index = 0; index < focused->widgets.count; ++index){
			if(widget_rect_contains((widget *)focused->widgets.data[index],(widget *)focused)){// if focusable check if in range
				child = (widget *)focused->widgets.data[index];

				if(child->widgets.data != NULL){
					while(index2 < child->widgets.count){
						child2 = (widget *)child->widgets.data[index2];

						if(widget_rect_contains(child2, (widget *)child2->parent)){// if focusable check if in range
							child = (widget *)child->widgets.data[index2];

							if(child->widgets.data == NULL){//if in range check if widget array is null or not.
								widget_set_release(child);
								return;
							}

							index2 = 0; //if not null repeat with new child index till one is found.
							continue;
						}
						++index2;
					}
					widget_set_release(child);
					return;
				}
				else{
					widget_set_release(child);
					return;
				}
			}
		}
	}
	widget_set_release(focused);
	return;
}

//checks if a widget was unclicked Default check, looks through any widget shown.if unclicked then do event.
void widget_mouse_release(void)
{
	widget *child;
	widget *child2; // used to determine if focusable
	uint16 index = 0;
	uint16 index2 = 0;

	if(ui.root->widgets.data != NULL){
		for( index = 0; index < ui.root->widgets.count; ++index){
			if(widget_rect_contains((widget *)ui.root->widgets.data[index], (widget *)ui.root)){// if focusable check if in range
				child = (widget *)ui.root->widgets.data[index];

				if(child->widgets.data != NULL){
					while(index2 < child->widgets.count){
						child2 = (widget *)child->widgets.data[index2];

						if(widget_rect_contains(child2, (widget *)child2->parent)){// if focusable check if in range
							child = (widget *)child->widgets.data[index2];

							if(child->widgets.data == NULL){//if in range check if widget array is null or not.
								if(widget_is_parent_focused(child)){
									widget_set_release(child);
								}
								return;
							}

							index2 = 0; //if not null repeat with new child index till one is found.
							continue;
						}
						++index2;
					}
					if(widget_is_parent_focused(child)){
						widget_set_release(child);
					}
					return;
				}
				else{
					if(widget_is_parent_focused(child)){
						widget_set_release(child);
					}
					return;
				}
			}
		}
	}
	widget_set_release(ui.root);
	return;
}

//checks if the widget was clicked Default, if clicked set as focused if focusable or set parent if clickable then do event.
void widget_mouse_press(void)
{
	widget *child;
	widget *child2; // used to determine if focusable
	uint16 index = 0;
	uint16 index2 = 0;
	widget *focused = (widget *)ui.screen.focused; //used to set focus to FALSE for the old focus before switch.

	if(ui.root->widgets.data != NULL){
		for( index = 0; index < ui.root->widgets.count; ++index){
			child2 = (widget *)ui.root->widgets.data[index];

			if(child2->action & WIDGET_CAN_FOCUS){//next child if focusable
				if(widget_rect_contains(child2, (widget *)child2->parent)){// if focusable check if in range
					child = (widget *)ui.root->widgets.data[index];

					if(child->widgets.data != NULL){
						while(index2 < child->widgets.count){
							child2 = (widget *)child->widgets.data[index2];
							if(child2->action & WIDGET_CAN_FOCUS){//next child if focusable
								if(widget_rect_contains(child2, (widget *)child2->parent)){// if focusable check if in range
									child = (widget *)child->widgets.data[index2];

									if(child->widgets.data == NULL){//if in range check if widget array is null or not.
										widget_set_focused(child,focused,index2);
										return;
									}

									index2 = 0; //if not null repeat with new child index till one is found.
									continue;
								}
								++index2;
							}
							else{
								if(widget_rect_contains(child2, (widget *)child2->parent)){//check to see if the non-focusable widget is in range.
									child = (widget *)child->widgets.data[index2];

									if(child->widgets.data == NULL){//if in range make sure widget array is null
										if(widget_is_parent_focused(child)){
											child->action |= WIDGET_CLICKED;
											child->mousepress(child,ui.screen.button, ui.screen.clicked);
										}
										return;
									}

									index2 = 0; //if not null repeat with new child index till one is found.
									continue;
								}
								++index2;
							}
						}

						if(child->action & WIDGET_CAN_FOCUS){
							widget_set_focused(child,focused,index2);
							return;
						}
						else{
							if(widget_is_parent_focused(child)){
								child->action |= WIDGET_CLICKED;
								child->mousepress(child,ui.screen.button, ui.screen.clicked);
							}
							return;
						}
					}
					else{
						widget_set_focused(child,focused,index2);
						return;
					}
				}
			}
			else{
				if(widget_rect_contains(child2, (widget *)child2->parent)){
					child = (widget *)ui.root->widgets.data[index];
					if(child->widgets.data != NULL){
						while(index2 < child->widgets.count){
							child2 = (widget *)child->widgets.data[index2];
							if(child2->action & WIDGET_CAN_FOCUS) {//next child if focusable
								if(widget_rect_contains(child2, (widget *)child2->parent)){// if focusable check if in range
									child = (widget *)child->widgets.data[index2];

									if(child->widgets.data == NULL){//if in range check if widget array is null or not.
										widget_set_focused(child,focused,index2);
										return;
									}

									index2 = 0; //if not null repeat with new child index till one is found.
									continue;
								}
								++index2;
							}
							else{
								if(widget_rect_contains(child2, (widget *)child2->parent)){//check to see if the non-focusable widget is in range.
									child = (widget *)child->widgets.data[index2];

									if(child->widgets.data == NULL){//if in range make sure widget array is null
										if(widget_is_parent_focused(child)){
											child->action |= WIDGET_CLICKED;
											child->mousepress(child,ui.screen.button, ui.screen.clicked);
										}
										return;
									}

									index2 = 0; //if not null repeat with new child index till one is found.
									continue;
								}
								++index2;
							}
						}

						if(child->action & WIDGET_CAN_FOCUS){
							widget_set_focused(child,focused,index2);
							return;
						}
						else{
							if(widget_is_parent_focused(child)){
								child->action |= WIDGET_CLICKED;
								child->mousepress(child,ui.screen.button, ui.screen.clicked);
							}
							return;
						}
					}
					else{
						if(widget_is_parent_focused(child)){
							child->action |= WIDGET_CLICKED;
							child->mousepress(child,ui.screen.button, ui.screen.clicked);
						}
						return;
					}
				}
			}
		}
	}
	ui.root->action &= ~(WIDGET_CLICKED);

	ui.root->mousepress(ui.root,ui.screen.button, ui.screen.clicked);
	return;
}

//only used to initiate a empty call to avoid Errors when there events are not setup.
//initpress and release have background clickable events to parent objects if turned on.
void widget_init_mouse_press(void *wgt, int button, int pressed)
{
	widget *control = (widget *)wgt;
	if(control->action & WIDGET_CAN_CLICK_BEHIND){
		widget *parent = (widget *)control->parent;

		if(parent != NULL && parent != ui.root){
			//TODO: finish widget background button press
			parent->mousepress(parent,button,pressed);
		}
	}
}

void widget_init_mouse_release(void *wgt, int button, int pressed){}
void widget_init_mouse_wheel(void *wgt, int moved){}
void widget_init_key_pressed(void *wgt, int key, int pressed){}
void widget_init_draw(void *wgt){}