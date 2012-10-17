/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis    
******************************************************************************/

#ifndef _TYPES_H
#define _TYPES_H

#include <glfw.h>
#include "integer.h"

typedef struct Screen
{
	int Width;
	int Height;
}Screen;

typedef struct Color
{
	unsigned int r;
	unsigned int g;
	unsigned int b;
	unsigned int a;
}Color;

typedef struct Vector2i // the coords for placing on screen
{
	int x;
	int y;
}Vector2i;

typedef struct Vector2ui // the coords for placing on screen
{
	unsigned int x;
	unsigned int y;
}Vector2ui;

typedef struct Vector2f
{
	float x;
	float y;
}Vector2f;

typedef struct Vector3i //for actule tile coords
{
	int y;
	int x;
	int z;
}Vector3i;

typedef struct Vector3ui //for actule tile coords
{
	unsigned int y;
	unsigned int x;
	unsigned int z;
}Vector3ui;

typedef struct Vector3f // for pixels
{
	float y;
	float x;
	float z;
}Vector3f;

typedef struct Vertex  
{
	Vector2i position;
	Vector2f texpos;
	Color color;
}Vertex;

typedef struct Image
{
	int Width; // le Width
	int Height; //le height
	int Format; // the format of the image.
	int BytesPerPixel; //how many bytes per pixel are in the image
	unsigned char *pixels; //actule pixel data of image
	char reload;
	GLuint texID; //image ID in opengl
}Image;

#endif