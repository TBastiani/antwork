#ifndef _TASK_H_
#define _TASK_H_

#include <vector>
#include <functional>
#include <memory>
#include <atomic>

/* Forward declaration */
class ThreadPool;

class Task
{
	public:
		Task(std::function<void (void)> workFunction);
		~Task();

		void addChild(std::shared_ptr<Task> child);
		void run(ThreadPool &threadPool);

	protected:
		std::function<void (void)> workFunction;

		std::vector<std::shared_ptr<Task> > children;
		unsigned numParents;
		std::atomic<unsigned> numParentsDone;
};


#endif /* _TASK_H_ */