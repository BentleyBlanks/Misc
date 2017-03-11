// obj_divide.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>
#include "iostream"
#include <string>

using namespace std;

struct RBVector3
{
  float x,y,z;
};

struct RBVector2
{
  float x, y;
};

int main(int argc, char* argv[])
{
  string file_name = argv[1];

  string output = "";
  std::ifstream ifile(file_name, std::ios::in);

  if (!ifile)
  {
    std::cout << "读取失败，没有这个obj文件，即将退出！" << std::endl;
    exit(0);
  }

  char in[256];
  string nameo = "";
  unsigned int nv = 0;
  unsigned int nvt = 0;
  unsigned int sub_nv = 0;
  unsigned int sub_nvt = 0;
  do
  {

    if (in[0] == '#'&&in[1] == ' '&&in[2] == 'o'&&in[3] == 'b'&&in[4] == 'j'&&in[5] == 'e')
    {

      if (nameo!="")
      { 
        std::ofstream ofile(nameo, std::ios::out);
        ofile << output;
        ofile.close();
        output.clear();
      }
      char name[64];
      sscanf(in, "# object %s", name);
      nameo = string(".\\") + name + ".obj";
      
      sub_nv = nv;
      sub_nvt = nvt;
    }
    
    if (in[0] == 'v'&&in[1] == ' ')
    {
      output += in;
      nv++;
    }
    else if (in[0] == 'v'&&in[1] == 't'&&in[2] == ' ')
    { 
      output += in;
      nvt++;
    }
    else if (in[0] == 'f'&&in[1] == ' ')
    {
      int ks[8];
      int k = sscanf(in, "f %d/%d %d/%d %d/%d %d/%d", &ks[0], &ks[1], &ks[2], &ks[3], &ks[4], &ks[5], &ks[6], &ks[7]);
      for (int i = 0; i < k; ++i)
      {
        if (i % 2==0)//v
          ks[i] -= sub_nv;
        else
          ks[i] -= sub_nvt;
      }
      char _o[256];
      if (k == 6)
      {
        sprintf(_o, "f %d/%d %d/%d %d/%d", ks[0], ks[1], ks[2], ks[3], ks[4], ks[5]);
        output += _o;
      }
      else if (k == 8)
      {
        sprintf(_o, "f %d/%d %d/%d %d/%d %d/%d", ks[0], ks[1], ks[2], ks[3], ks[4], ks[5], ks[6], ks[7]);
        output += _o;
      }
      else
      {
        printf("there is a k:%d",k);
        output += "";
      }

    }

    
    output += '\n';
    
  } while (ifile.getline(in, 128));
	return 0;
}

