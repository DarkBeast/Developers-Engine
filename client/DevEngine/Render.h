
#ifndef _Render_H
#define _Render_H

#include <glfw.h>
#include <stdio.h>
#include <stdlib.h>
#include "globals.h"

const Uint8 VertexCacheSize = 4;

typedef struct Screen
{
int Width;
int Height;

}Screen;

typedef struct Coords
{
int y;
int x;
int z;
}Coords;

typedef struct FCoords
{
float y;
float x;
float z;
}FCoords;

typedef struct Vertex
{
	Coords position;
	Coords TexPos;

}Vertex;

typedef struct StatesCache
    {
  
        CBool      glStatesSet;    ///< Are our internal GL states set yet?
        CBool      viewChanged;    ///< Has the current view changed since last draw?
        Uint64    lastTextureId;  ///< Cached texture
        CBool      useVertexCache; ///< Did we previously use the vertex cache?
        Vertex    vertexCache[VertexCacheSize]; ///< Pre-transformed vertices cache
    }StatesCache;

Screen screen;

void DrawStateReset();
void SetDrawView(int swidth, int sheight);
void SetDrawViewToScreen(Screen s);
#endif