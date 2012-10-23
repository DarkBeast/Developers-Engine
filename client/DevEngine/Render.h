/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis    
******************************************************************************/

#ifndef _Render_H
#define _Render_H

#include "types.h"

typedef struct 
{
	int Width;
	int Height;
}screensize;

screensize screen;

void drawPush();
void drawstatereset();
void SetdrawView(int x, int y, int swidth, int sheight);
void draw(image* img, vector2i vecpos, vector2f imgpos,int width, int height);
void LoadImage(char *name, image* img);
void reloadimage(image* img);
void initimage(image* img);
void GLFWCALL handleresize(int width,int height);
void clearscreen(int red, int blue, int green, int alpha);
void initscreen(int swidth, int sheight, int mode);

#endif