#ifndef _TYPES_H
#define _TYPES_H

#include "integer.h"

const int VertexCacheSize = 4;

typedef struct Screen
{
	int Width;
	int Height;

}Screen;

typedef struct SCoord
{
	int x1;
	int x2;
	int y1;
	int y2;
}SCoord;

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

typedef struct Image
{
	int Width, Height;
	int Format;
	int BytesPerPixel;
	unsigned char *pixels;
	GLuint texID;
}Image;


typedef struct StatesCache
{
	CBool      glStatesSet;    ///< Are our internal GL states set yet?
	CBool      viewChanged;    ///< Has the current view changed since last draw?
	Uint32    lastTextureId;  ///< Cached texture
	CBool      useVertexCache; ///< Did we previously use the vertex cache?
	Vertex    vertexCache[VertexCacheSize]; ///< Pre-transformed vertices cache
}StatesCache;

#endif