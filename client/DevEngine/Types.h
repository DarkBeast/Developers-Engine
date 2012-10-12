#ifndef _TYPES_H
#define _TYPES_H

#include "integer.h"
#include <glfw.h>

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

#endif