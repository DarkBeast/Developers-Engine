/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
*           Purlox
******************************************************************************/
#include <GL/glew.h>
#include "render.h"
#include "controls.h"
#include "function.h"
#include "error.h"
#include <stdlib.h>
#include "globals.h"

void set_control_image(widget *control, char *path)
{
	if(!control->img)
		control->img = (image *)calloc(1, sizeof(image));

	load_image(path, control->img);
}

void use_control_image(widget *control, widget *main, sbool freeit)
{
	if(control->img && freeit)
		free(control->img);

	control->action |= WIDGET_USED_CLONE;
	control->img = main->img;
	update_widget_vertex_buffer(control);

	if(control->type == CONTROL_HPROGRESSBAR || control->type == CONTROL_VPROGRESSBAR)
		widget_update_progressbars_vector(control->shown.data[0]);
}

void free_control_images(widget *control, widget *main)
{
	switch(control->type){
	case CONTROL_HPROGRESSBAR:
	case CONTROL_VPROGRESSBAR:
		if(control->shown.data[0]->img)
			free(control->shown.data[0]->img);
		if(control->img)
			free(control->img);
		break;

	case CONTROL_HSCROLL_BAR:
	case CONTROL_VSCROLL_BAR:
		if(control->shown.data[0]->img)
			free(control->shown.data[0]->img);
		if(control->shown.data[1]->img)
			free(control->shown.data[0]->img);
		if(control->shown.data[2]->img)
			free(control->shown.data[0]->img);
		if(control->img)
			free(control->img);
		break;

	case CONTROL_LISTBOX:{
		listbox *list = (listbox *)control->control;

		if(control->img)
			free(control->img);
		if(list->vbar.img)
			free(list->vbar.img);
		if(list->vbar.shown.data[0]->img)
			free(list->vbar.shown.data[0]->img);
		if(list->vbar.shown.data[1]->img)
			free(list->vbar.shown.data[1]->img);
		if(list->vbar.shown.data[2]->img)
			free(list->vbar.shown.data[2]->img);
		if(list->selectover->img)
			free(list->selectover->img);
		if(list->select->img)
			free(list->select->img);
						 }
						 break;

	default:
		if(control->img)
			free(control->img);
		break;
	}
}

void clone_control_images(widget *control, widget *main, sbool freeit, sbool update)
{
	if(freeit)
		free_control_images(control,main);

	switch(control->type){
	case CONTROL_HPROGRESSBAR:
	case CONTROL_VPROGRESSBAR:
		control->action |= WIDGET_USED_CLONE;
		control->shown.data[0]->action |= WIDGET_USED_CLONE;
		control->shown.data[0]->img = main->shown.data[0]->img;
		control->img = main->img;

		if(update){
			update_widget_vertex_buffer(control);
			update_widget_vertex_buffer(control->shown.data[0]);
			widget_update_progressbars_vector(control->shown.data[0]);
		}
		break;

	case CONTROL_HSCROLL_BAR:
	case CONTROL_VSCROLL_BAR:
		control->action |= WIDGET_USED_CLONE;
		control->shown.data[0]->action |= WIDGET_USED_CLONE;
		control->shown.data[1]->action |= WIDGET_USED_CLONE;
		control->shown.data[2]->action |= WIDGET_USED_CLONE;
		control->shown.data[0]->img = main->shown.data[0]->img;
		control->shown.data[1]->img = main->shown.data[1]->img;
		control->shown.data[2]->img = main->shown.data[2]->img;
		control->img = main->img;

		if(update){
			update_widget_vertex_buffer(control);
			update_widget_vertex_buffer(control->shown.data[0]);
			update_widget_vertex_buffer(control->shown.data[1]);
			update_widget_vertex_buffer(control->shown.data[2]);
		}
		break;

	case CONTROL_LISTBOX:{
		listbox *list = (listbox *)control->control;
		listbox *list2 = (listbox *)main->control;

		control->action |= WIDGET_USED_CLONE;
		list->vbar.action |= WIDGET_USED_CLONE;
		list->vbar.shown.data[0]->action |= WIDGET_USED_CLONE;
		list->vbar.shown.data[1]->action |= WIDGET_USED_CLONE;
		list->vbar.shown.data[2]->action |= WIDGET_USED_CLONE;
		list->selectover->action |= WIDGET_USED_CLONE;
		list->select->action |= WIDGET_USED_CLONE;

		list->vbar.img = list2->vbar.img;
		list->vbar.shown.data[0]->img = list2->vbar.shown.data[0]->img;
		list->vbar.shown.data[1]->img = list2->vbar.shown.data[1]->img;
		control->img = main->img;
		list->vbar.shown.data[2]->img = list2->vbar.shown.data[2]->img;
		list->selectover->img = list2->selectover->img;
		list->select->img = list2->select->img;

		if(update){
			update_widget_vertex_buffer(control);
			update_widget_vertex_buffer(&list->vbar);
			update_widget_vertex_buffer(list->vbar.shown.data[0]);
			update_widget_vertex_buffer(list->vbar.shown.data[1]);
			update_widget_vertex_buffer(list->vbar.shown.data[2]);
			update_widget_vertex_buffer(list->selectover);
			update_widget_vertex_buffer(list->select);
		}
						 }
						 break;

	default:
		control->action |= WIDGET_USED_CLONE;
		control->img = main->img;
		if(update)
			update_widget_vertex_buffer(control);

		break;
	}
}

void set_control_alpha(widget *control, uint8 alpha)
{
	control->buf.data[0].a = (alpha / 255.f);
	control->buf.data[1].a = (alpha / 255.f);
	control->buf.data[2].a = (alpha / 255.f);
	control->buf.data[3].a = (alpha / 255.f);

	update_widget_vertex_buffer(control);
}

void unload_base(widget *control)
{
	free(control->data);
	free(control->control);

	if(!(control->action & WIDGET_USED_CLONE))
		free(control->img);

	free(control->shown.data);
	free(control->hidden.data);

	glDeleteBuffers(1, &control->buf.buffer);
	glDeleteBuffers(1, &control->buf.index);
}

void unload_base2(widget *control)
{
	free(control->data);
	free(control->control);
	free(control->shown.data);
	free(control->hidden.data);
}

//Buttons
void create_button(widget *control, widget *parent, uint16 x, uint16 y, uint16 height, uint16 width, uint16 sizey, uint16 sizex, char *path, widget *clone)
{
	widget_init(control);
	control->pos.x = x;
	control->pos.y = y;
	control->height = height;
	control->width = width;
	control->sizey = sizey;
	control->sizex = sizex;
	control->imgpos.x = 0;
	control->imgpos.y = 0;
	control->type = CONTROL_BUTTON;
	control->controldraw = &draw_buttons;
	control->controlmousepress = &handle_button_click;
	control->controlmouserelease = &handle_button_release;
	control->controlupdatepos = &handle_button_move;
	control->controlmouseover = &handle_button_mouse_over;
	control->controlmouseexit = &handle_button_mouse_exit;
	if(clone)
		clone_control_images(control, clone, FALSE, FALSE);
	else
		set_control_image(control, comb_2str(GUI_PATH, path));

	widget_add(parent,control);
	create_widget_vertex_buffer(control);
}

void draw_buttons(widget *control)
{
	draw_widget(control);
}

void handle_button_release(widget *control, int button, int pressed)
{
	if (widget_has_mouse_over(control)){
		control->action &= ~(WIDGET_AVOID_BUFFER_UPDATE);
		handle_button_mouse_over(control);
	}
	control->mouserelease(control,button,pressed);
}

void handle_button_click(widget *control, int button, int pressed)
{
	if (widget_has_mouse_over(control)){
		if (control->action & WIDGET_CLICKED){
			control->imgpos.x = control->width;
			widget_update_texture_vector(control);
		}
	}

	control->mousepress(control,button,pressed);
}

void handle_button_mouse_over(widget *control)
{
	if(!(control->action & WIDGET_AVOID_BUFFER_UPDATE)){
		if (control->action & WIDGET_MOUSE_OVER){
			control->imgpos.x = 2 * control->width;
			control->action |= WIDGET_AVOID_BUFFER_UPDATE;
			widget_update_texture_vector(control);
		}
	}

	control->mouseover(control);
}

void handle_button_mouse_exit(widget *control)
{
	control->imgpos.x = 0;

	widget_update_texture_vector(control);

	control->mouseexit(control);
}

void handle_button_move(widget *control)
{
	widget_update_vector(control);
}

//labels
void create_label(widget *control, widget *parent, uint16 x, uint16 y, uint16 width, uint16 height, uint8 red, uint8 blue, uint8 green, uint8 alpha, sbool events, uint8 fontid, uint16 maxcharspl, sbool multi_lined, char *string)
{
	label *init_text;

	widget_init(control);
	control->pos.x = x;
	control->pos.y = y;
	control->imgpos.x = 0;
	control->imgpos.y = 0;
	control->type = CONTROL_LABEL;
	control->controldraw = &draw_label;
	control->controlmousepress = &handle_label_click;

	if(multi_lined){
		control->action |= WIDGET_IS_MULTI_LINED;
		control->controlupdatepos = &handle_mlabel_move;
	}
	else
		control->controlupdatepos = &handle_slabel_move;

	if(events)
		control->action |= WIDGET_CAN_USE_EVENT;

	init_text = (label *)calloc(1,sizeof(label));

	if(init_text == NULL)
		error_handler(DE_ERROR_POINTER_NULL,",init_text in create_label()\n");

	init_text->string = (text *)calloc(1,sizeof(text));

	if(init_text->string == NULL)
		error_handler(DE_ERROR_POINTER_NULL,",init_text->string in create_label()\n");

	text_set(init_text->string,0,0,width,height,0,0,0,maxcharspl,fontid,red,blue,green,alpha,string);

	control->height = height;
	control->width = width;
	control->control = init_text;

	widget_add(parent,control);
	create_text_vertex(init_text->string, control);
}

void unload_label(widget *control)
{
	label *init_text = (label *)control->control;

	free(init_text->string->buf.data);
	glDeleteBuffers(1, &init_text->string->buf.buffer);
	glDeleteBuffers(1, &init_text->string->buf.index);
	free(init_text->string);

	free(control->data);
	free(control->control);

	if(!(control->action & WIDGET_USED_CLONE))
		free(control->img);

	free(control->shown.data);
	free(control->hidden.data);
}

void draw_label(widget *control)
{
	label *data = (label *)control->control;

	if(data->string->buf.isize > 0)
		text_draw(data->string);
}

void handle_label_click(widget *control, int button, int pressed)
{
	if(control->action & WIDGET_CAN_USE_EVENT)
		control->mousepress(control,button,pressed);
}

void update_label_string(widget *control, char *string)
{
	label *data = (label *)control->control;
	sbool sizechange = FALSE;
	uint32 size = get_str_size(string) + 1;

	while( size >= data->string->size){
		data->string->size = data->string->size * 2;
		sizechange = TRUE;
	}

	if(sizechange)
		control->action |= WIDGET_BUFFER_RESIZE;

	data->string->data = string;
	update_text_vertex(data->string, control);
}

void handle_slabel_move(widget *control)
{
	label *data = (label *)control->control;
	text_position_supdate(data->string,control);
}

void handle_mlabel_move(widget *control)
{
	label *data = (label *)control->control;
	text_position_mupdate(data->string,control);
}

