#pragma once

#include "color_grading_priv.h"

#define dib_not_support				0
#define dib_bitmap_info_header		1

struct bitmap_info
{
	short				bpp               = 24;
	int					width             = 256;
	int					height            = 128;
	int					dpi				  = 72;
	int					cm				  = 0;
	int					dib               = dib_bitmap_info_header;
};

void bitmap_write(const char* filename, const color* pixels, const bitmap_info& info);

bool bitmap_load(const char* filename, color*& pixels, bitmap_info& info);