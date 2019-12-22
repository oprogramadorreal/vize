#include "lucca_qt/async.hpp"

namespace lucca_qt {

Async::Async(const Task& task)
	: _progressValue(SizeType(0)), _progressDialog("Operation in progress. Please wait.", "Cancel", 0, 100), _task(task)
{
	QObject::connect(&_futureWatcher, &FutureWatcher::finished, std::bind(&Async::_handleTaskFinished, this));
	QObject::connect(&_futureWatcher, &FutureWatcher::canceled, std::bind(&Async::_handleTaskCanceled, this));

	QObject::connect(&_progressDialog, &QProgressDialog::canceled, &_futureWatcher, &FutureWatcher::cancel);
	_progressDialog.setWindowFlags(Qt::WindowTitleHint | Qt::WindowStaysOnTopHint);
	_progressDialog.reset();

	const auto hideProgress = std::bind(&Async::hideProgressDialog, this);
	connectToTaskFinishedSignal(hideProgress);
	connectToTaskStoppedSignal(hideProgress);
}

Async::~Async() {
	stop();
}

void Async::run() {
	stop();

	_future = QtConcurrent::run(_task);
	_futureWatcher.setFuture(_future);
}

void Async::stop() {
	if (!isRunning()) {
		return; // nothing to do
	}

	_futureWatcher.cancel();
	_handleTaskCanceled();
}

bool Async::isRunning() const {
	return _futureWatcher.isRunning();
}

void Async::showProgressDialog(const std::string& dialogMessage, const std::string& dialogTitle) {
	_startProgressTimer();
	_progressDialog.setLabelText(QString::fromStdString(dialogMessage));
	_progressDialog.setWindowTitle(QString::fromStdString(dialogTitle));
	_progressDialog.show();
}

void Async::hideProgressDialog() {
	_stopProgressTimer();
	_progressDialog.reset();
}

void Async::_startProgressTimer() {
	if (_progressTimer) {
		return;
	}

	_progressTimer = std::make_unique<QTimer>();

	const auto updateProgress = [this]() {
		_progressDialog.setValue(int(_progressValue));
	};

	QObject::connect(_progressTimer.get(), &QTimer::timeout, updateProgress);
	_progressTimer->start();
}

void Async::_stopProgressTimer() {
	if (!_progressTimer) {
		return;
	}

	_progressTimer->stop();
	QObject::disconnect(_progressTimer.get(), &QTimer::timeout, 0, 0);
	_progressTimer.reset();
}

boost::signals2::connection Async::connectToTaskFinishedSignal(TaskFinishedSignalListener listener) {
	return _taskFinished.connect(listener);
}

boost::signals2::connection Async::connectToTaskStoppedSignal(TaskStoppedSignalListener listener) {
	return _taskStopped.connect(listener);
}

boost::signals2::connection Async::listenToTaskProgressSignal(TaskProgressSignal& signal) {
	const auto setProgressValue = [this](SizeType value) {
		_progressValue = glm::clamp(value, SizeType(0), SizeType(100));
	};

	return signal.connect(setProgressValue);
}

void Async::_handleTaskFinished() {
	if (!_futureWatcher.isCanceled()) {
		_taskFinished();
	}
}

void Async::_handleTaskCanceled() {
	if (_futureWatcher.isRunning()) {
		_taskStopped();
		_futureWatcher.waitForFinished(); // block here, wait for the task
	}
}

}