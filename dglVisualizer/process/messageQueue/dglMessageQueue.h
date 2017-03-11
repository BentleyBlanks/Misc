#pragma once

#include <vector>

struct dglMessageEntryHead
{
    dglMessageEntryHead() :type(0) {}

	int type;
};

typedef std::vector<dglMessageEntryHead*> MessageEntryVector;

struct dglMessageQueueHead
{
    dglMessageQueueHead() :head(0), tail(0) {}
	int head;
	int tail;
};

class dglMessageQueue
{
public:
	dglMessageQueue();
	~dglMessageQueue();

	void init(char* buffer, int buffer_size, int msg_size);
	void uninit();

	bool isEmpty();
	bool isFull();
	int  getSize();

	bool enqueue(const dglMessageEntryHead& msg);
	bool dequeue(dglMessageEntryHead& msg);

private:
	MessageEntryVector entries;
	dglMessageQueueHead* mqHead;
	int messageNum;
	int messageSize;

private:
	dglMessageQueue(const dglMessageQueue&);
	void operator=(const dglMessageQueue&);
};