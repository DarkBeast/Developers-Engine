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
typedef struct line_vertex_t line_vertex_t;
typedef struct sbuffer sbuffer;
typedef struct line_buffer line_buffer;
typedef struct vertex_array vertex_array;
typedef struct mbuffer mbuffer;
typedef struct vertex_array vertex_array;
typedef enum stats_t stats_t;
typedef enum vitals_t vitals_t;
typedef enum equipment_t equipment_t;
typedef enum dir_t dir_t;
typedef enum movement_t movement_t;
typedef enum target_type target_type;
typedef struct sprite sprite;

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

struct vector2i // the coordinates for placing on screen
{
	int16 x;
	int16 y;
};

struct framei // the coordinates for placing on screen
{
	int16 x;
	int16 y;
	int16 w;
	int16 h;
};

struct vector2ui // the coordinates for placing on screen
{
	uint16 x;
	uint16 y;
};

struct vector2f
{
	float x;
	float y;
};

struct vector3i //for actual tile coordinates
{
	int16 y;
	int16 x;
	int16 z;
};

struct vector3ui //for actual tile coordinates
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
	int32 width; //le Width
	int32 height; //le height
	uint16 format; // the format of the image.
	uint16 bpp; //how many bytes per pixel are in the image
	unsigned char *pixels; //actual pixel data of image
};

struct vertex_t {
	float x, y; //screen location.
	float u, v; //textures U axis and V axis
	float r, g, b, a; //red, green,blue and alpha.
};

struct line_vertex_t {
	float x, y; //screen location.
	float r, g, b, a; //red, green,blue and alpha.
};

struct vertex_array {
	vertex_t vertex[4]; //array of 4 vertex's
};

struct sbuffer{
	GLuint buffer; //texture vertex buffer.
	GLuint index; //index buffer.
	vertex_t data[4]; //array of 4 vertex's.
};

struct line_buffer{
	GLuint buffer; //texture vertex buffer.
	GLuint index; //index buffer.
	line_vertex_t data[2]; //array of 2 vertex's.
};

struct mbuffer{
	GLuint buffer; //texture vertex buffer.
	uint16 size;  //buffer size.
	uint16 count; //buffer count.
	GLuint index; //index buffer.
	uint16 isize; //index size.
	vertex_array *data; //array of vertex's.
};

struct sprite{
	sbuffer buffer;
	vector2f imgpos; //texture position
	vector2f pos;
	image img;
	float width;
	float height;
	float sizex;
	float sizey;
};

enum vitals_t{
	VITAL_HP = 0,
	VITAL_MP,
	VITAL_SP,
	VITAL_COUNT //always keep last
};

enum stats_t{
	STAT_STRENGTH = 0,
	STAT_DEFENSE,
	STAT_SPEED,
	STAT_MAGIC,
	STAT_COUNT //always keep last
};

enum equipment_t{
	EQUIPMENT_WEAPON = 0,
	EQUIPMENT_ARMOR,
	EQUIPMENT_HELMET,
	EQUIPMENT_SHIELD,
	EQUIPMENT_COUNT //always keep last
};

enum dir_t{
	DIR_NONE,
	DIR_UP,
	DIR_DOWN,
	DIR_LEFT,
	DIR_RIGHT,
	DIR_COUNT
};

enum movement_t{
	MOVEMENT_NONE,
	MOVEMENT_WALKING,
	MOVEMENT_RUNNING,
	MOVEMENT_COUNT
};

enum target_type{
	TARGET_NONE,
	TARGET_PLAYER,
	TARGET_NPC,
	TARGET_TYPE_COUNT
};

enum editor_type{
	EDITOR_NONE,
	EDITOR_MAP,
	EDITOR_ITEM,
	EDITOR_NPC,
	EDITOR_SPELL,
	EDITOR_SHOP
};
#endif