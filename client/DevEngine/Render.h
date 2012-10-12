
#ifndef _Render_H
#define _Render_H


#include "Types.h"



Screen screen;

void DrawPush();

void DrawStateReset();

void SetDrawView(int swidth, int sheight);

void SetDrawViewToScreen(Screen s);

int Draw(Image* img, SCoord pos);

int LoadImage(char *name, Image* img, int type);

void GLFWCALL handleResize(int width,int height);

void GLFWCALL handleKeypress(int key,int press);

void ClearScreen(int red, int blue, int green, int alpha);

void InitScreen(int swidth, int sheight, int mode);

#endif