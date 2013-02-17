/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/

#include "render.h"
#include "program_path.h"
#include "controls.h"
#include "function.h"
#include <stdlib.h>

//Buttons
void create_button(widget *control, uint16 x, uint16 y, uint16 height, uint16 width, widget *parent, char *image_path)
{
	widget_init(control);
	control->pos.x = x;
	control->pos.y = y;
	control->height = height;
	control->width = width;
	control->imgpos.x = 0;
	control->imgpos.y = 0;
	control->type = CONTROL_BUTTON;
	control->draw = &draw_buttons;
	control->control = NULL;
	control->controlmousepress = &handle_button_click;

	load_image(get_path(image_path), &control->img);
	widget_add(parent,control);
}

void draw_buttons(widget *control)
{
	if (widget_has_mouse_over(control)){
		if (control->action & WIDGET_CLICKED){
			control->imgpos.x = control->width;
			control->imgpos.y = 0;
		}
		else{
			control->imgpos.y = 0;
			control->imgpos.x = 2 * control->width;
		}
	}
	else{
		control->imgpos.y = 0;
		control->imgpos.x = 0;
	}

	draw_widget(control);
}

void handle_button_click(widget *control, int button, int pressed)
{
	control->mousepress(control,button,pressed);
}
//labels
void create_label(widget *control, uint16 x, uint16 y, uint8 size, uint8 red, uint8 blue, uint8 green, uint8 alpha, sbool can_use_mouse, widget *parent, char *label_text)
{
	label *init_text;
	vector2ui hw;

	widget_init(control);
	control->pos.x = x;
	control->pos.y = y;
	control->imgpos.x = 0;
	control->imgpos.y = 0;
	control->type = CONTROL_LABEL;
	control->draw = &draw_label;
	control->controlmousepress = &handle_label_click;

	if(can_use_mouse){
		control->action |= WIDGET_CAN_USE_EVENT;
	}

	init_text = (label *)calloc(1,sizeof(label));
	init_text->string = (text *)calloc(1,sizeof(text));
	text_set(init_text->string,x,y,size,red,blue,green,alpha,label_text);

	hw = text_get_string_hw(label_text,size);
	control->height = hw.y;
	control->width = hw.x;
	control->control = init_text;
	widget_add(parent,control);
}

void draw_label(widget *control)
{
	label *data = (label *)control->control;
	text_draw(data->string, control->parent);
}

void handle_label_click(widget *control, int button, int pressed)
{
	if(control->action & WIDGET_CAN_USE_EVENT){
		control->mousepress(control,button,pressed);
	}
}

void create_window(widget *control, uint16 x, uint16 y, uint16 height, uint16 width, widget *parent, char *image_path)
{
	window *init_window;

	widget_init(control);
	control->pos.x = x;
	control->pos.y = y;
	control->height = height;
	control->width = width;
	control->imgpos.x = 0;
	control->imgpos.y = 0;
	control->type = CONTROL_WINDOW;
	control->draw = &draw_windows;
	control->controlmousepress = &handle_window_click;

	init_window = (window *)calloc(1,sizeof(window));
	init_window->frame.x = 0;
	init_window->frame.y = 0;
	init_window->frame.w = width;
	init_window->frame.h = height;

	control ->control = init_window;

	load_image(get_path(image_path), &control->img);

	widget_add(parent,control);

	init_window = NULL;
	free(init_window);
}

void create_window_framed(widget *control, uint16 x, uint16 y, uint16 height, uint16 width, uint16 framex, uint16 framey,uint16 frameh, uint16 framew, widget *parent, char *image_path)
{
	window *init_window;

	widget_init(control);
	control->pos.x = x;
	control->pos.y = y;
	control->height = height;
	control->width = width;
	control->imgpos.x = 0;
	control->imgpos.y = 0;
	control->type = CONTROL_WINDOW;
	control->draw = &draw_windows;;
	control->controlmousepress = &handle_window_click;

	init_window = (window *)calloc(1,sizeof(window));
	init_window->frame.x = framex;
	init_window->frame.y = framey;
	init_window->frame.w = framew;
	init_window->frame.h = frameh;

	control->control = init_window;

	load_image(get_path(image_path), &control->img);
	widget_add(parent,control);
}

void draw_windows(widget *control)
{
	draw_widget(control);
}

void handle_window_click(widget *control, int button, int pressed){}

