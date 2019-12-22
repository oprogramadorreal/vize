#include "lucca/command/simple_command_handler.hpp"

#include "lucca/command/command.hpp"
#include "lucca/command/command_composite.hpp"

namespace lucca {

SimpleCommandHandler::SimpleCommandHandler() = default;
SimpleCommandHandler::~SimpleCommandHandler() = default;

void SimpleCommandHandler::executeCommand(std::unique_ptr<Command> command) {
	assert(command);

	if (_performExclusiveHistoryAction([&command]() { command->executeCommand(); })) {
		_handleExecutedCommand(std::move(command));
	}
}

void SimpleCommandHandler::_handleExecutedCommand(std::unique_ptr<Command> command) {
	const auto commandPointer = command.get();
	assert(commandPointer);

	auto successfullyExecuted = false;

	const auto historyAction = [this, &command, commandPointer, &successfullyExecuted]() {
		successfullyExecuted = command->wasSuccessfullyExecuted();

		if (successfullyExecuted) {
			if (command->getCommandWeight() > SizeType(0)) {
				_discardRedo();
				_history.push_back(std::move(command));
				_accumulateWeight(*commandPointer);
			}
		}
	};

	if (_performExclusiveHistoryAction(historyAction)) {
		if (successfullyExecuted) {
			// this is not "exclusive history action"... user can trigger new commands
			_executedSignal();
			commandPointer->notifyExecuted();
		}
	}
}

void SimpleCommandHandler::undoCommand() {
	_performExclusiveHistoryAction(
		[this]() {
			if (_canUndoCommand()) {
				const auto commandPointer = _history[SizeType(_history.size()) - _historyReverseIndex - 1].get();
				assert(commandPointer);
				commandPointer->undoCommand();
				++_historyReverseIndex;
				_undoneSignal();
				commandPointer->notifyUndone();
			}
		}
	);
}

void SimpleCommandHandler::redoCommand() {
	_performExclusiveHistoryAction(
		[this]() {
			if (_canRedoCommand()) {
				--_historyReverseIndex;
				const auto commandPointer = _history[SizeType(_history.size()) - _historyReverseIndex - 1].get();
				assert(commandPointer);
				commandPointer->redoCommand();
				_redoneSignal();
				commandPointer->notifyRedone();
			}
		}
	);
}

bool SimpleCommandHandler::_canUndoCommandImpl() const {
	return true;
}

bool SimpleCommandHandler::_canRedoCommandImpl() const {
	return true;
}

bool SimpleCommandHandler::_canUndoCommand() const {
	if (!_canUndoCommandImpl()) {
		return false;
	}

	return _historyReverseIndex < SizeType(_history.size());
}

bool SimpleCommandHandler::_canRedoCommand() const {
	if (!_canRedoCommandImpl()) {
		return false;
	}

	if (_historyReverseIndex < SizeType(1)) {
		return false;
	}

	return _historyReverseIndex - 1 < SizeType(_history.size());
}

void SimpleCommandHandler::discardAll() {
	_performExclusiveHistoryAction(
		[this]() {
			_accumulatedWeight = std::int32_t(0);
			_historyReverseIndex = SizeType(0);
			_history.clear();
		}
	);
}

void SimpleCommandHandler::discardDisposableCommands() {
	_performExclusiveHistoryAction(
		[this]() {
			_discardRedo(); //

			for (auto it = _history.begin(); it != _history.end();) {
				const auto commandWeight = (*it)->getCommandWeight();
				(*it)->notifyDiscardDisposableCommands();

				if ((*it)->isDisposableCommand()) {
					_subtractWeight(commandWeight);
					it = _history.erase(it);
				} else {
					++it;
				}
			}
		}
	);
}

boost::signals2::connection SimpleCommandHandler::connectToCommandExecutedSignal(const CommandExecutedSignalListener& listener) {
	return _executedSignal.connect(listener);
}

boost::signals2::connection SimpleCommandHandler::connectToCommandUndoneSignal(const CommandUndoneSignalListener& listener) {
	return _undoneSignal.connect(listener);
}

boost::signals2::connection SimpleCommandHandler::connectToCommandRedoneSignal(const CommandRedoneSignalListener& listener) {
	return _redoneSignal.connect(listener);
}

void SimpleCommandHandler::_accumulateWeight(const Command& command) {
	_accumulatedWeight += std::int32_t(command.getCommandWeight());

	while (!_history.empty() && (_accumulatedWeight > _maxAccumulatedWeight)) {
		_subtractWeight(*_history.front());
		_history.pop_front();
	}
}

void SimpleCommandHandler::_subtractWeight(const Command& command) {
	_subtractWeight(command.getCommandWeight());
}

void SimpleCommandHandler::_subtractWeight(SizeType weight) {
	_accumulatedWeight -= std::int32_t(weight);
	_accumulatedWeight = std::max(_accumulatedWeight, std::int32_t(0));
}

void SimpleCommandHandler::_discardRedo() {
	while (!_history.empty() && (_historyReverseIndex > SizeType(0))) {
		_subtractWeight(*_history.back());
		_history.pop_back();
		--_historyReverseIndex;
	}

	assert(SizeType(0) == _historyReverseIndex);
}

bool SimpleCommandHandler::_performExclusiveHistoryAction(const std::function<void()>& action) {
	if (_historyActionMutex) {
		AYLA_DEBUG_MESSAGE("SimpleCommandHandler::_performExclusiveHistoryAction() - Cannot perform another exclusive history action.");
		return false;
	}

	struct HistoryLockGuard final {
		HistoryLockGuard(bool& mutex) : _mutex(mutex) { _mutex = true; }
		~HistoryLockGuard() { _mutex = false; }
		bool& _mutex;
	};

	HistoryLockGuard lock(_historyActionMutex);
	action();

	return true;
}

}