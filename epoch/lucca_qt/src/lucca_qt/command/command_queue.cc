#include "lucca_qt/command/command_queue.hpp"

#include "lucca_qt/async.hpp"

#include <lucca/command/command.hpp>

namespace lucca_qt {

CommandQueue::CommandQueue(const CommandFinishedCallback& callback)
	: _commandFinishedCallback(callback)
{
	assert(nullptr != _commandFinishedCallback);
}

CommandQueue::~CommandQueue() = default;

void CommandQueue::addCommand(std::unique_ptr<lucca::Command> command) {
	std::lock_guard<std::mutex> lockGuard(_tasksMutex);
	
	auto commandPtr = command.get();
	auto task = [commandPtr]() { commandPtr->executeCommand(); };
	auto taskRunner = std::make_unique<Async>(task);

	_tasks.push(std::make_pair(std::move(taskRunner), std::move(command)));
	
	if (!isCommandRunning()) {
		_launchTask();
	}
}

bool CommandQueue::isCommandRunning() const {
	return _isCommandRunning;
}

void CommandQueue::_launchTask() {
	if (_tasks.empty()) {
		_isCommandRunning = false;

	} else {
		_isCommandRunning = true;

		_tasks.front().first->connectToTaskFinishedSignal([this](){ _commandFinished(); });
		_tasks.front().first->run();
	}
}

void CommandQueue::_commandFinished() {
	std::unique_ptr<lucca::Command> finishedCommand;
	
	{
		std::lock_guard<std::mutex> lockGuard(_tasksMutex);

		assert(!_tasks.empty());
		finishedCommand = std::move(_tasks.front().second);
		_tasks.pop();

		_launchTask(); // launch next task, if any
	}
	
	_commandFinishedCallback(std::move(finishedCommand));
}

}