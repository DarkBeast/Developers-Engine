/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/
#include "widget.h"
#include "render.h"
#include "program_path.h"
#include "globals.h"

void createbutton(widget *button, uint32 x, uint32 y, uint32 height, uint32 width, char* image)
{
	initwidget(button);
	button->pos.x = x;
	button->pos.y = y;
	button->height = height;
	button->width = width;
	button->imgpos.x = 0;
	button->imgpos.y = 0;
	button->type = BUTTON;
	button->draw = &drawbuttons;

	loadimage(getpath(image), &button->img);
}

void drawbuttons(void * wgt)
{
	widget *data;
	data = (widget *)wgt;

	if (data->mouseover && data->focused)
	{
		if (data->clicked != FALSE)
		{
			data->imgpos.y = data->width;
			drawwidget(data);
		}
		else
		{
			data->imgpos.y = 2 * data->width;
			drawwidget(data);
		}
	}
	else
	{
		data->imgpos.y = 0;
		drawwidget(data);
	}
}