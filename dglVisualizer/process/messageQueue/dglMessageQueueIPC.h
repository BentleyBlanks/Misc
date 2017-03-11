#pragma once

#include <string>
#include <windows.h>
#include <process/messageQueue/messageQueue.h>

struct dglMutex
{
public:
	dglMutex():mutex(NULL){}
	~dglMutex() {}

	void create(const wchar_t* name);
	void open(const wchar_t* name);

	void destroy();

	void lock();
	void unlock();
public:
	std::wstring name;
	HANDLE mutex = NULL;

private:
	dglMutex(const dglMutex&);
	void operator=(const dglMutex&);
};


struct dglMutexGuard
{
public:
	dglMutexGuard(dglMutex& mutex) : mutex(mutex) {
		mutex.lock();
	}

	~dglMutexGuard() {
		mutex.unlock();
	}
public:
	dglMutex& mutex;
};


struct MessageQueueBuffer
{
public:
	MessageQueueBuffer():fileMapping(0), buffer(NULL), size(0) {}
	~MessageQueueBuffer() {}

	void create(const wchar_t* name, int size);
	void open(const wchar_t* name, int size);

	void destroy();

	char* getBuffer();
	int getBufferSize();

public:
	std::wstring name;
	HANDLE fileMapping;
	char* buffer;
	int size;

private:
	MessageQueueBuffer(const MessageQueueBuffer&);
	void operator=(const MessageQueueBuffer&);
};

class dglMessageQueueIPC
{
public:
	dglMessageQueueIPC(){}
	~dglMessageQueueIPC(){}

	void init(const wchar_t* name, bool create, int msgMaxNum, int msgMaxSize);
	void uninit();

	bool isEmpty();
	bool isFull();
	int  getSize();

	bool enqueue(const dglMessageEntryHead& msg);
	bool dequeue(dglMessageEntryHead& msg);

private:
	std::wstring name;
	dglMutex mutex;
	MessageQueueBuffer buffer;
	dglMessageQueue queue;

private:
	dglMessageQueueIPC(const dglMessageQueueIPC&);
	void operator=(const dglMessageQueueIPC&);
};