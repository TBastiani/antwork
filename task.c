#include <stdlib.h>
#include <stdatomic.h>

#include "exception.h"
#include "task.h"
#include "thread_pool.h"

task_t *taskCreate(
		void (*fnPtr)(void *),
		void *argument)
{
	task_t *task = (task_t *) calloc(1, sizeof(task_t));
	REQUIRE_CONDITION(task != NULL, "Memory allocation failed");

	task->workFnPtr = fnPtr;
	task->argument = argument;
	return task;
}

void taskAddParent(
		task_t *task,
		task_t *parentTask)
{
	if (parentTask->numChildren == 0)
		parentTask->children = (task_t **) malloc(sizeof(task_t *));
	else
		parentTask->children = (task_t **) realloc(parentTask->children, sizeof(task_t *) * (parentTask->numChildren + 1));

	REQUIRE_CONDITION(parentTask->children != NULL, "Memory allocation failed");
	parentTask->children[parentTask->numChildren] = task;
	parentTask->numChildren++;
	task->numParents++;
}

void taskRun(
		task_t *task,
		struct thread_pool *pool)
{
	atomic_store(&task->numParentsDone, 0);
	task->workFnPtr(task->argument);

	/* Update children */
	for (unsigned index = 0; index < task->numChildren; index++)
	{
		task_t *childTask = task->children[index];
		if (atomic_fetch_add(&childTask->numParentsDone, 1) == childTask->numParents - 1)
			threadPoolAddTask(pool, childTask);
	}
}

void taskRelease(
		task_t *task)
{
	/* Release children */
	for (unsigned index = 0; index < task->numChildren; index++)
		taskRelease(task->children[index]);

	/* Release task */
	if (task->numParents <= 1)
	{
		if (task->children != NULL)
			free(task->children);
		free(task);
	}
	else
		task->numParents--;
}
