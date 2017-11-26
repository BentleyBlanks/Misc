#include "bitmap.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#define INCH_PER_METER	39.370079f

int dib_type_query(int dib_size)
{
	int type;

	switch (dib_size)
	{
	case 40:
		type = dib_bitmap_info_header;
		break;

	default:
		type = dib_not_support;
	}

	return type;
}

int compute_row_pitch_byte(int bpp , int width)
{
	return (int)std::floor((bpp * width + 31) / 32) * 4;
}

void write_int(ofstream& fo, int v)
{
	fo.write((const char*)&v, sizeof(int));
}

void write_short(ofstream& fo, short v)
{
	fo.write((const char*)&v, sizeof(short));
}

void write_byte(ofstream& fo, UBYTE v)
{
	fo.write((const char*)&v, sizeof(UBYTE));
}

void write_align(ofstream& fo, int byte_num)
{
	char temp[4] = { 0 };
	fo.write(temp, byte_num);
}

int get_current_pos(ofstream& fo)
{
	return (int)fo.tellp();
}

void set_current_pos(ofstream& fo, int pos)
{
	fo.seekp(pos);
}

void write_bitmap_header(ofstream& fo)
{
	write_short(fo, 0x4D42);
	write_int(fo, 0);
	write_short(fo, 0);
	write_short(fo, 0);
	write_int(fo, 0);
}

// common windows format is the BITMAPINFOHEADER header
void write_dib_header(ofstream& fo, const bitmap_info& info)
{
	write_int(fo, 40);
	write_int(fo, info.width);
	write_int(fo, info.height);
	write_short(fo, 1);								// color plane, must be 1
	write_short(fo, info.bpp);						// the number of bits per pixek
	write_int(fo, 0);								// the compression method. we use BI_RGB - no compression
	write_int(fo, 0);								// the image size. this is the size of raw bitmap data, a dummy 0 can be given for BI_RGB bitmaps
	write_int(fo, int(info.dpi * INCH_PER_METER));	// the horizontal resolution of the image(pixel per meter)
	write_int(fo, int(info.dpi * INCH_PER_METER));	// the vertical resolution of the image.(pixel per meter)
	write_int(fo, 0);								// the number of color palette
	write_int(fo, 0);								// importance colors used. 0 when every color is important. generally ignored
}

void write_pixel_array(ofstream& fo, const bitmap_info& info, const color* pixels)
{
	int pixel_offset = get_current_pos(fo);
	set_current_pos(fo, 10);
	write_int(fo, pixel_offset);
	set_current_pos(fo, pixel_offset);

	int row_padding_byte = compute_row_pitch_byte(info.bpp, info.width) - info.bpp * info.width / 8;

	bool reverse = info.height > 0;

	int height = info.height * (reverse ? 1 : -1);

	for (int r = 0; r < height; r++)
	{
		int row = reverse ? height - 1 - r : r;

		for (int col = 0; col < info.width; col++)
		{
			const color& pixel = pixels[row * info.width + col];

			write_byte(fo, pixel.b);
			write_byte(fo, pixel.g);
			write_byte(fo, pixel.r);
		}

		write_align(fo, row_padding_byte);
	}
}

void write_icc_color_profile(ofstream& fo)
{
	return;
}

void write_image_size(ofstream& fo)
{
	int image_size = get_current_pos(fo);

	set_current_pos(fo, 2);

	write_int(fo, image_size);
}

void bitmap_write(const char* filename, const color* pixels, const bitmap_info& info)
{
	if (!filename || !pixels)	return;

	ofstream fo(filename, ofstream::out | ofstream::binary);
	if (fo.is_open())
	{
		write_bitmap_header(fo);

		write_dib_header(fo, info);

		write_pixel_array(fo, info, pixels);

		write_image_size(fo);

		fo.close();
	}
}

int load_int(ifstream& fi)
{
	int v = 0;
	fi.read((char*)&v, sizeof(int));
	return v;
}

short load_short(ifstream& fi)
{
	short v = 0;
	fi.read((char*)&v, sizeof(short));
	return v;
}

UBYTE load_byte(ifstream& fi)
{
	UBYTE v = 0;
	fi.read((char*)&v, sizeof(UBYTE));
	return v;
}

void skip(ifstream& fi, int byte_num)
{
	char temp[4];
	fi.read(temp, byte_num);
}

bool load_bitmap_header(ifstream& fi, bitmap_info& info)
{
	short identify = load_short(fi);
	if (identify != 0x4D42)	return false;

	load_int(fi);
	load_short(fi);
	load_short(fi);

	info.dib = dib_type_query(load_int(fi) - 14);

	return true;
}

bool load_dib_header(ifstream& fi, bitmap_info& info)
{
	int header_size = load_int(fi);
	if (header_size != 40)	return false;

	info.width = load_int(fi);
	info.height = load_int(fi);

	int plane_count = load_short(fi);
	if (plane_count != 1)	return false;

	info.bpp = load_short(fi);
	info.cm = load_int(fi);
	load_int(fi);

	int res_hor = load_int(fi);
	int res_ver = load_int(fi);

	load_int(fi);
	load_int(fi);

	return true;
}

void load_pixel_array(ifstream& fi, bitmap_info& info, color*& pixels)
{
	bool reverse = info.height > 0;
	int height = reverse ? info.height : -info.height;

	pixels = new color[info.width * height];

	int row_padding_byte = compute_row_pitch_byte(info.bpp, info.width) - info.bpp * info.width / 8;

	for (int i = 0; i < height; i++)
	{
		int row = reverse ? height - 1 - i : i;

		for (int j = 0; j < info.width; j++)
		{
			color& pixel = pixels[row * info.width + j];

			pixel.b = load_byte(fi);
			pixel.g = load_byte(fi);
			pixel.r = load_byte(fi);
		}

		skip(fi, row_padding_byte);
	}
}

bool bitmap_load(const char* filename, color*& pixels, bitmap_info& info)
{
	bool result = false;
	ifstream fi(filename, ifstream::in | ifstream::binary);
	if (fi.is_open())
	{
		if (load_bitmap_header(fi, info) && info.dib == dib_bitmap_info_header)
		{
			if (load_dib_header(fi, info) && info.cm == 0 && info.bpp == 24)
			{
				load_pixel_array(fi, info, pixels);

				result = true;
			}
		}

		fi.close();
	}

	return result;
}