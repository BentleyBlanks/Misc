#include "grading.h"
#include "bitmap.h"
#include "color_grading_priv.h"

void generate_pixels(color* pixels, const bitmap_info& info, int size, int downscale_shift)
{
	int blue_stride = info.width / size;

	for (int row = 0; row < info.height; row++)
	{
		int g = row % size;

		int offset = (int)(row / size) * blue_stride;

		for (int col = 0; col < info.width; col++)
		{
			int r = col % size;

			int b = offset + col / size;

			auto& c = pixels[row * info.width + col];

			c.r = r << downscale_shift;
			c.g = g << downscale_shift;
			c.b = b << downscale_shift;
		}
	}
}

void grading_generate(const char* save_path, int size, int dpi)
{
	bitmap_info info;
	info.dpi = dpi;

	int downscale_shift = 0;
	color_grading_compute_size(size, info.width, info.height, downscale_shift);

	color* pixels = new color[info.width * info.height];

	generate_pixels(pixels, info, size, downscale_shift);

	bitmap_write(save_path, pixels, info);

	delete[] pixels;	pixels = nullptr;
}