void create_window(widget *control, widget *parent, uint16 x, uint16 y, uint16 height, uint16 width, uint16 sizey, uint16 sizex, char * path, widget *clone)
{
	window *init_window = (window *)calloc(1,sizeof(window));

	if(init_window == NULL)
		error_handler(DE_ERROR_POINTER_NULL,",init_window in create_window()\n");

	widget_init(control);
	control->pos.x = x;
	control->pos.y = y;
	control->height = height;
	control->width = width;
	control->sizey = sizey;
	control->sizex = sizex;
	control->imgpos.x = 0;
	control->imgpos.y = 0;
	control->type = CONTROL_WINDOW;
	control->controldraw = &draw_windows;
	control->controlmousepress = &handle_window_click;
	control->controlupdatepos = &handle_windows_move;

	if(clone)
		clone_control_images(control, clone, FALSE, FALSE);
	else
		set_control_image(control,comb_2str(GUI_PATH, path));

	init_window->frame.x = 0;
	init_window->frame.y = 0;
	init_window->frame.w = width;
	init_window->frame.h = height;
	control ->control = init_window;
	control->action |= WIDGET_CAN_FOCUS;

	widget_add(parent,control);

	create_widget_vertex_buffer(control); // must be called after widget add to get the correct Parent.
}

void create_window_framed(widget *control, widget *parent, uint16 x, uint16 y, uint16 height, uint16 width, uint16 sizey, uint16 sizex, uint16 framex, uint16 framey,uint16 frameh, uint16 framew, char *path, widget *clone)
{
	window *init_window = (window *)calloc(1,sizeof(window));

	if(init_window == NULL){
		error_handler(DE_ERROR_POINTER_NULL,",init_window in create_window_framed()\n"); return;
	}

	widget_init(control);
	control->pos.x = x;
	control->pos.y = y;
	control->height = height;
	control->width = width;
	control->sizey = sizey;
	control->sizex = sizex;
	control->imgpos.x = 0;
	control->imgpos.y = 0;
	control->type = CONTROL_WINDOW;
	control->controldraw = &draw_windows;;
	control->controlmousepress = &handle_window_click;
	control->controlupdatepos = &handle_windows_move;
	if(clone)
		clone_control_images(control, clone, FALSE, FALSE);
	else
		set_control_image(control, comb_2str(GUI_PATH, path));

	init_window->frame.x = framex;
	init_window->frame.y = framey;
	init_window->frame.w = framew;
	init_window->frame.h = frameh;
	control->control = init_window;
	control->action |= WIDGET_CAN_FOCUS;

	widget_add(parent,control);

	create_widget_vertex_buffer(control); // must be called after widget add to get the correct Parent.
}

void draw_windows(widget *control)
{
	draw_widget(control);
}

void handle_windows_move(widget *control)
{
	widget_update_vector(control);
}

void handle_window_click(widget *control, int button, int pressed){}

void create_checkbox(widget *control, widget *parent, uint16 x, uint16 y, uint16 height, uint16 width, uint16 sizey, uint16 sizex, char *path, widget *clone)
{
	widget_init(control);
	control->pos.x = x;
	control->pos.y = y;
	control->height = height;
	control->width = width;
	control->sizey = sizey;
	control->sizex = sizex;
	control->imgpos.x = 0;
	control->imgpos.y = control->height;
	control->type = CONTROL_CHECKBOX;
	control->controldraw = &draw_checkbox;
	control->controlmousepress = handle_check_click;
	control->controlupdatepos = &handle_check_move;
	if(clone)
		clone_control_images(control, clone, FALSE, FALSE);
	else
		set_control_image(control, comb_2str(GUI_PATH, path));

	widget_add(parent,control);

	create_widget_vertex_buffer(control);
}

void draw_checkbox(widget *control)
{
	draw_widget(control);
}

void handle_check_click(widget *control, int button, int pressed)
{
	if(button == 0 && pressed == TRUE){
		if(control->action & WIDGET_CHECKED)
			control->action &= ~(WIDGET_CHECKED);
		else
			control->action |= WIDGET_CHECKED;
	}

	if (control->action & WIDGET_CHECKED){
		control->imgpos.x = 0;
		control->imgpos.y = 0;
	}
	else{
		control->imgpos.y = control->height;
		control->imgpos.x = 0;
	}

	widget_update_texture_vector(control);
	control->mousepress(control,button,pressed);
}

void handle_check_move(widget *control)
{
	widget_update_vector(control);
}

void create_radio(widget *control, widget *parent, uint16 x, uint16 y, uint16 height, uint16 width, uint16 sizey, uint16 sizex, sbool istrue, char *path,widget *linkparent, widget *clone)
{
	radio *init_radio = (radio *)calloc(1,sizeof(radio));

	if(init_radio == NULL)
		error_handler(DE_ERROR_POINTER_NULL,",init_radio in create_radio()\n");

	widget_init(control);
	control->pos.x = x;
	control->pos.y = y;
	control->height = height;
	control->width = width;
	control->sizey = sizey;
	control->sizex = sizex;
	control->imgpos.x = 0;
	control->imgpos.y = height;
	control->type = CONTROL_RADIO;
	control->controldraw = &draw_radio;
	control->controlmousepress = &handle_radio_click;
	control->controlupdatepos = &handle_radio_move;
	if(clone)
		clone_control_images(control, clone, FALSE, FALSE);
	else
		set_control_image(control, comb_2str(GUI_PATH, path));

	if(istrue){
		control->action |= WIDGET_CHECKED;
		control->imgpos.y = 0;
	}
	init_radio->amount = 0;
	init_radio->count = 0;
	init_radio->main = NULL;
	init_radio->list = NULL;
	control->control = init_radio;

	widget_add(parent,control);
	create_widget_vertex_buffer(control);

	if(linkparent)
		link_radio(linkparent,control);
}

void unload_radio(widget *control)
{
	radio *init_radio = (radio *)control->control;
	uint32 i = 0;
	init_radio->main = NULL;

	if(init_radio){
		for(i = 0; i < init_radio->count; i++)
			init_radio->list[i] = NULL;

		free(init_radio->list);
	}

	free(control->data);
	free(control->control);

	if(!(control->action & WIDGET_USED_CLONE))
		free(control->img);

	free(control->shown.data);
	free(control->hidden.data);

	glDeleteBuffers(1, &control->buf.buffer);
	glDeleteBuffers(1, &control->buf.index);
}

void draw_radio(widget *control)
{
	draw_widget(control);
}

void resize_radio_list(radio *controls, uint16 size)
{
	widget **data;

	if(controls == NULL)
		error_handler(DE_ERROR_POINTER_NULL,"controls in resize_radio_list");

	data = (widget **)realloc(controls->list, size * sizeof(widget*));

	if (data == NULL)
		error_handler(DE_ERROR_POINTER_NULL,"data in resize_radio_list");

	controls->list = data;
}

void link_radio(widget *main, widget *control)
{
	uint16 i = 0;
	radio *data = (radio *)main->control;
	radio *controldata = (radio *)control->control;

	if(main == NULL && control == NULL)
		error_handler(DE_ERROR_POINTER_NULL,"main or control in resize_radio_list");

	if(data->count == 0)
	{
		data->list = (widget **)calloc(1,8 * sizeof(widget));

		if(data->list == NULL)
			error_handler(DE_ERROR_POINTER_NULL,",data->list in link_radio()\n");

		data->amount = 4;
		data->list[0]= control;
		data->count = 1;
	}
	else{
		for( i = 0; i <= data->amount; i++){
			if( i >= data->amount)
			{
				data->amount = (uint8)next_power_of_two(data->amount);
				resize_radio_list(data,data->amount);
			}

			if(data->list[i] == NULL){
				data->list[i] = control;
				++data->count;
				break;
			}
		}
	}
	controldata->main = main;
}

void reset_radio(widget *control)
{
	radio *data = (radio *)control->control;
	widget *wgt;
	uint16 i = 0;

	if(data->main){
		data->main->action &= ~(WIDGET_CHECKED);
		data->main->imgpos.y = control->height;
		data->main->imgpos.x = 0;
		widget_update_texture_vector(data->main);
		data = (radio *)data->main->control;
	}
	else{
		control->action &= ~(WIDGET_CHECKED);
		control->imgpos.y = control->height;
		control->imgpos.x = 0;
		widget_update_texture_vector(control);
	}

	for( i = 0; i < data->count; i++){
		wgt = (widget *)data->list[i];
		wgt->action &= ~(WIDGET_CHECKED);
		wgt->imgpos.y = control->height;
		wgt->imgpos.x = 0;
		widget_update_texture_vector(wgt);
	}
}

void handle_radio_click(widget *control, int button, int pressed)
{
	radio *data = (radio *)control->control;

	if(button == 0 && pressed == TRUE){
		if(data->main){
			reset_radio(control);
			control->action |= WIDGET_CHECKED;
		}
		else{
			if(data->count > 0){
				reset_radio(control);
				control->action |= WIDGET_CHECKED;
			}
			else{
				if (control->action & WIDGET_CHECKED)
					control->action &= ~(WIDGET_CHECKED);
				else
					control->action |= WIDGET_CHECKED;
			}
		}
	}

	if(data->main){
		if (data->main->action & WIDGET_CHECKED){
			data->main->imgpos.x = 0;
			data->main->imgpos.y = 0;
		}
		else{
			data->main->imgpos.y = data->main->height;
			data->main->imgpos.x = 0;
		}
		widget_update_texture_vector(data->main);
		data->main->mousepress(control,button,pressed);
	}

	if (control->action & WIDGET_CHECKED){
		control->imgpos.x = 0;
		control->imgpos.y = 0;
	}
	else{
		control->imgpos.y = control->height;
		control->imgpos.x = 0;
	}
	widget_update_texture_vector(control);
	control->mousepress(control,button,pressed);
}

void handle_radio_move(widget *control)
{
	widget_update_vector(control);
}

void update_progressbar_value(widget *control,uint32 value)
{
	control->value = value;
	widget_update_progressbars_vector(control->shown.data[0]);
}

void create_hprogressbar(widget *control, widget *parent, uint16 x, uint16 y, uint16 height, uint16 width, uint16 sizey, uint16 sizex, uint8 value, char *background, char *hbar, widget *clone)
{
	progressbar *bar = (progressbar *)calloc(1, sizeof(progressbar));

	if(bar == NULL)
		error_handler(DE_ERROR_POINTER_NULL,",bar in create_hprogressbar()\n");

	widget_init(control);
	widget_init(&bar->bar);

	control->pos.x = x;
	control->pos.y = y;
	control->height = height;
	control->width = width;
	control->sizey = sizey;
	control->sizex = sizex;
	control->imgpos.x = 0;
	control->imgpos.y = 0;
	control->type = CONTROL_HPROGRESSBAR;
	control->controldraw = &draw_hprogressbar;
	control->controlmousepress = &handle_hprogressbar_click;
	control->controlupdatepos = &handle_hprogressbar_move;

	if(clone)
		clone_control_images(control, clone, FALSE, FALSE);
	else
		set_control_image(control, comb_2str(GUI_PATH, background));

	control->value = value;

	if(!clone)
		set_control_image(&bar->bar, comb_2str(GUI_PATH, hbar));

	bar->bar.pos.x = 0;
	bar->bar.pos.y = 0;
	bar->bar.height = bar->bar.img->height;
	bar->bar.width = bar->bar.img->width;
	bar->bar.sizey = sizey;
	bar->bar.sizex = sizex;
	bar->bar.imgpos.x = 0;
	bar->bar.imgpos.y = 0;
	bar->bar.type = CONTROL_HPROGRESSBAR;
	bar->bar.controldraw = &draw_hprogressbar;
	bar->bar.controlmousepress = &handle_hprogressbar_click;
	bar->bar.controlupdatepos = &handle_hprogressbars_move;

	control->control = bar;

	widget_add(parent,control);
	widget_add(control, &bar->bar);

	create_widget_vertex_buffer(control);
	create_widget_vertex_buffer(&bar->bar);
	widget_update_progressbars_vector(&bar->bar);
}

