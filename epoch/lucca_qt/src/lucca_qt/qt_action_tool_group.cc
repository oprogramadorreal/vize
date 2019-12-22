#include "lucca_qt/qt_action_tool_group.hpp"

#include "lucca_qt/qt_action_tool.hpp"

namespace lucca_qt {

QtActionToolGroup::QtActionToolGroup(QObject* parent) {
	_actionGroup = std::make_unique<QActionGroup>(parent);
	_actionGroup->setExclusive(true);

	QObject::connect(_actionGroup.get(), &QActionGroup::triggered, std::bind(&QtActionToolGroup::_toolActivated, this, std::placeholders::_1));
}

QtActionToolGroup::~QtActionToolGroup() = default;

QtActionTool* QtActionToolGroup::getActiveActionTool() const {
	const auto actionTool = dynamic_cast<QtActionTool*>(_actionGroup->checkedAction());

	if (!actionTool) {
		return nullptr;
	}

	return actionTool;
}

lucca::Tool* QtActionToolGroup::getActiveTool() const {
	const auto actionTool = getActiveActionTool();

	if (!actionTool) {
		return nullptr;
	}

	return actionTool->getActualTool();
}

void QtActionToolGroup::activateDefaultTool() {
	if (_defaultTool) {
		_defaultTool->trigger();
	}
}

void QtActionToolGroup::_toolActivated(QAction* action) {
	lucca::Tool* newActiveTool = nullptr;

	if (const auto actionTool = dynamic_cast<QtActionTool*>(action)) {
		_actionToolActivatedSignal(*actionTool);
		newActiveTool = actionTool->getActualTool();
	}

	if (_activeTool) {
		_activeTool->deactivateTool();
	}

	_activeTool = newActiveTool;
}

boost::signals2::connection QtActionToolGroup::connectToActionToolActivatedSignal(ActionToolActivatedSignalListener listener) {
	return _actionToolActivatedSignal.connect(listener);
}

bool QtActionToolGroup::addTool(lucca::Tool* tool) {
	const auto actionTool = dynamic_cast<QtActionTool*>(tool);

	if (!actionTool) {
		return false;
	}

	tool->setToolGroup(this);
	actionTool->setCheckable(true);
		
	_actionGroup->addAction(actionTool);

	if (!_defaultTool) {
		_defaultTool = actionTool;
	}

	return true;
}

}