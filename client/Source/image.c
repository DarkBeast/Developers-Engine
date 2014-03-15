/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
*           LibPng Creators
******************************************************************************/

#include <setjmp.h>
#include <stdlib.h>
#include "error.h"
#include "image.h"
#include "integer.h"
#include "png.h"

#define PNGSIGBYTES 8

void load_png(const char *path, image *image)
{
	png_structp png_ptr;
	png_infop info_ptr, end_info;
	png_uint_32 rowbytes, numbytes;
	png_uint_32 bit_depth, color_type;
	png_byte* pixelz;
	png_byte** row_ptrs;
	uint32 i;

	unsigned char header[PNGSIGBYTES];

	FILE *png_file = fopen(path, "rb");

	if (!png_file){
		error_handler(DE_ERROR_FILE_ERROR, "error in load_png(), image not found.\n");
	}
	fread(header, 1, PNGSIGBYTES, png_file);
	if(png_sig_cmp(header, 0, PNGSIGBYTES))
		error_handler(DE_ERROR_FILE_ERROR, "libpng error\n");

	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr)
		error_handler(DE_ERROR_MISC_ERROR, "libpng error\n");

	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
		error_handler(DE_ERROR_MISC_ERROR, "libpng error\n");

	end_info = png_create_info_struct(png_ptr);
	if (!end_info)
		error_handler(DE_ERROR_MISC_ERROR, "libpng error\n");

	if(setjmp(png_jmpbuf(png_ptr)))
		error_handler(DE_ERROR_IO_ERROR, "libpng jump error\n");

	png_init_io(png_ptr, png_file);
	png_set_sig_bytes(png_ptr, PNGSIGBYTES);
	png_read_info(png_ptr, info_ptr);

	image->width = (uint32)png_get_image_width(png_ptr, info_ptr);
	image->height = (uint32)png_get_image_height(png_ptr, info_ptr);

	bit_depth = png_get_bit_depth(png_ptr, info_ptr);
	color_type = png_get_color_type(png_ptr, info_ptr);

	if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
		png_set_expand_gray_1_2_4_to_8(png_ptr);

	if (bit_depth == 16)
		png_set_strip_16(png_ptr);

	if(color_type == PNG_COLOR_TYPE_PALETTE)
		png_set_palette_to_rgb(png_ptr);
	else if(color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA){
		png_set_gray_to_rgb(png_ptr);
	}

	if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
		png_set_tRNS_to_alpha(png_ptr);
	else if(color_type == PNG_COLOR_TYPE_RGB_ALPHA){
		image->format = GL_RGBA;
	}
	else
		png_set_filler(png_ptr, 0xff, PNG_FILLER_AFTER);

	png_read_update_info(png_ptr, info_ptr);

	rowbytes = png_get_rowbytes(png_ptr, info_ptr);
	numbytes = rowbytes*(image->height);
	pixelz = (png_byte*)malloc(numbytes);
	row_ptrs = (png_byte**)malloc((image->height) * sizeof(png_byte*));

	if(row_ptrs == NULL || pixelz == NULL){
		error_handler(DE_ERROR_POINTER_NULL, "row_ptrs or pixelz in load_png()\n");
		return;
	}

	for (i=0; i<((uint32)image->height); i++)
		row_ptrs[i] = pixelz + ((image->height) - 1 - i)*rowbytes;

	png_read_image(png_ptr, row_ptrs);

	image->pixels = (unsigned char *)calloc(1, numbytes);

	if(image->pixels == NULL){
		error_handler(DE_ERROR_POINTER_NULL, "image->pixels in load_png()\n");
		return;
	}

	memcpy(image->pixels, pixelz, image->width * image->height * 4);

	free(pixelz);
	free(row_ptrs);
	pixelz = NULL;
	row_ptrs = NULL;
	png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
	fclose(png_file);
}