#ifndef _BARRIER_H_
#define _BARRIER_H_

#include <pthread.h>

#include "task.h"

typedef struct barrier
{
	unsigned done;
	pthread_cond_t doneCondition;
	pthread_mutex_t doneMutex;
} barrier_t;

task_t *barrierCreate(barrier_t *barrier);

void barrierDestroy(barrier_t *barrier);

void barrierWait(barrier_t *);

void barrierReset(barrier_t *);

#endif /* !_BARRIER_H_ */