/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/

#include "render.h"
#include "program_path.h"
#include "controls.h"
#include <stdlib.h>
#include "widget.h"
//Buttons
void createbutton(widget *data, uint16 x, uint16 y, uint16 height, uint16 width, char *image)
{
	button *box;

	initwidget(data);
	data->pos.x = x;
	data->pos.y = y;
	data->height = height;
	data->width = width;
	data->imgpos.x = 0;
	data->imgpos.y = 0;
	data->type = BUTTON;
	data->draw = &drawbuttons;

	box = (button *)calloc(1,sizeof(button));
	box->click = 0;

	data ->control = box;
	loadimage(getpath(image), &data->img);
}

void drawbuttons(void *wgt)
{
	widget *data = (widget *)wgt;

	if (ismouseover(data) == TRUE)
	{
		if (data->clicked != FALSE)
		{
			data->imgpos.x = data->width;
			data->imgpos.y = 0;
		}
		else
		{
			data->imgpos.y = 0;
			data->imgpos.x = 2 * data->width;
		}
	}
	else
	{
		data->imgpos.y = 0;
		data->imgpos.x = 0;
	}

	drawwidget(data);
}

//labels
void createlabel(widget *data, uint16 x, uint16 y, uint8 size, uint8 red, uint8 blue, uint8 green, uint8 alpha, int8 canusemouse, char *labeltext)
{
	label *inittext;
	vector2ui hw;

	initwidget(data);
	data->pos.x = x;
	data->pos.y = y;
	data->imgpos.x = 0;
	data->imgpos.y = 0;
	data->type = LABEL;
	data->draw = &drawlabel;
	inittext = (label *)calloc(1,sizeof(label));
	inittext->string = (text *)calloc(1,sizeof(text));
	settext(inittext->string,x,y,size,red,blue,green,alpha,labeltext);
	inittext->click = 0;
	inittext->canusemouse = canusemouse;
	hw = getmaxstringhw(labeltext,size);
	data->height = hw.y;
	data->width = hw.x;
	data->control = inittext;
}

void drawlabel(void *wgt)
{
	widget *control = (widget *)wgt;
	label *data = (label *)control->control;
	drawtext(data->string, (widget *)control->parent);
}