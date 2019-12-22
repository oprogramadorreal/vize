#ifndef LUCCA_QT_ASYNC_EXECUTION_COMMAND_HANDLER_HPP
#define LUCCA_QT_ASYNC_EXECUTION_COMMAND_HANDLER_HPP

#include "lucca_qt/config.hpp"

#include "lucca_qt/command/command_queue.hpp"

#include <lucca/command/simple_command_handler.hpp>

namespace lucca_qt {

/**
 * Executes the given commands asynchronously.
 *
 * This handler uses a command queue to manage the order of execution.
 * Otherwise, it behaves similarly to lucca::SimpleCommandHandler.
 *
 * @see lucca::SimpleCommandHandler
 * @see lucca_qt::CommandQueue
 */
class LUCCA_QT_API AsyncExecutionCommandHandler final : public lucca::SimpleCommandHandler {
public:
	AsyncExecutionCommandHandler();
	virtual ~AsyncExecutionCommandHandler();

private: // lucca::CommandHandler interface
	virtual void executeCommand(std::unique_ptr<lucca::Command> command) final override;

private: // lucca::SimpleCommandHandler interface
	virtual bool _canUndoCommandImpl() const final override;
	virtual bool _canRedoCommandImpl() const final override;

private:
	CommandQueue _commandQueue;
};

}

#endif // LUCCA_QT_ASYNC_EXECUTION_COMMAND_HANDLER_HPP