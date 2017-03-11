#include "sender.h"
#include "Vector2.h"
#include <string>


void OLNetDebugger::init()
{
	_ipc.init("TracerDebugger", false, 102400, 64);

	//create the socket and set to send to 127.0.0.1:11999
	udpConnection.Create();
	udpConnection.Connect("127.0.0.1",11999);
	udpConnection.SetNonBlocking(true);


	used = true;
}

void OLNetDebugger::clear_point()
{
	if(!used) return;
	ps.clear();
}

void OLNetDebugger::collecte_point(const RBVector3& v)
{
	if(!used) return;
	ps.push_back(v);
}

void OLNetDebugger::send()
{
	/*
	if(!used) return;
	std::string message="";
	//printf("%d\n",ps.size());
	for(unsigned int i=0; i<ps.size(); i++){
		message+=ofToString(ps[i].x)+"|"+ofToString(ps[i].y)+"|"+ofToString(ps[i].z)+"[/p]";
	}
	udpConnection.Send(message.c_str(),message.length());
	*/
}

void OLNetDebugger::disable()
{
	used = false;
}

void OLNetDebugger::enable()
{
	used = true;
}

void OLNetDebugger::set_clear()
{
	if(!used) return;
	/*
	std::string message="c";
	udpConnection.Send(message.c_str(),message.length());
	*/

	char mem[1] = { OLNetDebugger::MSGTypes::E_CLEAR };
	RBMessageEntryHead* mh = (RBMessageEntryHead*)mem;

	_ipc.enqueue(*mh);

}

void OLNetDebugger::set_a_point(const RBVector3& v)
{
	collecte_point(v);
	send();
	clear_point();
}

void OLNetDebugger::set_a_sspoint(const RBVector2& v)
{
	
	if(!used) return;
	//std::string message="s";
	//printf("%d\n",ps.size());
	//message+=ofToString(v.x)+"|"+ofToString(v.y)+"[/p]";

	//udpConnection.Send(message.c_str(),message.length());

	char mem[9] = { OLNetDebugger::MSGTypes::E_SSPOINT };
	memcpy(mem+1,&v,sizeof(v));
	_ipc.enqueue(*(RBMessageEntryHead*)mem);
}

void OLNetDebugger::draw_line(const RBVector3& v1,const RBVector3& v2)
{
	if(!used) return;
	//std::string message="";
	//printf("%d\n",ps.size());
	/*
	message+= ofToString(v1.x)+"|"+ofToString(v1.y)+"|"+ofToString(v1.z)+"|"+
		ofToString(v2.x)+"|"+ofToString(v2.y)+"|"+ofToString(v2.z)+"[/p]";
		
	udpConnection.Send(message.c_str(),message.length());
	*/
	char mem[25] = { OLNetDebugger::MSGTypes::E_DRAWLINE };
	memcpy(mem + 1, &v1, sizeof(v1));
	memcpy(mem+13,&v2,sizeof(v2));
	_ipc.enqueue(*(RBMessageEntryHead*)mem);
}

void OLNetDebugger::draw_sphere(const RBVector3& o,float r)
{
	if(!used) return;
	//std::string message="";
	/*
	message+= ofToString(o.x)+"|"+ofToString(o.y)+"|"+ofToString(o.z)+"|"+
		ofToString(r)+"[/p]";

	udpConnection.Send(message.c_str(),message.length());
	*/
	char mem[17] = { OLNetDebugger::MSGTypes::E_DRAWSPHERE };
	memcpy(mem + 1, &o, sizeof(o));
	memcpy(mem + 13, &r, sizeof(r));
	_ipc.enqueue(*(RBMessageEntryHead*)mem);
}

void OLNetDebugger::draw_tri(const RBVector3& v1,const RBVector3& v2,const RBVector3& v3)
{
	if(!used) return;
	//std::string message="";
	//printf("%d\n",ps.size());

	/*
	message+= ofToString(v1.x)+"|"+ofToString(v1.y)+"|"+ofToString(v1.z)+"|"+
		ofToString(v2.x)+"|"+ofToString(v2.y)+"|"+ofToString(v2.z)+ "|"+
		ofToString(v3.x)+"|"+ofToString(v3.y)+"|"+ofToString(v3.z)+"[/p]";

	udpConnection.Send(message.c_str(),message.length());
	*/
	char mem[37] = { OLNetDebugger::MSGTypes::E_DRAWTRI };
	memcpy(mem + 1, &v1, sizeof(v1));
	memcpy(mem + 13,&v2,sizeof(v2));
	memcpy(mem+25,&v3,sizeof(v3));
	_ipc.enqueue(*(RBMessageEntryHead*)mem);
}

void OLNetDebugger::send_number(int i)
{
	if(!used) return;
	//std::string message="i";
	/*
	message +=ofToString(i);
	udpConnection.Send(message.c_str(),message.length());
	*/
	char mem[5] = { OLNetDebugger::MSGTypes::E_SET_NUM };
	memcpy(mem + 1, &i, sizeof(i));
	_ipc.enqueue(*(RBMessageEntryHead*)mem);
}

