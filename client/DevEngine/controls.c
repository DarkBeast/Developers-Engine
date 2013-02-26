/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/

#include "render.h"
#include "program_path.h"
#include "controls.h"
#include "function.h"
#include <stdlib.h>

image *img;

void load_control_images(void)
{
	img = (image *)calloc(1, 6 * sizeof(image));

	load_image(get_path("image\\button.png"), &img[0]);
	load_image(get_path("image\\button.png"), &img[1]);
	load_image(get_path("image\\window.png"), &img[2]);
	load_image(get_path("image\\check.png"), &img[3]);
	load_image(get_path("image\\radio.png"), &img[4]);
	load_image(get_path("image\\progress.png"), &img[5]);
}

void set_control_image(widget *control, char *path)
{
	control->img = (image *)calloc(1,sizeof(image));

	load_image(get_path(path), control->img);
}

//Buttons
void create_button(widget *control, widget *parent, uint16 x, uint16 y, uint16 height, uint16 width, uint16 sizey, uint16 sizex, uint8 image_id)
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
	control->draw = &draw_buttons;
	control->controlmousepress = &handle_button_click;
	control->img = &img[image_id];

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
void create_label(widget *control, widget *parent, uint16 x, uint16 y, uint8 size, uint8 red, uint8 blue, uint8 green, uint8 alpha, sbool can_use_mouse, char *label_text)
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

	init_text = NULL;
	free(init_text);
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

void create_window(widget *control, widget *parent, uint16 x, uint16 y, uint16 height, uint16 width, uint16 sizey, uint16 sizex, uint8 image_id)
{
	window *init_window;

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
	control->draw = &draw_windows;
	control->controlmousepress = &handle_window_click;
	control->img = &img[image_id];

	init_window = (window *)calloc(1,sizeof(window));
	init_window->frame.x = 0;
	init_window->frame.y = 0;
	init_window->frame.w = width;
	init_window->frame.h = height;
	control ->control = init_window;

	widget_add(parent,control);

	init_window = NULL;
	free(init_window);
}

void create_window_framed(widget *control, widget *parent, uint16 x, uint16 y, uint16 height, uint16 width, uint16 sizey, uint16 sizex, uint16 framex, uint16 framey,uint16 frameh, uint16 framew, uint8 image_id)
{
	window *init_window;

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
	control->draw = &draw_windows;;
	control->controlmousepress = &handle_window_click;
	control->img = &img[image_id];

	init_window = (window *)calloc(1,sizeof(window));
	init_window->frame.x = framex;
	init_window->frame.y = framey;
	init_window->frame.w = framew;
	init_window->frame.h = frameh;
	control->control = init_window;

	widget_add(parent,control);

	init_window = NULL;
	free(init_window);
}

void draw_windows(widget *control)
{
	draw_widget(control);
}

void handle_window_click(widget *control, int button, int pressed){}

void create_checkbox(widget *control, widget *parent, uint16 x, uint16 y, uint16 height, uint16 width, uint16 sizey, uint16 sizex, uint8 image_id)
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
	control->type = CONTROL_CHECKBOX;
	control->draw = &draw_checkbox;
	control->controlmousepress = handle_check_click;
	control->img = &img[image_id];

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

void create_radio(widget *control, widget *parent, uint16 x, uint16 y, uint16 height, uint16 width, uint16 sizey, uint16 sizex, sbool istrue, uint8 image_id)
{
	radio *init_radio;

	widget_init(control);
	control->pos.x = x;
	control->pos.y = y;
	control->height = height;
	control->width = width;
	control->sizey = sizey;
	control->sizex = sizex;
	control->imgpos.x = 0;
	control->imgpos.y = 0;
	control->type = CONTROL_RADIO;
	control->draw = &draw_radio;
	control->controlmousepress = handle_radio_click;
	control->img = &img[image_id];

	if(istrue)
		control->action |= WIDGET_CHECKED;

	init_radio = (radio *)calloc(1,sizeof(radio));
	init_radio->ammount = 0;
	init_radio->count = 0;
	init_radio->main = NULL;
	init_radio->list = NULL;
	control->control = init_radio;

	widget_add(parent,control);

	init_radio = NULL;
	free(init_radio);
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
	data = NULL;
	free(data);
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
		for( i = 0; i <= data->ammount; i++){
			if( i >= data->ammount)
			{
				data->ammount = (uint8)next_power_of_two(data->ammount);
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

	data  = NULL;
	free(data );
	controldata = NULL;
	free(controldata);
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

	data = NULL;
	free(data);
	wgt = NULL;
	free(wgt);
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

	data = NULL;
	free(data);
}

//TEST STUFF

void create_progressbar(widget *control, widget *parent, uint16 x, uint16 y, uint16 height, uint16 width, uint16 sizey, uint16 sizex, uint8 value, uint8 image_id)
{
	progressbar *init_bar;

	widget_init(control);
	control->pos.x = x;
	control->pos.y = y;
	control->height = height;
	control->width = width;
	control->sizey = sizey;
	control->sizex = sizex;
	control->imgpos.x = 0;
	control->imgpos.y = 0;
	control->type = CONTROL_PROGRESSBAR;
	control->draw = &draw_progressbar;
	control->controlmousepress = handle_check_click;
	control->img = &img[image_id];

	init_bar = (progressbar *)calloc(1,sizeof(progressbar));
	init_bar->value = value;
	control->control = init_bar;

	widget_add(parent,control);

	init_bar = NULL;
	free(init_bar);
}

void draw_progressbar(widget *control)
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

void handle_progressbar_click(widget *control, int button, int pressed)
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