void draw_hprogressbar(widget *control)
{
	draw_widget(control);
}

void handle_hprogressbar_click(widget *control, int button, int pressed)
{
	control->mousepress(control,button,pressed);
}

void handle_hprogressbar_move(widget *control)
{
	widget_update_vector(control);
}

void handle_hprogressbars_move(widget *control)
{
	widget_update_progressbars_vector(control);
}

void create_picturebox(widget *control, widget *parent, uint16 x, uint16 y, uint16 height, uint16 width, uint16 sizey, uint16 sizex, char *imagepath, widget *clone)
{
	widget_init(control);
	control->pos.x = x;
	control->pos.y = y;
	control->height = height;
	control->width = width;
	control->sizey = sizey;
	control->sizex = sizex;
	control->imgpos.x = 0;
	control->imgpos.y = height;
	control->type = CONTROL_PICTUREBOX;
	control->controldraw = &draw_picturebox;
	control->controlmousepress = &handle_picturebox_click;
	control->controlupdatepos = &handle_picturebox_move;

	if(clone)
		clone_control_images(control, clone, FALSE, FALSE);
	else
		set_control_image(control,imagepath);

	widget_add(parent,control);
	create_widget_vertex_buffer(control);
}

void update_picturebox(widget *control, uint16 x, uint16 y, uint16 imgposx, uint16 imgposy, uint16 height, uint16 width, uint16 sizex, uint16 sizey, char *imagepath, widget *clone)
{
	if(imagepath != NULL){
		if(!(control->action & WIDGET_USED_CLONE))
			free(control->img);

		load_image(imagepath, control->img );
	}
	control->pos.x = x;
	control->pos.y = y;
	control->imgpos.x = imgposx;
	control->imgpos.y = imgposy;
	control->height = height;
	control->width = width;
	control->sizex = sizex;
	control->sizey = sizey;

	update_widget_vertex_buffer(control);
}

void draw_picturebox(widget *control)
{
	draw_widget(control);
}

void handle_picturebox_click(widget *control, int button, int pressed)
{
	control->mousepress(control,button,pressed);
}

void handle_picturebox_move(widget *control)
{
	widget_update_vector(control);
}

void create_hscrollbar(widget *control, widget *parent, uint16 x, uint16 y, uint16 height, uint16 width, uint16 buttonheight, uint16 buttonwidth, uint16 sizey, uint16 sizex, uint16 value, uint16 max_value, char *background,char *buttonleft, char *buttonright,char *scrollbar, widget *clone)
{
	scrollbar_t *scroll_t = (scrollbar_t *)calloc(1, sizeof(scrollbar_t));

	if(scroll_t == NULL )
		error_handler(DE_ERROR_POINTER_NULL,",scroll_t in create_hscrollbar()\n");

	widget_init(control);
	widget_init(&scroll_t->bar);//moving bar.
	widget_init(&scroll_t->button_1);//left
	widget_init(&scroll_t->button_2);//right

	if(clone)
		clone_control_images(control, clone, FALSE, FALSE);
	else
		set_control_image(control, comb_2str(GUI_PATH, background));

	control->pos.x = x;
	control->pos.y = y;
	control->height = height;
	control->width = width;
	control->sizey = sizey;
	control->sizex = sizex;
	control->imgpos.x = 0;
	control->imgpos.y = height;
	control->type = CONTROL_HSCROLL_BAR;
	control->controldraw = &draw_hscrollbar;
	control->controlmousepress = &handle_hscrollbar_click;
	control->controlupdatepos = &handle_hscrollbar_move;
	control->controlmouseover = &handle_hbar_slide;
	control->controlmouserelease = &handle_hscrollbar_release;

	control->value = value;

	if(!clone)
		set_control_image(&scroll_t->button_1, comb_2str(GUI_PATH, buttonleft));

	scroll_t->button_1.pos.x = 0;
	scroll_t->button_1.pos.y = 0;
	scroll_t->button_1.height = buttonheight;
	scroll_t->button_1.width = buttonwidth;
	scroll_t->button_1.sizex = buttonwidth;
	scroll_t->button_1.sizey = sizey;
	scroll_t->button_1.type = CONTROL_HSCROLL_BAR;
	scroll_t->button_1.controlmousepress = &handle_harrowleft_click;
	scroll_t->button_1.controlupdatepos = &handle_hscrollbar_move;
	scroll_t->button_1.controlmouseover = &handle_harrowleft_over;
	scroll_t->button_1.controlmouserelease = &handle_harrowleft_release;
	scroll_t->button_1.controlmouseexit = &handle_harrowleft_exit;
	scroll_t->button_1.controldraw = &draw_hscrollbar;

	if(!clone)
		set_control_image(&scroll_t->button_2, comb_2str(GUI_PATH, buttonright));

	scroll_t->button_2.height = buttonheight;
	scroll_t->button_2.width = buttonwidth;
	scroll_t->button_2.sizex = buttonwidth;
	scroll_t->button_2.sizey = sizey;
	scroll_t->button_2.pos.x = sizex - scroll_t->button_2.sizex;
	scroll_t->button_2.pos.y = 0;
	scroll_t->button_2.type = CONTROL_HSCROLL_BAR;
	scroll_t->button_2.controlmousepress = &handle_harrowright_click;
	scroll_t->button_2.controlupdatepos = &handle_hscrollbar_move;
	scroll_t->button_2.controlmouseover = &handle_harrowright_over;
	scroll_t->button_2.controlmouserelease = &handle_harrowright_release;
	scroll_t->button_2.controlmouseexit = &handle_harrowright_exit;
	scroll_t->button_2.controldraw = &draw_hscrollbar;

	if(!clone)
		set_control_image(&scroll_t->bar, comb_2str(GUI_PATH, scrollbar));

	scroll_t->bar.height = scroll_t->bar.img->height;
	scroll_t->bar.width = scroll_t->bar.img->width / 3;
	scroll_t->bar.imgpos.x = 0;
	scroll_t->bar.imgpos.y = 0;
	scroll_t->bar.type = CONTROL_HSCROLL_BAR;
	scroll_t->bar.sizex = (sizex + scroll_t->button_2.sizex  + scroll_t->button_1.sizex) / max_value + 1;
	scroll_t->bar.sizey = sizey;

	if(scroll_t->bar.sizex < 2)
		scroll_t->bar.sizex = 2;

	if(scroll_t->bar.sizex > sizex - (scroll_t->button_2.sizex  + scroll_t->button_1.sizex))
		scroll_t->bar.sizex = sizex - (scroll_t->button_2.sizex  + scroll_t->button_1.sizex) - max_value;

	scroll_t->bar.pos.x = (scroll_t->button_1.pos.x + scroll_t->button_1.sizex) + (value *(sizex - (scroll_t->button_1.sizex) - 1 ) / max_value);

	scroll_t->bar.value = max_value;
	scroll_t->bar.pos.y = 0;

	if(scroll_t->bar.pos.x < scroll_t->button_1.pos.x)
		scroll_t->bar.pos.x = scroll_t->button_1.pos.x + scroll_t->button_1.sizex;

	if(scroll_t->bar.pos.x > scroll_t->button_2.pos.x)
		scroll_t->bar.pos.x = scroll_t->button_2.pos.x;

	scroll_t->bar.controlupdatepos = &handle_hscrollbar_move;
	scroll_t->bar.controlmousepress = &handle_hbar_click;
	scroll_t->bar.controlmouserelease = &handle_hbar_release;
	scroll_t->bar.controlmouseover = &handle_hbar_over;
	scroll_t->bar.controlmouseexit = &handle_hbar_exit;
	scroll_t->bar.controldraw = &draw_hscrollbar;
	control->control = scroll_t;

	widget_add(parent,control);
	widget_add(control,&scroll_t->button_1);
	widget_add(control,&scroll_t->button_2);
	widget_add(control,&scroll_t->bar);

	create_widget_vertex_buffer(control);
	create_widget_vertex_buffer(&scroll_t->button_1);
	create_widget_vertex_buffer(&scroll_t->button_2);
	create_widget_vertex_buffer(&scroll_t->bar);
}

void draw_hscrollbar(widget *control)
{
	draw_widget(control);
}

void handle_hbar_over(widget *control)
{
	if(!(control->action & WIDGET_AVOID_BUFFER_UPDATE)){
		if (control->action & WIDGET_MOUSE_OVER){
			control->imgpos.y = 0;
			control->imgpos.x = 2 * control->width;
			control->action |= WIDGET_AVOID_BUFFER_UPDATE;
			widget_update_texture_vector(control);
		}
	}
}

void handle_hbar_exit(widget *control)
{
	control->imgpos.y = 0;
	control->imgpos.x = 0;

	widget_update_texture_vector(control);

	control->mouseexit(control);
}

void set_scrollbar_buttons(widget *control,void(*mousepress)(widget *,int,int))
{
	control->shown.data[0]->mousepress = mousepress;
	control->shown.data[1]->mousepress = mousepress;
}

void handle_harrowleft_click(widget *control, int button, int pressed)
{
	if (widget_has_mouse_over(control)){
		if (control->action & WIDGET_CLICKED){
			control->imgpos.x = control->width;
			control->imgpos.y = 0;
			widget_update_texture_vector(control);
		}
	}

	if(control->parent->value > 0)
		--control->parent->value;

	control->parent->shown.data[2]->pos.x = (control->pos.x + control->sizex) +
		(control->parent->value *(control->parent->sizex - (control->parent->shown.data[1]->pos.y + (control->parent->shown.data[1]->sizex * 2)+ control->parent->shown.data[2]->sizex - 1)) / control->parent->shown.data[2]->value);

	if(control->parent->shown.data[2]->pos.x < control->parent->shown.data[0]->pos.x + control->parent->shown.data[0]->sizex - 1)
		control->parent->shown.data[2]->pos.x = control->parent->shown.data[0]->pos.x + control->parent->shown.data[0]->sizex;

	widget_update_vector(control->parent->shown.data[2]);

	control->mousepress(control,button,pressed);
}

void handle_harrowright_click(widget *control, int button, int pressed)
{
	if (widget_has_mouse_over(control)){
		if (control->action & WIDGET_CLICKED){
			control->imgpos.x = control->width;
			control->imgpos.y = 0;
			widget_update_texture_vector(control);
		}
	}

	if(control->parent->value + 1 <= control->parent->shown.data[2]->value)
		++control->parent->value;

	control->parent->shown.data[2]->pos.x = (control->parent->shown.data[0]->pos.x + control->parent->shown.data[0]->sizex) +
		(control->parent->value *(control->parent->sizex - (control->parent->shown.data[1]->pos.y + (control->parent->shown.data[1]->sizex * 2) + control->parent->shown.data[2]->sizex )) / control->parent->shown.data[2]->value);
	widget_update_vector(control->parent->shown.data[2]);
	control->mousepress(control,button,pressed);
}

