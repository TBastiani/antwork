#ifndef _BARRIER_H_
#define _BARRIER_H_

#include <thread>
#include <memory>
#include <mutex>
#include <condition_variable>

#include "task.h"

class Barrier
{
	public:
		Barrier(Task **barrierTask);
		~Barrier();

		void wait();
		void reset();

	protected:
		void run();

		bool done;
		std::condition_variable doneCondition;
		std::mutex doneMutex;
};

#endif /* !_BARRIER_H_ */