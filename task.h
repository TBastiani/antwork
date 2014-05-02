#ifndef _TASK_H_
#define _TASK_H_

/* Forward declaration */
struct thread_pool;

typedef struct task
{
	void (*workFnPtr)(void *);
	void *argument;

	struct task **children;
	unsigned numChildren;

	unsigned numParents;
	unsigned numParentsDone;
} task_t;

task_t *taskCreate(
		void (*fnPtr)(void *),
		void *argument);
void taskRelease(task_t *task);

void taskAddParent(
		task_t *task,
		task_t *parentTask);
void taskRun(task_t *task, struct thread_pool *pool);

#endif /* _TASK_H_ */