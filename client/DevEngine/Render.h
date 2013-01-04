/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/

#ifndef DE_RENDER_H
#define DE_RENDER_H

#include "types.h"
#include "widget.h"

typedef struct
{
	int width;
	int height;
}screensize;

screensize getscreensize(void);
void clearscreen(int red, int blue, int green, int alpha);
void draw(image* img, vector2i vecpos, vector2i imgpos,int width, int height);
void drawpush(void);
void drawstatereset(void);
void drawwidget(widget* widget);
void GLFWCALL handleresize(int width,int height);
void initimage(image* img);
void initscreen(int swidth, int sheight, int mode);
void loadimage(char *name, image* img);
void reloadimage(image* img);
void SetdrawView(int x, int y, int swidth, int sheight);
int getscreenheight(void);
int getscreenwidth(void);
#endif