void handle_hscrollbar_move(widget *control)
{
	widget_update_vector(control);
}

void handle_harrowleft_over(widget *control)
{
	if(!(control->action & WIDGET_AVOID_BUFFER_UPDATE)){
		if (control->action & WIDGET_MOUSE_OVER){
			control->imgpos.y = 0;
			control->imgpos.x = 2 * control->width;
			control->action |= WIDGET_AVOID_BUFFER_UPDATE;
			widget_update_texture_vector(control);
		}
	}

	if(control->parent->shown.data[2]->action & WIDGET_MOVING){
		control->parent->value = 0;

		control->parent->shown.data[2]->pos.x = (control->pos.x + control->sizex) +
			(control->parent->value *(control->parent->sizex - (control->sizex * 2) - control->parent->shown.data[2]->sizex) / control->parent->shown.data[2]->value);

		widget_update_vector(control->parent->shown.data[2]);

		control->mousepress(control,0,1);
	}
}

void handle_harrowright_over(widget *control)
{
	if(!(control->action & WIDGET_AVOID_BUFFER_UPDATE)){
		if (control->action & WIDGET_MOUSE_OVER){
			control->imgpos.y = 0;
			control->imgpos.x = 2 * control->width;
			control->action |= WIDGET_AVOID_BUFFER_UPDATE;
			widget_update_texture_vector(control);
		}
	}

	if(control->parent->shown.data[2]->action & WIDGET_MOVING){
		control->parent->value =control->parent->shown.data[2]->value;

		control->parent->shown.data[2]->pos.x = (control->parent->shown.data[0]->pos.x + control->parent->shown.data[0]->sizex) +
			(control->parent->value *(control->parent->sizex - (control->parent->shown.data[0]->sizex * 2) - control->parent->shown.data[2]->sizex) / control->parent->shown.data[2]->value);

		widget_update_vector(control->parent->shown.data[2]);

		control->parent->shown.data[0]->mousepress(control->parent->shown.data[0],0,1);
	}
}

void handle_hbar_slide(widget *control)
{
	if(control->shown.data[2]->action & WIDGET_MOVING){
		float temppos = (widget_get_uip()->screen.newmousepos.x - control->actualpos.x - control->shown.data[0]->sizex - (control->shown.data[2]->sizex /2));
		float tempsize = ( control->sizex - (control->shown.data[0]->sizex * 2) - (control->shown.data[2]->sizex));

		if(temppos < 0)
			temppos = 0;

		if(temppos > tempsize)
			temppos = tempsize;

		control->value = (temppos / tempsize * control->shown.data[2]->value);

		if (control->value > control->shown.data[2]->value)
			control->value = control->shown.data[2]->value;

		if (control->value < 0)
			control->value = 0;

		control->shown.data[2]->pos.x = (control->shown.data[0]->pos.x + control->shown.data[0]->sizex) +
			(control->value *(control->sizex - (control->shown.data[0]->sizex * 2) - control->shown.data[2]->sizex - 1) / control->shown.data[2]->value);

		if(control->value == control->shown.data[2]->value)
			control->shown.data[2]->pos.x += 1;

		widget_update_vector(control->shown.data[2]);

		control->shown.data[0]->mousepress(control->shown.data[0],0,1);
	}
}

void handle_hscrollbar_click(widget *control, int button, int pressed)
{
	if(button == 0){
		float temppos = (widget_get_uip()->screen.newmousepos.x - control->actualpos.x - control->shown.data[0]->sizex - (control->shown.data[2]->sizex /2));
		float tempsize = ( control->sizex - (control->shown.data[0]->sizex * 2) - (control->shown.data[2]->sizex));

		if(temppos < 0)
			temppos = 0;

		if(temppos > tempsize)
			temppos = tempsize;

		control->value = (temppos / tempsize * control->shown.data[2]->value);

		if (control->value > control->shown.data[2]->value)
			control->value = control->shown.data[2]->value;

		if (control->value < 0)
			control->value = 0;

		control->shown.data[2]->pos.x = (control->shown.data[0]->pos.x + control->shown.data[0]->sizex) +
			(control->value *(control->sizex - (control->shown.data[0]->sizex * 2) - control->shown.data[2]->sizex - 1) / control->shown.data[2]->value);

		if(control->value == control->shown.data[2]->value)
			control->shown.data[2]->pos.x += 1;

		widget_update_vector(control->shown.data[2]);

		control->shown.data[0]->mousepress(control->shown.data[0],0,1);
	}
}

void handle_harrowright_exit(widget *control)
{
	control->imgpos.y = 0;
	control->imgpos.x = 0;

	widget_update_texture_vector(control);

	control->mouseexit(control);
}

void handle_harrowleft_exit(widget *control)
{
	control->imgpos.y = 0;
	control->imgpos.x = 0;

	widget_update_texture_vector(control);

	control->mouseexit(control);
}

void handle_hscrollbar_release(widget *control, int button, int pressed)
{
	if(control->shown.data[2]->action & WIDGET_MOVING)
		control->shown.data[2]->action &= ~(WIDGET_MOVING);
}

void handle_hbar_release(widget *control, int button, int pressed)
{
	if(control->action & WIDGET_MOVING)
		control->action &= ~(WIDGET_MOVING);

	if (widget_has_mouse_over(control)){
		control->imgpos.y = 0;
		control->imgpos.x = 2 * control->width;
		control->action |= WIDGET_AVOID_BUFFER_UPDATE;
		widget_update_texture_vector(control);
	}
}

void handle_hbar_click(widget *control, int button, int pressed)
{
	if(!(control->action & WIDGET_MOVING))
		control->action |= WIDGET_MOVING;

	if (widget_has_mouse_over(control)){
		if (control->action & WIDGET_CLICKED){
			control->imgpos.x = control->width;
			control->imgpos.y = 0;
			widget_update_texture_vector(control);
		}
	}
}

void handle_harrowleft_release(widget *control, int button, int pressed)
{
	if(control->parent->shown.data[2]->action & WIDGET_MOVING)
		control->parent->shown.data[2]->action &= ~(WIDGET_MOVING);

	if (widget_has_mouse_over(control)){
		control->imgpos.y = 0;
		control->imgpos.x = 2 * control->width;
		control->action |= WIDGET_AVOID_BUFFER_UPDATE;
		widget_update_texture_vector(control);
	}

	control->mouserelease(control,button,pressed);
}

void handle_harrowright_release(widget *control, int button, int pressed)
{
	if(control->parent->shown.data[2]->action & WIDGET_MOVING)
		control->parent->shown.data[2]->action &= ~(WIDGET_MOVING);

	if (widget_has_mouse_over(control)){
		control->imgpos.y = 0;
		control->imgpos.x = 2 * control->width;
		control->action |= WIDGET_AVOID_BUFFER_UPDATE;
		widget_update_texture_vector(control);
	}

	control->mouserelease(control,button,pressed);
}

//Vertical scrollbar testing
void create_vscrollbar(widget *control, widget *parent, uint16 x, uint16 y, uint16 height, uint16 width, uint16 buttonheight, uint16 buttonwidth, uint16 sizey, uint16 sizex, uint16 value, uint16 max_value, char *background,char *buttontop, char *buttonbottom,char *scrollbar, widget *clone)
{
	scrollbar_t *scroll_t = (scrollbar_t *)calloc(1, sizeof(scrollbar_t));

	if(scroll_t == NULL)
		error_handler(DE_ERROR_POINTER_NULL,",scroll_t in create_vscrollbar()\n");

	widget_init(control);
	widget_init(&scroll_t->bar);//moving bar.
	widget_init(&scroll_t->button_1);//top
	widget_init(&scroll_t->button_2);//bottom

	if(clone)
		clone_control_images(control, clone, FALSE, FALSE);
	else
		set_control_image(control, comb_2str(GUI_PATH, background));
	control->pos.x = x;
	control->pos.y = y;
	control->height = height;
	control->width = width;
	control->sizey = sizey;
	control->sizex = sizex;
	control->imgpos.x = 0;
	control->imgpos.y = height;
	control->type = CONTROL_VSCROLL_BAR;
	control->controldraw = &draw_vscrollbar;
	control->controlmousepress = &handle_vscrollbar_click;
	control->controlupdatepos = &handle_vscrollbar_move;
	control->controlmouseover = &handle_vbar_slide;
	control->controlmouserelease = &handle_vscrollbar_release;

	control->value = value;

	if(!clone)
		set_control_image(&scroll_t->button_1, comb_2str(GUI_PATH, buttontop));

	scroll_t->button_1.pos.x = 0;
	scroll_t->button_1.pos.y = 0;
	scroll_t->button_1.imgpos.x = 0;
	scroll_t->button_1.height = scroll_t->button_1.img->height;
	scroll_t->button_1.width = scroll_t->button_1.img->width  / 3;
	scroll_t->button_1.sizex = sizex;
	scroll_t->button_1.sizey = scroll_t->button_1.img->height;
	scroll_t->button_1.type = CONTROL_BUTTON;
	scroll_t->button_1.controldraw = &draw_vscrollbar;
	scroll_t->button_1.controlmousepress = &handle_varrowtop_click;
	scroll_t->button_1.controlupdatepos = &handle_vscrollbar_move;
	scroll_t->button_1.controlmouseover = &handle_varrowtop_over;
	scroll_t->button_1.controlmouserelease = &handle_varrowtop_release;
	scroll_t->button_1.controlmouseexit = &handle_varrowtop_exit;

	if(!clone)
		set_control_image(&scroll_t->button_2, comb_2str(GUI_PATH, buttonbottom));

	scroll_t->button_2.height = scroll_t->button_2.img->height ;
	scroll_t->button_2.width = scroll_t->button_2.img->width / 3;
	scroll_t->button_2.sizex = sizex;
	scroll_t->button_2.sizey = scroll_t->button_2.img->height ;
	scroll_t->button_2.pos.x = 0;
	scroll_t->button_2.pos.y = sizey - scroll_t->button_2.sizey;
	scroll_t->button_2.imgpos.x = 0;
	scroll_t->button_2.type = CONTROL_BUTTON;
	scroll_t->button_2.controldraw = &draw_vscrollbar;
	scroll_t->button_2.controlmousepress = &handle_varrowbottom_click;
	scroll_t->button_2.controlupdatepos = &handle_vscrollbar_move;
	scroll_t->button_2.controlmouseover = &handle_varrowbottom_over;
	scroll_t->button_2.controlmouserelease = &handle_varrowbottom_release;
	scroll_t->button_2.controlmouseexit = &handle_varrowbottom_exit;

	if(!clone)
		set_control_image(&scroll_t->bar, comb_2str(GUI_PATH, scrollbar));

	scroll_t->bar.height = scroll_t->bar.img->height;
	scroll_t->bar.width = scroll_t->bar.img->width / 3;
	scroll_t->bar.sizex = sizex;

	scroll_t->bar.sizey = (sizey + scroll_t->button_1.sizey  + scroll_t->button_2.sizey - scroll_t->bar.img->height) / max_value;
	if(scroll_t->bar.sizey < 2)
		scroll_t->bar.sizey = 2;

	if(scroll_t->bar.sizey > sizey - (scroll_t->button_2.sizey  + scroll_t->button_1.sizey))
		scroll_t->bar.sizey = sizey - (scroll_t->button_2.sizey  + scroll_t->button_1.sizey) - max_value;

	scroll_t->bar.pos.y = (scroll_t->button_1.pos.y + scroll_t->button_1.sizey) + (value *(sizey - (scroll_t->button_1.sizey * 2) - 1 ) / max_value);

	scroll_t->bar.value = max_value;
	scroll_t->bar.pos.x = 0;

	if(scroll_t->bar.pos.y < scroll_t->button_1.pos.y)
		scroll_t->bar.pos.y = scroll_t->button_1.pos.y;

	if(scroll_t->bar.pos.y > scroll_t->button_2.pos.y)
		scroll_t->bar.pos.y = scroll_t->button_2.pos.y;

	scroll_t->bar.controlupdatepos = &handle_vscrollbar_move;
	scroll_t->bar.controlmousepress = &handle_vbar_click;
	scroll_t->bar.controlmouserelease = &handle_vbar_release;
	scroll_t->bar.controlmouseexit = &handle_vbar_exit;
	scroll_t->bar.controlmouseover = &handle_vbar_over;
	scroll_t->bar.controldraw = &draw_vscrollbar;

	control->control = scroll_t;
	widget_add(parent,control);
	widget_add(control,&scroll_t->button_1);
	widget_add(control,&scroll_t->button_2);
	widget_add(control,&scroll_t->bar);

	create_widget_vertex_buffer(control);
	create_widget_vertex_buffer(&scroll_t->button_1);
	create_widget_vertex_buffer(&scroll_t->button_2);
	create_widget_vertex_buffer(&scroll_t->bar);
}

