#ifndef _image_H
#define _image_H

#include "Types.h"
Vector2i pos;
//TODO: Add BMP and TGA and JPEG.
int load_png(const char *name, Image *image);

#endif