void create_checkbox(widget *control, uint16 x, uint16 y, uint16 height, uint16 width, widget *parent, char *image_path)
{
	widget_init(control);
	control->pos.x = x;
	control->pos.y = y;
	control->height = height;
	control->width = width;
	control->imgpos.x = 0;
	control->imgpos.y = 0;
	control->type = CONTROL_CHECKBOX;
	control->draw = &draw_checkbox;
	control->control = NULL;
	control->controlmousepress = handle_check_click;

	load_image(get_path(image_path), &control->img);
	widget_add(parent,control);
}

void draw_checkbox(widget *control)
{
	if (control->action & WIDGET_CHECKED){
		control->imgpos.x = 0;
		control->imgpos.y = 0;
	}
	else{
		control->imgpos.y = control->width;
		control->imgpos.x = 0;
	}

	draw_widget(control);
}

void handle_check_click(widget *control, int button, int pressed)
{
	if(button == 0 && pressed == TRUE)
	{
		if(control->action & WIDGET_CHECKED){
			control->action &= ~(WIDGET_CHECKED);
		}
		else{
			control->action |= WIDGET_CHECKED;
		}
	}
	control->mousepress(control,button,pressed);
}

void create_radio(widget *control, uint16 x, uint16 y, uint16 height, uint16 width, sbool istrue, widget *parent, char *image_path)
{
	radio *init_radio;

	widget_init(control);
	control->pos.x = x;
	control->pos.y = y;
	control->height = height;
	control->width = width;
	control->imgpos.x = 0;
	control->imgpos.y = 0;
	control->type = CONTROL_RADIO;
	control->draw = &draw_radio;

	if(istrue)
		control->action |= WIDGET_CHECKED;

	init_radio = (radio *)calloc(1,sizeof(radio));
	init_radio->ammount = 0;
	init_radio->count = 0;
	init_radio->main = NULL;
	init_radio->list = NULL;

	control->control = init_radio;
	control->controlmousepress = handle_radio_click;

	load_image(get_path(image_path), &control->img);
	widget_add(parent,control);
}

void draw_radio(widget *control)
{
	if (control->action & WIDGET_CHECKED){
		control->imgpos.x = 0;
		control->imgpos.y = 0;
	}
	else{
		control->imgpos.y = control->width;
		control->imgpos.x = 0;
	}

	draw_widget(control);
}

void resize_radio_list(radio *controls, uint16 size)
{
	widget **data = NULL;

	if(controls == NULL)
		return;//TODO: add error handler here.

	data = (widget **)realloc(controls->list, size * sizeof(widget*));

	if (data == NULL)
		return;

	controls->list = data;
}

void link_radio(widget *main, widget *control)
{
	uint16 i = 0;
	radio *data = (radio *)main->control;
	radio *controldata = (radio *)control->control;

	if(main == NULL && control == NULL){
		return;
	}

	if(data->count == 0)
	{
		data->list = (widget **)calloc(1,8 * sizeof(widget));
		data->ammount = 4;
		data->list[0]= control;
		data->count = 1;
	}
	else{
		for( i == 0; i <= data->ammount; i++){
			if( i >= data->ammount)
			{
				data->ammount = (uint16)next_power_of_two(data->ammount);
				resize_radio_list(data,data->ammount);
			}

			if(data->list[i] == NULL){
				data->list[i] = control;
				++data->count;
				break;
			}
		}
	}
	controldata->main =(widget *)calloc(1,sizeof(widget));
	controldata->main = main;
}

void reset_radio(widget *control)
{
	radio *data = (radio *)control->control;
	widget *wgt = NULL;
	uint16 i = 0;

	if(data->main){
		data->main->action &= ~(WIDGET_CHECKED);
		data = (radio *)data->main->control;
	}
	else{
		control->action &= ~(WIDGET_CHECKED);
	}

	for( i = 0; i < data->count; i++){
		wgt = (widget *)data->list[i];
		wgt->action &= ~(WIDGET_CHECKED);
	}
}

void handle_radio_click(widget *control, int button, int pressed)
{
	radio *data = (radio *)control->control;

	if(button == 0 && pressed == TRUE)
	{
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
				if (control->action & WIDGET_CHECKED){
					control->action &= ~(WIDGET_CHECKED);
				}
				else{
					control->action |= WIDGET_CHECKED;
				}
			}
		}
	}

	if(data->main){
		data->main->mousepress(control,button,pressed);
	}
	else{
		control->mousepress(control,button,pressed);
	}
}