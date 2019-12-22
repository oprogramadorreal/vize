#ifndef LUCCA_QT_MAKE_QT_ACTION_TOOL_HPP
#define LUCCA_QT_MAKE_QT_ACTION_TOOL_HPP

#include "lucca_qt/config.hpp"

#include "lucca_qt/qt_action_tool.hpp"

#include <lucca/tool/tool_descriptor.hpp>

#include <QToolButton>

namespace lucca_qt {

/**
 * Helper function to create a lucca_qt::QtActionTool instance.
 *
 * @remarks Make sure to have declared a tool descriptor for the
 * chosen tool type and that this declaration is available in the
 * same file you call this function. Remember that a tool descriptor
 * must be declared inside "descriptor_declaration" namespace.
 *
 * @see lucca::ToolDescriptor
 * @see lucca_qt::QtActionTool
 *
 * @author O Programador
 */
template <class ToolType, class... ToolArgs>
std::unique_ptr<QtActionTool> makeQtActionTool(QObject* parent, ToolArgs&&... args) {
	auto action = std::make_unique<QtActionTool>(
		parent, std::make_unique<ToolType>(std::forward<ToolArgs>(args)...)
	);

	auto descriptor = std::make_unique<descriptor_declaration::ToolDescriptor<ToolType>>();
	action->setToolDescriptor(std::move(descriptor));

	return std::move(action);
}

/**
 * Creates a QToolButton bound to a lucca_qt::QtActionTool.
 * Note that the QToolButton will have the ownership of the created action.
 *
 * @see lucca_qt::makeQtActionTool (helper function)
 * @see lucca_qt::QtActionTool
 *
 * @author O Programador
 */
template <class ToolType, class... ToolArgs>
std::unique_ptr<QToolButton> makeQtActionToolButton(QWidget* parent, ToolArgs&&... args) {
	auto button = std::make_unique<QToolButton>(parent);
	auto action = makeQtActionTool<ToolType>(button.get(), std::forward<ToolArgs>(args)...).release();
	button->setAutoRaise(true);
	button->setDefaultAction(action);
	return button;
}

}

#endif // LUCCA_QT_MAKE_QT_ACTION_TOOL_HPP