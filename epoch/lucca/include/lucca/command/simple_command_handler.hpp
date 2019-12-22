#ifndef LUCCA_SIMPLE_COMMAND_HANDLER_HPP
#define LUCCA_SIMPLE_COMMAND_HANDLER_HPP

#include "lucca/config.hpp"

#include "lucca/command/command_handler.hpp"

#include <deque>

namespace lucca {

/**
 * @author O Programador
 */
class LUCCA_API SimpleCommandHandler : public CommandHandler {
public:
	SimpleCommandHandler();
	virtual ~SimpleCommandHandler();

private: // lucca::CommandHandler interface
	virtual void executeCommand(std::unique_ptr<Command> command) override;
	virtual void undoCommand() final override;
	virtual void redoCommand() final override;
	virtual void discardAll() final override;
	virtual void discardDisposableCommands() final override;
	virtual boost::signals2::connection connectToCommandExecutedSignal(const CommandExecutedSignalListener&) final override;
	virtual boost::signals2::connection connectToCommandUndoneSignal(const CommandUndoneSignalListener&) final override;
	virtual boost::signals2::connection connectToCommandRedoneSignal(const CommandRedoneSignalListener&) final override;

protected:
	void _handleExecutedCommand(std::unique_ptr<Command> command);

private:
	virtual bool _canUndoCommandImpl() const;
	virtual bool _canRedoCommandImpl() const;
	
private:
	bool _canUndoCommand() const;
	bool _canRedoCommand() const;

	void _discardRedo();

	void _accumulateWeight(const Command& command);
	void _subtractWeight(const Command& command);
	void _subtractWeight(SizeType weight);

	bool _performExclusiveHistoryAction(const std::function<void()>& action);

private:
	bool _historyActionMutex = false;
	std::deque<std::unique_ptr<Command> > _history;
	SizeType _historyReverseIndex = SizeType(0);
	std::int32_t _accumulatedWeight = std::int32_t(0);
	const std::int32_t _maxAccumulatedWeight = std::int32_t(200);
	CommandExecutedSignal _executedSignal;
	CommandUndoneSignal _undoneSignal;
	CommandRedoneSignal _redoneSignal;
};

}

#endif // LUCCA_SIMPLE_COMMAND_HANDLER_HPP