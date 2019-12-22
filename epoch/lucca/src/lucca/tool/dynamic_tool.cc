#include "lucca/tool/dynamic_tool.hpp"

#include "lucca/tool/tool_group.hpp"
#include "lucca/view/view.hpp"
#include "lucca/painter/painter.hpp"
#include "lucca/document/multi_view_document.hpp"

namespace lucca {

DynamicTool::DynamicTool(MultiViewDocument& document)
	: _document(document)
{
	_documentConnetion = _document.connectToSubViewActivatedSignal(std::bind(&DynamicTool::_subViewActivated, this, std::placeholders::_1));
	_toolConnetion = connectToActivatedSignal(std::bind(&DynamicTool::_dynamicToolActivated, this, std::placeholders::_1));
}

DynamicTool::~DynamicTool() = default;

std::unique_ptr<Painter> DynamicTool::createToolPainter() {
	return nullptr;
}

DynamicTool::EventHandlers DynamicTool::getToolEventHandlers() {
	return { };
}

void DynamicTool::setEventHandlersGroup(SizeType eventHandlersGroup) {
	_eventHandlersGroup = eventHandlersGroup;
}

MultiViewDocument& DynamicTool::_getDocument() const {
	return _document;
}

void DynamicTool::_subViewActivated(const View&) {
	if (isToolActive()) {
		activateTool(); // re-activate this tool
	}
}

void DynamicTool::_dynamicToolActivated(Tool&) {
	// make this tool handle the events of the active sub view
	if (const auto subView = _document.getActiveSubView()) {
		subView->clearConnections(_eventHandlersGroup); // remove any handlers from other tools

		const auto eventHandlers = getToolEventHandlers();

		// keep only the handlers of this tool
		for (auto handler : eventHandlers) {
			subView->connectEventHandler(*handler, _eventHandlersGroup);
		}
	}

	// make all sub views use the painter of this tool, if it exists
	_document.visitSubViews(
		[this](View& subView) {
			if (const auto painter = subView.getPainter()) {
				painter->addSubPainter(createToolPainter());
			}
		}
	);
}

void DynamicTool::_activateImpl() { }

bool DynamicTool::isToolActive() const {
	return _toolGroup ? (_toolGroup->getActiveTool() == this) : false;
}

void DynamicTool::setToolGroup(ToolGroup* toolGroup) {
	_toolGroup = toolGroup;
}

}