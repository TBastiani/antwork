#include <functional>

#include "thread_pool.h"
#include "exception.h"

using namespace std;

void
ThreadPool::threadRun()
{
	for (;;)
	{
		Task *task = NULL;
		{
			unique_lock<mutex> lock(waitMutex);

			queue<Task *> &taskQueue = tasks;
			waitCondition.wait(
					lock,
					[&taskQueue]() -> bool
					{ return taskQueue.empty() == false; });

			if ((task = tasks.front()) != NULL)
				tasks.pop();
		}

		if (task == NULL)
			break;
		task->run(*this);
	}
}

ThreadPool::ThreadPool(unsigned numThreads):
	threads(),
	tasks(),
	waitCondition(),
	waitMutex()
{
	REQUIRE_CONDITION(numThreads != 0, "Can't build a zero thread pool");

	/* Spawn threads */
	for (unsigned tid = 0; tid < numThreads; tid++)
		threads.push_back(move(thread(&ThreadPool::threadRun, this)));
}

ThreadPool::~ThreadPool()
{
	/* Enqueue and broadcast finish task */
	{
		unique_lock<mutex> lock(waitMutex);

		tasks.push(NULL);
		waitCondition.notify_all();
	}

	/* Join threads */
	for (unsigned tid = 0; tid < threads.size(); tid++)
		threads[tid].join();
}

void
ThreadPool::enqueueTask(Task *task)
{
	unique_lock<mutex> lock(waitMutex);

	tasks.push(task);
	waitCondition.notify_one();
}

