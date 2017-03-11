#include "DrawUtils.h"

Graph3D::Graph3D(float w,float l,int x_res,int y_res)
{
	this->x_res = x_res;
	this->y_res = y_res;
	this->w = w;
	this->l = l;
	_rediv();
}

void Graph3D::draw()
{
	
	_m.drawWireframe();
	
}


void Graph3D::_rediv()
{
	_m.clear();
	float dw = w/x_res;
	float dl = l/y_res;
	float a = 0;
	float b = 0;
	float c = 0;
	float d = 0;
	for(int i=0;i<y_res;i++)
	{
		for(int j=0;j<x_res;j++)
		{
			float u = ((float)i)/y_res;
			float v = ((float)j)/x_res;
			float y = (1-u)*(1-v)*a + (1-u)*v*b + u*(1-v)*c + u*v*d;
			_m.addVertex(ofVec3f(dw*j -w/2 ,y ,dl*i - l/2));
		}
	}
	for(int i=0;i<y_res-1;i++)
	{
		for(int j=0;j<x_res-1;j++)
		{
			_m.addIndex(i*x_res+j);
			_m.addIndex(i*x_res+j+1);
			_m.addIndex(i*x_res+x_res+j);

			_m.addIndex(i*x_res+j+1);
			_m.addIndex(i*x_res+x_res+j+1);
			_m.addIndex(i*x_res+x_res+j);
		}
	}

}
