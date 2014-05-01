#ifndef _TASK_H_
#define _TASK_H_

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
void taskRun(task_t *task);

#endif /* _TASK_H_ */