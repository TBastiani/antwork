#include <stdlib.h>

#include "thread_pool.h"
#include "exception.h"

static void
queuePush(queue_t *queue, void *ptr)
{
	/* Create container */
	node_t *node = (node_t *) calloc(1, sizeof(node_t));
	node->payload = ptr;

	pthread_mutex_lock(&queue->waitMutex);

	if (queue->head == NULL)
		queue->head = node;
	else
		queue->tail->next = node;
	queue->tail = node;

	pthread_mutex_unlock(&queue->waitMutex);
}

static void *
queuePop(queue_t *queue)
{
	void *payload = NULL;
	node_t *node = NULL;

	pthread_mutex_lock(&queue->waitMutex);

	/* Wait for content */
	while (queue->head == NULL)
		pthread_cond_wait(&queue->waitCondition, &queue->waitMutex);

	/* If payload is NULL leave it for other threads */
	if (queue->head->payload == NULL)
	{
		pthread_mutex_unlock(&queue->waitMutex);
		return NULL;
	}

	/* Pop head of queue */
	node = queue->head;
	queue->head = node->next;
	if (node->next == NULL)
		queue->tail = NULL;

	pthread_mutex_unlock(&queue->waitMutex);

	/* Destroy container */
	payload = node->payload;
	free(node);
	return payload;
}

static void *
threadRun(void *arg)
{
	thread_pool_t *pool = (thread_pool_t *) arg;
	queue_t *queue = &pool->queue;

	for (;;)
	{
		task_t *task = (task_t *) queuePop(queue);
		if (task == NULL)
			break;
		taskRun(task, pool);
	}
	return NULL;
}

thread_pool_t *
threadPoolCreate(unsigned numThreads)
{
	REQUIRE_CONDITION(numThreads != 0, "Can't build a zero thread pool");

	thread_pool_t *pool = (thread_pool_t *) calloc(1, sizeof(thread_pool_t));
	REQUIRE_CONDITION(pool != NULL, "Memory allocation failed");

	pool->numThreads = numThreads;
	pool->threads = (pthread_t *) malloc(sizeof(pthread_t) * pool->numThreads);
	REQUIRE_CONDITION(pool->threads != NULL, "Memory allocation failed");

	/* initialise queue */
	queue_t *queue = &pool->queue;
	pthread_mutex_init(&queue->waitMutex, NULL);
	pthread_cond_init(&queue->waitCondition, NULL);

	/* Spawn threads */
	for (unsigned tid = 0; tid < pool->numThreads; tid++)
	{
		pthread_t *thread = pool->threads + tid;
		int status = pthread_create(thread, NULL, threadRun, (void *) pool);
		DEBUG_MESSAGE("Thread %u created %s", tid, (status == 0)? "successfully":"unsuccessfully");
	}

	return pool;
}

void
threadPoolDestroy(thread_pool_t *pool)
{
	/* Enqueue and broadcast finish task */
	queuePush(&pool->queue, NULL);
	pthread_cond_broadcast(&pool->queue.waitCondition);

	/* Join threads */
	for (unsigned tid = 0; tid < pool->numThreads; tid++)
		pthread_join(pool->threads[tid], NULL);

	/* Free termination task */
	queue_t *queue = &pool->queue;
	REQUIRE_CONDITION(queue->head == queue->tail, "More than one element in pool");
	REQUIRE_CONDITION(queue->head != NULL, "Less than one element in pool");
	REQUIRE_CONDITION(queue->head->payload == NULL, "Non NULL task in pool");
	free(queue->head);

	/* Free memory */
	free(pool->threads);
	pthread_mutex_destroy(&queue->waitMutex);
	pthread_cond_destroy(&queue->waitCondition);
	free(pool);
}

void
threadPoolAddTask(thread_pool_t *pool, task_t *task)
{
	queuePush(&pool->queue, (void *) task);
	pthread_cond_signal(&pool->queue.waitCondition);
}

