#ifndef AYLA_THREAD_POOL_HPP
#define AYLA_THREAD_POOL_HPP

#include "ayla/config.hpp"

#include <vector>
#include <memory>

#include <boost/noncopyable.hpp>

namespace ayla {

/**
 * An abstraction that implements the "thread pool pattern".
 *
 * The only reason you should use this thread pool is to improve the application performance when you have multiple
 * independent tasks (and therefore can run in parallel).
 *
 * The multiple tasks you insert in a TasksGroup will be executed (in "as parallel as possible") only when you call the
 * "ThreadPool::execute" method passing the desired group. Note that the "ThreadPool::execute" will only return when all
 * tasks are completed. Thus, the user of thread pool should not worry with multithreading issues in any way.
 *
 * Also note that all methods in the public interface are NOT thread-safe. In other words, instances of this class
 * were NOT designed to be handled by multiple threads simultaneously.
 *
 * @see http://en.wikipedia.org/wiki/Thread_pool_pattern
 *
 * @author O Programador
 */
class AYLA_API ThreadPool final : boost::noncopyable {
public:
	/**
	 * One task to be executed by the pool.
	 */
	class AYLA_API Task {
	public:
		virtual void execute() = 0;
	};

	/**
	 * A group of tasks to be executed by a ThreadPool instance.
	 */
	typedef std::vector<std::unique_ptr<Task> > TasksGroup;

public:
	/**
	 * The internal worker threads will be created and launched here but they will stay in blocked state until you call "ThreadPool::execute".
	 *
	 * @param numOfThreads The number of worker threads available to execute all tasks. Note you cannot change this number later.
	 */
	ThreadPool(SizeType numOfThreads = ThreadPool::getHardwareConcurrency());

	/**
	 * The internal worker threads will be killed only here.
	 * This destructor will wait for all threads are finished.
	 */
	~ThreadPool();

	/**
	 * Run all tasks on the provided group of tasks.
	 *
	 * @remarks It will return only when all tasks are completed.
	 */
	void execute(const TasksGroup& tasks);

	/**
	 * @return The number of worker threads in this pool.
	 */
	SizeType getNumberOfThreads() const;

public:
	/**
	 * @return The number of hardware threads available on the current system
	 * (e.g. number of CPUs or cores or hyperthreading units), or 1 if this information is not available.
	 */
	static SizeType getHardwareConcurrency();

private:
	class WorkerThread;

	class Private;
	std::unique_ptr<Private> _p;
};

}

#endif // AYLA_THREAD_POOL_HPP