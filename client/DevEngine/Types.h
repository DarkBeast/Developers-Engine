/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis    
******************************************************************************/

#ifndef _TYPES_H
#define _TYPES_H

#include <glfw.h>
#include "integer.h"

typedef struct color
{
	unsigned int r;
	unsigned int g;
	unsigned int b;
	unsigned int a;
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

typedef struct vector3i //for actule tile coords
{
	int y;
	int x;
	int z;
}vector3i;

typedef struct vector3ui //for actule tile coords
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
	vector2f texpos;
	color color;
}vertex;

typedef struct image
{
	int width; // le Width
	int height; //le height
	int format; // the format of the image.
	int bpp; //how many bytes per pixel are in the image
	unsigned char *pixels; //actule pixel data of image
	char reload;
	GLuint texid; //image ID in opengl
}image;

#endif