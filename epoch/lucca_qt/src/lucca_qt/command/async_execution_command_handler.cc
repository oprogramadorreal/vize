#include "lucca_qt/command/async_execution_command_handler.hpp"

#include <lucca/command/command.hpp>

namespace lucca_qt {

AsyncExecutionCommandHandler::AsyncExecutionCommandHandler()
	: _commandQueue([this](std::unique_ptr<lucca::Command> command) { _handleExecutedCommand(std::move(command)); })
{ }

AsyncExecutionCommandHandler::~AsyncExecutionCommandHandler() = default;

void AsyncExecutionCommandHandler::executeCommand(std::unique_ptr<lucca::Command> command) {
	assert(command);
	_commandQueue.addCommand(std::move(command));
}

bool AsyncExecutionCommandHandler::_canUndoCommandImpl() const {
	return !_commandQueue.isCommandRunning(); // if command queue is not empty, "undo" is ignored
}

bool AsyncExecutionCommandHandler::_canRedoCommandImpl() const {
	return !_commandQueue.isCommandRunning(); // if command queue is not empty, "undo" is ignored
}

}