#ifndef _THREAD_POOL_H_
#define _THREAD_POOL_H_

#include <pthread.h>

#include "task.h"

typedef struct node
{
	void *payload;
	struct node *next;
}node_t;

typedef struct queue
{
	node_t *head;
	node_t *tail;

	pthread_cond_t waitCondition;
	pthread_mutex_t waitMutex;
}queue_t;

typedef struct thread_pool
{
	unsigned numThreads;
	pthread_t *threads;

	queue_t queue;
}thread_pool_t;

thread_pool_t *
threadPoolCreate(unsigned numThreads);

void
threadPoolDestroy(thread_pool_t *pool);

void
threadPoolAddTask(thread_pool_t *pool, task_t *task);

#endif /* !_THREAD_POOL_H_ */