#include "lucca/tool/tool_descriptor.hpp"

namespace lucca {

ToolDescriptor::~ToolDescriptor() = default;

std::string ToolDescriptor::getName() const {
	return "Unnamed Tool";
}

std::string ToolDescriptor::getIcon() const {
	return "";
}

std::string ToolDescriptor::getBigIcon() const {
	return "";
}

std::string ToolDescriptor::getToolTip() const {
	return "Unnamed Tool. Missing tool descriptor.";
}

std::string ToolDescriptor::getCursor() const {
	return "";
}

std::string ToolDescriptor::getShortcut() const {
	return "";
}

}