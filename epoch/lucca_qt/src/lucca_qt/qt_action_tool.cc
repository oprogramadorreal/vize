#include "lucca_qt/qt_action_tool.hpp"

#include <lucca/tool/tool_descriptor.hpp>

#include <QIcon>

namespace lucca_qt {

QtActionTool::QtActionTool(QObject* parent, std::unique_ptr<Tool> tool)
	: QAction(parent), _tool(std::move(tool))
{
	connect(this, &QAction::triggered, std::bind(&QtActionTool::_handleTriggered, this, std::placeholders::_1));
}

QtActionTool::~QtActionTool() = default;

void QtActionTool::_handleTriggered(bool checked) {
	// "QAction::triggered" signal comes from user interaction only.
	// See http://doc.qt.io/qt-5/qaction.html#triggered

	if (isCheckable()) {
		if (checked) {
			activateTool();
		} else {
			deactivateTool();
		}
	} else {
		activateTool();
	}
}

void QtActionTool::activateTool() {
	if (const auto tool = getActualTool()) {
		tool->activateTool();
	}

	setChecked(isToolActive());
}

void QtActionTool::deactivateTool() {
	if (const auto tool = getActualTool()) {
		tool->deactivateTool();
	}

	setChecked(isToolActive());
}

bool QtActionTool::isToolActive() const {
	if (const auto tool = getActualTool()) {
		return tool->isToolActive();
	}

	return isChecked();
}

void QtActionTool::setToolGroup(lucca::ToolGroup* toolGroup) {
	if (const auto tool = getActualTool()) {
		tool->setToolGroup(toolGroup);
	}
}

boost::signals2::connection QtActionTool::connectToActivatedSignal(ActivatedSignalListener listener) {
	if (const auto tool = getActualTool()) {
		return tool->connectToActivatedSignal(listener);
	}

	return boost::signals2::connection();
}

void QtActionTool::setToolDescriptor(std::unique_ptr<lucca::ToolDescriptor> descriptor) {
	QIcon icon;

	const auto iconFile = descriptor->getIcon();
	const auto bigIconFile = descriptor->getBigIcon();

	if (!iconFile.empty()) {
		icon.addFile(QString::fromStdString(iconFile));
	}
	if (!bigIconFile.empty()) {
		icon.addFile(QString::fromStdString(bigIconFile));
	}

	setIcon(icon);
	setToolTip(QString::fromStdString(descriptor->getToolTip()));
	setText(QString::fromStdString(descriptor->getName()));

	const auto shortcut = descriptor->getShortcut();

	if (!shortcut.empty()) {
		setShortcut(QString::fromStdString(shortcut));
	}
	
	_descriptor = std::move(descriptor);
}

std::string QtActionTool::getToolCursor() const {
	return _descriptor ? _descriptor->getCursor() : std::string("");
}

lucca::Tool* QtActionTool::getActualTool() const {
	return _tool.get();
}

}