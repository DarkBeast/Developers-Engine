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

GLFWwindow *get_the_window(void);
screen_size get_screen_size(void);
sbool is_window_open(void);
void set_if_window_should_close(void);
void clear_screen(int red, int blue, int green, int alpha);
void draw_primitive(image *img, vector2i vecpos, vector2i imgpos,int width, int height);
void draw_push(void);
void draw_state_reset(void);
void handle_resize(GLFWwindow * screen,int width,int height);
void init_image(image *img);
void init_screen(int swidth, int sheight, GLFWmonitor *monitor, GLFWwindow *shared);
void load_image(char *name, image *img);
void set_draw_view(int x, int y, int swidth, int sheight);
int get_screen_height(void);
int get_screen_width(void);

void draw_widget(widget *control);
void create_widget_vertex_buffer(widget *control);
void create_widget_vertex(widget *control);
void widget_update_vector(widget *control);
void widget_update_texture_vector(widget *control);
void widget_update_progressbars_vector(widget *control);
void update_widget_vertex_buffer(widget *control);

void create_cursor_vertex(widget *control);
void draw_cursor(widget *control);

void draw_test(sbuffer *buf, image* image1);
#endif