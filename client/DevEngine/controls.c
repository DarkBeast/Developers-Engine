/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/

#include "render.h"
#include "program_path.h"
#include "controls.h"
#include "function.h"
#include "error.h"
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
	control->controlmouserelease = &handle_button_release;
	control->controlupdatepos = &handle_button_move;
	control->controlmouseover = &handle_button_mouse_over;
	control->controlmouseexit = &handle_button_mouse_exit;
	set_control_image(control, path);

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
		//control->action |= WIDGET_MOUSE_OVER;
		handle_button_mouse_over(control);
	}
	control->mouserelease(control,button,pressed);
}

void handle_button_click(widget *control, int button, int pressed)
{
	if (widget_has_mouse_over(control)){
		if (control->action & WIDGET_CLICKED){
			control->imgpos.x = control->width;
			control->imgpos.y = 0;
			widget_update_texture_vector(control);
		}
	}

	control->mousepress(control,button,pressed);
}

void handle_button_mouse_over(widget *control)
{
	if(!(control->action & WIDGET_AVOID_BUFFER_UPDATE)){
		if (control->action & WIDGET_MOUSE_OVER){
			control->imgpos.y = 0;
			control->imgpos.x = 2 * control->width;
			control->action |= WIDGET_AVOID_BUFFER_UPDATE;
			widget_update_texture_vector(control);
		}
	}

	control->mouseover(control);
}

void handle_button_mouse_exit(widget *control)
{
	control->imgpos.y = 0;
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
	control->draw = &draw_label;
	control->controlmousepress = &handle_label_click;

	if(multi_lined){
		control->action |= WIDGET_IS_MULTI_LINED;
		control->controlupdatepos = &handle_mlabel_move;
	}
	else{
		control->controlupdatepos = &handle_slabel_move;
	}

	if(events){
		control->action |= WIDGET_CAN_USE_EVENT;
	}

	init_text = (label *)calloc(1,sizeof(label));

	if(init_text == NULL){
		fatal_error(ERROR_POINTER_NULL);
		return;
	}

	init_text->string = (text *)calloc(1,sizeof(text));

	if(init_text->string == NULL){
		fatal_error(ERROR_POINTER_NULL);
		return;
	}

	text_set(init_text->string,x,y,width,height,0,0,0,maxcharspl,fontid,red,blue,green,alpha,string);

	control->height = height;
	control->width = width;
	control->control = init_text;

	widget_add(parent,control);
	create_text_vertex(init_text->string, control);
}

void draw_label(widget *control)
{
	label *data = (label *)control->control;
	text_draw(data->string);
}

void handle_label_click(widget *control, int button, int pressed)
{
	if(control->action & WIDGET_CAN_USE_EVENT){
		control->mousepress(control,button,pressed);
	}
}