void draw_vscrollbar(widget *control)
{
	draw_widget(control);
}

void handle_vbar_over(widget *control)
{
	if(!(control->action & WIDGET_AVOID_BUFFER_UPDATE)){
		if (control->action & WIDGET_MOUSE_OVER){
			control->imgpos.y = 0;
			control->imgpos.x = 2 * control->width;
			control->action |= WIDGET_AVOID_BUFFER_UPDATE;
			widget_update_texture_vector(control);
		}
	}
}

void handle_vbar_exit(widget *control)
{
	control->imgpos.y = 0;
	control->imgpos.x = 0;

	widget_update_texture_vector(control);

	control->mouseexit(control);
}

void handle_varrowtop_over(widget *control)
{
	if(!(control->action & WIDGET_AVOID_BUFFER_UPDATE)){
		if (control->action & WIDGET_MOUSE_OVER){
			control->imgpos.y = 0;
			control->imgpos.x = 2 * control->width;
			control->action |= WIDGET_AVOID_BUFFER_UPDATE;
			widget_update_texture_vector(control);
		}
	}

	if(control->parent->shown.data[2]->action & WIDGET_MOVING){
		control->parent->value = 0;

		control->parent->shown.data[2]->pos.y = (control->pos.y + control->sizey) +
			(control->parent->value *(control->parent->sizey - (control->sizey * 2) - control->parent->shown.data[2]->sizey) / control->parent->shown.data[2]->value);

		widget_update_vector(control->parent->shown.data[2]);

		control->mousepress(control,0,1);
	}
}

void handle_varrowtop_exit(widget *control)
{
	control->imgpos.y = 0;
	control->imgpos.x = 0;

	widget_update_texture_vector(control);

	control->mouseexit(control);
}

void handle_varrowbottom_exit(widget *control)
{
	control->imgpos.y = 0;
	control->imgpos.x = 0;

	widget_update_texture_vector(control);

	control->mouseexit(control);
}

void handle_varrowbottom_over(widget *control)
{
	if(!(control->action & WIDGET_AVOID_BUFFER_UPDATE)){
		if (control->action & WIDGET_MOUSE_OVER){
			control->imgpos.y = 0;
			control->imgpos.x = 2 * control->width;
			control->action |= WIDGET_AVOID_BUFFER_UPDATE;
			widget_update_texture_vector(control);
		}
	}

	if(control->parent->shown.data[2]->action & WIDGET_MOVING){
		control->parent->value =control->parent->shown.data[2]->value;

		control->parent->shown.data[2]->pos.y = (control->parent->shown.data[0]->pos.y + control->parent->shown.data[0]->sizey) +
			(control->parent->value *(control->parent->sizey - (control->parent->shown.data[0]->sizey * 2) - control->parent->shown.data[2]->sizey) / control->parent->shown.data[2]->value);

		widget_update_vector(control->parent->shown.data[2]);

		control->parent->shown.data[0]->mousepress(control->parent->shown.data[0],0,1);
	}
}

void handle_vbar_slide(widget *control)
{
	if(control->shown.data[2]->action & WIDGET_MOVING){
		float temppos = (widget_get_uip()->screen.newmousepos.y - control->actualpos.y - control->shown.data[0]->sizey - (control->shown.data[2]->sizey /2));
		float tempsize = ( control->sizey - (control->shown.data[0]->sizey * 2) - (control->shown.data[2]->sizey));

		if(temppos < 0)
			temppos = 0;

		if(temppos > tempsize)
			temppos = tempsize;

		control->value = (temppos / tempsize * control->shown.data[2]->value);

		if (control->value > control->shown.data[2]->value)
			control->value = control->shown.data[2]->value;

		if (control->value < 0)
			control->value = 0;

		control->shown.data[2]->pos.y = (control->shown.data[0]->pos.y + control->shown.data[0]->sizey) +
			(control->value *(control->sizey - (control->shown.data[0]->sizey * 2) - control->shown.data[2]->sizey - 1) / control->shown.data[2]->value);

		if(control->value == control->shown.data[2]->value)
			control->shown.data[2]->pos.y += 1;

		widget_update_vector(control->shown.data[2]);

		control->shown.data[0]->mousepress(control->shown.data[0],0,1);
	}
}

void handle_vscrollbar_click(widget *control, int button, int pressed)
{
	if(button == 0){
		float temppos = (widget_get_uip()->screen.newmousepos.y - control->actualpos.y - control->shown.data[0]->sizey - (control->shown.data[2]->sizey /2));
		float tempsize = ( control->sizey - (control->shown.data[0]->sizey * 2) - (control->shown.data[2]->sizey));

		if(temppos < 0)
			temppos = 0;

		if(temppos > tempsize)
			temppos = tempsize;

		control->value = (temppos / tempsize * control->shown.data[2]->value);

		if (control->value > control->shown.data[2]->value)
			control->value = control->shown.data[2]->value;

		if (control->value < 0)
			control->value = 0;

		control->shown.data[2]->pos.y = (control->shown.data[0]->pos.y + control->shown.data[0]->sizey) +
			(control->value *(control->sizey - (control->shown.data[0]->sizey * 2) - control->shown.data[2]->sizey - 1) / control->shown.data[2]->value);

		if(control->value == control->shown.data[2]->value)
			control->shown.data[2]->pos.y += 1;

		widget_update_vector(control->shown.data[2]);

		control->shown.data[0]->mousepress(control->shown.data[0],0,1);
	}
}

void handle_vscrollbar_release(widget *control, int button, int pressed)
{
	if(control->shown.data[2]->action & WIDGET_MOVING)
		control->shown.data[2]->action &= ~(WIDGET_MOVING);
}

void handle_vbar_release(widget *control, int button, int pressed)
{
	if(control->action & WIDGET_MOVING)
		control->action &= ~(WIDGET_MOVING);

	if (widget_has_mouse_over(control)){
		control->imgpos.y = 0;
		control->imgpos.x = 2 * control->width;
		control->action |= WIDGET_AVOID_BUFFER_UPDATE;
		widget_update_texture_vector(control);
	}
}

void handle_vbar_click(widget *control, int button, int pressed)
{
	if(!(control->action & WIDGET_MOVING))
		control->action |= WIDGET_MOVING;

	if (widget_has_mouse_over(control)){
		if (control->action & WIDGET_CLICKED){
			control->imgpos.x = control->width;
			control->imgpos.y = 0;
			widget_update_texture_vector(control);
		}
	}
}

void handle_varrowtop_release(widget *control, int button, int pressed)
{
	if(control->parent->shown.data[2]->action & WIDGET_MOVING)
		control->parent->shown.data[2]->action &= ~(WIDGET_MOVING);

	if (widget_has_mouse_over(control)){
		control->imgpos.y = 0;
		control->imgpos.x = 2 * control->width;
		control->action |= WIDGET_AVOID_BUFFER_UPDATE;
		widget_update_texture_vector(control);
	}
	control->mouserelease(control,button,pressed);
}

void handle_varrowbottom_release(widget *control, int button, int pressed)
{
	if(control->parent->shown.data[2]->action & WIDGET_MOVING)
		control->parent->shown.data[2]->action &= ~(WIDGET_MOVING);

	if (widget_has_mouse_over(control)){
		control->imgpos.y = 0;
		control->imgpos.x = 2 * control->width;
		control->action |= WIDGET_AVOID_BUFFER_UPDATE;
		widget_update_texture_vector(control);
	}
	control->mouserelease(control,button,pressed);
}

void handle_varrowtop_click(widget *control, int button, int pressed)
{
	if (widget_has_mouse_over(control)){
		if (control->action & WIDGET_CLICKED){
			control->imgpos.x = control->width;
			control->imgpos.y = 0;
			widget_update_texture_vector(control);
		}
	}

	if(control->parent->shown.data[2]->action & WIDGET_MOVING)
		control->parent->shown.data[2]->action &= ~(WIDGET_MOVING);

	if(control->parent->value > 0)
		--control->parent->value;

	control->parent->shown.data[2]->pos.y = (control->pos.y + control->sizey) +
		(control->parent->value *(control->parent->sizey - (control->sizey * 2) - control->parent->shown.data[2]->sizey - 1 ) / control->parent->shown.data[2]->value);
	widget_update_vector(control->parent->shown.data[2]);
	control->mousepress(control,button,pressed);
}

void handle_varrowbottom_click(widget *control, int button, int pressed)
{
	if (widget_has_mouse_over(control)){
		if (control->action & WIDGET_CLICKED){
			control->imgpos.x = control->width;
			control->imgpos.y = 0;
			widget_update_texture_vector(control);
		}
	}

	if(control->parent->shown.data[2]->action & WIDGET_MOVING)
		control->parent->shown.data[2]->action &= ~(WIDGET_MOVING);

	if(control->parent->value + 1 <= control->parent->shown.data[2]->value)
		++control->parent->value;

	control->parent->shown.data[2]->pos.y = (control->parent->shown.data[0]->pos.y + control->parent->shown.data[0]->sizey) +
		(control->parent->value *(control->parent->sizey - (control->parent->shown.data[0]->sizey * 2) - control->parent->shown.data[2]->sizey) / control->parent->shown.data[2]->value);
	widget_update_vector(control->parent->shown.data[2]);
	control->mousepress(control,button,pressed);
}

void handle_vscrollbar_move(widget *control)
{
	widget_update_vector(control);
}

