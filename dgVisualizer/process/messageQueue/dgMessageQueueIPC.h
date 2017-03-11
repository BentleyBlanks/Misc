#pragma once

#include <string>
#include <windows.h>
#include <process/messageQueue/messageQueue.h>

struct dgMutex
{
public:
	dgMutex():mutex(NULL){}
	~dgMutex() {}

	void create(const wchar_t* name);
	void open(const wchar_t* name);

	void destroy();

	void lock();
	void unlock();
public:
	std::wstring name;
	HANDLE mutex = NULL;

private:
	dgMutex(const dgMutex&);
	void operator=(const dgMutex&);
};


struct dgMutexGuard
{
public:
	dgMutexGuard(dgMutex& mutex) : mutex(mutex) {
		mutex.lock();
	}

	~dgMutexGuard() {
		mutex.unlock();
	}
public:
	dgMutex& mutex;
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

class dgMessageQueueIPC
{
public:
	dgMessageQueueIPC(){}
	~dgMessageQueueIPC(){}

	void init(const wchar_t* name, bool create, int msgMaxNum, int msgMaxSize);
	void uninit();

	bool isEmpty();
	bool isFull();
	int  getSize();

	bool enqueue(const dgMessageEntryHead& msg);
	bool dequeue(dgMessageEntryHead& msg);

private:
	std::wstring name;
	dgMutex mutex;
	MessageQueueBuffer buffer;
	dgMessageQueue queue;

private:
	dgMessageQueueIPC(const dgMessageQueueIPC&);
	void operator=(const dgMessageQueueIPC&);
};