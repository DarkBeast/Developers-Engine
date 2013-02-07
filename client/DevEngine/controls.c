/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/

#include "render.h"
#include "program_path.h"
#include "controls.h"
#include <stdlib.h>

//Buttons
void create_button(widget *data, uint16 x, uint16 y, uint16 height, uint16 width, char *image_path)
{
	widget_init(data);
	data->pos.x = x;
	data->pos.y = y;
	data->height = height;
	data->width = width;
	data->imgpos.x = 0;
	data->imgpos.y = 0;
	data->type = CONTROL_BUTTON;
	data->draw = &draw_buttons;
	data ->control = NULL;

	load_image(get_path(image_path), &data->img);
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

//labels
void create_label(widget *data, uint16 x, uint16 y, uint8 size, uint8 red, uint8 blue, uint8 green, uint8 alpha, sbool can_use_mouse, char *label_text)
{
	label *init_text;
	vector2ui hw;

	widget_init(data);
	data->pos.x = x;
	data->pos.y = y;
	data->imgpos.x = 0;
	data->imgpos.y = 0;
	data->type = CONTROL_LABEL;
	data->draw = &draw_label;

	if(can_use_mouse){
		data->action |= WIDGET_CAN_USE_EVENT;
	}

	init_text = (label *)calloc(1,sizeof(label));
	init_text->string = (text *)calloc(1,sizeof(text));
	text_set(init_text->string,x,y,size,red,blue,green,alpha,label_text);

	hw = text_get_string_hw(label_text,size);
	data->height = hw.y;
	data->width = hw.x;
	data->control = init_text;
}

void draw_label(widget *control)
{
	label *data = (label *)control->control;
	text_draw(data->string, control->parent);
}

void change_label(label data)
{
}

void create_window(widget *data, uint16 x, uint16 y, uint16 height, uint16 width, char *image_path)
{
	window *init_window;

	widget_init(data);
	data->pos.x = x;
	data->pos.y = y;
	data->height = height;
	data->width = width;
	data->imgpos.x = 0;
	data->imgpos.y = 0;
	data->type = CONTROL_WINDOW;
	data->draw = &draw_windows;

	init_window = (window *)calloc(1,sizeof(window));
	init_window->frame.x = 0;
	init_window->frame.y = 0;
	init_window->frame.w = width;
	init_window->frame.h = height;

	data ->control = init_window;

	load_image(get_path(image_path), &data->img);

	init_window = NULL;
	free(init_window);
}

void create_window_framed(widget *data, uint16 x, uint16 y, uint16 height, uint16 width, uint16 framex, uint16 framey,uint16 frameh, uint16 framew, char *image_path)
{
	window init_window;

	widget_init(data);
	data->pos.x = x;
	data->pos.y = y;
	data->height = height;
	data->width = width;
	data->imgpos.x = 0;
	data->imgpos.y = 0;
	data->type = CONTROL_WINDOW;
	data->draw = &draw_windows;;

	init_window.frame.x = framex;
	init_window.frame.y = framey;
	init_window.frame.w = framew;
	init_window.frame.h = frameh;

	data ->control = &init_window;

	load_image(get_path(image_path), &data->img);
}

void draw_windows(widget *control)
{
	draw_widget(control);
}

void create_checkbox(widget *data, uint16 x, uint16 y, uint16 height, uint16 width, char *image_path)
{
	widget_init(data);
	data->pos.x = x;
	data->pos.y = y;
	data->height = height;
	data->width = width;
	data->imgpos.x = 0;
	data->imgpos.y = 0;
	data->type = CONTROL_CHECKBOX;
	data->draw = &draw_checkbox;
	data ->control = NULL;

	load_image(get_path(image_path), &data->img);
}

void draw_checkbox(widget *control)
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