void update_label_string(widget *control, char *string)
{
	label *data = (label *)control->control;

	data->string->data = string;
	create_text_vertex(data->string, control);
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

void create_window(widget *control, widget *parent, uint16 x, uint16 y, uint16 height, uint16 width, uint16 sizey, uint16 sizex, char * path)
{
	window *init_window = (window *)calloc(1,sizeof(window));

	if(init_window == NULL){
		fatal_error(ERROR_POINTER_NULL);
		return;
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
	control->draw = &draw_windows;
	control->controlmousepress = &handle_window_click;
	control->controlupdatepos = &handle_windows_move;

	set_control_image(control,path);

	init_window->frame.x = 0;
	init_window->frame.y = 0;
	init_window->frame.w = width;
	init_window->frame.h = height;
	control ->control = init_window;

	widget_add(parent,control);
	create_widget_vertex_buffer(control); // must be called after widget add to get the correct Parent.
}

void create_window_framed(widget *control, widget *parent, uint16 x, uint16 y, uint16 height, uint16 width, uint16 sizey, uint16 sizex, uint16 framex, uint16 framey,uint16 frameh, uint16 framew, char *path)
{
	window *init_window = (window *)calloc(1,sizeof(window));

	if(init_window == NULL){
		fatal_error(ERROR_POINTER_NULL);
		return;
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
	control->draw = &draw_windows;;
	control->controlmousepress = &handle_window_click;
	control->controlupdatepos = &handle_windows_move;
	set_control_image(control, path);

	init_window->frame.x = framex;
	init_window->frame.y = framey;
	init_window->frame.w = framew;
	init_window->frame.h = frameh;
	control->control = init_window;

	widget_add(parent,control);
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
	control->controlupdatepos = &handle_check_move;
	set_control_image(control, path);

	widget_add(parent,control);
	create_widget_vertex_buffer(control);
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

	if (control->action & WIDGET_CHECKED){
		control->imgpos.x = 0;
		control->imgpos.y = 0;
	}
	else{
		control->imgpos.y = control->width;
		control->imgpos.x = 0;
	}

	widget_update_texture_vector(control);
	control->mousepress(control,button,pressed);
}

void handle_check_move(widget *control)
{
	widget_update_vector(control);
}

void create_radio(widget *control, widget *parent, uint16 x, uint16 y, uint16 height, uint16 width, uint16 sizey, uint16 sizex, sbool istrue, char *path)
{
	radio *init_radio = (radio *)calloc(1,sizeof(radio));

	if(init_radio == NULL){
		fatal_error(ERROR_POINTER_NULL);
		return;
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
	control->type = CONTROL_RADIO;
	control->draw = &draw_radio;
	control->controlmousepress = &handle_radio_click;
	control->controlupdatepos = &handle_radio_move;
	set_control_image(control, path);

	if(istrue)
		control->action |= WIDGET_CHECKED;

	init_radio->ammount = 0;
	init_radio->count = 0;
	init_radio->main = NULL;
	init_radio->list = NULL;
	control->control = init_radio;

	widget_add(parent,control);
	create_widget_vertex_buffer(control);
}

void draw_radio(widget *control)
{
	draw_widget(control);
}

void resize_radio_list(radio *controls, uint16 size)
{
	widget **data;

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

		if(data->list == NULL){
			fatal_error(ERROR_POINTER_NULL);
			return;
		}

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
}

void reset_radio(widget *control)
{
	radio *data = (radio *)control->control;
	widget *wgt;
	uint16 i = 0;

	if(data->main){
		data->main->action &= ~(WIDGET_CHECKED);
		data->main->imgpos.y = control->width;
		data->main->imgpos.x = 0;
		widget_update_texture_vector(data->main);
		data = (radio *)data->main->control;
	}
	else{
		control->action &= ~(WIDGET_CHECKED);
		control->imgpos.y = control->width;
		control->imgpos.x = 0;
		widget_update_texture_vector(control);
	}

	for( i = 0; i < data->count; i++){
		wgt = (widget *)data->list[i];
		wgt->action &= ~(WIDGET_CHECKED);
		wgt->imgpos.y = control->width;
		wgt->imgpos.x = 0;
		widget_update_texture_vector(wgt);
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
		if (data->main->action & WIDGET_CHECKED){
			data->main->imgpos.x = 0;
			data->main->imgpos.y = 0;
		}
		else{
			data->main->imgpos.y = data->main->width;
			data->main->imgpos.x = 0;
		}
		widget_update_texture_vector(data->main);
		data->main->mousepress(control,button,pressed);
	}
	else{
		if (control->action & WIDGET_CHECKED){
			control->imgpos.x = 0;
			control->imgpos.y = 0;
		}
		else{
			control->imgpos.y = control->width;
			control->imgpos.x = 0;
		}
		widget_update_texture_vector(control);
		control->mousepress(control,button,pressed);
	}
}

void handle_radio_move(widget *control)
{
	widget_update_vector(control);
}

//TEST STUFF

void create_hprogressbar(widget *control, widget *parent, uint16 x, uint16 y, uint16 height, uint16 width, uint16 sizey, uint16 sizex, uint8 value, char *path)
{
	widget *bar = (widget *)calloc(1, sizeof(widget));

	if(bar == NULL){
		fatal_error(ERROR_POINTER_NULL);
		return;
	}

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
	control->controlmousepress = &handle_hprogressbar_click;
	control->controlupdatepos = &handle_hprogressbar_move;
	set_control_image(control, path);

	control->value = value;

	widget_init(bar);
	bar->pos.x = x;
	bar->pos.y = y;
	bar->height = height;
	bar->width = width;
	bar->sizey = sizey;
	bar->sizex = sizex;
	bar->imgpos.x = 0;
	bar->imgpos.y = height;
	bar->type = CONTROL_HPROGRESSBAR;
	bar->draw = &draw_hprogressbar;
	bar->controlmousepress = &handle_hprogressbar_click;
	bar->controlupdatepos = &handle_hprogressbars_move;
	bar->img->texid = control->img->texid;

	widget_add(parent,control);
	widget_add(control, bar);

	create_widget_vertex_buffer(control);
	create_widget_vertex_buffer(bar);
	widget_update_progressbars_vector(bar);
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
	control->controlmousepress = &handle_picturebox_click;
	control->controlupdatepos = &handle_picturebox_move;

	control->img  = (image *)calloc(1,sizeof(image));
	load_image(get_path(path), control->img );

	widget_add(parent,control);
	create_widget_vertex_buffer(control);
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

void create_hscrollbar(widget *control, widget *parent, uint16 x, uint16 y, uint16 height, uint16 width, uint16 sizey, uint16 sizex, uint16 value, uint16 max_value, char *background,char *buttonleft, char *buttonright,char *scrollbar)
{
	widget *button_left = (widget *)calloc(1, sizeof(widget));
	widget *button_right = (widget *)calloc(1, sizeof(widget));
	widget *bar = (widget *)calloc(1, sizeof(widget));

	if(button_left == NULL || button_right == NULL || bar == NULL){
		fatal_error(ERROR_POINTER_NULL);
		return;
	}

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
	control->controlmousepress = &handle_hscrollbar_click;
	control->controlupdatepos = &handle_hscrollbar_move;
	set_control_image(control, background);
	control->value = value;

	button_left->pos.x = 0;
	button_left->pos.y = 0;
	button_left->height = 22;
	button_left->width = 20;
	button_left->sizex = ( sizex / button_left->width) + 20;
	button_left->sizey = sizey;
	button_left->controlmousepress = &handle_harrowleft_click;
	button_left->controlupdatepos = &handle_hscrollbar_move;
	set_control_image(button_left, buttonleft);

	button_right->height = 22;
	button_right->width = 20;
	button_right->sizex = (sizex / button_right->width) + 20;
	button_right->sizey = sizey;
	button_right->pos.x = sizex - button_right->sizex;
	button_right->pos.y = 0;
	button_right->controlmousepress = &handle_harrowright_click;
	button_right->controlupdatepos = &handle_hscrollbar_move;
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
	bar->controlupdatepos = &handle_hscrollbar_move;

	widget_add(parent,control);
	widget_add(control,button_left);
	widget_add(control,button_right);
	widget_add(control,bar);

	create_widget_vertex_buffer(control);
	create_widget_vertex_buffer(button_left);
	create_widget_vertex_buffer(button_right);
	create_widget_vertex_buffer(bar);
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
	widget_update_vector(control->parent->shown.data[2]);

	control->mousepress(control,button,pressed);
}

void handle_harrowright_click(widget *control, int button, int pressed)
{
	++control->parent->value;

	if(control->parent->value > control->parent->shown.data[2]->value)
		control->parent->value = control->parent->shown.data[2]->value;

	control->parent->shown.data[2]->pos.x = (control->parent->shown.data[0]->pos.x + control->parent->shown.data[0]->sizex) +
		(control->parent->value *(control->parent->sizex - (control->parent->shown.data[0]->sizex * 2)) / control->parent->shown.data[2]->value);
	widget_update_vector(control->parent->shown.data[2]);
	control->mousepress(control,button,pressed);
}

void handle_hbar_click(widget *control, int button, int pressed)
{
	control->mousepress(control,button,pressed);
}

void handle_hscrollbar_move(widget *control)
{
	widget_update_vector(control);
}

//Vertical scrollbar testing
void create_vscrollbar(widget *control, widget *parent, uint16 x, uint16 y, uint16 height, uint16 width, uint16 sizey, uint16 sizex, uint16 value, uint16 max_value, char *background,char *buttontop, char *buttonbottom,char *scrollbar)
{
	widget *button_top = (widget *)calloc(1, sizeof(widget));
	widget *button_bottom= (widget *)calloc(1, sizeof(widget));
	widget *bar = (widget *)calloc(1, sizeof(widget));

	if(button_top == NULL || button_bottom == NULL || bar == NULL){
		fatal_error(ERROR_POINTER_NULL);
		return;
	}

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
	control->controlmousepress = &handle_vscrollbar_click;
	control->controlupdatepos = &handle_vscrollbar_move;
	set_control_image(control, background);
	control->value = value;

	button_top->pos.x = 0;
	button_top->pos.y = 0;
	button_top->height = 20;
	button_top->width = 22;
	button_top->sizex = sizex;
	button_top->sizey = (sizey / button_top->height) + 20;
	button_top->controlmousepress = &handle_varrowtop_click;
	button_top->controlupdatepos = &handle_vscrollbar_move;
	set_control_image(button_top, buttontop);

	button_bottom->height = 20;
	button_bottom->width = 22;
	button_bottom->sizex = sizex;
	button_bottom->sizey = (sizey / button_bottom->height) + 20;
	button_bottom->pos.x = 0;
	button_bottom->pos.y = sizey - button_bottom->sizey;
	button_bottom->controlmousepress = &handle_varrowbottom_click;
	button_bottom->controlupdatepos = &handle_vscrollbar_move;
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
	bar->controlupdatepos = &handle_vscrollbar_move;

	widget_add(parent,control);
	widget_add(control,button_top);
	widget_add(control,button_bottom);
	widget_add(control,bar);

	create_widget_vertex_buffer(control);
	create_widget_vertex_buffer(button_top);
	create_widget_vertex_buffer(button_bottom);
	create_widget_vertex_buffer(bar);
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
	widget_update_vector(control->parent->shown.data[2]);
	control->mousepress(control,button,pressed);
}

void handle_varrowbottom_click(widget *control, int button, int pressed)
{
	++control->parent->value;

	if(control->parent->value > control->parent->shown.data[2]->value)
		control->parent->value = control->parent->shown.data[2]->value;

	control->parent->shown.data[2]->pos.y = (control->parent->shown.data[0]->pos.y + control->parent->shown.data[0]->sizey) +
		(control->parent->value *(control->parent->sizey - (control->parent->shown.data[0]->sizey * 2)) / control->parent->shown.data[2]->value);
	widget_update_vector(control->parent->shown.data[2]);
	control->mousepress(control,button,pressed);
}

void handle_vbar_click(widget *control, int button, int pressed)
{
	control->mousepress(control,button,pressed);
}

void handle_vscrollbar_move(widget *control)
{
	widget_update_vector(control);
}

void create_vprogressbar(widget *control, widget *parent, uint16 x, uint16 y, uint16 height, uint16 width, uint16 sizey, uint16 sizex, uint8 value, char *path)
{
	widget *bar = (widget *)calloc(1, sizeof(widget));

	if(bar == NULL){
		fatal_error(ERROR_POINTER_NULL);
		return;
	}

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
	control->controlmousepress = &handle_vprogressbar_click;
	control->controlupdatepos = &handle_vprogressbar_move;
	set_control_image(control, path);
	control->value = value;

	widget_init(bar);
	bar->pos.x = x;
	bar->pos.y = y;
	bar->height = height;
	bar->width = width;
	bar->sizey = sizey;
	bar->sizex = sizex;
	bar->imgpos.x = width;
	bar->imgpos.y = height;
	bar->type = CONTROL_VPROGRESSBAR;
	bar->draw = &draw_vprogressbar;
	bar->controlmousepress = &handle_vprogressbar_click;
	bar->controlupdatepos = &handle_vprogressbars_move;
	bar->img = control->img;

	widget_add(parent,control);
	widget_add(control,bar);

	create_widget_vertex_buffer(control);
	create_widget_vertex_buffer(bar);
	widget_update_progressbars_vector(bar);
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

//text box
void create_stextbox(widget *control, widget *parent, uint16 x, uint16 y, uint16 width, uint16 height, uint8 offsetx, uint8 offsety, uint16 sx, uint16 sy, uint8 red, uint8 blue, uint8 green, uint8 alpha, uint16 maxchars, uint8 fontid, sbool ispass, char *imgpath)
{
	textbox *init_text = (textbox *)calloc(1,sizeof(textbox));
	widget *string = (widget *)calloc(1, sizeof(widget));

	if(init_text == NULL || string == NULL){
		fatal_error(ERROR_POINTER_NULL);
		return;
	}

	widget_init(control);
	control->pos.x = x;
	control->pos.y = y;
	control->sizex = sx;
	control->sizey = sy;
	control->imgpos.x = 0;
	control->imgpos.y = 0;
	control->type = CONTROL_TEXTBOX;
	control->draw = &draw_mtextbox;
	control->controlmousepress = &handle_stextbox_click;
	control->controlupdatepos = &handle_stextbox_move;
	control->controlkeypressed = &handle_stextbox_input;
	set_control_image(control, imgpath);

	widget_init(string);
	string->pos.x = x;
	string->pos.y = y;
	string->imgpos.x = 0;
	string->imgpos.y = 0;
	string->type = CONTROL_TEXTBOX_TEXT;
	string->draw = &draw_stextbox_text;
	string->controlmousepress = &handle_stextbox_click;
	string->controlupdatepos = &handle_stextbox_text_move;

	if(ispass){
		string->action |= WIDGET_IS_PASSWORD;
	}

	init_text->string = (text *)calloc(1,sizeof(text));

	if(init_text->string == NULL){
		fatal_error(ERROR_POINTER_NULL);
		return;
	}

	text_set(init_text->string,string->pos.x,string->pos.y,width,height,offsetx,offsety,maxchars,0,fontid,red,blue,green,alpha,NULL);

	control->height = height;
	control->width = width;
	string->height = height;
	string->width = width;
	string->control = init_text;

	widget_add(parent,control);
	widget_add(control,string);
	create_widget_vertex_buffer(control);
}

void draw_stextbox(widget *control)
{
	draw_widget(control);
}

void draw_stextbox_text(widget *control)
{
	textbox *data = (textbox *)control->control;

	if(data->string->buf.isize > 0){
		text_draw(data->string);
	}
}

void handle_stextbox_click(widget *control, int button, int pressed)
{
	textbox *data = (textbox *)control->shown.data[0]->control;

	if(data->cusorenabled == FALSE){
		data->cusorenabled = TRUE;
	}

	control->mousepress(control,button,pressed);
}

void handle_stextbox_move(widget *control)
{
	widget_update_vector(control);
}

void handle_stextbox_text_move(widget *control)
{
	textbox *data = (textbox *)control->control;
	if(data->string->buf.isize > 0){
		text_position_supdate(data->string,control);
	}
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
					if(data->string->displayoffset > 0){
						data->string->displayoffset--;
					}

					if(data->string->font->c[data->string->data[data->string->count - 1]].left >= 0){
						data->string->textwidth -= (uint16)(data->string->font->c[data->string->data[data->string->count - 1]].ax + data->string->font->c[data->string->data[data->string->count - 1]].left);
					}
					else{
						data->string->textwidth -= (uint16)data->string->font->c[data->string->data[data->string->count - 1]].ax;
					}

					data->string->count--;
				}
				data->string->data[data->string->count] = NULL;

				stextbox_text_update(data->string, control->shown.data[0]);
			}
			break;

		default:
			if(data->string->data){
				if(data->string->count < data->string->maxchars || data->string->maxchars == 0){
					uint16 i = 0;

					if(data->string->font->c[key].left >= 0){
						data->string->textwidth += (uint16)(data->string->font->c[key].ax + data->string->font->c[key].left);
					}
					else{
						data->string->textwidth += (uint16)(data->string->font->c[key].ax);
					}

					if(data->string->textwidth >= data->string->width - data->string->offsetx){
						uint16 offset = 0;
						i = 0;
						data->string->displayoffset++;

						for(i = data->string->displayoffset; i <= data->string->count + 1; i++){
							if(data->string->font->c[data->string->data[i]].left >= 0){
								offset += (uint16)(data->string->font->c[data->string->data[i]].ax + data->string->font->c[data->string->data[i]].left);
							}
							else{
								offset += (uint16)(data->string->font->c[data->string->data[i]].ax);
							}

							if(i == data->string->count +1){
								if(data->string->font->c[key].left >= 0){
									offset += (uint16)(data->string->font->c[key].ax + data->string->font->c[key].left);
								}
								else{
									offset += (uint16)(data->string->font->c[key].ax);
								}
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

void create_mtextbox(widget *control, widget *parent, uint16 x, uint16 y, uint16 width, uint16 height, uint8 offsetx, uint8 offsety, uint16 sx, uint16 sy, uint8 red, uint8 blue, uint8 green, uint8 alpha, uint16 maxchars, uint16 maxcharpl, uint8 fontid, char *imgpath)
{
	textbox *init_text = (textbox *)calloc(1,sizeof(textbox));
	widget *string = (widget *)calloc(1, sizeof(widget));

	if(init_text == NULL || string == NULL){
		fatal_error(ERROR_POINTER_NULL);
		return;
	}

	widget_init(control);
	control->pos.x = x;
	control->pos.y = y;
	control->sizex = sx;
	control->sizey = sy;
	control->imgpos.x = 0;
	control->imgpos.y = 0;
	control->type = CONTROL_TEXTBOX;
	control->draw = &draw_mtextbox;
	control->controlmousepress = &handle_mtextbox_click;
	control->controlupdatepos = &handle_mtextbox_move;
	control->controlkeypressed = &handle_mtextbox_input;
	set_control_image(control, imgpath);

	widget_init(string);
	string->pos.x = 0;
	string->pos.y = 0;
	string->imgpos.x = 0;
	string->imgpos.y = 0;
	string->type = CONTROL_TEXTBOX_TEXT;
	string->draw = &draw_mtextbox_text;
	string->controlmousepress = &handle_mtextbox_click;
	string->controlupdatepos = &handle_mtextbox_text_move;
	control->action |= WIDGET_IS_MULTI_LINED;
	string->action |= WIDGET_IS_MULTI_LINED;
	init_text->string = (text *)calloc(1,sizeof(text));

	if(init_text == NULL || string == NULL){
		fatal_error(ERROR_POINTER_NULL);
		return;
	}

	text_set(init_text->string,string->pos.x,string->pos.y,sx,sy,offsetx,offsety,maxchars,maxcharpl,fontid,red,blue,green,alpha,NULL);

	control->height = height;
	control->width = width;
	string->height = sy;
	string->width = sx;
	string->control = init_text;

	widget_add(parent,control);
	widget_add(control,string);
	create_widget_vertex_buffer(control);
}

void draw_mtextbox(widget *control)
{
	draw_widget(control);
}

void draw_mtextbox_text(widget *control)
{
	textbox *data = (textbox *)control->control;

	if(data->string->buf.isize > 0){
		text_draw(data->string);
	}
}

void handle_mtextbox_click(widget *control, int button, int pressed)
{
	textbox *data = (textbox *)control->shown.data[0]->control;

	if(data->cusorenabled == FALSE){
		data->cusorenabled = TRUE;
	}

	control->mousepress(control,button,pressed);
}

void handle_mtextbox_move(widget *control)
{
	widget_update_vector(control);
}

void handle_mtextbox_text_move(widget *control)
{
	textbox *data = (textbox *)control->control;
	if(data->string->buf.isize > 0){
		text_position_mupdate(data->string,control);
	}
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
				if(data->string->count > 0){
					data->string->count--;
				}
				data->string->data[data->string->count] = NULL;

				mtextbox_text_update(data->string, control->shown.data[0]);
			}
			break;

		default:
			if(data->string->data){
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