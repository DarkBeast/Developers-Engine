/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/

#ifndef DE_RENDER_H
#define DE_RENDER_H

#include "types.h"
#include "widget.h"
#include "controls.h"

typedef struct screen_size screen_size;

struct screen_size
{
	int width;
	int height;
};

screen_size get_screen_size(void);
void clear_screen(int red, int blue, int green, int alpha);
void draw(image *img, vector2i vecpos, vector2i imgpos,int width, int height);
void draw_push(void);
void draw_state_reset(void);
void draw_widget(widget *control);
void draw_widget_vprogressbar(widget *control);
void GLFWCALL handle_resize(int width,int height);
void init_image(image *img);
void init_screen(int swidth, int sheight, int mode);
void load_image(char *name, image *img);
void set_draw_view(int x, int y, int swidth, int sheight);
int get_screen_height(void);
int get_screen_width(void);
#endif