#include "barrier.h"

static void barrierRun(void *argument)
{
	barrier_t *barrier = (barrier_t *) argument;

	pthread_mutex_lock(&barrier->doneMutex);
	barrier->done = 1;
	pthread_mutex_unlock(&barrier->doneMutex);
	pthread_cond_broadcast(&barrier->doneCondition);
}

task_t *barrierCreate(barrier_t *barrier)
{
	barrier->done = 0;
	pthread_mutex_init(&barrier->doneMutex, NULL);
	pthread_cond_init(&barrier->doneCondition, NULL);

	return taskCreate(barrierRun, (void *) barrier);
}

void barrierDestroy(barrier_t *barrier)
{
	/* Wait on barrier */
	barrierWait(barrier);

	/* Free resources */
	pthread_mutex_destroy(&barrier->doneMutex);
	pthread_cond_destroy(&barrier->doneCondition);
}

void barrierWait(barrier_t *barrier)
{
	pthread_mutex_lock(&barrier->doneMutex);
	while (barrier->done == 0)
		pthread_cond_wait(&barrier->doneCondition, &barrier->doneMutex);
	pthread_mutex_unlock(&barrier->doneMutex);
}
