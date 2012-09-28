
#ifndef _Render_H
#define _Render_H

#include <glfw.h>
#include <stdio.h>
#include <stdlib.h>
#include <globals.h>

struct Screen
{
int Width;
int Height;

};

struct Coords
{
int y;
int x;
int z;
};

struct FCoords
{
float y;
float x;
float z;
};

struct Vertex
{
	Coords position;
	Coords TexPos;

};

struct StatesCache
    {
        enum {VertexCacheSize = 4};

        bool      glStatesSet;    ///< Are our internal GL states set yet?
        bool      viewChanged;    ///< Has the current view changed since last draw?
        Uint64    lastTextureId;  ///< Cached texture
        bool      useVertexCache; ///< Did we previously use the vertex cache?
        Vertex    vertexCache[VertexCacheSize]; ///< Pre-transformed vertices cache
    };

Screen screen;

void DrawStateReset();
void SetDrawView(int swidth, int sheight);
void SetDrawViewToScreen(Screen s);
#endif