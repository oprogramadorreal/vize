#ifndef LUCCA_QT_TOOL_BOX_HPP
#define LUCCA_QT_TOOL_BOX_HPP

#include "lucca_qt/config.hpp"

#include "lucca_qt/make_qt_action_tool.hpp"
#include "lucca_qt/qt_action_tool_group.hpp"

#include <boost/polymorphic_cast.hpp>

#include <QWidget>

class QGridLayout;

namespace lucca_qt {

/**
 * @author O Programador
 */
class LUCCA_QT_API QtToolBox final : public QWidget, public lucca_qt::QtActionToolGroup {
public:
	QtToolBox(QWidget* parent);
	virtual ~QtToolBox();

public:
	template <class ToolType, class... ToolArgs>
	QtActionTool* addActionTool(SizeType row, SizeType column, ToolArgs&&... args);

	void addSeparator(SizeType rowIndex);

private:
	bool _addActionToolImpl(SizeType row, SizeType column, QToolButton* toolButton, QtActionTool* actionTool);

private:
	QGridLayout* _layout = nullptr;
};

template <class ToolType, class... ToolArgs>
QtActionTool* QtToolBox::addActionTool(SizeType row, SizeType column, ToolArgs&&... args) {
	auto toolButton = makeQtActionToolButton<ToolType>(this, std::forward<ToolArgs>(args)...);
	const auto actionTool = boost::polymorphic_downcast<QtActionTool*>(toolButton->defaultAction());

	if (!_addActionToolImpl(row, column, toolButton.get(), actionTool)) {
		return nullptr;
	}

	toolButton.release();

	return actionTool;
}

}

#endif // LUCCA_QT_TOOL_BOX_HPP