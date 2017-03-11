#include <process/messageQueue/dglMessageQueue.h>
#include <cassert>

dglMessageQueue::dglMessageQueue()
{

}

dglMessageQueue::~dglMessageQueue()
{

}

void dglMessageQueue::init(char* buffer, int buffer_size, int msg_size)
{
	assert(buffer);
	assert(buffer_size > 0);
	assert(msg_size > 0);

	int queue_head_size = sizeof(dglMessageQueueHead);

	// at least enough for one message.
	assert(buffer_size > (queue_head_size + msg_size));

	int total_msg_size = buffer_size - queue_head_size;
	int total_msg_num = total_msg_size / msg_size;

	mqHead = (dglMessageQueueHead*)buffer;
	entries.resize(total_msg_num);

	buffer += queue_head_size;
	for (int i = 0; i < total_msg_num; i++) {
		dglMessageEntryHead* msg = (dglMessageEntryHead*)buffer;
		entries[i] = msg;
		buffer += msg_size;
	}

	messageNum = total_msg_num;
	messageSize = msg_size;
}

void dglMessageQueue::uninit()
{
	entries.clear();
	messageNum = 0;
	messageSize = 0;
	mqHead = NULL;
}

bool dglMessageQueue::isEmpty()
{
	assert(mqHead);
	return mqHead->tail == mqHead->head;
}

bool dglMessageQueue::isFull()
{
	assert(mqHead);
	assert(messageNum > 0);

	int tail = mqHead->tail;
	int head = mqHead->head;
	return (tail + 1) % messageNum == head;
}

int dglMessageQueue::getSize()
{
	assert(mqHead);
	assert(messageNum > 0);

	int tail = mqHead->tail;
	int head = mqHead->head;
	return (tail - head + messageNum) % messageNum;
}

bool dglMessageQueue::enqueue(const dglMessageEntryHead& msg)
{
	assert(mqHead);
	assert(messageNum > 0);

	if (isFull()) {
		return false;
	}
	
	int tail = mqHead->tail;
	dglMessageEntryHead* dst = entries[tail];

	const char* src_buffer = (const char*)&msg;
	char* dst_buffer = (char*)dst;
	memcpy(dst_buffer, src_buffer, messageSize);

	tail = (tail + 1) % messageNum;
	mqHead->tail = tail;

	return true;
}

bool dglMessageQueue::dequeue(dglMessageEntryHead& msg)
{
	assert(mqHead);
	assert(messageNum > 0);

	if (isEmpty()) {
		return false;
	}

	int head = mqHead->head;
	dglMessageEntryHead* src = entries[head];

	const char* src_buffer = (const char*)src;
	char* dst_buffer = (char*)&msg;
	memcpy(dst_buffer, src_buffer, messageSize);

	head = (head + 1) % messageNum;
    mqHead->head = head;

	return true;
}
