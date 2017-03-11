#pragma once
#include "RBMath.h"
#include <string>
#include <windows.h>
#include <mutex>
#include <cassert>
#include <vector>

struct RBProccessMutex
{
	RBProccessMutex() :_mutex(NULL){}
	~RBProccessMutex(){}

	void create(const char* name);
	void open(const char* name);
	void destroy();
	void lock();
	void unlock();


private:
	std::string name;
	HANDLE _mutex;

	RBProccessMutex(const RBProccessMutex&)=delete;
	void operator=(const RBProccessMutex&)=delete;
};

struct RBProcessMutexGuard
{
	RBProcessMutexGuard(RBProccessMutex& mut) : _mutex(mut)
	{
		_mutex.lock();
	}
	~RBProcessMutexGuard()
	{
		_mutex.unlock();
	}

private:
	RBProccessMutex& _mutex;
};



struct RBMessageEntryHead
{
	RBMessageEntryHead() : type(0){}
	enum MsgTypes
	{
		E_I4 = 0,
		E_I5
	};
	char type;

};

struct RBMessageQueueHead 
{
	RBMessageQueueHead() : head(0), tail(0){}
	int head;
	int tail;
};

class RBMessageQueue
{
public:
	RBMessageQueue(){}
	~RBMessageQueue(){}

	void init(char* buffer, int buffer_size, int msg_size);
	void ter();
	bool is_empty();
	bool is_full();
	int get_size();
	bool enqueue(const RBMessageEntryHead& msg);
	bool dequeue(RBMessageEntryHead& msg);
	

private:
	std::vector<RBMessageEntryHead*> _entries;
	RBMessageQueueHead* _mq_head;
	int message_num;
	int message_size;

	RBMessageQueue(const RBMessageQueue&) = delete;
	void operator=(const RBMessageQueue&) = delete;

};

class RBMessageQueueBuffer
{
public:
	RBMessageQueueBuffer() :file_mapping(0), buffer(NULL), size(0){}
	~RBMessageQueueBuffer(){}
	
	void create(const char* name, int size);
	void open(const char* name, int size);
	void  destroy();

	inline char* get_buffer()
	{
		return buffer;
	}

	inline int get_buffer_size()
	{
		return size;
	}


private:
	RBMessageQueueBuffer(const RBMessageQueueBuffer&)=delete;
	void operator=(const RBMessageQueueBuffer&)=delete;


	std::string name;
	HANDLE file_mapping;
	char* buffer;
	int size;
};

class RBMessageQueueIPC
{
public:
	RBMessageQueueIPC(){}
	~RBMessageQueueIPC(){}

	void init(const char* name, bool create, int msg_max_num, int msg_max_size);
	void ter();
	bool is_empty();
	bool is_full();
	int get_size();
	bool enqueue(const RBMessageEntryHead& msg);
	bool dequeue(RBMessageEntryHead& msg);


private:
	std::string name;

	RBProccessMutex _mut;
	RBMessageQueueBuffer _buffer;
	RBMessageQueue _queue;


	RBMessageQueueIPC(const RBMessageQueueIPC&) = delete;
	void operator=(const RBMessageQueueIPC&) = delete;
};

class RBProccess
{
public:

};

