#include <process/messageQueue/dgMessageQueue.h>
#include <cassert>

dgMessageQueue::dgMessageQueue()
{

}

dgMessageQueue::~dgMessageQueue()
{

}

void dgMessageQueue::init(char* buffer, int buffer_size, int msg_size)
{
	assert(buffer);
	assert(buffer_size > 0);
	assert(msg_size > 0);

	int queue_head_size = sizeof(dgMessageQueueHead);

	// at least enough for one message.
	assert(buffer_size > (queue_head_size + msg_size));

	int total_msg_size = buffer_size - queue_head_size;
	int total_msg_num = total_msg_size / msg_size;

	mqHead = (dgMessageQueueHead*)buffer;
	entries.resize(total_msg_num);

	buffer += queue_head_size;
	for (int i = 0; i < total_msg_num; i++) {
		dgMessageEntryHead* msg = (dgMessageEntryHead*)buffer;
		entries[i] = msg;
		buffer += msg_size;
	}

	messageNum = total_msg_num;
	messageSize = msg_size;
}

void dgMessageQueue::uninit()
{
	entries.clear();
	messageNum = 0;
	messageSize = 0;
	mqHead = NULL;
}

bool dgMessageQueue::isEmpty()
{
	assert(mqHead);
	return mqHead->tail == mqHead->head;
}

bool dgMessageQueue::isFull()
{
	assert(mqHead);
	assert(messageNum > 0);

	int tail = mqHead->tail;
	int head = mqHead->head;
	return (tail + 1) % messageNum == head;
}

int dgMessageQueue::getSize()
{
	assert(mqHead);
	assert(messageNum > 0);

	int tail = mqHead->tail;
	int head = mqHead->head;
	return (tail - head + messageNum) % messageNum;
}

bool dgMessageQueue::enqueue(const dgMessageEntryHead& msg)
{
	assert(mqHead);
	assert(messageNum > 0);

	if (isFull()) {
		return false;
	}
	
	int tail = mqHead->tail;
	dgMessageEntryHead* dst = entries[tail];

	const char* src_buffer = (const char*)&msg;
	char* dst_buffer = (char*)dst;
	memcpy(dst_buffer, src_buffer, messageSize);

	tail = (tail + 1) % messageNum;
	mqHead->tail = tail;

	return true;
}

bool dgMessageQueue::dequeue(dgMessageEntryHead& msg)
{
	assert(mqHead);
	assert(messageNum > 0);

	if (isEmpty()) {
		return false;
	}

	int head = mqHead->head;
	dgMessageEntryHead* src = entries[head];

	const char* src_buffer = (const char*)src;
	char* dst_buffer = (char*)&msg;
	memcpy(dst_buffer, src_buffer, messageSize);

	head = (head + 1) % messageNum;
    mqHead->head = head;

	return true;
}
