#include "task.h"
#include "thread_pool.h"

using namespace std;

Task::Task(function<void (void)> workFn):
	workFunction(workFn),
	children(),
	numParents(0),
	numParentsDone(0)
{
}

void
Task::addChild(shared_ptr<Task> child)
{
	children.push_back(child);
	child->numParents++;
}

void
Task::run(ThreadPool &threadPool)
{
	numParentsDone = 0;
	workFunction();

	/* Update children */
	for (unsigned index = 0; index < children.size(); index++)
	{
		Task &childTask = *children[index];
		if (childTask.numParentsDone.fetch_add(1) == childTask.numParents - 1)
			threadPool.enqueueTask(&childTask);
	}
}

Task::~Task()
{
}
