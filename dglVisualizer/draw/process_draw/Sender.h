#pragma once

//#include "ofxNetwork.h"

class ofxUDPManager
{
public:
	void Create(){}
	void Connect(const char * ip,int port){}
	void SetNonBlocking(bool b){}
	void Send(const char* message,int len){}

};

#include "Vector3.h"
#include <vector>
#include "base/RBProcess.h"
#define RECONNECT_TIME 400




class OLNetDebugger
{
public:
	enum MSGTypes
	{
		E_CLEAR = 0,
		E_APOINT,
		E_SSPOINT,
		E_DRAWLINE,
		E_DRAWSPHERE,
		E_DRAWTRI,
		E_SET_NUM,
		E_TOTAL
	};

	static OLNetDebugger *get()
	{
		static OLNetDebugger* _ins = nullptr;
		if(!_ins)
			_ins = new OLNetDebugger();
		return _ins;
	}
	void init();
	void clear_point();
	void collecte_point(const RBVector3& v);
	void send();
	void disable();
	void enable();
	void set_clear();
	void set_a_point(const RBVector3& v);
	void set_a_sspoint(const RBVector2& v);
	void draw_line(const RBVector3& v1,const RBVector3& v2);
	void draw_sphere(const RBVector3& o,float r);
	void draw_tri(const RBVector3& v1,const RBVector3& v2,const RBVector3& v3 );
	void send_number(int  i);
	ofxUDPManager udpConnection;

	RBMessageQueueIPC _ipc;

	bool used;
	std::vector<RBVector3> ps;
};


