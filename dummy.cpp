#include <memory>

#include "task.h"
#include "thread_pool.h"
#include "timer.h"
#include "barrier.h"
#include "stdio.h"
#include "exception.h"

using namespace std;

void dummyRun(size_t seconds)
{
	sleep(seconds / (25.0 * 80.0));
}

static Task *createDummyTask(size_t seconds)
{
	size_t &sec = seconds;
	return new Task([sec]() -> void
			{ dummyRun(sec); });
}

int main(int argc, char **argv)
{
	unsigned numThreads = 2;
	if (argc == 2)
		numThreads = atoi(argv[1]);

	ThreadPool threadPool(numThreads);

	shared_ptr<Task> rootTask(createDummyTask(0));
	shared_ptr<Task> joinTask(rootTask);
	for (unsigned levels = 0; levels < 4; levels++)
	{
		shared_ptr<Task> tasks[20];
		shared_ptr<Task> previousJoinTask(joinTask);
		joinTask = shared_ptr<Task>(createDummyTask(0));
		for (unsigned id = 0; id < 20; id++)
		{
			tasks[id] = shared_ptr<Task>(createDummyTask(1));
			previousJoinTask->addChild(tasks[id]);
			tasks[id]->addChild(joinTask);
		}
	}

	Task *finalJoinTask;
	Barrier barrier(&finalJoinTask);
	joinTask->addChild(shared_ptr<Task>(finalJoinTask));

	for (unsigned run = 0; run < 500; run++)
	{
		barrier.reset();
		rootTask->run(threadPool);
		barrier.wait();
		DEBUG_MESSAGE("All tasks have been completed");
	}

	return 0;
}