/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/

#include "render.h"
#include "program_path.h"
#include "controls.h"
#include "function.h"
#include <stdlib.h>

void set_control_image(widget *control, char *path)
{
	control->img = (image *)calloc(1,sizeof(image));

	load_image(get_path(path), control->img);
}

//Buttons
void create_button(widget *control, widget *parent, uint16 x, uint16 y, uint16 height, uint16 width, uint16 sizey, uint16 sizex, char *path)
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
	set_control_image(control, path);

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
	create_text_vertex(init_text->string, control);

	init_text = NULL;
	free(init_text);
}

void draw_label(widget *control)
{
	label *data = (label *)control->control;
	text_draw_beta(data->string, control->parent);
}

void handle_label_click(widget *control, int button, int pressed)
{
	if(control->action & WIDGET_CAN_USE_EVENT){
		control->mousepress(control,button,pressed);
	}
}

void create_window(widget *control, widget *parent, uint16 x, uint16 y, uint16 height, uint16 width, uint16 sizey, uint16 sizex, char * path)
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

	set_control_image(control,path);

	init_window = (window *)calloc(1,sizeof(window));
	init_window->frame.x = 0;
	init_window->frame.y = 0;
	init_window->frame.w = width;
	init_window->frame.h = height;
	control ->control = init_window;

	widget_add(parent,control);
	create_widget_vertex_buffer(control); // must be called after widget add to get the correct Parent.

	init_window = NULL;
	free(init_window);
}

void create_window_framed(widget *control, widget *parent, uint16 x, uint16 y, uint16 height, uint16 width, uint16 sizey, uint16 sizex, uint16 framex, uint16 framey,uint16 frameh, uint16 framew, char *path)
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
	set_control_image(control, path);

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
	//draw_widget(control);
	draw_widget_test(control);
}

void handle_window_click(widget *control, int button, int pressed){}

void create_checkbox(widget *control, widget *parent, uint16 x, uint16 y, uint16 height, uint16 width, uint16 sizey, uint16 sizex, char *path)
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
	set_control_image(control, path);

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

void create_radio(widget *control, widget *parent, uint16 x, uint16 y, uint16 height, uint16 width, uint16 sizey, uint16 sizex, sbool istrue, char *path)
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
	set_control_image(control, path);

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

void create_hprogressbar(widget *control, widget *parent, uint16 x, uint16 y, uint16 height, uint16 width, uint16 sizey, uint16 sizex, uint8 value, char *path)
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
	control->type = CONTROL_HPROGRESSBAR;
	control->draw = &draw_hprogressbar;
	control->controlmousepress = handle_hprogressbar_click;
	set_control_image(control, path);

	control->value = value;

	widget_add(parent,control);
}

void draw_hprogressbar(widget *control)
{
	draw_widget(control);
	draw_widget_hprogressbar(control);
}

void handle_hprogressbar_click(widget *control, int button, int pressed)
{
	control->mousepress(control,button,pressed);
}

void create_picturebox(widget *control, widget *parent, uint16 x, uint16 y, uint16 height, uint16 width, uint16 sizey, uint16 sizex, char *path)
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
	control->draw = &draw_picturebox;
	control->controlmousepress = handle_picturebox_click;

	control->img  = (image *)calloc(1,sizeof(image));
	load_image(get_path(path), control->img );

	widget_add(parent,control);
}

void update_picturebox(widget *control, uint16 x, uint16 y, uint16 imgposx, uint16 imgposy, uint16 height, uint16 width, uint16 sizex, uint16 sizey, char *path)
{
	if(path != NULL){
		free(control->img);
		control->img  = (image *)calloc(1,sizeof(image));
		load_image(get_path(path), control->img );
	}
	control->pos.x = x;
	control->pos.y = y;
	control->imgpos.x = imgposx;
	control->imgpos.y = imgposy;
	control->height = height;
	control->width = width;
	control->sizex = sizex;
	control->sizey = sizey;
}

void draw_picturebox(widget *control)
{
	draw_widget(control);
}

void handle_picturebox_click(widget *control, int button, int pressed)
{
	control->mousepress(control,button,pressed);
}

