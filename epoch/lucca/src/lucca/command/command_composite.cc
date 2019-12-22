#include "lucca/command/command_composite.hpp"

namespace lucca {

CommandComposite::CommandComposite() = default;
CommandComposite::~CommandComposite() = default;

void CommandComposite::visitSubCommands(const std::function<void(Command&)>& visitor) const {
	for (const auto& c : _commands) {
		assert(c);
		visitor(*c);
	}
}

bool CommandComposite::isEmpty() const {
	return _commands.empty();
}

void CommandComposite::clear() {
	_commands.clear();
}

void CommandComposite::executeCommand() {
	visitSubCommands(
		[](Command& command) {
			command.executeCommand();
		}
	);
}

void CommandComposite::undoCommand() {
	for (auto it = _commands.rbegin(); it != _commands.rend(); ++it) {
		(*it)->undoCommand();
	}
}

void CommandComposite::redoCommand() {
	for (const auto& c : _commands) {
		c->redoCommand();
	}
}

SizeType CommandComposite::getCommandWeight() const {
	auto weight = SizeType(0);

	visitSubCommands(
		[&weight](Command& command) {
			weight += command.getCommandWeight();
		}
	);

	return weight;
}

bool CommandComposite::wasSuccessfullyExecuted() const {
	for (const auto& c : _commands) {
		if (!c->wasSuccessfullyExecuted()) {
			return false;
		}
	}

	return !_commands.empty();
}

bool CommandComposite::isDisposableCommand() const {
	for (const auto& c : _commands) {
		if (!c->isDisposableCommand()) {
			return false;
		}
	}

	return true;
}

void CommandComposite::notifyDiscardDisposableCommands() {
	visitSubCommands(
		[](Command& command) {
			command.notifyDiscardDisposableCommands();
		}
	);
}

void CommandComposite::notifyExecuted() {
	visitSubCommands(
		[](Command& command) {
			command.notifyExecuted();
		}
	);
}

void CommandComposite::notifyUndone() {
	for (auto it = _commands.rbegin(); it != _commands.rend(); ++it) {
		(*it)->notifyUndone();
	}
}

void CommandComposite::notifyRedone() {
	visitSubCommands(
		[](Command& command) {
			command.notifyRedone();
		}
	);
}

void CommandComposite::addSubCommand(std::unique_ptr<Command> subCommand) {
	_commands.push_back(std::move(subCommand));
}

}