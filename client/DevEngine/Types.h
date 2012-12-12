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

typedef struct
{
	char *data;
	size_t  count;
	size_t  size;
}cstring;

typedef struct vector2i // the coords for placing on screen
{
	int16 x;
	int16 y;
}vector2i;

typedef struct vector2ui // the coords for placing on screen
{
	uint16 x;
	uint16 y;
}vector2ui;

typedef struct vector2f
{
	float x;
	float y;
}vector2f;

typedef struct vector3i //for actual tile coords
{
	int16 y;
	int16 x;
	int16 z;
}vector3i;

typedef struct vector3ui //for actual tile coords
{
	uint16 y;
	uint16 x;
	uint16 z;
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
	int16 width; // le Width
	int16 height; //le height
	int16 format; // the format of the image.
	int16 bpp; //how many bytes per pixel are in the image
	unsigned char *pixels; //actual pixel data of image
	uint8 reload;
}image;

#endif