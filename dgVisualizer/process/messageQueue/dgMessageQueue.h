#pragma once

#include <vector>

struct dgMessageEntryHead
{
    dgMessageEntryHead() :type(0) {}

	int type;
};

typedef std::vector<dgMessageEntryHead*> MessageEntryVector;

struct dgMessageQueueHead
{
    dgMessageQueueHead() :head(0), tail(0) {}
	int head;
	int tail;
};

class dgMessageQueue
{
public:
	dgMessageQueue();
	~dgMessageQueue();

	void init(char* buffer, int buffer_size, int msg_size);
	void uninit();

	bool isEmpty();
	bool isFull();
	int  getSize();

	bool enqueue(const dgMessageEntryHead& msg);
	bool dequeue(dgMessageEntryHead& msg);

private:
	MessageEntryVector entries;
	dgMessageQueueHead* mqHead;
	int messageNum;
	int messageSize;

private:
	dgMessageQueue(const dgMessageQueue&);
	void operator=(const dgMessageQueue&);
};