#ifndef LUCCA_COMMAND_COMPOSITE_HPP
#define LUCCA_COMMAND_COMPOSITE_HPP

#include "lucca/config.hpp"

#include "lucca/command/command.hpp"

#include <functional>
#include <vector>

namespace lucca {

/**
 * @see lucca::Command
 *
 * @author O Programador
 */
class LUCCA_API CommandComposite final : public Command {
public:
	CommandComposite();
	virtual ~CommandComposite();

public:
	void visitSubCommands(const std::function<void(Command&)>& visitor) const;

	bool isEmpty() const;

	void clear();

public: // lucca::Command interface
	virtual void executeCommand() final override;
	virtual void undoCommand() final override;
	virtual void redoCommand() final override;
	virtual SizeType getCommandWeight() const final override;
	virtual bool wasSuccessfullyExecuted() const final override;
	virtual bool isDisposableCommand() const final override;
	virtual void notifyDiscardDisposableCommands() final override;
	virtual void notifyExecuted() final override;
	virtual void notifyUndone() final override;
	virtual void notifyRedone() final override;
	virtual void addSubCommand(std::unique_ptr<Command> subCommand) final override;

private:
	std::vector<std::unique_ptr<Command> > _commands;
};

}

#endif // LUCCA_COMMAND_COMPOSITE_HPP