void create_vprogressbar(widget *control, widget *parent, uint16 x, uint16 y, uint16 height, uint16 width, uint16 sizey, uint16 sizex, uint8 value, char *background, char *vbar, widget *clone)
{
	progressbar *bar = (progressbar *)calloc(1, sizeof(progressbar));

	if(bar == NULL)
		error_handler(DE_ERROR_POINTER_NULL,",bar in create_vprogressbar()\n");

	widget_init(control);
	control->pos.x = x;
	control->pos.y = y;
	control->height = height;
	control->width = width;
	control->sizey = sizey;
	control->sizex = sizex;
	control->imgpos.x = 0;
	control->imgpos.y = 0;
	control->type = CONTROL_VPROGRESSBAR;
	control->controldraw = &draw_vprogressbar;
	control->controlmousepress = &handle_vprogressbar_click;
	control->controlupdatepos = &handle_vprogressbar_move;
	if(clone)
		clone_control_images(control, clone, FALSE, FALSE);
	else
		set_control_image(control, comb_2str(GUI_PATH, background));
	control->value = value;

	widget_init(&bar->bar);

	if(!clone)
		set_control_image(&bar->bar, comb_2str(GUI_PATH, vbar));

	bar->bar.pos.x = 0;
	bar->bar.pos.y = 0;
	bar->bar.height = bar->bar.img->height - 1;
	bar->bar.width = bar->bar.img->width;
	bar->bar.sizey = sizey - 1;
	bar->bar.sizex = sizex;
	bar->bar.imgpos.x = 0;
	bar->bar.imgpos.y = 0;
	bar->bar.type = CONTROL_VPROGRESSBAR;
	bar->bar.controldraw = &draw_vprogressbar;
	bar->bar.controlmousepress = &handle_vprogressbar_click;
	bar->bar.controlupdatepos = &handle_vprogressbars_move;

	control->control = bar;

	widget_add(parent,control);
	widget_add(control,&bar->bar);

	create_widget_vertex_buffer(control);
	create_widget_vertex_buffer(&bar->bar);
	widget_update_progressbars_vector(&bar->bar);
}

void draw_vprogressbar(widget *control)
{
	draw_widget(control);
}

void handle_vprogressbar_click(widget *control, int button, int pressed)
{
	control->mousepress(control,button,pressed);
}

void handle_vprogressbar_move(widget *control)
{
	widget_update_vector(control);
}

void handle_vprogressbars_move(widget *control)
{
	widget_update_progressbars_vector(control);
}

void create_stextbox(widget *control, widget *parent, uint16 x, uint16 y, uint16 width, uint16 height, uint8 offsetx, uint8 offsety, uint16 sx, uint16 sy, uint8 red, uint8 blue, uint8 green, uint8 alpha, uint16 maxchars, uint8 fontid, sbool ispass, char *imgpath, widget *clone)
{
	textbox *init_text = (textbox *)calloc(1,sizeof(textbox));
	textbox_w *init_widget = (textbox_w *)calloc(1,sizeof(textbox_w));

	if(init_text == NULL || init_widget == NULL)
		error_handler(DE_ERROR_POINTER_NULL,",init_text or init_widget in create_stextbox()\n");

	widget_init(control);
	control->pos.x = x;
	control->pos.y = y;
	control->sizex = sx;
	control->sizey = sy;
	control->imgpos.x = 0;
	control->imgpos.y = 0;
	control->type = CONTROL_TEXTBOX;
	control->controldraw = &draw_stextbox;
	control->controlmousepress = &handle_stextbox_click;
	control->controlupdatepos = &handle_stextbox_move;
	control->controlkeypressed = &handle_stextbox_input;
	control->action |= WIDGET_CAN_FOCUS;
	if(clone)
		clone_control_images(control, clone, FALSE, FALSE);
	else
		set_control_image(control, comb_2str(GUI_PATH, imgpath));

	widget_init(&init_widget->wstring);
	init_widget->wstring.pos.x = 0;
	init_widget->wstring.pos.y = 0;
	init_widget->wstring.imgpos.x = 0;
	init_widget->wstring.imgpos.y = 0;
	init_widget->wstring.type = CONTROL_TEXTBOX;
	init_widget->wstring.controldraw = &draw_stextbox_text;
	init_widget->wstring.controlmousepress = &handle_stextbox_click;
	init_widget->wstring.controlupdatepos = &handle_stextbox_text_move;

	if(ispass)
		init_widget->wstring.action |= WIDGET_IS_PASSWORD;

	init_text->string = (text *)calloc(1,sizeof(text));

	if(init_text->string == NULL)
		error_handler(DE_ERROR_POINTER_NULL, ",init_text->string in create_stextbox()\n");

	text_set(init_text->string,init_widget->wstring.pos.x,init_widget->wstring.pos.y,width,height,offsetx,offsety,maxchars,0,fontid,red,blue,green,alpha,NULL);

	init_text->cursorblink = .8;
	init_text->cursortime = 0;
	init_text->cusorenabled = FALSE;
	init_text->cursorheight = init_text->string->font.h - 3;
	init_text->cursorwidth = 2;

	control->height = height;
	control->width = width;
	init_widget->wstring.height = sy;
	init_widget->wstring.width = sx;
	init_widget->wstring.control = init_text;
	control->control = init_widget;

	widget_add(parent,control);
	widget_add(control,&init_widget->wstring);

	create_widget_vertex_buffer(control);
	create_cursor_vertex(&init_widget->wstring);
}

void draw_stextbox(widget *control)
{
	draw_widget(control);
}

void unload_textbox(widget *control)
{
	textbox *init_text;

	if(!control->shown.data){
		init_text = (textbox *)control->control;

		free(init_text->string->buf.data);
		free(init_text->string->data);
		glDeleteBuffers(1, &init_text->string->buf.buffer);
		glDeleteBuffers(1, &init_text->string->buf.index);
		free(init_text->string);

		glDeleteBuffers(1, &init_text->cursorbuffer.buffer);
		glDeleteBuffers(1, &init_text->cursorbuffer.index);
	}
	else{
		glDeleteBuffers(1, &control->buf.buffer);
		glDeleteBuffers(1, &control->buf.index);
	}

	free(control->data);
	free(control->control);

	if(!(control->action & WIDGET_USED_CLONE))
		free(control->img);

	free(control->shown.data);
	free(control->hidden.data);
}

void draw_stextbox_text(widget *control)
{
	textbox *data = (textbox *)control->control;

	if(data->string->buf.isize > 0)
		text_draw(data->string);

	if(data->cusorenabled){
		double tick = glfwGetTime();

		if(control->parent != widget_get_focused())
			data->cusorenabled = FALSE;

		if(data->cursortime > tick)
			draw_cursor(control);
		else{
			if(data->cursortime + data->cursorblink < tick || data ->cursortime == 0)
				data->cursortime = tick + data->cursorblink;
		}
	}
}

void handle_stextbox_click(widget *control, int button, int pressed)
{
	textbox *data = (textbox *)control->shown.data[0]->control;

	if(data->cusorenabled == FALSE)
		data->cusorenabled = TRUE;

	control->mousepress(control,button,pressed);
}

void handle_stextbox_move(widget *control)
{
	widget_update_vector(control);
}

void handle_stextbox_text_move(widget *control)
{
	textbox *data = (textbox *)control->control;
	text_position_supdate(data->string,control);
}

void handle_stextbox_input(widget *control, int key)
{
	textbox *data = (textbox *)control->shown.data[0]->control;

	if(data->string->data == NULL){
		data->string->data = (char *)calloc(1, 16 * sizeof(char));
		data->string->size = 16;
	}

	if(data->cusorenabled){
		switch(key)
		{
		case GLFW_KEY_BACKSPACE:
			if(data->string->data){
				if(data->string->count > 0){
					if(data->string->displayoffset > 0)
						data->string->displayoffset--;

					if(control->shown.data[0]->action & WIDGET_IS_PASSWORD)
						data->string->textwidth -= (uint16)(data->string->font.c['*'].ax + data->string->font.c['*'].left);
					else{
						if(data->string->font.c[data->string->data[data->string->count - 1]].left >= 0)
							data->string->textwidth -= (uint16)(data->string->font.c[data->string->data[data->string->count - 1]].ax + data->string->font.c[data->string->data[data->string->count - 1]].left);
						else
							data->string->textwidth -= (uint16)data->string->font.c[data->string->data[data->string->count - 1]].ax;
					}
					data->string->count--;
				}
				data->string->data[data->string->count] = '\0';

				stextbox_text_update(data->string, control->shown.data[0]);
			}
			break;

		default:
			if(data->string->data){
				if(data->string->count < data->string->maxchars || data->string->maxchars == 0){
					uint16 i = 0;

					if(control->shown.data[0]->action & WIDGET_IS_PASSWORD){
						data->string->textwidth += (uint16)(data->string->font.c['*'].ax + data->string->font.c['*'].left);

						if(data->string->textwidth >= data->string->width - data->string->offsetx){
							uint16 offset = 0;
							i = 0;
							data->string->displayoffset++;

							for(i = data->string->displayoffset; i <= data->string->count + 1; i++){
								offset += (uint16)(data->string->font.c['*'].ax + data->string->font.c['*'].left);

								if(i == data->string->count +1)
									offset += (uint16)(data->string->font.c['*'].ax + data->string->font.c['*'].left);

								if(offset >= data->string->width - data->string->offsetx){
									data->string->displayoffset++;
									i = data->string->displayoffset;
									offset = 0;
								}
							}
						}

						if(data->string->count + 1 >= data->string->size){
							data->string->size = data->string->size * 2;
							string_resize(data->string, data->string->size);
							control->action |= WIDGET_BUFFER_RESIZE;
						}

						data->string->data[data->string->count] = (char)key;
						data->string->count++;
						stextbox_text_update(data->string, control->shown.data[0]);
					}
					else{
						if(data->string->font.c[key].left >= 0)
							data->string->textwidth += (uint16)(data->string->font.c[key].ax + data->string->font.c[key].left);
						else
							data->string->textwidth += (uint16)(data->string->font.c[key].ax);

						if(data->string->textwidth >= data->string->width - data->string->offsetx){
							uint16 offset = 0;
							i = 0;
							data->string->displayoffset++;

							for(i = data->string->displayoffset; i <= data->string->count + 1; i++){
								if(data->string->font.c[data->string->data[i]].left >= 0)
									offset += (uint16)(data->string->font.c[data->string->data[i]].ax + data->string->font.c[data->string->data[i]].left);
								else
									offset += (uint16)(data->string->font.c[data->string->data[i]].ax);

								if(i == data->string->count +1){
									if(data->string->font.c[key].left >= 0)
										offset += (uint16)(data->string->font.c[key].ax + data->string->font.c[key].left);
									else
										offset += (uint16)(data->string->font.c[key].ax);
								}
								if(offset >= data->string->width - data->string->offsetx){
									data->string->displayoffset++;
									i = data->string->displayoffset;
									offset = 0;
								}
							}
						}

						if(data->string->count + 1 >= data->string->size){
							data->string->size = data->string->size * 2;
							string_resize(data->string, data->string->size);
							control->action |= WIDGET_BUFFER_RESIZE;
						}

						data->string->data[data->string->count] = (char)key;
						data->string->count++;
						stextbox_text_update(data->string, control->shown.data[0]);
					}
				}
			}
		}
	}
}

