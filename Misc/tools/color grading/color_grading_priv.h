#pragma once

#define UBYTE unsigned char

#define SAFE_DELETE_ARRAY(a)	if((a)) {delete[] (a); (a) = nullptr;}

struct color
{
	UBYTE r = 0;
	UBYTE g = 0;
	UBYTE b = 0;
	UBYTE a = 255;
};

bool color_grading_compute_size(UBYTE size, int& width, int& height, int& downscale_shift);