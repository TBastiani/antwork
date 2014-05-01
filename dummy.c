#include "task.h"
#include "timer.h"
#include "stdio.h"

void dummyRun(void *argument)
{
	sleep((double) ((size_t) argument));
}

static task_t *createDummyTask(size_t seconds)
{
	return taskCreate(dummyRun, (void *) seconds);
}

int main()
{
	task_t *rootTask = createDummyTask(1);
	task_t *childTask1 = createDummyTask(2);
	task_t *childTask2 = createDummyTask(3);

	taskAddParent(childTask1, rootTask);
	taskAddParent(childTask2, rootTask);

	taskRun(rootTask);

	taskRelease(rootTask);
	return 0;
}