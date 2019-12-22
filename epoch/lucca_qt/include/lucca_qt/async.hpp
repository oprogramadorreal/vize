#ifndef LUCCA_QT_ASYNC_HPP
#define LUCCA_QT_ASYNC_HPP

#include "lucca_qt/config.hpp"

#include <QProgressDialog>
#include <QFutureWatcher>
#include <QtConcurrent/QtConcurrent>

#include <boost/signals2.hpp>

#include <functional>
#include <atomic>

namespace lucca_qt {

/**
 * @author O Programador
 */
class LUCCA_QT_API Async final {
public:
	using Task = std::function<void()>;

public:
	Async(const Task& task);
	~Async();

	/**
	 * Run task asynchronously.
	 */
	void run();

	/**
	 * Stop the current running task.
	 *
	 * @warning A "task stopped" signal will be emitted and this method will blocked
	 * until the current task is finished. The user (who has created the task) is
	 * responsible for exiting its task properly.
	 */
	void stop();

	bool isRunning() const;

public:
	void showProgressDialog(const std::string& dialogMessage, const std::string& dialogTitle = "");
	void hideProgressDialog();

public: // signals
	using TaskFinishedSignal = boost::signals2::signal<void()>;
	using TaskFinishedSignalListener = TaskFinishedSignal::slot_function_type;

	/**
	 * Register a listener to be notified when a task is completed.
	 */
	boost::signals2::connection connectToTaskFinishedSignal(TaskFinishedSignalListener listener);

	using TaskStoppedSignal = boost::signals2::signal<void()>;
	using TaskStoppedSignalListener = TaskStoppedSignal::slot_function_type;

	/**
	 * Register a listener to be notified when a task is stopped before being finished.
	 */
	boost::signals2::connection connectToTaskStoppedSignal(TaskStoppedSignalListener listener);

	using TaskProgressSignal = boost::signals2::signal<void(SizeType)>;

	/**
	 * Register this async to be notified when a progress @param signal is emitted.
	 */
	boost::signals2::connection listenToTaskProgressSignal(TaskProgressSignal& signal);

private:
	void _handleTaskFinished();
	void _handleTaskCanceled();

private:
	void _startProgressTimer();
	void _stopProgressTimer();

protected:
	using Future = QFuture<void>;
	using FutureWatcher = QFutureWatcher<void>;

	Future _future;
	FutureWatcher _futureWatcher;
	TaskFinishedSignal _taskFinished;
	TaskStoppedSignal _taskStopped;

	std::atomic<SizeType> _progressValue;
	QProgressDialog _progressDialog;
	std::unique_ptr<QTimer> _progressTimer;

private:
	Task _task;
};

}

#endif // LUCCA_QT_ASYNC_HPP