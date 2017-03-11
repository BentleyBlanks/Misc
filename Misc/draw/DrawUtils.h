#pragma once
#include "ofMain.h"


class Graph3D
{
public:
	Graph3D(float w,float l,int x_res,int y_res);
	void set_x(int x_res);
	void set_y(int y_res);

	void set_val();

	void draw();
protected:

private:
	void _rediv();

	float w,l;
	int x_res;
	int y_res;
	ofMesh _m;
};

