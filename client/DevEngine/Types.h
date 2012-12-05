/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/

#ifndef _TYPES_H
#define _TYPES_H

#include <glfw.h>
#include "integer.h"

typedef struct
{
	void **data;
	size_t size;
	size_t count;
}voidarray;

typedef struct color
{
	float r; // Red
	float g; // Green
	float b; // Blue
	float a; // Alpha transparency
}color;

typedef struct vector2i // the coords for placing on screen
{
	int x;
	int y;
}vector2i;

typedef struct vector2ui // the coords for placing on screen
{
	unsigned int x;
	unsigned int y;
}vector2ui;

typedef struct vector2f
{
	float x;
	float y;
}vector2f;

typedef struct vector3i //for actual tile coords
{
	int y;
	int x;
	int z;
}vector3i;

typedef struct vector3ui //for actual tile coords
{
	unsigned int y;
	unsigned int x;
	unsigned int z;
}vector3ui;

typedef struct vector3f // for pixels
{
	float y;
	float x;
	float z;
}vector3f;

typedef struct vertex
{
	vector2i position;
	vector2i texpos;
	color color;
}vertex;

typedef struct image
{
	GLuint texid; //image ID in opengl
	int width; // le Width
	int height; //le height
	int format; // the format of the image.
	int bpp; //how many bytes per pixel are in the image
	unsigned char *pixels; //actual pixel data of image
	char reload;
}image;

#endif