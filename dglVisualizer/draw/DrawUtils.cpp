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

void Graph3D::draw_aabb(RBVector3 mi,RBVector3 ma)
{
	ofLine(ofPoint(mi.x),ofPoint());
	ofLine(ofPoint(),ofPoint());
	ofLine(ofPoint(),ofPoint());
	ofLine(ofPoint(),ofPoint());
	ofLine(ofPoint(),ofPoint());
	ofLine(ofPoint(),ofPoint());
	ofLine(ofPoint(),ofPoint());
	ofLine(ofPoint(),ofPoint());
	ofLine(ofPoint(),ofPoint());
	ofLine(ofPoint(),ofPoint());
	ofLine(ofPoint(),ofPoint());
	ofLine(ofPoint(),ofPoint());
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

void Axis3D::draw()
{
	
}

void Graph2D::draw_function(float xa,float xb,int xres)
{
	float dx = (xb - xa )/xres;
	for(float i=xa;i<xb;i+=dx)
	{
		
		float f1 = 1 + cos(0.04*(i-dx)*(i-dx));
		float f2 = 1 + cos(0.04*i*i);

		/*
		float f1n = 0;
		{
			float t = i;
			while (t>xa)
			{
				float j = (0.08*(i-t));
				j = sin(3.1415926*j)/(3.1415926*j);
				f1n+=j*f2;
				t-=dx;
			}
			t = i;
			while (t<xb)
			{
				float j = -(0.08*(i -t));
				j = sin(3.1415926*j)/(3.1415926*j);
				f1n+=j*f2;
				t+=dx;
			}
		}

		float f2n = 0;
		{
			float t = i-dx;
			while (t>xa)
			{
				float j = (0.08*(i-dx-t));
				j = sin(3.1415926*j)/(3.1415926*j);
				f2n+=j*f1;
				t-=dx;
			}
			t = i-dx;
			while (t<xb)
			{
				float j = -(0.08*(i-dx -t));
				j = sin(3.1415926*j)/(3.1415926*j);
				f2n+=j*f1;
				t+=dx;
			}
		}
		
		f1 = f1n;//sin(3.1415926*(i-dx))/3.1415926/(i-dx);
		f2 = f2n;//sin(3.1415926*(i))/3.1415926/(i);
		*/
		ofLine(ofPoint((i-dx)*10+100,ofGetWindowHeight()- f1*100-100),ofPoint(i*10+100,ofGetWindowHeight()- f2*100-100));
	}
}

void Graph2D::draw_function1(float xa,float xb,int xres)
{
	float dx = (xb - xa )/xres;
	for(float i=xa;i<xb;i+=dx)
	{
		
		float f1 = 1 + cos(0.04*(i-dx)*(i-dx));
		float f2 = 1 + cos(0.04*i*i);

		
		f1 = sin(3.1415926*(0.04*(i-dx)))/3.1415926/(0.04*(i-dx));
		f2 = sin(3.1415926*(0.04*i))/3.1415926/(0.04*i);

		ofLine(ofPoint((i-dx)*10+100,ofGetWindowHeight()- f1*100-100),ofPoint(i*10+100,ofGetWindowHeight()- f2*100-100));
	}
}

void Graph2D::sample_concentric_disk(float u1,float u2,float *dx,float *dy)
{
	float r, theta;
	// Map uniform random numbers to $[-1,1]^2$
	float sx = 2 * u1 - 1;
	float sy = 2 * u2 - 1;

	// Map square to $(r,\theta)$

	// Handle degeneracy at the origin
	if (sx == 0.0 && sy == 0.0) {
		*dx = 0.0;
		*dy = 0.0;
		return;
	}
	if (sx >= -sy) {
		if (sx > sy) {
			// Handle first region of disk
			r = sx;
			if (sy > 0.0) theta = sy/r;
			else          theta = 8.0f + sy/r;
		}
		else {
			// Handle second region of disk
			r = sy;
			theta = 2.0f - sx/r;
		}
	}
	else {
		if (sx <= sy) {
			// Handle third region of disk
			r = -sx;
			theta = 4.0f - sy/r;
		}
		else {
			// Handle fourth region of disk
			r = -sy;
			theta = 6.0f + sx/r;
		}
	}
	theta *= PI / 4.f;
	*dx = r * cosf(theta);
	*dy = r * sinf(theta);
}

RBVector3 Graph2D::sample_cosine_hemisphere(float u1,float u2)
{
	RBVector3 ret;
	sample_concentric_disk(u1,u2,&ret.x,&ret.z);
	ret.y = sqrt(RBMath::get_max(0.f,1.f-ret.x*ret.x-ret.z*ret.z));
	ret.z = -ret.z;
	return ret;
}
