/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/
#include <GL/glew.h>
#include <stdlib.h>
#include "widget.h"
#include "integer.h"
#include "globals.h"
#include "function.h"
#include "controls.h"
#include "Render.h"
#include "error.h"

user_interface ui;
widget *focused; //holds the widget currently focused on.
widget *lastmouseover; //holds data for proper mouse exit sequences.

//default get Main ui, probably never needed but there if someone needs everything.
user_interface widget_get_ui(void)
{
	return ui;
}

user_interface widget_get_uip(void)
{
	return ui;
}

widget *widget_get_focused(void)
{
	return focused;
}

void widget_move(int16 x, int16 y)
{
	ui.screen.newmousepos.x = x;
	ui.screen.newmousepos.y = y;

	if(ui.moving){
		int windowx, windowy;

		glfwGetWindowPos(get_the_window(),&windowx, &windowy);
		glfwSetWindowPos(get_the_window(),x + windowx - ui.screen.mouseclick.x, y + windowy - ui.screen.mouseclick.y);
	}
	else{
		if(focused){
			if(focused->action & WIDGET_MOVING){
				int16 tempx = x - ui.screen.mousepos.x ;
				int16 tempy = y - ui.screen.mousepos.y ;

				if(focused->pos.x + tempx <= 0 || focused->pos.y + tempy <= 0){
					return;
				}

				if(focused->pos.x + focused->width + tempx>= get_screen_width() || focused->pos.y + focused->height + tempy >= get_screen_height()){
					return;
				}

				focused->pos.x = focused->pos.x + tempx ;

				focused->pos.y = focused->pos.y + tempy ;
				widget_position_update(focused);
			}

			if(widget_rect_contains(focused))
				widget_mouse_over(focused);
		}
		else{
			if(ui.root){
				widget_mouse_over(ui.root);
			}
		}
	}
}

void widget_has_exited(widget *control)
{
	if(!lastmouseover){
		lastmouseover = control;
		return;
	}

	if(control != lastmouseover){
		if(lastmouseover->action & WIDGET_MOUSE_OVER){
			lastmouseover->action &= ~(WIDGET_AVOID_BUFFER_UPDATE);
			lastmouseover->action &= ~(WIDGET_MOUSE_OVER);
			lastmouseover->controlmouseexit(lastmouseover);
		}

		lastmouseover = control;
	}
}

void widget_mouse_over(widget *control)
{
	widget *child;
	widget *child2; // used to determine if focusable
	int16 index = ui.root->shown.count -1;
	int16 index2 = 0;
	int16 last = 0;
	uint8 moveup = 0;

	if(control->shown.data){
		for( index = control->shown.count -1; index >= 0; --index){
			child = control->shown.data[index];

			if(widget_rect_contains(child)){// if focusable check if in range
				if(child->shown.data){
					index2 = child->shown.count -1;
					last = index2;

					if(index2 >= 0){
						while(index2 >= 0){
							child2 = child->shown.data[index2];

							if(widget_rect_contains(child2)){
								if(child2->shown.data == NULL && widget_usable(child2)){//if in range check if widget array is null or not.
									widget_has_exited(child2);
									child2->action |= WIDGET_MOUSE_OVER;
									child2->controlmouseover(child2);
									return;
								}

								if(child2->shown.data == NULL){
									return;
								}

								index2 = child2->shown.count - 1;
								last = index2;
								moveup = 0;
								child = child2->shown.data[index2];

								if(child->shown.data){
									index2 = child->shown.count -1;
								}
								else{
									if(widget_rect_contains(child) && widget_usable(child)){
										widget_has_exited(child);
										child->action |= WIDGET_MOUSE_OVER;
										child->controlmouseover(child);
										return;
									}
									else{
										if(moveup == 0){
											child = child->parent;
											index2 = last;
											moveup = 1;
										}
										--index2;
									}
								}
								continue;
							}

							if(moveup == 0){
								child = child2->parent;
								index2 = last;
								moveup = 1;
							}
							--index2;

							if(index2 == -1){
								if(widget_usable(child)){
									widget_has_exited(child);
									child->action |= WIDGET_MOUSE_OVER;
									child->controlmouseover(child);
								}
								return;
							}
						}

						if(widget_usable(child)){
							widget_has_exited(child);
							child->action |= WIDGET_MOUSE_OVER;
							child->controlmouseover(child);
						}
						return;
					}
				}
				else{
					if(widget_usable(child)){
						child->action |= WIDGET_MOUSE_OVER;
						child->controlmouseover(child);
					}
					return;
				}
			}
			else{
				widget_has_exited(child);
			}
		}
	}

	if(widget_usable(control)){
		widget_has_exited(control);
		control->controlmouseover(control);
	}
}

