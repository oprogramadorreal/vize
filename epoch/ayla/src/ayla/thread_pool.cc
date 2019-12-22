#ifdef __GNUC__
	#pragma GCC diagnostic push	
	#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#endif // __GNUC__

#include "ayla/thread_pool.hpp"

#include <boost/atomic.hpp>
#include <boost/thread.hpp> // hidden boost header

namespace ayla {

class ThreadPool::Private {
public:
	void executeTasks() {
		auto task = _askForNewTask(); // current task

		while (task) {
			task->execute();
			task = _askForNewTask();
		}
	}

private:
	Task* _askForNewTask() {
		assert(currentGroup);

		const auto newTaskIndex = currentTask++;

		if (newTaskIndex < currentGroup->size()) {
			return (*currentGroup)[newTaskIndex].get();
		}

		return nullptr; // no task remaining
	}

public:
	std::vector<std::unique_ptr<WorkerThread> > workers;
	const ThreadPool::TasksGroup* currentGroup = nullptr;
	boost::atomic_uint currentTask;
	bool destroyed = false;

	std::unique_ptr<boost::barrier> execBarrier, finishBarrier;
};

///////////////////////////////////////////////

class ThreadPool::WorkerThread {
public:
	WorkerThread(const ThreadPool& pool)
		: _pool(pool)
	{
		_thread = std::make_unique<boost::thread>(boost::bind(&WorkerThread::_execution, this));
	}

	void join() const { _thread->join(); }

private:
	// This is the execution path for each thread in the pool.
	// Note that this starts when the pool is created and only finish when the pool is destroyed (see the ThreadPool destructor).
	void _execution() {
		while (true) {
			_pool._p->execBarrier->wait(); // waiting for a call to "executeTasks"

			if (_pool._p->destroyed) {
				return;
			}

			_pool._p->executeTasks();
			_pool._p->finishBarrier->wait(); // wait for all threads to finish
		}
	}

private:
	std::unique_ptr<boost::thread> _thread;
	const ThreadPool& _pool;
};

///////////////////////////////////////////////

ThreadPool::ThreadPool(SizeType numOfThreads)
	: _p(std::make_unique<Private>())
{
	assert(numOfThreads > 0u);

	AYLA_DEBUG_MESSAGE("ayla::ThreadPool :: (threads / hardware threads) == (" + std::to_string(numOfThreads) + " / " + std::to_string(ayla::ThreadPool::getHardwareConcurrency()) + ")");

	_p->destroyed = false;
	_p->execBarrier = std::make_unique<boost::barrier>(unsigned(numOfThreads));
	_p->finishBarrier = std::make_unique<boost::barrier>(unsigned(numOfThreads));

	const SizeType numOfWorkers = numOfThreads - 1;

	_p->workers.reserve(numOfWorkers);

	for (unsigned i = 0u; i < numOfWorkers; ++i) { // create new threads
		_p->workers.push_back(std::make_unique<WorkerThread>(*this));
	}
}

ThreadPool::~ThreadPool() { //
	_p->destroyed = true;
	_p->execBarrier->wait();

	for (auto it = _p->workers.begin(); it != _p->workers.end(); ++it) {
		(*it)->join();
	}
}

void ThreadPool::execute(const TasksGroup& tasks) {
	_p->currentGroup = &tasks;
	_p->currentTask = 0u;

	_p->execBarrier->wait(); // launch all threads
	_p->executeTasks(); // also execute tasks in main thread
	_p->finishBarrier->wait(); // wait for all threads to finish
}

SizeType ThreadPool::getNumberOfThreads() const {
	return SizeType(_p->workers.size() + 1);
}

SizeType ThreadPool::getHardwareConcurrency() {
	return boost::thread::hardware_concurrency();
}

}

#ifdef __GNUC__
	#pragma GCC diagnostic pop
#endif // __GNUC__