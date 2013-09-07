/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/
#include <GL/glew.h>
#include <glfw3.h>
#include <stdlib.h>
#include "error.h"
#include "globals.h"
#include "image.h"
#include "integer.h"
#include "render.h"
#include "bool.h"

screen_size the_screen;
GLFWwindow *the_window;
sbool window_open;

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
	//glEnable(GL_SMOOTH);
	glEnable(GL_BLEND); //Enable alpha blending for better image quality, turn off for better performance.
	glAlphaFunc ( GL_GREATER, (GLclampf)0.2);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
		render_error(ERROR_GLFWINIT_ERROR);

	// Finally we can Open an OpenGL window

	the_window = glfwCreateWindow( swidth, sheight, TITLE, monitor, shared);

	if(the_window == NULL)
		render_error(ERROR_GLFWWIN_ERROR);

	glfwMakeContextCurrent(the_window);

	window_open = TRUE;
	err = glewInit();

	if(err!=GLEW_OK)
	{
		//Problem: glewInit failed, something is seriously wrong.
		render_error(ERROR_GLEWINIT_ERROR);
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

	if(img->pixels != NULL){
		free(img->pixels);
		img->pixels = NULL;
	}
}

void load_image(char *name, image *img)
{
	init_image(img);
	load_png( name, img);

	glGenTextures( 1, &img->texid);
	glBindTexture( GL_TEXTURE_2D, img ->texid);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexImage2D( GL_TEXTURE_2D, 0, img ->format,
		img ->width, img ->height, 0, img ->format,
		GL_UNSIGNED_BYTE, (void*) img->pixels);

	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	free(img->pixels);
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

	glTexCoord2f (x1, y2);	glVertex2i (vecpos.x, vecpos.y);
	glTexCoord2f (x2, y2);	glVertex2i (vecpos.x + width, vecpos.y);
	glTexCoord2f (x2, y1);	glVertex2i (vecpos.x + width, vecpos.y +height);
	glTexCoord2f (x1, y1);	glVertex2i (vecpos.x, vecpos.y +height);

	glEnd ();//stop drawing to the new frame buffer.
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

void widget_update_vector(widget *control)
{
	control->actualpos.x = control->pos.x + control->parent->actualpos.x;
	control->actualpos.y = control->pos.y + control->parent->actualpos.y;

	/*index 0*/
	control->buf.data[0].x = (float)control->actualpos.x; control->buf.data[0].y = (float)control->actualpos.y;

	/*index 1*/
	control->buf.data[1].x = (float)control->actualpos.x + control->sizex; control->buf.data[1].y = (float)control->actualpos.y;

	/*index 2*/
	control->buf.data[2].x = (float)control->actualpos.x + control->sizex; control->buf.data[2].y = (float)control->actualpos.y + control->sizey;

	/*index 3*/
	control->buf.data[3].x = (float)control->actualpos.x; control->buf.data[3].y = (float)control->actualpos.y + control->sizey;

	glBindBuffer(GL_ARRAY_BUFFER,control->buf.buffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * sizeof(struct vertex_t), &control->buf.data[0]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void widget_update_progressbars_vector(widget *control)
{
	float value,x;

	control->actualpos.x = control->pos.x + control->parent->actualpos.x;
	control->actualpos.y = control->pos.y + control->parent->actualpos.y;

	if(control->type == CONTROL_HPROGRESSBAR){
		value = (float)(control->value / 100.f);
		/*0*/control->buf.data[0].x = (float)control->actualpos.x; control->buf.data[0].y = (float)control->actualpos.y;
		/*1*/control->buf.data[1].x = (float)control->actualpos.x + (control->sizex * value); control->buf.data[1].y = (float)control->actualpos.y;
		/*2*/control->buf.data[2].x = (float)control->actualpos.x + (control->sizex * value); control->buf.data[2].y = (float)control->actualpos.y + control->sizey;
		/*3*/control->buf.data[3].x = (float)control->actualpos.x; control->buf.data[3].y = (float)control->actualpos.y + control->sizey;
	}
	else{
		value = (float)(control->value / -100.f);
		if(value == -1){
			x = 0;
		}else{
			x = -1;
		}
		/*0*/control->buf.data[0].x = (float)control->actualpos.x; control->buf.data[0].y = (float)control->actualpos.y + x;
		/*1*/control->buf.data[1].x = (float)control->actualpos.x + control->sizex ; control->buf.data[1].y = (float)control->actualpos.y + x;
		/*2*/control->buf.data[2].x = (float)control->actualpos.x + control->sizex ; control->buf.data[2].y = (float)control->actualpos.y + (control->sizey * value);
		/*3*/control->buf.data[3].x = (float)control->actualpos.x; control->buf.data[3].y = (float)control->actualpos.y + (control->sizey * value);
	}
	glBindBuffer(GL_ARRAY_BUFFER,control->buf.buffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * sizeof(struct vertex_t), &control->buf.data[0]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void widget_update_texture_vector(widget *control)
{
	float x2,x1;
	float y2,y1;

	x1 = (float)control->imgpos.x  / control->img->width;
	x2 = (float)(control->imgpos.x + control->width) / control->img->width;
	y1 = (float)control->imgpos.y / control->img->height;
	y2 = (float)(control->imgpos.y + control->height) /control->img->height;

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

	x1 = (float)control->imgpos.x  / control->img->width;
	x2 = (float)(control->imgpos.x + control->width) / control->img->width;
	y1 = (float)control->imgpos.y / control->img->height;
	y2 = (float)(control->imgpos.y + control->height) / control->img->height;

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
	glBufferData(GL_ARRAY_BUFFER,sizeof(control->buf.data),control->buf.data,GL_STREAM_DRAW);//fill up the array with vertex and color-data
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

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}