void widget_position_update(widget *parent)//used to update the parent widgets position and its children's positions.
{
	widget *child;
	uint16 *id = (uint16 *)calloc(1, 32 * sizeof(uint16));
	uint16 index = 0;
	uint16 idindex;
	uint16 idsize;

	if(id == NULL){
		error_handler(DE_ERROR_POINTER_NULL);
		return;
	}

	idsize = 32;
	idindex = 0;
	id[idindex] = 0;

	parent->controlupdatepos(parent);

	for( index = 0; index < parent->shown.count; ++index){
		child = parent->shown.data[index];
		child->controlupdatepos(child);

		if(child->shown.data){
			id[idindex] = 0;
			while(id[idindex] <= child->shown.count){
				if(child->shown.data){
					if(id[idindex]  >= child->shown.count && idindex != 0){
						id[idindex] = 0;

						--idindex;
						child = child->parent;
					}
					else{
						if(id[idindex] < child->shown.count){
							if (child->shown.data[id[idindex]]){
								child = child->shown.data[id[idindex]];
								child->controlupdatepos(child);
								++id[idindex];
								++idindex;//we set the z buffer index to know which layer we are in

								if(idindex + 1 >= idsize){//make sure there is not too many layers for the id array.
									idsize = (uint16)next_power_of_two(idsize);
									widget_resize_id(&id,idsize);
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
						child = child->parent;
					}
					else{
						break;
					}
				}
			}
		}
	}

	free(id);
}

//Pre-checked if we are on a focused object to decide if we should check inside it or not.
sbool widget_check_focus(void)
{
	if(focused){
		if(focused->action & WIDGET_MOVING){
			focused->action &= ~(WIDGET_MOVING);
		}

		if(widget_rect_contains(focused)){
			return TRUE;
		}
	}
	return FALSE;
}

//sets the mouse's position form screen movement.
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

//initializes the widget system.
void widget_init_system(void)
{
	/*
	*use only once for initializing a new Root Widget.
	*root widget being that of entire screen entity's or a place holder for
	*a in game widgets. Always unload the root before reusing the root.
	*/

	ui.screen.mouseclick.x = 0;
	ui.screen.mouseclick.y = 0;

	//sets to -1 to stop a mouse bug from happening if something is in the top corner.
	ui.screen.mousepos.x = -1;
	ui.screen.mousepos.y = -1;

	//creates the focused items to contain widgets that are focused.
	focused = (widget *)calloc(1, sizeof (widget));
	focused = NULL;

	lastmouseover = (widget *)calloc(1, sizeof (widget));
	lastmouseover = NULL;

	ui.screen.clicked = 0;

	//create an initialize the ROOT which is the main screens hidden widget control.
	ui.root = (widget *)calloc(1, sizeof (widget));
	widget_init(ui.root);

	//sets the show widget array
	ui.root->shown.data  = (widget **)calloc(1, WIDGET_MAX * sizeof (widget)); //set the size of the widget array
	ui.root->shown.count = (ui.root->shown.count > WIDGET_MAX) ? WIDGET_MAX : ui.root->shown.count;
	ui.root->shown.size = WIDGET_MAX;

	//sets the hidden widget array
	ui.root->hidden.data  = (widget **)calloc(1, WIDGET_MAX * sizeof (widget)); //set the size of the widget array
	ui.root->hidden.count = (ui.root->hidden.count > WIDGET_MAX) ? WIDGET_MAX : ui.root->hidden.count;
	ui.root->hidden.size = WIDGET_MAX;
	ui.root->type = CONTROL_ROOT;
}

//switches widgets around in an array.
void widget_switch(widget_array *wgt, uint32 a, uint32 b)//switch's widget positions in the arrays. very useful.
{
	widget *clone;

	if(wgt == NULL || a >= wgt->size || b >= wgt->size)
		return;

	clone = wgt->data[a];
	wgt->data[a] = wgt->data[b];
	wgt->data[b] = clone;
}

//takes a shown widget and hides it.
void widget_hide(widget *parent, uint16 index)// Parent: the holder of the widgets to be moved, Index the widget to be moved.
{
	uint16 i;

	if( parent == NULL || parent->shown.data[index] == NULL)
		return;

	if(parent->hidden.size == 0){
		widget_init_hidden(parent);
	}

	if(parent->hidden.count == 0){//if we are starting a new array then create the first object.
		parent->hidden.data[0] = parent->shown.data[index];
		parent->hidden.count += 1;
		parent->shown.data[index] = NULL;
		return;
	}

	for( i = 0; i < parent->hidden.count; i++){//search through existing count for Nulled data if Null then use.
		if(parent->hidden.data[i] == NULL){
			parent->hidden.data[i] = parent->shown.data[index];
			parent->shown.data[index] = NULL;
			return;
		}
	}

	if( parent->hidden.count < parent->hidden.size){// if none are nulled then check to see if we reach limit if not add to the count.
		parent->hidden.data[parent->hidden.count] = parent->shown.data[index];
		parent->hidden.count += 1;
		parent->shown.data[index] = NULL;
		return;
	}
}

//takes a hidden widget and shows it
void widget_show(widget *parent, uint16 index) // Parent: the holder of the widgets to be moved, Index the widget to be moved.
{
	uint16 i;

	if(parent == NULL || parent->hidden.data[index] == NULL)
		return;

	if(parent->shown.size == 0){
		widget_init_shown(parent);
	}

	if(parent->shown.count == 0){
		parent->shown.data[0] = parent->hidden.data[index];
		parent->shown.count += 1;
		parent->hidden.data[index] = NULL;
		return;
	}

	for( i = 0; i < parent->shown.count; i++){
		if(parent->shown.data[i] == NULL){
			parent->shown.data[i] = parent->hidden.data[index];
			parent->hidden.data[index] = NULL;
			return;
		}
	}

	if( parent->shown.count < parent->shown.size){
		parent->shown.data[parent->shown.count] = parent->hidden.data[index];
		parent->shown.count += 1;
		parent->hidden.data[index] = NULL;
		return;
	}
}

//adds a widget to another widgets array either Roots or another widget.
void widget_add(widget *container, widget *child)
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

	if(parent->shown.size == 0){
		widget_init_shown(parent);
	}

	if(parent->shown.count == 0){
		parent->shown.data[0] = child;
		parent->shown.count += 1;
		return;
	}

	for( i = 0; i < parent->shown.count; i++){
		if(parent->shown.data[i] == NULL){
			parent->shown.data[i] = child;
			return;
		}
	}

	if( parent->shown.count +1 < parent->shown.size){
		parent->shown.data[parent->shown.count] = child;
		parent->shown.count += 1;
		return;
	}
	else{
		//TODO: add error handler for overloaded array.
	}
}

void widget_add_hidden(widget *container, widget *child)
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

	if(parent->hidden.size == 0){
		widget_init_hidden(parent);
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
}

void widget_init(widget *wgt)//initializes a widget.
{
	if(wgt == NULL)
		return;

	wgt->parent = NULL;
	wgt->control = NULL;
	wgt->data = NULL;
	wgt->shown.data  = NULL;
	wgt->shown.count = 0;
	wgt->shown.size = 0;
	wgt->hidden.data  = NULL;
	wgt->hidden.count = 0;
	wgt->hidden.size = 0;
	wgt->pos.x = 0;
	wgt->pos.y = 0;
	wgt->actualpos.x = 0;
	wgt->actualpos.y = 0;
	wgt->imgpos.x = 0;
	wgt->imgpos.y = 0;
	wgt->img = NULL;
	wgt->width = 0;
	wgt->height = 0;
	wgt->sizex = 0;
	wgt->sizey = 0;
	wgt->action = 0;
	wgt->type = 0;
	wgt->buf.buffer = 0;
	wgt->buf.index = 0;
	wgt->buf.isize = 0;
	wgt->buf.size = 0;
	wgt->buf.count = 0;
	wgt->draw = &widget_init_draw;
	wgt->mousepress = &widget_init_mouse_press;
	wgt->mouserelease = &widget_init_mouse_release;
	wgt->mousewheel = &widget_init_mouse_wheel;
	wgt->keypressed = &widget_init_key_pressed;
	wgt->mouseover = &widget_init_mouse_over;
	wgt->mouseexit = &widget_init_mouse_exit;
	wgt->controlmousepress = &widget_init_control_mouse_press;
	wgt->controlmouserelease = &widget_init_control_mouse_release;
	wgt->controlmousewheel = &widget_init_control_mouse_wheel;
	wgt->controlkeypressed = &widget_init_control_key_pressed;
	wgt->controlupdatepos = &widget_init_control_update_pos;
	wgt->controlmouseover = &widget_init_control_mouse_over;
	wgt->controlmouseexit = &widget_init_control_mouse_exit;
}

//unloads a widget from memory
void widget_unload(widget *parent)
{
	if(!widget_clear_parent(parent))
		return;// add error message
}

void widget_init_shown(widget *parent)// only use once and if only we wanted to add a widget to a Array with no size.
{
	parent->shown.data  = (widget **)calloc(1, WIDGET_MAX * sizeof (widget)); //set the size of the widget array
	parent->shown.count = (parent->shown.count > WIDGET_MAX) ? WIDGET_MAX : parent->shown.count;
	parent->shown.size = WIDGET_MAX;
}

//initializes a widgets dormant arrays, we set them to dormant to save ram.
void widget_init_hidden(widget *parent)// only use once and if only we wanted to add a widget to a Array with no size.
{
	parent->hidden.data  = (widget **)calloc(1, WIDGET_MAX * sizeof (widget)); //set the size of the widget array
	parent->hidden.count = (parent->hidden.count > WIDGET_MAX) ? WIDGET_MAX : parent->hidden.count;
	parent->hidden.size = WIDGET_MAX;
}

//resizes the amount of widgets the show, or hidden array can hold, use if you need more than default.
void widget_shown_resize(widget *parent, uint16 size)
{
	widget **data;

	data = (widget **)realloc(parent->shown.data, size * sizeof(widget));

	if (data == NULL)
		return;

	parent->shown.data = data;
	parent->shown.count = (parent->shown.count > size) ? size : parent->shown.count;
	parent->shown.size =  size;
}

void widget_hidden_resize(widget *parent, uint16 size)
{
	widget **data;

	data = (widget **)realloc(parent->hidden.data, size * sizeof(widget));

	if (data == NULL)
		return;

	parent->hidden.data = data;
	parent->hidden.count = (parent->hidden.count > size) ? size : parent->hidden.count;
	parent->hidden.size =  size;
}

void free_widget_data(widget *control)
{
	switch(control->type){
	case CONTROL_LISTBOX: unload_list_elements(control); break;
	case CONTROL_LABEL: unload_label_elements(control); break;
	case CONTROL_TEXTBOX: unload_textbox_elements(control); break;
	case CONTROL_RADIO: unload_radio_elements(control); break;
	}
	free(control->data);
	free(control->control);

	if(!(control->action & WIDGET_USED_CLONE))
		free(control->img);

	free(control->shown.data);
	if(control->hidden.data){
		widget_clear_hidden(control);
	}
	free(control->hidden.data);

	glBindBuffer(GL_ARRAY_BUFFER,control->buf.buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,control->buf.index);
	glBufferData(GL_ARRAY_BUFFER,control->buf.size *( 4 * sizeof(vertex_t)),NULL,GL_STREAM_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, control->buf.isize * sizeof(GLuint),NULL,GL_STATIC_DRAW);
	control->hidden.data = NULL;
	control->data = NULL;
	control->control = NULL;
	control->img = NULL;
	control->shown.data = NULL;
}

void widget_clear_shown(widget *parent)//used to draw the widgets onto the screen.
{
	widget *child;
	uint16 *id = (uint16 *)calloc(1, 32 * sizeof(uint16));
	uint16 index = 0;
	uint16 idindex;
	uint16 idsize;

	if(id == NULL){
		error_handler(DE_ERROR_POINTER_NULL);
		return;
	}

	idsize = 32;
	idindex = 0;
	id[idindex] = 0;

	for( index = 0; index < parent->shown.count; ++index){
		child = parent->shown.data[index];

		if(child->shown.data){
			id[idindex] = 0;
			while(id[idindex] <= child->shown.count){
				if(child->shown.data){
					if(id[idindex]  >= child->shown.count){
						if(idindex != 0){
							id[idindex] = 0;
							--idindex;

							free_widget_data(child);
							child = child->parent;
							if(id[idindex] < child->shown.count){
								//free(child->shown.data[id[idindex]]);
								child->shown.data[id[idindex]] = NULL;
							}
							++id[idindex];
						}
						else{
							free_widget_data(child);
							child = child->parent;
							if(id[idindex] < child->shown.count){
								free(child->shown.data[id[idindex]]);
								child->shown.data[id[idindex]] = NULL;
							}
							++id[idindex];
						}
					}
					else{
						if(id[idindex] < child->shown.count){
							if (child->shown.data[id[idindex]]){
								child = child->shown.data[id[idindex]];

								++idindex;//we set the z buffer index to know which layer we are in

								if(idindex + 1 >= idsize){//make sure there is not too many layers for the id array.
									idsize = (uint16)next_power_of_two(idsize);
									widget_resize_id(&id,idsize);
								}
								id[idindex] = 0;//set the new ID index to 0
							}
							else{
								free_widget_data(child);
								child = child->parent;
								if(id[idindex] < child->shown.count){
									free(child->shown.data[id[idindex]]);
									child->shown.data[id[idindex]] = NULL;
								}
								++id[idindex];
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
						free_widget_data(child);
						child = child->parent;
						if(id[idindex] < child->shown.count){
							//	free(child->shown.data[id[idindex]]);
							child->shown.data[id[idindex]] = NULL;
						}
						++id[idindex];
					}
					else{
						free_widget_data(child);
						child = child->parent;
						if(id[idindex] < child->shown.count){
							//free(child->shown.data[id[idindex]]);
							child->shown.data[id[idindex]] = NULL;
						}
						break;
					}
				}
			}
		}
	}

	free(id);
}
//frees just the widgets in the shown array under the parent.
void widget_clear_shown_old(widget *parent);
void widget_clear_shown_old(widget *parent)//used to draw the widgets onto the screen.
{
	widget *child;
	uint16 *id = (uint16 *)calloc(1, 32 * sizeof(uint16));
	uint16 index = 0;
	uint16 idindex;
	uint16 idsize;

	if(id == NULL){
		error_handler(DE_ERROR_POINTER_NULL);
		return;
	}

	idsize = 32;
	idindex = 0;
	id[idindex] = 0;

	for( index = 0; index < parent->shown.count; ++index){
		child =  parent->shown.data[index];

		if(child->shown.data){
			id[idindex] = 0;
			while(id[idindex] <= child->shown.count){
				if(child->shown.data){
					if(id[idindex]  >= child->shown.count && idindex != 0){
						id[idindex] = 0;
						--idindex;

						switch(child->type){
						case CONTROL_LISTBOX: unload_list_elements(child); break;
						case CONTROL_LABEL: unload_label_elements(child); break;
						case CONTROL_TEXTBOX: unload_textbox_elements(child); break;
						case CONTROL_RADIO: unload_radio_elements(child); break;
						}
						free(child->data);
						free(child->control);

						if(!(child->action & WIDGET_USED_CLONE))
							free(child->img);

						free(child->shown.data);
						if(child->hidden.data){
							widget_clear_hidden(child);
						}
						free(child->hidden.data);
						child->hidden.data = NULL;
						child->data = NULL;
						child->control = NULL;
						child->img = NULL;
						child->shown.data = NULL;

						child = child->parent;
						if(idindex != 0){
							free(child->shown.data[id[idindex]]);
							child->shown.data[id[idindex]] = NULL;
						}
					}
					else{
						if(id[idindex] < child->shown.count){
							if (child->shown.data[id[idindex]]){
								child = child->shown.data[id[idindex]];

								++id[idindex];
								++idindex;//we set the z buffer index to know which layer we are in

								if(idindex + 1 >= idsize){//make sure there is not too many layers for the id array.
									idsize = (uint16)next_power_of_two(idsize);
									widget_resize_id(&id,idsize);
								}
								id[idindex] = 0;//set the new ID index to 0
							}
							else{
								if(idindex != 0){
									switch(child->type){
									case CONTROL_LISTBOX: unload_list_elements(child); break;
									case CONTROL_LABEL: unload_label_elements(child); break;
									case CONTROL_TEXTBOX: unload_textbox_elements(child); break;
									case CONTROL_RADIO: unload_radio_elements(child); break;
									}
									free(child->data);
									free(child->control);
									free(child->shown.data);
									if(!(child->action & WIDGET_USED_CLONE))
										free(child->img);

									if(child->hidden.data)
										widget_clear_hidden(child);

									free(child->hidden.data);

									child->hidden.data = NULL;
									child->data = NULL;
									child->control = NULL;
									child->img = NULL;
									child->shown.data = NULL;

									child = child->parent;

									free(child->parent->shown.data[id[idindex]]);
									child->parent->shown.data[id[idindex]] = NULL;
								}
								++id[idindex];
							}
						}
						else{
							switch(child->type){
							case CONTROL_LISTBOX: unload_list_elements(child); break;
							case CONTROL_LABEL: unload_label_elements(child); break;
							case CONTROL_TEXTBOX: unload_textbox_elements(child); break;
							case CONTROL_RADIO: unload_radio_elements(child); break;
							}
							free(child->data);
							free(child->control);
							free(child->shown.data);
							if(!(child->action & WIDGET_USED_CLONE))
								free(child->img);

							if(child->hidden.data)
								widget_clear_hidden(child);

							free(child->hidden.data);

							child->hidden.data = NULL;
							child->data = NULL;
							child->control = NULL;
							child->img = NULL;
							child->shown.data = NULL;

							free(child->parent->shown.data[id[idindex]]);
							child->parent->shown.data[id[idindex]] = NULL;
							++id[idindex];
						}
					}
				}
				else{
					if(idindex != 0){
						id[idindex] = 0;
						--idindex;
						switch(child->type){
						case CONTROL_LISTBOX: unload_list_elements(child); break;
						case CONTROL_LABEL: unload_label_elements(child); break;
						case CONTROL_TEXTBOX: unload_textbox_elements(child); break;
						case CONTROL_RADIO: unload_radio_elements(child); break;
						}
						free(child->data);
						free(child->control);
						if(!(child->action & WIDGET_USED_CLONE))
							free(child->img);
						free(child->shown.data);

						if(child->hidden.data)
							widget_clear_hidden(child);

						free(child->hidden.data);
						child->hidden.data = NULL;
						child->data = NULL;
						child->control = NULL;
						child->img = NULL;
						child->shown.data = NULL;
						child = child->parent;
						if(id[idindex] < child->shown.count){
							free(child->shown.data[id[idindex]]);
							child->shown.data[id[idindex]] = NULL;
						}
						else{
							++id[idindex];
						}
					}
					else{
						switch(child->type){
						case CONTROL_LISTBOX: unload_list_elements(child); break;
						case CONTROL_LABEL: unload_label_elements(child); break;
						case CONTROL_TEXTBOX: unload_textbox_elements(child); break;
						case CONTROL_RADIO: unload_radio_elements(child); break;
						}
						free(child->data);
						free(child->control);
						if(!(child->action & WIDGET_USED_CLONE))
							free(child->img);
						free(child->shown.data);

						if(child->hidden.data)
							widget_clear_hidden(child);

						free(child->hidden.data);
						child->hidden.data = NULL;
						child->data = NULL;
						child->control = NULL;
						child->img = NULL;
						child->shown.data = NULL;
						child = child->parent;
						free(child->shown.data[id[idindex]]);
						child->shown.data[id[idindex]] = NULL;
						break;
					}
				}
			}
			switch(child->type){
			case CONTROL_LISTBOX: unload_list_elements(child); break;
			case CONTROL_LABEL: unload_label_elements(child); break;
			case CONTROL_TEXTBOX: unload_textbox_elements(child); break;
			case CONTROL_RADIO: unload_radio_elements(child); break;
			}
			free(child->data);
			free(child->control);
			if(!(child->action & WIDGET_USED_CLONE))
				free(child->img);

			free(child->shown.data);
			if(child->hidden.data)
				widget_clear_hidden(child);

			free(child->hidden.data);
			child->hidden.data = NULL;
			child->data = NULL;
			child->control = NULL;
			child->img = NULL;
			child->shown.data = NULL;
		}
	}
	free(id);
}

//frees just the widgets in the hidden array under the parent.
void widget_clear_hidden(widget *parent)//used to draw the widgets onto the screen.
{
	widget *child;
	uint16 *id = (uint16 *)calloc(1, 32 * sizeof(uint16));
	uint16 index = 0;
	uint16 idindex;
	uint16 idsize;

	if(id == NULL){
		error_handler(DE_ERROR_POINTER_NULL);
		return;
	}

	idsize = 32;
	idindex = 0;
	id[idindex] = 0;

	for( index = 0; index < parent->hidden.count; ++index){
		child =  parent->hidden.data[index];

		if(child->hidden.data){
			id[idindex] = 0;
			while(id[idindex] <= child->hidden.count){
				if(child->hidden.data){
					if(id[idindex]  >= child->hidden.count && idindex != 0){
						id[idindex] = 0;
						--idindex;
						switch(child->type){
						case CONTROL_LISTBOX: unload_list_elements(child); break;
						case CONTROL_LABEL: unload_label_elements(child); break;
						case CONTROL_TEXTBOX: unload_textbox_elements(child); break;
						case CONTROL_RADIO: unload_radio_elements(child); break;
						}
						free(child->data);
						free(child->control);
						if(!(child->action & WIDGET_USED_CLONE))
							free(child->img);

						free(child->hidden.data);
						if(child->shown.data)
							widget_clear_shown(child);

						free(child->shown.data);
						child->data = NULL;
						child->control = NULL;
						child->img = NULL;
						child->hidden.data = NULL;
						child->shown.data = NULL;
						child = child->parent;
						if(idindex != 0){
							free(child->hidden.data[id[idindex]]);
							child->hidden.data[id[idindex]] = NULL;
						}
					}
					else{
						if(id[idindex] < child->hidden.count){
							if (child->hidden.data[id[idindex]]){
								child = child->hidden.data[id[idindex]];

								++id[idindex];
								++idindex;//we set the z buffer index to know which layer we are in

								if(idindex + 1 >= idsize){//make sure there is not too many layers for the id array.
									idsize = (uint16)next_power_of_two(idsize);
									widget_resize_id(&id,idsize);
								}
								id[idindex] = 0;//set the new ID index to 0
							}
							else{
								if(idindex != 0){
									switch(child->type){
									case CONTROL_LISTBOX: unload_list_elements(child); break;
									case CONTROL_LABEL: unload_label_elements(child); break;
									case CONTROL_TEXTBOX: unload_textbox_elements(child); break;
									case CONTROL_RADIO: unload_radio_elements(child); break;
									}
									free(child->data);
									free(child->control);
									free(child->hidden.data);
									if(!(child->action & WIDGET_USED_CLONE))
										free(child->img);

									if(child->shown.data)
										widget_clear_shown(child);

									free(child->shown.data);

									child->hidden.data = NULL;
									child->data = NULL;
									child->control = NULL;
									child->img = NULL;
									child->shown.data = NULL;
								}
								++id[idindex];
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
						switch(child->type){
						case CONTROL_LISTBOX: unload_list_elements(child); break;
						case CONTROL_LABEL: unload_label_elements(child); break;
						case CONTROL_TEXTBOX: unload_textbox_elements(child); break;
						case CONTROL_RADIO: unload_radio_elements(child); break;
						}
						free(child->data);
						free(child->control);
						if(!(child->action & WIDGET_USED_CLONE))
							free(child->img);

						free(child->hidden.data);
						if(child->shown.data)
							widget_clear_shown(child);

						free(child->shown.data);
						child->data = NULL;
						child->control = NULL;
						child->img = NULL;
						child->hidden.data = NULL;
						child->shown.data = NULL;
						child = child->parent;
						if(idindex != 0){
							free(child->hidden.data[id[idindex]]);
							child->hidden.data[id[idindex]] = NULL;
						}
					}
					else{
						switch(child->type){
						case CONTROL_LISTBOX: unload_list_elements(child); break;
						case CONTROL_LABEL: unload_label_elements(child); break;
						case CONTROL_TEXTBOX: unload_textbox_elements(child); break;
						case CONTROL_RADIO: unload_radio_elements(child); break;
						}
						free(child->data);
						free(child->control);
						if(!(child->action & WIDGET_USED_CLONE))
							free(child->img);

						free(child->hidden.data);
						if(child->shown.data)
							widget_clear_shown(child);

						free(child->shown.data);
						child->data = NULL;
						child->control = NULL;
						child->img = NULL;
						child->hidden.data = NULL;
						child->shown.data = NULL;
						child = child->parent;
						free(child->hidden.data[id[idindex]]);
						child->hidden.data[id[idindex]] = NULL;
						break;
					}
				}
			}
			switch(child->type){
			case CONTROL_LISTBOX: unload_list_elements(child); break;
			case CONTROL_LABEL: unload_label_elements(child); break;
			case CONTROL_TEXTBOX: unload_textbox_elements(child); break;
			case CONTROL_RADIO: unload_radio_elements(child); break;
			}
			free(child->data);
			free(child->control);
			if(!(child->action & WIDGET_USED_CLONE))
				free(child->img);

			free(child->hidden.data);
			if(child->shown.data)
				widget_clear_shown(child);

			free(child->shown.data);
			child->data = NULL;
			child->control = NULL;
			child->img = NULL;
			child->hidden.data = NULL;
			child->shown.data = NULL;
			child = child->parent;
			free(child->hidden.data[id[idindex]]);
			child->hidden.data[id[idindex]] = NULL;
		}
	}

	free(id);
}

//Frees all the widgets in the widget arrays shown/hidden so we can free the parent.
sbool widget_clear_parent(widget *parent)
{
	if (parent == NULL) {return FALSE;}

	if (parent->hidden.data){
		widget_clear_hidden(parent);
	}

	if (parent->shown.data){
		widget_clear_shown(parent);
	}

	free(parent->hidden.data);
	free(parent->shown.data);
	free(parent->data);
	free(parent->control);
	free(parent->img);
	focused = NULL;
	lastmouseover = NULL;
	parent->control = NULL;
	parent->img = NULL;
	parent->data = NULL;
	parent->hidden.data = NULL;
	parent->shown.data = NULL;
	parent->hidden.size = 0;
	parent->shown.size = 0;
	parent->hidden.count = 0;
	parent->shown.count = 0;

	return TRUE;
}

//resizes the ID for the widget manager when handling deep UI systems, default Z is 32.
void widget_resize_id(uint16 **id, uint16 size)
{
	uint16 *data = NULL;

	if(id == NULL)
		return;//TODO: add error handler here.

	data = (uint16 *)realloc(id, size * sizeof(uint16));

	if (data == NULL)
		return;

	*id = data;
}

//This is the Main widget That displays All the widgets in the Widget array aka shown. it Displays them via Tree branch style.
void widget_manager(void)//used to draw the widgets onto the screen.
{
	widget *child;
	uint16 *id = (uint16 *)calloc(1, 32 * sizeof(uint16));
	uint16 index = 0;
	uint16 idindex;
	uint16 idsize;

	if(id == NULL){
		error_handler(DE_ERROR_POINTER_NULL);
		return;
	}

	idsize = 32;
	idindex = 0;
	id[idindex] = 0;

	for( index = 0; index < ui.root->shown.count; ++index){
		child = ui.root->shown.data[index];
		child->draw(child);//then draw there parent.

		if(child->shown.data){
			id[idindex] = 0;
			while(id[idindex] <= child->shown.count){
				if(child->shown.data){
					if(id[idindex]  >= child->shown.count && idindex != 0){
						id[idindex] = 0;

						--idindex;
						child = child->parent;
					}
					else{
						if(id[idindex] < child->shown.count){
							if (child->shown.data[id[idindex]]){
								child = child->shown.data[id[idindex]];
								child->draw(child);//then draw the child.
								++id[idindex];
								++idindex;//we set the z buffer index to know which layer we are in

								if(idindex + 1 >= idsize){//make sure there is not too many layers for the id array.
									idsize = (uint16)next_power_of_two(idsize);
									widget_resize_id(&id,idsize);
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
						child = child->parent;
					}
					else{
						break;
					}
				}
			}
		}
	}

	free(id);
}

//checks if the widget is in the area the mouse clicked.
sbool widget_rect_contains(widget *control)
{
	if(control->type == CONTROL_WINDOW){
		window *data = (window *)control->control;

		if(ui.screen.mousepos.x < control->actualpos.x + data->frame.x)return FALSE;
		if(ui.screen.mousepos.x > control->actualpos.x + data->frame.w) return FALSE;
		if(ui.screen.mousepos.y < control->actualpos.y + data->frame.y) return FALSE;
		if(ui.screen.mousepos.y > control->actualpos.y + data->frame.h) return FALSE;
		return TRUE;
	}

	if(ui.screen.mousepos.x < control->actualpos.x ) return FALSE;
	if(ui.screen.mousepos.x > control->actualpos.x + control->sizex) return FALSE;
	if(ui.screen.mousepos.y < control->actualpos.y ) return FALSE;
	if(ui.screen.mousepos.y > control->actualpos.y + control->sizey) return FALSE;
	return TRUE;
}

sbool widget_is_parent_focused(widget *control)
{
	widget *parent;
	int8 i = TRUE;

	if(control->parent){
		parent = control->parent;
	}
	else{
		return FALSE;
	}

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

					if(parent->action & WIDGET_FOCUS_CLICK){
						parent->action |= WIDGET_CLICKED;
						parent->controlmousepress(parent,ui.screen.button, ui.screen.clicked);
					}

					i = FALSE;
					return TRUE;
				}
			}
			else{
				parent = parent->parent;
			}
		}
		else{
			i = FALSE;
			return FALSE;
		}
	}
	return FALSE;
}

//checks if the mouse is in the area the mouse moved onto.
sbool widget_has_mouse_over(widget *control)
{
	widget *parent;
	int8 i = TRUE;

	if(control->parent){
		parent = control->parent;
	}
	else{
		return FALSE;
	}

	if(!widget_rect_contains(control)){
		return FALSE;
	}

	if(control->action & WIDGET_ALWAYS_USEABLE){
		return TRUE;
	}

	if(!(control->action & WIDGET_IS_FOCUSED)){
		while(i){
			if(parent != ui.root){
				if(parent->action & WIDGET_CAN_FOCUS && parent->action & WIDGET_IS_FOCUSED || parent->action & WIDGET_ALWAYS_USEABLE){
					i = FALSE;
					return TRUE;
				}
				else{
					parent = parent->parent;
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

	return FALSE; //if can't find any then FALSE
}

sbool widget_usable(widget *control)
{
	widget *parent;
	int8 i = TRUE;

	if(control->parent){
		parent = control->parent;
	}
	else{
		return FALSE;
	}

	if(control->action & WIDGET_ALWAYS_USEABLE){
		return TRUE;
	}

	if(!(control->action & WIDGET_IS_FOCUSED)){
		while(i){
			if(parent != ui.root){
				if(parent->action & WIDGET_CAN_FOCUS && parent->action & WIDGET_IS_FOCUSED || parent->action & WIDGET_ALWAYS_USEABLE){
					i = FALSE;
					return TRUE;
				}
				else{
					parent = parent->parent;
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

	return FALSE; //if can't find any then FALSE
}

//Manually sets a widget to Focus
void widget_manual_focused(widget * control)
{
	if(control != ui.root){
		widget *parent = control->parent;
		uint16 i = 0;
		if(control->action & WIDGET_CAN_FOCUS){
			for (i = 0; i < parent->shown.count; i++){
				if(control == parent->shown.data[i] && i != parent->shown.count -1){
					widget_switch(&parent->shown, i, parent->shown.count - 1);
					break;
				}
			}

			if(focused){
				focused->action &= ~(WIDGET_IS_FOCUSED);
			}

			control->action |= WIDGET_IS_FOCUSED;
			focused = control;
		}
	}
}

//function to allow change of focus ability not needed but helpful to some. Default is FALSE.
void widget_set_focusable(widget *control, int8 boolean)
{
	if(boolean){
		control->action |= WIDGET_CAN_FOCUS;
	}
	else{
		control->action &= ~(WIDGET_CAN_FOCUS);
	}
}

//allows you to set the widget to drag able stat or to not drag, Default is FALSE.
void widget_set_moveable(widget *control, int8 boolean)
{
	if(boolean){
		control->action |= WIDGET_MOVEABLE;
	}
	else{
		control->action &= ~(WIDGET_MOVEABLE);
	}
}

void widget_mouse_release(widget * control)
{
	widget *child;
	widget *child2; // used to determine if focusable
	int16 index = control->shown.count -1;
	int16 index2 = 0;
	int16 last = 0;
	uint8 moveup = 0;

	if(control->shown.data){
		for( index = control->shown.count -1; index >= 0; --index){
			child = control->shown.data[index];

			if(widget_rect_contains(child)){// if focusable check if in range
				if(child->shown.data){
					index2 = child->shown.count -1;
					last = index2;

					while(index2 >= 0){
						child2 = child->shown.data[index2];

						if(widget_rect_contains(child2)){
							if(child2->shown.data == NULL){//if in range check if widget array is null or not.
								widget_set_release(child2);
								return;
							}

							index2 = child2->shown.count - 1;
							last = index2;
							moveup = 0;
							child = child2->shown.data[index2];

							if(child->shown.data){
								index2 = child->shown.count -1;
							}
							else{
								if(widget_rect_contains(child)){
									widget_set_release(child);
									return;
								}
								else{
									if(moveup == 0){
										child = child->parent;
										index2 = last;
										moveup = 1;
									}
									--index2;
								}
							}
							continue;
						}

						if(moveup == 0){
							child = child2->parent;
							index2 = last;
							moveup = 1;
						}
						--index2;
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

	widget_set_release(control);
}

//checks if the widget was clicked Default, if clicked set as focused if focusable or set parent if click able then do event.
void widget_mouse_press(widget * control)
{
	widget *child;
	widget *child2; // used to determine if focusable
	int16 index = control->shown.count -1;
	int16 index2 = 0;
	int16 last = 0;
	uint8 moveup = 0;

	if(control->shown.data){
		for( index = control->shown.count -1; index >= 0; --index){
			child = control->shown.data[index];

			if(widget_rect_contains(child)){// if focusable check if in range
				if(child->shown.data){
					index2 = child->shown.count -1;
					last = index2;

					while(index2 >= 0){
						child2 = child->shown.data[index2];

						if(widget_rect_contains(child2)){
							if(child2->shown.data == NULL){//if in range check if widget array is null or not.
								set_widget_mouse_press_event(child2,index2);
								return;
							}

							index2 = child2->shown.count - 1;
							last = index2;
							moveup = 0;
							child = child2->shown.data[index2];

							if(child->shown.data){
								index2 = child->shown.count -1;
							}
							else{
								if(widget_rect_contains(child)){
									set_widget_mouse_press_event(child,index2);
									return;
								}
								else{
									if(moveup == 0){
										child = child->parent;
										index2 = last;
										moveup = 1;
									}
									--index2;
								}
							}
							continue;
						}

						if(moveup == 0){
							child = child2->parent;
							index2 = last;
							moveup = 1;
						}
						--index2;
					}
					set_widget_mouse_press_event(child,index);
					return;
				}
				else{
					set_widget_mouse_press_event(child,index);
					return;
				}
			}
		}
	}
	if(control->action & WIDGET_MOVEABLE){
		if(widget_rect_contains(control)){
			control->action |= WIDGET_MOVING;
		}
	}

	widget_set_clicked(control);
}

void set_widget_mouse_press_event(widget *control,uint16 index)
{
	if(control->action & WIDGET_CAN_FOCUS){
		widget_set_focused(control,index);
	}
	else{
		if(widget_is_parent_focused(control)){
			widget_set_clicked(control);
		}
	}
}

void widget_set_release(widget *control)
{
	if(control->action & WIDGET_WINDOW_MOVEABLE){
		ui.moving = FALSE;
	}

	control->action &= ~(WIDGET_CLICKED);
	control->controlmouserelease(control,ui.screen.button, ui.screen.clicked);
}

void widget_set_clicked(widget *control)
{
	if(control->action & WIDGET_WINDOW_MOVEABLE){
		ui.moving = TRUE;
	}

	control->action |= WIDGET_CLICKED;
	control->controlmousepress(control,ui.screen.button, ui.screen.clicked);
}

void widget_set_focused(widget *control, uint32 index)
{
	if(control != control->parent->shown.data[control->parent->shown.count - 1]){
		widget_switch(&control->parent->shown,index,control->parent->shown.count - 1);
	}

	if(focused){
		focused->action &= ~(WIDGET_IS_FOCUSED);
	}

	control->action |= WIDGET_IS_FOCUSED;
	focused = control;
	control->action |= WIDGET_CLICKED;

	if(control->action & WIDGET_MOVEABLE){
		if(widget_rect_contains(control)){
			control->action |= WIDGET_MOVING;
		}
	}
	control->controlmousepress(control,ui.screen.button, ui.screen.clicked);
}

//only used to initiate a empty call to avoid Errors when there events are not setup.
void widget_init_mouse_press(widget *control, int button, int pressed)
{
	if(control->action & WIDGET_CAN_CLICK_BEHIND){
		if(control->parent && control->parent != ui.root){
			//TODO finish widget background button press
			control->parent->controlmousepress(control->parent,button,pressed);
			control->parent->mousepress(control->parent,button,pressed);
		}
	}
}
void widget_init_mouse_release(widget *control, int button, int pressed) {}
void widget_init_mouse_wheel(widget *control, int moved) {}
void widget_init_key_pressed(widget *control, int key, int pressed) {}
void widget_init_draw(widget *control) {}
void widget_init_mouse_over(widget *control) {}
void widget_init_mouse_exit(widget *control) {}

void widget_init_control_mouse_press(widget *control, int button, int pressed) {control->mousepress(control,button, pressed);}
void widget_init_control_mouse_release(widget *control, int button, int pressed) {control->mouserelease(control,button, pressed);}
void widget_init_control_mouse_wheel(widget *control, int moved) {}
void widget_init_control_key_pressed(widget *control, int key) {}
void widget_init_control_update_pos(widget *control) {}
void widget_init_control_mouse_over(widget *control) {control->mouseover(control);}
void widget_init_control_mouse_exit(widget *control) {control->mouseexit(control);}