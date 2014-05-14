#include <functional>

#include "barrier.h"

using namespace std;

void
Barrier::run()
{
	unique_lock<mutex> lock(doneMutex);

	done = true;
	doneCondition.notify_all();
}

Barrier::Barrier(Task **barrierTask):
	done(false),
	doneCondition(),
	doneMutex()
{
	Barrier &barrier = *this;
	*barrierTask = new Task(
			[&barrier]() -> void
			{ barrier.run(); });
}

Barrier::~Barrier()
{
	/* Wait on barrier */
	wait();
}

void
Barrier::wait()
{
	unique_lock<mutex> lock(doneMutex);

	bool &finished = done;
	doneCondition.wait(
			lock,
			[&finished]() -> bool
			{ return finished; });
}

void
Barrier::reset()
{
	unique_lock<mutex> lock(doneMutex);
	done = 0;
}
