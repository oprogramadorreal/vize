#ifndef LUCCA_COMMAND_HPP
#define LUCCA_COMMAND_HPP

#include "lucca/config.hpp"

#include <boost/noncopyable.hpp>

namespace lucca {

/**
 * @remarks You can execute a command by calling "Command::executeCommand" method directly.
 * However, it is preferable that you pass a command instance to a "CommandHandler", which
 * can undo and redo commands in sequence, in the order that they were executed.
 *
 * @see lucca::CommandHandler
 *
 * @author O Programador
 */
class LUCCA_API Command : boost::noncopyable {
public:
	virtual ~Command() = default;

	/**
	 * Executes this command.
	 *
	 * @warning This may be called from another thread by the command handler.
	 */
	virtual void executeCommand() = 0;

	/**
	 * Undoes the effect of executing this command.
	 */
	virtual void undoCommand() = 0;

	/**
	 * Redoes the effect of executing this command.
	 */
	virtual void redoCommand() = 0;

	/**
	 * @remarks If returns 0 this command will be ignored by the command handler.
	 *
	 * @see lucca::CommandHandler
	 */
	virtual SizeType getCommandWeight() const = 0;

	/**
	 * @remarks Only successfully executed commands are kept by the command handler.
	 *
	 * @see lucca::CommandHandler
	 */
	virtual bool wasSuccessfullyExecuted() const = 0;

	/**
	 * @remarks Disposable commands can be easily removed from the commands history of a command handler.
	 *
	 * @see lucca::CommandHandler::discardDisposableCommands()
	 */
	virtual bool isDisposableCommand() const = 0;

	/**
	 * Called by lucca::CommandHandler when disposable commands are discarted.
	 */
	virtual void notifyDiscardDisposableCommands() = 0;

	/**
	 * Called by lucca::CommandHandler when this command is executed.
	 */
	virtual void notifyExecuted() = 0;

	/**
	 * Called by lucca::CommandHandler when this command is undone.
	 */
	virtual void notifyUndone() = 0;

	/**
	 * Called by lucca::CommandHandler when this command is redone.
	 */
	virtual void notifyRedone() = 0;

	/**
	 * @see lucca::CommandComposite
	 */
	virtual void addSubCommand(std::unique_ptr<Command>) {
		assert(false);
	}
};

}

#endif // LUCCA_COMMAND_HPP