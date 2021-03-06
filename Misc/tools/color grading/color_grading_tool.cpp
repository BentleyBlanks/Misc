#include <iostream>
#include <string>
#include "grading.h"
#include "lut.h"
#include "color_grading_priv.h"

bool color_grading_compute_size(UBYTE size, int& width, int& height, int& downscale_shift)
{
	if (size == 0 || (size & (size - 1)))	return false;

	int power_count = -1;
	int s = size;
	while (s > 0)
	{
		s >>= 1;
		power_count++;
	}

	width = size << (power_count - power_count / 2);
	height = size << (power_count / 2);

	downscale_shift = 8 - power_count;

	return true;
}

bool bmp_filename_valid(const char* filename)
{
	if (!filename)	return false;

	int len = (int)strlen(filename);

	bool filename_valid = false;

	if (len >= 4 && filename[len - 4] == '.' &&
		(filename[len - 3] == 'b' || filename[len - 3] == 'B') &&
		(filename[len - 2] == 'm' || filename[len - 2] == 'M') &&
		(filename[len - 1] == 'p' || filename[len - 1] == 'P')
		)
	{
		filename_valid = true;
	}

	return filename_valid;
}

bool dds_filename_valid(const char* filename)
{
	if (!filename)	return false;

	int len = (int)strlen(filename);

	bool filename_valid = false;

	if (len >= 4 && filename[len - 4] == '.' &&
		(filename[len - 3] == 'd' || filename[len - 3] == 'D') &&
		(filename[len - 2] == 'd' || filename[len - 2] == 'D') &&
		(filename[len - 1] == 's' || filename[len - 1] == 'S')
		)
	{
		filename_valid = true;
	}

	return filename_valid;
}

bool grading_size_isvalid(int size)
{
	return (size > 0) && (size < 256) && !(size & (size - 1));
}

void cmd_display_grading_helper()
{
	std::cout << std::endl;

	std::cout << "-grading:" << std::endl;
	std::cout << "	" << "<save_path>" << "	" << "file extension must be bmp" << std::endl;
	std::cout << "	" << "<grading_size>" << "	" << "must be power of two" << std::endl;
	std::cout << "	" << "[dpi]" << "		" << "default value 72" << std::endl;
}

void cmd_display_lut_helper()
{
	std::cout << std::endl;

	std::cout << "-lut:" << std::endl;

	std::cout << "	" << "<src_path>" << "	" << "file extension must be bmp" << std::endl;
	std::cout << "	" << "<dst_path>" << "	" << "file extension must be bmp" << std::endl;
	std::cout << "	" << "<lut_path>" << "	" << "file extension must be dds" << std::endl;
	std::cout << "	" << "<grading_size>" << "	" << "must be power of two" << std::endl;
	std::cout << std::endl;
	std::cout << "	" << "Note: bmp just support 24-bit uncompress version" << std::endl;
}

void cmd_display_helper()
{
	std::cout << std::endl;

	std::cout << "Usage: color_grading_tool <command>" << std::endl << std::endl;

	std::cout << "Command:" << std::endl;

	std::cout << "	" << "-grading" << "	" << "generate color grading map" << std::endl;
	std::cout << "	" << "-lut" << "		" << "generate 3d lut" << std::endl;

	cmd_display_grading_helper();

	cmd_display_lut_helper();
}

void cmd_parse_grading(int argc, char* argv[])
{
	if (argc < 4)
	{
		cmd_display_grading_helper();
		return;
	}

	const char* save_path = argv[2];

	if (!bmp_filename_valid(save_path))
	{
		std::cout << std::endl << "filename is invalid. file extension must be bmp" << std::endl;
		return;
	}

	int size = atoi(argv[3]);
	if (!grading_size_isvalid(size))
	{
		std::cout << std::endl << "size is invalid. must be power of two, must less than 256" << std::endl;
		return;
	}

	int dpi = 72;
	if (argc == 5)	dpi = atoi(argv[4]);
	if (dpi <= 0)
	{
		std::cout << std::endl << "dpi is invalid. must greater than zero." << std::endl;
		return;
	}

	grading_generate(save_path, size, dpi);

	std::cout << std::endl << "generate finish,path:" << save_path << std::endl;
}

void cmd_parse_lut(int argc, char* argv[])
{
	if (argc < 6)
	{
		cmd_display_lut_helper();
		return;
	}

	const char* src_path = argv[2];
	const char* dst_path = argv[3];

	if (!bmp_filename_valid(src_path) || !bmp_filename_valid(dst_path))
	{
		std::cout << std::endl << "src_path and dst_path is invalid, filename extension must be bmp" << std::endl;
		return;
	}

	const char* lut_path = argv[4];

	if (!dds_filename_valid(lut_path))
	{
		std::cout << std::endl << "lut_path is invalid, filename extension must be dds" << std::endl;
		return;
	}

	int size = atoi(argv[5]);

	if (!grading_size_isvalid(size))
	{
		std::cout << std::endl << "size is invalid. must be power of two, must less than 256" << std::endl;
		return;
	}

	if (lut_generate(src_path, dst_path, lut_path, size))
	{
		std::cout << std::endl << "generate success,path:" << lut_path << std::endl;
	}
	else
	{
		std::cout << std::endl << "generate failed" << std::endl;
	}
}

void cmd_parse(int argc, char* argv[])
{
	if (argc < 2)
	{
		cmd_display_helper();
		return;
	}

	if (strcmp(argv[1], "?") == 0)
	{
		cmd_display_helper();
	}
	else if (strcmp(argv[1], "-grading") == 0)
	{
		cmd_parse_grading(argc, argv);
	}
	else if (strcmp(argv[1], "-lut") == 0)
	{
		cmd_parse_lut(argc, argv);
	}
	else
	{
		std::cout << std::endl << "unsupport command" << std::endl;

		cmd_display_helper();
	}
}

int main(int argc, char *argv[])
{
	cmd_parse(argc, argv);

    return 0;
}