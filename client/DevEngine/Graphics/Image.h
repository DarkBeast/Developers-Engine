#ifndef _image_H
#define _image_H

#include <glfw.h>

int read_png_file(char* file_name);
GLuint png_texture_load(const char* file_name, int width, int height);
void write_png_file(char* file_name);
#endif