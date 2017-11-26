#include "lut.h"
#include "bitmap.h"
#include <fstream>

struct accColor
{
	unsigned int r = 0;
	unsigned int g = 0;
	unsigned int b = 0;
};

void lut_set(const color& src_color, const color& dst_color, int size, accColor* lut, int* acc_count, int downscale_shift)
{
	int index = (src_color.b >> downscale_shift) * size * size + (src_color.g >> downscale_shift) * size + (src_color.r >> downscale_shift);

	accColor& pixel = lut[index];
	pixel.r += dst_color.r;
	pixel.g += dst_color.g;
	pixel.b += dst_color.b;
	acc_count[index]++;
}

bool average_lut(const accColor* acc_pixels, color* lut_pixels, const int* acc_count, int total_size)
{
	bool all_valid = true;

	for (int i = 0; i < total_size; i++)
	{
		if (acc_count[i] > 0)
		{
			lut_pixels[i].r = acc_pixels[i].r / acc_count[i];
			lut_pixels[i].g = acc_pixels[i].g / acc_count[i];
			lut_pixels[i].b = acc_pixels[i].b / acc_count[i];
		}
		else
		{
			all_valid = false;
			break;
		}
	}

	return all_valid;
}

#define DDS_MAGIC ' SDD'

#define DDSD_CAPS			0x1
#define DDSD_HEIGHT			0x2
#define DDSD_WIDTH			0x4
#define DDSD_PIXELFORMAT	0x1000
#define DDSD_DEPTH			0x800000

#define DDSCAPS_TEXTURE		0x1000

#define DDPF_ALPHAPIXELS	0x1
#define DDPF_RGB			0x40

struct dds_pixel_format
{
	int size = 32;
	int flag;
	int fourcc;
	int rgb_bit_count;
	int red_bit_mask;
	int green_bit_mask;
	int blue_bit_mask;
	int alpha_bit_mask;
};

struct dds_header
{
	int magic = DDS_MAGIC;
	int size = 124;
	int flag;
	int height;
	int width;
	int pitch_or_linear_size;
	int depth;
	int mip_map_count = 1;
	int reserved1[11];
	dds_pixel_format format;
	int caps;
	int caps2;
	int caps3;
	int caps4;
	int reserved2;
};

void write_dds(const char* filename, color* lut, int size)
{
	if (!filename)	return;

	std::ofstream fo(filename, std::ofstream::out | std::ofstream::binary);

	if (fo.is_open())
	{
		dds_header header;
		header.height               = size;
		header.width                = size;
		header.pitch_or_linear_size = (size * 32 + 7) / 8;
		header.depth                = size;
		header.flag					= DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT | DDSD_DEPTH;
		header.caps                 = DDSCAPS_TEXTURE;
		header.format.flag          = DDPF_RGB | DDPF_ALPHAPIXELS;
		header.format.rgb_bit_count = 32;
		header.format.red_bit_mask   = 0x000000ff;
		header.format.green_bit_mask = 0x0000ff00;
		header.format.blue_bit_mask  = 0x00ff0000;
		header.format.alpha_bit_mask = 0xff000000;

		fo.write((const char*)&header, sizeof(header));
		
		fo.write((const char*)lut, sizeof(color) * size * size * size);

		fo.close();
	}
}

bool lut_generate(const char* src_filename, const char* dst_filename, const char* lut_filename, int size)
{
	color* src_pixels = nullptr;
	bitmap_info src_info;
	if (!bitmap_load(src_filename, src_pixels, src_info))	return false;

	color* dst_pixels = nullptr;
	bitmap_info dst_info;
	if (!bitmap_load(dst_filename, dst_pixels, dst_info))	return false;

	if (src_info.width != dst_info.width || src_info.height != dst_info.height)	return false;

	int grading_width = 0, grading_height = 0, right_shift = 0;
	color_grading_compute_size(size, grading_width, grading_height, right_shift);

	int row_offset = src_info.height - grading_height;
	int col_offset = src_info.width - grading_width;

	int total_size = size * size * size;
	accColor* acc_pixels  = new accColor[total_size];
	int* acc_count = new int[total_size];
	memset(acc_count, 0, total_size * sizeof(int));

	for (int irow = row_offset; irow < src_info.height; irow++)
	{
		for (int icol = col_offset; icol < src_info.width; icol++)
		{
			const color& src_color = src_pixels[irow* src_info.width + icol];

			const color& dst_color = dst_pixels[irow* src_info.width + icol];

			lut_set(src_color, dst_color, size, acc_pixels, acc_count, right_shift);
		}
	}

	color* lut_pixels = new color[total_size];

	if (!average_lut(acc_pixels, lut_pixels, acc_count, total_size))	return false;

	write_dds(lut_filename, lut_pixels, size);

	SAFE_DELETE_ARRAY(acc_pixels);
	SAFE_DELETE_ARRAY(lut_pixels);
	SAFE_DELETE_ARRAY(acc_count);
	SAFE_DELETE_ARRAY(src_pixels);
	SAFE_DELETE_ARRAY(dst_pixels);

	return true;
}