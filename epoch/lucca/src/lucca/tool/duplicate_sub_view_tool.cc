#include "lucca/tool/duplicate_sub_view_tool.hpp"

#include "lucca/view/view_composite.hpp"

namespace lucca {

DuplicateSubViewTool::DuplicateSubViewTool(View& subView)
	: _subView(subView)
{ }

DuplicateSubViewTool::~DuplicateSubViewTool() = default;

void DuplicateSubViewTool::_activateImpl() {
	if (auto owner = _subView.getComposite()) {
		owner->addSubView(_subView.clone());
	}
}

}