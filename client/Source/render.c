/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/
#include <GL/glew.h>
#include <glfw3.h>
#include <stdlib.h>
#include "error.h"
#include "globals.h"
#include "integer.h"
#include "render.h"
#include "bool.h"

screen_size the_screen;
GLFWwindow *the_window;
sbool window_open;
GLuint current_texid = 0;

GLuint get_current_texid(void)
{
	return current_texid;
}

void set_current_texid(GLuint id)
{
	current_texid = id;
}

GLFWwindow *get_the_window(void)
{
	return the_window;
}

screen_size get_screen_size(void)
{
	return the_screen;
}

sbool is_window_open(void)
{
	return window_open;
}

void set_if_window_should_close(void)
{
	window_open = FALSE;
}

int get_screen_height(void)
{
	return the_screen.height;
}

int get_screen_width(void)
{
	return the_screen.width;
}

void draw_push(void)
{
	glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
}

void draw_state_reset(void)
{
	//TODO: redo with graphical level options later on =].
	//glEnable(GL_COLOR_MATERIAL);//sets a vertex color, will slow down system some.
	glDisable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_ALPHA_TEST);// transparency
	glEnable(GL_TEXTURE_2D);
	//glShadeModel(GL_SMOOTH);
	glEnable(GL_SMOOTH);
	glEnable(GL_BLEND); //Enable alpha blending for better image quality, turn off for better performance.
	glAlphaFunc ( GL_GREATER, (GLclampf)0.2);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_FASTEST);           // Fastest
	glHint(GL_POINT_SMOOTH_HINT, GL_FASTEST); //Fastest
	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); //nicest
	//glHint(GL_POINT_SMOOTH_HINT,GL_NICEST); //nicest
	//TODO: add in option handler to switch looks around.
	glMatrixMode(GL_MODELVIEW);
}

void set_draw_view(int x, int y, int swidth, int sheight)
{
	//Tell OpenGL how to convert from coordinates to pixel values
	glViewport( x, y, swidth, sheight);

	glMatrixMode( GL_PROJECTION); //Switch to setting the camera perspective
	//Set the camera perspective
	glLoadIdentity(); //reset the camera
	glOrtho (0, swidth, sheight, 0, 0, 1);

	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
	glTranslatef(0.375, 0.375, 0);
}

//sets the screen, GLFW , and the Screen Title
void init_screen(int swidth, int sheight, GLFWmonitor *monitor, GLFWwindow *shared)
{
	GLenum err = 0;

	if(glfwInit() == FALSE)
		error_handler(DE_ERROR_GLFWINIT_ERROR);

	// Finally we can Open an OpenGL window
	glfwWindowHint(GLFW_DECORATED, GL_FALSE);
	the_window = glfwCreateWindow( swidth, sheight, TITLE, monitor, shared);

	if(the_window == NULL)
		error_handler(DE_ERROR_GLFWWIN_ERROR);

	glfwMakeContextCurrent(the_window);

	window_open = TRUE;
	err = glewInit();

	if(err!=GLEW_OK)
	{
		//Problem: glewInit failed, something is seriously wrong.
		error_handler(DE_ERROR_GLEWINIT_ERROR);
	}
	handle_resize(the_window,swidth,sheight);
}

void clear_screen(int red, int blue, int green, int alpha)
{
	glClearColor(red / 255.f, green / 255.f, blue / 255.f, alpha / 255.f);
	glClear(GL_COLOR_BUFFER_BIT);
}

//Called when the window is resized
void handle_resize(GLFWwindow * screen,int width,int height)
{
	//Tell OpenGL how to convert from coordinates to pixel values
	//glfwGetFramebufferSize(screen, &width, &height);
	glViewport( 0, 0, width, height);

	glMatrixMode( GL_PROJECTION); //Switch to setting the camera perspective
	//Set the camera perspective
	glLoadIdentity(); //reset the camera
	glOrtho (0, width, height, 0, 0, 1);

	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
	glTranslatef(0.375, 0.375, 0);
	//set the new screen size from the resize
	the_screen.height = height;
	the_screen.width = width;
}

void init_image(image* img)
{
	img->bpp = 0;
	img->format = GL_RGBA;
	img->height = 0;
	img ->width = 0;
	img ->texid = 0;
	img->pixels = NULL;
}

