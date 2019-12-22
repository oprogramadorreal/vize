#include "lucca/tool/tool_impl.hpp"

namespace lucca {

ToolImpl::ToolImpl() = default;
ToolImpl::~ToolImpl() = default;

void ToolImpl::activateTool() {
	_activated(*this);
	_activateImpl();
}

void ToolImpl::deactivateTool() {
	// TODO: deactivation signal
	_deactivateImpl();
}

void ToolImpl::_deactivateImpl() { }

boost::signals2::connection ToolImpl::connectToActivatedSignal(ActivatedSignalListener listener) {
	return _activated.connect(listener);
}

bool ToolImpl::isToolActive() const {
	return false;
}

void ToolImpl::setToolGroup(ToolGroup*) { }

}