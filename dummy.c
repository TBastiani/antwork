#include "task.h"
#include "thread_pool.h"
#include "timer.h"
#include "barrier.h"
#include "stdio.h"
#include "exception.h"

void dummyRun(void *argument)
{
	sleep((double) ((size_t) argument));
}

static task_t *createDummyTask(size_t seconds)
{
	return taskCreate(dummyRun, (void *) seconds);
}

int main(int argc, char **argv)
{
	unsigned numThreads = 2;
	if (argc == 2)
		numThreads = atoi(argv[1]);

	thread_pool_t *threadPool = threadPoolCreate(numThreads);

	task_t *rootTask = createDummyTask(1);
	task_t *childTask1 = createDummyTask(1);
	task_t *childTask2 = createDummyTask(2);

	taskAddParent(childTask1, rootTask);
	taskAddParent(childTask2, rootTask);

	task_t *grandKid1 = createDummyTask(1);
	task_t *grandKid2 = createDummyTask(1);
	task_t *grandKid3 = createDummyTask(1);
	task_t *grandKid4 = createDummyTask(1);
	task_t *grandKid5 = createDummyTask(1);
	task_t *grandKid6 = createDummyTask(1);
	task_t *grandKid7 = createDummyTask(1);

	taskAddParent(grandKid1, childTask1);
	taskAddParent(grandKid2, childTask1);
	taskAddParent(grandKid3, childTask1);
	taskAddParent(grandKid4, childTask1);
	taskAddParent(grandKid4, childTask2);
	taskAddParent(grandKid5, childTask2);
	taskAddParent(grandKid6, childTask2);
	taskAddParent(grandKid7, childTask2);

	barrier_t barrier;
	task_t *joinTask = barrierCreate(&barrier);
	taskAddParent(joinTask, grandKid1);
	taskAddParent(joinTask, grandKid2);
	taskAddParent(joinTask, grandKid3);
	taskAddParent(joinTask, grandKid4);
	taskAddParent(joinTask, grandKid5);
	taskAddParent(joinTask, grandKid6);
	taskAddParent(joinTask, grandKid7);

	for (unsigned run = 0; run < 3; run++)
	{
		barrierReset(&barrier);
		taskRun(rootTask, threadPool);
		barrierWait(&barrier);
		DEBUG_MESSAGE("All tasks have been completed");
	}

	barrierDestroy(&barrier);
	threadPoolDestroy(threadPool);
	taskRelease(rootTask);
	return 0;
}