void create_mtextbox(widget *control, widget *parent, uint16 x, uint16 y, uint16 width, uint16 height, uint8 offsetx, uint8 offsety, uint16 sx, uint16 sy, uint8 red, uint8 blue, uint8 green, uint8 alpha, uint16 maxchars, uint16 maxcharpl, uint8 fontid, char *imgpath, widget *clone)
{
	textbox *init_text = (textbox *)calloc(1,sizeof(textbox));
	textbox_w *init_widget = (textbox_w *)calloc(1,sizeof(textbox_w));

	if(init_text == NULL || init_widget == NULL)
		error_handler(DE_ERROR_POINTER_NULL,",init_text or init_widget in create_mtextbox()\n");

	widget_init(control);
	control->pos.x = x;
	control->pos.y = y;
	control->sizex = sx;
	control->sizey = sy;
	control->imgpos.x = 0;
	control->imgpos.y = 0;
	control->type = CONTROL_TEXTBOX;
	control->controldraw = &draw_mtextbox;
	control->controlmousepress = &handle_mtextbox_click;
	control->controlupdatepos = &handle_mtextbox_move;
	control->controlkeypressed = &handle_mtextbox_input;
	if(clone)
		clone_control_images(control, clone, FALSE, FALSE);
	else
		set_control_image(control, comb_2str(GUI_PATH, imgpath));

	widget_init(&init_widget->wstring);
	init_widget->wstring.pos.x = 0;
	init_widget->wstring.pos.y = 0;
	init_widget->wstring.imgpos.x = 0;
	init_widget->wstring.imgpos.y = 0;
	init_widget->wstring.type = CONTROL_TEXTBOX;
	init_widget->wstring.controldraw = &draw_mtextbox_text;
	init_widget->wstring.controlmousepress = &handle_mtextbox_click;
	init_widget->wstring.controlupdatepos = &handle_mtextbox_text_move;
	control->action |= WIDGET_IS_MULTI_LINED;
	init_widget->wstring.action |= WIDGET_IS_MULTI_LINED;
	control->action |= WIDGET_CAN_FOCUS;

	init_text->string = (text *)calloc(1,sizeof(text));

	if(init_text->string == NULL)
		error_handler(DE_ERROR_POINTER_NULL, ",init_text->string in create_mtextbox()\n");

	text_set(init_text->string,init_widget->wstring.pos.x,init_widget->wstring.pos.y,sx,sy,offsetx,offsety,maxchars,maxcharpl,fontid,red,blue,green,alpha,NULL);
	init_text->cursorblink = .8;
	init_text->cursortime = 0;
	init_text->cusorenabled = FALSE;
	init_text->cursorheight = init_text->string->font.h - 3;
	init_text->cursorwidth = 2;

	control->height = height;
	control->width = width;
	init_widget->wstring.height = sy;
	init_widget->wstring.width = sx;
	init_widget->wstring.control = init_text;

	control->control = init_widget;
	widget_add(parent,control);
	widget_add(control,&init_widget->wstring);

	create_widget_vertex_buffer(control);
	create_cursor_vertex(&init_widget->wstring);
}

void draw_mtextbox(widget *control)
{
	draw_widget(control);
}

void draw_mtextbox_text(widget *control)
{
	textbox *data = (textbox *)control->control;

	if(data->string->buf.isize > 0)
		text_draw(data->string);

	if(data->cusorenabled){
		double tick = glfwGetTime();

		if(control->parent != widget_get_focused())
			data->cusorenabled = FALSE;

		if(data->cursortime > tick)
			draw_cursor(control);
		else{
			if(data->cursortime + data->cursorblink < tick || data ->cursortime == 0)
				data->cursortime = tick + data->cursorblink;
		}
	}
}

void handle_mtextbox_click(widget *control, int button, int pressed)
{
	textbox *data = (textbox *)control->shown.data[0]->control;

	if(data->cusorenabled == FALSE)
		data->cusorenabled = TRUE;

	control->mousepress(control,button,pressed);
}

void handle_mtextbox_move(widget *control)
{
	widget_update_vector(control);
}

void handle_mtextbox_text_move(widget *control)
{
	textbox *data = (textbox *)control->control;
	text_position_mupdate(data->string,control);
}

void handle_mtextbox_input(widget *control, int key)
{
	textbox *data = (textbox *)control->shown.data[0]->control;

	if(data->string->data == NULL){
		data->string->data = (char *)calloc(1, 16 * sizeof(char));
		data->string->size = 16;
	}

	if(data->cusorenabled){
		switch(key)
		{
		case GLFW_KEY_BACKSPACE:
			if(data->string->data){
				if(data->string->count > 0)
					data->string->count--;

				data->string->data[data->string->count] = '\0';

				mtextbox_text_update(data->string, control->shown.data[0]);
			}
			break;

		default:
			if(data->string->data && data->string->textheight <= data->string->height - data->string->offsety){
				if(data->string->count < data->string->maxchars || data->string->maxchars == 0){
					if(data->string->count + 1 >= data->string->size){
						data->string->size = data->string->size * 2;
						string_resize(data->string, data->string->size);
						control->action |= WIDGET_BUFFER_RESIZE;
					}

					data->string->data[data->string->count] = (char)key;
					data->string->count++;

					mtextbox_text_update(data->string, control->shown.data[0]);
				}
			}
		}
	}
}

void create_listbox(widget *control, widget *parent, uint16 x, uint16 y, uint16 height, uint16 width,uint16 barwidth, uint16 buttonheight, uint16 buttonwidth, uint16 sizey, uint16 sizex, uint8 offsetx, uint8 offsety, uint32 amount, uint8 maxchars, uint8 fontid, uint8 red, uint8 green, uint8 blue, uint8 alpha, char *imglistbg, char *imgbg, char *imgup, char *imgdown, char *bar, char *select, char *mouseover, widget *clone)
{
	listbox *list = (listbox *)calloc(1,sizeof(listbox));
	listbox *list2 = NULL;

	uint32 i = 0;
	atlas prefont = get_atlas(fontid);

	list->list = (widget **)calloc(1, amount * sizeof(widget));
	list->select = (widget *)calloc(1,sizeof(widget));
	list->selectover = (widget *)calloc(1,sizeof(widget));

	if(list == NULL || list->list == NULL || list->select == NULL || list->selectover == NULL)
		error_handler(DE_ERROR_POINTER_NULL, ",list, list->list,list->select or list->selectover in create_listbox()\n");

	widget_init(control);
	if(clone){
		list2 = (listbox *)clone->control;
		clone_control_images(control, clone, FALSE, FALSE);
	}
	else
		set_control_image(control, comb_2str(GUI_PATH, imglistbg));
	control->pos.x = x;
	control->pos.y = y;
	control->height = control->img->height;
	control->width = control->img->width;
	control->sizey = sizey;
	control->sizex = sizex;
	control->imgpos.x = 0;
	control->imgpos.y = 0;
	control->controlmousepress = &handle_listbox_click;
	control->controlmouserelease = &handle_listbox_release;
	control->controlmouseover = &handle_listbox_mouse_over;
	control->controlmouseexit = &handle_listbox_mouse_exit;
	control->controlupdatepos = &handle_listbox_move;
	control->type = CONTROL_LISTBOX;
	control->controldraw = &draw_listbox;

	widget_init(list->select);
	if(!clone)
		set_control_image(list->select, comb_2str(GUI_PATH, select));
	list->select->pos.x = offsetx;
	list->select->pos.y = offsety;
	list->select->height = list->select->img->height;
	list->select->width = list->select->img->width;
	list->select->sizey = prefont.h;
	list->select->sizex = sizex  - 22;
	list->select->imgpos.x = 0;
	list->select->imgpos.y = 0;
	list->select->type = CONTROL_PICTUREBOX;
	list->select->parent = control;

	widget_init(list->selectover);
	if(!clone)
		set_control_image(list->selectover, comb_2str(GUI_PATH, mouseover));
	list->selectover->pos.x = offsetx;
	list->selectover->pos.y = offsety;
	list->selectover->height = list->selectover->img->height;
	list->selectover->width = list->selectover->img->width;
	list->selectover->sizey = prefont.h;
	list->selectover->sizex = sizex  - 22;
	list->selectover->imgpos.x = 0;
	list->selectover->imgpos.y = 0;
	list->selectover->type = CONTROL_PICTUREBOX;
	list->selectover->parent = control;

	list->offsetx = offsetx;
	list->offsety = offsety;
	list->voffset = 0;

	widget_add(parent,control);

	create_widget_vertex_buffer(control);
	create_widget_vertex_buffer(list->selectover);
	create_widget_vertex_buffer(list->select);

	for(i = 0; i < amount; i++){
		label *init_text = (label *)calloc(1,sizeof(label));

		list->list[i] = (widget *)calloc(1,sizeof(widget));

		if(init_text == NULL || list->list[i] == NULL)
			error_handler(DE_ERROR_POINTER_NULL,"init_text or list->list[i] in create_listbox()\n");

		widget_init(list->list[i]);

		list->list[i]->imgpos.x = 0;
		list->list[i]->imgpos.y = 0;
		list->list[i]->type = CONTROL_LABEL;
		list->list[i]->controldraw = &draw_label;
		list->list[i]->controlmousepress = &handle_listbox_label_click;
		list->list[i]->controlmouseover = &handle_listbox_label_mouse_over;
		list->list[i]->controlmouseexit = &handle_listbox_label_mouse_exit;
		list->list[i]->controlupdatepos = &handle_slabel_move;
		list->list[i]->action |= WIDGET_CAN_USE_EVENT;
		list->list[i]->parent = control;

		init_text->string = (text *)calloc(1,sizeof(text));

		if(init_text->string == NULL)
			error_handler(DE_ERROR_POINTER_NULL,"init_text->string in create_listbox()\n");

		text_set(init_text->string,0,0,width - 22,prefont.h,offsetx,offsety,maxchars,0,fontid,red,blue,green,alpha,int_to_string(i + 1));

		list->list[i]->height = prefont.h;
		list->list[i]->sizey = prefont.h;
		list->list[i]->width = width - 22;
		list->list[i]->sizex = width - 22;
		list->list[i]->control = init_text;
		list->list[i]->value = i;

		create_text_vertex(init_text->string, list->list[i]);
	}

	list->max = height/(prefont.h + offsety);

	for(i = 0; i < list->max; i++){
		label *lbl = (label *)list->list[i]->control;

		if( i == 0){
			list->list[i]->pos.x = offsetx;
			list->list[i]->pos.y = offsety ;
			text_position_supdate(lbl->string, list->list[i]);
		}
		else{
			list->list[i]->pos.x = offsetx;
			list->list[i]->pos.y = offsety + ( i * prefont.h );
			text_position_supdate(lbl->string, list->list[i]);
		}
	}

	list->count = amount;
	list->size = amount;

	control->control = list;
	if(list2)
		create_vscrollbar(&list->vbar,control,width - barwidth,0,height,barwidth,buttonheight,buttonwidth,sizey,22,0,amount - list->max,imgbg,imgup,imgdown,bar,&list2->vbar);
	else
		create_vscrollbar(&list->vbar,control,width - barwidth,0,height,barwidth,buttonheight,buttonwidth,sizey,22,0,amount - list->max,imgbg,imgup,imgdown,bar,NULL);
	set_scrollbar_buttons(&list->vbar, &handle_listbox_scroll);
}

