#ifndef _THREAD_POOL_H_
#define _THREAD_POOL_H_

#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>

#include "task.h"

class ThreadPool
{
	public:
		ThreadPool(unsigned numThreads);
		~ThreadPool();
		void enqueueTask(Task *task);

	protected:
		void threadRun();

		std::vector<std::thread> threads;

		std::queue<Task *> tasks;
		std::condition_variable waitCondition;
		std::mutex waitMutex;
};

#endif /* !_THREAD_POOL_H_ */