#ifndef LUCCA_COMMAND_HANDLER_HPP
#define LUCCA_COMMAND_HANDLER_HPP

#include "lucca/config.hpp"

#include "lucca/command/command_handler_signals.hpp"

#include <boost/noncopyable.hpp>

namespace lucca {

class Command;

/**
 * Generic "command handler" interface.
 *
 * @see lucca::Command
 * @see lucca::SimpleCommandHandler
 *
 * @author O Programador
 */
class LUCCA_API CommandHandler : boost::noncopyable {
public:
	virtual ~CommandHandler() = default;

	/**
	 * Ask this handler to execute a command.
	 */
	virtual void executeCommand(std::unique_ptr<Command> command) = 0;

	/**
	 * Undo the last tracked command that was executed.
	 */
	virtual void undoCommand() = 0;

	/**
	 * Redo the last command that was undone.
	 */
	virtual void redoCommand() = 0;

	/**
	 * Stop keeping track of all commands.
	 */
	virtual void discardAll() = 0;

	/**
	 * Stop keeping track of all disposable commands.
	 */
	virtual void discardDisposableCommands() = 0;

	/**
	 * Listener will be notified after a command be successfully executed.
	 * If the executed command has zero weight, this signal will not be emitted.
	 */
	virtual boost::signals2::connection connectToCommandExecutedSignal(const CommandExecutedSignalListener&) = 0;

	/**
	 * Listener will be notified after a command be undone.
	 */
	virtual boost::signals2::connection connectToCommandUndoneSignal(const CommandUndoneSignalListener&) = 0;

	/**
	 * Listener will be notified after a command be redone.
	 */
	virtual boost::signals2::connection connectToCommandRedoneSignal(const CommandRedoneSignalListener&) = 0;
};

}

#endif // LUCCA_COMMAND_HANDLER_HPP