void unload_listbox(widget *control)
{
	uint32 i = 0;
	listbox *list = (listbox *)control->control;

	for(i = 0; i < list->count; i++){
		label *init_text = (label *)list->list[i]->data;
		free(init_text->string->buf.data);
		glDeleteBuffers(1, &init_text->string->buf.buffer);
		glDeleteBuffers(1, &init_text->string->buf.index);
		free(init_text->string);
		init_text->string->data = NULL;

		free(list->list[i]->data);
		free(list->list[i]->control);

		if(!(list->list[i]->action & WIDGET_USED_CLONE))
			free(list->list[i]->img);

		free(list->list[i]->shown.data);
		free(list->list[i]->hidden.data);
		widget_null(list->list[i]);
		free(list->list[i]);
		list->list[i] = NULL;
	}

	free(list->list);
	list->list = NULL;
	widget_null(list->control);
	list->control = NULL;

	free(list->select->data);
	free(list->select->control);

	if(!(list->select->action & WIDGET_USED_CLONE))
		free(list->select->img);

	free(list->select->shown.data);
	free(list->select->hidden.data);
	glDeleteBuffers(1, &list->select->buf.buffer);
	glDeleteBuffers(1, &list->select->buf.index);
	widget_null(list->select);
	free(list->select);
	list->select = NULL;

	free(list->selectover->data);
	free(list->selectover->control);

	if(!(list->selectover->action & WIDGET_USED_CLONE))
		free(list->selectover->img);

	free(list->selectover->shown.data);
	free(list->selectover->hidden.data);
	glDeleteBuffers(1, &list->selectover->buf.buffer);
	glDeleteBuffers(1, &list->selectover->buf.index);
	widget_null(list->selectover);
	free(list->selectover);
	list->selectover = NULL;

	free(control->data);
	free(control->control);

	if(!(control->action & WIDGET_USED_CLONE))
		free(control->img);

	free(control->shown.data);
	free(control->hidden.data);
	glDeleteBuffers(1, &control->buf.buffer);
	glDeleteBuffers(1, &control->buf.index);
}

void draw_listbox(widget *control)
{
	listbox *list = (listbox *)control->control;
	uint32 i = 0;

	draw_widget(control);

	if(list->select->value)
		draw_widget(list->select);

	if(list->selectover->value)
		draw_widget(list->selectover);

	for(i = list->voffset; i < list->voffset + list->max; i++)
		list->list[i]->controldraw(list->list[i]);
}

void handle_listbox_release(widget *control, int button, int pressed)
{
	listbox *list = (listbox *)control->control;
	uint32 i = 0;

	for(i = list->voffset; i < list->voffset + list->max; i++){
		if(widget_rect_contains(list->list[i])){
			if(list->list[i]->action & WIDGET_CLICKED){
				list->list[i]->action &= ~(WIDGET_CLICKED);
				list->list[i]->controlmousepress(list->list[i],button,pressed);
			}
		}
	}
}

void handle_listbox_click(widget *control, int button, int pressed)
{
	listbox *list = (listbox *)control->control;
	uint32 i = 0;

	if(list->control && widget_rect_contains(list->control)){
		list->select->value = FALSE;
		list->control = NULL;
	}
	else{
		for(i = list->voffset; i < list->voffset + list->max; i++){
			if(widget_rect_contains(list->list[i])){
				list->list[i]->action |= WIDGET_CLICKED;
				list->list[i]->controlmousepress(list->list[i],button,pressed);
			}
		}
	}
}

void handle_listbox_mouse_over(widget *control)
{
	listbox *list = (listbox *)control->control;
	uint32 i = 0;

	for(i = list->voffset; i < list->voffset + list->max; i++){
		if(widget_rect_contains(list->list[i])){
			list->list[i]->action |= WIDGET_MOUSE_OVER;
			list->list[i]->controlmouseover(list->list[i]);
		}
		else{
			if(list->list[i]->action & WIDGET_MOUSE_OVER){
				list->list[i]->action &= ~(WIDGET_AVOID_BUFFER_UPDATE);
				list->list[i]->action &= ~(WIDGET_MOUSE_OVER);
				list->list[i]->controlmouseexit(list->list[i]);
			}
		}
	}
}

void handle_listbox_mouse_exit(widget *control)
{
	listbox *list = (listbox *)control->control;
	uint32 i = 0;

	//clear all if mouse was still preset to over after moving off the list box.
	for(i = list->voffset; i < list->voffset + list->max; i++){
		if(list->list[i]->action & WIDGET_MOUSE_OVER){
			list->list[i]->action &= ~(WIDGET_AVOID_BUFFER_UPDATE);
			list->list[i]->action &= ~(WIDGET_MOUSE_OVER);
			list->list[i]->controlmouseexit(list->list[i]);
		}
	}
}

void handle_listbox_label_click(widget *control, int button, int pressed)
{
	listbox *list = (listbox *)control->parent->control;

	if (widget_usable(control)){
		if (control->action & WIDGET_CLICKED){
			list->select->parent = control;
			list->select->value = TRUE;
			widget_update_vector(list->select);
			list->control = control;
		}
	}

	control->mousepress(control,button,pressed);
}

void handle_listbox_label_mouse_over(widget *control)
{
	listbox *list = (listbox *)control->parent->control;

	if(!(control->action & WIDGET_AVOID_BUFFER_UPDATE)){
		if (control->action & WIDGET_MOUSE_OVER){
			list->selectover->parent = control;
			list->selectover->value = TRUE;
			widget_update_vector(list->selectover);
			control->action |= WIDGET_AVOID_BUFFER_UPDATE;
		}
	}

	control->mouseover(control);
}

void handle_listbox_label_mouse_exit(widget *control)
{
	listbox *list = (listbox *)control->parent->control;
	uint32 i = 0;
	sbool isover = FALSE;

	for(i = list->voffset; i < list->voffset + list->max; i++){
		if(list->list[i]->action & WIDGET_MOUSE_OVER)
			isover = TRUE;
	}

	if(!isover)
		list->selectover->value = FALSE;

	control->mouseexit(control);
}

void handle_listbox_scroll(widget *control, int button, int pressed)
{
	listbox *list = (listbox *)control->parent->parent->control;
	uint32 i = 0;
	sbool isonlist = FALSE;

	list->voffset = control->parent->value;

	for(i = list->voffset; i < list->voffset + list->max; i++){
		label *lbl = (label *)list->list[i]->control;

		if(list->control && list->control == list->list[i])
			isonlist = TRUE;

		if( i == 0 + list->voffset){
			list->list[i]->pos.x =  list->offsetx;
			list->list[i]->pos.y = list->offsety;
			text_position_supdate(lbl->string, list->list[i]);
		}
		else{
			list->list[i]->pos.x = list->offsetx;
			list->list[i]->pos.y = list->offsety + ( (i - list->voffset) * lbl->string->font.h);
			text_position_supdate(lbl->string, list->list[i]);
		}
	}

	if(isonlist){
		list->select->value = TRUE;
		widget_update_vector(list->select);
	}
	else
		list->select->value = FALSE;
}

void handle_listbox_move(widget *control)
{
	listbox *list = (listbox *)control->control;
	uint32 i = 0;

	widget_update_vector(control);

	//clear all if mouse was still preset to over after moving off the list box.
	for(i = list->voffset; i < list->voffset + list->max; i++)
		list->list[i]->controlupdatepos(list->list[i]);
}

void *get_list_data(widget *control)
{
	listbox * list = (listbox *)control->control;
	return list->control->data;
}

listbox *get_list(widget *control)
{
	listbox * list = (listbox *)control->control;
	return list;
}

void create_frame(widget *control, widget *parent, uint16 x, uint16 y, uint16 height, uint16 width, sbool events,sbool window_moveable)
{
	widget_init(control);
	control->pos.x = x;
	control->pos.y = y;
	control->height = height;
	control->width = width;
	control->sizey = height;
	control->sizex = width;
	control->imgpos.x = 0;
	control->imgpos.y = 0;
	control->type = CONTROL_FRAME;
	control->controlmousepress = &handle_frame_click;
	control->controlupdatepos = &handle_frame_move;

	if(events)
		control->action |= WIDGET_CAN_USE_EVENT;

	if(window_moveable)
		control->action |= WIDGET_WINDOW_MOVEABLE;

	widget_add(parent,control);
	control->actualpos.x = control->pos.x + control->parent->actualpos.x;
	control->actualpos.y = control->pos.y + control->parent->actualpos.y;
}

void unload_frame(widget *control)
{
	free(control->data);
	free(control->control);
	free(control->shown.data);
	free(control->hidden.data);
}

void handle_frame_click(widget *control, int button, int pressed)
{
	if(control->action & WIDGET_CAN_USE_EVENT)
		control->mousepress(control,button,pressed);

	if(control->action & WIDGET_WINDOW_MOVEABLE){
		if(pressed == 1)
			widget_get_focused()->action &= ~WIDGET_MOVING;
		else{
			if(control->action & WIDGET_WINDOW_MOVEABLE)
				widget_get_focused()->action &= ~WIDGET_MOVING;
		}
	}
}

void handle_frame_move(widget *control)
{
	control->actualpos.x = control->pos.x + control->parent->actualpos.x;
	control->actualpos.y = control->pos.y + control->parent->actualpos.y;
}

//******clip box*******

void create_clipbox(widget *control, widget *parent, uint16 x, uint16 y, uint16 height, uint16 width, uint16 sizey, uint16 sizex)
{
	widget_init(control);
	control->pos.x = x;
	control->pos.y = y;
	control->height = height;
	control->width = width;
	control->sizey = sizey;
	control->sizex = sizex;
	control->imgpos.x = 0;
	control->imgpos.y = 0;
	control->type = CONTROL_CLIPBOX;
	control->controldraw = &draw_clipbox;
	control->controlupdatepos = &handle_clipbox_move;
	widget_add(parent,control);
	control->actualpos.x = control->pos.x + control->parent->actualpos.x;
	control->actualpos.y = control->pos.y + control->parent->actualpos.y;
}

void handle_clipbox_move(widget *control)
{
	control->actualpos.x = control->pos.x + control->parent->actualpos.x;
	control->actualpos.y = control->pos.y + control->parent->actualpos.y;
	control->updatepos(control);
}

void draw_clipbox(widget *control)
{
	glScissor(control->actualpos.x,get_screen_height() - control->height - control->actualpos.y,control->width,control->height);
	glEnable(GL_SCISSOR_TEST);
	control->draw(control);
	glDisable(GL_SCISSOR_TEST);
}

void create_statusbox(widget *control, widget *parent, uint16 x, uint16 y, char *text, widget *stsclone, widget *btnclone)
{
	statusbox_t *gui = (statusbox_t *)calloc(1, sizeof(statusbox_t));

	if(gui == NULL)
		error_handler(DE_ERROR_POINTER_NULL,"status box is NULL in create_statusbox");

	create_picturebox(control,NULL,0,0,32,32,get_screen_height(),get_screen_width(),comb_2str(GUI_PATH, "hide.png"), NULL);
	create_window(&gui->hide,control,x,y,170,378,170,378,"statuswidget.png",stsclone);
	create_label(&gui->lbltext,&gui->hide,15,70,200,100,0,0,0,120,FALSE,1,30,FALSE,text); //0
	create_button(&gui->btnok,&gui->hide,105,110,50,180,50,180,"smallbutton.png", btnclone); //1
	create_label(&gui->lblok, &gui->btnok,75,10,80,25,0,0,0,120,FALSE,2,8,FALSE,"OK"); //2
	gui->lblok.action |= WIDGET_CAN_CLICK_BEHIND;
	control->action |= WIDGET_CAN_FOCUS;
	control->control = gui;
}

void status_box_set_click_event(widget *control, void(*mousepress)(widget *,int,int))
{
	statusbox_t *gui = (statusbox_t *)control->control;
	gui->btnok.mousepress = mousepress;
}

void status_box_text(widget *control, char *text)
{
	statusbox_t *gui = (statusbox_t *)control->control;
	update_label_string(&gui->lbltext, text);
}