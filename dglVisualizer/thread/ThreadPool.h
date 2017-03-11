#pragma once

#include <iostream>
#include <functional>
#include <thread>
#include <condition_variable>
#include <future>
#include <atomic>
#include <vector>
#include <queue>

class RBSem
{
public:
	RBSem(int init_c = 0) :count(init_c) {}
	//wait
	void wait()
	{
		std::unique_lock<std::mutex> lock(mx);
		if (--count < 0)
		{
			cv.wait(lock, [&]()->bool {return count >= 0; });
		}
	}
	//signal
	void signal()
	{
		std::lock_guard<std::mutex> lock(mx);
		++count;
		cv.notify_one();

	}

private:
	int count;
	std::condition_variable cv;
	std::mutex mx;

};

class RBTaskExecutor
{
private:

	typedef std::function<void() > Task;

	std::vector<std::thread> pool;
	std::queue<Task> tasks;
	std::mutex m_task;
	std::condition_variable cv_task;
	std::atomic<bool> stop;
	std::atomic<int> runned;

public:
	RBTaskExecutor(size_t size = 4) : stop(false), runned(0)
	{
		size = size < 1 ? 1 : size;
		for(size_t i=0;i<size;++i)
		{
			pool.emplace_back(&RBTaskExecutor::schedual,this);
		}
	}
	~RBTaskExecutor()
	{
		for(std::thread& thread : pool)
		{
			thread.detach();
		}
	}

	void shutdown()
	{
		this->stop.store(true);
	}

	void restart()
	{
		this->stop.store(false);
	}

	template<class F,class... Args>
	auto commit(F&& f,Args&&... args)->std::future<decltype(f(args...))>
	{
		/*
		if(stop.load())
		{
			return std::future<void>();
		}
		*/

		typedef decltype(f(args...)) ResT;
		auto task = std::make_shared<std::packaged_task<ResT()> >(
			std::bind(std::forward<F>(f),std::forward<Args>(args)...)
			);
		{
			std::lock_guard<std::mutex> lock(m_task);
			tasks.emplace([task](){(*task)();});
		}

		cv_task.notify_all();

		std::future<ResT> fut = task->get_future();
		return fut;

	}

	void join()
	{
		for (std::thread& thread : pool)
		{
			thread.join();
		}
	}

private:
	Task get_one_task()
	{
		std::unique_lock<std::mutex> lock(m_task);
		cv_task.wait(lock,[this]()
		{
			return !tasks.empty();
		}
		);
		Task task(std::move(tasks.front()));
		tasks.pop();
		return task;
	}

	void schedual()
	{
		while (true)
		{
			if(Task task = get_one_task())
			{
				
				task();
				runned++;
			}
			else
				return;
		}
	}
};