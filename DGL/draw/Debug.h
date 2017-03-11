#pragma once
#include <vector>
#include <map>
#include <string>
#include <queue>
#include <mutex>
#include "ofColor.h"

//thread safe
class RBDebugDraw
{
public:
	enum DrawType
	{
		DRAWTRIANGLE,
		DRAWGRID,
		DRAWPOINT,
		DRAWRECT,
		DRAWQUAD,
		DRAWLINE,
	};
	struct Pak
	{
		Pak(DrawType t, float x,float y,float x1=0,float y1=0,float x2=0,float y2=0,float x3=0,float y3 =0) 
		{
			tp = t;
			v[0] = x;
			v[1] = y;
			v[2] = x1;
			v[3] = y1;
			v[4] = x2;
			v[5] = y2;
			v[6] = x3;
			v[7] = y3;
		}
		DrawType tp;
		float v[8];
		ofColor c;
		bool fill;
	};
	void draw_grid(ofColor c,bool fill, float x,float y);
	void draw_point(ofColor c,bool fill, float x,float y);
	void draw_rect(ofColor c,bool fill, float x,float y,float w,float h);
	void draw_quad(ofColor c,bool fill, float x,float y,float x1,float y1,float x2,float y2,float x3,float y3);
	void draw_line(ofColor c,bool fill, float x,float y,float x1,float y1);
	void draw_triangle(ofColor c,bool fill, float x,float y,float x1,float y1,float x2,float y2);

	void draw();

	std::queue<Pak> _queue;
	std::mutex _mut;
};

extern RBDebugDraw* debugdraw;

class RBDataCollector
{
public:
	static RBDataCollector* get_inst()
	{
		static RBDataCollector*_inst = nullptr;
		if(!_inst)
			_inst = new RBDataCollector();
		return _inst;
	}
	void put_int_int(int key,int val);
	int  get_int_int(int key);
	void put_float(std::string name,float val);
	void put_string(std::string name,std::string val);
	void put_ptr(std::string name,void* ptr);
	float get_float(std::string name);
	std::string get_string(std::string name);
	void* get_ptr(std::string name);
	float& get_float_ref(std::string name);
	std::string& get_string_ref(std::string name);
	void*& get_ptr_ref(std::string name);
	float get_float_avg();
	void output_floats();
	void output_strings();
	void output_ptrs();
	void output();
private:
	std::map<std::string,float> _data_float;
	std::map<std::string,std::string> _data_string;
	std::map<std::string,void*> _data_ptr;
	std::vector<int> _data_int_int;

protected:
private:
};

#include "RBMath\inc\Vector3.h"
#include "RBMath\inc\Matrix.h"

class RBDebugArrow
{
public:
	RBVector3 v_s;
	RBVector3 v_e;

	void draw(class RBCamera* c);
	void set_vector(const RBVector3& v);

};

#include "Shader.h"
#include "GeometryStructures.h"
#include "RBMath\inc\Color.h"
#include "Primitive.h"

class OLPrimitiveCreator
{
public:
	struct PrimitiveParam
	{
		float shade_rate;
		OLShader* surface_shader;
		OLShader* displacement_shader;
		RBMatrix transform;
		RBColorf color;
		PrimitiveParam():shade_rate(1.0),surface_shader(nullptr),displacement_shader(nullptr),transform(RBMatrix::identity){}
	};

	struct SphereParam : public PrimitiveParam
	{
		float radius;
		float theta;
		float zmin;
		float zmax;
	};

	struct PanelParam : public PrimitiveParam
	{
		float w;
		float l;
		float h;
	};

	struct DiskParam: public PrimitiveParam
	{
		float r;
		float theta;
		float height;
	};

	struct CylinderParam :public PrimitiveParam
	{
		float zmax;
		float zmin;
		float theta;
		float radius;
	};

	struct ConeParam : public PrimitiveParam
	{
		float height;
		float theta;
		float radius;
	};

	struct ParaboloidParam : public PrimitiveParam
	{
		float zmax;
		float zmin;
		float theta;
		float rmax;
	};

	struct TorusParam : public PrimitiveParam
	{
		float phimin;
		float phimax;
		float theta;
		float majorradius;
		float minorradius;
	};

	struct HyperboloidParam : public PrimitiveParam
	{
		float theta;
		RBVector3 point2;
		RBVector3 point1;
	};

	struct BilinearPatchesParam : public PrimitiveParam
	{
		RBVector3 hs[4];
	};

	struct TriangleParam : public PrimitiveParam
	{
		RBVector3 p[3];
	};

	static OLPrimitive* create_sphere(SphereParam param);
	static OLPrimitive* create_panel(PanelParam param);
	static OLPrimitive* create_disk(DiskParam param);
	static OLPrimitive* create_cylinder(CylinderParam param);
	static OLPrimitive* create_cone(ConeParam param);
	static OLPrimitive* create_paraboloid(ParaboloidParam param);
	static OLPrimitive* create_torus(TorusParam param);
	static OLPrimitive* create_hyperboloid(HyperboloidParam param);
	static OLPrimitive* create_bilinearpatches(BilinearPatchesParam param);
	static OLTriangle* create_triangle(TriangleParam param);
};

class OLLightCreator
{
public:


};