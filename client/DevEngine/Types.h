/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/

#ifndef DE_TYPES_H
#define DE_TYPES_H

#include <glfw3.h>
#include "integer.h"

typedef struct image image;
typedef struct vertex vertex;
typedef struct vector3f vector3f;
typedef struct vector3ui vector3ui;
typedef struct vector3i vector3i;
typedef struct vector2f vector2f;
typedef struct vector2ui vector2ui;
typedef struct framei framei;
typedef struct vector2i vector2i;
typedef struct cstring cstring;
typedef struct color color;
typedef struct void_array void_array;
typedef struct vertex_t vertex_t;
typedef struct sbuffer sbuffer;
typedef struct vertex_array vertex_array;
typedef struct mbuffer mbuffer;
typedef struct vertex_array vertex_array;

struct void_array
{
	void **data;
	size_t size;
	size_t count;
};

struct color
{
	float r; // Red
	float g; // Green
	float b; // Blue
	float a; // Alpha transparency
};

struct cstring
{
	char *data;
	size_t  count;
	size_t  size;
};

struct vector2i // the coords for placing on screen
{
	int16 x;
	int16 y;
};

struct framei // the coords for placing on screen
{
	int16 x;
	int16 y;
	int16 w;
	int16 h;
};

struct vector2ui // the coords for placing on screen
{
	uint16 x;
	uint16 y;
};

struct vector2f
{
	float x;
	float y;
};

struct vector3i //for actual tile coords
{
	int16 y;
	int16 x;
	int16 z;
};

struct vector3ui //for actual tile coords
{
	uint16 y;
	uint16 x;
	uint16 z;
};

struct vector3f{
	float y;
	float x;
	float z;
};

struct vertex{
	vector2i position;
	vector2i texpos;
	color color;
};

struct image{
	GLuint texid; //image ID in opengl
	int32 width; // le Width
	int32 height; //le height
	uint16 format; // the format of the image.
	uint16 bpp; //how many bytes per pixel are in the image
	unsigned char *pixels; //actual pixel data of image
};

struct vertex_t {
	float x, y;
	float u, v;
	float r, g, b, a;
};

struct vertex_array {
	vertex_t vertex[4];
};

struct sbuffer{
	GLuint buffer; //texture vertex buffer.
	uint16 size;  //buffer size.
	uint16 count; //buffer count.
	GLuint index; //index buffer.
	uint16 isize; //index size.
	vertex_t data[4];
};

struct mbuffer{
	GLuint buffer; //texture vertex buffer.
	uint16 size;  //buffer size.
	uint16 count; //buffer count.
	GLuint index; //index buffer.
	uint16 isize; //index size.
	vertex_array *data;
};

#endif