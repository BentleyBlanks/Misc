#pragma once
#include "ofMain.h"
#include "Vector3.h"


class Graph3D
{
public:
	Graph3D(float w,float l,int x_res,int y_res);
	void set_x(int x_res);
	void set_y(int y_res);

	void set_val();

	void draw();

	static void draw_aabb(RBVector3 mi,RBVector3 ma);
protected:

private:
	void _rediv();

	float w,l;
	int x_res;
	int y_res;
	ofMesh _m;
};

class Axis3D
{
public:
	void draw();
};

class Graph2D
{
public:
	static void draw_function(float xa,float xb,int xres);
	static void draw_function1(float xa,float xb,int xres);
	static void sample_concentric_disk(float u1,float u2,float *dx,float *dy);
	static RBVector3 sample_cosine_hemisphere(float u1,float u2);
};