void create_hscrollbar(widget *control, widget *parent, uint16 x, uint16 y, uint16 height, uint16 width, uint16 sizey, uint16 sizex, uint16 value, uint16 max_value, char *background,char *buttonleft, char *buttonright,char *scrollbar)
{
	widget *button_left = (widget *)calloc(1, sizeof(widget));
	widget *button_right = (widget *)calloc(1, sizeof(widget));
	widget *bar = (widget *)calloc(1, sizeof(widget));

	widget_init(control);
	widget_init(bar);//moving bar.
	widget_init(button_left);//left
	widget_init(button_right);//right

	control->pos.x = x;
	control->pos.y = y;
	control->height = height;
	control->width = width;
	control->sizey = sizey;
	control->sizex = sizex;
	control->imgpos.x = 0;
	control->imgpos.y = height;
	control->type = CONTROL_HSCROLL_BAR;
	control->draw = &draw_hscrollbar;
	control->controlmousepress = handle_hscrollbar_click;
	set_control_image(control, background);
	control->value = value;

	button_left->pos.x = 0;
	button_left->pos.y = 0;
	button_left->height = 22;
	button_left->width = 20;
	button_left->sizex = ( sizex / button_left->width) + 20;
	button_left->sizey = sizey;
	button_left->controlmousepress = handle_harrowleft_click;
	set_control_image(button_left, buttonleft);

	button_right->height = 22;
	button_right->width = 20;
	button_right->sizex = (sizex / button_right->width) + 20;
	button_right->sizey = sizey;
	button_right->pos.x = sizex - button_right->sizex;
	button_right->pos.y = 0;
	button_right->controlmousepress = handle_harrowright_click;
	set_control_image(button_right, buttonright);

	bar->height = 22;
	bar->width = 8;
	bar->sizex = (sizex - button_right->sizex) / max_value;
	bar->sizey = sizey;
	if(bar->sizex < 3)
		bar->sizex = 3;

	bar->pos.x = (button_left->pos.x + button_left->sizex) + (value *(sizex - (button_left->sizex * 2) ) / max_value);

	bar->value = max_value;
	bar->pos.y = 0;
	set_control_image(bar, scrollbar);

	if(bar->pos.x < button_left->pos.x)
		bar->pos.x = button_left->pos.x;

	if(bar->pos.x > button_right->pos.x)
		bar->pos.x = button_right->pos.x;

	widget_add(parent,control);
	widget_add(control,button_left);
	widget_add(control,button_right);
	widget_add(control,bar);
}

void draw_hscrollbar(widget *control)
{
	draw_widget(control);

	draw_widget(control->shown.data[0]);

	draw_widget(control->shown.data[1]);

	draw_widget(control->shown.data[2]);
}

void handle_hscrollbar_click(widget *control, int button, int pressed)
{
	control->mousepress(control,button,pressed);
}

void handle_harrowleft_click(widget *control, int button, int pressed)
{
	--control->parent->value;

	if(control->parent->value < 0)
		control->parent->value = 0;

	control->parent->shown.data[2]->pos.x = (control->pos.x + control->sizex) +
		(control->parent->value *(control->parent->sizex - (control->sizex * 2) ) / control->parent->shown.data[2]->value);

	control->mousepress(control,button,pressed);
}

void handle_harrowright_click(widget *control, int button, int pressed)
{
	++control->parent->value;

	if(control->parent->value > control->parent->shown.data[2]->value)
		control->parent->value = control->parent->shown.data[2]->value;

	control->parent->shown.data[2]->pos.x = (control->parent->shown.data[0]->pos.x + control->parent->shown.data[0]->sizex) +
		(control->parent->value *(control->parent->sizex - (control->parent->shown.data[0]->sizex * 2)) / control->parent->shown.data[2]->value);

	control->mousepress(control,button,pressed);
}

void handle_hbar_click(widget *control, int button, int pressed)
{
	control->mousepress(control,button,pressed);
}

