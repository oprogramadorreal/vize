#include "lucca_qt/qt_tool_box.hpp"

#include <QFrame>
#include <QGridLayout>

namespace lucca_qt {

QtToolBox::QtToolBox(QWidget* parent)
	: QWidget(parent), QtActionToolGroup(this)
{
	_layout = std::make_unique<QGridLayout>(this).release();
	_layout->setSizeConstraint(QLayout::SetFixedSize);
}

QtToolBox::~QtToolBox() = default;

void QtToolBox::addSeparator(SizeType rowIndex) {
	auto line = std::make_unique<QFrame>(this).release();
	line->setFrameShape(QFrame::HLine);
	line->setFrameShadow(QFrame::Sunken);
	_layout->addWidget(line, rowIndex, 0, 1, 0);
}

bool QtToolBox::_addActionToolImpl(SizeType row, SizeType column, QToolButton* toolButton, QtActionTool* actionTool) {
	if (!addTool(actionTool)) { // add tool to action group
		return false;
	}

	assert(nullptr != toolButton);
	_layout->addWidget(toolButton, row, column, 1, 1);

	return true;
}

}