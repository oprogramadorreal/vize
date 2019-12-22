#include "vize/tool/model_transformer_switch_tool.hpp"
#include "vize/ui/model_transformer_widget.hpp"

namespace vize {

ModelTransformerSwitchTool::ModelTransformerSwitchTool(ModelTransformerWidget& widget)
	: _widget(widget)
{ }

ModelTransformerSwitchTool::~ModelTransformerSwitchTool() = default;

bool ModelTransformerSwitchTool::isToolActive() const {
	return _widget.isVisible();
}

void ModelTransformerSwitchTool::_activateImpl() {
	_widget.setVisible(true);
}

void ModelTransformerSwitchTool::_deactivateImpl() {
	_widget.setVisible(false);
}

}