//Vertical scrollbar testing
void create_vscrollbar(widget *control, widget *parent, uint16 x, uint16 y, uint16 height, uint16 width, uint16 sizey, uint16 sizex, uint16 value, uint16 max_value, char *background,char *buttontop, char *buttonbottom,char *scrollbar)
{
	widget *button_top = (widget *)calloc(1, sizeof(widget));
	widget *button_bottom= (widget *)calloc(1, sizeof(widget));
	widget *bar = (widget *)calloc(1, sizeof(widget));

	widget_init(control);
	widget_init(bar);//moving bar.
	widget_init(button_top);//top
	widget_init(button_bottom);//bottom

	control->pos.x = x;
	control->pos.y = y;
	control->height = height;
	control->width = width;
	control->sizey = sizey;
	control->sizex = sizex;
	control->imgpos.x = 0;
	control->imgpos.y = height;
	control->type = CONTROL_VSCROLL_BAR;
	control->draw = &draw_vscrollbar;
	control->controlmousepress = handle_vscrollbar_click;
	set_control_image(control, background);
	control->value = value;

	button_top->pos.x = 0;
	button_top->pos.y = 0;
	button_top->height = 20;
	button_top->width = 22;
	button_top->sizex = sizex;
	button_top->sizey = (sizey / button_top->height) + 20;
	button_top->controlmousepress = handle_varrowtop_click;
	set_control_image(button_top, buttontop);

	button_bottom->height = 20;
	button_bottom->width = 22;
	button_bottom->sizex = sizex;
	button_bottom->sizey = (sizey / button_bottom->height) + 20;
	button_bottom->pos.x = 0;
	button_bottom->pos.y = sizey - button_bottom->sizey;
	button_bottom->controlmousepress = handle_varrowbottom_click;
	set_control_image(button_bottom, buttonbottom);

	bar->height = 8;
	bar->width = 22;
	bar->sizex = sizex;
	bar->sizey = (sizey - button_bottom->sizey) / max_value;
	if(bar->sizey < 3)
		bar->sizey = 3;

	bar->pos.y = (button_top->pos.y + button_top->sizey) + (value *(sizey - (button_top->sizey * 2) ) / max_value);

	bar->value = max_value;
	bar->pos.x = 0;
	set_control_image(bar, scrollbar);

	if(bar->pos.y < button_top->pos.y)
		bar->pos.y = button_top->pos.y;

	if(bar->pos.y > button_bottom->pos.y)
		bar->pos.y = button_bottom->pos.y;

	widget_add(parent,control);
	widget_add(control,button_top);
	widget_add(control,button_bottom);
	widget_add(control,bar);
}

void draw_vscrollbar(widget *control)
{
	draw_widget(control);

	draw_widget(control->shown.data[0]);

	draw_widget(control->shown.data[1]);

	draw_widget(control->shown.data[2]);
}

void handle_vscrollbar_click(widget *control, int button, int pressed)
{
	control->mousepress(control,button,pressed);
}

void handle_varrowtop_click(widget *control, int button, int pressed)
{
	--control->parent->value;

	if(control->parent->value < 0)
		control->parent->value = 0;

	control->parent->shown.data[2]->pos.y = (control->pos.y + control->sizey) +
		(control->parent->value *(control->parent->sizey - (control->sizey * 2) ) / control->parent->shown.data[2]->value);

	control->mousepress(control,button,pressed);
}

void handle_varrowbottom_click(widget *control, int button, int pressed)
{
	++control->parent->value;

	if(control->parent->value > control->parent->shown.data[2]->value)
		control->parent->value = control->parent->shown.data[2]->value;

	control->parent->shown.data[2]->pos.y = (control->parent->shown.data[0]->pos.y + control->parent->shown.data[0]->sizey) +
		(control->parent->value *(control->parent->sizey - (control->parent->shown.data[0]->sizey * 2)) / control->parent->shown.data[2]->value);

	control->mousepress(control,button,pressed);
}

void handle_vbar_click(widget *control, int button, int pressed)
{
	control->mousepress(control,button,pressed);
}

void create_vprogressbar(widget *control, widget *parent, uint16 x, uint16 y, uint16 height, uint16 width, uint16 sizey, uint16 sizex, uint8 value, char *path)
{
	widget_init(control);
	control->pos.x = x;
	control->pos.y = y;
	control->height = height;
	control->width = width;
	control->sizey = sizey;
	control->sizex = sizex;
	control->imgpos.x = width;
	control->imgpos.y = height;
	control->type = CONTROL_VPROGRESSBAR;
	control->draw = &draw_vprogressbar;
	control->controlmousepress = handle_vprogressbar_click;
	set_control_image(control, path);
	control->value = value;

	widget_add(parent,control);
}

void draw_vprogressbar(widget *control)
{
	draw_widget(control);
	draw_widget_vprogressbar(control);
}

void handle_vprogressbar_click(widget *control, int button, int pressed)
{
	control->mousepress(control,button,pressed);
}