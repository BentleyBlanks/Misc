#include "Debug.h"
#include "Utilities.h"
#include "RBMath/Inc/Matrix.h"
#include "PipelineObjects.h"
#include "config.h"
#include "Primitive.h"

RBDebugDraw* debugdraw;

void RBDataCollector::put_int_int(int key,int val)
{
	_data_int_int[key] = val;
}

int RBDataCollector::get_int_int(int key)
{
	return _data_int_int[key];
}

void RBDataCollector::put_float(std::string name,float val)
{
	_data_float[name] = val;
}

void RBDataCollector::put_string(std::string name,std::string val)
{
	_data_string[name] = val;
}

void RBDataCollector::put_ptr(std::string name,void* ptr)
{
	_data_ptr[name] = ptr;
}

float RBDataCollector::get_float(std::string name)
{
	return _data_float[name];
}

std::string RBDataCollector::get_string(std::string name)
{
	return _data_string[name];
}

void* RBDataCollector::get_ptr(std::string name)
{
	return _data_ptr[name];
}

float& RBDataCollector::get_float_ref(std::string name)
{
	return _data_float[name];
}

std::string& RBDataCollector::get_string_ref(std::string name)
{
	return _data_string[name];
}

void*& RBDataCollector::get_ptr_ref(std::string name)
{
	return _data_ptr[name];
}

float RBDataCollector::get_float_avg()
{
	float s = 0.f;
	int n = 0;
	for (auto i : _data_float)
	{
		n++;
		s += i.second;
	}
	return s/n;
}

void RBDataCollector::output_floats()
{
	for(auto i : _data_float)
		printf("%s : %f  |",i.first.c_str(),i.second);
	printf("\n");
}

void RBDataCollector::output_strings()
{
	for(auto i : _data_string)
		printf("%s : %s  |",i.first.c_str(),i.second.c_str());
	printf("\n");
}

void RBDataCollector::output_ptrs()
{
	for(auto i : _data_ptr)
		printf("%s : %d  |",i.first.c_str(),i.second);
	printf("\n");
}

void RBDataCollector::output()
{
}

void RBDebugArrow::draw(RBCamera* c) 
{
	RBVector4 vs(v_s,1.f);
	RBVector4 ve(v_e,1.f);

	RBMatrix v;
	c->get_view_matrix(v);

	RBVector4 vve = v.transform_vector3(v_e);
	RBVector4 vvs = vs*v;
	RBMatrix m;
	c->get_perspective_matrix(m);
	RBVector4 tve = vve;
	tve.safe_normal3();
	tve.w = 1.f;
	tve+=vvs;
	tve.apply_matrix(m);
	tve/=tve.w;

	m=v*m;

	vs.apply_matrix(m);
	vs /= vs.w;
	ve.apply_matrix(m);
	ve /= ve.w;

	RBVector2 a = OLFramework::trasform_to_view_port(RBVector2(vs.x,vs.z),1280,720);
	RBVector2 b = OLFramework::trasform_to_view_port(RBVector2(tve.x,tve.z),1280,720);


	ofPushStyle();
	ofSetColor(ofColor::blue);
	draw_line(a.x,a.y,b.x,b.y);
	ofSetColor(ofColor::red);
	draw_point(b.x,b.y);
	ofPopStyle();


}

void RBDebugArrow::set_vector(const RBVector3& v)
{
	v_s = RBVector3::zero_vector;
	v_e = v;
}

OLPrimitive* OLPrimitiveCreator::create_sphere(SphereParam param)
{
	OLSphere* p = new OLSphere(param.radius,param.zmin,param.zmax,param.theta);
	p->attri = new OLAttributes();
	p->attri->shading_rate = param.shade_rate;
	p->attri->surface_shader = param.surface_shader;
	p->attri->displacement_shader = param.displacement_shader;
	p->attri->color = param.color;
	p->tran = param.transform;
	return p;
}

OLPrimitive* OLPrimitiveCreator::create_panel(PanelParam param)
{
	OLPanel* p = new OLPanel(param.w,param.l,param.h);
	p->attri = new OLAttributes();
	p->attri->shading_rate = param.shade_rate;
	p->attri->surface_shader = param.surface_shader;
	p->attri->displacement_shader = param.displacement_shader;
	p->attri->color = param.color;
	p->tran = param.transform;
	return p;
}

OLPrimitive* OLPrimitiveCreator::create_disk(DiskParam param)
{
	OLDisk* p = new OLDisk(param.r,param.theta,param.height);
	p->attri = new OLAttributes();
	p->attri->shading_rate = param.shade_rate;
	p->attri->surface_shader = param.surface_shader;
	p->attri->displacement_shader = param.displacement_shader;
	p->attri->color = param.color;
	p->tran = param.transform;
	return p;
}

OLPrimitive* OLPrimitiveCreator::create_cylinder(CylinderParam param)
{
	OLCylinder* p = new OLCylinder(param.radius,param.theta,param.zmax,param.zmin);
	p->attri = new OLAttributes();
	p->attri->shading_rate = param.shade_rate;
	p->attri->surface_shader = param.surface_shader;
	p->attri->displacement_shader = param.displacement_shader;
	p->attri->color = param.color;
	p->tran = param.transform;
	return p;
}

OLPrimitive* OLPrimitiveCreator::create_cone(ConeParam param)
{
	OLCone* p = new OLCone(param.radius,param.theta,param.height);
	p->attri = new OLAttributes();
	p->attri->shading_rate = param.shade_rate;
	p->attri->surface_shader = param.surface_shader;
	p->attri->displacement_shader = param.displacement_shader;
	p->attri->color = param.color;
	p->tran = param.transform;
	return p;
}