void load_image(char *path, image *img)
{
	init_image(img);
	load_png( path, img);

	glGenTextures( 1, &img->texid);
	glBindTexture( GL_TEXTURE_2D, img->texid);
	glPixelStoref(GL_UNPACK_ALIGNMENT, (GLfloat)1);

	glTexImage2D( GL_TEXTURE_2D, 0, img->format,
		img->width, img->height, 0, img->format,
		GL_UNSIGNED_BYTE, (void*) img->pixels);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	free(img->pixels);
	img->pixels = NULL;
}

void draw_primitive(image *img, vector2i vecpos, vector2i imgpos,int width, int height)
{
	float x2,x1;
	float y2,y1;

	x1 = (float)imgpos.x / img->width;
	x2 = (float)(imgpos.x + width) / img->width;
	y1 = (float)imgpos.y / img->height;
	y2 = (float)(imgpos.y +height) / img->height;

	glBindTexture(GL_TEXTURE_2D, img ->texid);//bind
	glColor4f(1, 1, 1, 1);//set allows colors amounts

	glBegin (GL_QUADS); //draw SQ

	glTexCoord2f (x1, y2);	glVertex2f (vecpos.x, vecpos.y);
	glTexCoord2f (x2, y2);	glVertex2f (vecpos.x + width, vecpos.y);
	glTexCoord2f (x2, y1);	glVertex2f (vecpos.x + width, vecpos.y + height);
	glTexCoord2f (x1, y1);	glVertex2f (vecpos.x, vecpos.y + height);

	glEnd ();//stop drawing to the new frame buffer.

	glBindTexture(GL_TEXTURE_2D, 0);
}

