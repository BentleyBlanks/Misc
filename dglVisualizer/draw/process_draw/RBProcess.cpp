#include "RBProcess.h"

void RBProccessMutex::create(const char* name)
{
	_mutex = ::CreateMutexA(NULL, false, name);
	assert(_mutex);

	this->name = name;
}

void RBProccessMutex::open(const char* name)
{
	_mutex = ::OpenMutexA(SYNCHRONIZE, false, name);
	assert(_mutex);

	this->name = name;
}

void RBProccessMutex::destroy()
{
	if (_mutex)
	{
		::CloseHandle(_mutex);
		_mutex = NULL;
	}
	name.clear();
}

void RBProccessMutex::lock()
{
	if (_mutex)
	{
		::WaitForSingleObject(_mutex, INFINITE);
	}
}

void RBProccessMutex::unlock()
{
	if (_mutex)
	{
		::ReleaseMutex(_mutex);
	}
}





void RBMessageQueue::init(char* buffer, int buffer_size, int msg_size)
{
	assert(buffer);
	assert(buffer_size > 0);
	assert(msg_size > 0);

	int queue_head_size = sizeof(RBMessageQueueHead);

	//printf("buffer_size:%d\nqueue_head_size:%d\nmessage_size\n%d\n", buffer_size, queue_head_size, msg_size);
	//至少一条消息长
	assert(buffer_size > (queue_head_size + msg_size));
	


	int total_msg_size = buffer_size - queue_head_size;
	int total_msg_num = total_msg_size / msg_size;

	_mq_head = (RBMessageQueueHead*)buffer;
	_entries.resize(total_msg_num);

	buffer += queue_head_size;
	for (int i = 0; i < total_msg_num; ++i)
	{
		RBMessageEntryHead* msg = (RBMessageEntryHead*)buffer;
		_entries[i] = msg;
		buffer += msg_size;
	}

	message_num = total_msg_num;
	message_size = msg_size;
}

void RBMessageQueue::ter()
{
	_entries.clear();
	message_num = 0;
	message_size = 0;
	_mq_head = NULL;
}

bool RBMessageQueue::is_empty()
{
	assert(_mq_head);
	return _mq_head->tail == _mq_head->head;
}

bool RBMessageQueue::is_full()
{
	assert(_mq_head);
	assert(message_num > 0);

	int tail = _mq_head->tail;
	int head = _mq_head->head;
	return (tail + 1) % message_num == head;
}

int RBMessageQueue::get_size()
{
	assert(_mq_head);
	assert(message_num > 0);

	int tail = _mq_head->tail;
	int head = _mq_head->head;
	return (tail - head + message_num) % message_num;
}

bool RBMessageQueue::enqueue(const RBMessageEntryHead& msg)
{
	assert(_mq_head);
	assert(message_num > 0);

	if (is_full()) {
		return false;
	}

	int tail = _mq_head->tail;
	RBMessageEntryHead* dst = _entries[tail];

	const char* src_buffer = (const char*)&msg;
	char* dst_buffer = (char*)dst;
	memcpy(dst_buffer, src_buffer, message_size);

	tail = (tail + 1) % message_num;
	_mq_head->tail = tail;

	return true;
}

bool RBMessageQueue::dequeue(RBMessageEntryHead& msg)
{

	assert(_mq_head);
	assert(message_num > 0);

	if (is_empty()) {
		return false;
	}

	int head = _mq_head->head;
	RBMessageEntryHead* src = _entries[head];

	const char* src_buffer = (const char*)src;
	char* dst_buffer = (char*)&msg;
	memcpy(dst_buffer, src_buffer, message_size);

	head = (head + 1) % message_num;
	_mq_head->head = head;

	return true;
}








void RBMessageQueueBuffer::create(const char* name, int size)
{
	file_mapping = ::CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, size, name);
	assert(file_mapping);

	buffer = (char*)::MapViewOfFile(file_mapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	assert(buffer);

	this->size = size;
	this->name = name;
}

void RBMessageQueueBuffer::open(const char* name, int size)
{
	file_mapping = ::OpenFileMappingA(FILE_MAP_ALL_ACCESS, false, name);
	assert(file_mapping);

	buffer = (char*)::MapViewOfFile(file_mapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	assert(buffer);

	this->size = size;
	this->name = name;
}

void RBMessageQueueBuffer::destroy()
{
	if (buffer) {
		::UnmapViewOfFile(buffer);
		buffer = NULL;
	}
	if (file_mapping) {
		CloseHandle(file_mapping);
		file_mapping = NULL;
	}
	size = 0;
	name.clear();
}

void RBMessageQueueIPC::init(const char* name, bool create, int msg_max_num, int msg_max_size)
{
	std::string mq_name(name);
	std::string mq_lock_name = mq_name + std::string("_lock");

	int buffer_size = sizeof(RBMessageQueueHead) + (msg_max_num + 1) * msg_max_size;
	if (!create) {
		_buffer.open(mq_name.c_str(), buffer_size);
		_mut.open(mq_lock_name.c_str());
	}
	else {
		_buffer.create(mq_name.c_str(), buffer_size);
		_mut.create(mq_lock_name.c_str());
	}
	_queue.init(_buffer.get_buffer(), buffer_size, msg_max_size);

	this->name = mq_name;
}

void RBMessageQueueIPC::ter()
{
	_queue.ter();
	_buffer.destroy();
	_mut.destroy();
}

bool RBMessageQueueIPC::is_empty()
{
	RBProcessMutexGuard gurad(_mut);
	return _queue.is_empty();
}

bool RBMessageQueueIPC::is_full()
{
	RBProcessMutexGuard guard(_mut);
	return _queue.is_full();
}

int RBMessageQueueIPC::get_size()
{
	RBProcessMutexGuard gurad(_mut);
	return _queue.get_size();
}

bool RBMessageQueueIPC::enqueue(const RBMessageEntryHead& msg)
{
	RBProcessMutexGuard gurad(_mut);
	return _queue.enqueue(msg);
}

bool RBMessageQueueIPC::dequeue(RBMessageEntryHead& msg)
{
	RBProcessMutexGuard gurad(_mut);
	return _queue.dequeue(msg);
}