OLPrimitive* OLPrimitiveCreator::create_paraboloid(ParaboloidParam param)
{
	OLParaboloid* p = new OLParaboloid(param.rmax,param.theta,param.zmax,param.zmin);
	p->attri = new OLAttributes();
	p->attri->shading_rate = param.shade_rate;
	p->attri->surface_shader = param.surface_shader;
	p->attri->displacement_shader = param.displacement_shader;
	p->attri->color = param.color;
	p->tran = param.transform;
	return p;
}

OLPrimitive* OLPrimitiveCreator::create_torus(TorusParam param)
{
	OLTorus* p = new OLTorus(param.theta,param.phimax,param.phimin,param.majorradius,param.minorradius);
	p->attri = new OLAttributes();
	p->attri->shading_rate = param.shade_rate;
	p->attri->surface_shader = param.surface_shader;
	p->attri->displacement_shader = param.displacement_shader;
	p->attri->color = param.color;
	p->tran = param.transform;
	return p;
}

OLPrimitive* OLPrimitiveCreator::create_hyperboloid(HyperboloidParam param)
{
	OLHyperboloid* p = new OLHyperboloid(param.theta,param.point1,param.point2);
	p->attri = new OLAttributes();
	p->attri->shading_rate = param.shade_rate;
	p->attri->surface_shader = param.surface_shader;
	p->attri->displacement_shader = param.displacement_shader;
	p->attri->color = param.color;
	p->tran = param.transform;
	return p;
}

OLPrimitive* OLPrimitiveCreator::create_bilinearpatches(BilinearPatchesParam param)
{
	OLBilinearPatches* p = new OLBilinearPatches(param.hs[0],param.hs[1],param.hs[2],param.hs[3]);
	p->attri = new OLAttributes();
	p->attri->shading_rate = param.shade_rate;
	p->attri->surface_shader = param.surface_shader;
	p->attri->displacement_shader = param.displacement_shader;
	p->attri->color = param.color;
	p->tran = param.transform;
	return p;
}

OLTriangle* OLPrimitiveCreator::create_triangle(TriangleParam param)
{
	OLTriangle* p = new OLTriangle(param.p[0],param.p[1],param.p[2]);
	p->attri = new OLAttributes();
	p->attri->shading_rate = param.shade_rate;
	p->attri->surface_shader = param.surface_shader;
	p->attri->displacement_shader = param.displacement_shader;
	p->attri->color = param.color;
	p->tran = param.transform;
	return p;
}

void RBDebugDraw::draw_grid(ofColor c,bool fill, float x,float y)
{
	_mut.lock();
	Pak p(DRAWGRID,x,y);
	p.c = c;
	p.fill = fill;
	_queue.push(p);
	_mut.unlock();
}

void RBDebugDraw::draw_point(ofColor c,bool fill, float x,float y)
{
	_mut.lock();
	Pak p(DRAWPOINT,x,y);
	p.c = c;
	p.fill = fill;
	_queue.push(p);
	_mut.unlock();
}

void RBDebugDraw::draw_rect(ofColor c,bool fill, float x,float y,float w,float h)
{
	_mut.lock();
	Pak p(DRAWRECT,x,y,w,h);
	p.c = c;
	p.fill = fill;
	_queue.push(p);
	_mut.unlock();
}

void RBDebugDraw::draw_quad(ofColor c,bool fill, float x,float y,float x1,float y1,float x2,float y2,float x3,float y3)
{
	_mut.lock();
	Pak p(DRAWQUAD,x,y,x1,y1,x2,y2,x3,y3);
	p.c = c;
	p.fill = fill;
	_queue.push(p);
	_mut.unlock();
}

void RBDebugDraw::draw_line(ofColor c,bool fill, float x,float y,float x1,float y1)
{
	_mut.lock();
	Pak p(DRAWLINE,x,y,x1,y1);
	p.c = c;
	p.fill = fill;
	_queue.push(p);
	_mut.unlock();
}

void RBDebugDraw::draw_triangle(ofColor c,bool fill, float x,float y,float x1,float y1,float x2,float y2)
{
	_mut.lock();
	Pak p(DRAWTRIANGLE,x,y,x1,y1,x2,y2);
	p.c = c;
	p.fill = fill;
	_queue.push(p);
	_mut.unlock();
}

void RBDebugDraw::draw()
{
	ofPushStyle();
	while (!_queue.empty())
	{
		auto p = _queue.front();

		ofSetColor(p.c);
		if(p.fill)
			ofFill();
		else
			ofNoFill();

		switch (p.tp)
		{
		case DRAWGRID:
			::draw_grid(p.v[0],p.v[1]);
			break;
		case DRAWLINE:
			::draw_line(p.v[0],p.v[1],p.v[2],p.v[3]);
			break;
		case DRAWPOINT:
			::draw_point(p.v[0],p.v[1]);
			break;
		case DRAWQUAD:
			::draw_quad(p.v[0],p.v[1],p.v[2],p.v[3],p.v[4],p.v[5],p.v[6],p.v[7]);
			break;
		case DRAWRECT:
			::draw_rect(p.v[0],p.v[1],p.v[2],p.v[3]);
			break;
		case DRAWTRIANGLE:
			ofTriangle(ofPoint(
				p.v[0],
				p.v[1]),
				ofPoint(
				p.v[2],
				p.v[3]),
				ofPoint(
				p.v[4],
				p.v[5]));
			break;
		}
		_queue.pop();
	}
	ofPopStyle();
}