void create_widget_vertex_buffer(widget *control)
{
	GLuint index[4] = {0,1,2,3};

	create_widget_vertex(control);

	glGenBuffers(1,&control->buf.buffer);
	glGenBuffers(1,&control->buf.index);

	glBindBuffer(GL_ARRAY_BUFFER,control->buf.buffer);
	glBufferData(GL_ARRAY_BUFFER,sizeof(control->buf.data),control->buf.data,GL_STREAM_DRAW);//fill up the array with vertex and color-data

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,control->buf.index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,4 * sizeof(GLuint),index,GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void create_sprite_vertex_buffer(sprite *s)
{
	GLuint index[4] = {0,1,2,3};
	float x2,x1;
	float y2,y1;

	x1 = s->imgpos.x  / s->img.width;
	x2 = (s->imgpos.x + s->width) / s->img.width;
	y1 = s->imgpos.y / s->img.height;
	y2 = (s->imgpos.y + s->height) / s->img.height;

	/*index 0*/
	s->buffer.data[0].u = x1; s->buffer.data[0].v = y2;
	s->buffer.data[0].x = s->pos.x; s->buffer.data[0].y = s->pos.y;
	s->buffer.data[0].r = 1.0; s->buffer.data[0].b = 1.0; s->buffer.data[0].g = 1.0; s->buffer.data[0].a = 1.0;

	/*index 1*/
	s->buffer.data[1].u = x2; s->buffer.data[1].v = y2;
	s->buffer.data[1].x = s->pos.x + s->sizex; s->buffer.data[1].y = s->pos.y;
	s->buffer.data[1].r = 1.0; s->buffer.data[1].b = 1.0; s->buffer.data[1].g = 1.0; s->buffer.data[1].a = 1.0;

	/*index 2*/
	s->buffer.data[2].u = x2; s->buffer.data[2].v = y1;
	s->buffer.data[2].x = s->pos.x + s->sizex; s->buffer.data[2].y = s->pos.y + s->sizey;
	s->buffer.data[2].r = 1.0; s->buffer.data[2].b = 1.0; s->buffer.data[2].g = 1.0; s->buffer.data[2].a = 1.0;

	/*index 3*/
	s->buffer.data[3].u = x1; s->buffer.data[3].v = y1;
	s->buffer.data[3].x = s->pos.x; s->buffer.data[3].y = s->pos.y + s->sizey;
	s->buffer.data[3].r = 1.0; s->buffer.data[3].b = 1.0; s->buffer.data[3].g = 1.0; s->buffer.data[3].a = 1.0;

	glGenBuffers(1,&s->buffer.buffer);
	glGenBuffers(1,&s->buffer.index);

	glBindBuffer(GL_ARRAY_BUFFER,s->buffer.buffer);
	glBufferData(GL_ARRAY_BUFFER,sizeof(s->buffer.data),s->buffer.data,GL_STREAM_DRAW);//fill up the array with vertex and color-data

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,s->buffer.index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,4 * sizeof(GLuint),index,GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void create_tile_vertex_buffer(tile_layout_data *s, uint32 x, uint32 y, uint32 tx, uint32 ty)
{
	GLuint index[4] = {0,1,2,3};
	float x2,x1;
	float y2,y1;
	vertex_array arr;

	x1 = tx  / get_map_image(s->imageid)->width;
	x2 = (tx + TILE_SIZE) / get_map_image(s->imageid)->width;
	y1 = ty / get_map_image(s->imageid)->height;
	y2 = (ty + TILE_SIZE) / get_map_image(s->imageid)->height;

	/*index 0*/
	arr.vertex[0].u = x1; arr.vertex[0].v = y2;
	arr.vertex[0].x = x; arr.vertex[0].y = y;
	arr.vertex[0].r = 1.0; arr.vertex[0].b = 1.0; arr.vertex[0].g = 1.0; arr.vertex[0].a = 1.0;

	/*index 1*/
	arr.vertex[1].u = x2; arr.vertex[1].v = y2;
	arr.vertex[1].x = x + TILE_SIZE; arr.vertex[1].y = y;
	arr.vertex[1].r = 1.0; arr.vertex[1].b = 1.0; arr.vertex[1].g = 1.0; arr.vertex[1].a = 1.0;

	/*index 2*/
	arr.vertex[2].u = x2; arr.vertex[2].v = y1;
	arr.vertex[2].x = x + TILE_SIZE; arr.vertex[2].y = y + TILE_SIZE;
	arr.vertex[2].r = 1.0; arr.vertex[2].b = 1.0; arr.vertex[2].g = 1.0; arr.vertex[2].a = 1.0;

	/*index 3*/
	arr.vertex[3].u = x1; arr.vertex[3].v = y1;
	arr.vertex[3].x = x; arr.vertex[3].y = y + TILE_SIZE;
	arr.vertex[3].r = 1.0; arr.vertex[3].b = 1.0; arr.vertex[3].g = 1.0; arr.vertex[3].a = 1.0;

	glGenBuffers(1,&s->buffer);
	glGenBuffers(1,&s->index);

	glBindBuffer(GL_ARRAY_BUFFER,s->buffer);
	glBufferData(GL_ARRAY_BUFFER,sizeof(arr.vertex),arr.vertex,GL_STREAM_DRAW);//fill up the array with vertex and color-data

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,s->index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,4 * sizeof(GLuint),index,GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void update_tile_array(tile_layout_data *s, uint32 x, uint32 y, uint32 tx, uint32 ty)
{
	GLuint index[4] = {0,1,2,3};
	float x2,x1;
	float y2,y1;
	vertex_array arr;

	x1 = tx  / get_map_image(s->imageid)->width;
	x2 = (tx + TILE_SIZE) / get_map_image(s->imageid)->width;
	y1 = ty / get_map_image(s->imageid)->height;
	y2 = (ty + TILE_SIZE) / get_map_image(s->imageid)->height;

	/*index 0*/
	arr.vertex[0].u = x1; arr.vertex[0].v = y2;
	arr.vertex[0].x = x; arr.vertex[0].y = y;
	arr.vertex[0].r = 1.0; arr.vertex[0].b = 1.0; arr.vertex[0].g = 1.0; arr.vertex[0].a = 1.0;

	/*index 1*/
	arr.vertex[1].u = x2; arr.vertex[1].v = y2;
	arr.vertex[1].x = x + TILE_SIZE; arr.vertex[1].y = y;
	arr.vertex[1].r = 1.0; arr.vertex[1].b = 1.0; arr.vertex[1].g = 1.0; arr.vertex[1].a = 1.0;

	/*index 2*/
	arr.vertex[2].u = x2; arr.vertex[2].v = y1;
	arr.vertex[2].x = x + TILE_SIZE; arr.vertex[2].y = y + TILE_SIZE;
	arr.vertex[2].r = 1.0; arr.vertex[2].b = 1.0; arr.vertex[2].g = 1.0; arr.vertex[2].a = 1.0;

	/*index 3*/
	arr.vertex[3].u = x1; arr.vertex[3].v = y1;
	arr.vertex[3].x = x; arr.vertex[3].y = y + TILE_SIZE;
	arr.vertex[3].r = 1.0; arr.vertex[3].b = 1.0; arr.vertex[3].g = 1.0; arr.vertex[3].a = 1.0;

	glBindBuffer(GL_ARRAY_BUFFER,s->buffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * sizeof(struct vertex_t), &arr.vertex[0]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void draw_tile(tile_layout_data *s)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glBindTexture(GL_TEXTURE_2D, get_map_image(s->imageid)->texid);

	glBindBuffer(GL_ARRAY_BUFFER, s->buffer);
	glVertexPointer(2, GL_FLOAT, sizeof(struct vertex_t), 0);
	glTexCoordPointer(2, GL_FLOAT, sizeof(struct vertex_t), (GLvoid *)offsetof(struct vertex_t, u));
	glColorPointer(4, GL_FLOAT, sizeof(struct vertex_t), (GLvoid *)offsetof(struct vertex_t, r));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,  s->index);
	glIndexPointer(GL_UNSIGNED_INT,sizeof(GLuint),0);

	glDrawElements(GL_QUADS,4,GL_UNSIGNED_INT,0); //GL_TRIANGLE_STRIP

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void sprite_update_vector(sprite *s)
{
	/*index 0*/
	s->buffer.data[0].x = s->pos.x; s->buffer.data[0].y = s->pos.y;

	/*index 1*/
	s->buffer.data[1].x = s->pos.x + s->sizex; s->buffer.data[1].y = s->pos.y;

	/*index 2*/
	s->buffer.data[2].x = s->pos.x + s->sizex; s->buffer.data[2].y = s->pos.y + s->sizey;

	/*index 3*/
	s->buffer.data[3].x = s->pos.x; s->buffer.data[3].y = s->pos.y + s->sizey;

	glBindBuffer(GL_ARRAY_BUFFER,s->buffer.buffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * sizeof(struct vertex_t), &s->buffer.data[0]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void sprite_update_texture_vector(sprite *s)
{
	float x2,x1;
	float y2,y1;

	x1 = s->imgpos.x  / s->img.width;
	x2 = (s->imgpos.x + s->width) / s->img.width;
	y1 = s->imgpos.y / s->img.height;
	y2 = (s->imgpos.y + s->height) / s->img.height;

	s->buffer.data[0].u = x1; s->buffer.data[0].v = y2;

	/*index 1*/
	s->buffer.data[1].u = x2; s->buffer.data[1].v = y2;

	/*index 2*/
	s->buffer.data[2].u = x2; s->buffer.data[2].v = y1;

	/*index 3*/
	s->buffer.data[3].u = x1; s->buffer.data[3].v = y1;

	glBindBuffer(GL_ARRAY_BUFFER,s->buffer.buffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * sizeof(struct vertex_t), &s->buffer.data[0]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void widget_update_vector(widget *control)
{
	control->actualpos.x = control->pos.x + control->parent->actualpos.x;
	control->actualpos.y = control->pos.y + control->parent->actualpos.y;

	/*index 0*/
	control->buf.data[0].x = control->actualpos.x; control->buf.data[0].y = control->actualpos.y;

	/*index 1*/
	control->buf.data[1].x = control->actualpos.x + control->sizex; control->buf.data[1].y = control->actualpos.y;

	/*index 2*/
	control->buf.data[2].x = control->actualpos.x + control->sizex; control->buf.data[2].y = control->actualpos.y + control->sizey;

	/*index 3*/
	control->buf.data[3].x = control->actualpos.x; control->buf.data[3].y = control->actualpos.y + control->sizey;

	glBindBuffer(GL_ARRAY_BUFFER,control->buf.buffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * sizeof(struct vertex_t), &control->buf.data[0]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void widget_update_progressbars_vector(widget *control)
{
	float value;

	if(control->type == CONTROL_HPROGRESSBAR){
		value = (control->parent->value / 100.f);
		control->actualpos.x = control->pos.x + control->parent->actualpos.x;
		control->actualpos.y = control->pos.y + control->parent->actualpos.y;
		/*0*/control->buf.data[0].x = control->actualpos.x + .1; control->buf.data[0].y = control->actualpos.y;
		/*1*/control->buf.data[1].x = control->actualpos.x + (control->sizex * value); control->buf.data[1].y = control->actualpos.y;
		/*2*/control->buf.data[2].x = control->actualpos.x + (control->sizex * value); control->buf.data[2].y = control->actualpos.y + control->sizey;
		/*3*/control->buf.data[3].x = control->actualpos.x + .1; control->buf.data[3].y = control->actualpos.y + control->sizey;
	}
	else{
		value = (control->parent->value / -100.f);

		control->actualpos.x = control->pos.x + control->parent->actualpos.x;
		control->actualpos.y = control->pos.y + control->height + control->parent->actualpos.y;
		/*0*/control->buf.data[0].x = control->actualpos.x; control->buf.data[0].y = control->actualpos.y;
		/*1*/control->buf.data[1].x = control->actualpos.x + control->sizex ; control->buf.data[1].y = control->actualpos.y ;
		/*2*/control->buf.data[2].x = control->actualpos.x + control->sizex ; control->buf.data[2].y = control->actualpos.y + (control->sizey * value);
		/*3*/control->buf.data[3].x = control->actualpos.x; control->buf.data[3].y = control->actualpos.y + (control->sizey * value);
	}
	glBindBuffer(GL_ARRAY_BUFFER,control->buf.buffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * sizeof(struct vertex_t), &control->buf.data[0]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void widget_update_texture_vector(widget *control)
{
	float x2,x1;
	float y2,y1;

	x1 = control->imgpos.x  / control->img->width;
	x2 = (control->imgpos.x + control->width) / control->img->width;
	y1 = control->imgpos.y / control->img->height;
	y2 = (control->imgpos.y + control->height) /control->img->height;

	control->buf.data[0].u = x1; control->buf.data[0].v = y2;

	/*index 1*/
	control->buf.data[1].u = x2; control->buf.data[1].v = y2;

	/*index 2*/
	control->buf.data[2].u = x2; control->buf.data[2].v = y1;

	/*index 3*/
	control->buf.data[3].u = x1; control->buf.data[3].v = y1;

	glBindBuffer(GL_ARRAY_BUFFER,control->buf.buffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * sizeof(struct vertex_t), &control->buf.data[0]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void create_widget_vertex(widget *control)
{
	float x2,x1;
	float y2,y1;

	control->actualpos.x = control->pos.x + control->parent->actualpos.x;
	control->actualpos.y = control->pos.y + control->parent->actualpos.y;

	x1 = control->imgpos.x  / control->img->width;
	x2 = (control->imgpos.x + control->width) / control->img->width;
	y1 = control->imgpos.y / control->img->height;
	y2 = (control->imgpos.y + control->height) / control->img->height;

	/*index 0*/
	control->buf.data[0].u = x1; control->buf.data[0].v = y2;
	control->buf.data[0].x = control->actualpos.x; control->buf.data[0].y = control->actualpos.y;
	control->buf.data[0].r = 1.0; control->buf.data[0].b = 1.0; control->buf.data[0].g = 1.0; control->buf.data[0].a = 1.0;

	/*index 1*/
	control->buf.data[1].u = x2; control->buf.data[1].v = y2;
	control->buf.data[1].x = control->actualpos.x + control->sizex; control->buf.data[1].y = control->actualpos.y;
	control->buf.data[1].r = 1.0; control->buf.data[1].b = 1.0; control->buf.data[1].g = 1.0; control->buf.data[1].a = 1.0;

	/*index 2*/
	control->buf.data[2].u = x2; control->buf.data[2].v = y1;
	control->buf.data[2].x = control->actualpos.x + control->sizex; control->buf.data[2].y = control->actualpos.y + control->sizey;
	control->buf.data[2].r = 1.0; control->buf.data[2].b = 1.0; control->buf.data[2].g = 1.0; control->buf.data[2].a = 1.0;

	/*index 3*/
	control->buf.data[3].u = x1; control->buf.data[3].v = y1;
	control->buf.data[3].x = control->actualpos.x; control->buf.data[3].y = control->actualpos.y + control->sizey;
	control->buf.data[3].r = 1.0; control->buf.data[3].b = 1.0; control->buf.data[3].g = 1.0; control->buf.data[3].a = 1.0;
}

void update_widget_vertex_buffer(widget *control)
{
	create_widget_vertex(control);

	glBindBuffer(GL_ARRAY_BUFFER,control->buf.buffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(control->buf.data), &control->buf.data[0]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void draw_widget(widget *control)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glBindTexture(GL_TEXTURE_2D, control->img->texid);

	glBindBuffer(GL_ARRAY_BUFFER, control->buf.buffer);
	glVertexPointer(2, GL_FLOAT, sizeof(struct vertex_t), 0);
	glTexCoordPointer(2, GL_FLOAT, sizeof(struct vertex_t), (GLvoid *)offsetof(struct vertex_t, u));
	glColorPointer(4, GL_FLOAT, sizeof(struct vertex_t), (GLvoid *)offsetof(struct vertex_t, r));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, control->buf.index);
	glIndexPointer(GL_UNSIGNED_INT,sizeof(GLuint),0);

	glDrawElements(GL_QUADS,4,GL_UNSIGNED_INT,0); //GL_TRIANGLE_STRIP

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void draw_sprite(sprite *s)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glBindTexture(GL_TEXTURE_2D, s->img.texid);

	glBindBuffer(GL_ARRAY_BUFFER, s->buffer.buffer);
	glVertexPointer(2, GL_FLOAT, sizeof(struct vertex_t), 0);
	glTexCoordPointer(2, GL_FLOAT, sizeof(struct vertex_t), (GLvoid *)offsetof(struct vertex_t, u));
	glColorPointer(4, GL_FLOAT, sizeof(struct vertex_t), (GLvoid *)offsetof(struct vertex_t, r));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,  s->buffer.index);
	glIndexPointer(GL_UNSIGNED_INT,sizeof(GLuint),0);

	glDrawElements(GL_QUADS,4,GL_UNSIGNED_INT,0); //GL_TRIANGLE_STRIP

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void create_cursor_vertex(widget *control)
{
	GLuint index[2] = {0,1};
	textbox *text = (textbox *)control->control;

	text->cursorpos.x = control->pos.x + control->parent->actualpos.x + text->string->offsetx;
	text->cursorpos.y = control->pos.y + control->parent->actualpos.y + text->string->offsety;

	/*index 0*/
	text->cursorbuffer.data[0].x = text->cursorpos.x + text->string->offsetx; text->cursorbuffer.data[0].y = text->cursorpos.y + text->string->font.h;
	text->cursorbuffer.data[0].r = text->string->col.r; text->cursorbuffer.data[0].b = text->string->col.b; text->cursorbuffer.data[0].g = text->string->col.g; text->cursorbuffer.data[0].a = text->string->col.a;

	/*index 1*/
	text->cursorbuffer.data[1].x = text->cursorpos.x + text->string->offsetx; text->cursorbuffer.data[1].y = text->cursorpos.y + text->string->font.h - text->cursorheight + 2;
	text->cursorbuffer.data[1].r = text->string->col.r; text->cursorbuffer.data[1].b = text->string->col.b; text->cursorbuffer.data[1].g = text->string->col.g; text->cursorbuffer.data[1].a = text->string->col.a;

	glGenBuffers(1,&text->cursorbuffer.buffer);
	glGenBuffers(1,&text->cursorbuffer.index);

	glBindBuffer(GL_ARRAY_BUFFER,text->cursorbuffer.buffer);
	glBufferData(GL_ARRAY_BUFFER,sizeof(text->cursorbuffer.data),text->cursorbuffer.data,GL_STREAM_DRAW);//fill up the array with vertex and color-data

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,text->cursorbuffer.index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,2 * sizeof(GLuint),index,GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void draw_cursor(widget *control)
{
	textbox *text = (textbox *)control->control;

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glLineWidth(text->cursorwidth);

	glBindBuffer(GL_ARRAY_BUFFER, text->cursorbuffer.buffer);
	glVertexPointer(2, GL_FLOAT, sizeof(struct line_vertex_t), 0);
	glColorPointer(4, GL_FLOAT, sizeof(struct line_vertex_t), (GLvoid *)offsetof(struct line_vertex_t, r));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, text->cursorbuffer.index);
	glIndexPointer(GL_UNSIGNED_INT,sizeof(GLuint),0);

	glDrawElements(GL_LINES,2,GL_UNSIGNED_INT,0); //GL_TRIANGLE_STRIP

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}