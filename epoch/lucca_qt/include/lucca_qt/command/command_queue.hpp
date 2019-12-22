#ifndef LUCCA_QT_COMMAND_QUEUE_HPP
#define LUCCA_QT_COMMAND_QUEUE_HPP

#include "lucca_qt/config.hpp"

#include <queue>
#include <mutex>
#include <functional>

namespace lucca {
	class Command;
}

namespace lucca_qt {
	
class Async;
	
/**
 * Executes commands asynchronously.
 */
class LUCCA_QT_API CommandQueue final {
public:
	using CommandFinishedCallback = std::function<void(std::unique_ptr<lucca::Command>)>;

public:
	CommandQueue(const CommandFinishedCallback& callback);
	~CommandQueue();
	
	/**
	 * Adds a command to the queue. If no other command is running, the passed command will start immediately.
	 * If not, it'll be put at the end of the queue.
	 * 
	 * When a command finished executing, _commandFinished will be called.
	 */
	void addCommand(std::unique_ptr<lucca::Command> command);
	
	bool isCommandRunning() const;

private:
	/**
	 * This method will be called every time a command finished running.
	 * It checks the queue to see if there's another command to run. 
	 * 
	 * This method will also call the _commandFinishedCallback passed at the construction of this queue.
	 */
	void _commandFinished();

	void _launchTask();

private:
	using CommandQueueEntry = std::pair<std::unique_ptr<Async>, std::unique_ptr<lucca::Command> >;
	std::queue<CommandQueueEntry> _tasks;
	std::mutex _tasksMutex;
	bool _isCommandRunning = false;
	CommandFinishedCallback _commandFinishedCallback;
};

}

#endif // LUCCA_QT_COMMAND_QUEUE_HPP