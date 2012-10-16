
#ifndef _Render_H
#define _Render_H

#include "Types.h"

Screen screen;

void DrawPush();
void DrawStateReset();
void SetDrawView(int x, int y, int swidth, int sheight);
int Draw(Image* img, Vector2i vecpos, Vector2f imgpos,int width, int height);
int LoadImage(char *name, Image* img, int type);
void reloadimage(Image* img);
void initimage(Image* img);
void GLFWCALL handleResize(int width,int height);
void ClearScreen(int red, int blue, int green, int alpha);
void InitScreen(int swidth, int sheight, int mode);

#endif