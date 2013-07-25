/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/
#include <GL/glew.h>
#include <glfw.h>
#include <stdlib.h>
#include "error.h"
#include "globals.h"
#include "image.h"
#include "integer.h"
#include "render.h"
#include "bool.h"

screen_size the_screen;

screen_size get_screen_size(void)
{
	return the_screen;
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
void init_screen(int swidth, int sheight, int mode)
{
	GLenum err = 0;

	if( !glfwInit())
		render_error(ERROR_GLFWINIT_ERROR);

	// Finally we can Open an OpenGL window
	if(!glfwOpenWindow ( swidth, sheight, 0,0,0,0,0,0, mode))
		render_error(ERROR_GLFWWIN_ERROR);

	glfwSetWindowTitle( TITLE);//Sets the Windows Name
	the_screen.height = sheight;
	the_screen.width = swidth;

	err = glewInit();

	if(err!=GLEW_OK)
	{
		//Problem: glewInit failed, something is seriously wrong.
		render_error(ERROR_GLEWINIT_ERROR);
	}
}

void clear_screen(int red, int blue, int green, int alpha)
{
	glClearColor(red / 255.f, green / 255.f, blue / 255.f, alpha / 255.f);
	glClear(GL_COLOR_BUFFER_BIT);
}

//Called when the window is resized
void GLFWCALL handle_resize(int width,int height)
{
	//Tell OpenGL how to convert from coordinates to pixel values
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

	glBindTexture( GL_TEXTURE_2D, NULL);
	free(img->pixels);
}

void draw(image *img, vector2i vecpos, vector2i imgpos,int width, int height)
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

void draw_widget(widget *control)
{
	float x2,x1;
	float y2,y1;

	control->actualpos.x = control->pos.x + control->parent->actualpos.x;
	control->actualpos.y = control->pos.y + control->parent->actualpos.y;

	x1 = (float)control->imgpos.x  / control->img->width;
	x2 = (float)(control->imgpos.x + control->width) / control->img->width;
	y1 = (float)control->imgpos.y / control->img->height;
	y2 = (float)(control->imgpos.y + control->height) /control->img->height;

	glBindTexture(GL_TEXTURE_2D, control->img->texid);
	glColor4f(1, 1, 1, 1);

	glBegin (GL_QUADS);

	glTexCoord2f (x1, y2);	glVertex2i (control->actualpos.x, control->actualpos.y);
	glTexCoord2f (x2, y2);	glVertex2i (control->actualpos.x + control->sizex, control->actualpos.y);
	glTexCoord2f (x2, y1);	glVertex2i (control->actualpos.x + control->sizex, control->actualpos.y + control->sizey);
	glTexCoord2f (x1, y1);	glVertex2i (control->actualpos.x, control->actualpos.y + control->sizey);

	glEnd ();
}

void draw_widget_vprogressbar(widget *control)
{
	float x2,value;
	float y2,x;

	control->actualpos.x = control->pos.x + control->parent->actualpos.x;
	control->actualpos.y = control->pos.y + control->parent->actualpos.y + control->sizey;

	value = (float)(control->value / -100.f);
	x2 = (float)(control->width / control->img->width);
	y2 = (float)(control->height / control->img->height) * value;

	if(value == -1){
		x = 0;
	}else{
		x = -1;
	}
	glBindTexture(GL_TEXTURE_2D, control->img->texid);
	glColor4f(1, 1, 1, 1);

	glBegin (GL_QUADS);

	glTexCoord2f (.5, y2);	glVertex2i (control->actualpos.x, control->actualpos.y + x);
	glTexCoord2f (x2, y2);	glVertex2i (control->actualpos.x + control->sizex, control->actualpos.y + x);
	glTexCoord2f (x2, 0);	glVertex2i (control->actualpos.x + control->sizex, control->actualpos.y + (control->sizey * value));
	glTexCoord2f (.5, 0);	glVertex2i (control->actualpos.x, control->actualpos.y + (control->sizey * value));

	glEnd ();
}

void draw_widget_hprogressbar(widget *control)
{
	float x2,value;
	float y2;

	control->actualpos.x = control->pos.x + control->parent->pos.x;
	control->actualpos.y = control->pos.y + control->parent->pos.y;

	value = (float)(control->value / 100.f);
	x2 = (float)(control->width / control->img->width) * value;
	y2 = (float)(control->height / control->img->height);

	glBindTexture(GL_TEXTURE_2D, control->img->texid);
	glColor4f(1, 1, 1, 1);

	glBegin (GL_QUADS);

	glTexCoord2f (0, y2);	glVertex2i (control->actualpos.x, control->actualpos.y);
	glTexCoord2f (x2, y2);	glVertex2i (control->actualpos.x + (control->sizex * value), control->actualpos.y);
	glTexCoord2f (x2, .5);	glVertex2i (control->actualpos.x + (control->sizex * value), control->actualpos.y + control->sizey);
	glTexCoord2f (0, .5);	glVertex2i (control->actualpos.x, control->actualpos.y + control->sizey);

	glEnd ();
}

void create_widget_vertex_buffer(widget *control)
{
	GLuint index[4] = {0,1,2,3};

	create_widget_vertex(control);

	glGenBuffers(1,&control->oglbuf.vertex_buffer);
	glGenBuffers(1,&control->oglbuf.index_buffer);

	glBindBuffer(GL_ARRAY_BUFFER,control->oglbuf.vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER,sizeof(control->oglbuf.buffer),control->oglbuf.buffer,GL_STATIC_DRAW);//fill up the array with vertex and color-data

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,control->oglbuf.index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,4 * sizeof(GLuint),index,GL_STATIC_DRAW);//this one with indices
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
	control->oglbuf.buffer[0].u = x1; control->oglbuf.buffer[0].v = y2;
	control->oglbuf.buffer[0].x = control->actualpos.x; control->oglbuf.buffer[0].y = control->actualpos.y;
	control->oglbuf.buffer[0].r = 1.0; control->oglbuf.buffer[0].b = 1.0; control->oglbuf.buffer[0].g = 1.0; control->oglbuf.buffer[0].a = 1.0;

	/*index 1*/
	control->oglbuf.buffer[1].u = x2; control->oglbuf.buffer[1].v = y2;
	control->oglbuf.buffer[1].x = control->actualpos.x + control->sizex; control->oglbuf.buffer[1].y = control->actualpos.y;
	control->oglbuf.buffer[1].r = 1.0; control->oglbuf.buffer[1].b = 1.0; control->oglbuf.buffer[1].g = 1.0; control->oglbuf.buffer[1].a = 1.0;

	/*index 2*/
	control->oglbuf.buffer[2].u = x2; control->oglbuf.buffer[2].v = y1;
	control->oglbuf.buffer[2].x = control->actualpos.x + control->sizex; control->oglbuf.buffer[2].y = control->actualpos.y + control->sizey;
	control->oglbuf.buffer[2].r = 1.0; control->oglbuf.buffer[2].b = 1.0; control->oglbuf.buffer[2].g = 1.0; control->oglbuf.buffer[2].a = 1.0;

	/*index 3*/
	control->oglbuf.buffer[3].u = x1; control->oglbuf.buffer[3].v = y1;
	control->oglbuf.buffer[3].x = control->actualpos.x; control->oglbuf.buffer[3].y = control->actualpos.y + control->sizey;
	control->oglbuf.buffer[3].r = 1.0; control->oglbuf.buffer[3].b = 1.0; control->oglbuf.buffer[3].g = 1.0; control->oglbuf.buffer[3].a = 1.0;
}

void draw_widget_test(widget *control)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glBindTexture(GL_TEXTURE_2D, control->img->texid);

	glBindBuffer(GL_ARRAY_BUFFER, control->oglbuf.vertex_buffer);
	glVertexPointer(2, GL_FLOAT, sizeof(struct vertex_t), 0);
	glTexCoordPointer(2, GL_FLOAT, sizeof(struct vertex_t), (GLvoid *)offsetof(struct vertex_t, u));
	glColorPointer(4, GL_FLOAT, sizeof(struct vertex_t), (GLvoid *)offsetof(struct vertex_t, r));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, control->oglbuf.index_buffer);
	glIndexPointer(GL_UNSIGNED_INT,sizeof(GLuint),0);

	glDrawElements(GL_QUADS,4,GL_UNSIGNED_INT,0); //GL_TRIANGLE_STRIP

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}