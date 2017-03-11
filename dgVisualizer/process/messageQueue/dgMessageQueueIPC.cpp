#include <process/messageQueue/dgMessageQueueIPC.h>

#include <cassert>

void dgMutex::create(const wchar_t* name)
{
	mutex = ::CreateMutex(NULL, false, name);
	assert(mutex);

	name = name;
}

void dgMutex::open(const wchar_t* name)
{
	mutex = ::OpenMutex(SYNCHRONIZE, false, name);
	assert(mutex);

	name = name;
}

void dgMutex::destroy()
{
	if (mutex) {
		::CloseHandle(mutex);
		mutex = NULL;
	}
	name.clear();
}

void dgMutex::lock()
{
	if (mutex) {
		::WaitForSingleObject(mutex, INFINITE);
	}
}

void dgMutex::unlock()
{
	if (mutex) {
		::ReleaseMutex(mutex);
	}
}


void MessageQueueBuffer::create(const wchar_t* name, int size)
{
    // 创建共享内存块
	fileMapping = ::CreateFileMapping(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0, size,
		name);
	
	assert(fileMapping);

    // 获取共享内存指针
	buffer = (char*)::MapViewOfFile(fileMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	assert(buffer);

	size   = size;
	name    = name;
}

void MessageQueueBuffer::open(const wchar_t* name, int size)
{
	fileMapping = ::OpenFileMapping(FILE_MAP_ALL_ACCESS, false, name);
	assert(fileMapping);

	buffer = (char*)::MapViewOfFile(fileMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	assert(buffer);

	size = size;
	name = name;
}

void MessageQueueBuffer::destroy()
{
	if (buffer) {
		::UnmapViewOfFile(buffer);
		buffer = NULL;
	}
	if (fileMapping) {
		CloseHandle(fileMapping);
		fileMapping = NULL;
	}
	size = 0;
	name.clear();
}

char* MessageQueueBuffer::getBuffer()
{
	return buffer;
}

int MessageQueueBuffer::getBufferSize()
{
	return size;
}

void dgMessageQueueIPC::init(const wchar_t* _name, bool create, int msg_max_num, int msg_max_size)
{
	std::wstring mq_name(_name);
	std::wstring mq_lock_name = mq_name + std::wstring(L"_lock");

    // 消息取最大跨距做单一消息内存分配
	int buffer_size = sizeof(dgMessageQueueHead) + (msg_max_num+1) * msg_max_size;
	if (!create) {
		buffer.open(mq_name.c_str(), buffer_size);
		mutex.open(mq_lock_name.c_str());
	}
	else {
		buffer.create(mq_name.c_str(), buffer_size);
		mutex.create(mq_lock_name.c_str());
	}
	queue.init(buffer.getBuffer(), buffer_size, msg_max_size);

	this->name = mq_name;
}

void dgMessageQueueIPC::uninit()
{
	queue.uninit();
	buffer.destroy();
	mutex.destroy();
}

bool dgMessageQueueIPC::isEmpty()
{
	dgMutexGuard guard(mutex);

	return queue.isEmpty();
}

bool dgMessageQueueIPC::isFull()
{
	dgMutexGuard guard(mutex);

	return queue.isFull();
}

int dgMessageQueueIPC::getSize()
{
	dgMutexGuard guard(mutex);

	return queue.getSize();
}

bool dgMessageQueueIPC::enqueue(const dgMessageEntryHead& msg)
{
	dgMutexGuard guard(mutex);

	return queue.enqueue(msg);
}

bool dgMessageQueueIPC::dequeue(dgMessageEntryHead& msg)
{
	dgMutexGuard guard(mutex);

	return queue.dequeue(msg);
}
